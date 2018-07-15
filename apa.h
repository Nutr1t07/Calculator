#ifndef APA_H

#define APA_H
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Wint;
string to_string(const Wint wint);

const int SCALE = 10000;	//进制
const int COUNT = 4;		//几个0

struct Wint : vector<int>
{
	Wint() = default;

	Wint(string str){
		auto len = str.size();
		while(len / COUNT){
			len -= COUNT;
			push_back(stoi(str.substr(len, COUNT)));
		}
		if(len)
			push_back(stoi(str.substr(0, len)));
	}

	Wint(int num){
		push_back(num);
		carry();
	}

	Wint(size_t size, int n){
		resize(size, n);
	}

	Wint &carry(){
		if(empty())
			return *this;
		for(int i = 1; i < size(); ++i){
			if((*this)[i-1] < 0){
				((*this)[i-1] = SCALE + (*this)[i-1]);
				if((*this)[i] < 0)
					(*this)[i] += 1;
				else
					(*this)[i] -= 1;
			}
			(*this)[i] += (*this)[i-1] / SCALE;
			(*this)[i-1] %= SCALE;
		}

		while(back() / SCALE){
			push_back(back() / SCALE);
			(*this)[size()-2] %= SCALE;
		}
		while(back() == 0){

			pop_back();
		}
		return *this;
	}

	bool operator==(const Wint &w1) const{
		if(w1.size() != this->size())
			return false;
		for(int i = 0; i < this->size(); ++i){
			if((*this)[i] != w1[i])
				return false;
		}
		return true;
	}

	bool operator!=(const Wint &w1) const{
		return !(*this == w1);
	}

	bool operator>(const Wint &w1) const{
		if(w1.size() != this->size())
			return this->size() > w1.size();
		int max_size(max(w1.size(), this->size()));
		for(int i = size() - 1; i >= 0; --i)
			if((*this)[i] != w1[i])
				return (*this)[i] > w1[i];
		return false;
	}

	bool operator<(const Wint &w1) const{
		return (w1 > *this);
	}

	bool operator>=(const Wint &w1) const{
		return !(*this < w1);
	}

	bool operator<=(const Wint &w1) const{
		return !(*this > w1);
	}



	Wint& operator+=(const Wint &w1){
		resize(size() + w1.size());
		for(int i = 0; i < size(); ++i){
			(*this)[i] = (*this)[i] + w1[i];
		}
		return carry();
	}

	Wint operator+(const Wint &w1){
		return *this += w1;
	}

	Wint& operator-=(const Wint &w1){
		int max_size = max(size(), w1.size());
		Wint left(max_size, 0), right(max_size, 0);
		bool negative = false;
		if(w1 > *this){
			negative = true;
			left = w1;
			right = *this;
		}
		else{
			left = *this;
			right = w1;
		}

		resize(max_size);
		for(int i = 0; i < max_size; ++i){
			(*this)[i] = left[i] - right[i];
		}

		carry();
		if(negative){
			back() = 0 - back();
		}
		return *this;
	}

	Wint operator-(const Wint &w1){
		return *this -= w1;
	}

	Wint operator*(const Wint &w1){
		Wint ans(size() + w1.size(), 0);
		for(int i = 0; i < size(); ++i)
			for(int j = 0; j < w1.size(); ++j)
				ans[i+j] = (*this)[i] * w1[j];
		ans.carry();

		return ans;
	}

	Wint& operator*=(const Wint &w1){
		return *this = *this * w1;
	}

	Wint operator/(const Wint &w1){
		string s1(to_string(*this));
		string s2(to_string(w1));
		bool negative = true;
		auto t1 = remove(s1.begin(), s1.end(), '-');
		auto t2 = remove(s2.begin(), s2.end(), '-');
		if((t1 == s1.end() && t2 == s2.end()) || (t1 != s1.end() && t2 != s2.end()))
			negative = false;
		int s2_length = s2.size();
		Wint left(s1), right;
		int count = 0;
		while(s2.size() < s1.size()){
			s2 += '0';
			++count;
		}

		string result;
		while(count >= 0){
			right = Wint(s2.substr(0, s2_length + count));
			int c = 0;

			while(left >= right){
				left -= right;
				++c;
			}
			result += to_string(c);
			--count;
		}
		return Wint(result);
	}

	Wint& operator/=(const Wint &w1){
		return *this = *this / w1;
	}

	Wint operator%(const Wint &w1){
		Wint wint = *this / w1;
		wint *= w1;
		return *this - wint;
	}

	Wint& operator%=(const Wint &w1){
		return *this = *this % w1;
	}


	Wint& operator++(){
		*this += 1;
		return *this;
	}

	Wint operator++(int){
		Wint wint = *this;
		*this += 1;
		return wint; 
	}

	Wint& operator--(){
		*this -= 1;
		return *this;
	}

	Wint operator--(int){
		Wint wint = *this;
		*this -= 1;
		return wint; 
	}
};

string to_string(const Wint wint){
	Wint copy;
	copy.resize(wint.size());
	reverse_copy(wint.begin(), wint.end(), copy.begin());
	string result;
	bool negative = false;
	for(auto n : wint){
		string ns(to_string(n));
		if(ns[0] == '-'){
			result = string(COUNT + 1 - ns.size(), '0') + ns.substr(1) + result;
			negative = true;
		}
		else
			result = string(COUNT - ns.size(), '0') + ns + result;
	}
	result.erase(result.begin(), find_if(result.begin(), result.end(),[](char ch){ return ch != '0'; }));
	if(result == "")
		return "0";
	if(negative)
		result = '-' + result;
	return result;
}

#endif