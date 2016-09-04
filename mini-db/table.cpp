#include<iostream>
#include<string>
#include<fstream>

#include "database.h"
#include "table.h"
#include "sqlstatement.h"
#include "MemPool.h"
using namespace std;


/**
*  \brief 构造函数
*/
Table::Table(std::string new_path) 
{
	path = new_path;			/* 获取表单存储路径 */
}

/**
*  \brief 析构函数
*/
Table::~Table() 
{

}

/**
*  \brief 打开目标表单，成功返回true，失败返回false
*/
bool Table::UseTable()
{
	std::string table_name_fields = path + table_name + "_fields";	/* 构建命令打开表头文件 */
	fstream fp_fields;
	fp_fields.open(table_name_fields.c_str(), std::ios::binary | std::ios::in);
	if (fp_fields.bad())										/* 如果打开失败，则返回false */
	{
		return false;
	}

	char type[2],field_name[20];
	int i = 0;
	while (fp_fields.read(type, sizeof(char)* 2))				/* 读取字段对应的数据类型进入内存 */
	{
		if (type[0] == '0') fields[i].SetFieldType(kIntegerType);/* 0-整型，1-字符串 */
		else fields[i].SetFieldType(kStringType);
		fp_fields.read(field_name, sizeof(char)* 20);			/* 读取字段名称进入内存 */
		fields[i].SetFieldName(field_name);
	}
	fp_fields.close();											/* 关闭文件 */
	return true;
}

/**
*  \brief 获取表单名称
*/
std::string Table::GetTableName()
{
	return table_name;
}

/**
*  \brief 创建新表单
*/
bool Table::CreateTable(SQLCreateTable &sql)
{
	table_name = sql.GetTableName();	/* 获得表单名称 */
	if (table_name.size() >= 20)		/* 若表单名称过长则失败 */
	{
		return false;
	}

	std::string cmd = "md " + path + table_name;	/* 构建命令创建表单文件夹 */
	if (Table::UseTable())							/* 判断表单是否已经存在 */
	{
		return false;
	}
	else {
		if (!system(cmd.c_str()))
		{
			fields = sql.GetFields();				/* 获取表头数据 */
			
			fstream fp;
			std::string table_name_fields = path + table_name + "\\" + table_name + "_fields";/* 构建表头文件名table_name_fields */
			fp.open(table_name_fields.c_str(), std::ios::binary | std::ios::out);		/* 创建表头文件 */

			for (int i = 0; i < fields.size(); i++)						/* 判断是否有重名的字段 */
			{
				for (int j = 0; j < i; j++)
				{
					if (fields[i].GetFieldName() == fields[j].GetFieldName())
					{
						return false;									/* 若存在重名字段，则返回false */
					}
				}
			}

			for (int i = 0; i < fields.size(); i++)						/* 写入表头数据 */
				{
				std::string name = fields[i].GetFieldName() + '\0';
				ValueType type = fields[i].GetFieldType();				/* 获取字段对应的数据类型 */
				std::string type_;										/* type_标记数据类型 */
				if (type == kIntegerType) type_ = "0\0";				/* 标号0：整形，标号1：字符串 */
				else type_ = "1\0";
				fp.write(type_.c_str(), 2);								/* 数据类型占2位，字段名20位 */
				fp.write(name.c_str(),20);
			}
			fp.close();													/* 关闭文件 */
			return true;
		}
		else return false;
	}
}

