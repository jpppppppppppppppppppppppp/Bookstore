//
// Created by 14908 on 2022/12/5.
//
#include "interpreter.h"
#include "error.hpp"
#include <fstream>
void Interpreter::init(){
	scanner.ignoreWhitespace();
	scanner.scanNumbers();
	book_manager.setup("book");
	user_manager.setup("user");
	if(user_manager.num==0){
		user root{};
		root.type = 7;
		strcpy(root.main_key,az("root",30).c_str());
		strcpy(root.UserName,az("root",30).c_str());
		strcpy(root.UserPW,az("sjtu",30).c_str());
		user_manager.w(root);
	}
}

void Interpreter::input(const std::string &s){
	scanner.setInput(s);
}

void Interpreter::run(){
	std::string mode = scanner.nextToken();
	if(mode == "su"){
		std::string UserID,UserPW;
		if(scanner.hasMoreTokens())UserID = scanner.nextToken();
		else throw ErrorException("Invalid");
		if(!loggerUsers.empty() and std::find(loggerUsers.begin(), loggerUsers.end(),UserID)!=loggerUsers.end())throw ErrorException("Invalid");
		user temp{};
		strcpy(temp.main_key,az(UserID,30).c_str());
		std::set<user>s;
		user_manager.f(temp,s);
		if(!s.empty()){
			if(s.begin()->type < priority){
				users.push(*(s.begin()));
				priority = s.begin()->type;
				loggerUsers.push_back(UserID);
			}
			else{
				if(scanner.hasMoreTokens())UserPW = scanner.nextToken();
				else throw ErrorException("Invalid");
				if(scanner.hasMoreTokens())throw ErrorException("Invalid");
				if(std::string(s.begin()->UserPW)== az(UserPW,30)){
					users.push(*(s.begin()));
					priority = s.begin()->type;
					loggerUsers.push_back(UserID);
				}
				else throw ErrorException("Invalid");
			}
		}
		else throw ErrorException("Invalid");
	}
	else if(mode == "logout"){
		if(!users.empty()){
			loggerUsers.erase(std::find(loggerUsers.begin(), loggerUsers.end(),ez(std::string (users.top().main_key))));
			users.pop();
			if(!users.empty())priority = users.top().type;
			else priority = 0;
		}
		else throw ErrorException("Invalid");
	}
	else if(mode == "register"){
		std::string UserID,UserPW,UserName;
		if(scanner.hasMoreTokens())UserID=scanner.nextToken();
		else throw ErrorException("Invalid");
		if(scanner.hasMoreTokens())UserPW=scanner.nextToken();
		else throw ErrorException("Invalid");
		if(scanner.hasMoreTokens())UserName=scanner.nextToken();
		else throw ErrorException("Invalid");
		if(scanner.hasMoreTokens())throw ErrorException("Invalid");
		user temp;
		strcpy(temp.main_key, az(UserID,30).c_str());
		std::set<user>s;
		user_manager.f(temp,s);
		if(!s.empty())throw ErrorException("Invalid");
		else{
			strcpy(temp.UserPW, az(UserPW,30).c_str());
			strcpy(temp.UserName, az(UserName,30).c_str());
			temp.type = 1;
			user_manager.w(temp);
		}
	}
	else if(mode == "passwd"){
		std::string UserID,CWD,WD;
		if(priority==0)throw ErrorException("Invalid");
		if(scanner.hasMoreTokens())UserID=scanner.nextToken();
		else throw ErrorException("Invalid");
		if(scanner.hasMoreTokens())CWD=scanner.nextToken();
		else throw ErrorException("Invalid");
		if(scanner.hasMoreTokens()){
			WD = scanner.nextToken();
			if(scanner.hasMoreTokens())throw ErrorException("Invalid");
			user temp;
			strcpy(temp.main_key, az(UserID,30).c_str());
			std::set<user>s;
			user_manager.f(temp,s);
			if(s.empty())throw ErrorException("Invalid");
			else{
				if(std::string((*s.begin()).UserPW) == az(CWD,30)){
					user_manager.d(temp);
					strcpy(temp.UserPW,az(WD,30).c_str());
					strcpy(temp.UserName,s.begin()->UserName);
					user_manager.w(temp);
				}else throw ErrorException("Invalid");
			}
		}else{
			if(priority==7){
				user temp;
				strcpy(temp.main_key, az(UserID,30).c_str());
				std::set<user>s;
				user_manager.f(temp,s);
				if(s.empty())throw ErrorException("Invalid");
				else{
					user_manager.d(temp);
					strcpy(temp.UserPW,az(CWD,30).c_str());
					strcpy(temp.UserName,s.begin()->UserName);
					user_manager.w(temp);
				}
			}else throw ErrorException("Invalid");
		}
	}
}

std::string Interpreter::az(const std::string &s, const int n){
	std::string ans;
	for(int i=0;i<n-s.length();i++)ans+=' ';
	ans+=s;
	return ans;
}
std::string Interpreter::ez(const std::string &s){
	std::string ans;
	for(auto i:s)
		if(i!=' ')ans+=i;
	return  ans;
}