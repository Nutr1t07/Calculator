#include <iostream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <unordered_set>
#include "fraction.h"
#include "operators.h"

using namespace std;

// string basic_calculate(string str);
string basc(string str);
string bracket_process(string str);
Fraction get_operands(const string str, const size_t found, Fraction &right, size_t &length, size_t &st_left);
void print_error(string error);

int main(){
	string str;
	while(cout << "Enter the expression: " && getline(cin, str)){
		str.erase(remove_if(str.begin(), str.end(), ::isspace), str.end());	//去除表达式中的空格。
		str = bracket_process(str);											//处理空格。
		string answer = basc(str);
		cout << "The answer is: " << answer << "\n" << endl;
	}
}

string basc(string str){
	bool stopFlag;
	bool has_sign = 0;
	size_t found;

	// for(auto oper = opers2.cbegin(); oper != opers2.cend(); ++oper){
	// 	found = str.find(oper->first);
	// 	if(found != string::npos)
	// 		break;
	// }
	cout << "F: " << str[found] << endl;
	while(1){
		stopFlag = 1;
		for(auto oper = opers2.cbegin(); oper != opers2.cend(); ++oper){
			if(!has_sign)
				found = str.find(oper->first);
			else
				found = str.find(oper->first, found + 1);
			has_sign = 0;
			if(found != string::npos){
				string str_oper = oper->first;
				Fraction left, right;
				int st_left = found - 1;
				int st_right = found + str_oper.size();
				for(; st_left != -1 && (isdigit(str[st_left]) || str[st_left] == '.'); --st_left);
					++st_left;
				for(; st_right != str.size() + 1 && (isdigit(str[st_right]) || str[st_right] == '.'); ++st_right);
					--st_right;
				if(st_left == found){
					has_sign = 1;
					break;
				}
				left = Fraction(str.substr(st_left, found - st_left));
				right = Fraction(str.substr(found + str_oper.size(), st_right - found + 2));

				Fraction ans = oper->second(left, right);
				str.replace(st_left, st_right - st_left + 1, to_string(ans));
				cout << "str : " << str;
				cout << " ans: " << to_string(ans) ;
				cout << " st_l: " << to_string(st_left) << " st_right: " << to_string(st_right);
				cout << " left: " << to_string(left) << " right: " << to_string(right) << endl;
 				stopFlag = 0;
			}
		}
		if(stopFlag)
			break;
	}
	return str;
}

//基本运算。
// string basic_calculate(string cal_str){
// 	size_t found;				//运算符号位置。
// 	size_t length;				//计算对象长度。
// 	size_t st_left, st_right;	//执行运算部分的起始位置。
// 	Fraction left, right, result;	//运算符左右侧数字以及计算结果。

// 	//执行四级运算(函数运算)。
// 	unordered_set<string> func{
// 		"sin",
// 		"cos",
// 		"tan",
// 		"cot",
// 		"sec",
// 		"csc"
// 	};



// 	//执行三级运算。
// 	found = cal_str.find_first_of("^!");
// 	while(found != string::npos){
// 		left = get_operands(cal_str, found, right, length, st_left);
// 		if(cal_str[found] == '^'){
// 			if(right.n == 0){
// 				result.n = 1;
// 				result.d = 1;
// 			}
// 			else{
// 				result = Fraction("1");
// 				Wint zero(0);
// 				while(right.n != zero){
// 					if(right.n[right.n.size() - 1] % 2)
// 						result = result * left;
// 					left = left * left;
// 					right.n = right.n / 2;
// 				}
// 				if(right.n < 0){
// 					result = Fraction() / result;
// 				}
// 			}

