#ifndef APA_H

#define APA_H
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Wint;
string to_string(const Wint wint);
bool operator==(const Wint &w1, const Wint &w2);
bool operator!=(const Wint &w1, const Wint &w2);
bool operator>(const Wint &w1, const Wint &w2);
bool operator<(const Wint &w1, const Wint &w2);
bool operator>=(const Wint &w1, const Wint &w2);
bool operator<=(const Wint &w1, const Wint &w2);


Wint operator+(const Wint w1, const Wint w2);
Wint operator-(const Wint w1, const Wint w2);
Wint operator*(const Wint w1, const Wint w2);
Wint operator/(const Wint w1, const Wint w2);
Wint operator%(const Wint w1, const Wint w2);

const int SCALE = 10000;	//进制。
const int PRECISION = 4;	//精度。

struct Wint : vector<int>
{
	bool isNegative = 0;

	Wint() = default;
	Wint(string str){
		size_t sign = str.find_first_of("+-");
		if(sign != string::npos){
			if(str[sign] == '-')
				isNegative = 1;
			str.erase(sign, 1);
		}
		auto len = str.size();
		while(len / PRECISION){
			len -= PRECISION;
			push_back(stoi(str.substr(len, PRECISION)));
		}
		if(len)
			push_back(stoi(str.substr(0, len)));
	}
	Wint(int num){
		if(num < 0){
			isNegative = 1;
			num = 0 - num;
		}
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

	Wint& operator+=(const Wint &w1){
		resize(max(size(), w1.size()));
		if(this->isNegative + w1.isNegative != 1){	//若同号：
			for(int i = 0; i < size(); ++i){
				(*this)[i] = (*this)[i] + w1[i];
			}
		}
		else{
			Wint left = *this, right = w1;
			if(left.isNegative){
				left.isNegative = 0;
				*this = right - left;
			}
			else{
				right.isNegative = 0;
				*this = left - right;
			}
		}
		return carry();
	}

	Wint& operator-=(const Wint &w1){
		int max_size = max(size(), w1.size());
		Wint left(max_size, 0), right(max_size, 0);
		left = *this;
		right = w1;
		if(left.isNegative + right.isNegative == 1){	//若两数异号：
			if(left.isNegative){
				left.isNegative = 0;
				return *this = left + right;
			}
			else{
				right.isNegative = 0;
				return *this = left + right;
			}
		}
		if(left.isNegative){		//若同为负：
			right.isNegative = 0;
			return *this = left + right;
		}

		bool reverse = 0;
		if(w1 > *this){
			reverse = 1;
			left = w1;
			right = *this;
		}

		resize(max_size);
		for(int i = 0; i < max_size; ++i){
			(*this)[i] = left[i] - right[i];
		}

		carry();
		if(reverse){
			if((0 - back()) < 0){
				isNegative = 1;
			}
			else
				back() = 0 - back();
		}
		return *this;
	}

	Wint& operator*=(const Wint &w1){
		return *this = *this * w1;
	}

	Wint& operator/=(const Wint &w1){
		return *this = *this / w1;
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
	for(auto n : wint){
		string ns(to_string(n));
		result = string(PRECISION - ns.size(), '0') + ns + result;
	}
	result.erase(result.begin(), find_if(result.begin(), result.end(),[](char ch){ return ch != '0'; }));
	if(result == "")
		return "0";
	if(wint.isNegative)
		result = '-' + result;
	return result;
}

bool operator==(const Wint &w1, const Wint &w2){
	if(w1.size() != w2.size())
		return 0;
	for(int i = 0; i < w1.size(); ++i){
		if(w1[i] != w2[i])
			return 0;
	}
	return 1;
}

bool operator!=(const Wint &w1, const Wint &w2){
	return !(w1 == w2);
}

bool operator>(const Wint &w1, const Wint &w2){
	if((w1.isNegative + w2.isNegative) == 1){	//若两数异号：
		return w1.isNegative;
	}
	Wint left = w1, right = w2;
	if(w1.isNegative == 1){	//若两数同为负：
		left = w2;
		right = w1;
	}
	if(left.size() != right.size())
		return left.size() > right.size();
	int max_size(max(left.size(), right.size()));
	for(int i = left.size() - 1; i >= 0; --i)
		if(left[i] != right[i])
			return left[i] > right[i];
	return 0;
}

bool operator<(const Wint &w1, const Wint &w2){
	return (w2 > w1);
}

bool operator>=(const Wint &w1, const Wint &w2){
	return !(w1 < w2);
}

bool operator<=(const Wint &w1, const Wint &w2){
	return !(w1 > w2);
}


Wint operator+(const Wint w1, const Wint w2){
	Wint left = w1;
	return left += w2;
}

Wint operator-(const Wint w1, const Wint w2){
	Wint left = w1;
	return left -= w2;
}

Wint operator*(const Wint w1, const Wint w2){
	Wint ans(w1.size() + w2.size(), 0);
	for(int i = 0; i < w1.size(); ++i)
		for(int j = 0; j < w2.size(); ++j)
			ans[i+j] = w1[i] * w2[j];
	ans.carry();
	if(w1.isNegative + w2.isNegative == 1)
		ans.isNegative = 1;
	return ans;
}

Wint operator/(const Wint w1, const Wint w2){
	Wint left = w1, right = w2;
	left.isNegative = 0;
	right.isNegative = 0;
	string s1(to_string(left));
	string s2(to_string(right));

	int s2_length = s2.size();
	int PRECISION = 0;
	while(s2.size() < s1.size()){
		s2 += '0';
		++PRECISION;
	}

	string result;
	while(PRECISION >= 0){
		right = Wint(s2.substr(0, s2_length + PRECISION));
		int c = 0;

		while(left >= right){
			left -= right;
			++c;
		}
		result += to_string(c);
		--PRECISION;
	}
	Wint ans(result);
	if(w1.isNegative + w2.isNegative == 1)
		ans.isNegative = 1;
	return ans;
}

Wint operator%(const Wint w1, const Wint w2){
	Wint left = w1, right = w2;
	left.isNegative = 0;
	right.isNegative = 0;
	if(left < right){
		left = w1;
		right = w2;
	}

	Wint wint = left / right;
	wint *= w2;
	return left - wint;
}

Wint gcd(const Wint m, const Wint n){		//最大公因数。
	if(n.size() == 0){
		return m;
	}
	return gcd(n, m%n);
}

Wint lcm(const Wint m, const Wint n){		//最小公倍数。
	return m * n / gcd(m, n);
}

#endif
