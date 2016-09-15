#include<regex>
#include<iostream>

#include "interpreter.h"
#include "sqlstatement.h"

Interpreter::Interpreter(Controller * controller)
	:sql_type_(kSQLUndefined),
	controller_(controller)
{

}

Interpreter::~Interpreter()
{
	sql_type_ = kSQLUndefined;
	controller_ = nullptr;
	sql_statement_.clear();
	sql_token_.clear();
}

bool Interpreter::SQLInterpret(std::string sql_statement)
{
	sql_statement_ = sql_statement;
	SQLPretreatment();
	SplitSQL();
	GetSQLType();
	ParseSQL();
	if (sql_type_ == kSQLQuit)
	{
		return false;
	}
	return true;
}

void Interpreter::SQLPretreatment()
{
	//去除换行与tab
	std::regex rg("[\r\n\t]");
	sql_statement_ = std::regex_replace(sql_statement_, rg, " ");

	//去除分号及其后的内容
	rg = ";.*$";
	sql_statement_ = std::regex_replace(sql_statement_, rg, "");

	//去除命令开头与最后的空格
	rg = "(^ +)|( +$)";
	sql_statement_ = std::regex_replace(sql_statement_, rg, "");

	//在( ) , = < > ! \左右添加空格
	rg = " ?(\\(|\\)|,|=|<|>|!|'|\\\\) ?";
	sql_statement_ = std::regex_replace(sql_statement_, rg, " $1 ");

	//去除单词间一个以上的空格
	rg = " +";
	sql_statement_ = std::regex_replace(sql_statement_, rg, " ");

	//将<= >= !=合并为一个token
	rg = "< *=";
	sql_statement_ = std::regex_replace(sql_statement_, rg, "<=");
	rg = "> *=";
	sql_statement_ = std::regex_replace(sql_statement_, rg, ">=");
	rg = "! *=";
	sql_statement_ = std::regex_replace(sql_statement_, rg, "!=");
}

void Interpreter::SplitSQL()
{
	char * c_sql = const_cast<char *>(sql_statement_.c_str());
	char * next = std::strtok(c_sql, " ");

	while (next != nullptr)
	{
		sql_token_.push_back(next);
		next = std::strtok(nullptr, " ");
	}
}

void Interpreter::GetSQLType()
{
	if (sql_token_.size() == 0)
	{
		std::cerr << "未识别到有效的SQL语句." << std::endl;
		return;
	}

	ToLower(sql_token_[0]);

	if (sql_token_[0] == "create")
	{
		if (sql_token_[0].size() < 2)
		{
			sql_type_ = kSQLUndefined;
			std::cerr << "未识别到有效的SQL语句." << std::endl;
			return;
		}
		ToLower(sql_token_[1]);
		if (sql_token_[1] == "database")
		{
			sql_type_ = kSQLCreateDatabase;
		}
		else if (sql_token_[1] == "table")
		{
			sql_type_ = kSQLCreateTable;
		}
		else if (sql_token_[1] == "index")
		{
			sql_type_ = kSQLCreateIndex;
		}
		else
		{
			sql_type_ = kSQLUndefined;
			std::cerr << "未识别到有效的SQL语句." << std::endl;
			return;
		}
	}
	else if (sql_token_[0] == "use")
	{
		sql_type_ = kSQLUse;
	}
	else if (sql_token_[0] == "insert")
	{
		sql_type_ = kSQLInsert;
	}
	else if (sql_token_[0] == "delete")
	{
		sql_type_ = kSQLDelete;
	}
	else if (sql_token_[0] == "update")
	{
		sql_type_ = kSQLUpdate;
	}
	else if (sql_token_[0] == "select")
	{
		sql_type_ = kSQLSelect;
	}
	else if (sql_token_[0] == "quit")
	{
		sql_type_ = kSQLQuit;
	}
	else
	{
		sql_type_ = kSQLUndefined;
		std::cerr << "未识别到有效的SQL语句." << std::endl;
		return;
	}
}

bool Interpreter::ParseSQL()
{
	SQLBase * sql;
	switch (sql_type_)
	{
	case kSQLCreateDatabase:
		return controller_->
			CreateDatabase(new SQLCreateDatabase(sql_token_));
	case kSQLCreateTable:
		return controller_->
			CreateTable(new SQLCreateTable(sql_token_));
	case kSQLCreateIndex:
		return controller_->
			CreateIndex(new SQLCreateIndex(sql_token_));
	case kSQLUse:
		return controller_->Use(new SQLUse(sql_token_));
	case kSQLInsert:
		return controller_->Insert(new SQLInsert(sql_token_));
	case kSQLDelete:
		return controller_->Delete(new SQLDelete(sql_token_));
	case kSQLUpdate:
		return controller_->Update(new SQLUpdate(sql_token_));
	case kSQLSelect:
		return controller_->Select(new SQLSelect(sql_token_));
	case kSQLQuit:
		return true;
	case kSQLUndefined:
		return false;
	default:
		return false;
	}
}

void Interpreter::ToLower(std::string & str)
{
	std::transform(str.begin(), str.end(), 
		str.begin(), tolower);
}


