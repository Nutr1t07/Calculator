#ifndef RPN_H

#define RPN_H

#include <stack>
#include <string>
#include <functional>
#include <unordered_map>
#include "fraction.h"
#include <iostream>

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

unordered_map<string, function<Fraction(Fraction, Fraction)>> operators{
	{"+", add},
	{"-", sub},
	{"*", mult},
	{"/", devide},

};

stack<string> transform(const string &str){
	stack<char> opers;
	stack<string> nums;
	for(auto iter = str.cbegin(); iter != str.cend(); ++iter){
		char ch = *iter;
		if(isdigit(ch) || ch == '.'){	//若遇到操作数：
			string num;
			while(isdigit(ch) || ch == '.'){
				num += ch;
				ch = *++iter;
			}
			--iter;
			nums.push(num);
			continue;
		}

		if(ch == '('){
			opers.push(ch);
			continue;
		}
		if(ch == ')'){
			while(opers.top() != '('){
				nums.push(string(1, opers.top()));
				opers.pop();
			}
			if(opers.top() == '(')
				opers.pop();
			continue;
		}

		if(ch > 64 && ch < 90){			//若遇到函数：
			//还不知道怎么搞..
		}

		//若遇到运算符：
		if(opers.empty() || opers.top() == '('){
			opers.push(ch);
			continue;
		}
		if(ch >= opers.top()){
			opers.push(ch);
			continue;
		}
		else{
			while(!opers.empty() && ch < opers.top()){
				nums.push(string(1, opers.top()));
				opers.pop();
				if(opers.top() == '(' || opers.empty()){
					opers.push(ch);
					break;
				}
				if(ch > opers.top()){
					opers.push(ch);
					break;
				}
			}
		}
	}
	while(!opers.empty()){
		nums.push(string(1, opers.top()));
		opers.pop();
	}
	return nums;
}

string calculate(string str){

	stack<string> s1 = transform(str);
	stack<string> reverse_s1;
	stack<string> sum;
	while(!s1.empty()){
		reverse_s1.push(s1.top());
		s1.pop();
	}
	while(!reverse_s1.empty()){
		if(isdigit(reverse_s1.top()[0]))
			sum.push(reverse_s1.top());
		else{
			Fraction right(sum.top());
			sum.pop();
			Fraction left(sum.top());
			Fraction ans = operators[reverse_s1.top()](left, right);
			sum.push(to_string(ans));
		}
		reverse_s1.pop();
	}
	return sum.top();
}
#endif