#include<iostream>
#include<algorithm>
#include "sqlstatement.h"

SQLBase::SQLBase()
	:is_parse_succeed(false)
{

}

SQLBase::~SQLBase()
{
	is_parse_succeed = false;
}

bool SQLBase::IsParseSucceed()
{
	return is_parse_succeed;
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

bool SQLBase::ParseValue(std::string sql, Value & value)
{
	auto size = sql.size();
	if (sql.at(0) == '\''&&sql.at(size - 1) == '\'') {
		value.SetValue(sql.substr(1, size - 2), kStringType);
		return true;
	}
	else {
		value.SetValue(sql, kIntegerType);
		for (auto iter = sql.begin();iter != sql.end();iter++) {
			if (*iter > '9' || *iter < '0')
				return false;
		}
		return true;
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
	database_name_.clear();
	database_path_.clear();
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
		std::cerr << "Create Database ÃüÁîÓï·¨´íÎó." << std::endl;
		return;
	}

	database_name_ = sql_token[2];

	if (sql_token.size() == 4) {
		database_path_ = sql_token[3];
	}
	else if (sql_token.size() < 4) {
		database_path_ = ".\\Database";
	}
	else {
		std::cerr << "Create Database ÃüÁîÓï·¨´íÎó." << std::endl;
		return;
	}

	is_parse_succeed = true;
}

/* --------SQLUse--------- */

SQLUse::SQLUse(std::vector<std::string> sql_token)
{
	Parse(sql_token);
}

SQLUse::~SQLUse()
{
	database_name_.clear();
	database_path_.clear();
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
	if (sql_token.size() < 2) {
		std::cerr << "Use ÃüÁîÓï·¨´íÎó." << std::endl;
		return;
	}

	database_name_ = sql_token[1];

	if (sql_token.size() == 3) {
		database_path_ = sql_token[2];
	}
	else if (sql_token.size() < 3) {
		database_path_ = ".\\Database";
	}
	else {
		std::cerr << "Use ÃüÁîÓï·¨´íÎó." << std::endl;
		return;
	}

	is_parse_succeed = true;
}

/* --------SQLCreateTable--------- */

SQLCreateTable::SQLCreateTable(std::vector<std::string> sql_token)
{
	Parse(sql_token);
}

SQLCreateTable::~SQLCreateTable()
{
	table_name_.clear();
	fields_.clear();
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
		std::cerr << "Create Table ÃüÁîÓï·¨´íÎó." << std::endl;
		return;
	}

	table_name_ = sql_token[2];

	int i = 3;
	try {
		if (sql_token.at(i++) == "(") {
			Field temp; 

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
					std::cerr << "Create Table ÃüÁîÓï·¨´íÎó." << std::endl;
					return;
				}

				temp.SetFieldName(data);
				temp.SetFieldType(type); 
				fields_.push_back(temp);

				if (sql_token.at(++i) == ",") {
					i++;
				}
				else if (sql_token.at(i) == ")") {
					break;
				}
				else {
					std::cerr << "Create Table ÃüÁîÓï·¨´íÎó." << std::endl;
					return;
				}	
			}

			if (++i < sql_token.size()) {
				std::cerr << "Create Table ÃüÁîÓï·¨´íÎó." << std::endl;
				return;
			}
		} else {
			std::cerr << "Create Table ÃüÁîÓï·¨´íÎó." << std::endl;
			return;
		}
	} catch (std::out_of_range) {
		std::cerr << "Create Table ÃüÁîÓï·¨´íÎó." << std::endl;
		return;
	}

	is_parse_succeed = true;
}

/* --------SQLInsert--------- */

SQLInsert::SQLInsert(std::vector<std::string> sql_token)
{
	Parse(sql_token);
}

