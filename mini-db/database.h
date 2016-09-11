#ifndef _DATABASE_H_
#define _DATABASE_H_

#include<string>
#include<vector>

#include "global.h"
#include "sqlstatement.h"
#include "table.h"
#include "BPlusTree.h"


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
	Value() { ; }

	/**
	*  \brief ��������
	*/
	~Value() { ; }

	/**
	*  \brief ��ȡValue��ֵ
	*/
	std::string GetValueData();

	/**
	*  \brief ��ȡValue������
	*/
	ValueType GetValueType();

	/**
	*  \brief ����Value
	*/
	void SetValue(std::string new_data, ValueType new_type);

	/**
	*  \brief ����Value��ֵ
	*/
	void SetValuedata(std::string new_data);


private:
	std::string value_data;      /* Value��ֵ */
	ValueType value_type;        /* Value������ */
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
	Record() { ; }

	/**
	*  \brief ��������
	*/
	~Record() { ; }

	/**
	*  \brief ��ʾ��¼
	*/
	bool Display(std::vector<Value> values_data, std::vector<Field> fields);

	/**
	*  \brief ����Value��ֵ
	*/
	void SetValue(std::vector<Value> values_data);

private:
	int record_id;                         /* ��¼���� */
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
	~Field() { ; }

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
	void SetFieldName(std::string new_name);

	/**
	*  \brief �����ֶ���������
	*/
	void SetFieldType(ValueType new_type);

	bool IsCreateIndex();

	void SetIsCreateIndex(bool is_create_index);

private:
	std::string field_name_;       /* �ֶ���  */
	ValueType field_type_;         /* �ֶ��������� */
	bool is_create_index_;         /* ��ʶ���ֶ��Ƿ������� */
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
	Index(std::string index_name, std::string field_name, ValueType type, std::string path);

	Index(std::string index_name, std::string field_name, ValueType type);

	/**
	*  \brief ��������
	*/
	~Index();

	bool InsertNode(std::string value, int data_id);

	bool DeleteNode(std::string value);

	int SearchNode(std::string value);

	bool SearchNode(std::string value, std::vector<int>& id);

  bool UpdateNode(std::string new_value, std::string old_value);

	std::string GetFieldName();

public:
	BPlusTree<int> * bplustree_int_;               /* int����������B+�� */
	BPlusTree<std::string> * bplustree_string_;    /* string����������B+�� */

private:
	std::string field_name_;                       /* ������Ӧ���ֶ��� */
	std::string index_name_;                       /* ������ */
	ValueType type_;                               /* �������� */
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
	Database() { ; }

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
	std::vector<Table *> GetTable();

	/**
	*  \brief ������
	*/
	bool CreateTable(SQLCreateTable & st);

	/**
	*  \brief �򿪱�
	*/
	bool UseTable(std::string DatabasePath);

private:
	std::string database_name;          /* ���ݿ����� */
	std::vector<Table *> table_;      /* ���ݿ��б� */
	std::string database_path;          /* ���ݿ�·�� */
};
#endif


