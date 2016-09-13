#ifndef _SQLSTATEMENT_H_
#define _SQLSTATEMENT_H_

#include<vector>
#include<string>

#include "global.h"
#include "database.h"
#include "sqltype.h"

/**
 *  \brief SQL语句基类
 */
class SQLBase
{
public:
	/**
	 *  \brief 构造函数
	 */
	SQLBase(); 

	/**
	 *  \brief 析构函数
	 */
	~SQLBase();

	/**
	 *  \brief 获得解析结果
	 *
	 *  \return 解析成功返回true，失败返回false
	 */
	bool IsParseSucceed();

	/** 
	 *  \brief 命令解析（纯虚函数）
	 *
	 *  \param sql_token SQL标识符数组
	 */
	virtual void Parse(std::vector<std::string> sql_token) = 0;

protected:
	bool is_parse_succeed;        /* 是否解析成功的标识 */

	/**
	 *  \brief 将整个语句的字母都转换为小写
	 *
	 *  \param str 待转换的语句
	 */
	void ToLower(std::string & str);

	/**
	 *  \brief 将单引号的标识符合并为同一个
	 *
	 *  \param sql_token SQL标识符数组
	 *  \param pos 开始合并的位置
	 */
	void MergeValue(std::vector<std::string> & sql_token, int pos);

	/**
	 *  \brief 解析value的值与类型
	 *  
	 *  \param sql 待解析的value标识符
	 *  \param value 解析结果的容器
	 *
	 *  \return value有效返回true，无效返回false
	 */
	bool ParseValue(std::string sql, Value & value);

	/**
	 *  \brief 解析操作符
	 *  
	 *  \param op 操作符标识符
	 * 
	 *  \return 操作符类型
	 */
	OperatorType ParseOperator(std::string op);
};

/**
 *  \brief 创建数据库语句
 *         格式：CREATE DATABASE databasename databasepath;  
 */
class SQLCreateDatabase :public SQLBase
{
public:
	/**
	 *  \brief 构造函数
	 *
	 *  \param sql_token SQL标识符数组
	 */
	SQLCreateDatabase(std::vector<std::string> sql_token);

	/**
	 *  \brief 析构函数
	 */
	~SQLCreateDatabase();

	/**
	 *  \brief 获取数据库名称
	 *
	 *  \return 数据库名称
	 */
	std::string GetDatabaseName();

	/**
	 *  \brief 获取数据库路径
	 *
	 *  \return 数据库路径
	 */
	std::string GetDatabasePath();

	/**
	 *  \brief 命令解析（重载）
	 *
	 *  \param sql_token SQL标识符数组
	 */
	void Parse(std::vector<std::string> sql_token) override;

private:
	std::string database_name_;  /* 数据库名称 */
	std::string database_path_;  /* 数据库路径 */
};

/**
 *  \brief 打开数据库语句
 *         格式：USE databasename databasepath;
 */
class SQLUse :public SQLBase
{
public:
	/**
	 *  \brief 构造函数
	 *
	 *  \param sql_token SQL标识符数组
	 */
	SQLUse(std::vector<std::string> sql_token);

	/**
	 *  \brief 析构函数
	 */
	~SQLUse();

	/**
	 *  \brief 获取数据库名称
	 *
	 *  \return 数据库名称
	 */
	std::string GetDatabaseName();

	/**
	 *  \brief 获取数据库路径
	 *
	 *  \return 数据库路径
	 */
	std::string GetDatabasePath();

	/**
	 *  \brief 命令解析（重载）
	 *
	 *  \param sql_token SQL标识符数组
	 */
	void Parse(std::vector<std::string> sql_token) override;

private:
	std::string database_name_;  /* 数据库名称 */
	std::string database_path_;  /* 数据库路径 */
};

/**
 *  \brief 建表语句
 *         格式：CREATE TABLE tablename
 *              (fieldname1 type1, fieldname2 type2, ...); 
 */
class SQLCreateTable :public SQLBase
{
public:
	/**
	 *  \brief 构造函数
	 *
	 *  \param sql_token SQL标识符数组
	 */
	SQLCreateTable(std::vector<std::string> sql_token);

