#pragma once
#include"pch.h"
#include<vector>
using namespace std;
class SCAN
{
public:
	SCAN();
	~SCAN();
	void getLocation();//获取代码位置
	vector<string> getText();//获取代码
private:
	string Location;//存储代码位置
	string pro;//存储代码文本
	vector<string> V;//存储代码文本
};

