//
// Created by 14908 on 2022/12/5.
//
#include "interpreter.h"
#include "error.hpp"
#include <fstream>
#include <string>
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
	if(l==0)finance.clear();
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
		if(priority<3)throw ErrorException("Invalid");
		bool isbn_b = false, nams_b = false, author_b = false, keyword_b = false, price_b = false;
		if(!users.top().IsSelect)throw ErrorException("Invalid");
		std::set<book>ss;
		book_manager.f(users.top().select,ss);
		if(!ss.empty())users.top().select = *(ss.begin());
		if(n==1)throw ErrorException("Invalid");
		int mei = 1;
		while (mei<n){
			std::string m = ps[mei];
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
		}
	}
	else if(mode == "log"){
		if(priority!=7)throw ErrorException("Invalid");
		if(n>1)throw ErrorException("Invalid");
	}
	else if(mode.empty()){
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
}

bool Interpreter::p(){
	std::string p = ps[0];
	if(p=="quit" or p=="exit")return true;
	else{
		return false;
	}
}
