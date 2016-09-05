#ifndef _DATABASE_H_
#define _DATABASE_H_

#include<string>
#include<vector>
#include"sqlstatement.h"
#include"table.h"
#include"BPlusTree.h"

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

	bool IsCreateIndex();

private:
	std::string field_name;       /* �ֶ���  */
	ValueType field_type;         /* �ֶ��������� */
	bool is_create_index;         /* ��ʶ���ֶ��Ƿ������� */
};

/**
*  \brief ������
*/
template<class KEYTYPE>
class Index
{
public:
	/**
	*  \brief ���캯��
	*/
	Index(std::string index_name,std::string field_name);

	/**
	*  \brief ��������
	*/
	~Index();

	bool InsertNode(KEYTYPE key, int data_id);

	bool DeleteNode(KEYTYPE key);

	int SearchNode(KEYTYPE key);

	bool UpdateNode(KEYTYPE key);
private:
	BPlusTree * bplustree_;   /* ������B+�� */
	std::string field_name;  /* ������Ӧ���ֶ��� */
	std::string index_name;  /* ������ */
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
	Database();

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
	std::string database_path;          /* ���ݿ�·�� */
};
#endif



template<class KEYTYPE>
inline Index<KEYTYPE>::Index(std::string index_name, std::string field_name)
{
	index_name_ = index_name;
	field_name_ = field_name;
	
	bplustree_ = new BPlusTree();
}

template<class KEYTYPE>
inline Index<KEYTYPE>::~Index()
{
	if (bplustree_ != nullptr)
	{
		delete bplustree_;
		bplustree_ = nullptr;
	}
}

template<class KEYTYPE>
inline bool Index<KEYTYPE>::InsertNode(KEYTYPE key, int data_id)
{
	return bplustree_->InsertNode(key,data_id);
}

template<class KEYTYPE>
inline bool Index<KEYTYPE>::DeleteNode(KEYTYPE key)
{
	return bplustree_->DeleteNode(key);
}

template<class KEYTYPE>
inline int Index<KEYTYPE>::SearchNode(KEYTYPE key)
{
	return bplustree_->SearchNode(key);
}

template<class KEYTYPE>
inline bool Index<KEYTYPE>::UpdateNode(KEYTYPE key)
{
	return bplustree_->UpdateNode(key);
}

