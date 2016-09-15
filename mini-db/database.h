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
	kIntegerType,     /* ������ */
	kStringType,      /* �ַ����� */
	kNullType         /* ����Ϊ�� */
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
	 *
	 *  \return Value��ֵ
	 */
	std::string GetValueData();

	/**
	 *  \brief ��ȡValue������
	 *
	 *  \return Value������
	 */
	ValueType GetValueType();

	/**
	 *  \brief ����Value
	 *
	 *  \param new_data Value����ֵ Value������������
	 */
	void SetValue(std::string new_data, ValueType new_type);

	/**
	 *  \brief ����Value��ֵ
	 *
	 *  \param new_data Value����ֵ
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
	 *
	 *  \param values_data һ����¼�е�ֵ���� fields �ֶ�����
	 *
	 *  \return �ɹ�����true ʧ�ܷ���false
	 */
	bool Display(std::vector<Value> values_data, std::vector<Field> fields);

	/**
	 *  \brief ����Value��ֵ
	 *
	 *  \param values_data ��ֵ������
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
	 *
	 *  \return �ֶ���
	 */
	std::string GetFieldName();

	/**
	 *  \brief ��ȡ�ֶ���������
	 *
	 *  \return �ֶ���������
	 */
	ValueType GetFieldType();

	/**
	 *  \brief �����ֶ���
	 *
	 *  \param new_name �µ��ֶ���
	 */
	void SetFieldName(std::string new_name);

	/**
	 *  \brief �����ֶ���������
	 *
	 *  \param new_type �µ��ֶ���������
	 */
	void SetFieldType(ValueType new_type);
	
	/**
	 *  \brief �жϸ��ֶ��Ƿ�������
	 *
	 *  \return �Ѿ���������true δ��������false
	 */
	bool IsCreateIndex();

	/**
	 *  \brief ��Ǹ��ֶ��Ƿ��Ѿ���������
	 *
	 *  \param ��ʶ�Ƿ�������
	 */
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

	/**
	*  \brief ����ڵ�
	*
	*  \param value �ڵ��ֵ data_id �ڵ��id
	*
	*  \return ����ɹ�����true ����ʧ�ܷ���false
	*/
	bool InsertNode(std::string value, USER_INT data_id);

	/**
	*  \brief ɾ���ڵ�
	*
	*  \param value �ڵ��ֵ
	*
	*  \return ɾ���ɹ�����true ����ʧ�ܷ���false
	*/
	bool DeleteNode(std::string value);

	/**
	*  \brief ���ҵ�һ���ؼ��ֵĽڵ�
	*
	*  \param value �ڵ��ֵ
	*
	*  \return �ڵ��id
	*/
	int SearchNode(std::string value);

	/**
	*  \brief �������йؼ��ֵĽڵ�
	*
	*  \param value �ڵ��ֵ id �ڵ��id����
	*
	*  \return ���ҳɹ�����true ʧ�ܷ���false
	*/
	bool SearchNode(std::string value, std::vector<USER_INT>& id);

	/**
	*  \brief ���ҷ��Ϲ�ϵʽ�Ľڵ�
	*
	*  \param value �ڵ��ֵ id �ڵ��id���� op ������
	*
	*  \return ���ҳɹ�����true ʧ�ܷ���false
	*/
	bool SearchNode(std::string value, std::vector<USER_INT>& id, OperatorType op);

	/**
	*  \brief ���ҷ��Ϲ�ϵʽ�Ľڵ�
	*
	*  \param new_value �µĽڵ��ֵ old_value �ɵĽڵ��ֵ
	*
	*  \return ���³ɹ�����true ʧ�ܷ���false
	*/
    bool UpdateNode(std::string new_value, std::string old_value);

	/**
	*  \brief ��������id
	*
	*  \param id������
	*
	*  \return �ɹ�����true ʧ�ܷ���false
	*/
	bool ShowAllId(std::vector<USER_INT> &id);

	/**
	*  \brief ��ȡ������Ӧ���ֶ���
	*
	*  \return ������Ӧ���ֶ���
	*/
	std::string GetFieldName();

public:
	BPlusTree<USER_INT> * bplustree_int_;          /* int����������B+�� */
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
	*
	*  \param sql�������ݿ�
	*
	*  \return �����ɹ�����true ʧ�ܷ���false
	*/
	bool CreateDatabase(SQLCreateDatabase &st);

	/**
	*  \brief �����ݿ�
	*
	*  \param sql�����ݿ�
	*
	*  \return ���ݿ�·��
	*/
	std::string UseDatabase(SQLUse &st);

	/**
	*  \brief ��ȡ���ݿ�����
	*
	*  \return ���ݿ�����
	*/
	std::string GetDatabaseName();

	/**
	*  \brief ��ȡ����
	*
	*  \return ����
	*/
	std::vector<Table *> GetTable();

	/**
	*  \brief �����������洢����
	*
	*  \param sql������
	*
	*  \return �����ɹ�����true ʧ�ܷ���false
	*/
	bool CreateTable(SQLCreateTable & st);

	/**
	*  \brief �����ݿ��еı�
	*
	*  \param  ���ݿ�·��
	*
	*  \return �򿪳ɹ�����true ʧ�ܷ���false
	*/
	bool UseTable(std::string DatabasePath);

private:
	std::string database_name;          /* ���ݿ����� */
	std::vector<Table *> table_;      /* ���ݿ��б� */
	std::string database_path;          /* ���ݿ�·�� */
};
#endif


