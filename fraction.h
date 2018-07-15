#ifndef FRACTION_H

#define FRACTION_H
#include <iostream>
#include <string>
#include <cmath>
#include "apa.h"

using namespace std;

struct Fraction;
string to_string(Fraction &fract);

struct Fraction{
	Wint n;		//分子。
	Wint d = 1;		//分母。
	Fraction() = default;

	Fraction(string str){
		size_t found;
		found = str.find('.');
		if(found != string::npos){
			d = Wint("1" + string(str.size() - 1, '0'));
			str.erase(found, 1);
			n = str;
			reduce();
			return;
		}
		found = str.find('|');
		if(found != string::npos){
			n = str.substr(0, found + 1);
			d = str.substr(found + 1);
			return;
		}
		n = str;
		d = Wint("1");
	}

	Fraction& reduce(){					//约分。
		Wint cd = gcd(n, d);
		n /= cd;
		d /= cd;
		return *this;
	}

	Wint gcd(Wint m, Wint n){		//最大公因数。
		if(n.size() == 0){
			return m;
		}

		return gcd(n, m%n);
	}

	Wint lcm(Wint m, Wint n){		//最小公倍数。
		return m * n / gcd(m, n);
	}

	void rtcd(Fraction &f1, Fraction &f2){		//通分。
		Wint a = lcm(f1.d, f2.d);
		Wint f1_a = a / f1.d;
		f1.d = a;
		f1.n *= f1_a;
		Wint f2_a = a / f2.d;
		f2.d = a;
		f2.n *= f2_a;
	}

	Fraction operator+(const Fraction &f1){
		Fraction fract = f1;
		rtcd(fract, *this);
		
		Fraction result;
		result.d = fract.d;
		result.n = fract.n + this->n;
		result.reduce();
		return result;
	}

	Fraction operator*(const Fraction &f1){
		Fraction fract = f1;
		Fraction result;

		result.d = fract.d * this->d;
		result.n = fract.n * this->n;
		result.reduce();
		return result;
	}

	Fraction operator/(const Fraction &f1){
		Fraction result;
		result.d = this->d * f1.n;
		result.n = this->n * f1.d;
		result.reduce();
		return result;
	}

	Fraction operator-(const Fraction &f1){
		Fraction fract = f1;
		rtcd(fract, *this);
		
		Fraction result;
		result.d = fract.d;
		result.n = this->n - fract.n;
		result.reduce();
		return result;
	}
};

string to_string(Fraction& fract){
	if(fract.d == 1)
		return to_string(fract.n);
	else{
		//判断分数是否可以转为小数。
		Fraction f(to_string(fract.n / fract.d));

		if(f.n == fract.n && f.d == fract.d)
		 	return to_string(fract.n / fract.d);
		else{
			return to_string(fract.n) + "|" + to_string(fract.d);
		}
	}
}

#endif