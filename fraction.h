#ifndef FRACTION_H

#define FRACTION_H
#include <string>
#include "apa.h"

class Fraction;
void rtcd(Fraction &f1, Fraction &f2);

Fraction operator+(const Fraction f1, const Fraction f2);
Fraction operator-(const Fraction f1, const Fraction f2);
Fraction operator*(const Fraction f1, const Fraction f2);
Fraction operator/(const Fraction f1, const Fraction f2);
Fraction operator%(const Fraction f1, const Fraction f2);

class Fraction{
public:
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
			d = Wint("1" + string(str.size() - found - 1, '0'));
			str.erase(found, 1);
			n = Wint(str);
			reduce();
			return;
		}
		found = str.find('|');
		if(found != string::npos){
			n = str.substr(0, found + 1);
			d = str.substr(found + 1);
			return;
		}
		n = Wint(str);
	}
	Fraction(int i){
		n = i;
	}

	operator string() const {
		if(d == 1)
			return n;
		else{
			//判断分数是否可以转为小数。
			// Fraction f(n / d);
			// if(f.n == n && f.d == d)
			//  	return n / d;
			// else
				return static_cast<string>(n) + "|" + static_cast<string>(d);
		}
	}

	Fraction& reduce(){					//约分。
		if(d == 1)
			return *this;
		else if(d == 0){
			this->d = 0;
			this->n = 0;
			return *this;
		}
		Wint cd;						//分子分母的公因数。
		if(n.isNeg + d.isNeg != 1){		//若分子分母同号，将分数改为正数。
			n.isNeg = d.isNeg = 0;
			cd = gcd(n, d);
		}
		else{							//取两数公因数，需要将两数改为非负数。
			Wint n_copy(n), d_copy(d);	
			n_copy.isNeg = d_copy.isNeg = 0;
			cd = gcd(n_copy, d_copy);
		}
		if(cd == 1)
			return *this;
		n /= cd;
		d /= cd;
		return *this;
	}

	Fraction& operator+=(Fraction right){
		rtcd(*this, right);
		this->n += right.n;
		return this->reduce();
	}

	Fraction& operator-=(Fraction right){
		rtcd(*this, right);
		this->n -= right.n;
		return this->reduce();
	}

	Fraction& operator*=(Fraction right){
		this->n *= right.n;
		this->d *= right.d;
		return this->reduce();
	}

	Fraction& operator/=(Fraction right){
		this->n *= right.d;
		this->d *= right.n;
		return this->reduce();
	}

	Fraction& operator%=(Fraction right){
		if(this->d == right.d && this->d == 1)
			this->n -= (this->n / right.n) * right.n;
		return this->reduce();
	}
};

void rtcd(Fraction &f1, Fraction &f2){		//通分。
	if(f1.d == f2.d)
		return;
	Wint cm = lcm(f1.d, f2.d);	//取最小公倍数。
	f1.n *= cm / f1.d;	
	f2.n *= cm / f2.d;	
	f1.d = f2.d = cm;
}

Fraction operator+(const Fraction f1, const Fraction f2){
	Fraction result(f1);
	return result += f2;
}

Fraction operator-(const Fraction f1, const Fraction f2){
	Fraction result(f1);
	return result -= f2;
}

Fraction operator*(const Fraction f1, const Fraction f2){
	Fraction result(f1);
	return result *= f2;
}

Fraction operator/(const Fraction f1, const Fraction f2){
	Fraction result(f1);
	return result /= f2;
}

Fraction operator%(const Fraction f1, const Fraction f2){
	Fraction result(f1);
	return result %= f2;
}

bool operator<(Fraction f1, Fraction f2){
	rtcd(f1, f2);
	return f1.n < f2.n;
}

bool operator>(Fraction f1, Fraction f2){
	return f2 < f1;
}

bool operator<=(Fraction f1, Fraction f2){
	return !(f1 > f2);
}

bool operator>=(Fraction f1, Fraction f2){
	return !(f1 < f2);
}

#endif
