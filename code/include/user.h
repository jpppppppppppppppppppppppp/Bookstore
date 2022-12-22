//
// Created by 14908 on 2022/12/7.
//

#ifndef MAIN_CPP_USER_H
#define MAIN_CPP_USER_H
#include <cstring>


struct book{
	char main_key[31]{};
	char Bookname[61]{};
	char Author[61]{};
	char Keyword[61]{};
	double price=0;
	int quantity=0;
	inline book & operator =(const book &B){
		strcpy(this->main_key,B.main_key);
		strcpy(this->Bookname,B.Bookname);
		strcpy(this->Author,B.Author);
		strcpy(this->Keyword,B.Keyword);
		this->price = B.price;
		this->quantity = B.quantity;
		return *this;
	}
	book(std::string &name1){
		strcpy(main_key, name1.c_str());
	}
	book():main_key(""){	}
	inline friend bool operator < (const book &lhs, const book &rhs) {
		if (std::string(lhs.main_key) < std::string(rhs.main_key))
			return true;
		return false;
	}
	inline friend bool operator==(const book &lhs, const book &rhs){
		return std::string(lhs.main_key) == std::string(rhs.main_key);
	}
	inline friend bool operator>(const book &lhs, const book &rhs) {
		if (std::string(lhs.main_key) > std::string(rhs.main_key))
			return true;
		return false;
	}
};
struct user{
	char main_key[31]{};
	char UserPW[31]{};
	char UserName[31]{};
	int type;
	book select{};
	bool IsSelect = false;
	inline friend bool operator<(const user &lhs, const user &rhs) {
		if (std::string(lhs.main_key) < std::string(rhs.main_key))
			return true;
		return false;
	}
	inline friend bool operator==(const user &lhs, const user &rhs){
		return std::string(lhs.main_key) == std::string(rhs.main_key);
	}
	inline friend bool operator>(const user &lhs, const user &rhs) {
		if (std::string(lhs.main_key) > std::string(rhs.main_key))
			return true;
		return false;
	}
};
#endif //MAIN_CPP_USER_H
