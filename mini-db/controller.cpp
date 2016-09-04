#include "controller.h"
#include <iostream>

Controller::Controller()
	:current_database_(nullptr)
{
}

Controller::~Controller()
{
	if (current_database_ != nullptr)
	{
		delete current_database_;
		current_database_ = nullptr;
	}
}

bool Controller::CreateDatabase(SQLCreateDatabase st)
{
	if (!st.IsParseSucceed())
	{
		return false;
	}
	current_database_ = new Database();
	if (!current_database_->CreateDatabase(st))
	{
		return true;
	}
}

bool Controller::CreateTable(SQLCreateTable st)
{
	if (current_database_ == nullptr)
	{
		std::cerr << "请先打开数据库" << std::endl;
		return false;
	}

	auto temp = current_database_->table_name;
	Table * t = new Table (st.GetTableName());
	t->CreateTable(st);
}


bool Controller::CreateIndex(SQLCreateIndex st)
{
	auto temp = current_database_-> table_name;
	for (auto iter = temp.begin();iter != temp.end();iter++)
	{
		if (iter->GetTableName() == st.GetTableName())
		{
			iter->CreateIndex(st);
			return true;
		}
	}
	return false;
}

bool Controller::Use(SQLUse st)
{
	string path = current_database_->UseDatabase(st);
	Table * t = new Table(path);
}

bool Controller::Insert(SQLInsert st)
{
}

bool Controller::Delete(SQLDelete st)
{
}

bool Controller::Update(SQLUpdate st)
{
}

bool Controller::Select(SQLSelect st)
{
}


