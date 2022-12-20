//
// Created by 14908 on 2022/12/7.
//

#ifndef MAIN_CPP_USER_H
#define MAIN_CPP_USER_H
#include <cstring>
struct user{
	char main_key[31];
	char UserPW[31];
	char UserName[31];
	int type;
	int select=0;
	inline friend bool operator<(const user &lhs, const user &rhs) {
		if (std::string(lhs.main_key) < std::string(rhs.main_key))
			return true;
		return false;
	}
	inline friend bool operator==(const user &lhs, const user &rhs){
		return std::string(lhs.main_key) == std::string(rhs.main_key);
	}
	inline friend bool operator>(const user &lhs, const user &rhs) {
		return !(lhs < rhs) and !(lhs == rhs);
	}
};

struct book{
	char main_key[21]{};
	char Bookname[61]{};
	char Author[61]{};
	char Keyword[61]{};
	double price;
	int quality;

	book(std::string &name1){
		strcpy(main_key, name1.c_str());
	}
	book():main_key(""){	}
	inline friend bool operator<(const book &lhs, const book &rhs) {
		if (std::string(lhs.main_key) < std::string(rhs.main_key))
			return true;
		return false;
	}
	inline friend bool operator==(const book &lhs, const book &rhs){
		return std::string(lhs.main_key) == std::string(rhs.main_key);
	}
	inline friend bool operator>(const book &lhs, const book &rhs) {
		return !(lhs < rhs) and !(lhs == rhs);
	}
};
#endif //MAIN_CPP_USER_H