// 			cal_str.replace(st_left, length, to_string(result));
// 		}
// 		else if(cal_str[found] == '!'){
// 			if((found+1) != cal_str.size() && cal_str[found + 1] == '!'){		//判断是否为双阶乘
// 				Wint ans(1);
// 				int i;
// 				if(left.n % 2 == 1)
// 					i = 1;
// 				else
// 					i = 2;
// 				for(; left.n >= i; i += 2)
// 					ans *= i;
// 				length = found - st_left + 3;
// 				result.n = ans;
// 			}
// 			else{
// 				Wint ans(1);
// 				left.n += 1;
// 				for(int i = 1; i != left.n; ++i)
// 					ans *= i;
// 				length = found - st_left + 2;
// 				result.n = ans;
// 			}
// 		}
// 		cal_str.replace(st_left, length, to_string(result));

// 		found = cal_str.find_first_of("^!");
// 	}

// 	//执行二级运算(乘除运算)。
// 	found = cal_str.find_first_of("*/");
// 	while(found != string::npos){
// 		left = get_operands(cal_str, found, right, length, st_left);
// 		if(cal_str[found] == '*'){	//执行乘法运算。
// 			result = left * right;

// 		}
// 		else if(cal_str[found] == '/'){	//执行除法运算。
// 			result = left / right;
// 		}

// 		cal_str.replace(st_left, length, to_string(result));
// 		found = cal_str.find_first_of("*/");
// 	}

// 	//执行一级运算(加减运算)。
// 	found = cal_str.find_first_of("+-");
// 	while(found != string::npos){
// 		left = get_operands(cal_str, found, right, length, st_left);
// 		if(st_left == found){
// 			found = cal_str.find_first_of("+-", found + 1);
// 			continue;
// 		}
// 		if(cal_str[found] == '+'){
// 			result = left + right;
// 		}
// 		else if(cal_str[found] == '-'){
// 			result = left - right;
// 		}

// 		cal_str.replace(st_left, length, to_string(result));
// 		found = cal_str.find_first_of("+-");
// 	}
// 	return cal_str;
// }

//处理括号内的数据。
string bracket_process(string str){
	auto front1_brac = str.find_first_of("{[(");					//寻找第一个括号。
	if(front1_brac != string::npos){								//如果找到了括号：
		auto front2_brac = str.find_first_of("{[(", front1_brac + 1);
		if(front2_brac != string::npos){								//如果有第二个括号：
			string str2 = str.substr(front2_brac);
			size_t str2_size = str2.size();
			str2 = bracket_process(str2);									//对第二个括号后进行递归。
			str.replace(front2_brac, str2_size, str2);						//将处理后的字符串替换掉原字符串。
		}
		auto back1_bracket = str.find_first_of("}])");					//寻找后括号。

		if(back1_bracket != string::npos){
			string caled_str = basc(str.substr(front1_brac + 1, back1_bracket - front1_brac - 1));
			str.replace(front1_brac, back1_bracket - front1_brac + 1, caled_str);
		}

	}
	return str;
}


size_t get_operands(const string str, const size_t found, Fraction &left, Fraction &right, size_t &st_left){
	st_left = found;
	size_t st_right = found;
	unordered_set<char> exclusions{'+', '-', '*', '/', '^', '!'};
	while(st_left != 0){
		if(exclusions.find(str[st_left - 1]) == exclusions.end()){
			--st_left;
			continue;
		}
		if(!isdigit(str[st_left - 2])){	//忽略正负号。
			--st_left;
			continue;
		}
	 	break;
	 }

	while(st_right != str.size()){
		if (exclusions.find(str[st_right + 1]) == exclusions.end()){
			++st_right;
	 		continue;
		}
		if(st_right == found){
			++st_right;
			continue;
		}
		break;
	}

	try{
		left = Fraction(str.substr(st_left, found - st_left));
		right = Fraction(str.substr(found + 1, st_right - found));
	}
	catch(invalid_argument){
		if(str[found] != '!')
			print_error("INVALID INPUT");
	}
	return st_right - st_left + 1;
}

void print_error(const string error){
	cerr << "\n---WARNING---\n" << error << endl;
}