/**
*  \brief 增加新记录
*/
bool Table::CreateRecord(SQLInsert &st)
{
	table_name = st.GetTableName();							/* 获取表单名称 */
	if (records_num >= record_num - 1) return false;		/* 判断当前表单是否被填满 */

	if (Table::UseTable())									/* 打开目标表单文件 */
	{
		int Record_id = MemPool::NewNode();					/* 获得新记录的主键 */
		char records_no_[1];
		itoa(Record_id / record_num, records_no_, 1);
		std::string table_name_records = path + st.GetTableName() + "\\" + st.GetTableName() + "_records_" + records_no_;/* 构建表单记录文件名 */

		fstream fp;
		fp.open(table_name_records.c_str(), std::ios::binary | std::ios::out);/* 打开or创建记录文件 */

		if (st.IsInputField())								
		{/* 当插入的value指定了相应字段时 */
			int key;										/* key判断插入是否成功 */
			for (int i = 0; i < st.GetFields().size(); i++) /* 遍历匹配制定字段与表头字段 */
			{
				key = 0;
				if (st.GetValues().at(i).GetValueData().size() >= 255)			/* 若字符串过长，则存储失败 */
				{
					return false;
				}

				for (int j = 0; j < fields.size(); j++)
				{
					if (st.GetFields().at(i) == fields[j].GetFieldName())	/* 匹配到同名字段key = 1 */
					{
						key = 1;
						if (st.GetFields().at(i) == kNullType || st.GetValues().at(i).GetValueType() == fields[j].GetFieldType())/* 若数据类型也匹配，则在指定位置写入文件 */
						{
							fp.seekp(((Record_id%record_num - 1) * fields.size() + j) * 255, ios::beg);
							fp.write(st.GetValues().at(i).GetValueData().c_str(), 255);
						}
						else return false;									/* 同名字段数据类型不匹配，则插入记录失败 */
					}
				}
				if (key == 0) return false;					/* 当前字段名未匹配到同名表头字段，插入失败 */
			}
		}
		else {/* 当插入的value未指定相应字段时 */
			if (st.GetValues().size() > fields.size()) return false;		/* 若插入value数目大于字段数，则插入失败 */
			else {
				for (int i = 0; i < st.GetValues().size(); i++)				/* 按默认顺序插入value */
				{
					if (st.GetValues().at(i).GetValueData().size() >= 255)	/* 若字符串过长，则存储失败 */
					{
						return false;
					}
					
					if (st.GetFields().at(i) == kNullType || st.GetValues().at(i).GetValueType() == fields[i].GetFieldType())/* 若数据类型匹配 */
					{
						fp.seekp(((Record_id%record_num - 1) * fields.size() + i) * 255, ios::beg);/* 在指定位置写入记录数据 */
						fp.write(st.GetValues().at(i).GetValueData().c_str(), 255);
					}
					else return false;										/* 数据类型不匹配时，插入失败 */
				}
			}
		}
		records_num++;										/* 插入成功，表单中记录总数加一 */
		return true;										/* 返回成功 */
	}
	else return false;										/* 打开目标表单失败，返回false */
}

/**
*  \brief 删除记录
*/
bool Table::DeleteRecord(SQLDelete &sd)
{
	table_name = sd.GetTableName();			/* 获取表单名称 */
	if (UseTable())												
	{/* 打开表单成功 */
		if (!Table::SelectRecord(sd))
		{
			return false;					/* 搜索目标记录失败，返回false */
		}
		else {
			int Record_id;					/* 搜索目标记录成功，Record_id记录当前要删除的记录主键 */
			std::string Null_str = "";		/* 删除记录即为将记录置空 */
			for (int i = 0; i < select_id.size(); i++)
			{/* 从要删除主键池中按序取出主键，删除对应记录 */
			 /* 在delete的SQL类中，有IsInputWhere()，true为部分删除，false为全表删除。若全表删除，select方法应把所有的主键id放入主键池 */
				Record_id = select_id[i];
				char records_no[1];
				itoa(Record_id / record_num, records_no, 1);						
				std::string record_file = path + table_name + "\\" + table_name + "_records_" + records_no;/* 构建目标文件名 */

				fstream fp;
				fp.open(record_file, std::ios::binary | std::ios::out);				/* 打开目标文件 */
				fp.seekp((Record_id%record_num - 1)*fields.size() * 255, ios::beg); /* 定位到要更改的位置 */
				for (int j = 0; j < fields.size(); j++)								/* 修改字段中的每个值 */
				{
					fp.write(Null_str.c_str(), 255);
				}
			}
			records_num--;
			return true;					/* 修改成功，返回true */
		}
	}
	else return false;						/* 表单打开失败，返回false */
}

/**
*  \brief 更新/更改记录
*/
bool Table::UpdateRecord(SQLUpdate &su)
{
	table_name = su.GetTableName();
	if (UseTable())
	{
		if (!Table::SelectRecord(su))
		{
			return false;
		}
		else {
			if (fields.size() != su.GetNewField().size())
			{
				return false;
			}

			for (int i = 0; i < su.GetNewValue().size(); i++)
			{
				if (su.GetNewValue().at(i).size() >= 255)
				{
					return false;
				}
			}

			fstream fp;


			int Record_id;
			for (int i = 0; i < select_id.size(); i++)
			{
				Record_id = select_id[i];
				for (int j = 0; j < su.GetNewField().size(); j++)
				{
					bool key = false;
					for (int k = 0; k < fields.size(); k++)
					{
						if (su.GetNewField().at(j) == fields[k].GetFieldName())
						{
							key = true;
							if (su.GetNewValue().at(j).GetValueType() == fields[k].GetFieldType()
								|| su.GetNewValue().at(j).GetValueType() == kNullType)
							{

							}
						}
					}
				}
			}
		}
	}
	else{
		return false;
	}
}