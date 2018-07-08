#ifndef FRACTION_H
#define FRACTION_H
#include <iostream>
#include <string>
#include <cmath>

using namespace std;

struct Fraction{
	long long n = 1;		//分子。
	long long d = 1;		//分母。
	Fraction() = default;
	Fraction(string str){
		size_t found;
		found = str.find('.');
		if(found != string::npos){
			d = pow(10, str.size() - found - 1);
			str.erase(found, 1);
			n = stoi(str);
			reduce();
			return;
		}
		found = str.find('|');
		if(found != string::npos){
			n = stoi(str);
			cout << "alive" << endl;
			d = stoi(str.substr(found + 1));
			cout << "die" << endl;
			return;
		}
		n = stoi(str);
	}

	Fraction& reduce(){			//约分。
		long long cd = gcd(n, d);
		n /= cd;
		d /= cd;
		return *this;
	}

	long long gcd(long long m, long long n){		//最大公因数。
		if(n == 0)
			return m;
		return gcd(n, m%n);
	}

	long long lcm(long long m, long long n){		//最小公倍数。
		return m * n / gcd(m, n);
	}

	void rtcd(Fraction &f1, Fraction &f2){		//通分。
		long long a = lcm(f1.d, f2.d);
		long long f1_a = a / f1.d;
		f1.d = a;
		f1.n *= f1_a;
		long long f2_a = a / f2.d;
		f2.d = a;
		f2.n *= f2_a;
	}

	ostream &operator<<(ostream &out);

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
		Fraction fract;
		fract.n = f1.d;
		fract.d = f1.n;
		
		Fraction result;
		result.d = fract.d * this->d;
		result.n = fract.n * this->n;
		result.reduce();
		return result;
	}

	Fraction operator-(const Fraction &f1){
		Fraction fract = f1;
		rtcd(fract, *this);
		
		Fraction result;
		result.d = fract.d;
		result.n = fract.n - this->n;
		result.reduce();
		return result;
	}
};

ostream &operator<<(ostream &out, Fraction &fract1){
	if(fract1.d == 1)
		out << to_string(fract1.n);
	else{
		Fraction f(to_string((double)(fract1.n) / (double)(fract1.d)));
		if(f.n == fract1.n && f.d == fract1.d)
			out << to_string((double)(fract1.n) / (double)(fract1.d));
		else
			out << to_string(fract1.n) << "|" << to_string(fract1.d);
	}
	return out;
}

string to_string(Fraction f){
	if(f.d == 1)
		return to_string(f.n);
	else{
		// if(f.n == f.n && f.d == f.d)
		// 	return to_string((double)(f.n) / (double)(f.d))
		// else
			return to_string(f.n) + "|" + to_string(f.d);
	}

}

#endif