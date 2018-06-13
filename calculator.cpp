#include <iostream>
#include <sstream>

using namespace std;

string basic_calculate(string::iterator begin, string::iterator end);

int main(){
	string str = "";
	
	cout << "Enter the expression: ";
	while(cin >> str){
		str = basic_calculate(str.begin(), str.end());
		cout << "The answer is: " << str << endl;
		cout << "Enter the expression: ";
	}
}


//基本运算。
string basic_calculate(string::iterator begin, string::iterator end){
	string cal_str(begin, end);
	size_t found;				//运算符号位置。
	size_t st_left, st_right;	//执行运算部分的起始位置。
	double left, right, result;	//运算符左右侧数字以及计算结果。
	ostringstream sstream;		//将double转为string的流。

	//执行三级运算(幂运算)。
	found = cal_str.find_first_of("^");
	while(found != string::npos){
		st_left = found - 1;
		st_right = found + 1;
		sstream << stod(cal_str.substr(st_right));
		st_right += sstream.str().size();

		while(st_left != 0 && (cal_str[st_left - 1] == '.' || isdigit(cal_str[st_right - 1]))){
			--st_left;
		}

		left = stod(cal_str.substr(st_left));
		right = stod(sstream.str());
		result = left;
		while(--right != 0){
			result *= left;
		}


		sstream.str("");
		sstream << result;
		cal_str.replace(st_left, st_right - st_left, sstream.str());
		found = cal_str.find_first_of("^");
	}

	//执行二级运算(乘除运算)。
	found = cal_str.find_first_of("*/");
	while(found != string::npos){
		st_left = found - 1;
		st_right = found + 1;
		if(cal_str[found] == '*'){	//执行乘法运算。
			//将运算符后的字符转为数字后转回字符，得到double。
			sstream.str("");
			sstream << stod(cal_str.substr(st_right));
			st_right += sstream.str().size();
			//从运算符开始向前递减，直到遇到非数字或非小数点字符。
			while(st_left != 0 && (cal_str[st_left - 1] == '.' || isdigit(cal_str[st_left - 1]))){
				--st_left;
			}

			left = stod(cal_str.substr(st_left));
			right = stod(sstream.str());
			result = left * right;
			//将结果(double)转为string。
			sstream.str("");			//重置流。
			sstream << result;
			cal_str.replace(st_left, st_right - st_left, sstream.str());
		}
		else if(cal_str[found] == '/'){	//执行除法运算。
			sstream << stod(cal_str.substr(st_right));
			st_right += sstream.str().size();

			while(st_left != 0 && (cal_str[st_left - 1] == '.' || isdigit(cal_str[st_left - 1]))){
				--st_left;
			}

			left = stod(cal_str.substr(st_left));
			right = stod(sstream.str());
			result = left / right;

			sstream.str("");
			ostringstream sstream;
			sstream << result;
			cal_str.replace(st_left, st_right - st_left, sstream.str());
		}
		found = cal_str.find_first_of("*/");

	}

	//执行一级运算(加减运算)。
	found = cal_str.find_first_of("+-");
	while(found != string::npos){
		st_left = found - 1;
		st_right = found + 1;
		if(cal_str[found] == '+'){
			if(cal_str[found - 1] == 'e'){		//忽略科学记数法。
				found = cal_str.find_first_of("+-", found + 1);
				continue;
			}
			sstream.str("");
			sstream << stod(cal_str.substr(st_right));
			st_right += sstream.str().size();

			while(st_left != 0 && cal_str[st_left - 1] == '.' || isdigit(cal_str[st_left - 1])){
				--st_left;
			}

			left = stod(cal_str.substr(st_left));
			right = stod(sstream.str());
			result = left + right;

			sstream.str("");
			sstream << result;
			cal_str.replace(st_left, st_right - st_left, sstream.str());
		}
		else if(cal_str[found] == '-'){
			sstream.str("");
			sstream << stod(cal_str.substr(st_right));
			st_right += sstream.str().size();

			while(st_left != 0 && cal_str[st_left - 1] == '.' || isdigit(cal_str[st_left - 1])){
				--st_left;
			}

			left = stod(cal_str.substr(st_left));
			right = stod(sstream.str());
			result = left - right;

			sstream.str("");
			sstream << result;
			cal_str.replace(st_left, st_right - st_left, sstream.str());
		}
		found = cal_str.find_first_of("+-");
	}

	return cal_str;
}

//调试语句。
//cout << "st_left: " << st_left << " st_right: " << st_right << endl;
//cout << "left: " << left << " right: " << right << " result: " << result << endl ;
//cout << "var: " << sstream.str() << " cal_str: " << cal_str << "  found: " << found << endl;