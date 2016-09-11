#include<iostream>
#include "controller.h"
#include "interpreter.h"

int name_num_;

int main()
{
  //freopen("order.txt", "r", stdin);
	std::string str;
	char c;
	Controller * controller = new Controller();

	std::cout << "Mini-db������" << endl;
	std::cout << "�봴��һ�������ݿ���ߴ�һ���������ݿ�" << endl;
	std::cout << "�����Էֺš�;����Ϊ�������" << endl;
	
	while (true)
	{
		Interpreter interpreter(controller);

		str.clear();
		while ((c = std::cin.get()) != ';')
		{
			str += c;
		}
		
		if (interpreter.SQLInterpret(str) == false)
		{
			break;
		}
	}

	if (controller != nullptr)
	{
		delete controller;
		controller = nullptr;
	}
	return 0;
}