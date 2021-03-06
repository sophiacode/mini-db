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
	*
	*  \return Value的类型
	*/
	ValueType GetValueType();

	/**
	*  \brief 重置Value
	*
	*  \param new_data 新的Value的值 new_type 新的Value的类型
	*/
	void SetValue(std::string new_data, ValueType new_type);

	/**
	*  \brief 重置Value的值
	*
	*  \param 新的Value的值
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
	*
	*  \param values_data 一条记录中值的数组 fields 字段数组
	*
	*  \return 成功返回true 失败返回false
	*/
	bool Display(std::vector<Value> values_data, std::vector<Field> fields);

	/**
	*  \brief 设置Value的值
	*
	*  \param values_data 值的数组
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
	*
	*  \return 字段名
	*/
	std::string GetFieldName();

	/**
	*  \brief 获取字段数据类型
	*
	*  \return 字段数据类型
	*/
	ValueType GetFieldType();

	/**
	*  \brief 更改字段名
	*
	*  \param new_name 新的字段名
	*/
	void SetFieldName(std::string new_name);

	/**
	*  \brief 更改字段数据类型
	*
	*  \param new_type 新的字段数据类型
	*/
	void SetFieldType(ValueType new_type);

	/**
	*  \brief 判断字段是否已经创建索引
	*
	*  \return 已经创建返回true 未创建返回false
	*/
	bool IsCreateIndex();

	/**
	*  \brief 设置标识字段是否建立索引
	*
	*  \param is_create_index 标识该字段是否建立索引
	*/
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

	bool InsertNode(std::string value, USER_INT data_id);

	bool DeleteNode(std::string value);


	/**
	*  \brief 查找第一个关键字的节点
	*
	*  \param value 节点的值
	*
	*  \return 节点的id
	*/
	int SearchNode(std::string value);

	/**
	*  \brief 查找所有关键字的节点
	*
	*  \param value 节点的值 id 节点的id数组
	*
	*  \return 查找成功返回true 失败返回false
	*/
	bool SearchNode(std::string value, std::vector<USER_INT>& id);

	/**
	*  \brief 查找符合关系式的节点
	*
	*  \param value 节点的值 id 节点的id数组 op 操作符
	*
	*  \return 查找成功返回true 失败返回false
	*/
	bool SearchNode(std::string value, std::vector<USER_INT>& id, OperatorType op);

	/**
	*  \brief 查找符合关系式的节点
	*
	*  \param new_value 新的节点的值 old_value 旧的节点的值
	*
	*  \return 更新成功返回true 失败返回false
	*/
    bool UpdateNode(std::string new_value, std::string old_value);

	/**
	*  \brief 查找所有id
	*
	*  \param id的数组
	*
	*  \return 成功返回true 失败返回false
	*/
	bool ShowAllId(std::vector<USER_INT> &id);

	/**
	*  \brief 获取索引对应的字段名
	*
	*  \return 索引对应的字段名
	*/
	std::string GetFieldName();

public:
	BPlusTree<USER_INT> * bplustree_int_;          /* int类型索引的B+树 */
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
	*
	*  \param &st sql创建数据库
	*
	*  \return 成功返回true 失败返回false
	*/
	bool CreateDatabase(SQLCreateDatabase &st);

	/**
	*  \brief 打开数据库
	*
	*  \param &st sql打开数据库
	*
	*  \return 数据库路径
	*/
	std::string UseDatabase(SQLUse &st);

	/**
	*  \brief 获取数据库名称
	*
	*  \return 数据库名称
	*/
	std::string GetDatabaseName();

	/**
	*  \brief 获取表名
	*
	*  \return 表名数组
	*/
	std::vector<Table *> GetTable();
	
	/**
	*  \brief 创建表单
	*
	*  \param st sql创建表
	*
	*  \return 创建成功返回true 失败返回false
	*/
	bool CreateTable(SQLCreateTable & st);

	/**
	*  \brief 打开表单
	*
	*  \param DatabasePath 数据库路径
	*
	*  \return 打开成功返回true 失败返回false
	*/
	bool UseTable(std::string DatabasePath);

private:
	std::string database_name;          /* 数据库名称 */
	std::vector<Table *> table_;      /* 数据库中表单 */
	std::string database_path;          /* 数据库路径 */
};
#endif

