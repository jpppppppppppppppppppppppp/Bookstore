//
// Created by 14908 on 2022/12/5.
//
#include "interpreter.h"

void Interpreter::init(){
	scanner.ignoreWhitespace();
	scanner.scanNumbers();
}

void Interpreter::input(const std::string &s){
	scanner.setInput(s);
}

void Interpreter::run(){
	std::string mode = scanner.nextToken();
	if(mode == "exit" || mode == "quit")exit(0);
	else if(mode == "su"){

	}
}
