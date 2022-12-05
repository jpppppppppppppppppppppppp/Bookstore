#ifndef BOOKSTORE_INTERPRETER_H
#define BOOKSTORE_INTERPRETER_H

#include "tokenScanner.hpp"
class Interpreter{
public:
	TokenScanner scanner;
	void init();
	void input(const std::string &s);
	void run();
};


#endif //BOOKSTORE_INTERPRETER_H
