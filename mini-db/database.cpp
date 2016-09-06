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
		if (valuetype ==kNullType)   //数据为空
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
		path = "md " + db_path + "\\" + db_name;
		system(path.c_str());
		database_name = db_name;
		database_path = db_path;
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

	std::cout << path << std::endl;

	char str[1000];
	strcpy(str, path.c_str());
	if (!_access(str, 0))
	{
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

std::vector<Table> Database::GetTableName()
{
	return table_name;
}

/************Value************/

Index::Index(std::string index_name, std::string field_name, ValueType type,std::string path)
{
	index_name_ = index_name;
	field_name_ = field_name;
	type_ = type;

	if (type_ == kIntegerType)
	{
		bplustree_int_ = new BPlusTree<int>(path);
		bplustree_string_ = nullptr;
	}
	if (type_ == kStringType)
	{
		bplustree_int_ = nullptr;
		bplustree_string_ = new BPlusTree<std::string>(path);
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
		return bplustree_int_->InsertNode(temp, data_id);
	}

	else if (type_ == kStringType)
	{
		return bplustree_string_->InsertNode(value, data_id);
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

std::string Index::GetFieldName()
{
	return field_name_;
}

/*bool Index::UpdateNode(std::string value)
{
	if (type_ == kIntegerType)
	{
		int temp = atoi(value.c_str());
		return bplustree_int_->UpdateNode(temp);
	}

	else if (type_ == kStringType)
	{
		return bplustree_string_->UpdateNode(value);
	}
}*/
