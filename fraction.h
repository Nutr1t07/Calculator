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
	Wint n = 1;		//分子。
	Wint d = 1;		//分母。

	Fraction() = default;
	Fraction(string str){
		if(str == ""){
			return;
		}
		size_t found;
		found = str.find('.');
		if(found != string::npos){
			d = Wint("1" + string(str.size() - 2, '0'));
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
	}

	Fraction& reduce(){					//约分。
		if(d == 1)
			return *this;
		Wint n_copy = n, d_copy = d;
		if(n.isNegative + d.isNegative != 1)
			n.isNegative = d.isNegative = 0;
		n_copy.isNegative = d_copy.isNegative = 0;
		Wint cd = gcd(n_copy, d_copy);
		n /= cd;
		d /= cd;
		return *this;
	}
};

void rtcd(Fraction &f1, Fraction &f2){		//通分。
	Wint a = lcm(f1.d, f2.d);
	Wint f1_a = a / f1.d;
	f1.d = a;
	f1.n *= f1_a;
	Wint f2_a = a / f2.d;
	f2.d = a;
	f2.n *= f2_a;
}

Fraction operator+(const Fraction f1, const Fraction f2){
	Fraction left = f1;
	Fraction right = f2;
	rtcd(left, right);
	Fraction result;
	result.d = left.d;
	result.n = left.n + right.n;
	result.reduce();
	return result;
}

Fraction operator-(const Fraction f1, const Fraction f2){
	Fraction left = f1;
	Fraction right = f2;
	rtcd(right, left);
	Fraction result;
	result.d = right.d;
	result.n = left.n - right.n;
	result.reduce();
	return result;
}

Fraction operator*(const Fraction f1, const Fraction f2){
	Fraction result;
	result.d = f1.d * f2.d;
	result.n = f1.n * f2.n;
	result.reduce();
	return result;
}

Fraction operator/(const Fraction f1, const Fraction f2){
	Fraction result;
	result.d = f1.d * f2.n;
	result.n = f1.n * f2.d;
	result.reduce();
	return result;
}

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
