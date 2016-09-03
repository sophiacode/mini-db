#include<iostream>
#include<algorithm>
#include "sqlstatement.h"

SQLBase::SQLBase()
{

}

void SQLBase::ToLower(std::string & str)
{
	std::transform(str.begin(), str.end(),
		str.begin(), tolower);
}

/* --------SQLCreateDataBase--------- */

SQLCreateDatabase::SQLCreateDatabase(std::vector<std::string> sql_token)
{
	Parse(sql_token);
}

SQLCreateDatabase::~SQLCreateDatabase()
{
}

std::string SQLCreateDatabase::GetDatabaseName()
{
	return database_name_;
}

void SQLCreateDatabase::SetDatabaseName(std::string database_name)
{
	database_name_ = database_name;
}

std::string SQLCreateDatabase::GetDatabasePath()
{
	return database_path_;
}

void SQLCreateDatabase::SetDatabasePath(std::string database_path)
{
	database_path_ = database_path;
}

void SQLCreateDatabase::Parse(std::vector<std::string> sql_token)
{
	if (sql_token.size() < 3) {
		std::cerr << "SyntaxError:Please input the name of the database." << std::endl;
	}

	database_name_ = sql_token[2];

	if (sql_token.size() == 4) {
		database_path_ = sql_token[3];
	} else {
		database_path_ = "Database\\";
	}
}

/* --------SQLUse--------- */

SQLUse::SQLUse(std::vector<std::string> sql_token)
{
	Parse(sql_token);
}

SQLUse::~SQLUse()
{
}

std::string SQLUse::GetDatabaseName()
{
	return database_name_;
}

void SQLUse::SetDatabaseName(std::string database_name)
{
	database_name_ = database_name;
}

std::string SQLUse::GetDatabasePath()
{
	return database_path_;
}

void SQLUse::SetDatabasePath(std::string database_path)
{
	database_path_ = database_path;
}

void SQLUse::Parse(std::vector<std::string> sql_token)
{
	if (sql_token.size() < 3) {
		std::cerr << "SyntaxError:Please input the name of the database." << std::endl;
		return;
	}

	database_name_ = sql_token[2];

	if (sql_token.size() == 4) {
		database_path_ = sql_token[3];
	} else {
		database_path_ = "Database\\";
	}
}

/* --------SQLCreateTable--------- */

SQLCreateTable::SQLCreateTable(std::vector<std::string> sql_token)
{
	Parse(sql_token);
}

SQLCreateTable::~SQLCreateTable()
{
}

std::string SQLCreateTable::GetTableName()
{
	return table_name_;
}

void SQLCreateTable::SetTableName(std::string table_name)
{
	table_name_ = table_name;
}

std::vector<Field> SQLCreateTable::GetFields()
{
	return fields_;
}

void SQLCreateTable::SetFields(std::vector<Field> fields)
{
	fields_ = fields;
}

void SQLCreateTable::Parse(std::vector<std::string> sql_token)
{
	if (sql_token.size() < 3 || sql_token[2] == "(") {
		std::cerr << "SyntaxError:Please input the name of the table." << std::endl;
		return;
	}

	table_name_ = sql_token[2];

	if (sql_token[3] == "(") {
		Value temp;
		int i = 4;
		while (sql_token[i] != ")") {
			std::string data;
			ValueType type;

			try {
				data = sql_token.at(i);

				if (sql_token.at(++i) == "int") {
					type = kIntegerType;
				}
				else if (sql_token.at(i) == "string") {
					type = kStringType;
				}
				else {
					std::cerr << "SyntaxError:Invalid Field." << std::endl;
				}

				temp.SetValue(data, type);

				if (sql_token.at(++i) == ",") {
					i++;
				}
				else if (sql_token.at(i) == ")") {
					break;
				}
				else {
					std::cerr << "SyntaxError:Invalid Field." << std::endl;
				}
			}
			catch (std::out_of_range) {
				std::cerr << "SyntaxError:Invalid Field." << std::endl;
			}

		}
	} else {
		std::cerr << "SynataxError:Invalid Field." << std::endl;
	}
}

/* --------SQLInsert--------- */

SQLInsert::SQLInsert(std::vector<std::string> sql_token)
{
	Parse(sql_token);
}

SQLInsert::~SQLInsert()
{
}

std::string SQLInsert::GetTableName()
{
	return table_name_;
}

void SQLInsert::SetTableName(std::string table_name)
{
	table_name_ = table_name;
}

std::vector<std::string> SQLInsert::GetFields()
{
	return fields_;
}

void SQLInsert::SetFields(std::vector<std::string> fields)
{
	fields_ = fields;
}

std::vector<Value> SQLInsert::GetValues()
{
	return values_;
}

void SQLInsert::SetValues(std::vector<Value> values)
{
	values_ = values;
}

bool SQLInsert::IsInputField()
{
	return is_input_field_;
}

