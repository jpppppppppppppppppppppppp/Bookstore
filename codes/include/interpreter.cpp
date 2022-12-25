//
// Created by 14908 on 2022/12/5.
//
#include "interpreter.h"
#include "error.hpp"
#include <fstream>
#include <string>
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[92m"
#define ANSI_COLOR_YELLOW  "\x1b[93m"
#define ANSI_COLOR_OLD_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[95m"
#define ANSI_COLOR_CYAN    "\x1b[96m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void Interpreter::init(){
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
	std::fstream os("finance");
	os.seekg(0,os.end);
	int l = os.tellg();
	if(l<=0)finance.clear();
	else{
		os.seekg(0);
		double n,m;
		for(int i=0;i<l/2/ sizeof(double);i++){
			os.read(reinterpret_cast<char *>(&n), sizeof(double ));
			os.read(reinterpret_cast<char *>(&m),sizeof (double ));
			finance.push_back(std::make_pair(n,m));
		}
	}
	os.close();
	os.open("cmds");
	os.seekg(0,os.end);
	l = os.tellg();
	if(l<=0)cmds.clear();
	else{
		os.seekg(0);
		char temp[301];
		for(int i=0;i<l/ sizeof(temp);i++){
			os.read(temp,sizeof(temp));
			cmds.push_back(std::string(temp));
		}
	}
	os.close();
	loggerUsers.clear();
}

void Interpreter::input(const std::string &s){
	n=0;
	ps.clear();
	std::string input = s+" ";
	std::string temp;
	for(auto i:input){
		if(i<=31 or i>=127)throw ErrorException("Invalid");
		if(i==' '){
			if(!temp.empty()){
				ps.push_back(temp);
				n++;
				temp = "";
			}
		}
		else temp+=i;
	}
	if(n==0){
		n++;
		ps.push_back("");
	}
}

