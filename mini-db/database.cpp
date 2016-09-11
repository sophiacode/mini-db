#include<iostream>
#include<string>
#include<vector>
#include"database.h"
#include"direct.h"
#include"io.h"
#include"sqlstatement.h"

/************Value************/
std::string Value::GetValueData()
{
	return value_data;
}

ValueType Value::GetValueType()
{
	return value_type;
}

void Value::SetValue(std::string new_data, ValueType new_type)
{
	value_data = new_data;
	value_type = new_type;
}

void Value::SetValuedata(std::string new_data)
{
	value_data = new_data;
}

/*************Record*************/
bool Record::Display(std::vector<Value> values_data, std::vector<Field> fields)
{
	for (int i = 0; i < fields.capacity(); i++)
	{

		ValueType valuetype;
		std::string valuedata;
		valuetype = values_data[i].GetValueType();
		valuedata = values_data[i].GetValueData();
		if (valuetype == kNullType)   //数据为空
			continue;
		else
		{
			std::cout << fields[i].GetFieldName() << ":" << std::endl;
			std::cout << valuedata << std::endl;
		}
	}

	return true;
}

void Record::SetValue(std::vector<Value> values_data)
{
	for (int i = 0; i < values_data.capacity(); i++)
	{
		ValueType valuetype;
		std::string valuedata;
		valuetype = values_data[i].GetValueType();
		valuedata = values_data[i].GetValueData();
		record_data[i].SetValue(valuedata, valuetype);
	}
}

/**************Field*****************/
Field::Field()
	:is_create_index_(false)
{
	
}

std::string Field::GetFieldName()
{
	return field_name_;
}

ValueType Field::GetFieldType()
{
	return field_type_;
}

void Field::SetFieldName(std::string new_name)
{
	field_name_ = new_name;
}

void Field::SetFieldType(ValueType new_type)
{
	field_type_ = new_type;
}

bool Field::IsCreateIndex()
{
	return is_create_index_;
}

void Field::SetIsCreateIndex(bool is_create_index)
{
	is_create_index_ = is_create_index;
}

/*************Database**************/
Database::~Database()
{
	for (auto iter : table_)
	{
		if (iter != nullptr)
		{
			delete iter;
			iter = nullptr;
		}
	}
}

bool Database::CreateDatabase(SQLCreateDatabase &st)
{
	std::string db_name;
	std::string db_path;
	db_name = st.GetDatabaseName();
	db_path = st.GetDatabasePath();
	std::string path;
	path = db_path + "\\" + db_name;

	if (!access(path.c_str(), 0))    //数据库已经存在
	{
		std::cerr << "数据库已存在！" << std::endl;
		return false;
	}
	else
	{
		database_path = path;
		database_name = db_name;
		path = "md " + db_path + "\\" + db_name;
		system(path.c_str());
		std::cout << "创建成功！" << std::endl;
		return true;
	}
}

std::string Database::UseDatabase(SQLUse &st)
{
	std::string db_name;
	std::string db_path;
	db_name = st.GetDatabaseName();
	db_path = st.GetDatabasePath();
	std::string path;
	path = db_path + "\\" + db_name;
	database_path = path;
	//std::cout << path << std::endl;

	char str[1000];
	strcpy(str, path.c_str());
	if (!_access(str, 0))
	{
		UseTable(path);
		std::cout << "打开成功！" << std::endl;

		return path;
	}
	else
	{
		std::cerr << "打开失败！" << std::endl;
		return "\0";
	}
}

std::string Database::GetDatabaseName()
{
	return database_name;
}

std::vector<Table *> Database::GetTable()
{
	return table_;
}

bool Database::UseTable(std::string DatabasePath)
{
	ifstream fp;
	std::string path;
	path = DatabasePath + "\\" + "table_name";

	fp.open(path);
	if (!fp.is_open())
	{
		//std::cout << "打开失败" << endl;
		fp.close();
		return false;
	}
	else
	{
		std::string table_name;
		/*while (fp.read(table_name_, sizeof(char) * 20))
		{
			//fp.read(table_name_, sizeof(char)* 20);
			std::string table_name(table_name_);
			Table *table = new Table(DatabasePath);
			table->SetTableName(table_name);
			if (table->UseTable())
			{
				table_.push_back(table);
			}
			fp.close();
		}
		std::cout << "打开成功" << endl;*/
		filebuf *pbuf;
		long int size;
		char * buffer;

		pbuf = fp.rdbuf();
		size = pbuf->pubseekoff(0, ios::end, ios::in);
		int database_num = size / 20;

		/*pbuf->pubseekpos(0, ios::in);
		buffer = new char[size];
		pbuf->sgetn(buffer, size);
		std::string buffer_(buffer);*/

		char table_name_[20];
		for (int i = 0; i < database_num; i++)
		{
			fp.seekg(sizeof(char)*i*20, ios::beg);
			fp >> table_name_;
			Table *table = new Table(DatabasePath);
			table->SetTableName(table_name_);
			if (table->UseTable())
			{
				table_.push_back(table);
			}
		}

		fp.close();
		return true;
	}

}

