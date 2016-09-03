#ifndef _TABLE_H_
#define _TABLE_H_

#include <string>
#include <vector>

#define record_num 1000

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
	Field();

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
	Table(std::string nem_table_name);

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
	bool UpdateRecord(SQLInsert &su);

	/**
	*  \brief ɾ����¼
	*/
	bool DeleteRecord(SQLDelete &sd);

	/**
	*  \brief ͨ��SQLֱ�Ӳ��Ҽ�¼
	*/
	bool SelectRecord(SQLSelect &sql);

	/**
	*  \brief Delete����Select����
	*/
	bool SelectRecord(SQLDelete &sd);

	/**
	*  \brief Update����Select����
	*/
	bool SelectRecord(SQLUpdate &su);

private:
	int records_num;				     /* ���������������� */
	std::string table_name;				 /* ������ */
	Record record;						 /* ��¼ */
	std::vector<Field> fields;           /* �ֶ� */
	std::vector<Index> indexs;           /* ���� */
	std::vector<int> select_id;			 /* ��¼selectѡ�е�id */
};

#endif