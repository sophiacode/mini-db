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

void Interpreter::SQLInterpret(std::string sql_statement)
{
	sql_statement_ = sql_statement;
	SQLPretreatment();
	SplitSQL();
	GetSQLType();
	ParseSQL();
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

	//在( ) , = < > !左右添加空格
	rg = " ?(\\(|\\)|,|=|<|>|!|') ?";
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
		std::cerr << "SyntaxError:No Valid SQL Statement" << std::endl;
		return;
	}

	ToLower(sql_token_[0]);

	if (sql_token_[0] == "create")
	{
		if (sql_token_[0].size() < 2)
		{
			std::cerr << "SyntaxError:No Valid SQL Statement" << std::endl;
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
			std::cerr << "SyntaxError:No Valid SQL Statement" << std::endl;
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
	else
	{
		sql_type_ = kSQLUndefined;
		std::cerr << "SyntaxError:No Valid SQL Statement" << std::endl;
		return;
	}
}

//Todo:pass to controller
void Interpreter::ParseSQL()
{
	switch (sql_type_)
	{
	case kSQLCreateDatabase:
		SQLCreateDatabase * sql = new SQLCreateDatabase(sql_token_);
		break;
	case kSQLCreateTable:
		SQLCreateTable * sql= new SQLCreateTable(sql_token_);
		break;
	case kSQLCreateIndex:
		SQLCreateIndex * sql=new SQLCreateIndex(sql_token_);
		break;
	case kSQLUse:
		SQLUse * sql = new SQLUse(sql_token_);
		break;
	case kSQLInsert:
		SQLInsert * sql = new SQLInsert(sql_token_);
		break;
	case kSQLDelete:
		SQLDelete * sql = new SQLDelete(sql_token_);
		break;
	case kSQLUpdate:
		SQLUpdate * sql = new SQLUpdate(sql_token_);
		break;
	case kSQLSelect:
		SQLSelect * sql = new SQLSelect(sql_token_);
		break;
	}
}

void Interpreter::ToLower(std::string & str)
{
	std::transform(str.begin(), str.end(), 
		str.begin(), tolower);
}


