#include<iostream>
#include<string>
#include<fstream>

#include "database.h"
#include "table.h"
#include "sqltype.h"

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
	ofstream file_stream_,fp;
	std::string pool_file = path + "\\" + table_name + "\\" + table_name + "_idPool";
	file_stream_.open(pool_file.c_str(), ios::in |ios::out| ios::binary);
	file_stream_.seekp(0, ios::beg);
	file_stream_.write((char*)(idPool), sizeof(*idPool));
	file_stream_.close();

	std::string table_name_fields = path + "\\" + table_name + "\\" + table_name + "_fields";/* 构建表头文件名 */
	fp.open(table_name_fields,ios::binary|ios::in);
	char records_numb[10];
	itoa(records_num, records_numb, 10);				/* 将新的记录数据条数更新 */
	fp.seekp(0, ios::beg);
	fp.write(records_numb, sizeof(char)* 11);
    fp.flush();
	fp.close();											/* 关闭写表头文件 */

	fwp.close();										/* 关闭写文件 */
	frp.close();										/* 关闭写文件 */


	ofstream findex;
	std::string sign;
	std::string index_tree = path + "\\" + table_name + "\\index\\index_tree";
	findex.open(index_tree, ios::binary);

	for (auto iter : indexs)
	{
		if (iter != nullptr)
		{
			if (iter->bplustree_int_ != nullptr)
			{
				iter->bplustree_int_->CloseTree();
				sign = "0\0";
				findex.write(sign.c_str(), sizeof(char)* 2);
				findex.write((char*)(iter->bplustree_int_), sizeof(*(iter->bplustree_int_)));
			}
			else if (iter->bplustree_string_ != nullptr)
			{
				iter->bplustree_string_->CloseTree();
				sign = "1\0";
				findex.write(sign.c_str(), sizeof(char)* 2);
				findex.write((char*)(iter->bplustree_string_), sizeof(*(iter->bplustree_string_)));
			}
			delete iter;
			iter = nullptr;
		}
	}
	findex.close();
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
	if (fields.size() != 0)										/* 若表单已经打开，返回true */
	{
		return true;
	}

	std::string table_name_records = path + "\\" + table_name + "\\" + table_name + "_records";/* 构建表单记录文件名 */
	fwp.open(table_name_records, ios::in | ios::binary);
	frp.open(table_name_records, ios::binary);

	ifstream fp_fields;
	fp_fields.open(table_name_fields, ios::in | ios::binary);
	if (!fp_fields.is_open())									/* 如果打开失败，则返回false */
	{
		return false;
	}

	char is_index[2], type[2], field_name[20], records_numb[11], fields_numb[11];
	fp_fields.seekg(0, ios::beg);
	fp_fields.read(records_numb, sizeof(records_numb));				/* 读取当前记录数量 */
	records_num = atoi(records_numb);
	fp_fields.seekg(sizeof(char)* 16, ios::beg);
	fp_fields.read(fields_numb, sizeof(fields_numb));				/* 读取字段数 */
	USER_INT fields_num = atoi(fields_numb);
	USER_INT i = 0;
	record_leng = 0;

	Index * id_index = new Index("id_index", "id_index", kIntegerType);
	indexs.push_back(id_index);

	while (i <fields_num)										/* 读取字段对应的数据类型进入内存 */
	{
		Field temp;
		ValueType _type;
		fp_fields.seekg((32 + 24 * i)*sizeof(char), ios::beg);
		fp_fields.read(is_index, sizeof(char)* 2);
		fp_fields.read(type, sizeof(char)* 2);
		if (type[0] == '0') {
			_type = kIntegerType;
			temp.SetFieldType(kIntegerType);
			record_leng += true_int;
		}														/* 0-整型，1-字符串 */
		else
		{
			_type = kStringType;
			temp.SetFieldType(kStringType);
			record_leng += true_len;
		}
		fp_fields.read(field_name, sizeof(char)* 20);			/* 读取字段名称进入内存 */
		temp.SetFieldName(field_name);
		fields.push_back(temp);

		if (is_index[0] == '1')
		{
			Index * index = new Index("", field_name, _type);
			indexs.push_back(index);
		}
		i++;
	}

	fp_fields.close();											/* 关闭文件 */

	ifstream findex;
	std::string index_tree = path + "\\" + table_name + "\\index\\index_tree";
	char sign[2];
	findex.open(index_tree,ios::binary);
	int index_no = 0;
	for (auto iter : indexs)
	{
		findex.read(sign, sizeof(char)* 2);
		if (sign[0] == '0')
		{
			findex.read((char*)(iter->bplustree_int_), sizeof(*(iter->bplustree_int_)));
		}
		else if (sign[0] == '1'){
			findex.read((char*)(iter->bplustree_string_), sizeof(*(iter->bplustree_string_)));
		}
	}
	findex.close();

    idPool = new IDPool();
	ifstream file_stream_;										/* 读入主键内存池 */
	std::string pool_file = path + "\\" + table_name + "\\" + table_name + "_idPool";
	file_stream_.open(pool_file, ios::in | ios::binary);
	file_stream_.seekg(0, ios::beg);
	file_stream_.read((char*)(idPool), sizeof(*idPool));
	file_stream_.close();
	return true;
}

