#ifndef _TABLE_H_
#define _TABLE_H_

#include <string>
#include <vector>

#include "database.h"

/**
*  \brief �ֶ���
*/
class Field
{
public:
	/**
	*  \brief ���캯��
	*/
	Field(std::string Field_name);

	/**
	*  \brief ��������
	*/
	~Field();

	/**
	*  \brief ��ȡ�ֶ���
	*/
	std::string GetFieldName();

	/**
	*  \brief ��ȡ�ֶ���������
	*/
	ValueType GetFieldType();

	/**
	*  \brief �����ֶ���
	*/
	bool SetFieldName(std::string new_name);

	/**
	*  \brief �����ֶ���������
	*/
	bool SetFieldType(ValueType new_type);

private:
	std::string field_name;       /* �ֶ���  */
	ValueType field_type;         /* �ֶ��������� */
};

/**
*  \brief ����
*/
class Table
{
public:
	/**
	*  \brief ���캯��
	*/
	Table();

	/**
	*  \brief ��������
	*/
	~Table();

	/**
	*  \brief ������
	*/
	bool CreateTable(SQLCreateTable &sql);

	/**
	*  \brief �򿪱�
	*/
	bool UseTable();

	/**
	*  \brief ���Ӽ�¼
	*/
	bool CreateRecord(SQLInsert &st);

	/**
	*  \brief ���¼�¼
	*/
	bool UpdateRecord(SQLInsert &st);

	/**
	*  \brief ɾ����¼
	*/
	bool DeleteRecord(SQLInsert &st);


private:
	long long int record_num;           /* ���������������� */
	int fields_num;                     /* �����ֶ��� */
	std::string table_name;             /* ������ */
	std::vector<Record> records;        /* ��¼ */
	std::vector<Field> fields;          /* �ֶ� */
	std::vector<Index> indexs;          /* ���� */
};

#endif