#include<iostream>
#include<string>
#include<fstream>

#define record_num 1000

#include "database.h"
#include "table.h"
#include "sqlstatement.h"
#include "MemPool.h"
using namespace std;


/**
*  \brief 构造函数
*/
Table::Table() {}

/**
*  \brief 析构函数
*/
Table::~Table() {}

/**
*  \brief 打开目标表单，成功返回true，失败返回false
*/
bool Table::UseTable()
{
	std::string cmd = "start ..\\" + table_name;	/* 构建命令打开表单文件夹 */
	if (system(cmd.c_str()))
	{
		return true;
	}
	else return false;
}

/**
*  \brief 创建新表单
*/
bool Table::CreateTable(SQLCreateTable &sql)
{
	table_name = sql.GetTableName();	/* 获得表单名称 */
	std::string cmd = "md ..\\" + table_name;		/* 构建命令创建表单文件夹 */
	if (Table::UseTable())							/* 判断表单是否已经存在 */
	{
		return false;
	}
	else {
		if (system(cmd.c_str()))
		{
			fields = sql.GetFields();				/* 获取表头数据 */
			
			Table::UseTable();						/* 打开表单文件夹 */
			fstream fp;
			std::string table_name_fields = table_name + "_fields";						/* 构建表头文件名table_name_fields */
			fp.open(table_name_fields.c_str(), std::ios::binary | std::ios::out);		/* 创建表头文件 */

			for (int i = 0; i < fields_num; i++)						/* 写入表头数据 */
			{
				std::string name = fields[i].GetFieldName() + '\0';
				ValueType type = fields[i].GetFieldType();
				std::string type_;
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
	if (Table::UseTable())
	{
		int Record_id = MemPool::NewNode();
		if (Record_id % record_num == 1)
		{
			int records_no = Record_id / record_num;
			fstream fp;
			std::string table_name_records = st.GetTableName() + "_records";
		}
	}
	else return false;
}