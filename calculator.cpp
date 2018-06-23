#include <iostream>
#include <sstream>
#include <iomanip>
#include <limits>

using namespace std;

string basic_calculate(size_t begin, size_t count, string str);
string bracket_process(size_t begin, size_t count, string str);

void print_error(string error);
void verify(long double d);

ostringstream sstream;		//将double转为string的流。

inline long double get_operands(const string str, const size_t found, long double &right, size_t &length, size_t &st_left);

int main(){
	sstream.precision(std::numeric_limits<double>::digits10);

	string str = "";
	while(cout << "Enter the expression: " && cin >> str){
		sstream.str("");
		sstream << bracket_process(0, str.size(), str);
		cout << "The answer is: " << basic_calculate(0, str.size(), sstream.str()) << "\n" << endl;
	}
}


//基本运算。
string basic_calculate(size_t begin, size_t count, string str){
	string cal_str(str.substr(begin, count));
	size_t found;				//运算符号位置。
	size_t length;				//计算对象长度。
	size_t st_left, st_right;	//执行运算部分的起始位置。
	long double left, right, result;	//运算符左右侧数字以及计算结果。
	

	//执行三级运算(幂运算)。
	found = cal_str.find_first_of("^");
	while(found != string::npos){
		left = get_operands(cal_str, found, right, length, st_left);

		result = left;
		if(right == 0){
			result = 1;
		}
		else if(right > 0){
			while(--right != 0){
				result *= left;
			}
		}
		else if(right < 0){
			while(++right != 0){
				result *= left;
			}
			result = 1 / result;
		}
		verify(result);

		sstream.str("");
		sstream << result;
		cal_str.replace(st_left, length, sstream.str());
		found = cal_str.find_first_of("^");
	}

	//执行二级运算(乘除运算)。
	found = cal_str.find_first_of("*/");
	while(found != string::npos){
		st_left = found - 1;
		st_right = found + 1;
		if(cal_str[found] == '*'){	//执行乘法运算。
			left = get_operands(cal_str, found, right, length, st_left);
			result = left * right;
			verify(result);

			//将结果(double)转为string。
			sstream.str("");			//重置流。
			sstream << result;
			cal_str.replace(st_left, length, sstream.str());
		}
		else if(cal_str[found] == '/'){	//执行除法运算。
			left = get_operands(cal_str, found, right, length, st_left);
			// right = stod(sstream.str());
			result = left / right;
			verify(result);

			sstream.str("");
			ostringstream sstream;
			sstream << result;
			cal_str.replace(st_left, length, sstream.str());
		}
		found = cal_str.find_first_of("*/");

	}
	//执行一级运算(加减运算)。
	found = cal_str.find_first_of("+-");
	while(found != string::npos){
		if(cal_str[found - 1] == 'e'){		//忽略科学记数法。
				found = cal_str.find_first_of("+-", found + 1);
				continue;
		}
		if(cal_str[found] == '+'){
			left = get_operands(cal_str, found, right, length, st_left);
			result = left + right;
			verify(result);

			sstream.str("");
			sstream << result;
			cal_str.replace(st_left, length, sstream.str());
		}
		else if(cal_str[found] == '-'){
			if((found - 1 >= 0) && isdigit(cal_str[found - 1])){
				left = get_operands(cal_str, found, right, length, st_left);
				result = left - right;
				verify(result);

				sstream.str("");
				sstream << result;
				cal_str.replace(st_left, length, sstream.str());
			}
			else{
				found = cal_str.find_first_of("+-", found + 1);
				continue;
			}
		}
		found = cal_str.find_first_of("+-");
	}
	return cal_str;
}

//处理括号内的数据。
string bracket_process(size_t begin, size_t count, string str){
	string pending_str(str.substr(begin, count));
	auto fir_front_bracket = pending_str.find('(');
	if(fir_front_bracket != string::npos){
		auto sec_front_bracket = pending_str.find('(', fir_front_bracket + 1);
		if(sec_front_bracket != string::npos){
			string sec_pending_str = pending_str.substr(sec_front_bracket);
			//cout << "sec_pending_str: " << sec_pending_str << endl;
			pending_str.erase(sec_front_bracket, sec_pending_str.size());
			sec_pending_str = bracket_process(0, sec_pending_str.size(), sec_pending_str);
			pending_str += sec_pending_str;
		}
		auto fir_back_bracket = pending_str.find(')');

		if(fir_back_bracket != string::npos){
			string calculated_str = basic_calculate(fir_front_bracket + 1, fir_back_bracket - fir_front_bracket - 1, pending_str);
			pending_str.replace(fir_front_bracket, fir_back_bracket - fir_front_bracket + 1, calculated_str);
		}

	}
	return pending_str;
}


inline long double get_operands(const string str, const size_t found, long double &right, size_t &length, size_t &st_left){
	st_left = found - 1;
	size_t st_right = found + 1;
	while(st_left != 0 && (isdigit(str[st_left]) || str[st_left] == '.'))
		--st_left;
	while(st_right != str.size() && (isdigit(str[st_right] || str[st_right] == '.')))
		++st_right;
	right = stod(str.substr(found + 1));
	length = length;
	return stod(str.substr(st_left));
}

void print_error(const string error){
	cerr << "\n---WARNING---\n" << error << endl;
}

void verify(const long double d){
	if(d + 100 == d)
		print_error("Overflow\n");
}

//调试语句。
//cout << "st_left: " << st_left << " st_right: " << st_right << endl;
//cout << "left: " << left << " right: " << right << " result: " << result << endl ;
//cout << "var: " << sstream.str() << " cal_str: " << cal_str << "  found: " << found << endl;