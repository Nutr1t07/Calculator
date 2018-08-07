#ifndef APA_H

#define APA_H
#include <vector>
#include <algorithm>

using namespace std;

class Wint;

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

class Wint : public vector<int>
{
public:
	bool isNeg = 0;	//是否为负数。

	Wint() = default;
	Wint(string str){
		size_t sign = str.find_first_of("+-");
		if(sign != string::npos){
			if(str[sign] == '-')
				isNeg = 1;
			str.erase(sign, 1);
		}

		auto len = str.size();
		//压位。
		while(len / PRECISION){
			len -= PRECISION;
			push_back(stoi(str.substr(len, PRECISION)));
		}
		if(len)
			push_back(stoi(str.substr(0, len)));
	}
	Wint(int num){
		if(num < 0){
			isNeg = 1;
			num = 0 - num;
		}
		push_back(num);
		carry();
	}
	Wint(size_t size, int n){
		resize(size, n);
	}

	operator string() const{
		string result;
		for(auto n : *this){
			string ns(to_string(n));
			//用0填充空白。
			result = string(PRECISION - ns.size(), '0') + ns + result;
		}
		//去掉最高位上的0。
		result.erase(result.begin(), find_if(result.begin(), result.end(),
			[](char ch){ return ch != '0'; }));
		if(result == "")
			return "0";
		if(this->isNeg)
			result = "-" + result;
		return result;
	}

	Wint &carry(){		//进位。
		if(empty())
			return *this;
		for(unsigned int i = 1; i < size(); ++i){
			(*this)[i] += (*this)[i-1] / SCALE;
			(*this)[i-1] %= SCALE;
		}
		while(back() / SCALE){			//若最高位没有进位：
			push_back(back() / SCALE);
			(*this)[size()-2] %= SCALE;
		}
		while(back() == 0 && size() != 1){				//去除为零的最高位。
			pop_back();
		}
		return *this;
	}

	Wint& operator+=(const Wint &w1){
		resize(max(size(), w1.size()));		//将储存结果的容器的长度设为最大值。
		if(this->isNeg + w1.isNeg != 1){	//若同号：
			for(unsigned int i = 0; i < size(); ++i){
				(*this)[i] = (*this)[i] + w1[i];		//位权相同的两元素相加。
			}
		}
		else{										//若异号：(两数相减)
			Wint left = *this, right = w1;
			if(left.isNeg){
				left.isNeg = 0;
				*this = right - left;
			}
			else{
				right.isNeg = 0;
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
		if(left.isNeg + right.isNeg == 1){	//若两数异号：
			//将两数的符号转为正，再将两数相加。
			if(left.isNeg){
				left.isNeg = 0;
				*this = left + right;
				this->isNeg = 1;		//将两数相加的结果转为负数。
			}
			else{
				right.isNeg = 0;
				*this = left + right;
			}
			return *this;
		}
		if(left.isNeg){					//若两数同为负数：
			left.isNeg = right.isNeg = 0;
			return *this = left + right;
		}

		//若左数比右数小，则将左右顺序颠倒。
		bool reverse = 0;
		if(*this < w1){
			reverse = 1;
			left = w1;
			right = *this;
		}

		//将位权相等的数相减。
		resize(max_size);
		for(int i = 0; i < max_size; ++i){
			(*this)[i] = left[i] - right[i];
		}

		carry();
		if(reverse)
			isNeg = 1;
		return carry();
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

	//前置递增运算符。
	Wint& operator++(){
		*this += 1;
		return *this;
	}
	//后置递增运算符。
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

bool operator==(const Wint &w1, const Wint &w2){
	if(w1.size() != w2.size())
		return 0;
	for(unsigned int i = 0; i < w1.size(); ++i){
		if(w1[i] != w2[i])
			return 0;
	}
	return 1;
}

bool operator!=(const Wint &w1, const Wint &w2){
	return !(w1 == w2);
}

bool operator<(const Wint &w1, const Wint &w2){
	if((w1.isNeg + w2.isNeg) == 1){	//若两数异号：
		return w1.isNeg;
	}
	Wint left = w1, right = w2;
	if(w1.isNeg == 1){	//若两数同为负：
		left = w2;
		right = w1;
	}
	if(left.size() != right.size())
		return left.size() < right.size();
	for(int i = left.size() - 1; i >= 0; --i)
		if(left[i] != right[i])
			return left[i] < right[i];
	return 0;
}

bool operator>(const Wint &w1, const Wint &w2){
	return w2 < w1;
}

bool operator>=(const Wint &w1, const Wint &w2){
	return !(w1 < w2);
}

bool operator<=(const Wint &w1, const Wint &w2){
	return !(w1 > w2);
}


Wint operator+(const Wint w1, const Wint w2){
	Wint ans = w1;
	return ans += w2;
}

Wint operator-(const Wint w1, const Wint w2){
	Wint ans = w1;
	return ans -= w2;
}

Wint operator*(const Wint w1, const Wint w2){
	Wint ans(w1.size() + w2.size(), 0);
	for(unsigned int i = 0; i < w1.size(); ++i)
		for(unsigned int j = 0; j < w2.size(); ++j)
			ans[i+j] = w1[i] * w2[j];
	ans.carry();
	if(w1.isNeg + w2.isNeg == 1)
		ans.isNeg = 1;
	return ans.carry();
}

Wint operator/(const Wint w1, const Wint w2){
	Wint left = w1, right = w2;
	left.isNeg = 0;
	right.isNeg = 0;
	string s1(left);
	string s2(right);

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
	if(w1.isNeg + w2.isNeg == 1)
		ans.isNeg = 1;
	return ans;
}

Wint operator%(Wint w1, Wint w2){
	w1.isNeg = w2.isNeg = 0;
	return w1 - (w1 / w2) * w2;
}

Wint gcd(const Wint &m, const Wint &n){		//最大公因数。
	if(n == 0)
		return m;
	return gcd(n, m%n);
}

Wint lcm(const Wint &m, const Wint &n){		//最小公倍数。
	return m * n / gcd(m, n);
}

#endif
