#ifndef BOOKSTORE_INTERPRETER_H
#define BOOKSTORE_INTERPRETER_H

#include "tokenScanner.hpp"
#include "user.h"
#include <stack>
class Interpreter{
public:
	TokenScanner scanner;
	std::stack<user>users;
	void init();
	void input(const std::string &s);
	void run();

private:
	std::string az(const std::string &s);
	char * ez(const char * &s);
};


#endif //BOOKSTORE_INTERPRETER_H
