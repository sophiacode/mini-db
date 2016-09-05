#ifndef _DATABASE_H_
#define _DATABASE_H_

#include<string>
#include<vector>
#include"sqlstatement.h"
#include"table.h"
#include"BPlusTree.h"

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

	bool IsCreateIndex();

private:
	std::string field_name;       /* 字段名  */
	ValueType field_type;         /* 字段数据类型 */
	bool is_create_index;         /* 标识该字段是否建立索引 */
};

/**
*  \brief 索引类
*/
template<class KEYTYPE>
class Index
{
public:
	/**
	*  \brief 构造函数
	*/
	Index(std::string index_name,std::string field_name);

	/**
	*  \brief 析构函数
	*/
	~Index();

	bool InsertNode(KEYTYPE key, int data_id);

	bool DeleteNode(KEYTYPE key);

	int SearchNode(KEYTYPE key);

	bool UpdateNode(KEYTYPE key);
private:
	BPlusTree * bplustree_;   /* 索引的B+树 */
	std::string field_name;  /* 索引对应的字段名 */
	std::string index_name;  /* 索引名 */
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
	Database();

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
	std::string database_path;          /* 数据库路径 */
};
#endif



template<class KEYTYPE>
inline Index<KEYTYPE>::Index(std::string index_name, std::string field_name)
{
	index_name_ = index_name;
	field_name_ = field_name;
	
	bplustree_ = new BPlusTree();
}

template<class KEYTYPE>
inline Index<KEYTYPE>::~Index()
{
	if (bplustree_ != nullptr)
	{
		delete bplustree_;
		bplustree_ = nullptr;
	}
}

template<class KEYTYPE>
inline bool Index<KEYTYPE>::InsertNode(KEYTYPE key, int data_id)
{
	return bplustree_->InsertNode(key,data_id);
}

template<class KEYTYPE>
inline bool Index<KEYTYPE>::DeleteNode(KEYTYPE key)
{
	return bplustree_->DeleteNode(key);
}

template<class KEYTYPE>
inline int Index<KEYTYPE>::SearchNode(KEYTYPE key)
{
	return bplustree_->SearchNode(key);
}

template<class KEYTYPE>
inline bool Index<KEYTYPE>::UpdateNode(KEYTYPE key)
{
	return bplustree_->UpdateNode(key);
}

