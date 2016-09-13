#ifndef _SQLSTATEMENT_H_
#define _SQLSTATEMENT_H_

#include<vector>
#include<string>

#include "global.h"
#include "database.h"
#include "sqltype.h"

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

	bool IsParseSucceed();
	virtual void Parse(std::vector<std::string> sql_token) = 0;
protected:
	bool is_parse_succeed;
	void ToLower(std::string & str);
	void MergeValue(std::vector<std::string> & sql_token, int pos);
	bool ParseValue(std::string sql, Value & value);
	OperatorType ParseOperator(std::string op);
};

/* " CREATE DATABASE databasename databasepath; " */
class SQLCreateDatabase :public SQLBase
{
public:
	SQLCreateDatabase(std::vector<std::string> sql_token);
	~SQLCreateDatabase();
	std::string GetDatabaseName();
	std::string GetDatabasePath();
	void Parse(std::vector<std::string> sql_token) override;
private:
	std::string database_name_;
	std::string database_path_;
};

/* "USE databasename databasepath;" */
class SQLUse :public SQLBase
{
public:
	SQLUse(std::vector<std::string> sql_token);
	~SQLUse();
	std::string GetDatabaseName();
	std::string GetDatabasePath();
	void Parse(std::vector<std::string> sql_token) override;
private:
	std::string database_name_;
	std::string database_path_;
};

/* "CREATE TABLE tablename(fieldname1 type1, fieldname2 type2, ...);" */
class SQLCreateTable :public SQLBase
{
public:
	SQLCreateTable(std::vector<std::string> sql_token);
	~SQLCreateTable();
	std::string GetTableName();
	std::vector<Field> GetFields();
	void Parse(std::vector<std::string> sql_token) override;
private:
	std::string table_name_;
	std::vector<Field> fields_;
};

/* "INSERT INTO tablename (field1,field2,...) VALUES (value1,value2,...)" */
class SQLInsert :public SQLBase
{
public:
	SQLInsert(std::vector<std::string> sql_token);
	~SQLInsert();
	std::string GetTableName();
	std::vector<std::string> GetFields();
	std::vector<Value> GetValues();
	bool IsInputField();
	void Parse(std::vector<std::string> sql_token) override;
private:
	std::string table_name_;
	std::vector<std::string> fields_;
	std::vector<Value> values_;
	bool is_input_field_;
};

/* "DELETE FROM tablename WHERE field = value" */
class SQLDelete :public SQLBase
{
public:
	SQLDelete(std::vector<std::string> sql_token);
	~SQLDelete();
	std::string GetTableName();
	std::string GetField();
	Value GetValue();
	OperatorType GetOperatorType();
	bool IsInputWhere();
	void Parse(std::vector<std::string> sql_token) override;
private:
	std::string table_name_;
	std::string field_;
	Value * value_;
	bool is_input_where_;
	OperatorType op_;
};

/* "UPDATE tablename SET field1=value1, field2=value2, ... WHERE field=value;" */
class SQLUpdate :public SQLBase
{
public:
	SQLUpdate(std::vector<std::string> sql_token);
	~SQLUpdate();
	std::string GetTableName();
	std::vector<std::string> GetNewField();
	std::vector<Value> GetNewValue();
	std::string GetWhereField();
	Value GetWhereValue();
	OperatorType GetOperatorType();
	void Parse(std::vector<std::string> sql_token) override;
private:
	std::string table_name_;
	std::vector<std::string> new_fields_;
	std::vector<Value> new_values_;
	std::string where_field_;
	Value * where_value_;
	OperatorType op_;
};

/* "SELECET FROM tablename WHERE field op value" */
class SQLSelect :public SQLBase
{
public:
	SQLSelect(std::vector<std::string> sql_token);
	~SQLSelect();
	std::string GetTableName();
	std::string GetField();
	Value GetValue();
	OperatorType GetOperatorType();
	bool IsInputWhere();
	void Parse(std::vector<std::string> sql_token) override;
private:
	std::string table_name_;
	std::string field_;
	Value * value_;
	OperatorType op_;
	bool is_input_where_;
};

/* CREATE INDEX indexname ON tablename(fieldname) */
class SQLCreateIndex :public SQLBase
{
public:
	SQLCreateIndex(std::vector<std::string> sql_token);
	~SQLCreateIndex();
	std::string GetTableName();
	std::string GetField();
	std::string GetIndex();
	void Parse(std::vector<std::string> sql_token) override;
private:
	std::string table_name_;
	std::string field_;
	std::string index_;
};

#endif
