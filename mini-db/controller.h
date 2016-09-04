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
	std::vector<Database *> databases_; /* ���������ݿ� */
	Database * current_database_;     /* ��ǰʹ�õ����ݿ� */
	std::string path_;                /* ��ǰʹ�õ����ݿ�����·�� */
};