/**
*  \brief 找到字段对应的索引编号
*/
int Table::FindIndex(std::string index_field_name)
{
	for (int i = 1; i < indexs.size(); i++)
	{
		if (index_field_name == indexs[i]->GetFieldName())
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

	fields = sql.GetFields();									/* 获取表头数据 */
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

	std::string cmd = "md " + path + "\\" + table_name;		/* 构建命令创建表单文件夹 */
	std::string table = path + "\\" + table_name;
	if (!_access(table.c_str(),0))							/* 判断表单是否已经存在 */
	{
		std::cout << "已存在该表单！" << endl;
		return false;
	}
	else {
		if (!system(cmd.c_str()))
		{
			records_num = 0;
			record_leng = 0;

			fstream fp;
			std::string table_name_fields = path + "\\" + table_name + "\\" + table_name + "_fields";/* 构建表头文件名table_name_fields */
			fp.open(table_name_fields, std::ios::out|ios::binary);		/* 创建表头文件 */
																		
			char records_numb[11], fields_numb[11];						/* 后16位写入字段数，前16位写入记录数量 */
			itoa(0, records_numb, 10);
			fp.seekp(0, ios::beg);
			fp.write(records_numb, sizeof(records_numb));
			itoa(fields.size(), fields_numb, 10);
			fp.seekp(16, ios::beg);
			fp.write(fields_numb, sizeof(fields_numb));
			fp.flush();

			std::string name;
			ValueType type;												/* 获取字段对应的数据类型 */
			std::string type_;											/* type_标记数据类型 */
			std::string is_index;										/* is_index标记是否存在索引 */
			for (int i = 0; i < fields.size(); i++)						/* 写入表头数据 */
			{
				name = fields[i].GetFieldName() + '\0';
				type = fields[i].GetFieldType();						/* 获取字段对应的数据类型 */
				is_index = "0\0";										/* is_index标记是否存在索引 */
				if (type == kIntegerType)
				{
					type_ = "0\0";										/* 标号0：整形，标号1：字符串 */
					record_leng += true_int;
				}
				else {
					type_ = "1\0";
					record_leng += true_len;
				}
				
				fp.seekp((32 + i * 24)*sizeof(char), ios::beg);
				fp.write(is_index.c_str(), 2);							/* 索引标记占2位 */
				fp.write(type_.c_str(), 2);								/* 数据类型占2位，字段名20位 */
				fp.write(name.c_str(), 20);
			}
			fp.close();													/* 关闭文件 */

      
			fstream file_stream_;										/* 创建主键存储池的物理文件 */
			std::string pool_file = path + "\\" + table_name + "\\" + table_name + "_idPool";
			idPool = new IDPool(pool_file);
			file_stream_.open(pool_file, ios::out | ios::binary);
			file_stream_.close();

			std::string table_name_records = path + "\\" + table_name + "\\" + table_name + "_records";/* 构建表单记录文件名 */
			fwp.open(table_name_records,ios::binary);								/* 创建记录文件 */
			fwp.close();

			fwp.open(table_name_records, ios::in|ios::binary);						/* 打开记录文件 */
			frp.open(table_name_records);

			string index_path = path + "\\" + table_name + "\\index";
			if (_access(index_path.c_str(), 0))
			{
				string cmd = "md " + index_path;
				system(cmd.c_str());
			}
			index_path = index_path + "\\id_index";
			Index * temp = new Index("id_index", "id_index", kIntegerType, index_path);
			indexs.push_back(temp);

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
	select_id.clear();
	if (sql.IsInputWhere())																	/* 若select存在where子句，显示选中记录 */
	{											
		int field_id = Table::FindIndex(sql.GetField());									/* 查找字段对应的索引id */
		if (field_id != -1)
		{
			for (int i = 0; i < fields.size(); i++)
			{
				if (fields.at(i).GetFieldName() == sql.GetField())
				{
					if (sql.GetValue().GetValueType() != kNullType 
						&& fields.at(i).GetFieldType() != sql.GetValue().GetValueType())
					{
						std::cout << "数据类型无法匹配！" << endl;
						return false;
					}
				}
			}

			if (indexs.at(field_id)->
				SearchNode(sql.GetValue().GetValueData(), 
					       select_id, sql.GetOperatorType()) == true)/* 依照索引查找 */
			{
				for (USER_INT iter = 0; iter < select_id.size();iter++)						/* 查找成功 */
				{
					Display(select_id[iter],iter);
				}
				std::cout << endl;
				return true;
			}
			else
			{
				std::cout << "不存在符合条件的记录！" << endl;	/* 查找失败 */
				return false;
			}
		}
		else {
			if (OrderSelect(sql.GetField(), sql.GetValue(), sql.GetOperatorType()))
			{
				for (USER_INT iter = 0; iter < select_id.size(); iter++)						/* 查找成功 */
				{
					Display(select_id[iter], iter);
				}
				std::cout << endl;
				return true;
			}
			else {
				return false;
			}
		}
	}
	else {/* select 不存在where子句，显示全表记录 */
		Table::Display();
		return true;
	}
}

/**
*  \brief Delete调用Select功能
*/
bool Table::SelectRecord(SQLDelete &sql)
{
	table_name = sql.GetTableName();
	select_id.clear();
	if (sql.IsInputWhere())								/* 若delete语句存在where子句 */
	{
		int field_id = Table::FindIndex(sql.GetField());/* 查找索引对应序号 */
		if (field_id != -1)								/* 存在索引，按索引查找 */
		{
			for (int i = 0; i < fields.size(); i++)
			{
				if (fields.at(i).GetFieldName() == sql.GetField())
				{
					if (fields.at(i).GetFieldType() != sql.GetValue().GetValueType())
					{
						std::cout << "数据类型不符合！" << endl;
						return false;
					}
				}
			}

			if (indexs.at(field_id)->
				SearchNode(sql.GetValue().GetValueData(),
					select_id, sql.GetOperatorType()) == true)
			{
				return true;
			}
			else
			{
				std::cout << "不存在符合条件的记录！" << endl;
				return false;
			}
		}
		else {											/* 不存在索引，顺序查找 */
			if (OrderSelect(sql.GetField(), sql.GetValue(), kOpEqual))
			{
				return true;
			}
			else {
				return false;
			}
		}

	}
	else {
		char record__data[record_len];
		USER_INT k = 0, i = 0;
		while (k < records_num)
		{
			bool key = false;
			for (int j = 0; j < fields.size(); j++)
			{
				record__data[0] = '\0';
				frp.seekg(sizeof(char)*true_len*(i*fields.size() + j), ios::beg);
				frp.read(record__data, sizeof(char)* record_len);
				//frp >> record__data;
				if (record__data[0] != '\a')
				{
					key = true;
					break;
				}
			}

			if (key == true)
			{
				select_id.push_back(i);
				k++;
			}
			i++;
		}
		return true;
	}
}

/**
*  \brief Update调用Select功能
*/
bool Table::SelectRecord(SQLUpdate &sql)
{
	table_name = sql.GetTableName();
	select_id.clear();
	
	int id;
	int field_id = Table::FindIndex(sql.GetWhereField());	/* 查找索引对应编号id */
	if (field_id != -1)
	{														/* 若存在索引，按索引查找 */
		for (int i = 0; i < fields.size(); i++)
		{
			if (fields.at(i).GetFieldName() == sql.GetWhereField())
			{
				if (fields.at(i).GetFieldType() != sql.GetWhereValue().GetValueType())
				{
					std::cout << "数据类型不符合！" << endl;
					return false;
				}
			}
		}

		if (indexs.at(field_id)->
			SearchNode(sql.GetWhereValue().GetValueData(),
				select_id, sql.GetOperatorType()) == true)
		{
			return true;
		}
		else
		{
			std::cout << "不存在符合条件的记录！" << endl;
			return false;
		}
	}
	else {													/* 不存在索引，顺序查找 */
		if (OrderSelect(sql.GetWhereField(), sql.GetWhereValue(), kOpEqual))
		{
			return true;
		}
		else {
			return false;
		}
	}
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
			records__data.push_back("\a");
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
		USER_INT Record_id = idPool->NewNode();					/* 获得新记录的主键 */
		USER_INT offset = 0;
		
		char id_char[20];
		itoa(Record_id, id_char, 10);
		indexs[0]->InsertNode(id_char, Record_id);

		for (int i = 0; i < fields.size(); i++)
		{
			fwp.seekp(Record_id*record_leng + offset, ios::beg); /* 指针定位，更改写入位置 */
			if (fields.at(i).GetFieldType() == kIntegerType)
			{
				fwp.write(records__data[i].c_str(), int_len);	/* 按字段顺序写入文件 */
				offset += true_int;
			}
			else {
				fwp.write(records__data[i].c_str(), record_len);/* 按字段顺序写入文件 */
				offset += true_len;
			}
			fwp.flush();

			if (fields[i].IsCreateIndex())						/* 当这个字段存在索引时，维护索引 */
			{
				int index_id = FindIndex(fields[i].GetFieldName());/* 找到该字段index_id（索引对应编号） */
				if (index_id != -1)
				{
					indexs[index_id]->InsertNode(records__data[i], Record_id);/* 插入索引结点 */
				}
			}
		}

		records_num++;										/* 插入成功，表单中记录总数加一 */
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
	char record__data[record_len];
	if (UseTable())
	{/* 打开表单成功 */
		if (!Table::SelectRecord(sd))
		{
			return false;					/* 搜索目标记录失败，返回false */
		}
		else {
			USER_INT Record_id;					/* 搜索目标记录成功，Record_id记录当前要删除的记录主键 */
			USER_INT offset;
			std::string Null_str = "\a";		/* 删除记录即为将记录置空 */
			for (USER_INT i = 0; i < select_id.size(); i++)
			{/* 从要删除主键池中按序取出主键，删除对应记录 */
				/* 在delete的SQL类中，有IsInputWhere()，true为部分删除，false为全表删除。若全表删除，select方法应把所有的主键id放入主键池 */
				Record_id = select_id[i];
				offset = 0;
				std::vector<string> records__data;

				for (int j = 0; j < fields.size(); j++)				/* 将原记录读入records__data */
				{
					frp.seekg(Record_id*record_leng + offset, ios::beg);/* 指针定位 */
					frp >> record__data;
					records__data.push_back(record__data);

					fwp.seekp(Record_id*record_leng + offset, ios::beg);/* 指针定位 */
					if (fields.at(j).GetFieldType() == kIntegerType)
					{
						fwp.write(Null_str.c_str(), int_len);
						offset += true_int;
					}
					else {
						fwp.write(Null_str.c_str(), record_len);
						offset += true_len;
					}
					fwp.flush();
				}
				
				for (int j = 0; j < fields.size(); j++)								/* 修改字段中的每个值 */
				{
					//fwp.seekp((Record_id*fields.size() + j) *true_len, ios::beg);/* 指针定位 */
					//fwp.write(Null_str.c_str(), record_len);
					//fwp.flush();
					if (fields[j].IsCreateIndex())									/* 当对应字段存在索引时，对索引进行维护 */
					{
						int index_id = FindIndex(fields[j].GetFieldName());			/* 找到该字段对应的索引编号index_id */
						if (index_id == -1)
						{
							std::cout << "索引匹配失败！" << endl;
							continue;
						}
						indexs[index_id]->DeleteNode(records__data[j]);				/* 删除对应结点 */
					}
				}
			
				records_num--;										/* 插入成功，表单中记录总数减一 */
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
	char record__data[record_len];
	if (UseTable())												/* 打开表单文件夹 */
	{
		if (!Table::SelectRecord(su))							/* 搜索需要更新的记录 */
		{
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

			/* ---------------------------------------------匹配字段与值----------------------------------------------------- */
			USER_INT Record_id;									/* Record_id记录当前操作记录主键 */
			USER_INT offset;
			//frp.sync();
			for (USER_INT i = 0; i < select_id.size(); i++)
			{
				Record_id = select_id[i];
				offset = 0;
				std::vector<string> records__data1;				/* records__data1记录原有记录信息 */
				std::vector<string> records__data2;				/* records__data2记录当前需要更改的记录信息 */

				for (int j = 0; j < fields.size(); j++)							/* 读入要更改的记录信息 */
				{
					std::string r1, r2;
					frp.seekg(Record_id*record_leng + offset, ios::beg);/* 定位读文件指针 */
					if (fields.at(j).GetFieldType() == kIntegerType)
					{
						frp.read(record__data, sizeof(char)*int_len);
						offset += true_int;
					}
					else {
						frp.read(record__data, sizeof(char)*record_len);
						offset += true_len;
					}
					records__data1.push_back(r1);
					records__data2.push_back(r2);
					records__data1[j] = record__data;
					records__data2[j] = record__data;
				}


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
								
								if (fields[k].IsCreateIndex())	/* 维护索引 */
								{
									int index_id = FindIndex(fields[j].GetFieldName());
									if (index_id != -1)
									{
										indexs[index_id]->UpdateNode(records__data2[index_id], records__data1[index_id]);
									}
								}
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
				offset = 0;
				for (int j = 0; j < fields.size(); j++)
				{
					fwp.seekp(Record_id*record_leng + offset, ios::beg); /* 指针定位，更改写入位置 */
					if (fields.at(j).GetFieldType() == kIntegerType)
					{
						fwp.write(records__data2[j].c_str(), int_len);	/* 按字段顺序写入文件 */
						offset += true_int;
					}
					else {
						fwp.write(records__data2[j].c_str(), record_len);/* 按字段顺序写入文件 */
						offset += true_len;
					}
					fwp.flush();
				}
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
	int i = 0;
	USER_INT offset = 0;
	for (auto iter = fields.begin(); iter != fields.end(); iter++)
	{
		if (iter->GetFieldName() == si.GetField())
		{
			iter->SetIsCreateIndex(true);
			type = iter->GetFieldType();
			flag = true;
			break;
		}

		if (fields.at(i).GetFieldType() == kIntegerType)
		{
			offset += true_int;
		}
		else {
			offset += true_len;
		}
		i++;
	}

	if (flag == false)
	{
		std::cout << "字段" << si.GetField() << "不存在." << std::endl;
		return false;
	}

	string index_path = path + "\\" + table_name + "\\index";
	if (_access(index_path.c_str(), 0))
	{
		string cmd = "md " + index_path;
		system(cmd.c_str());
	}
	index_path = index_path + "\\" + si.GetField();
	Index * temp = new Index(si.GetIndex(), si.GetField(), type, index_path);

	ofstream fip;
	std::string table_name_fields = path + "\\" + table_name + "\\" + table_name + "_fields";/* 构建表头文件名table_name_fields */
	std::string is_index = "1\0";							/* is_index标记是否存在索引 */
	fip.open(table_name_fields,ios::in|ios::binary);
	fip.seekp((32 + 24 * i)*sizeof(char), ios::beg);
	fip.write(is_index.c_str(), sizeof(char)* 2);
	fip.flush();
	fip.close();

	int k = 0;
	char record_field_data[record_len];
	if (records_num != 0)
	{
		while (k < records_num)
		{
			frp.seekg((k*record_leng + offset)*sizeof(char), ios::beg);
			frp >> record_field_data;
			temp->InsertNode(record_field_data, k);
			k++;
		}
	}
	indexs.push_back(temp);

	std::cout << "索引" << si.GetIndex() << "建立成功." << std::endl;
	return true;
}

/**
*  \brief 表格显示
*/
bool Table::Display()
{
	char record__data[record_len];
	std::vector<string> record__datas;
	if (Table::UseTable())
	{
		USER_INT k = 0, offset = 0, i = 0;
		//frp.sync();
		while(k < records_num)
		{
			record__datas.clear();
			bool key = false;
			offset = 0;
			for (int j = 0; j < fields.size(); j++) 
			{
				record__data[0] = '\0';
				frp.seekg(sizeof(char)*(i*record_leng+offset), ios::beg);
				if (fields.at(j).GetFieldType() == kIntegerType)
				{
					frp.read(record__data, sizeof(char)*int_len);
					offset += true_int;
				}
				else {
					frp.read(record__data, sizeof(char)*record_len);
					offset += true_len;
				}

				if (record__data[0] != '\a')
				{
					key = true;
				}
				else {
					record__data[0] = '\0';
				}
				record__datas.push_back(record__data);
			}

			if (key == true)
			{
				std::cout << "------ No." << k+1 << " ------" << endl;
				for (int j = 0; j < fields.size(); j++)
				{
					std::cout << fields.at(j).GetFieldName() << ":" << record__datas.at(j) << "  " << endl;
				}
				k++;
			}	
			i++;
		}
		std::cout << endl;
		return true;
	}
	else {
		std::cout << "打开表单失败！" << endl;
		return false;
	}
}

/**
*  \brief 表格显示
*/
bool Table::Display(USER_INT id,USER_INT iter)
{
	char record__data[record_len];
	if (UseTable())
	{
		//frp.sync();
		std::cout << "------ No." << iter+1 << " ------" << endl;
		USER_INT offset = 0;
		for (int j = 0; j < fields.size(); j++)
		{
			frp.seekg(sizeof(char)*(id*record_leng + offset), ios::beg);
			if (fields.at(j).GetFieldType() == kIntegerType)
			{
				frp.read(record__data, sizeof(char)*int_len);
				offset += true_int;
			}
			else {
				frp.read(record__data, sizeof(char)*record_len);
				offset += true_len;
			}

			std::cout << fields.at(j).GetFieldName() << ":" << record__data << "  " << endl;
		}
		return true;
	}
	else {
		std::cout << "表单打开失败！" << endl;
		return false;
	}
}

/**
*  \brief 顺序查找
*/
bool Table::OrderSelect(string select_field, Value select_value, OperatorType select_op)
{
	real_id.clear();
	indexs.at(0)->ShowAllId(real_id);

	std::string field = select_field;
	Value value = select_value;
	OperatorType op = select_op;
	char record__data[record_len];
	int field_id = 0;
	USER_INT count = 0, k = 0, i = 0, offset = 0,length;

	while (fields.at(field_id).GetFieldName() != field || fields.at(field_id).GetFieldType() != value.GetValueType())
	{
		if (fields.at(field_id).GetFieldType() == kIntegerType)
		{
			offset += true_int;
		}
		else {
			offset += true_len;
		}
		field_id++;
		if (field_id >= fields.size()) break;
	}

	if (field_id == fields.size())
	{
		std::cout << "字段名或数据类型无法匹配！" << endl;
		return false;
	}
	else
	{
		if (fields.at(field_id).GetFieldType() == kIntegerType)
		{
			length = true_int;
		}
		else {
			length = true_len;
		}

		while (k < records_num)
		{
			i = real_id.at(k++);
			frp.seekg(sizeof(char)*(i*record_leng + offset), ios::beg);
			frp.read(record__data, length*sizeof(char));
			if (record__data[0] == '\0')
			{
				continue;
			}
			switch (op)
			{
			case kOpEqual:
				if (record__data == value.GetValueData())
				{
					select_id.push_back(i);
					count++;
				}
				break;
			case kOpGreater:
				if (value.GetValueType()==kStringType)
				{
					if (record__data > value.GetValueData())
					{
						select_id.push_back(i);
						count++;
					}
				}
				else
				{
					int record_data_int,value_data_int;
					record_data_int = atoi(record__data);
					value_data_int = atoi(value.GetValueData().c_str());
					if (record_data_int > value_data_int)
					{
						select_id.push_back(i);
						count++;
					}
				}
				break;
			case kOpLess:
				if (value.GetValueType() == kStringType)
				{
					if (record__data < value.GetValueData())
					{
						select_id.push_back(i);
						count++;
					}
				}
				else
				{
					int record_data_int, value_data_int;
					record_data_int = atoi(record__data);
					value_data_int = atoi(value.GetValueData().c_str());
					if (record_data_int < value_data_int)
					{
						select_id.push_back(i);
						count++;
					}
				}
				break;
			case kOpNotEqual:
				if (record__data != value.GetValueData())
				{
					select_id.push_back(i);
					count++;
				}
				break;
			case kOpGreterOrEqual:
				if (value.GetValueType() == kStringType)
				{
					if (record__data >= value.GetValueData())
					{
						select_id.push_back(i);
						count++;
					}
				}
				else
				{
					int record_data_int, value_data_int;
					record_data_int = atoi(record__data);
					value_data_int = atoi(value.GetValueData().c_str());
					if (record_data_int >= value_data_int)
					{
						select_id.push_back(i);
						count++;
					}
				}
				break;
			case kOpLessOrEqual:
				if (value.GetValueType() == kStringType)
				{
					if (record__data <= value.GetValueData())
					{
						select_id.push_back(i);
						count++;
					}
				}
				else
				{
					int record_data_int, value_data_int;
					record_data_int = atoi(record__data);
					value_data_int = atoi(value.GetValueData().c_str());
					if (record_data_int <= value_data_int)
					{
						select_id.push_back(i);
						count++;
					}
				}
				break;
			default:
				std::cout << "操作符匹配失败！" << endl;
				return false;
			}
		}

		if (!count)
		{
			std::cout << "不存在符合条件的数据！" << endl;
			return false;
		}

		return true;
	}
}