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

/*************Record*************/
bool Record::Display(std::vector<Value> values_data, std::vector<Field> fields)
{
	for (int i = 0; i < 10; i++)
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
}

void Record::SetValue(std::vector<Value> values_data)
{
	for (int i = 0; i < 10; i++)
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
	return field_name;
}

ValueType Field::GetFieldType()
{
	return field_type;
}

bool Field::SetFieldName(std::string new_name)
{
	field_name = new_name;
}

bool Field::SetFieldType(ValueType new_type)
{
	field_type = new_type;
}



/*************Database**************/
bool Database::CreateDatabase(SQLCreateDatabase &st)
{
	std::string db_name;
	std::string db_path;
	db_name = st.GetDatabaseName();
	db_path = st.GetDatabasePath();
	std::string path;
	path = "md " + db_path + "\\" + db_name;
	char str[1000];
	strcpy(str, path.c_str());
	if (!access(str, 0))    //数据库已经存在
	{
		std::cerr << "数据库已存在！" << std::endl;
		return false;
	}
	else
	{
		system(str);
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
	db_name = st.GetDatabaseName;
	db_path = st.GetDatabasePath;
	std::string path;
	path = db_path + "\\" + db_name;
	char str[1000];
	strcpy(str, path.c_str());
	if (!access(str, 0))
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