SQLInsert::~SQLInsert()
{
	table_name_.clear();
	fields_.clear();
	values_.clear();
	is_input_field_ = false;
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
			std::cerr << "Insert ÃüÁîÓï·¨´íÎó." << std::endl;
			return;
		}
	} catch (std::out_of_range) {
		std::cerr << "Insert ÃüÁîÓï·¨´íÎó." << std::endl;
		return;
	}

	try {
		table_name_ = sql_token.at(++i);
	} catch (std::out_of_range) {
		std::cerr << "Insert ÃüÁîÓï·¨´íÎó." << std::endl;
		return;
	}

	try {
		ToLower(sql_token.at(++i));
		if (sql_token.at(i) == "values") {
			is_input_field_ = false;
			i++;
		} else if (sql_token.at(i++) == "(") {
			while (sql_token.at(i) != ")") {
				fields_.push_back(sql_token.at(i));
				if (sql_token.at(++i) == ",") {
					i++;
				} else if (sql_token.at(i) == ")") {
					break;
				} else {
					std::cerr << "Insert ÃüÁîÓï·¨´íÎó." << std::endl;
					return;
				}
			}
			is_input_field_ = true;

			ToLower(sql_token.at(++i));
			if (sql_token.at(i) == "values") {
				i++;
			} else {
				std::cerr << "Insert ÃüÁîÓï·¨´íÎó." << std::endl;
				return;
			}
		} else {
			std::cerr << "Insert ÃüÁîÓï·¨´íÎó." << std::endl;
			return;
		}

		if (sql_token.at(i) == "(") {
			i++;
			while (true) {
				Value temp;
				if (sql_token.at(i) == "," || sql_token.at(i) == ")")
				{
					temp.SetValue("", kNullType);
					values_.push_back(temp);
				}
				else {
					MergeValue(sql_token, i);
					if(ParseValue(sql_token.at(i), temp) == false) {
						std::cerr << "InsertÊ§°Ü£ºÖµÓë×Ö¶ÎµÄÀàÐÍ²»Æ¥Åä." << std::endl;
						return;
					}
					values_.push_back(temp);
					i++;
				}

				if (sql_token.at(i) == ",") {
					i++;
				}
				else if (sql_token.at(i) == ")") {
					break;
				}
				else {
					std::cerr << "Insert ÃüÁîÓï·¨´íÎó." << std::endl;
					return;
				}
			}

			if (++i < sql_token.size()) {
				std::cerr << "Insert ÃüÁîÓï·¨´íÎó." << std::endl;
				return;
			}
		}
	} catch (std::out_of_range) {
		std::cerr << "Insert ÃüÁîÓï·¨´íÎó." << std::endl;
		return;
	}

	if (is_input_field_ == true
		&& fields_.size() != values_.size()) {
		std::cerr << "InsertÊ§°Ü£º×Ö¶ÎÓëÖµµÄÊýÁ¿²»Í¬." << std::endl;
		return;
	}

	is_parse_succeed = true;
}


/* --------SQLDelete--------- */

SQLDelete::SQLDelete(std::vector<std::string> sql_token)
{
	value_ = new Value;
	is_input_where_ = false;
	Parse(sql_token);
}

SQLDelete::~SQLDelete()
{
	table_name_.clear();
	field_.clear();
	is_input_where_ = false;
	if (value_ != nullptr)
	{
		delete value_;
		value_ = nullptr;
	}
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
	return *value_;
}

void SQLDelete::SetValue(Value value)
{
	value_ = &value;
}

bool SQLDelete::IsInputWhere()
{
	return is_input_where_;
}

void SQLDelete::Parse(std::vector<std::string> sql_token)
{
	int i = 1;
	try {
		ToLower(sql_token.at(i));
		if (sql_token.at(i) == "from") {
			table_name_ = sql_token.at(++i);
			i++;
		}
		else {
			std::cerr << "Delete ÃüÁîÓï·¨´íÎó." << std::endl;
			return;
		}
	}
	catch (std::out_of_range) {
		std::cerr << "Delete ÃüÁîÓï·¨´íÎó." << std::endl;
		return;
	}

	if (i >= sql_token.size())
	{
		is_input_where_ = false;
	}
	else
	{
		try {
			ToLower(sql_token.at(i));
			if (sql_token.at(i) == "where") {
				field_ = sql_token.at(++i);
				if (sql_token.at(++i) == "=") {
					MergeValue(sql_token, ++i);
					if (ParseValue(sql_token.at(i), *value_) == false) {
						std::cerr << "DeleteÊ§°Ü£ºÖµÓë×Ö¶ÎµÄÀàÐÍ²»Æ¥Åä." << std::endl;
						return;
					}

					if (++i < sql_token.size()) {
						std::cerr << "Delete ÃüÁîÓï·¨´íÎó." << std::endl;
						return;
					}
				}
				else {
					std::cerr << "Delete ÃüÁîÓï·¨´íÎó." << std::endl;
					return;
				}
				is_input_where_ = true;
			}
			else {
				std::cerr << "Delete ÃüÁîÓï·¨´íÎó." << std::endl;
				return;
			}
		}
		catch (std::out_of_range) {
			std::cerr << "Delete ÃüÁîÓï·¨´íÎó." << std::endl;
			return;
		}
	}

	is_parse_succeed = true;
}

/* --------SQLUpdate--------- */

SQLUpdate::SQLUpdate(std::vector<std::string> sql_token)
{
	where_value_ = new Value;
	Parse(sql_token);
}

SQLUpdate::~SQLUpdate()
{
	table_name_.clear();
	new_fields_.clear();
	new_values_.clear();
	where_field_.clear();

	if (where_value_ != nullptr)
	{
		delete where_value_;
		where_value_ = nullptr;
	}
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
	return *where_value_;
}

