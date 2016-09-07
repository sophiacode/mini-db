#include "controller.h"
#include <iostream>

Controller::Controller()
	:current_database_(nullptr)
{
	databases_.clear();
	path_.clear();
	is_use_database_ = false;
}

Controller::~Controller()
{
	for (auto iter = databases_.begin();iter != databases_.end();iter++) {
		if (*iter != nullptr) {
			delete (*iter);
			(*iter) = nullptr;
		}
	}

	if (current_database_ != nullptr) {
		delete current_database_;
		current_database_ = nullptr;
	}

	path_.clear();
	is_use_database_ = true;
}

bool Controller::CreateDatabase(SQLCreateDatabase *st)
{
	if (!st->IsParseSucceed())
	{
		return false;
	}

	/*Database * db = new Database();
	if (db->CreateDatabase(*st))
	{
		databases_.push_back(db);
		return true;
	}
	return false;*/

	Database db;
	return db.CreateDatabase(*st);
}

bool Controller::CreateTable(SQLCreateTable *st)
{
	if (!st->IsParseSucceed())
	{
		return false;
	}

	if (!is_use_database_)
	{
		std::cerr << "请先打开数据库." << std::endl;
		return false;
	}

	/*auto tables = current_database_->GetTableName();
	Table * table = new Table(path_);
	return table->CreateTable(*st);*/

	/*Table table(path_);
	return table.CreateTable(*st);*/

	return current_database_->CreateTable(*st);
}


bool Controller::CreateIndex(SQLCreateIndex *st)
{
	if (!st->IsParseSucceed())
	{
		return false;
	}

	if (!is_use_database_)
	{
		std::cerr << "请先打开数据库." << std::endl;
		return false;
	}

	auto tables = current_database_->GetTableName();
	for (auto iter = tables.begin();iter != tables.end();iter++)
	{
		if (iter->GetTableName() == st->GetTableName())
		{
			return (*iter).CreateIndex(*st);
		}
	}

	std::cerr << "表" << st->GetTableName() << "不存在." << std::endl;
	return false;

	

	/*Table table(path_);
	return table.CreateIndex(*st);*/
}

bool Controller::Use(SQLUse *st)
{
	if (!st->IsParseSucceed())
	{
		return false;
	}

	/*for (auto iter = databases_.begin();iter != databases_.end();iter++)
	{
		if ((*iter)->GetDatabaseName() == st->GetDatabaseName())
		{
			current_database_ = *iter;
			if ((path_ = current_database_->UseDatabase(*st)).empty())
			{
				return false;
			}
			is_use_database_ = true;
			return true;
		}
	}*/

	/*Database db;
	path_ = db.UseDatabase(*st);

	if (path_.empty())
	{
		std::cerr << "数据库" << st->GetDatabaseName() << "不存在." << std::endl;
		is_use_database_ = false;
		return false;
	}

	is_use_database_ = true;
	return true;*/

	if (current_database_ != nullptr)
	{
		delete current_database_;
		current_database_ = nullptr;
	}
	current_database_ = new Database;
	path_ = current_database_->UseDatabase(*st);
	if (path_.empty())
	{
		return false;
	}
	is_use_database_ = true;
	return true;
}

bool Controller::Insert(SQLInsert *st)
{
	if (!st->IsParseSucceed())
	{
		return false;
	}

	if (!is_use_database_)
	{
		std::cerr << "请先打开数据库." << std::endl;
		return false;
	}

	auto tables = current_database_->GetTableName();
	for (auto iter = tables.begin();iter != tables.end();iter++)
	{
		if (iter->GetTableName() == st->GetTableName())
		{
			return (*iter).CreateRecord(*st);
		}
	}

	std::cerr << "表" << st->GetTableName() << "不存在." << std::endl;
	return false;

	//Table table(path_);
	//return table.CreateRecord(*st);
}

bool Controller::Delete(SQLDelete *st)
{
	if (!st->IsParseSucceed())
	{
		return false;
	}

	if (!is_use_database_)
	{
		std::cerr << "请先打开数据库." << std::endl;
		return false;
	}

	auto tables = current_database_->GetTableName();
	for (auto iter = tables.begin();iter != tables.end();iter++)
	{
		if (iter->GetTableName() == st->GetTableName())
		{
			return (*iter).DeleteRecord(*st);
		}
	}

	std::cerr << "表" << st->GetTableName() << "不存在." << std::endl;
	return false;

	

	/*Table table(path_);
	return table.DeleteRecord(*st);*/
}

bool Controller::Update(SQLUpdate *st)
{
	if (!st->IsParseSucceed())
	{
		return false;
	}


	if (!is_use_database_)
	{
		std::cerr << "请先打开数据库." << std::endl;
		return false;
	}

	auto tables = current_database_->GetTableName();
	for (auto iter = tables.begin();iter != tables.end();iter++)
	{
		if (iter->GetTableName() == st->GetTableName())
		{
			return (*iter).UpdateRecord(*st);
		}
	}

	std::cerr << "表" << st->GetTableName() << "不存在." << std::endl;
	return false;


	/*Table table(path_);
	return table.UpdateRecord(*st);*/
}

bool Controller::Select(SQLSelect *st)
{
	if (!st->IsParseSucceed())
	{
		return false;
	}

	if (!is_use_database_)
	{
		std::cerr << "请先打开数据库." << std::endl;
		return false;
	}

	auto tables = current_database_->GetTableName();
	for (auto iter = tables.begin();iter != tables.end();iter++)
	{
		if (iter->GetTableName() == st->GetTableName())
		{
			return (*iter).SelectRecord(*st);
		}
	}

	std::cerr << "表" << st->GetTableName() << "不存在." << std::endl;
	return false;

	

	/*Table table(path_);
	return table.SelectRecord(*st);*/
}


