#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

int limit = 10000;	//进制
int number = 4;		//几个0

string multiply(string s1, string s2){
	vector<int> ans;
	vector<int> a1, a2;

	string temp;
	int len1 = s1.size();
	while(len1 / number){
		temp = s1.substr(len1 - number, number);
		a1.push_back(stoi(temp));
		len1 -= number;
	}
	if(len1){
		temp = s1.substr(0, len1);
		a1.push_back(stoi(temp));
	}
	int len2 = s2.size();
	while(len2 / number){
		temp = s2.substr(len2 - number, number);
		a2.push_back(stoi(temp));
		len2 -= number;
	}
	if(len2){
		temp = s2.substr(0, len2);
		a2.push_back(stoi(temp));
	}

	ans.resize(a1.size() + a2.size());
	int x = 0;	//进位
	for(auto i = 0; i < a1.size(); ++i){
		for(auto j = 0; j < a2.size(); ++j){
			ans[i+j] += a1[i] * a2[j];
		}
	}

	for(int a = 0; a != ans.size(); ++a){
		ans[a] += x;
		x = ans[a] / limit;
		ans[a] %= limit;
	}

	if(x > 0){
		ans.push_back(x % limit);
		x %= limit;
	}
	reverse(ans.begin(), ans.end());
	string result;
	for(auto n : ans){
		string sn = to_string(n);
		result += string(number - sn.size(), '0') + sn;
	}
	//移除开头空格
	result.erase(result.begin(), find_if(result.begin(), result.end(), [](auto ch){return ch != '0';}));
	return result;
}