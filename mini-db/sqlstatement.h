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
	void SetDatabaseName(std::string database_name);
	std::string GetDatabasePath();
	void SetDatabasePath(std::string database_path);
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
	void SetDatabaseName(std::string database_name);
	std::string GetDatabasePath();
	void SetDatabasePath(std::string database_path);
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
	void SetTableName(std::string table_name);
	std::vector<Field> GetFields();
	void SetFields(std::vector<Field> fields);
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
	void SetTableName(std::string table_name);
	std::vector<std::string> GetFields();
	void SetFields(std::vector<std::string> fields);
	std::vector<Value> GetValues();
	void SetValues(std::vector<Value> values);
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
	void SetTableName(std::string table_name);
	std::string GetField();
	void SetField(std::string field);
	Value GetValue();
	void SetValue(Value value);
	bool IsInputWhere();
	void Parse(std::vector<std::string> sql_token) override;
private:
	std::string table_name_;
	std::string field_;
	Value * value_;
	bool is_input_where_;
};

/* "UPDATE tablename SET field1=value1, field2=value2, ... WHERE field=value;" */
class SQLUpdate :public SQLBase
{
public:
	SQLUpdate(std::vector<std::string> sql_token);
	~SQLUpdate();
	std::string GetTableName();
	void SetTableName(std::string table_name);

	std::vector<std::string> GetNewField();
	void SetNewField(std::vector<std::string> new_fields);

	std::vector<Value> GetNewValue();
	void SetNewValue(std::vector<Value> new_values);

	std::string GetWhereField();
	void SetWhereField(std::string where_field);

	Value GetWhereValue();
	void SetWhereValue(Value where_value);

	void Parse(std::vector<std::string> sql_token) override;
private:
	std::string table_name_;
	std::vector<std::string> new_fields_;
	std::vector<Value> new_values_;
	std::string where_field_;
	Value * where_value_;
};

/* "SELECET FROM tablename WHERE field op value" */
class SQLSelect :public SQLBase
{
public:
	SQLSelect(std::vector<std::string> sql_token);
	~SQLSelect();
	std::string GetTableName();
	void SetTableName(std::string table_name);
	std::string GetField();
	void SetField(std::string field);
	Value GetValue();
	void SetValue(Value value);
	void Parse(std::vector<std::string> sql_token) override;
private:
	std::string table_name_;
	std::string field_;
	Value * value_;
	OperatorType op_;
};

/* CREATE INDEX indexname ON tablename(fieldname) */
class SQLCreateIndex :public SQLBase
{
public:
	SQLCreateIndex(std::vector<std::string> sql_token);
	~SQLCreateIndex();
	std::string GetTableName();
	void SetTableName(std::string table_name);
	std::string GetField();
	void SetField(std::string field);
	std::string GetIndex();
	void SetIndex(std::string index);
	void Parse(std::vector<std::string> sql_token) override;
private:
	std::string table_name_;
	std::string field_;
	std::string index_;
};

#endif
