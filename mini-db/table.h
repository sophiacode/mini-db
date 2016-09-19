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
	*
	*  \param new_path 数据库存储路径
	*/
	Table(std::string new_path);

	/**
	*  \brief 析构函数
	*/
	~Table();

	/**
	*  \brief 创建表单
	*
	*  \param sql SQL有效信息类
	*
	*  \return 创建成功返回true，否则返回false
	*/
	bool CreateTable(SQLCreateTable &sql);

	/**
	*  \brief 获取表单名称
	*
	*  \return 当前表的名称
	*/
	std::string GetTableName();

	/**
	*  \brief 获取表头信息
	*
	*  \return 当前表头存储的vector
	*/
	std::vector<Field> GetTableFields();

	/**
	*  \brief 重置表名
	*
	*  \param new_name 新的表名
	*/
	void SetTableName(std::string new_name);

	/**
	*  \brief 打开表单
	*
	*  \return 打开成功返回true,否则返回false
	*/
	bool UseTable();

	/**
	*  \brief 增加记录
	*
	*  \param st SQL语句信息类
	*
	*  \return 插入成功返回true,否则返回false
	*/
	bool CreateRecord(SQLInsert &st);

	/**
	*  \brief 更新记录
	*
	*  \param su SQL语句信息类
	*
	*  \return 更新成功返回true,否则返回false
	*/
	bool UpdateRecord(SQLUpdate &su);

	/**
	*  \brief 删除记录
	*
	*  \param sd SQL语句信息类
	*
	*  \return 删除成功返回true,否则返回false
	*/
	bool DeleteRecord(SQLDelete &sd);

	/**
	*  \brief 通过SQL直接查找记录
	*
	*  \param sql SQL语句信息类
	*
	*  \return 查找成功返回true,否则返回false
	*/
	bool SelectRecord(SQLSelect &sql);

	/**
	*  \brief Delete调用Select功能
	*
	*  \param sd SQL语句信息类
	*
	*  \return 查找成功返回true,否则返回false
	*/
	bool SelectRecord(SQLDelete &sd);

	/**
	*  \brief Update调用Select功能
	*
	*  \param su SQL语句信息类
	*
	*  \return 查找成功返回true,否则返回false
	*/
	bool SelectRecord(SQLUpdate &su);

	/**
	*  \brief 创建索引
	*
	*  \param si SQL语句信息类
	*
	*  \return 创建成功返回true,否则返回false
	*/
	bool CreateIndex(SQLCreateIndex &si);

	/**
	*  \brief 全表显示
	*/
	bool Display();

	/**
	*  \brief 记录显示
	*
	*  \param id 要显示的记录id
	*  \param iter 输出的顺序编号
	*
	*  \return 显示成功返回true，否则false
	*/
	bool Display(USER_INT id,USER_INT iter);

	/**
	*  \brief 顺序查找
	*
	*  \param select_field 要查找的字段名
	*  \param select_value 要查找的数据信息
	*  \param select_op 对应的运算符
	*
	*  \return 查找成功返回true，否则false
	*/
	bool OrderSelect(string select_field, Value select_value, OperatorType select_op);

private:

	/**
	*  \brief 找到字段对应的索引编号
	*
	*  \param index_field_name 索引对应的字段名
	*
	*  \return 查找到的索引对应编号，若没有索引返回-1
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