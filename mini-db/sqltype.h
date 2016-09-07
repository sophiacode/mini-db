#pragma once

/**
*  \brief SQL命令类型
*/
enum SQLType
{
	kSQLUndefined,         /* 未定义 */
	kSQLCreateDatabase,    /* 创建数据库 */
	kSQLUse,               /* 使用数据库 */
	kSQLCreateTable,       /* 创建表 */
	kSQLInsert,            /* 增加记录 */
	kSQLDelete,            /* 删除记录 */
	kSQLUpdate,            /* 更新记录 */
	kSQLSelect,            /* 查询记录 */
	kSQLCreateIndex,        /* 创建索引 */
	kSQLDisplay
};

/**
 *  \brief 操作符类型
 */
enum OperatorType
{
	kOpUndefined,        /*未定义*/
	kOpEqual,            /*等于*/
	kOpGreater,          /*大于*/
	kOpLess,             /*小于*/
	kOpNotEqual,         /*不等于*/ 
	kOpGreterOrEqual,    /*大于等于*/
	kOpLessOrEqual       /*小于等于*/
};
