#include<iostream>
#include<string>
#include<fstream>

#include "database.h"
#include "table.h"
#include "sqlstatement.h"

using namespace std;


/**
*  \brief 构造函数
*/
Table::Table(std::string new_path)
{
	path = new_path;			/* 获取表单存储路径 */
}

/**
*  \brief 析构函数，更新主键内存池
*/
Table::~Table()
{
	fstream file_stream_;
	std::string pool_file = path + "\\" + table_name + "\\" + table_name + "_idPool";
	file_stream_.open(pool_file.c_str(), ios::out | ios::binary);
	file_stream_.write((char*)(&idPool), sizeof(idPool));
	file_stream_.close();
}

/**
*  \brief 重置表名
*/
void Table::SetTableName(std::string new_name)
{
	table_name = new_name;
}

/**
*  \brief 打开目标表单，成功返回true，失败返回false
*/
bool Table::UseTable()
{
	std::string table_name_fields = path + "\\" + table_name + "\\" + table_name + "_fields";/* 构建命令打开表头文件 */
	fstream fp_fields;
	fp_fields.open(table_name_fields.c_str(), std::ios::in);
	if (!fp_fields.is_open())										/* 如果打开失败，则返回false */
	{
		std::cout << table_name_fields << endl;
		return false;
	}

	char type[2], field_name[20],records_numb[4];
	fp_fields.read(records_numb, sizeof(char)* 4);				/* 读取当前记录数量 */
	records_num = atoi(records_numb);

	if (fields.size() == 0)										/* 当表单不是第一次读入时 */
	{
		while (fp_fields.read(type, sizeof(char)* 2))				/* 读取字段对应的数据类型进入内存 */
		{
			Field temp;
			if (type[0] == '0') temp.SetFieldType(kIntegerType);	/* 0-整型，1-字符串 */
			else temp.SetFieldType(kStringType);
			fp_fields.read(field_name, sizeof(char)* 20);			/* 读取字段名称进入内存 */
			temp.SetFieldName(field_name);
			fields.push_back(temp);
		}
		fp_fields.close();											/* 关闭文件 */

		fstream file_stream_;										/* 读入主键内存池 */
		std::string pool_file = path + "\\" + table_name + "\\" + table_name + "_idPool";
		file_stream_.open(pool_file, ios::out | ios::binary);
		file_stream_.read((char*)(&idPool), sizeof(idPool));
		file_stream_.close();
	}
	return true;
}

/**
*  \brief 找到字段对应的索引编号
*/
int Table::FindIndex(std::string index_field_name)
{
	for (int i = 0; i < indexs.size(); i++)
	{
		if (index_field_name == indexs[i].GetFieldName())
		{
			return i;
		}
	}
	return -1;
}

/**
*  \brief 获取表单名称
*/
std::string Table::GetTableName()
{
	return table_name;
}

/**
*  \brief 获取表头信息
*/
std::vector<Field> Table::GetTableFields()
{
	return fields;
}

/**
*  \brief 创建新表单
*/
bool Table::CreateTable(SQLCreateTable &sql)
{
	table_name = sql.GetTableName();	/* 获得表单名称 */
	if (table_name.size() >= 20)		/* 若表单名称过长则失败 */
	{
		std::cout << "表单名称过长！" << endl;
		return false;
	}

	std::string cmd = "md " + path + "\\" + table_name;/* 构建命令创建表单文件夹 */
	if (Table::UseTable())							/* 判断表单是否已经存在 */
	{
		std::cout << "已存在该表单！" << endl;
		return false;
	}
	else {
		if (!system(cmd.c_str()))
		{
			fields = sql.GetFields();				/* 获取表头数据 */

			fstream fp;
			std::string table_name_fields = path + "\\" + table_name + "\\" + table_name + "_fields";/* 构建表头文件名table_name_fields */
			fp.open(table_name_fields.c_str(), std::ios::out);		/* 创建表头文件 */

			for (int i = 0; i < fields.size(); i++)						/* 判断是否有重名的字段 */
			{
				for (int j = 0; j < i; j++)
				{
					if (fields[i].GetFieldName() == fields[j].GetFieldName())
					{
						std::cout << "存在重名字段！" << endl;
						return false;									/* 若存在重名字段，则返回false */
					}
				}
			}

			char records_num[4];										/* 前4位写入记录数量 */
			itoa(0, records_num, 10);
			fp.write(records_num, 4);
			
			for (int i = 0; i < fields.size(); i++)						/* 写入表头数据 */
			{
				std::string name = fields[i].GetFieldName() + '\0';
				ValueType type = fields[i].GetFieldType();				/* 获取字段对应的数据类型 */
				std::string type_;										/* type_标记数据类型 */
				if (type == kIntegerType) type_ = "0\0";				/* 标号0：整形，标号1：字符串 */
				else type_ = "1\0";
				fp.write(type_.c_str(), 2);								/* 数据类型占2位，字段名20位 */
				fp.write(name.c_str(), 20);
			}
			fp.close();													/* 关闭文件 */

			fstream file_stream_;										/* 创建主键存储池的物理文件 */
			std::string pool_file = path + "\\" + table_name + "\\" + table_name + "_idPool";
			file_stream_.open(pool_file, ios::out | ios::binary);
			file_stream_.close();

			std::cout << "创建成功！" << endl;
			return true;
		}
		else {
			std::cout << "创建命令失败！" << endl;
			return false;
		}
	}
}

