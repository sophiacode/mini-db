#include<iostream>
#include "controller.h"
#include "interpreter.h"

int name_num_;

int main()
{
	std::string str;
	char c;
	Controller * controller = new Controller();

	std::cout << "Mini-db解释器" << endl;
	std::cout << "请创建一个新数据库或者打开一个已有数据库" << endl;
	std::cout << "命令以分号‘;’作为结束标记" << endl;
	
	while (true)
	{
		Interpreter interpreter(controller);

		str.clear();
		while ((c = std::cin.get()) != ';')
		{
			str += c;
		}
		interpreter.SQLInterpret(str);
	}

	return 0;
}