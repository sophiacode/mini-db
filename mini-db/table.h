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
	*
	*  \param new_path ���ݿ�洢·��
	*/
	Table(std::string new_path);

	/**
	*  \brief ��������
	*/
	~Table();

	/**
	*  \brief ������
	*
	*  \param sql SQL��Ч��Ϣ��
	*
	*  \return �����ɹ�����true�����򷵻�false
	*/
	bool CreateTable(SQLCreateTable &sql);

	/**
	*  \brief ��ȡ������
	*
	*  \return ��ǰ�������
	*/
	std::string GetTableName();

	/**
	*  \brief ��ȡ��ͷ��Ϣ
	*
	*  \return ��ǰ��ͷ�洢��vector
	*/
	std::vector<Field> GetTableFields();

	/**
	*  \brief ���ñ���
	*
	*  \param new_name �µı���
	*/
	void SetTableName(std::string new_name);

	/**
	*  \brief �򿪱�
	*
	*  \return �򿪳ɹ�����true,���򷵻�false
	*/
	bool UseTable();

	/**
	*  \brief ���Ӽ�¼
	*
	*  \param st SQL�����Ϣ��
	*
	*  \return ����ɹ�����true,���򷵻�false
	*/
	bool CreateRecord(SQLInsert &st);

	/**
	*  \brief ���¼�¼
	*
	*  \param su SQL�����Ϣ��
	*
	*  \return ���³ɹ�����true,���򷵻�false
	*/
	bool UpdateRecord(SQLUpdate &su);

	/**
	*  \brief ɾ����¼
	*
	*  \param sd SQL�����Ϣ��
	*
	*  \return ɾ���ɹ�����true,���򷵻�false
	*/
	bool DeleteRecord(SQLDelete &sd);

	/**
	*  \brief ͨ��SQLֱ�Ӳ��Ҽ�¼
	*
	*  \param sql SQL�����Ϣ��
	*
	*  \return ���ҳɹ�����true,���򷵻�false
	*/
	bool SelectRecord(SQLSelect &sql);

	/**
	*  \brief Delete����Select����
	*
	*  \param sd SQL�����Ϣ��
	*
	*  \return ���ҳɹ�����true,���򷵻�false
	*/
	bool SelectRecord(SQLDelete &sd);

	/**
	*  \brief Update����Select����
	*
	*  \param su SQL�����Ϣ��
	*
	*  \return ���ҳɹ�����true,���򷵻�false
	*/
	bool SelectRecord(SQLUpdate &su);

	/**
	*  \brief ��������
	*
	*  \param si SQL�����Ϣ��
	*
	*  \return �����ɹ�����true,���򷵻�false
	*/
	bool CreateIndex(SQLCreateIndex &si);

	/**
	*  \brief ȫ����ʾ
	*/
	bool Display();

	/**
	*  \brief ��¼��ʾ
	*
	*  \param id Ҫ��ʾ�ļ�¼id
	*  \param iter �����˳����
	*
	*  \return ��ʾ�ɹ�����true������false
	*/
	bool Display(USER_INT id,USER_INT iter);

	/**
	*  \brief ˳�����
	*
	*  \param select_field Ҫ���ҵ��ֶ���
	*  \param select_value Ҫ���ҵ�������Ϣ
	*  \param select_op ��Ӧ�������
	*
	*  \return ���ҳɹ�����true������false
	*/
	bool OrderSelect(string select_field, Value select_value, OperatorType select_op);

private:

	/**
	*  \brief �ҵ��ֶζ�Ӧ���������
	*
	*  \param index_field_name ������Ӧ���ֶ���
	*
	*  \return ���ҵ���������Ӧ��ţ���û����������-1
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