#pragma once

/**
*  \brief SQL��������
*/
enum SQLType
{
	kSQLUndefined,         /* δ���� */
	kSQLCreateDatabase,    /* �������ݿ� */
	kSQLUse,               /* ʹ�����ݿ� */
	kSQLCreateTable,       /* ������ */
	kSQLInsert,            /* ���Ӽ�¼ */
	kSQLDelete,            /* ɾ����¼ */
	kSQLUpdate,            /* ���¼�¼ */
	kSQLSelect,            /* ��ѯ��¼ */
	kSQLCreateIndex,        /* �������� */
	kSQLDisplay
};

/**
 *  \brief ����������
 */
enum OperatorType
{
	kOpUndefined,        /*δ����*/
	kOpEqual,            /*����*/
	kOpGreater,          /*����*/
	kOpLess,             /*С��*/
	kOpNotEqual,         /*������*/ 
	kOpGreterOrEqual,    /*���ڵ���*/
	kOpLessOrEqual       /*С�ڵ���*/
};
