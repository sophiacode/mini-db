#ifndef _SQLSTATEMENT_H_
#define _SQLSTATEMENT_H_

#include<vector>
#include<string>

#include "global.h"
#include "database.h"
#include "sqltype.h"

/**
 *  \brief SQL������
 */
class SQLBase
{
public:
	/**
	 *  \brief ���캯��
	 */
	SQLBase(); 

	/**
	 *  \brief ��������
	 */
	~SQLBase();

	/**
	 *  \brief ��ý������
	 *
	 *  \return �����ɹ�����true��ʧ�ܷ���false
	 */
	bool IsParseSucceed();

	/** 
	 *  \brief ������������麯����
	 *
	 *  \param sql_token SQL��ʶ������
	 */
	virtual void Parse(std::vector<std::string> sql_token) = 0;

protected:
	bool is_parse_succeed;        /* �Ƿ�����ɹ��ı�ʶ */

	/**
	 *  \brief ������������ĸ��ת��ΪСд
	 *
	 *  \param str ��ת�������
	 */
	void ToLower(std::string & str);

	/**
	 *  \brief �������ŵı�ʶ���ϲ�Ϊͬһ��
	 *
	 *  \param sql_token SQL��ʶ������
	 *  \param pos ��ʼ�ϲ���λ��
	 */
	void MergeValue(std::vector<std::string> & sql_token, int pos);

	/**
	 *  \brief ����value��ֵ������
	 *  
	 *  \param sql ��������value��ʶ��
	 *  \param value �������������
	 *
	 *  \return value��Ч����true����Ч����false
	 */
	bool ParseValue(std::string sql, Value & value);

	/**
	 *  \brief ����������
	 *  
	 *  \param op ��������ʶ��
	 * 
	 *  \return ����������
	 */
	OperatorType ParseOperator(std::string op);
};

/**
 *  \brief �������ݿ����
 *         ��ʽ��CREATE DATABASE databasename databasepath;  
 */
class SQLCreateDatabase :public SQLBase
{
public:
	/**
	 *  \brief ���캯��
	 *
	 *  \param sql_token SQL��ʶ������
	 */
	SQLCreateDatabase(std::vector<std::string> sql_token);

	/**
	 *  \brief ��������
	 */
	~SQLCreateDatabase();

	/**
	 *  \brief ��ȡ���ݿ�����
	 *
	 *  \return ���ݿ�����
	 */
	std::string GetDatabaseName();

	/**
	 *  \brief ��ȡ���ݿ�·��
	 *
	 *  \return ���ݿ�·��
	 */
	std::string GetDatabasePath();

	/**
	 *  \brief ������������أ�
	 *
	 *  \param sql_token SQL��ʶ������
	 */
	void Parse(std::vector<std::string> sql_token) override;

private:
	std::string database_name_;  /* ���ݿ����� */
	std::string database_path_;  /* ���ݿ�·�� */
};

/**
 *  \brief �����ݿ����
 *         ��ʽ��USE databasename databasepath;
 */
class SQLUse :public SQLBase
{
public:
	/**
	 *  \brief ���캯��
	 *
	 *  \param sql_token SQL��ʶ������
	 */
	SQLUse(std::vector<std::string> sql_token);

	/**
	 *  \brief ��������
	 */
	~SQLUse();

	/**
	 *  \brief ��ȡ���ݿ�����
	 *
	 *  \return ���ݿ�����
	 */
	std::string GetDatabaseName();

	/**
	 *  \brief ��ȡ���ݿ�·��
	 *
	 *  \return ���ݿ�·��
	 */
	std::string GetDatabasePath();

	/**
	 *  \brief ������������أ�
	 *
	 *  \param sql_token SQL��ʶ������
	 */
	void Parse(std::vector<std::string> sql_token) override;

private:
	std::string database_name_;  /* ���ݿ����� */
	std::string database_path_;  /* ���ݿ�·�� */
};

/**
 *  \brief �������
 *         ��ʽ��CREATE TABLE tablename
 *              (fieldname1 type1, fieldname2 type2, ...); 
 */
class SQLCreateTable :public SQLBase
{
public:
	/**
	 *  \brief ���캯��
	 *
	 *  \param sql_token SQL��ʶ������
	 */
	SQLCreateTable(std::vector<std::string> sql_token);

	/**
	 *  \brief ��������
	 */
	~SQLCreateTable();

