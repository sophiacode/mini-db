#ifndef _INTERPRETER_H_
#define _INTERPRETER_H_

#include<string>
#include<vector>

#include"sqltype.h"
#include"controller.h"

/**
 *  \brief ��������
 */
class Interpreter
{
public:
	/**
	 *  \brief ���캯��
	 */
	Interpreter(Controller * controller);
	
	/**
	 *  \brief ��������
	 */
	~Interpreter();

	/**
	 *  \brief ����SQL������
	 *  
	 *  \param sql_statement SQL���
	 */
	void SQLInterpret(std::string sql_statement);

private:
	std::string sql_statement_;            /* SQL��� */
	std::vector<std::string> sql_token_;   /* SQL���ָ��õ��ı�ʶ������ */
	SQLType sql_type_;                     /* SQL�������� */
	Controller * controller_;                /* ������ */

	/**
	 *  \brief SQL���Ԥ����
	 */
	void SQLPretreatment();

	/**
	 *  \brief SQL���ָ�
	 */
	void SplitSQL();

	/**
	 *  \brief ������ȡSQL��������
	 */
	void GetSQLType();

	/**
	 *  \brief ����SQL�ಢ���ݸ�������
	 */
	void ParseSQL();

	void ToLower(std::string & str);
};

#endif 