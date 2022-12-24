#include <iostream>
#include "include/interpreter.h"
#include "include/error.hpp"



int main(){
	Interpreter interpreter;
	interpreter.init();
	while (true) {
		try {
			std::string input;
			if (std::cin.eof())
				return 0;
			getline(std::cin, input);
			if (input.empty())
				continue;
			interpreter.input(input);
			if(interpreter.p())return 0;
			interpreter.run();
		} catch (ErrorException &ex) {
			std::cout << ex.getMessage() << std::endl;
		}
	}
}
