#ifndef _INTERPRETER_H_
#define _INTERPRETER_H_

#include<string>
#include<vector>

#include"sqltype.h"
#include"controller.h"

/**
 *  \brief 翻译器类
 */
class Interpreter
{
public:
	/**
	 *  \brief 构造函数
	 */
	Interpreter(Controller * controller);
	
	/**
	 *  \brief 析构函数
	 */
	~Interpreter();

	/**
	 *  \brief 进行SQL语句分析
	 *  
	 *  \param sql_statement SQL语句
	 */
	void SQLInterpret(std::string sql_statement);

private:
	std::string sql_statement_;            /* SQL语句 */
	std::vector<std::string> sql_token_;   /* SQL语句分割后得到的标识符数组 */
	SQLType sql_type_;                     /* SQL命令类型 */
	Controller * controller_;                /* 控制器 */

	/**
	 *  \brief SQL语句预处理
	 */
	void SQLPretreatment();

	/**
	 *  \brief SQL语句分割
	 */
	void SplitSQL();

	/**
	 *  \brief 分析获取SQL命令类型
	 */
	void GetSQLType();

	/**
	 *  \brief 创建SQL类并传递给控制器
	 */
	void ParseSQL();

	void ToLower(std::string & str);
};

#endif 