#ifndef _DATABASE_H_
#define _DATABASE_H_

#include<string>
#include<vector>
#include"sqlstatement.h"

/**
*  \brief ��������
*/
enum ValueType
{
	kIntegerType,
	kStringType,
	kNullType
};

/**
*  \brief ֵ��
*/
class Value
{
public:
	/**
	*  \brief ���캯��
	*/
	Value();

	/**
	*  \brief ��������
	*/
	~Value();

	/**
	*  \brief ��ȡValue��ֵ
	*/
	std::string GetValueData();

	/**
	*  \brief ��ȡValue������
	*/
	ValueType GetValueType();

	/**
	*  \brief ����Value��ֵ
	*/
	void SetValue(std::string new_data, ValueType new_type);

private:
	std::string value_data;      /* ���Value��ֵ */
	ValueType value_type;         /*  */
};

/**
*  \brief ��¼��
*/
class Record
{
public:
	/**
	*  \brief ���캯��
	*/
	Record();

	/**
	*  \brief ��������
	*/
	~Record();

	/**
	*  \brief ��ʾ��¼
	*/
	bool Display(std::vector<Value> values_data, std::vector<Field> fields);

	/**
	*  \brief ����Value��ֵ
	*/
	void SetValue(std::vector<Value> values_data);

private:
	int record_id;                /* ��¼���� */
	std::vector<Value> record_data;        /* ��¼���� */
};

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
*  \brief ������
*/
class Index
{
public:
	/**
	*  \brief ���캯��
	*/
	Index(std::string Field_name);

	/**
	*  \brief ��������
	*/
	~Index();

	/**
	*  \brief ������������
	*/
	bool AddIndex(std::string field_name);

private:
	BPlusTree bplustree;     /* ������B+�� */
	std::string field_name;  /* ������Ӧ���ֶ��� */
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

/**
*  \brief ���ݿ���
*/
class Database
{
public:
	/**
	*  \brief ���캯��
	*/
	Database(std::string Database_name);

	/**
	*  \brief ��������
	*/
	~Database();

	/**
	*  \brief �������ݿ�
	*/
	bool CreateDatabase(SQLCreateDatabase &st);

	/**
	*  \brief �����ݿ�,�����ص�ǰ���ݿ��·��
	*/
	std::string UseDatabase(SQLUse &st);


	/**
	*  \brief ��ȡ���ݿ�����
	*/
	std::string GetDatabaseName();

	/**
	*  \brief ��ȡ����
	*/
	std::vector<Table> GetTableName();

private:
	std::string database_name;          /* ���ݿ����� */
	std::vector<Table> table_name;      /* ���ݿ��б� */
};
#endif