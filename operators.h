#ifndef OPERATORS_H

#define OPERATORS_H
#include <map>
#include <functional>
#include "fraction.h"

Fraction add(Fraction left, Fraction right){
	return left + right;
}

Fraction sub(Fraction left, Fraction right){
	return left - right;
}

Fraction multi(Fraction left, Fraction right){
	return left * right;
}

Fraction devide(Fraction left, Fraction right){
	return left / right;
}

map<string, function<Fraction(Fraction, Fraction)>> opers2{
	{"+", add},
	{"-", sub},
	{"*", multi},
	{"/", devide}
};

#endif