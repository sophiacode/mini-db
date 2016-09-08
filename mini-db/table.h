#ifndef _TABLE_H_
#define _TABLE_H_

#include <string>
#include <vector>

#define record_num 10000
#define record_max 10000
#define record_len 256

#include "database.h"
#include "IDPool.h"

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
	bool Display(int id);

private:

	/**
	*  \brief �ҵ��ֶζ�Ӧ���������
	*/
	int FindIndex(std::string index_field_name);

private:
	int records_num;				     /* ���������������� */
	int indexs_num;						 /* ���������������� */
	std::string table_name;				 /* ������ */
	std::vector<Field> fields;           /* �ֶ� */
	std::vector<Index *> indexs;         /* ���� */
	std::vector<int> select_id;			 /* ��¼selectѡ�е�id */
	std::string path;					 /* �洢���ݿ�·�� */
	IDPool idPool;						 /* �����ڴ�أ�����������id */
	ofstream fwp;						 /* ��¼�ļ�д */
	ifstream frp;						 /* ��¼�ļ��� */
};

#endif