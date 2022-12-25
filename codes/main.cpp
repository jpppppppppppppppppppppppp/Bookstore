#include <iostream>
#include "include/interpreter.h"
#include "include/error.hpp"



int main(){
	Interpreter interpreter;
	interpreter.init();
	while (true) {
		try {
			std::string input;
			if (std::cin.eof()){
				interpreter.cmds.push_back("exit");
				return 0;
			}
			getline(std::cin, input);
			if (input.empty())
				continue;
			interpreter.input(input);
			interpreter.cmds.push_back(input);
			if(interpreter.p())return 0;
			interpreter.run();
		} catch (ErrorException &ex) {
			interpreter.cmds.pop_back();
			std::cout << ex.getMessage() << std::endl;
		}
	}
}
