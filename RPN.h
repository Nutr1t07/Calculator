#ifndef RPN_H

#define RPN_H

#include <stack>
#include <string>
#include <unordered_map>
#include "fraction.h"
#include "operators.h"

int comp(char &ch){
	switch(ch){
		case '+':
		case '-':
			return 0;
		case '*':
		case '/':
			return 1;
	}
	return -1;
}

inline bool isNum(const char &ch){
	return isdigit(ch) || ch == '.' || ch == '|';
}

//将带符号的数字转成运算的形式，如: -1转为(0-1), --1转为(0-(0-1))。
//错了。(下星期回来再修。)
string repair(string str){
	size_t found = str.find_last_of("+-");
	while(found != string::npos){
		if((found == 0) || (str[found - 1] > 39 && str[found - 1] < 48 && str[found - 1] != ')') ){	//若该运算符前有运算符，则该运算符是符号。
			str.insert(found, "(0");		//在符号前插入(0。
			found += 2;						//调整found位置。
			size_t num_end = found + 1;		//右运算对象的结束位置。

			//-(0-1)的情况下，右运算对象是(0-1)。所以不仅要判断数字，还要判断括号。
			for(; num_end != str.size() && (isNum(str[num_end]) || str[num_end] == '('); ++num_end);{
				if(num_end != str.size() && str[num_end] == '(')
					for(; num_end != str.size() && str[num_end] != ')'; ++num_end);
			}
			//循环结束后，num_end应该在最后的位置。
			str.insert(num_end, ")");		
		}
		found = str.find_last_of("+-", found - 1);
	}
	cout << "str: " << str << endl;
	return str;
}

stack<string> transform(const string str){
	stack<char> opers;
	stack<string> nums;
	for(auto iter = str.cbegin(); iter != str.cend(); ++iter){
		char ch = *iter;
		if(isdigit(ch) || ch == '.' || ch == '|'){	//若遇到操作数：
			string num;
			bool flag = 0;
			while(iter != str.cend() && (isdigit(ch) || ch == '.' || ch == '|')){
				flag = 1;
				num += ch;
				ch = *++iter;
			}
			if(flag)
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

		//若遇到二元运算符：
		if(opers2.find(string(1, ch)) != opers2.end()){
			if(opers.empty() || opers.top() == '('){	//若运算符栈为空或栈顶为"(":
				opers.push(ch);
				continue;
			}
			else{
				if(comp(ch) > comp(opers.top())){	//若当前运算符比运算符栈栈顶优先级高:
					opers.push(ch);
					continue;
				}
				else{
					//若非如此，将栈顶运算符弹出。
					while(!opers.empty() && comp(ch) <= comp(opers.top())){
						nums.push(string(1, opers.top()));
						opers.pop();
						if(opers.empty() || opers.top() == '(' || comp(ch) > comp(opers.top())){
							opers.push(ch);
							break;
						}
					}
				}
			}
		}


		string oper;
		bool flag = 0;
		while(iter != str.cend() && !isdigit(ch) && ch != '.' && ch != '|' && ch == '!'){
			oper += ch;
			ch = *++iter;
			flag = 1;
		}
		if(flag) 
			--iter;
		if(opers1.find(oper) != opers1.end()){
			Fraction ans(opers1.at(oper)(nums.top()));
			nums.pop();
			nums.push(ans);
			continue;
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
			Fraction ans = opers2.at(reverse_s1.top())(left, right);
			sum.push(ans);
		}
		reverse_s1.pop();
	}
	return sum.top();
}

#endif