/**
*  \brief 通过SQL直接查找记录
*/
bool Table::SelectRecord(SQLSelect &sql)
{
	table_name = sql.GetTableName();
	/*int field_id = Table::FindIndex(sql.GetField());
	int id = indexs.at(field_id).SearchNode(sql.GetValue().GetValueData());
	select_id.push_back(id);*/
	Table::Display();
	return false;
}

/**
*  \brief Delete调用Select功能
*/
bool Table::SelectRecord(SQLDelete &sd)
{
	table_name = sd.GetTableName();
	int field_id = Table::FindIndex(sd.GetField());
	return false;
}

/**
*  \brief Update调用Select功能
*/
bool Table::SelectRecord(SQLUpdate &su)
{
	table_name = su.GetTableName();
	int field_id = Table::FindIndex(su.GetWhereField());
	return false;
}

/**
*  \brief 增加新记录
*/
bool Table::CreateRecord(SQLInsert &st)
{
	table_name = st.GetTableName();							/* 获取表单名称 */

	if (Table::UseTable())									/* 打开目标表单文件 */
	{
		if (records_num >= record_max - 1) return false;	/* 判断当前表单是否被填满 */
		std::vector<string> records__data;					/* records__data存储要插入的数据 */
		for (int i = 0; i < fields.size(); i++)				/* 初始值置空 */
		{
			std::string temp = "";
			records__data.push_back(temp);
		}

		/* ---------------------------------------------匹配字段与值----------------------------------------------------- */
		if (st.IsInputField())
		{/* 当插入的value指定了相应字段时 */
			for (int i = 0; i < st.GetValues().size(); i++)						/* 判断字符串长度 */
			{
				if (st.GetValues().at(i).GetValueData().size() >= record_len)			/* 若字符串过长，则存储失败 */
				{
					std::cout << "输入的值过长！" << endl;
					return false;
				}
			}

			int key;										/* key判断插入是否成功 */
			for (int i = 0; i < st.GetFields().size(); i++) /* 遍历匹配制定字段与表头字段 */
			{
				key = 0;
				for (int j = 0; j < fields.size(); j++)
				{
					if (st.GetFields().at(i) == fields[j].GetFieldName())	/* 匹配到同名字段key = 1 */
					{
						key = 1;
						if (st.GetValues().at(i).GetValueType() == kNullType
							|| st.GetValues().at(i).GetValueType() == fields[j].GetFieldType())/* 若数据类型也匹配，存入记录 */
						{
							records__data.at(j) = st.GetValues().at(i).GetValueData();
						}
						else {
							std::cout << "数据类型不匹配！" << endl;
							return false;									/* 同名字段数据类型不匹配，则插入记录失败 */
						}
					}
				}
				if (key == 0)
				{
					std::cout << "字段名匹配失败！" << endl;
					return false;					/* 当前字段名未匹配到同名表头字段，插入失败 */
				}
			}
		}
		else {/* 当插入的value未指定相应字段时 */
			if (st.GetValues().size() > fields.size()) return false;		/* 若插入value数目大于字段数，则插入失败 */
			else {
				for (int i = 0; i < st.GetValues().size(); i++)						/* 判断字符串长度 */
				{
					if (st.GetValues().at(i).GetValueData().size() >= record_len)			/* 若字符串过长，则存储失败 */
					{
						std::cout << "输入的值过长！" << endl;
						return false;
					}
				}

				for (int i = 0; i < st.GetValues().size(); i++)				/* 按默认顺序插入value */
				{
					if (st.GetValues().at(i).GetValueType() == kNullType
						|| st.GetValues().at(i).GetValueType() == fields[i].GetFieldType())/* 若数据类型匹配 */
					{
						records__data.at(i) = st.GetValues().at(i).GetValueData();
					}
					else  {
						std::cout << "数据类型不匹配！" << endl;
						return false;										/* 同名字段数据类型不匹配，则插入记录失败 */
					}
				}
			}
		}

		/* -----------------------------------------匹配成功后，进行文件存储------------------------------------------------- */
		int Record_id = idPool.NewNode();					/* 获得新记录的主键 */
		char records_no_[2];
		itoa(Record_id / record_num, records_no_, 10);
		std::string table_name_records = path + "\\" + table_name + "\\" + table_name + "_records_" + records_no_;/* 构建表单记录文件名 */
		std::string table_name_fields  = path + "\\" + table_name + "\\" + table_name + "_fields";/* 构建表头文件名 */

		fstream fp;
		fp.open(table_name_records.c_str(),std::ios::app);/* 打开or创建记录文件 */
		/*long l = fp.tellp();
		cout << l << endl;*/
		fp.seekp((Record_id%record_num) * fields.size() * record_len, ios::beg); /* 指针定位，更改写入位置 */
		/*l = fp.tellp();
		cout << l << endl;*/

		for (int i = 0; i < fields.size(); i++)
		{
			fp.write(records__data[i].c_str(), record_len);		/* 按字段顺序写入文件 */
			if (fields[i].IsCreateIndex())					/* 当这个字段存在索引时，维护索引 */
			{
				int index_id = FindIndex(fields[i].GetFieldName());/* 找到该字段index_id（索引对应编号） */
				if (index_id != -1)
				{
					indexs[index_id].InsertNode(records__data[i].c_str(), Record_id);/* 插入索引结点 */
				}
			}
		}
		fp.close();											/* 关闭写记录文件 */

		fp.open(table_name_fields.c_str());
		records_num++;										/* 插入成功，表单中记录总数加一 */
		char records_numb[4];
		itoa(records_num, records_numb, 10);				/* 将新的记录数据条数更新 */
		fp.seekp(0, ios::beg);
		fp.write(records_numb, sizeof(char)* 4);
		fp.close();											/* 关闭写表头文件 */
		std::cout << "插入成功！" << endl;
		return true;										/* 返回成功 */
	}
	else {
		std::cout << "表单打开失败！" << endl;
		return false;										/* 打开目标表单失败，返回false */
	}
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
			std::cout << "搜索失败！" << endl;
			return false;					/* 搜索目标记录失败，返回false */
		}
		else {
			int Record_id;					/* 搜索目标记录成功，Record_id记录当前要删除的记录主键 */
			std::string Null_str = "";		/* 删除记录即为将记录置空 */
			for (int i = 0; i < select_id.size(); i++)
			{/* 从要删除主键池中按序取出主键，删除对应记录 */
				/* 在delete的SQL类中，有IsInputWhere()，true为部分删除，false为全表删除。若全表删除，select方法应把所有的主键id放入主键池 */
				Record_id = select_id[i];
				char records_no[1], record__data[record_len];
				std::vector<string> records__data;
				itoa(Record_id / record_num, records_no, 10);
				std::string record_file = path + "\\" + table_name + "\\" + table_name + "_records_" + records_no;/* 构建目标文件名 */
				std::string table_name_fields = path + "\\" + table_name + "\\" + table_name + "_fields";/* 构建表头文件名 */

				fstream fp;
				fp.open(record_file,  std::ios::in);								/* 打开读文件 */
				fp.seekg((Record_id%record_num)*fields.size() * record_len, ios::beg);/* 指针定位 */
				for (int j = 0; j < fields.size(); j++)								/* 将原记录读入records__data */
				{
					fp.read(record__data, sizeof(char)* record_len);
					std::string r;
					records__data.push_back(r);
					records__data[j] = record__data;
				}
				fp.close();															/* 关闭读文件 */

				fp.open(record_file);												/* 打开目标文件 */
				fp.seekp((Record_id%record_num)*fields.size() * record_len, ios::beg);     /* 定位到要更改的位置 */
				for (int j = 0; j < fields.size(); j++)								/* 修改字段中的每个值 */
				{
					fp.write(Null_str.c_str(), record_len);
					if (fields[j].IsCreateIndex())									/* 当对应字段存在索引时，对索引进行维护 */
					{
						int index_id = FindIndex(fields[j].GetFieldName());			/* 找到该字段对应的索引编号index_id */
						if (index_id == -1)
						{
							std::cout << "该字段不存在索引！" << endl;
							return false;
						}
						indexs[index_id].DeleteNode(records__data[j]);				/* 删除对应结点 */
					}
				}
				fp.close();															/* 关闭写文件 */

				fp.open(table_name_fields.c_str());
				records_num--;										/* 插入成功，表单中记录总数减一 */
				char records_numb[4];
				itoa(records_num, records_numb, 10);				/* 将新的记录数据更新 */
				fp.seekp(0, ios::beg);
				fp.write(records_numb, sizeof(char)* 4);
				fp.close();
			}
			std::cout << "删除成功！" << endl;
			return true;					/* 修改成功，返回true */
		}
	}
	else {
		std::cout << "表单打开失败！" << endl;
		return false;						/* 表单打开失败，返回false */
	}
}

