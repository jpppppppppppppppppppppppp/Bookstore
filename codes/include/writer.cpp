//
// Created by 14908 on 2022/12/18.
//
#include <iostream>
#include <fstream>
#include <set>
#include <cstring>
#include <algorithm>
#define max_s 300
#define min_s 150
template<class T>
class writer{
public:
	int num = 0;
	std::string filename;
	std::string index_filename;
	std::string num_filename;
	int unit=0;
	T index[500];

	struct block {
		int len = 0;
		char begin[31];
		char end[31];
		T books[max_s];
	};

	void setup(const std::string& fname){
		filename = fname;
		index_filename = fname + "_index";
		num_filename = fname + "_num";
		unit = sizeof(block);
		std::fstream os;
		os.open(num_filename);
		os.seekg(0, os.end);
		int l = os.tellg();
		if(l==0)num=0;
		else{
			os.seekg(0);
			os.read(reinterpret_cast<char *>(&num), sizeof(int));
		}
		os.close();
		os.open(index_filename);
		os.seekg(0,os.end);
		l = os.tellg();
		if(l!=0){
			os.seekg(0);
			os.read(reinterpret_cast<char *>(&index), sizeof(index));
		}
		os.close();
	}
	~writer(){
		std::ofstream o(num_filename);
		o.write(reinterpret_cast<char *>(&num),sizeof(int));
		o.close();
		o.open(index_filename);
		o.write(reinterpret_cast<char *>(&index),sizeof(index));
		o.close();
	}
	void w(T &temp){
		if(num == 0){
			num++;
			block t;
			t.len = 1;
			strcpy(t.begin, temp.main_key);
			strcpy(t.end, temp.main_key);
			t.books[0] = temp;
			std::ofstream out(filename);
			out.write(reinterpret_cast<char*>(&t), unit);
			out.close();
			index[0] = temp;
			return;
		}
		int nth = std::lower_bound(index, index+num, temp) - index;
		nth = std::max(nth-1,0);
		std::fstream io;
		io.open(filename);
		for(int j = nth; j < num; j++){
			io.seekg(j * unit);
			block body;
			io.read(reinterpret_cast<char*>(&body), unit);
			if(body.len < max_s){
				if(std::string (temp.main_key) <std::string (body.begin)){
					strcpy(body.begin, temp.main_key);
					for(int i = body.len; i > 0; i--){
						body.books[i] = body.books[i - 1];
					}
					body.books[0] = temp;
					body.len++;
					io.seekg(j * unit);
					io.write(reinterpret_cast<char*>(&body), unit);
					io.close();
					index[j] = temp;
					return;
				}
				else if(std::string (temp.main_key) <=std::string (body.end)){
					if(temp < index[j])index[j] = temp;
					long insert = std::upper_bound(body.books, body.books + body.len, temp) - body.books;
					//cerr << insert << endl;
					if(insert != 0){
						if(body.books[insert - 1] == temp)return;
						if(std::string(body.books[insert - 1].main_key) == std::string(temp.main_key) and insert == body.len and
						   j + 1 != num)
							continue;
					}
					for(int i = body.len; i > insert; i--){
						body.books[i] = body.books[i - 1];
					}
					body.books[insert] = temp;
					body.len++;
					io.seekg(j * unit);
					io.write(reinterpret_cast<char*>(&body), unit);
					io.close();
					return;
				}
				else{
					if(j + 1 != num){
						io.seekg((j + 1) * unit);
						block n;
						io.read(reinterpret_cast<char*>(&n), unit);
						if(temp.main_key >= n.begin){
							continue;
						}
					}
					strcpy(body.end, temp.main_key);
					body.books[body.len] = temp;
					body.len++;
					io.seekg(j * unit);
					io.write(reinterpret_cast<char*>(&body), unit);
					io.close();
					return;
				}
			}
			if(body.len == max_s){
				if(std::string (temp.main_key) >std::string (body.end) and j + 1 != num)continue;
				else{
					num++;
					block n;
					strcpy(body.end, body.books[min_s - 1].main_key);
					strcpy(n.begin, body.books[min_s].main_key);
					strcpy(n.end, body.books[max_s - 1].main_key);
					for(int i = 0; i < min_s; i++){
						n.books[i] = body.books[min_s + i];
						T b {};
						body.books[min_s + i] = b;
					}
					body.len = min_s, n.len = min_s;
					for(int t = num - 2; t >= j; t--){
						io.seekg(t * unit);
						block tt;
						io.read(reinterpret_cast<char*>(&tt), unit);
						io.write(reinterpret_cast<char*>(&tt), unit);
						index[t + 1] = index[t];
					}
					io.seekg(j * unit);
					io.write(reinterpret_cast<char*>(&body), unit);
					io.write(reinterpret_cast<char*>(&n), unit);
					index[j + 1] = n.books[0];
					j--;
				}
			}

		}
	}

	void d(T &temp){
		int nth = std::lower_bound(index, index+num, temp) - index;
		nth = std::max(nth-1,0);
		for(int j = nth; j < num; j++){
			std::fstream io;
			io.open(filename);
			block body;
			io.seekg(j * unit);
			io.read(reinterpret_cast<char*>(&body), unit);
			if(std::string(temp.main_key) < std::string (body.begin))return;
			else if(std::string (temp.main_key) <=std::string (body.end)){
				char pb[31], pe[31];
				if(body.len > 1){
					strcpy(pb, body.books[1].main_key);
					strcpy(pe, body.books[body.len - 2].main_key);
				}
				if(temp > body.books[body.len - 1]){
					continue;
				}
				long del = std::lower_bound(body.books, body.books + body.len, temp) - body.books;
				if(temp == body.books[del]){
					for(int i = del; i < body.len - 1; i++){
						body.books[i] = body.books[i + 1];
					}
					if(del == 0)index[j] = body.books[0];
					body.len--;
					if(body.len != 0){
						if(temp.main_key == body.begin and temp.main_key != pb)strcpy(body.begin, pb);
						if(temp.main_key == body.end and temp.main_key != pe)strcpy(body.end, pe);
						io.seekg(j * unit);
						io.write(reinterpret_cast<char*>(&body), unit);
					}
					else{
						int ttt = (num - j - 1) * unit;
						char tt[ttt];
						io.seekg((j + 1) * unit);
						io.read(tt, ttt);
						io.seekg(j * unit);
						io.write(tt, ttt);
						num--;
						for(int i = j; i <= num; i++){
							index[i] = index[i + 1];
						}
						return;
					}
				}
				else{
					continue;
				}
				return;
			}
			else{
				continue;
			}
		}
	}

	void f(T &temp, std::set<T>& set){
		std::fstream io;
		io.open(filename);
		int nth = std::lower_bound(index, index+num, temp) - index;
		nth = std::max(nth-1,0);
		for(int j = nth; j < num; j++){
			block body;
			io.seekg(j * unit);
			io.read(reinterpret_cast<char*>(&body), unit);
			if(std::string(temp.main_key) <std::string(body.begin)){
				return;
			}
			if(std::string(temp.main_key) <=std::string(body.end)){
				long fin = std::lower_bound(body.books, body.books + body.len, temp) - body.books;
				for(int i = fin; i < body.len; i++){
					if(std::string (body.books[i].main_key) == std::string (temp.main_key)){
						set.insert(body.books[i]);
					}
					else break;
				}
				continue;
			}
			else{
				continue;
			}
		}
	}
};