#include <iostream>
#include "include/interpreter.h"
#include "include/error.hpp"



int main(){
	Interpreter interpreter;
	interpreter.init();
	while (true) {
		try {
			std::string input;
			getline(std::cin, input);
			if (input.empty())
				return 0;
			if (input == "exit")return 0;
			if (input == "quit")return 0;
			interpreter.input(input);
			interpreter.run();
		} catch (ErrorException &ex) {
			std::cout << ex.getMessage() << std::endl;
		}
	}
	return 0;
}