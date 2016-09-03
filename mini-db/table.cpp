#include<iostream>
#include<string>
#include<fstream>

#define record_num 1000

#include "database.h"
#include "table.h"
#include "sqlstatement.h"
#include "MemPool.h"
using namespace std;


/**
*  \brief ���캯��
*/
Table::Table() {}

/**
*  \brief ��������
*/
Table::~Table() {}

/**
*  \brief ��Ŀ������ɹ�����true��ʧ�ܷ���false
*/
bool Table::UseTable()
{
	std::string cmd = "start ..\\" + table_name;	/* ��������򿪱��ļ��� */
	if (system(cmd.c_str()))
	{
		return true;
	}
	else return false;
}

/**
*  \brief �����±�
*/
bool Table::CreateTable(SQLCreateTable &sql)
{
	table_name = sql.GetTableName();	/* ��ñ����� */
	std::string cmd = "md ..\\" + table_name;		/* ������������ļ��� */
	if (Table::UseTable())							/* �жϱ��Ƿ��Ѿ����� */
	{
		return false;
	}
	else {
		if (system(cmd.c_str()))
		{
			fields = sql.GetFields();				/* ��ȡ��ͷ���� */
			
			Table::UseTable();						/* �򿪱��ļ��� */
			fstream fp;
			std::string table_name_fields = table_name + "_fields";						/* ������ͷ�ļ���table_name_fields */
			fp.open(table_name_fields.c_str(), std::ios::binary | std::ios::out);		/* ������ͷ�ļ� */

			for (int i = 0; i < fields_num; i++)						/* д���ͷ���� */
			{
				std::string name = fields[i].GetFieldName() + '\0';
				ValueType type = fields[i].GetFieldType();
				std::string type_;
				if (type == kIntegerType) type_ = "0\0";				/* ���0�����Σ����1���ַ��� */
				else type_ = "1\0";
				fp.write(type_.c_str(), 2);								/* ��������ռ2λ���ֶ���20λ */
				fp.write(name.c_str(),20);
			}
			fp.close();													/* �ر��ļ� */
			return true;
		}
		else return false;
	}
}

/**
*  \brief �����¼�¼
*/
bool Table::CreateRecord(SQLInsert &st)
{
	if (Table::UseTable())
	{
		int Record_id = MemPool::NewNode();
		if (Record_id % record_num == 1)
		{
			int records_no = Record_id / record_num;
			fstream fp;
			std::string table_name_records = st.GetTableName() + "_records";
		}
	}
	else return false;
}