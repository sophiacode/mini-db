#ifndef _TABLE_H_
#define _TABLE_H_

#include <string>
#include <vector>
#include <fstream>
#include <io.h>

#define record_max 1000000
#define record_len 256
#define int_len    10
#define true_int   16
#define true_len   272

#include "database.h"
#include "IDPool.h"
#include "sqlstatement.h"
#include "sqltype.h"

/**
*  \brief ����
*/
class Table
{
public:
	/**
	*  \brief ���캯��
	*/
	Table(std::string new_path);

	/**
	*  \brief ��������
	*/
	~Table();

	/**
	*  \brief ������
	*/
	bool CreateTable(SQLCreateTable &sql);

	/**
	*  \brief ��ȡ������
	*/
	std::string GetTableName();

	/**
	*  \brief ��ȡ��ͷ��Ϣ
	*/
	std::vector<Field> GetTableFields();

	/**
	*  \brief ���ñ���
	*/
	void SetTableName(std::string new_name);

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
	bool UpdateRecord(SQLUpdate &su);

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

	/**
	*  \brief ��������
	*/
	bool CreateIndex(SQLCreateIndex &si);

	/**
	*  \brief ȫ����ʾ
	*/
	bool Display();

	/**
	*  \brief ��¼��ʾ
	*/
	bool Display(USER_INT id,USER_INT iter);

	/**
	*  \brief ˳�����
	*/
	bool OrderSelect(string select_field, Value select_value, OperatorType select_op);
private:

	/**
	*  \brief �ҵ��ֶζ�Ӧ���������
	*/
	int FindIndex(std::string index_field_name);

	bool WildcardSearch(std::string pattern, int field_id, int index_id);

	bool WildcardMatch(std::string str, std::string pattern);

private:
	USER_INT records_num;				 /* ���������������� */
	USER_INT record_leng;				 /* һ����¼��ʵ�ʳ��� */
	int indexs_num;						 /* ���������������� */
	std::string table_name;				 /* ������ */
	std::vector<Field> fields;           /* �ֶ� */
	std::vector<Index *> indexs;         /* ���� */
	std::vector<USER_INT> select_id;	 /* ��¼selectѡ�е�id */
	std::vector<USER_INT> real_id;		 /* ��Чid��ų� */
	std::string path;					 /* �洢���ݿ�·�� */
	IDPool * idPool;					 /* �����ڴ�أ�����������id */
	ofstream fwp;						 /* ��¼�ļ�д */
	ifstream frp;						 /* ��¼�ļ��� */
};

#endif