void SQLUpdate::SetWhereValue(Value where_value)
{
	where_value_ = &where_value;
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
					if (ParseValue(sql_token.at(i), temp_value) == false) {
						std::cerr << "UpdateÊ§°Ü£ºset×ÓÓï¾äÖÐµÄÖµÓë×Ö¶ÎµÄÀàÐÍ²»Æ¥Åä." << std::endl;
						return;
					}
					new_values_.push_back(temp_value);
				} else {
					std::cerr << "Update ÃüÁîÓï·¨´íÎó." << std::endl;
					return;
				}

				ToLower(sql_token.at(++i));
				if (sql_token.at(i) == ",") {
					continue;
				} else if (sql_token.at(i) == "where") {
					break;
				} else {
					std::cerr << "Update ÃüÁîÓï·¨´íÎó." << std::endl;
					return;
				}		
			} 
			where_field_ = sql_token.at(++i);
			if (sql_token.at(++i) == "=") {
				MergeValue(sql_token, ++i);
				if (ParseValue(sql_token.at(i), *where_value_) == false) {
					std::cerr << "UpdateÊ§°Ü£ºwhere×ÓÓï¾äÖÐµÄÖµÓë×Ö¶ÎµÄÀàÐÍ²»Æ¥Åä." << std::endl;
					return;
				}

				if (++i < sql_token.size()) {
					std::cerr << "Update ÃüÁîÓï·¨´íÎó." << std::endl;
					return;
				}
			} else {
				std::cerr << "Update ÃüÁîÓï·¨´íÎó." << std::endl;
				return;
			}
		} else {
			std::cerr << "Update ÃüÁîÓï·¨´íÎó." << std::endl;
			return;
		}
	} catch (std::out_of_range) {
		std::cerr << "Update ÃüÁîÓï·¨´íÎó." << std::endl;
		return;
	}

	is_parse_succeed = true;
}

/* --------SQLSelect--------- */

SQLSelect::SQLSelect(std::vector<std::string> sql_token)
{
	value_ = new Value();
	is_input_where_ = false;
	Parse(sql_token);
}

SQLSelect::~SQLSelect()
{
	table_name_.clear();
	field_.clear();
	op_ = kOpUndefined;
	if (value_ != nullptr)
	{
		delete value_;
		value_ = nullptr;
	}
}

OperatorType SQLSelect::GetOperatorType()
{
	return op_;
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
	return *value_;
}

void SQLSelect::SetValue(Value value)
{
	value_ = &value;
}

bool SQLSelect::IsInputWhere()
{
	return is_input_where_;
}

void SQLSelect::Parse(std::vector<std::string> sql_token)
{
	int i = 1;
	try {
		ToLower(sql_token.at(i));
		if (sql_token.at(i) == "from") {
			table_name_ = sql_token.at(++i);
		} else {
			std::cerr << "Select ÃüÁîÓï·¨´íÎó." << std::endl;
			return;
		}

		if (++i >= sql_token.size())
		{
			is_input_where_ = false;
			is_parse_succeed = true;
			return;
		}
		ToLower(sql_token.at(i));
		if (sql_token.at(i) == "where") {
			field_ = sql_token.at(++i);
			if ((op_ = ParseOperator(sql_token.at(++i))) == kOpUndefined) {
				std::cerr << "Select ÃüÁîÓï·¨´íÎó." << std::endl;
			}
			MergeValue(sql_token,++i);
			if (ParseValue(sql_token.at(i), *value_) == false) {
				std::cerr << "SelectÊ§°Ü£ºÖµÓë×Ö¶ÎµÄÀàÐÍ²»Æ¥Åä." << std::endl;
				return;
			}

			if (++i < sql_token.size()) {
				std::cerr << "Select ÃüÁîÓï·¨´íÎó." << std::endl;
				return;
			}

			is_input_where_ = true;
		}
	} catch(std::out_of_range) {
		std::cerr << "Select ÃüÁîÓï·¨´íÎó." << std::endl;
		return;
	}

	is_parse_succeed = true;
}

/* --------SQLCreateIndex--------- */

SQLCreateIndex::SQLCreateIndex(std::vector<std::string> sql_token)
{
	Parse(sql_token);
}

SQLCreateIndex::~SQLCreateIndex()
{
	table_name_.clear();
	field_.clear();
	index_.clear();
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
			i++;
			if (sql_token.at(i) == "(" && sql_token.at(i+2) == ")" ) {
				field_ = sql_token.at(++i);
			} else {
				std::cerr << "Create Index ÃüÁîÓï·¨´íÎó." << std::endl;
				return;
			}

			if (i+2 < sql_token.size()) {
				std::cerr << "Create Index ÃüÁîÓï·¨´íÎó." << std::endl;
				return;
			}
		} else {
			std::cerr << "Create Index ÃüÁîÓï·¨´íÎó." << std::endl;
			return;
		}
	} catch (std::out_of_range) {
		std::cerr << "Create Index ÃüÁîÓï·¨´íÎó." << std::endl;
		return;
	}

	is_parse_succeed = true;
}