	/**
	 *  \brief ��ȡ����
	 *
	 *  \return ����
	 */
	std::string GetTableName();

	/**
	 *  \brief ��ȡ�ֶ�����
	 *
	 *  \return �ֶ�����
	 */
	std::vector<Field> GetFields();

	/**
	 *  \brief ������������أ�
	 *
	 *  \param sql_token SQL��ʶ������
	 */
	void Parse(std::vector<std::string> sql_token) override;

private:
	std::string table_name_;      /* ���� */
	std::vector<Field> fields_;   /* �ֶ����� */
};

/**
 *  \brief �������
 *         ��ʽ��INSERT INTO tablename (field1,field2,...) 
 *               VALUES (value1,value2,...)
 */
class SQLInsert :public SQLBase
{
public:
	/**
	 *  \brief ���캯��
	 *
	 *  \param sql_token SQL��ʶ������
	 */
	SQLInsert(std::vector<std::string> sql_token);

	/**
	 *  \brief ��������
	 */
	~SQLInsert();

	/**
	 *  \brief ��ȡ����
	 *
	 *  \return ����
	 */
	std::string GetTableName();

	/**
	 *  \brief ��ȡ�ֶ�������
	 *
	 *  \return �ֶ�������
	 */
	std::vector<std::string> GetFields();

	/**
	 *  \brief ��ȡֵ����
	 *
	 *  \return ֵ����
	 */
	std::vector<Value> GetValues();

	/**
	 *  \brief ����Ƿ����ֶ�
	 *  
	 *  \return �����ֶη���true�������з���false
	 */
	bool IsInputField();

	/**
	 *  \brief ������������أ�
	 *
	 *  \param sql_token SQL��ʶ������
	 */
	void Parse(std::vector<std::string> sql_token) override;

private:
	std::string table_name_;                /* ���� */
	std::vector<std::string> fields_;       /* �ֶ������� */
	std::vector<Value> values_;             /* ֵ���� */
	bool is_input_field_;                   /* �Ƿ����ֶεı�ʶ */  
};

/**
 *  \brief ɾ�����
 *         ��ʽ��DELETE FROM tablename WHERE field op value;
 */
class SQLDelete :public SQLBase
{
public:
	/**
	 *  \brief ���캯��
	 *
	 *  \param sql_token SQL��ʶ������
	 */
	SQLDelete(std::vector<std::string> sql_token);

	/**
	 *  \brief ��������
	 */
	~SQLDelete();

	/**
	 *  \brief ��ȡ����
	 *
	 *  \return ����
	 */
	std::string GetTableName();

	/**
	 *  \brief ��ȡwhere�Ӿ��е��ֶ�
	 *  
	 *  \return where�Ӿ��е��ֶ�
	 */
	std::string GetField();

	/**
	 *  \brief ��ȡwhere�Ӿ��е�ֵ
	 *
	 *  \return where�Ӿ��е�ֵ
	 */
	Value GetValue();

	/**
	 *  \brief ��ȡwhere�Ӿ��еĲ�����
	 *
	 *  \return where�Ӿ��еĲ�����
	 */
	OperatorType GetOperatorType();

	/**
	 *  \brief ����Ƿ���where�Ӿ�
	 *
	 *  \return ����where�Ӿ䷵��true�������з���false
	 */
	bool IsInputWhere();

	/**
	 *  \brief ������������أ�
	 *
	 *  \param sql_token SQL��ʶ������
	 */
	void Parse(std::vector<std::string> sql_token) override;

private:
	std::string table_name_;         /* ���� */
	std::string field_;              /* where�Ӿ��е��ֶ��� */
	Value * value_;                  /* where�Ӿ��е�ֵ */
	OperatorType op_;                /* where�Ӿ��еĲ����� */
	bool is_input_where_;            /* ����Ƿ���where�Ӿ�ı�ʶ */
	
};

/**
 *  \brief �������
 *         ��ʽ��UPDATE tablename SET field1=value1, field2=value2, ... 
 *		         WHERE field op value;
 */
class SQLUpdate :public SQLBase
{
public:
	/**
	 *  \brief ���캯��
	 *
	 *  \param sql_token SQL��ʶ������
	 */
	SQLUpdate(std::vector<std::string> sql_token);

	/**
	 *  \brief ��������
	 */
	~SQLUpdate();

	/**
	 *  \brief ��ȡ����
	 *
	 *  \return ����
	 */
	std::string GetTableName();

