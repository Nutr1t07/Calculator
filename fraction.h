#include <iostream>
#ifndef FRACTION_H
#define FRACTION_H

struct Fraction{
	int n;		//分子。
	int d;		//分母。

	Fraction& reduce(){
		int cd = gcd(n, d);
		n /= cd;
		d /= cd;
		return *this;
	}

	int gcd(int m, int n){
		if(n == 0)
			return m;
		return gcd(n, m%n);
	}

	int lcm(int m, int n){
		return m * n / gcd(m, n);
	}

	void rtcd(Fraction &f1, Fraction &f2){
		int a = lcm(f1.d, f2.d);
		int f1_a = a / f1.d;
		f1.d = a;
		f1.n *= f1_a;
		int f2_a = a / f2.d;
		f2.d = a;
		f2.n *= f2_a;
	}

	Fraction &operator+(Fraction &f1, Fraction &f2){
		rtcd(f1, f2);
		Fraction result;
		result.d = f1.d;
		result.n = f1.n + f2.n;
		result.reduce();
		return result;
	}
};

#endif