void SQLInsert::Parse(std::vector<std::string> sql_token)
{
	int i = 1;
	try {
		ToLower(sql_token.at(i));
		if (sql_token.at(i) != "into") {
			std::cerr << "SyntaxError:Insert Error" << std::endl;
			return;
		}
	} catch (std::out_of_range) {
		std::cerr << "SyntaxError:Insert Error" << std::endl;
		return;
	}

	try {
		table_name_ = sql_token.at(++i);
	} catch (std::out_of_range) {
		std::cerr << "SyntaxError:Please input the name of the table." << std::endl;
		return;
	}

	try {
		ToLower(sql_token.at(++i));
		if (sql_token.at(i) == "value") {
			is_input_field_ = false;
			i++;
		} else if (sql_token.at(i) == "(") {
			while (sql_token.at(++i) != ")") {
				fields_.push_back(sql_token.at(i));
				if (sql_token.at(++i) == ",") {
					i++;
				} else if (sql_token.at(i) == ")") {
					break;
				} else {
					std::cerr << "SyntaxError:Invalid Field." << std::endl;
					return;
				}
			}
			is_input_field_ = true;

			ToLower(sql_token.at(++i));
			if (sql_token.at(i) == "Value") {
				i++;
			} else {
				std::cerr << "SyntaxError" << std::endl;
				return;
			}
		} else {
			std::cerr << "SyntaxError" << std::endl;
			return;
		}

		if (sql_token.at(i) == "(") {
			i++;
			while (sql_token.at(i) != ")") {
				Value temp;
				auto size = sql_token.at(i).size();
				if (*sql_token.at(i).begin() == '\''&&*sql_token.at(i).end() == '\'') {
					temp.SetValue(sql_token.at(i).substr(1, size - 1), kStringType);
				} else {
					temp.SetValue(sql_token.at(i).substr(1, size - 1), kIntegerType);
				}
				values_.push_back(temp);

				if (sql_token.at(++i) == ",") {
					i++;
				} else if (sql_token.at(++i) == ")") {
					break;
				} else {
					std::cerr << "SyntaxError" << std::endl;
					return;
				}
			}
		}
	} catch (std::out_of_range) {
		std::cerr << "SyntaxError" << std::endl;
	}
}

/* --------SQLDelete--------- */

SQLDelete::SQLDelete(std::vector<std::string> sql_token)
{
	Parse(sql_token);
}

SQLDelete::~SQLDelete()
{
}

std::string SQLDelete::GetTableName()
{
	return table_name_;
}

void SQLDelete::SetTableName(std::string table_name)
{
	table_name = table_name_;
}

std::string SQLDelete::GetField()
{
	return field_;
}

void SQLDelete::SetField(std::string field)
{
	field_ = field;
}

std::string SQLDelete::GetValue()
{
	return value_;
}

void SQLDelete::SetValue(std::string value)
{
	value_ = value;
}

void SQLDelete::Parse(std::vector<std::string> sql_token)
{
	int i = 1;
	try {
		ToLower(sql_token.at(i));
		if (sql_token.at(i) == "from") {
			table_name_ = sql_token.at(++i);
			i++;
		} else {
			std::cerr << "SyntaxError" << std::endl;
			return;
		}

		ToLower(sql_token.at(i));
		if(sql_token.at(i)=="where") {
			while (sql_token.at(i) != "=") {
				
			}
		}
	} catch(std::out_of_range) {
		std::cerr << "SyntaxError" << std::endl;
		return;
	}
}

/* --------SQLUpdate--------- */

SQLUpdate::SQLUpdate(std::vector<std::string> sql_token)
{
}

SQLUpdate::~SQLUpdate()
{
}

std::string SQLUpdate::GetTableName()
{
	return std::string();
}

void SQLUpdate::SetTableName(std::string table_name)
{
}

std::string SQLUpdate::GetNewField()
{
	return std::string();
}

void SQLUpdate::SetNewField(std::string new_field)
{
}

std::string SQLUpdate::GetNewValue()
{
	return std::string();
}

void SQLUpdate::SetNewValue(std::string new_value)
{
}

std::string SQLUpdate::GetWhereField()
{
	return std::string();
}

void SQLUpdate::SetWhereField(std::string where_field)
{
}

std::string SQLUpdate::GetWhereValue()
{
	return std::string();
}

void SQLUpdate::SetWhereValue(std::string where_value)
{
}

void SQLUpdate::Parse(std::vector<std::string> sql_token)
{
}

/* --------SQLSelect--------- */

SQLSelect::SQLSelect(std::vector<std::string> sql_token)
{
}

SQLSelect::~SQLSelect()
{
}

std::string SQLSelect::GetTableName()
{
	return std::string();
}

void SQLSelect::SetTableName(std::string table_name)
{
}

std::string SQLSelect::GetField()
{
	return std::string();
}

void SQLSelect::SetField(std::string field)
{
}

std::string SQLSelect::GetValue()
{
	return std::string();
}

void SQLSelect::SetValue(std::string value)
{
}

void SQLSelect::Parse(std::vector<std::string> sql_token)
{
}

/* --------SQLCreateIndex--------- */

SQLCreateIndex::SQLCreateIndex(std::vector<std::string> sql_token)
{
}

SQLCreateIndex::~SQLCreateIndex()
{
}

std::string SQLCreateIndex::GetTableName()
{
	return std::string();
}

void SQLCreateIndex::SetTableName(std::string table_name)
{
}

std::string SQLCreateIndex::GetField()
{
	return std::string();
}

void SQLCreateIndex::SetField(std::string field)
{
}

std::string SQLCreateIndex::GetIndex()
{
	return std::string();
}

void SQLCreateIndex::SetIndex(std::string index)
{
}

void SQLCreateIndex::Parse(std::vector<std::string> sql_token)
{
}
