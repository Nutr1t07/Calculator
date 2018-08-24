#include <iostream>
#include <algorithm>
#include "RPN.h"

using namespace std;

int main(){
	string str;
	while(cout << "Enter the expression: " && getline(cin, str)){
		str.erase(remove_if(str.begin(), str.end(), ::isspace), str.end());	//去除表达式中的空格。
		cout << "The result is: " << calculate(str) << "\n" << endl;
	}
}
