#ifndef _TABLE_H_
#define _TABLE_H_

#include <string>
#include <vector>

#define record_num 10000
#define record_max 10000
#define record_len 256

#include "database.h"
#include "IDPool.h"

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
	bool Display(int id);

private:

	/**
	*  \brief 找到字段对应的索引编号
	*/
	int FindIndex(std::string index_field_name);

private:
	int records_num;				     /* 表中已有数据数量 */
	int indexs_num;						 /* 表中已有索引数量 */
	std::string table_name;				 /* 表单名称 */
	std::vector<Field> fields;           /* 字段 */
	std::vector<Index *> indexs;         /* 索引 */
	std::vector<int> select_id;			 /* 记录select选中的id */
	std::string path;					 /* 存储数据库路径 */
	IDPool idPool;						 /* 主键内存池，用于申请新id */
	ofstream fwp;						 /* 记录文件写 */
	ifstream frp;						 /* 记录文件读 */
};

#endif