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

void SQLBase::MergeValue(std::vector<std::string> & sql_token, int pos)
{
	try {
		sql_token.at(pos);
	} catch(std::out_of_range) {
		return;
	}

	auto iter = sql_token.begin() + pos;

	while(iter!=sql_token.end()) {
		if (*iter == "\'") {
			iter = sql_token.erase(iter);
			auto temp = iter + 1;
			while (temp != sql_token.end() && *temp != "\'") {
				*iter = *iter + " " + *temp;
				temp = sql_token.erase(temp);
			}
			temp = sql_token.erase(temp);

			*iter = "\'" + *iter + "\'";
			iter = temp;
			break;
		} else {
			iter++;
		}
	}
}

void SQLBase::ParseValue(std::string sql, Value & value)
{
	auto size = sql.size();
	if (*sql.begin() == '\''&&*sql.end() == '\'') {
		value.SetValue(sql.substr(1, size - 1), kStringType);
	}
	else {
		value.SetValue(sql.substr(1, size - 1), kIntegerType);
	}
}

OperatorType SQLBase::ParseOperator(std::string op)
{
	if (op == "=") {
		return kOpEqual;
	} else if (op == ">") {
		return kOpGreater;
	} else if (op == "<") {
		return kOpLess;
	} else if (op == "<=") {
		return kOpLessOrEqual;
	} else if (op == ">=") {
		return kOpGreterOrEqual;
	} else if (op == "!=") {
		return kOpNotEqual;
	} else {
		return kOpUndefined;
	}
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
		return;
	}

	database_name_ = sql_token[2];

	if (sql_token.size() >= 4) {
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

	if (sql_token.size() >= 4) {
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

	int i = 3;
	try {
		if (sql_token.at(i++) == "(") {
			//TODO
			//Field temp(); 

			while (sql_token.at(i) != ")") {
				std::string data;
				ValueType type;

				data = sql_token.at(i);

				ToLower(sql_token.at(++i));
				if (sql_token.at(i) == "int") {
					type = kIntegerType;
				}
				else if (sql_token.at(i) == "string") {
					type = kStringType;
				}
				else {
					std::cerr << "SyntaxError:Invalid Field." << std::endl;
					return;
				}

				//TODO
				//temp.SetValue(data, type); 
				//fields_.push_back(temp);

				if (sql_token.at(++i) == ",") {
					i++;
				}
				else if (sql_token.at(i) == ")") {
					break;
				}
				else {
					std::cerr << "SyntaxError:Invalid Field." << std::endl;
					return;
				}	
			}
		} else {
			std::cerr << "SynataxError:Invalid Field." << std::endl;
			return;
		}
	} catch (std::out_of_range) {
		std::cerr << "SyntaxError" << std::endl;
		return;
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
		if (sql_token.at(i) == "values") {
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
			if (sql_token.at(i) == "values") {
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
				MergeValue(sql_token, i);
				ParseValue(sql_token.at(i), temp);
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

Value SQLDelete::GetValue()
{
	return value_;
}

void SQLDelete::SetValue(Value value)
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
			field_ = sql_token.at(++i);
			if (sql_token.at(++i) == "=") {
				MergeValue(sql_token, ++i);
				ParseValue(sql_token.at(i), value_);
			} else {
				std::cerr << "SyntaxError" << std::endl;
				return;
			}
		} else {
			std::cerr << "SyntaxError" << std::endl;
		}
	} catch(std::out_of_range) {
		std::cerr << "SyntaxError" << std::endl;
		return;
	}
}

/* --------SQLUpdate--------- */

SQLUpdate::SQLUpdate(std::vector<std::string> sql_token)
{
	Parse(sql_token);
}

SQLUpdate::~SQLUpdate()
{
}

std::string SQLUpdate::GetTableName()
{
	return table_name_;
}

void SQLUpdate::SetTableName(std::string table_name)
{
	table_name_ = table_name;
}

std::vector<std::string> SQLUpdate::GetNewField()
{
	return new_fields_;
}

void SQLUpdate::SetNewField(std::vector<std::string> new_fields)
{
	new_fields_ = new_fields;
}

std::vector<Value> SQLUpdate::GetNewValue()
{
	return new_values_;
}

void SQLUpdate::SetNewValue(std::vector<Value> new_values)
{
	new_values_ = new_values;
}

std::string SQLUpdate::GetWhereField()
{
	return where_field_;
}

void SQLUpdate::SetWhereField(std::string where_field)
{
	where_field_ = where_field;
}

Value SQLUpdate::GetWhereValue()
{
	return where_value_;
}

void SQLUpdate::SetWhereValue(Value where_value)
{
	where_value_ = where_value;
}

void SQLUpdate::Parse(std::vector<std::string> sql_token)
{
	int i = 1;
	try {
		table_name_ = sql_token.at(i);
		ToLower(sql_token.at(++i));
		if (sql_token.at(i) == "set") {
			while (true) {
				std::string temp_field;
				Value temp_value;

				temp_field = sql_token.at(++i);
				new_fields_.push_back(temp_field);
				if (sql_token.at(++i) == "=") {
					MergeValue(sql_token, ++i);
					ParseValue(sql_token.at(i), temp_value);
					new_values_.push_back(temp_value);
				} else {
					std::cerr << "SyntaxError" << std::endl;
					return;
				}

				ToLower(sql_token.at(++i));
				if (sql_token.at(i) == ",") {
					++i;
				} else if (sql_token.at(i) == "where") {
					break;
				} else {
					std::cerr << "SyntaxError" << std::endl;
					return;
				}		
			} 
			where_field_ = sql_token.at(++i);
			if (sql_token.at(++i) == "=") {
				MergeValue(sql_token, ++i);
				ParseValue(sql_token.at(i), where_value_);
			} else {
				std::cerr << "SyntaxError" << std::endl;
				return;
			}
		} else {
			std::cerr << "SyntaxError" << std::endl;
			return;
		}
	} catch (std::out_of_range) {
		std::cerr << "SyntaxError" << std::endl;
		return;
	}
}

/* --------SQLSelect--------- */

SQLSelect::SQLSelect(std::vector<std::string> sql_token)
{
	Parse(sql_token);
}

SQLSelect::~SQLSelect()
{
}

std::string SQLSelect::GetTableName()
{
	return table_name_;
}

void SQLSelect::SetTableName(std::string table_name)
{
	table_name_ = table_name;
}

std::string SQLSelect::GetField()
{
	return field_;
}

void SQLSelect::SetField(std::string field)
{
	field_ = field;
}

Value SQLSelect::GetValue()
{
	return value_;
}

void SQLSelect::SetValue(Value value)
{
	value_ = value;
}

void SQLSelect::Parse(std::vector<std::string> sql_token)
{
	int i = 1;
	try {
		ToLower(sql_token.at(i));
		if (sql_token.at(i) == "from") {
			table_name_ = sql_token.at(++i);
		} else {
			std::cerr << "SyntaxError" << std::endl;
			return;
		}

		ToLower(sql_token.at(++i));
		if (sql_token.at(i) == "where") {
			field_ = sql_token.at(++i);
			if ((op_ = ParseOperator(sql_token.at(++i))) == kOpUndefined) {
				std::cerr << "SyntaxError" << std::endl;
			}
			MergeValue(sql_token,++i);
			ParseValue(sql_token.at(i), value_);
		}
	} catch(std::out_of_range) {
		std::cerr << "SyntaxError" << std::endl;
		return;
	}
}

/* --------SQLCreateIndex--------- */

SQLCreateIndex::SQLCreateIndex(std::vector<std::string> sql_token)
{
	Parse(sql_token);
}

SQLCreateIndex::~SQLCreateIndex()
{
}

std::string SQLCreateIndex::GetTableName()
{
	return table_name_;
}

void SQLCreateIndex::SetTableName(std::string table_name)
{
	table_name_ = table_name;
}

std::string SQLCreateIndex::GetField()
{
	return field_;
}

void SQLCreateIndex::SetField(std::string field)
{
	field_ = field;
}

std::string SQLCreateIndex::GetIndex()
{
	return index_;
}

void SQLCreateIndex::SetIndex(std::string index)
{
	index_ = index;
}

void SQLCreateIndex::Parse(std::vector<std::string> sql_token)
{
	int i = 2;
	try {
		index_ = sql_token.at(i);
		
		ToLower(sql_token.at(++i));
		if (sql_token.at(i) == "on") {
			table_name_ = sql_token.at(++i);
			if (sql_token.at(i) == "(" && sql_token.at(i+2) == ")" ) {
				field_ = sql_token.at(++i);
			} else {
				std::cerr << "SyntaxError" << std::endl;
				return;
			}
		} else {
			std::cerr << "SyntaxError" << std::endl;
			return;
		}
	} catch (std::out_of_range) {
		std::cerr << "SyntaxError" << std::endl;
		return;
	}
}