	/**
	 *  \brief 析构函数
	 */
	~SQLCreateTable();

	/**
	 *  \brief 获取表名
	 *
	 *  \return 表名
	 */
	std::string GetTableName();

	/**
	 *  \brief 获取字段数组
	 *
	 *  \return 字段数组
	 */
	std::vector<Field> GetFields();

	/**
	 *  \brief 命令解析（重载）
	 *
	 *  \param sql_token SQL标识符数组
	 */
	void Parse(std::vector<std::string> sql_token) override;

private:
	std::string table_name_;      /* 表名 */
	std::vector<Field> fields_;   /* 字段数组 */
};

/**
 *  \brief 插入语句
 *         格式：INSERT INTO tablename (field1,field2,...) 
 *               VALUES (value1,value2,...)
 */
class SQLInsert :public SQLBase
{
public:
	/**
	 *  \brief 构造函数
	 *
	 *  \param sql_token SQL标识符数组
	 */
	SQLInsert(std::vector<std::string> sql_token);

	/**
	 *  \brief 析构函数
	 */
	~SQLInsert();

	/**
	 *  \brief 获取表名
	 *
	 *  \return 表名
	 */
	std::string GetTableName();

	/**
	 *  \brief 获取字段名数组
	 *
	 *  \return 字段名数组
	 */
	std::vector<std::string> GetFields();

	/**
	 *  \brief 获取值数组
	 *
	 *  \return 值数组
	 */
	std::vector<Value> GetValues();

	/**
	 *  \brief 语句是否含有字段
	 *  
	 *  \return 含有字段返回true，不含有返回false
	 */
	bool IsInputField();

	/**
	 *  \brief 命令解析（重载）
	 *
	 *  \param sql_token SQL标识符数组
	 */
	void Parse(std::vector<std::string> sql_token) override;

private:
	std::string table_name_;                /* 表名 */
	std::vector<std::string> fields_;       /* 字段名数组 */
	std::vector<Value> values_;             /* 值数组 */
	bool is_input_field_;                   /* 是否含有字段的标识 */  
};

/**
 *  \brief 删除语句
 *         格式：DELETE FROM tablename WHERE field op value;
 */
class SQLDelete :public SQLBase
{
public:
	/**
	 *  \brief 构造函数
	 *
	 *  \param sql_token SQL标识符数组
	 */
	SQLDelete(std::vector<std::string> sql_token);

	/**
	 *  \brief 析构函数
	 */
	~SQLDelete();

	/**
	 *  \brief 获取表名
	 *
	 *  \return 表名
	 */
	std::string GetTableName();

	/**
	 *  \brief 获取where子句中的字段
	 *  
	 *  \return where子句中的字段
	 */
	std::string GetField();

	/**
	 *  \brief 获取where子句中的值
	 *
	 *  \return where子句中的值
	 */
	Value GetValue();

	/**
	 *  \brief 获取where子句中的操作符
	 *
	 *  \return where子句中的操作符
	 */
	OperatorType GetOperatorType();

	/**
	 *  \brief 语句是否含有where子句
	 *
	 *  \return 含有where子句返回true，不含有返回false
	 */
	bool IsInputWhere();

	/**
	 *  \brief 命令解析（重载）
	 *
	 *  \param sql_token SQL标识符数组
	 */
	void Parse(std::vector<std::string> sql_token) override;

private:
	std::string table_name_;         /* 表名 */
	std::string field_;              /* where子句中的字段名 */
	Value * value_;                  /* where子句中的值 */
	OperatorType op_;                /* where子句中的操作符 */
	bool is_input_where_;            /* 语句是否含有where子句的标识 */
	
};

/**
 *  \brief 更新语句
 *         格式：UPDATE tablename SET field1=value1, field2=value2, ... 
 *		         WHERE field op value;
 */
class SQLUpdate :public SQLBase
{
public:
	/**
	 *  \brief 构造函数
	 *
	 *  \param sql_token SQL标识符数组
	 */
	SQLUpdate(std::vector<std::string> sql_token);

	/**
	 *  \brief 析构函数
	 */
	~SQLUpdate();

