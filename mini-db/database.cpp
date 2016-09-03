#include<iostream>
#include<string>
#include<vector>
#include"database.h"
#include"direct.h"
#include"io.h"

/************Value************/
std::string Value::GetValueData()
{
	return value_data;
};

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
bool Record::Display(SQLInsert &st)
{

}



/***********Database**********/
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
	if (!access(str, 0))
	{
		std::cerr << "���ݿ��Ѵ��ڣ�" << std::endl;
		return false;
	}
	else
	{
		system(str);
		std::cerr << "�����ɹ���" << std::endl;
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
		std::cerr << "�򿪳ɹ���" << std::endl;
		return path;
	}
	else
	{
		std::cerr << "��ʧ�ܣ�" << std::endl;
		return "\0";
	}
}