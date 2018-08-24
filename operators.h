#ifndef OPERATORS_H

#define OPERATORS_H

#include <unordered_map>
#include <functional>
#include <string>
#include "fraction.h"
#include "apa.h"

Fraction add(const Fraction&, const Fraction&);
Fraction sub(const Fraction&, const Fraction&);
Fraction mult(const Fraction&, const Fraction&);
Fraction devide(const Fraction&, const Fraction&);
Fraction mod(const Fraction&, const Fraction&);
Fraction power(const Fraction&, const Fraction&);

Fraction factorial(const Fraction&);
Fraction dfactorial(const Fraction&);

// Fraction sqrt1(const Fraction&);

int comp(const char &ch){
	switch(ch){
		case '+':
		case '-':
			return 0;
		case '*':
		case '/':
		case '%':
			return 1;
		case '^':
			return 2;
	}
	return -1;
}

// const unordered_map<string, function<Fraction(Fraction)>> funcs{
// 	{"sqrt", sqrt1}
// };

const unordered_map<string, function<Fraction(Fraction, Fraction)>> opers2{
	{"+", add},
	{"-", sub},
	{"*", mult},
	{"/", devide},
	{"%", mod},
	{"^", power},
};

const unordered_map<string, function<Fraction(Fraction)>> opers1{
	{"!", factorial},
	{"!!", dfactorial}
};



Fraction add(const Fraction &f1, const Fraction &f2){
	return f1 + f2;
}

Fraction sub(const Fraction &f1, const Fraction &f2){
	return f1 - f2;
}

Fraction mult(const Fraction &f1, const Fraction &f2){
	return f1 * f2;
}

Fraction devide(const Fraction &f1, const Fraction &f2){
	return f1 / f2;
}

Fraction mod(const Fraction &f1, const Fraction &f2){
	return f1 % f2;
}

Fraction power(const Fraction &f1, const Fraction &f2){
	Fraction result("1");
	Fraction base(f1);
	Wint expo(f2.n);
	expo.isNeg = 0;
	while(expo != 0){
		if(expo[expo.size() - 1] % 2 == 1) 
			result = result * base;
		base = base * base;
		expo = expo / 2;
	}
	if(f2.n.isNeg)
		result = Fraction(1) / result;
	return result;
}




Fraction factorial(const Fraction &f1){
	Fraction ans(1);
	for(Fraction i = ans; i <= f1; i += 1)
		ans *= i;
	return ans;
}

Fraction dfactorial(const Fraction &f1){
	Fraction ans(2 - (f1 % 2));
	for(Fraction i = ans; i <= f1; i += 2)
		ans *= i;
	return ans;
}



// Fraction sqrt1(const Fraction &f1){

// }

#endif