#ifndef _DATABASE_H_
#define _DATABASE_H_

#include<string>
#include<vector>

#include "global.h"
#include "sqlstatement.h"
#include "table.h"
#include "BPlusTree.h"


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
	Value() { ; }

	/**
	*  \brief 析构函数
	*/
	~Value() { ; }

	/**
	*  \brief 获取Value的值
	*/
	std::string GetValueData();

	/**
	*  \brief 获取Value的类型
	*/
	ValueType GetValueType();

	/**
	*  \brief 重置Value
	*/
	void SetValue(std::string new_data, ValueType new_type);

	/**
	*  \brief 重置Value的值
	*/
	void SetValuedata(std::string new_data);


private:
	std::string value_data;      /* Value的值 */
	ValueType value_type;        /* Value的类型 */
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
	Record() { ; }

	/**
	*  \brief 析构函数
	*/
	~Record() { ; }

	/**
	*  \brief 显示记录
	*/
	bool Display(std::vector<Value> values_data, std::vector<Field> fields);

	/**
	*  \brief 设置Value的值
	*/
	void SetValue(std::vector<Value> values_data);

private:
	int record_id;                         /* 记录主键 */
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
	~Field() { ; }

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
	void SetFieldName(std::string new_name);

	/**
	*  \brief 更改字段数据类型
	*/
	void SetFieldType(ValueType new_type);

	bool IsCreateIndex();

	void SetIsCreateIndex(bool is_create_index);

private:
	std::string field_name_;       /* 字段名  */
	ValueType field_type_;         /* 字段数据类型 */
	bool is_create_index_;         /* 标识该字段是否建立索引 */
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
	Index(std::string index_name, std::string field_name, ValueType type, std::string path);

	Index(std::string index_name, std::string field_name, ValueType type);

	/**
	*  \brief 析构函数
	*/
	~Index();

	bool InsertNode(std::string value, int data_id);

	bool DeleteNode(std::string value);

	int SearchNode(std::string value);

	bool SearchNode(std::string value, std::vector<int>& id);

  bool UpdateNode(std::string new_value, std::string old_value);

	std::string GetFieldName();

public:
	BPlusTree<int> * bplustree_int_;               /* int类型索引的B+树 */
	BPlusTree<std::string> * bplustree_string_;    /* string类型索引的B+树 */

private:
	std::string field_name_;                       /* 索引对应的字段名 */
	std::string index_name_;                       /* 索引名 */
	ValueType type_;                               /* 索引类型 */
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
	Database() { ; }

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
	std::vector<Table *> GetTable();

	/**
	*  \brief 创建表单
	*/
	bool CreateTable(SQLCreateTable & st);

	/**
	*  \brief 打开表单
	*/
	bool UseTable(std::string DatabasePath);

private:
	std::string database_name;          /* 数据库名称 */
	std::vector<Table *> table_;      /* 数据库中表单 */
	std::string database_path;          /* 数据库路径 */
};
#endif


