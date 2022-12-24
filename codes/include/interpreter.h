#ifndef BOOKSTORE_INTERPRETER_H
#define BOOKSTORE_INTERPRETER_H

#include "tokenScanner.hpp"
#include "user.h"
#include <stack>
#include <vector>
#include "writer.cpp"
class Interpreter{
public:
	~Interpreter();
	int n=0;
	std::vector<std::string>ps;
	std::stack<user>users;
	std::vector<std::string>loggerUsers;
	writer<book>book_manager;
	writer<user>user_manager;
	std::vector<std::pair<double,double>>finance;
	int priority=0;
	void init();
	void input(const std::string &s);
	void run();
	bool p();
private:
	std::string az(const std::string &s, const int n);
	std::string ez(const std::string &s);
};


#endif //BOOKSTORE_INTERPRETER_H