/**
*  \brief 更新/更改记录
*/
bool Table::UpdateRecord(SQLUpdate &su)
{
	table_name = su.GetTableName();								/* 获取表单名称 */
	if (UseTable())												/* 打开表单文件夹 */
	{
		if (!Table::SelectRecord(su))							/* 搜索需要更新的记录 */
		{
			std::cout << "搜索失败！" << endl;
			return false;
		}
		else {
			/* ---------------------------------------------判断字长和字段数----------------------------------------------------- */
			if (fields.size() < su.GetNewField().size())		/* 判断更新格式是否匹配 */
			{
				std::cout << "输入字段数过多！" << endl;
				return false;
			}

			for (int i = 0; i < su.GetNewValue().size(); i++)	/* 判断更新字长是否匹配 */
			{
				if (su.GetNewValue().at(i).GetValueData().size() >= 255)
				{
					std::cout << "输入的长度过大！" << endl;
					return false;								/* 字符串过长，更新失败 */
				}
			}

			fstream fwp, frp;
			/* ---------------------------------------------匹配字段与值----------------------------------------------------- */
			int Record_id;										/* Record_id记录当前操作记录主键 */
			for (int i = 0; i < select_id.size(); i++)
			{
				Record_id = select_id[i];
				char records_no[1], record__data[record_len];			/* record__data记录一个字段的信息 */
				std::vector<string> records__data1;				/* records__data1记录原有记录信息 */
				std::vector<string> records__data2;				/* records__data2记录当前需要更改的记录信息 */

				itoa(Record_id / record_num, records_no, 10);
				std::string record_file = path + "\\" + table_name + "\\" + table_name + "_records_" + records_no;/* 构建记录文件名 */
				frp.open(record_file.c_str(),  std::ios::in); /* 打开读记录文件 */
				frp.seekg((Record_id%record_num )*fields.size() * record_len, ios::beg);/* 定位读文件指针 */
				for (int j = 0; j < fields.size(); j++)							/* 读入要更改的记录信息 */
				{
					frp.read(record__data, sizeof(char) * record_len);
					std::string *r1 = new string();
					std::string *r2 = new string();
					records__data1.push_back(*r1);
					records__data2.push_back(*r2);
					records__data1[j] = record__data;
				}
				frp.close();													/* 关闭读文件 */


				for (int j = 0; j < su.GetNewField().size(); j++)
				{
					bool key = false;
					for (int k = 0; k < fields.size(); k++)
					{
						if (su.GetNewField().at(j) == fields[k].GetFieldName())/* 判断更新记录的字段名是否可匹配 */
						{
							key = true;
							if (su.GetNewValue().at(j).GetValueType() == fields[k].GetFieldType()/* 判断数据类型是否匹配 */
								|| su.GetNewValue().at(j).GetValueType() == kNullType)
							{
								records__data2[k] = su.GetNewValue().at(j).GetValueData();
							}
							else {
								std::cout << "数据类型不匹配！" << endl;
								return false;					/* 数据类型不匹配，返回false */
							}
						}
					}
					if (key == false)							/* 若字段名无法匹配，则返回false */
					{
						std::cout << "存在无法匹配的字段名！" << endl;
						return false;
					}
				}
				/* -----------------------------------------匹配成功后，进行文件存储------------------------------------------------- */
				fwp.open(record_file.c_str());/* 打开写记录文件 */
				fwp.seekp((Record_id%record_num)*fields.size() * record_len, ios::beg);/* 指针定位 */
				for (int j = 0; j < fields.size(); j++)
				{
					fwp.write(records__data2[j].c_str(), record_len);	/* 按照字段序列进行更改 */
					if (fields[j].IsCreateIndex())				/* 当该字段存在索引时，对索引进行维护 */
					{
						int index_id = FindIndex(fields[j].GetFieldName());/* 找到字段对应索引的编号index_id */
						if (index_id == -1)
						{
							std::cout << "该字段不存在索引！" << endl;
							return false;
						}
						//indexs[index_id].UpdateNode(records__data2[j], records__data1[j]);/* 更新索引结点 */
					}
				}
				fwp.close();									/* 关闭写文件 */
			}
			std::cout << "更新成功！" << endl;
			return true;										/* 更新成功 */
		}
	}
	else {
		std::cout << "表单打开失败！" << endl;
		return false;											/* 打开表单文件失败，返回false */
	}
}

