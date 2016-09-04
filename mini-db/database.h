#ifndef _DATABASE_H_
#define _DATABASE_H_

#include<string>
#include<vector>
#include"sqlstatement.h"

/**
*  \brief 数据类型
*/
enum ValueType
{
	kIntegerType,
	kStringType,
	kNullType
};

/**
*  \brief 值类
*/
class Value
{
public:
	/**
	*  \brief 构造函数
	*/
	Value();

	/**
	*  \brief 析构函数
	*/
	~Value();

	/**
	*  \brief 获取Value的值
	*/
	std::string GetValueData();

	/**
	*  \brief 获取Value的类型
	*/
	ValueType GetValueType();

	/**
	*  \brief 重置Value的值
	*/
	void SetValue(std::string new_data, ValueType new_type);

private:
	std::string value_data;      /* 这个Value的值 */
	ValueType value_type;         /*  */
};

/**
*  \brief 记录类
*/
class Record
{
public:
	/**
	*  \brief 构造函数
	*/
	Record();

	/**
	*  \brief 析构函数
	*/
	~Record();

	/**
	*  \brief 显示记录
	*/
	bool Display(std::vector<Value> values_data, std::vector<Field> fields);

	/**
	*  \brief 设置Value的值
	*/
	void SetValue(std::vector<Value> values_data);

private:
	int record_id;                /* 记录主键 */
	std::vector<Value> record_data;        /* 记录数据 */
};

/**
*  \brief 字段类
*/
class Field
{
public:
	/**
	*  \brief 构造函数
	*/
	Field();

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
*  \brief 索引类
*/
class Index
{
public:
	/**
	*  \brief 构造函数
	*/
	Index(std::string Field_name);

	/**
	*  \brief 析构函数
	*/
	~Index();

	/**
	*  \brief 创建新增索引
	*/
	bool AddIndex(std::string field_name);

private:
	BPlusTree bplustree;     /* 索引的B+树 */
	std::string field_name;  /* 索引对应的字段名 */
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

/**
*  \brief 数据库类
*/
class Database
{
public:
	/**
	*  \brief 构造函数
	*/
	Database(std::string Database_name);

	/**
	*  \brief 析构函数
	*/
	~Database();

	/**
	*  \brief 创建数据库
	*/
	bool CreateDatabase(SQLCreateDatabase &st);

	/**
	*  \brief 打开数据库,并返回当前数据库的路径
	*/
	std::string UseDatabase(SQLUse &st);


	/**
	*  \brief 获取数据库名称
	*/
	std::string GetDatabaseName();

	/**
	*  \brief 获取表名
	*/
	std::vector<Table> GetTableName();

private:
	std::string database_name;          /* 数据库名称 */
	std::vector<Table> table_name;      /* 数据库中表单 */
};
#endif