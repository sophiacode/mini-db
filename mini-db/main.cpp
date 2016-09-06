#include<iostream>
#include "controller.h"
#include "interpreter.h"

int name_num_;

int main()
{
	std::string str;
	char c;
	Controller * controller = new Controller();
	
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