/**
*  \brief 创建索引
*/
bool Table::CreateIndex(SQLCreateIndex &si)
{
	if (!UseTable())
	{
		return false;
	}

	ValueType type;
	bool flag = false;
	for (auto iter = fields.begin(); iter != fields.end(); iter++)
	{
		if (iter->GetFieldName() == si.GetField())
		{
			iter->SetIsCreateIndex(true);
			type = iter->GetFieldType();
			flag = true;
			break;
		}
	}

	if (flag == false)
	{
		std::cout << "字段" << si.GetField() << "不存在." << std::endl;
		return false;
	}

	string index_path = path + "\\" + table_name + "\\";
	Index temp(si.GetIndex(), si.GetField(), type, index_path);
	string cmd = "md " + index_path + "index";
	system(cmd.c_str());

	indexs.push_back(temp);

	return true;
}

/**
*  \brief 表格显示
*/
bool Table::Display()
{
	std::string table_record = path + "\\" + table_name + "\\" + table_name + "_records_";
	if (Table::UseTable())
	{
		fstream frp;
		for (int i = 0; i < records_num / record_num; i++)
		{
			char records_no[2];
			itoa(i, records_no, 10);
			std::string table_name_n = table_record + records_no;
			frp.open(table_name_n.c_str(), std::ios::in);
			for (int k = 0; k < record_num; k++)
			{
				for (int j = 0; j < fields.size(); j++)
				{
					char record_data[record_len];
					frp.read(record_data, sizeof(char)* record_len);
					std::cout << fields.at(j).GetFieldName() << ":" << record_data << "   ";
				}
				std::cout << ";" << endl;
			}
			frp.close();
		}

		char records_no[2];
		itoa(records_num / record_num, records_no, 10);
		std::string table_name_n = table_record + records_no;
		frp.open(table_name_n.c_str(), std::ios::in);
		for (int k = 0; k < records_num%record_num; k++)
		{
		
			std::cout << k+1 << endl;
			for (int j = 0; j < fields.size(); j++) 
			{
				char record_data[record_len];
				record_data[0] = '\0';
				frp.read(record_data, sizeof(char)* record_len);
				//frp >> record_data;
				std::cout << fields.at(j).GetFieldName() << ":" << record_data << "  " <<endl;
			}
			
		}
		frp.close();
		return true;
	}
	else {
		std::cout << "打开表单失败！" << endl;
		return false;
	}
}