void Interpreter::run(){
	std::string mode = ps[0];
	if(mode == "su"){
		std::string UserID, UserPW;
		if(n>1)UserID = ps[1];
		else throw ErrorException("Invalid");
		if(UserID.length()>30)throw ErrorException("Invalid");
		for(auto i:UserID){
			if(!((i>='0' and i<='9')or(i>='a' and i<='z')or(i>='A' and i<='Z')or i=='_'))throw ErrorException("Invalid");
			if(i<=32 or i>=127)throw ErrorException("Invalid");
		}
		user temp {};
		strcpy(temp.main_key, az(UserID, 30).c_str());
		std::set<user> s;
		user_manager.f(temp, s);
		if(!s.empty()){
			if(s.begin()->type < priority){
				if(n>2){
					UserPW = ps[2];
					if(n>3)throw ErrorException("Invalid");
					if(UserPW.length()>30)throw ErrorException("Invalid");
					for(auto i:UserPW){
						if(!((i>='0' and i<='9')or(i>='a' and i<='z')or(i>='A' and i<='Z')or i=='_'))throw ErrorException("Invalid");
						if(i<=32 or i>=127)throw ErrorException("Invalid");
					}
					if(std::string(s.begin()->UserPW) != az(UserPW, 30)){
						throw ErrorException("Invalid");
					}
				}
				users.push(*(s.begin()));
				priority = s.begin()->type;
				loggerUsers.push_back(UserID);
				users.top().IsSelect = false;
			}
			else{
				if(n>2)UserPW = ps[2];
				else throw ErrorException("Invalid");
				if(n>3)throw ErrorException("Invalid");
				if(UserPW.length()>30)throw ErrorException("Invalid");
				for(auto i:UserPW){
					if(!((i>='0' and i<='9')or(i>='a' and i<='z')or(i>='A' and i<='Z')or i=='_'))throw ErrorException("Invalid");
					if(i<=32 or i>=127)throw ErrorException("Invalid");
				}
				if(std::string(s.begin()->UserPW) == az(UserPW, 30)){
					users.push(*(s.begin()));
					priority = s.begin()->type;
					loggerUsers.push_back(UserID);
					users.top().IsSelect = false;
				}
				else throw ErrorException("Invalid");
			}
		}
		else throw ErrorException("Invalid");
	}
	else if(mode == "logout"){
		if(n>1)throw ErrorException("Invalid");
		if(!users.empty()){
			loggerUsers.pop_back();
			users.pop();
			if(!users.empty())priority = users.top().type;
			else priority = 0;
		}
		else throw ErrorException("Invalid");
	}
	else if(mode == "register"){
		std::string UserID, UserPW, UserName;
		if(n>1)UserID = ps[1];
		else throw ErrorException("Invalid");
		if(n>2)UserPW = ps[2];
		else throw ErrorException("Invalid");
		if(n>3)UserName = ps[3];
		else throw ErrorException("Invalid");
		if(n>4)throw ErrorException("Invalid");
		if(UserID.length()>30)throw ErrorException("Invalid");
		if(UserPW.length()>30)throw ErrorException("Invalid");
		if(UserName.length()>30)throw ErrorException("Invalid");
		for(auto i:UserID){
			if(!((i>='0' and i<='9')or(i>='a' and i<='z')or(i>='A' and i<='Z')or i=='_'))throw ErrorException("Invalid");
			if(i<=32 or i>=127)throw ErrorException("Invalid");
		}
		for(auto i:UserPW){
			if(!((i>='0' and i<='9')or(i>='a' and i<='z')or(i>='A' and i<='Z')or i=='_'))throw ErrorException("Invalid");
			if(i<=32 or i>=127)throw ErrorException("Invalid");
		}
		for(auto i:UserName){
			if(i<=32 or i>=127)throw ErrorException("Invalid");
		}
		user temp;
		strcpy(temp.main_key, az(UserID, 30).c_str());
		std::set<user> s;
		user_manager.f(temp, s);
		if(!s.empty())throw ErrorException("Invalid");
		else{
			strcpy(temp.UserPW, az(UserPW, 30).c_str());
			strcpy(temp.UserName, az(UserName, 30).c_str());
			temp.type = 1;
			user_manager.w(temp);
		}
	}
	else if(mode == "passwd"){
		std::string UserID, CWD, WD;
		if(priority == 0)throw ErrorException("Invalid");
		if(n>1)UserID = ps[1];
		else throw ErrorException("Invalid");
		if(n>2)CWD = ps[2];
		else throw ErrorException("Invalid");
		if(UserID.length()>30)throw ErrorException("Invalid");
		if(CWD.length()>30)throw ErrorException("Invalid");
		for(auto i:UserID){
			if(!((i>='0' and i<='9')or(i>='a' and i<='z')or(i>='A' and i<='Z')or i=='_'))throw ErrorException("Invalid");
			if(i<=32 or i>=127)throw ErrorException("Invalid");
		}
		for(auto i:CWD){
			if(!((i>='0' and i<='9')or(i>='a' and i<='z')or(i>='A' and i<='Z')or i=='_'))throw ErrorException("Invalid");
			if(i<=32 or i>=127)throw ErrorException("Invalid");
		}
		if(n>3){
			WD = ps[3];
			if(WD.length()>30)throw ErrorException("Invalid");
			for(auto i:WD){
				if(!((i>='0' and i<='9')or(i>='a' and i<='z')or(i>='A' and i<='Z')or i=='_'))throw ErrorException("Invalid");
				if(i<=32 or i>=127)throw ErrorException("Invalid");
			}
			if(n>4)throw ErrorException("Invalid");
			user temp;
			strcpy(temp.main_key, az(UserID, 30).c_str());
			std::set<user> s;
			user_manager.f(temp, s);
			if(s.empty())throw ErrorException("Invalid");
			else{
				if(std::string((*s.begin()).UserPW) == az(CWD, 30)){
					user_manager.d(temp);
					strcpy(temp.UserPW, az(WD, 30).c_str());
					strcpy(temp.UserName, s.begin()->UserName);
					temp.type = s.begin()->type;
					user_manager.w(temp);
				}
				else throw ErrorException("Invalid");
			}
		}
		else{
			if(priority == 7){
				user temp;
				strcpy(temp.main_key, az(UserID, 30).c_str());
				std::set<user> s;
				user_manager.f(temp, s);
				if(s.empty())throw ErrorException("Invalid");
				else{
					user_manager.d(temp);
					strcpy(temp.UserPW, az(CWD, 30).c_str());
					strcpy(temp.UserName, s.begin()->UserName);
					temp.type = s.begin()->type;
					user_manager.w(temp);
				}
			}
			else throw ErrorException("Invalid");
		}
	}
	else if(mode == "useradd"){
		if(priority < 3)throw ErrorException("Invalid");
		std::string UserID, PW, p, UserName;
		if(n>1)UserID = ps[1];
		else throw ErrorException("Invalid");
		if(n>2)PW = ps[2];
		else throw ErrorException("Invalid");
		if(n>3)p = ps[3];
		else throw ErrorException("Invalid");
		if(n>4)UserName = ps[4];
		else throw ErrorException("Invalid");
		if(n>5)throw ErrorException("Invalid");
		if(!(p[0]>='0' and p[0]<='9') or p.length()!=1)throw ErrorException("Invalid");
		if(UserID.length()>30)throw ErrorException("Invalid");
		for(auto i:UserID){
			if(!((i>='0' and i<='9')or(i>='a' and i<='z')or(i>='A' and i<='Z')or i=='_'))throw ErrorException("Invalid");
			if(i<=32 or i>=127)throw ErrorException("Invalid");
		}
		if(PW.length()>30)throw ErrorException("Invalid");
		for(auto i:PW){
			if(!((i>='0' and i<='9')or(i>='a' and i<='z')or(i>='A' and i<='Z')or i=='_'))throw ErrorException("Invalid");
			if(i<=32 or i>=127)throw ErrorException("Invalid");
		}
		if(UserName.length()>30)throw ErrorException("Invalid");
		for(auto i:UserName){
			if(i<=32 or i>=127)throw ErrorException("Invalid");
		}
		int pp = p[0] - '0';
		if(pp != 1 and pp != 3 and pp != 7)throw ErrorException("Invalid");
		if(pp >= priority)throw ErrorException("Invalid");
		user temp;
		strcpy(temp.main_key, az(UserID, 30).c_str());
		std::set<user> s;
		user_manager.f(temp, s);
		if(s.empty()){
			strcpy(temp.UserPW, az(PW, 30).c_str());
			strcpy(temp.UserName, az(UserName, 30).c_str());
			temp.type = pp;
			user_manager.w(temp);
		}
		else throw ErrorException("Invalid");
	}
	else if(mode == "delete"){
		if(priority < 7)throw ErrorException("Invalid");
		std::string UserID;
		if(n>1)UserID = ps[1];
		else throw ErrorException("Invalid");
		if(n>2)throw ErrorException("Invalid");
		if(UserID.length()>30)throw ErrorException("Invalid");
		for(auto i:UserID){
			if(!((i>='0' and i<='9')or(i>='a' and i<='z')or(i>='A' and i<='Z')or i=='_'))throw ErrorException("Invalid");
			if(i<=32 or i>=127)throw ErrorException("Invalid");
		}
		user temp;
		strcpy(temp.main_key, az(UserID, 30).c_str());
		std::set<user> s;
		user_manager.f(temp, s);
		if(std::find(loggerUsers.begin(), loggerUsers.end(), UserID) != loggerUsers.end())
			throw ErrorException("Invalid");
		if(s.empty())throw ErrorException("Invalid");
		else user_manager.d(temp);

	}
	else if(mode == "select"){
		if(priority<3)throw ErrorException("Invalid");
		std::string isbn;
		if(n>1)isbn = ps[1];
		else throw ErrorException("Invalid");
		if(n>2)throw ErrorException("Invalid");
		if(isbn.length()>20)throw ErrorException("Invalid");
		for(auto i:isbn){
			if(i<=32 or i>=127)throw ErrorException("Invalid");
		}
		book b;
		strcpy(b.main_key, az(isbn, 30).c_str());
		std::set<book> s;
		book_manager.f(b, s);
		if(s.empty())book_manager.w(b), users.top().select = b;
		else users.top().select = *(s.begin());
		users.top().IsSelect = true;
	}
	else if(mode == "buy"){
		if(priority==0)throw ErrorException("Invalid");
		std::string isbn, quantity;
		if(n>1)isbn = ps[1];
		else throw ErrorException("Invalid");
		if(n>2)quantity = ps[2];
		else throw ErrorException("Invalid");
		if(n>3)throw ErrorException("Invalid");
		if(isbn.length()>20)throw ErrorException("Invalid");
		for(auto i:isbn){
			if(i<=32 or i>=127)throw ErrorException("Invalid");
		}
		if(quantity.length()>10)throw ErrorException("Invalid");
		int q = 0;
		for(auto i: quantity){
			if(i >= '0' and i <= '9')q = q * 10 + (i - '0');
			else throw ErrorException("Invalid");
		}
		if(q <= 0)throw ErrorException("Invalid");
		book b;
		strcpy(b.main_key, az(isbn, 30).c_str());
		std::set<book> s;
		book_manager.f(b, s);
		if(s.empty())throw ErrorException("Invalid");
		b = *(s.begin());
		if(b.quantity >= q)b.quantity -= q;
		else throw ErrorException("Invalid");
		book_manager.d(b);
		book_manager.w(b);
		printf("%.2f\n" ,b.price * q);
		double n,m;
		if(!finance.empty()){
			n = finance[0].first, m = finance[0].second;
		}else n=0,m=0;
		n+=b.price*q;
		finance.insert(finance.begin(), std::make_pair(n,m));
	}
	else if(mode == "modify"){
		std::string input;
		if(priority<3)throw ErrorException("Invalid");
		bool isbn_b = false, nams_b = false, author_b = false, keyword_b = false, price_b = false;
		if(!users.top().IsSelect)throw ErrorException("Invalid");
		std::set<book>ss;
		book_manager.f(users.top().select,ss);
		if(!ss.empty())users.top().select = *(ss.begin());
		if(n==1)throw ErrorException("Invalid");
		int mei = 1;
		input = "modify "+ez(std::string(users.top().select.main_key));
		while (mei<n){
			std::string m = ps[mei];
			input+=" " + ps[mei];
			mei++;
			if(m[0] != '-'){
				throw ErrorException("Invalid");
			}
			if(m[1] == 'n'){//name
				if(nams_b)throw ErrorException("Invalid");
				nams_b = true;
				if(m.length()<=8)throw ErrorException("Invalid");
				std::string check;
				for(int i=0;i<7;i++)check+=m[i];
				if(check!="-name=\"")throw ErrorException("Invalid");
				if(m[m.length()-1]!='\"')throw ErrorException("Invalid");
				std::string name;
				for(int i = 7; i < m.length() - 1; i++){
					name += m[i];
				}
				if(name.length()>60)throw ErrorException("Invalid");
				for(auto i:name){
					if(i=='\"' or i<=32 or i>=127)throw ErrorException("Invalid");
				}
				strcpy(users.top().select.Bookname, az(name, 60).c_str());
			}
			else if(m[1] == 'k'){//keyword
				if(keyword_b)throw ErrorException("Invalid");
				keyword_b = true;
				if(m.length()<=11)throw ErrorException("Invalid");
				std::string check;
				for(int i=0;i<10;i++)check+=m[i];
				if(check!="-keyword=\"")throw ErrorException("Invalid");
				if(m[m.length()-1]!='\"')throw ErrorException("Invalid");
				std::string keyword;
				for(int i = 10; i < m.length() - 1; i++){
					keyword += m[i];
				}
				if(keyword.length()>60)throw ErrorException("Invalid");
				for(auto i:keyword){
					if(i=='\"' or i<=32 or i>=127)throw ErrorException("Invalid");
				}
				std::string tt = keyword+"|";
				std::string temp;
				std::vector<std::string>ttttt;
				temp = "";
				for(auto i:tt){
					if(i=='|'){
						if(temp.empty())throw ErrorException("Invalid");
						if(std::find(ttttt.begin(), ttttt.end(),temp)!=ttttt.end())throw ErrorException("Invalid");
						else ttttt.push_back(temp);
						temp = "";
					}
					else temp+=i;
				}
				strcpy(users.top().select.Keyword, az(keyword, 60).c_str());
			}
			else if(m[1] == 'a'){//author
				if(author_b)throw ErrorException("Invalid");
				author_b = true;
				if(m.length()<=10)throw ErrorException("Invalid");
				std::string check;
				for(int i=0;i<9;i++)check+=m[i];
				if(check!="-author=\"")throw ErrorException("Invalid");
				if(m[m.length()-1]!='\"')throw ErrorException("Invalid");
				std::string author;
				for(int i = 9; i < m.length() - 1; i++){
					author += m[i];
				}
				if(author.length()>60)throw ErrorException("Invalid");
				for(auto i:author){
					if(i=='\"' or i<=32 or i>=127)throw ErrorException("Invalid");
				}
				strcpy(users.top().select.Author, az(author, 60).c_str());
			}
			else if(m[1] == 'I'){//ISBN
				if(isbn_b)throw ErrorException("Invalid");
				isbn_b = true;
				if(m.length()<=6)throw ErrorException("Invalid");
				std::string check;
				for(int i=0;i<6;i++)check+=m[i];
				if(check!="-ISBN=")throw ErrorException("Invalid");
				std::string isbn;
				for(int i = 6; i < m.length(); i++){
					isbn += m[i];
				}
				if(isbn.length()>20)throw ErrorException("Invalid");
				for(auto i:isbn){
					if(i<=32 or i>=127)throw ErrorException("Invalid");
				}
				if(std::string(users.top().select.main_key) == az(isbn, 30))throw ErrorException("Invalid");
				book temp;
				strcpy(temp.main_key,az(isbn,30).c_str());
				std::set<book>s;
				book_manager.f(temp,s);
				if(!s.empty())throw ErrorException("Invalid");
				book_manager.d(users.top().select);
				std::stack<user>usert;
				std::string yuanbende = std::string(users.top().select.main_key);
				while(!users.empty()){
					if(users.top().IsSelect){
						if(std::string (users.top().select.main_key)==yuanbende){
							strcpy(users.top().select.main_key,az(isbn,30).c_str());
						}
					}
					usert.push(users.top());
					users.pop();
				}
				while(!usert.empty()){
					users.push(usert.top());
					usert.pop();
				}
				strcpy(users.top().select.main_key, az(isbn, 30).c_str());
			}
			else if(m[1] == 'p'){//price
				if(price_b)throw ErrorException("invalid");
				price_b = true;
				if(m.length()>20)throw ErrorException("Invalid");
				if(m.length()<=7)throw ErrorException("Invalid");
				std::string check;
				for(int i=0;i<7;i++)check+=m[i];
				if(check!="-price=")throw ErrorException("Invalid");
				double price=0;
				int n = 0;
				bool flag = false;
				if(m[m.length()-1]=='.')throw ErrorException("Invalid");
				if(m[7]=='.')throw ErrorException("Invalid");
				for(int i = 7; i < m.length(); i++){
					if((m[i]<'0' or m[i]>'9') and m[i]!='.')throw ErrorException("Invalid");
					if(m[i]<=31 or m[i]>=127)throw ErrorException("Invalid");
					if(flag)n++;
					if(m[i] == '.'){
						if(flag)throw ErrorException("Invalid");
						flag = true;
					}
					if(m[i]>='0' and m[i]<='9')price = price * 10 + m[i] - '0';
				}

				for(int i = 0; i < n; i++)price /= 10;
				users.top().select.price = price;
			}
			else throw ErrorException("Invalid");
		}
		book_manager.d(users.top().select);
		book_manager.w(users.top().select);
		cmds.pop_back();
		cmds.push_back(input);
	}
	else if(mode == "import"){
		if(priority<3)throw ErrorException("Invalid");
		std::string Quantity, TotalCost;
		if(n>1)Quantity = ps[1];
		else throw ErrorException("Invalid");
		if(n>2)TotalCost = ps[2];
		else throw ErrorException("Invalid");
		if(n>3)throw ErrorException("Invalid");
		if(Quantity.length()>10)throw ErrorException("Invalid");
		if(TotalCost.length()>13)throw ErrorException("Invalid");
		if(!users.top().IsSelect)throw ErrorException("Invalid");
		std::set<book>s;
		book_manager.f(users.top().select,s);
		if(!s.empty())users.top().select = *(s.begin());
		int q = 0;
		double t = 0;
		for(auto i: Quantity){
			if(i<=31 or i==127)throw ErrorException("Invalid");
			if(i >= '0' and i <= '9'){
				q = q * 10 + i - '0';
			}
			else throw ErrorException("Invalid");
		}
		if(q==0)throw ErrorException("Invalid");
		bool flag= false;
		int dsa=0;
		if(TotalCost[TotalCost.length()-1]=='.')throw ErrorException("Invalid");
		if(TotalCost[0]=='.')throw ErrorException("Invalid");
		for(auto i: TotalCost){
			if(flag)dsa++;
			if(i<=31 or i>=127)throw ErrorException("Invalid");
			if(i >= '0' and i <= '9'){
				t = t * 10 + i - '0';
			}
			if((i<'0' or i>'9') and i!='.')throw ErrorException("Invalid");
			if(i=='.'){
				if(flag)throw ErrorException("Invalid");
				flag = true;
			}
		}

		for(int i=0;i<dsa;i++)t/=10;
		if(t<=0)throw ErrorException("Invalid");
		users.top().select.quantity += q;
		book_manager.d(users.top().select);
		book_manager.w(users.top().select);
		double n,m;
		if(!finance.empty()){
			n = finance[0].first, m = finance[0].second;
		}else n=0,m=0;
		m+=t;
		finance.insert(finance.begin(), std::make_pair(n,m));
		std::string input = "import "+ez(std::string(users.top().select.main_key)) + " "+ Quantity + " "+ TotalCost;
		cmds.pop_back();
		cmds.push_back(input);
	}
	else if(mode == "show"){
		if(priority==0)throw ErrorException("Invalid");
		if(n>1){
			std::string pattern = ps[1];
			if(pattern[0] != '-'){
				if(pattern == "finance"){
					if(priority!=7)throw ErrorException("Invalid");
					if(n>2){
						std::string count = ps[2];
						if(n>3)throw ErrorException("Invalid");
						if(count.length()>10)throw ErrorException("Invalid");
						long c=0;
						for(auto i:count){
							if(i>='0' and i<='9')c=c*10+i-'0';
							else throw ErrorException("Invalid");
						}
						if(c>2147483647)throw ErrorException("Invalid");
						if(c==0)std::cout << '\n';
						else{
							if(c>finance.size())throw ErrorException("Invalid");
							else if(c==finance.size()){
								if(finance.empty())std::cout << "+ 0.00 - 0.00\n";
								else printf("+ %.2f - %.2f\n",finance[0].first,finance[0].second);
							}
							else{
								if(finance.empty())std::cout << "+ 0.00 - 0.00\n";
								else printf("+ %.2f - %.2f\n",finance[0].first-finance[c].first,finance[0].second-finance[c].second);
							}
						}
					}else{
						if(finance.empty())std::cout << "+ 0.00 - 0.00\n";
						else printf("+ %.2f - %.2f\n",finance[0].first,finance[0].second);
					}
				}else throw ErrorException("Invalid");
			}
			else{
				if(n>2)throw ErrorException("Invalid");
				int ans = 0;
				std::string p;
				if(pattern[1] == 'I'){
					if(pattern.length() <= 6)throw ErrorException("Invalid");
					std::string check;
					for(int i=0;i<6;i++)check+=pattern[i];
					if(check!="-ISBN=")throw ErrorException("Invalid");
					for(int i = 6; i < pattern.length(); i++){
						p += pattern[i];
					}
					if(p.length()>20)throw ErrorException("Invalid");
					for(auto i:p){
						if(i=='\"' or i<=31 or i>=127)throw ErrorException("Invalid");
					}
				}
				else if(pattern[1] == 'n'){
					if(pattern.length() <= 8)throw ErrorException("Invalid");
					std::string check;
					for(int i=0;i<7;i++)check+=pattern[i];
					if(check!="-name=\"")throw ErrorException("Invalid");
					if(pattern[pattern.length()-1]!='\"')throw ErrorException("Invalid");
					for(int i = 7; i < pattern.length() - 1; i++){
						p += pattern[i];
					}
					if(p.length()>60)throw ErrorException("Invalid");
					for(auto i:p){
						if(i=='\"' or i<=31 or i>=127)throw ErrorException("Invalid");
					}
				}
				else if(pattern[1] == 'a'){
					if(pattern.length() <= 10)throw ErrorException("Invalid");
					std::string check;
					for(int i=0;i<9;i++)check+=pattern[i];
					if(check!="-author=\"")throw ErrorException("Invalid");
					if(pattern[pattern.length()-1]!='\"')throw ErrorException("Invalid");
					for(int i = 9; i < pattern.length() - 1; i++){
						p += pattern[i];
					}
					if(p.length()>60)throw ErrorException("Invalid");
					for(auto i:p){
						if(i=='\"' or i<=31 or i>=127)throw ErrorException("Invalid");
					}
				}
				else if(pattern[1] == 'k'){
					if(pattern.length() <= 11)throw ErrorException("Invalid");
					std::string check;
					for(int i=0;i<10;i++)check+=pattern[i];
					if(check!="-keyword=\"")throw ErrorException("Invalid");
					if(pattern[pattern.length()-1]!='\"')throw ErrorException("Invalid");
					for(int i = 10; i < pattern.length() - 1; i++){
						p += pattern[i];
					}
					if(p.length()>60)throw ErrorException("Invalid");
					for(auto i:p){
						if(i=='\"' or i<=31 or i>=127 or i=='|')throw ErrorException("Invalid");
					}
				}else throw ErrorException("Invalid");
				std::fstream temp(book_manager.filename);
				for(int i = 0; i < book_manager.num; i++){
					writer<book>::block tt;
					temp.read(reinterpret_cast<char*>(&tt), book_manager.unit);
					for(int j = 0; j < tt.len; j++){
						bool flag = false;
						if(pattern[1] == 'I'){
							if(ez(std::string(tt.books[j].main_key)) == p)flag = true;
						}
						else if(pattern[1] == 'n'){
							if(ez(std::string(tt.books[j].Bookname)) == p)flag = true;
						}
						else if(pattern[1] == 'a'){
							if(ez(std::string(tt.books[j].Author)) == p)flag = true;
						}
						else if(pattern[1] == 'k'){
							std::string yuanbende = ez(std::string(tt.books[j].Keyword));
							if(std::count(p.begin(), p.end(), '|'))throw ErrorException("Invalid");
							yuanbende+='|';
							std::string tttt;
							for(auto cha:yuanbende){
								if(cha=='|'){
									if(tttt == p)flag = true;
									tttt = "";
								}
								else tttt+=cha;
							}
						}else throw ErrorException("Invalid");
						if(flag){
							std::cout << ez(std::string(tt.books[j].main_key)) << '\t'
							          << ez(std::string(tt.books[j].Bookname)) << '\t'
							          << ez(std::string(tt.books[j].Author)) << '\t'
							          << ez(std::string(tt.books[j].Keyword)) << '\t';

							printf("%.2f", tt.books[j].price);

							std::cout << '\t' << tt.books[j].quantity << '\n';
							ans++;
						}
					}
				}
				if(ans == 0)std::cout << '\n';
			}
		}
		else{
			std::fstream temp(book_manager.filename);
			for(int i = 0; i < book_manager.num; i++){
				writer<book>::block tt;
				temp.read(reinterpret_cast<char*>(&tt), book_manager.unit);
				for(int j = 0; j < tt.len; j++){
					std::cout << ez(std::string(tt.books[j].main_key)) << '\t' << ez(std::string(tt.books[j].Bookname))
					          << '\t' << ez(std::string(tt.books[j].Author)) << '\t'
					          << ez(std::string(tt.books[j].Keyword)) << '\t';

					printf("%.2f", tt.books[j].price);

					std::cout << '\t'  << tt.books[j].quantity << '\n';
				}
			}
			if(book_manager.num==0)std::cout << '\n';
		}
	}
	else if(mode == "log"){
		if(priority != 7)throw ErrorException("Invalid");
		TokenScanner ss;
		ss.addWordCharacters(",.<>/?;'\":[]{}\\|-=_+()&*%^#$!@~`");
		ss.scanStrings();
		ss.ignoreWhitespace();
		int nn = 0;
		std::cout << ANSI_COLOR_OLD_YELLOW
		          << "=============================================================BEGIN LOG=============================================================\n"
		          << ANSI_COLOR_RESET;
		for(auto i: cmds){
			ss.setInput(i);
			std::string mode_ = ss.nextToken();
			if(mode_ == "su"){
				std::cout << ANSI_COLOR_CYAN;
				for(int j = 0; j <= nn; j++)std::cout << " | ";
				std::cout << ANSI_COLOR_GREEN;
				if(nn != 0){
					std::cout << " \\ ";
				}
				nn++;
				std::string id = ss.nextToken();
				std::cout << id << '\n' << ANSI_COLOR_RESET;
			}
			else if(mode_ == "logout"){
				std::cout << ANSI_COLOR_CYAN;
				for(int j = 0; j <= nn - 1; j++)std::cout << " | ";
				nn--;
				std::cout << ANSI_COLOR_GREEN;
				std::cout << " / \n" << ANSI_COLOR_RESET;
			}
			else if(mode_ == "exit" or mode_ == "quit"){
				std::cout << ANSI_COLOR_MAGENTA;
				for(int j = 0; j <= nn; j++)std::cout << "===";
				std::cout << '\n' << ANSI_COLOR_RESET;
				nn = 0;
			}
			else if(mode_ == "register"){
				std::cout << ANSI_COLOR_CYAN;
				for(int j = 0; j <= nn; j++)std::cout << " | ";
				std::string UserID, UserPW, UserName;
				UserID = ss.nextToken();
				UserPW = ss.nextToken();
				UserName = ss.nextToken();
				std::cout << ANSI_COLOR_BLUE;
				std::cout << "Useradd\t" << ANSI_COLOR_RED << "type:  " << ANSI_COLOR_YELLOW << "1\t" << ANSI_COLOR_RED
				          << "ID:  " << ANSI_COLOR_GREEN << UserID << ANSI_COLOR_RED << "\tPassword:  "
				          << ANSI_COLOR_YELLOW << UserPW << ANSI_COLOR_RED << "\tName:  " << ANSI_COLOR_MAGENTA
				          << UserName << '\n' << ANSI_COLOR_RESET;
			}
			else if(mode_ == "useradd"){
				std::cout << ANSI_COLOR_CYAN;
				for(int j = 0; j <= nn; j++)std::cout << " | ";
				std::string UserID, PW, p, UserName;
				UserID = ss.nextToken();
				PW = ss.nextToken();
				p = ss.nextToken();
				UserName = ss.nextToken();
				std::cout << ANSI_COLOR_BLUE;
				std::cout << "Useradd\t" << ANSI_COLOR_RED << "type:  " << ANSI_COLOR_YELLOW << p << "\t"
				          << ANSI_COLOR_RED << "ID:  " << ANSI_COLOR_GREEN << UserID << ANSI_COLOR_RED
				          << "\tPassword:  " << ANSI_COLOR_YELLOW << PW << ANSI_COLOR_RED << "\tName:  "
				          << ANSI_COLOR_MAGENTA << UserName << '\n' << ANSI_COLOR_RESET;
			}
			else if(mode_ == "passwd"){
				std::cout << ANSI_COLOR_CYAN;
				for(int j = 0; j <= nn; j++)std::cout << " | ";
				std::string UserID, WD;
				UserID = ss.nextToken();
				WD = ss.nextToken();
				if(ss.hasMoreTokens())WD = ss.nextToken();
				std::cout << ANSI_COLOR_BLUE;
				std::cout << "ChangePassword\t" << ANSI_COLOR_RED << "ID:  " << ANSI_COLOR_GREEN << UserID
				          << ANSI_COLOR_RED << "\tNewPassWord:  " << ANSI_COLOR_YELLOW << WD << '\n'
				          << ANSI_COLOR_RESET;
			}
			else if(mode_ == "delete"){
				std::cout << ANSI_COLOR_CYAN;
				for(int j = 0; j <= nn; j++)std::cout << " | ";
				std::string UserID = ss.nextToken();
				std::cout << ANSI_COLOR_BLUE;
				std::cout << "DeleteUser\t" << ANSI_COLOR_RED << "ID:  " << ANSI_COLOR_GREEN << UserID << '\n'
				          << ANSI_COLOR_RESET;
			}
			else if(mode == "buy"){
				std::cout << ANSI_COLOR_CYAN;
				for(int j = 0; j <= nn; j++)std::cout << " | ";
				std::string isbn = ss.nextToken();
				std::string quantity = ss.nextToken();
				std::cout << ANSI_COLOR_BLUE;
				std::cout << "Bought\t" << ANSI_COLOR_RED << "ISBN:  " << ANSI_COLOR_GREEN << isbn << ANSI_COLOR_RED
				          << "\tQuantity:  " << ANSI_COLOR_YELLOW << quantity << '\n' << ANSI_COLOR_RESET;
			}
			else if(mode == "modify"){
				std::cout << ANSI_COLOR_CYAN;
				for(int j = 0; j <= nn; j++)std::cout << " | ";
				std::string isbn = ss.nextToken();
				std::cout << ANSI_COLOR_BLUE;
				std::cout << "Modify\t" << ANSI_COLOR_RED << "ISBN:  " << ANSI_COLOR_GREEN << isbn;
				std::cout << ANSI_COLOR_YELLOW;
				while (ss.hasMoreTokens()){
					std::string p = ss.nextToken();
					std::cout << "\t" << p;
				}
				std::cout << '\n' << ANSI_COLOR_RESET;
			}
			else if(mode == "import"){
				std::cout << ANSI_COLOR_CYAN;
				for(int j = 0; j <= nn; j++)std::cout << " | ";
				std::string isbn = ss.nextToken();
				std::string quantity = ss.nextToken();
				std::string t = ss.nextToken();
				std::cout << ANSI_COLOR_BLUE;
				std::cout << "Import\t" << ANSI_COLOR_RED << "ISBN:  " << ANSI_COLOR_GREEN << isbn << ANSI_COLOR_RED
				          << "\tQuantity:  " << ANSI_COLOR_YELLOW << quantity << ANSI_COLOR_RED << "\tTotalCost"
				          << ANSI_COLOR_YELLOW << t << '\n' << ANSI_COLOR_RESET;
			}
			else continue;
		}
		std::cout << ANSI_COLOR_OLD_YELLOW
		          << "============================================================= FINANCE =============================================================\n";
		double nnn=0,mmm=0;
		if(!finance.empty())nnn=finance[0].first,mmm=finance[0].second;
		std::cout << ANSI_COLOR_GREEN << " + " << ANSI_COLOR_RED << nnn << ANSI_COLOR_GREEN << " - "
		          << ANSI_COLOR_BLUE << mmm << '\n' << ANSI_COLOR_RESET;
		if(ss.hasMoreTokens() and ss.nextToken() == "jy"){
			std::cout
				<< "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n@@@@@@@@@@@@@@@@000000@@000000@0000000000000000000000@@@0000000@00000000000000000000@@00@@@@@@@@@@@@\n";
			std::cout
				<< "@@@@@@@@@@@@@@@@@@@0000@@@0@@0000000000000@@@000000@00000000@@0000@0000000@000000@@@@00@@@@@@@@@@@@@\n";
			std::cout
				<< "@@@@@@@@@@@@@@@@00@@@@00000000000000000000000000000000000000@0000000@0000000000000000000@@000000@@@@\n";
			std::cout
				<< "@@@@@@@@@@@@@@@@@@@@0@@@000@0000@000000@@0000000008CGC000@00@@000000@00000000CGGC00000@@@@000000@@@@\n";
			std::cout
				<< "@@@@@@@@@@@@@@@@00@@00000000CGC8000@00000000000008CCCC000@000000000000@000CCCCGC800000@@@@000000@@@@\n";
			std::cout
				<< "@@@@@@@@@@@@@@@0000@00000000CCCGCC80000000000008CCGCCC8000@0000000000000CGCGGCC80000000@@@000000@@@@\n";
			std::cout
				<< "@@@@@@@@@@@@@@@000@0000000000CCCCCCC800000008CCCGf,tGC8000@0000000000CCGCC1,LCC80@@@@00@@@@@@@@@@@@@\n";
			std::cout
				<< "@@@@@@@@@@@@@@@@000000000000008CCGLGGGCCCCGCGGt;. .:CCC00000000@000CCCGt,..tCG800@@@@@0@@@@@@@@@@@@@\n";
			std::cout
				<< "@@@@@@@@000000080000000@00000008GGL..,:;i1;,.... ..,LCC000000008CCGGGi....:GCC0000@00000000000@@0000\n";
			std::cout
				<< "@@@@00@@00000CCCCCCC880000000000CGC;......  ..... . ;CCLC88CCGCCCLi,  ....iCCC0000000000000000000000\n";
			std::cout
				<< "@@@@00000@000008CGCCGGGCCCC88888CCGi  ...... ... .. .:fCCCGCGLi...........tCC800000000000008CCCCG800\n";
			std::cout
				<< "@@@@0000000@000000CCCCftGCCCGGGGGLi...  ............ ............. .... ..LGC0000000008CCGGCCCGC8000\n";
			std::cout
				<< "@@@@00000000@@@00000CGCC;,...............................................:GCC800088CCCCCGftCGCC00000\n";
			std::cout
				<< "@@@@@@0000000000000008CCCL................................................tGGCCCCCGGL1;,iGCGG0000000\n";
			std::cout
				<< "00@@@@@@@@@@@@@000000008CGL,................................................;;;;:,.. .;GGCC800000000\n";
			std::cout
				<< "0000@@@@@@@@@@@@00000008CCC;............................................ .......... ;GCCC0000000@0@@\n";
			std::cout
				<< "00@@0@008888888CCCGCGGCGCGf............. .... ....  .. ................... .....  :LCCC000@0@@000@@@\n";
			std::cout
				<< "@@00@@08CCCCCCCCCLft1;, .. ............. ..  .......... ................. .. ...;LGCC8000000000@@@@@\n";
			std::cout
				<< "0@@@00@000008CCCCL1,.. .. ... ..............,:,..... .................... . .. 1GCC80000000000000@@@\n";
			std::cout
				<< "000000@0@0000000CGCCGt,.....................fGG,....:GCi................ ..  1CCCC0000@0000000@@@@@@\n";
			std::cout
				<< "0000@@00000@@@000008GCGi.................. ..,,... .,i1,....................tGCC00000000000000000@@@\n";
			std::cout
				<< "@@@@@@@@@@00@@@000008CG1.....................GCG8CGGCi................... .fGC800000000000000000@@@@\n";
			std::cout
				<< "@@@@@@@@@@000000008CGGf..................... iGCCCCGf:....................;GCC000000000000000000@@@@\n";
			std::cout
				<< "00@@@@00000000008CGGL: ................... .   . ... .. ................. ,fCCCCCC888888880000000@@@\n";
			std::cout
				<< "0@000000000000CCCCL,.. ................................................. ...:tGGCCGGCCCCGGGCCCGC0000\n";
			std::cout
				<< "00000000000CCCCGi,..... ...................................................  ...... . ..   ,1GCCCCC8\n";
			std::cout
				<< "000000008GGGGf,....... ................................................. ....  ........;LGGGCCCGCCC0\n";
			std::cout
				<< "000008CCGCCCCGCGCGLL1.................................................... ... ......;LCCCCC880000000\n";
			std::cout
				<< "008CCCCCCCCCCCCCCCCCGCGt.......................................................... iCGG8000000000000\n";
			std::cout
				<< "CCCCCC000000000000000CGG..........................................................,LCC80000000000@00\n";
			std::cout
				<< "0800000000000@000000CCG1  ....................................................... .LGC0000@000000@@@\n";
			std::cout
				<< "00000000000000@0008CCC1........ ...................................................1GGG00000000000@0\n";
			std::cout
				<< "0000000000000000CCCCCCGCGG1,....  ................................... ..   .. ......GCG80000@0000@@@\n";
			std::cout
				<< "@@000000000008CGCCCCCC888CCGf. .....  .................. ... .... .... .. ..;ifLGGGGCCG8C800000@@@@@\n";
			std::cout
				<< "00@0000@0008CCCGG8000000008CGffGCCGCft;,....... .............:ittt;,... .:GG8GGCGCCGGCCCCCC800000@@@\n";
			std::cout
				<< "0000000008CCCC80000@@@0000CCGGCCCCCCCCCC;.. ............. .,CCGCGGCCGGf:.tGG800000000000000000000@@@\n";
			std::cout
				<< "@0@0000CCGGC8000000@0000000088008CCL,................... ....,,.,LCCCCGCGCGCC80000000000000000000@@@\n";
			std::cout
				<< "00000008C800000@0000000000000000CCCi. ... .............. .... .:tGCC0008CCCCCC8000000000@@@@@@@@@@@@\n";
			std::cout
				<< "0000000@@00@@00000000000@00000000CGGCCCGG1...................,fCCGC0000@0008CG8000000000000000000@@@\n";
			std::cout
				<< "000000000@0000@0000000000@00000@008CGCGCGi..... .............;CGC0000@00@0000@00@0@@@0@0000@@000@@@@\n";
			std::cout
				<< "000@@000@@@0@@@@@@@@@@0@@@@@@@@0000000CCC;.  ... .  ... . .. fCC8000@00@@@000000@@@@@@@@@@@@@@@@@@@@\n";
			std::cout
				<< "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@000@000CCCi  . ...  ... ......GGC0@@0@@0@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
			std::cout
				<< "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@0000000CCG;.. .. ......  ,::::GCC000@@0@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
			std::cout
				<< "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@008CGGCCG;....  ....  .iCGGCGCGC80@0@@0@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
			std::cout
				<< "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@0CCCGf1i:........ ....1GCGi:fCCC80@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
			std::cout
				<< "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@0CCCf,.. ... ....  ...1GCL, ,fGC80@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
			std::cout
				<< "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@00CCCCt11ii;;;ii11i11;iLCCCCGCCC00@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
			std::cout
				<< "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@0008CG8CCCCCCCCCCCCGCCCCCGCCGCC000@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
			std::cout
				<< "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@0000000000008800000000000000000@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
			std::cout
				<< "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
		}
	}
	else if(mode.empty()){
		cmds.pop_back();
		return;
	}
	else throw ErrorException("Invalid");
}

std::string Interpreter::az(const std::string &s, const int n){
	std::string ans=s;
	for(int i=0;i<n-s.length();i++)ans+=' ';
	return ans;
}
std::string Interpreter::ez(const std::string &s){
	std::string ans;
	for(auto i:s)
		if(i!=' ')ans+=i;
	return  ans;
}

Interpreter::~Interpreter(){
	std::ofstream os("finance");
	for(auto &i:finance){
		os.write(reinterpret_cast<char *>(&(i.first)), sizeof(double));
		os.write(reinterpret_cast<char *>(&(i.second)), sizeof(double));
	}
	os.close();
	os.open("cmds");
	for(auto i:cmds){
		char temp[301];
		strcpy(temp,i.c_str());
		os.write(temp, sizeof(temp));
	}
	os.close();
}

bool Interpreter::p(){
	std::string p = ps[0];
	if(p=="quit" or p=="exit")return true;
	else{
		return false;
	}
}
