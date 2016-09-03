#ifndef _TABLE_H_
#define _TABLE_H_

#include <string>
#include <vector>

#include "database.h"

/**
*  \brief 字段类
*/
class Field
{
public:
	/**
	*  \brief 构造函数
	*/
	Field(std::string Field_name);

	/**
	*  \brief 析构函数
	*/
	~Field();

	/**
	*  \brief 获取字段名
	*/
	std::string GetFieldName();

	/**
	*  \brief 获取字段数据类型
	*/
	ValueType GetFieldType();

	/**
	*  \brief 更改字段名
	*/
	bool SetFieldName(std::string new_name);

	/**
	*  \brief 更改字段数据类型
	*/
	bool SetFieldType(ValueType new_type);

private:
	std::string field_name;       /* 字段名  */
	ValueType field_type;         /* 字段数据类型 */
};

/**
*  \brief 表单类
*/
class Table
{
public:
	/**
	*  \brief 构造函数
	*/
	Table();

	/**
	*  \brief 析构函数
	*/
	~Table();

	/**
	*  \brief 创建表单
	*/
	bool CreateTable(SQLCreateTable &sql);

	/**
	*  \brief 打开表单
	*/
	bool UseTable();

	/**
	*  \brief 增加记录
	*/
	bool CreateRecord(SQLInsert &st);

	/**
	*  \brief 更新记录
	*/
	bool UpdateRecord(SQLInsert &st);

	/**
	*  \brief 删除记录
	*/
	bool DeleteRecord(SQLInsert &st);


private:
	long long int record_num;           /* 表中已有数据数量 */
	int fields_num;                     /* 表中字段数 */
	std::string table_name;             /* 表单名称 */
	std::vector<Record> records;        /* 记录 */
	std::vector<Field> fields;          /* 字段 */
	std::vector<Index> indexs;          /* 索引 */
};

#endif