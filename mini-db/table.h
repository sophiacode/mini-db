#ifndef _TABLE_H_
#define _TABLE_H_

#include <string>
#include <vector>
#include <fstream>
#include <io.h>

#define record_max 1000000
#define record_len 256
#define int_len    10
#define true_int   16
#define true_len   272

#include "database.h"
#include "IDPool.h"
#include "sqlstatement.h"
#include "sqltype.h"

/**
*  \brief 表单类
*/
class Table
{
public:
	/**
	*  \brief 构造函数
	*/
	Table(std::string new_path);

	/**
	*  \brief 析构函数
	*/
	~Table();

	/**
	*  \brief 创建表单
	*/
	bool CreateTable(SQLCreateTable &sql);

	/**
	*  \brief 获取表单名称
	*/
	std::string GetTableName();

	/**
	*  \brief 获取表头信息
	*/
	std::vector<Field> GetTableFields();

	/**
	*  \brief 重置表名
	*/
	void SetTableName(std::string new_name);

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
	bool UpdateRecord(SQLUpdate &su);

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

	/**
	*  \brief 创建索引
	*/
	bool CreateIndex(SQLCreateIndex &si);

	/**
	*  \brief 全表显示
	*/
	bool Display();

	/**
	*  \brief 记录显示
	*/
	bool Display(USER_INT id,USER_INT iter);

	/**
	*  \brief 顺序查找
	*/
	bool OrderSelect(string select_field, Value select_value, OperatorType select_op);
private:

	/**
	*  \brief 找到字段对应的索引编号
	*/
	int FindIndex(std::string index_field_name);

	bool WildcardSearch(std::string pattern, int field_id, int index_id);

	bool WildcardMatch(std::string str, std::string pattern);

private:
	USER_INT records_num;				 /* 表中已有数据数量 */
	USER_INT record_leng;				 /* 一条记录的实际长度 */
	int indexs_num;						 /* 表中已有索引数量 */
	std::string table_name;				 /* 表单名称 */
	std::vector<Field> fields;           /* 字段 */
	std::vector<Index *> indexs;         /* 索引 */
	std::vector<USER_INT> select_id;	 /* 记录select选中的id */
	std::vector<USER_INT> real_id;		 /* 有效id存放池 */
	std::string path;					 /* 存储数据库路径 */
	IDPool * idPool;					 /* 主键内存池，用于申请新id */
	ofstream fwp;						 /* 记录文件写 */
	ifstream frp;						 /* 记录文件读 */
};

#endif