bool Database::CreateTable(SQLCreateTable & st)
{

	Table * table = new Table(database_path);

	if (table->CreateTable(st))
	{
		table_.push_back(table);
		ofstream fp;
		std::string path;
		std::string table_name_ = st.GetTableName();
		path = database_path + "\\" + "table_name";   /*创建一个文件名为table_name的文件夹存放表名*/
		fp.open(path, std::ios::app);
		fp.seekp(table_.size() * 20 * sizeof(char), ios::beg);
		fp.write(table_name_.c_str(), 20*sizeof(char));
		fp.close();
		return true;
	}
	return false;
}

/************Value************/

Index::Index(std::string index_name, std::string field_name, ValueType type, std::string path)
{
	index_name_ = index_name;
	field_name_ = field_name;
	type_ = type;

	if (type_ == kIntegerType)
	{
		bplustree_int_ = new BPlusTree<int>(path+".dbi");
		bplustree_string_ = nullptr;
	}
	if (type_ == kStringType)
	{
		bplustree_int_ = nullptr;
		bplustree_string_ = new BPlusTree<std::string>(path+".dbi");
	}
}

Index::Index(std::string index_name, std::string field_name, ValueType type)
{
	index_name_ = index_name;
	field_name_ = field_name;
	type_ = type;
	if (type_ == kIntegerType)
	{
		bplustree_int_ = new BPlusTree<int>();
		bplustree_string_ = nullptr;
	}
	if (type_ == kStringType)
	{
		bplustree_int_ = nullptr;
		bplustree_string_ = new BPlusTree<string>();
	}
}

Index::~Index()
{
	if (bplustree_int_ != nullptr)
	{
		delete bplustree_int_;
		bplustree_int_ = nullptr;
	}

	if (bplustree_string_ != nullptr)
	{
		delete bplustree_string_;
		bplustree_string_ = nullptr;
	}

}

bool Index::InsertNode(std::string value, int data_id)
{
	if (type_ == kIntegerType)
	{
		int temp = atoi(value.c_str());
		if (bplustree_int_->InsertNode(temp, data_id))
		{
			bplustree_int_->DeleteCache();
			return true;
		}
		return false;
	}

	else if (type_ == kStringType)
	{
		if (bplustree_string_->InsertNode(value, data_id))
		{
			bplustree_string_->DeleteCache();
			return true;
		}
		return false;
	}
}

bool Index::DeleteNode(std::string value)
{
	if (type_ == kIntegerType)
	{
		int temp = atoi(value.c_str());
		return bplustree_int_->DeleteNode(temp);
	}

	else if (type_ == kStringType)
	{
		return bplustree_string_->DeleteNode(value);
	}
}

int Index::SearchNode(std::string value)
{
	if (type_ == kIntegerType)
	{
		int temp = atoi(value.c_str());
		return bplustree_int_->SearchID(temp);
	}

	else if (type_ == kStringType)
	{
		return bplustree_string_->SearchID(value);
	}
}

bool Index::SearchNode(std::string value, std::vector<int>& id)
{
	if (type_ == kIntegerType)
	{
		int temp = atoi(value.c_str());
		return bplustree_int_->SearchID(temp,id);
	}

	else if (type_ == kStringType)
	{
		return bplustree_string_->SearchID(value,id);
	}
}

std::string Index::GetFieldName()
{
	return field_name_;
}

bool Index::UpdateNode(std::string new_value,std::string old_value)
{
if (type_ == kIntegerType)
{
int newkey = atoi(new_value.c_str());
int oldkey = atoi(old_value.c_str());
return bplustree_int_->UpdateNode(oldkey,newkey);
}

else if (type_ == kStringType)
{
return bplustree_string_->UpdateNode(old_value,new_value);
}
}
