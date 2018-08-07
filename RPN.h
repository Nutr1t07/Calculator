#ifndef RPN_H

#define RPN_H

#include <stack>
#include <string>
#include <functional>
#include <unordered_map>
#include "fraction.h"

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

Fraction pow(const Fraction &f1, const Fraction &f2){
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

int comp(char &ch){
	switch(ch){
		case '+':
		case '-':
			return 0;
		case '*':
		case '/':
			return 1;
	}
}

unordered_map<string, function<Fraction(Fraction, Fraction)>> operators{
	{"+", add},
	{"-", sub},
	{"*", mult},
	{"/", devide},

};

string repair(string str){
	size_t found = str.find_first_of("+-");
	while(found != string::npos){
		if(!isdigit(str[found - 1])){
			str.insert(found, "(0");
			found += 2;
			size_t num_end = found + 1;
			for(; num_end != str.size() && (isdigit(str[num_end]) || str[num_end] == '.'); ++num_end);
			str.insert(num_end, ")");

		}
		found = str.find_first_of("+-", found + 1);
	}
	return str;
}

stack<string> transform(const string str){
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

		if((ch > 64 && ch < 90) || ch == '!'){			//若遇到函数(字母)：
			if(ch == '!'){

			}
		}

		//若遇到运算符：
		if(opers.empty() || opers.top() == '('){
			opers.push(ch);
			continue;
		}
		if(comp(ch) >= comp(opers.top())){
			opers.push(ch);
			continue;
		}
		else{
			while(!opers.empty() && comp(ch) < comp(opers.top())){
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
	stack<string> s1 = transform(repair(str));
	stack<string> reverse_s1;
	stack<string> sum;
	while(!s1.empty()){
		reverse_s1.push(s1.top());
		s1.pop();
	}
	// while(!reverse_s1.empty()){
	// 	cout << reverse_s1.top() << " ";
	// 	reverse_s1.pop();
	// }
	while(!reverse_s1.empty()){
		if(isdigit(reverse_s1.top()[0]))
			sum.push(reverse_s1.top());
		else{
			Fraction right(sum.top());
			sum.pop();
			Fraction left(sum.top());
			sum.pop();
			Fraction ans = operators[reverse_s1.top()](left, right);
			sum.push(ans);
		}
		reverse_s1.pop();
	}
	return sum.top();
}

#endif