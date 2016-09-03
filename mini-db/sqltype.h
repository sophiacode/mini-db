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
	kSQLCreateIndex        /* 创建索引 */
};
