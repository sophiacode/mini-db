#pragma once

#include"sqlstatement.h"
#include"database.h"
#include"table.h"

class Controller
{
public:
	Controller();
	~Controller();
	bool CreateDatabase(SQLCreateDatabase st);
	bool CreateTable(SQLCreateTable st);
	bool CreateIndex(SQLCreateIndex st);
	bool Use(SQLUse st);
	bool Insert(SQLInsert st);
	bool Delete(SQLDelete st);
	bool Update(SQLUpdate st);
	bool Select(SQLSelect st);
private:
	Database * current_database_;     /* 当前使用的数据库 */
};