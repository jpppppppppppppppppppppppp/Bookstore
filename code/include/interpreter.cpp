//
// Created by 14908 on 2022/12/5.
//
#include "interpreter.h"
#include "error.hpp"
#include <fstream>
void Interpreter::init(){
	scanner.ignoreWhitespace();
	scanner.scanNumbers();
	std::fstream io;
	io.open("User1",std::fstream::out);
	io.write(az("root").c_str(),31);
	io.write(az("sjtu").c_str(),31);
	io.write(az("Administer").c_str(),31);
	int t=7;
	io.write(reinterpret_cast<char *>(&t), sizeof(int));
	io.close();
}

void Interpreter::input(const std::string &s){
	scanner.setInput(s);
}

void Interpreter::run(){
	std::string mode = scanner.nextToken();
	if(mode == "exit" || mode == "quit")exit(0);
	else if(mode == "su"){
		std::string UserID,UserPW;
		if(scanner.hasMoreTokens())UserID = scanner.nextToken();
		else throw ErrorException("Invalid");
		if(scanner.hasMoreTokens())UserPW = scanner.nextToken();
		else throw ErrorException("Invalid");
		if(scanner.hasMoreTokens())throw ErrorException("Invalid");

	}
}

std::string Interpreter::az(const std::string &s){
	std::string temp;
	for(int i=0;i<30-s.size();i++)
		temp+=' ';
	for(auto i:s)
		temp+=i;
	return temp;
}
