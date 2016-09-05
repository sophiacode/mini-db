#pragma once

#include"sqlstatement.h"
#include"database.h"
#include"table.h"

class Controller
{
public:
	Controller();
	~Controller();
	bool CreateDatabase(SQLCreateDatabase * st);
	bool CreateTable(SQLCreateTable * st);
	bool CreateIndex(SQLCreateIndex * st);
	bool Use(SQLUse * st);
	bool Insert(SQLInsert * st);
	bool Delete(SQLDelete * st);
	bool Update(SQLUpdate * st);
	bool Select(SQLSelect * st);
private:
	std::vector<Database *> databases_; /* 创建的数据库 */
	Database * current_database_;     /* 当前使用的数据库 */
	std::string path_;                /* 当前使用的数据库所在路径 */
};