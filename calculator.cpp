#include <iostream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <set>
#include "fraction.h"

using namespace std;

string basic_calculate(size_t begin, size_t count, string str);
string bracket_process(size_t begin, size_t count, string str);

void print_error(string error);
void verify(const Fraction d);

inline Fraction get_operands(const string str, const size_t found, Fraction &right, size_t &length, size_t &st_left);

int main(){
	string str = "";
	while(cout << "Enter the expression: " && getline(cin, str)){
		str.erase(remove_if(str.begin(), str.end(), ::isspace), str.end());
		str = bracket_process(0, str.size(), str);
		string answer = basic_calculate(0, str.size(), str);
		cout << "The answer is: " << answer << "\n" << endl;
	}
}


//基本运算。
string basic_calculate(size_t begin, size_t count, string str){
	string cal_str(str.substr(begin, count));
	size_t found;				//运算符号位置。
	size_t length;				//计算对象长度。
	size_t st_left, st_right;	//执行运算部分的起始位置。
	Fraction left, right, result;	//运算符左右侧数字以及计算结果。
	

	//执行三级运算(幂运算)。
	found = cal_str.find_first_of("^!");
	while(found != string::npos){
		left = get_operands(cal_str, found, right, length, st_left);
		if(cal_str[found] == '^'){
			result = left;
			if(right.n == 0){
				result.n = 1;
				result.d = 1;
			}
			else if(right.n > 0){
				while(--right.n != 0){
					result = result * left;
				}
			}
			else if(right.n < 0){
				while(++right.n != 0){
					result = result * left;
				}
				result = Fraction() / result;
			}
			cal_str.replace(st_left, length, to_string(result));
		}
		else if(cal_str[found] == '!'){
			if((found+1) != cal_str.size() && cal_str[found + 1] == '!'){		//判断是否为双阶乘
				Wint ans(1);
				int i;
				if(left.n % 2 == 1)
					i = 1;
				else
					i = 2;
				for(; left.n >= i; i += 2)
					ans *= i;
				length = found - st_left + 3;
				result.n = ans;
			}
			else{
				Wint ans(1);
				for(int i = 1; left.n != i; ++i)
					ans *= i;
				length = found - st_left + 2;
				result.n = ans;

			}
		}

		verify(result);
		cal_str.replace(st_left, length, to_string(result));

		found = cal_str.find_first_of("^!");
	}

	//执行二级运算(乘除运算)。
	found = cal_str.find_first_of("*/");
	while(found != string::npos){
		left = get_operands(cal_str, found, right, length, st_left);
		if(cal_str[found] == '*'){	//执行乘法运算。
			result = left * right;

		}
		else if(cal_str[found] == '/'){	//执行除法运算。
			result = left / right;
		}

		verify(result);

		cal_str.replace(st_left, length, to_string(result));
		found = cal_str.find_first_of("*/");
	}

	//执行一级运算(加减运算)。
	found = cal_str.find_first_of("+-");
	while(found != string::npos){
		left = get_operands(cal_str, found, right, length, st_left);
		if(st_left == found){
			found = cal_str.find_first_of("+-", found + 1);
			continue;
		}
		if(cal_str[found] == '+'){
			result = left + right;
		}
		else if(cal_str[found] == '-'){
			result = left - right;
		}

		verify(result);

		cal_str.replace(st_left, length, to_string(result));
		found = cal_str.find_first_of("+-");
	}
	return cal_str;
}

//处理括号内的数据。
string bracket_process(size_t begin, size_t count, string str){
	string pending_str(str.substr(begin, count));
	auto fir_front_bracket = pending_str.find_first_of("{[(");
	if(fir_front_bracket != string::npos){
		auto sec_front_bracket = pending_str.find_first_of("{[(", fir_front_bracket + 1);
		if(sec_front_bracket != string::npos){
			string sec_pending_str = pending_str.substr(sec_front_bracket);
			pending_str.erase(sec_front_bracket, sec_pending_str.size());
			sec_pending_str = bracket_process(0, sec_pending_str.size(), sec_pending_str);
			pending_str += sec_pending_str;
		}
		auto fir_back_bracket = pending_str.find_first_of("}])");

		if(fir_back_bracket != string::npos){
			string calculated_str = basic_calculate(fir_front_bracket + 1, fir_back_bracket - fir_front_bracket - 1, pending_str);
			pending_str.replace(fir_front_bracket, fir_back_bracket - fir_front_bracket + 1, calculated_str);
		}

	}
	return pending_str;
}


inline Fraction get_operands(const string str, const size_t found, Fraction &right, size_t &length, size_t &st_left){
	Fraction left;
	st_left = found;
	size_t st_right = found;
	set<char> exclusions{'+', '-', '*', '/', '^', '!'};
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
	length = st_right - st_left + 1;
	return left;
}

void print_error(const string error){
	cerr << "\n---WARNING---\n" << error << endl;
}

void verify(const Fraction d){
	// if(d + 100 == d)
	// 	print_error("OVERFLOW\n");
}