	/**
	 *  \brief 获取表名
	 *
	 *  \return 表名
	 */
	std::string GetTableName();

	/**
	 *  \brief 获取set语句中的字段名数组
	 *
	 *  \return set语句中的字段名数组
	 */
	std::vector<std::string> GetNewField();

	/**
	 *  \brief 获取set语句中的新值数组
	 *
	 *  \return set语句中的新值数组
	 */
	std::vector<Value> GetNewValue();

	/**
	 *  \brief 获取where子句中的字段
	 *
	 *  \return where子句中的字段
	 */
	std::string GetWhereField();

	/**
	 *  \brief 获取where子句中的旧值
	 *
	 *  \return where子句中的旧值
	 */
	Value GetWhereValue();

	/**
	 *  \brief 获取where子句中的操作符
	 *
	 *  \return where子句中的操作符
	 */
	OperatorType GetOperatorType();

	/**
	 *  \brief 命令解析（重载）
	 *
	 *  \param sql_token SQL标识符数组
	 */
	void Parse(std::vector<std::string> sql_token) override;

private:
	std::string table_name_;               /* 表名 */
	std::vector<std::string> new_fields_;  /* set语句中的字段名数组 */
	std::vector<Value> new_values_;        /* set语句中的新值数组 */
	std::string where_field_;              /* where子句中的字段 */
	Value * where_value_;                  /* where子句中的旧值 */
	OperatorType op_;                      /* where子句中的操作符 */
};

/**
 *  \brief 查询语句
 *         格式：SELECET FROM tablename WHERE field op value
 */
class SQLSelect :public SQLBase
{
public:
	/**
	 *  \brief 构造函数
	 *
	 *  \param sql_token SQL标识符数组
	 */
	SQLSelect(std::vector<std::string> sql_token);

	/**
	 *  \brief 析构函数
	 */
	~SQLSelect();

	/**
	 *  \brief 获取表名
	 *
	 *  \return 表名
	 */
	std::string GetTableName();

	/**
	 *  \brief 获取where子句中的字段
	 *
	 *  \return where子句中的字段
	 */
	std::string GetField();

	/**
	 *  \brief 获取where子句中的值
	 *
	 *  \return where子句中的值
	 */
	Value GetValue();

	/**
	 *  \brief 获取where子句中的操作符
	 *
	 *  \return where子句中的操作符
	 */
	OperatorType GetOperatorType();

	/**
	 *  \brief 语句是否含有where子句
	 *
	 *  \return 含有where子句返回true，不含有返回false
	 */
	bool IsInputWhere();

	/**
	 *  \brief 命令解析（重载）
	 *
	 *  \param sql_token SQL标识符数组
	 */
	void Parse(std::vector<std::string> sql_token) override;

private:
	std::string table_name_;    /* 表名 */
	std::string field_;         /* where子句中的字段名 */
	Value * value_;             /* where子句中的值 */
	OperatorType op_;           /* where子句中的操作符 */
	bool is_input_where_;       /* 语句是否含有where子句 */
};

/**
 *  \brief 创建索引语句
 *         格式：CREATE INDEX indexname ON tablename(fieldname); 
 */
class SQLCreateIndex :public SQLBase
{
public:
	/**
	 *  \brief 构造函数
	 *
	 *  \param sql_token SQL标识符数组
	 */
	SQLCreateIndex(std::vector<std::string> sql_token);

	/**
	 *  \brief 析构函数
	 */
	~SQLCreateIndex();

	/**
	 *  \brief 获取表名
	 *
	 *  \return 表名
	 */
	std::string GetTableName();

	/**
	 *  \brief 获取字段名
	 *
	 *  \return 字段名
	 */
	std::string GetField();

	/**
	 *  \brief 获取索引名
	 *
	 *  \return 索引名
	 */
	std::string GetIndex();

	/**
	 *  \brief 命令解析（重载）
	 *
	 *  \param sql_token SQL标识符数组
	 */
	void Parse(std::vector<std::string> sql_token) override;

private:
	std::string table_name_;   /* 表名 */
	std::string field_;        /* 字段名 */
	std::string index_;        /* 索引名 */
};

#endif