	/**
	 *  \brief ��ȡset����е��ֶ�������
	 *
	 *  \return set����е��ֶ�������
	 */
	std::vector<std::string> GetNewField();

	/**
	 *  \brief ��ȡset����е���ֵ����
	 *
	 *  \return set����е���ֵ����
	 */
	std::vector<Value> GetNewValue();

	/**
	 *  \brief ��ȡwhere�Ӿ��е��ֶ�
	 *
	 *  \return where�Ӿ��е��ֶ�
	 */
	std::string GetWhereField();

	/**
	 *  \brief ��ȡwhere�Ӿ��еľ�ֵ
	 *
	 *  \return where�Ӿ��еľ�ֵ
	 */
	Value GetWhereValue();

	/**
	 *  \brief ��ȡwhere�Ӿ��еĲ�����
	 *
	 *  \return where�Ӿ��еĲ�����
	 */
	OperatorType GetOperatorType();

	/**
	 *  \brief ������������أ�
	 *
	 *  \param sql_token SQL��ʶ������
	 */
	void Parse(std::vector<std::string> sql_token) override;

private:
	std::string table_name_;               /* ���� */
	std::vector<std::string> new_fields_;  /* set����е��ֶ������� */
	std::vector<Value> new_values_;        /* set����е���ֵ���� */
	std::string where_field_;              /* where�Ӿ��е��ֶ� */
	Value * where_value_;                  /* where�Ӿ��еľ�ֵ */
	OperatorType op_;                      /* where�Ӿ��еĲ����� */
};

/**
 *  \brief ��ѯ���
 *         ��ʽ��SELECET FROM tablename WHERE field op value
 */
class SQLSelect :public SQLBase
{
public:
	/**
	 *  \brief ���캯��
	 *
	 *  \param sql_token SQL��ʶ������
	 */
	SQLSelect(std::vector<std::string> sql_token);

	/**
	 *  \brief ��������
	 */
	~SQLSelect();

	/**
	 *  \brief ��ȡ����
	 *
	 *  \return ����
	 */
	std::string GetTableName();

	/**
	 *  \brief ��ȡwhere�Ӿ��е��ֶ�
	 *
	 *  \return where�Ӿ��е��ֶ�
	 */
	std::string GetField();

	/**
	 *  \brief ��ȡwhere�Ӿ��е�ֵ
	 *
	 *  \return where�Ӿ��е�ֵ
	 */
	Value GetValue();

	/**
	 *  \brief ��ȡwhere�Ӿ��еĲ�����
	 *
	 *  \return where�Ӿ��еĲ�����
	 */
	OperatorType GetOperatorType();

	/**
	 *  \brief ����Ƿ���where�Ӿ�
	 *
	 *  \return ����where�Ӿ䷵��true�������з���false
	 */
	bool IsInputWhere();

	/**
	 *  \brief ������������أ�
	 *
	 *  \param sql_token SQL��ʶ������
	 */
	void Parse(std::vector<std::string> sql_token) override;

private:
	std::string table_name_;    /* ���� */
	std::string field_;         /* where�Ӿ��е��ֶ��� */
	Value * value_;             /* where�Ӿ��е�ֵ */
	OperatorType op_;           /* where�Ӿ��еĲ����� */
	bool is_input_where_;       /* ����Ƿ���where�Ӿ� */
};

/**
 *  \brief �����������
 *         ��ʽ��CREATE INDEX indexname ON tablename(fieldname); 
 */
class SQLCreateIndex :public SQLBase
{
public:
	/**
	 *  \brief ���캯��
	 *
	 *  \param sql_token SQL��ʶ������
	 */
	SQLCreateIndex(std::vector<std::string> sql_token);

	/**
	 *  \brief ��������
	 */
	~SQLCreateIndex();

	/**
	 *  \brief ��ȡ����
	 *
	 *  \return ����
	 */
	std::string GetTableName();

	/**
	 *  \brief ��ȡ�ֶ���
	 *
	 *  \return �ֶ���
	 */
	std::string GetField();

	/**
	 *  \brief ��ȡ������
	 *
	 *  \return ������
	 */
	std::string GetIndex();

	/**
	 *  \brief ������������أ�
	 *
	 *  \param sql_token SQL��ʶ������
	 */
	void Parse(std::vector<std::string> sql_token) override;

private:
	std::string table_name_;   /* ���� */
	std::string field_;        /* �ֶ��� */
	std::string index_;        /* ������ */
};

#endif
