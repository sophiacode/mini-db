#ifndef _TABLE_H_
#define _TABLE_H_

#include <string>
#include <vector>

#define record_num 1000

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
*  \brief 表单类
*/
class Table
{
public:
	/**
	*  \brief 构造函数
	*/
	Table(std::string nem_table_name);

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
	bool UpdateRecord(SQLInsert &su);

	/**
	*  \brief 删除记录
	*/
	bool DeleteRecord(SQLDelete &sd);

	/**
	*  \brief 通过SQL直接查找记录
	*/
	bool SelectRecord(SQLSelect &sql);

	/**
	*  \brief Delete调用Select功能
	*/
	bool SelectRecord(SQLDelete &sd);

	/**
	*  \brief Update调用Select功能
	*/
	bool SelectRecord(SQLUpdate &su);

private:
	int records_num;				     /* 表中已有数据数量 */
	std::string table_name;				 /* 表单名称 */
	Record record;						 /* 记录 */
	std::vector<Field> fields;           /* 字段 */
	std::vector<Index> indexs;           /* 索引 */
	std::vector<int> select_id;			 /* 记录select选中的id */
};

#endif