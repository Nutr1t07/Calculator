#include <iostream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <algorithm>
#include "fraction.h"
#include "RPN.h"

using namespace std;

int main(){
	string str;
	while(cout << "Enter the expression: " && getline(cin, str)){
		str.erase(remove_if(str.begin(), str.end(), ::isspace), str.end());	//去除表达式中的空格。
		// str = bracket_process(str);										//处理空格。
		string answer = calculate(str);
		cout << "The answer is: " << answer << "\n" << endl;
	}
}
