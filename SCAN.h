#pragma once
#include"pch.h"
#include<vector>
using namespace std;
class SCAN
{
public:
	SCAN();
	~SCAN();
	void getLocation();//��ȡ����λ��
	vector<string> getText();//��ȡ����
private:
	string Location;//�洢����λ��
	string pro;//�洢�����ı�
	vector<string> V;//�洢�����ı�
};

