#pragma once
#include"pch.h"
#include<vector>
using namespace std;
class Lexical
{
public:
	friend class Output;
	Lexical(vector<string>);
	~Lexical();
	void showPro();//��ӡ�ı�����
	string getPro();//��ȡ����
	TokenSet getToken();//��ȡToken
	vector<TokenSet> getTokenSet();//����TokenSet(�����﷨����)
	string tokToName(TokenType, string);//��tokenת��Ϊ��Ӧ�ַ�����
	void doLexical();//ִ�дʷ�����
	void showResult();//չʾ�ʷ��������
	TokenType idCase(string);//��tokenΪIDʱ��������Ƿ�Ϊ������
private:
	int len; // �����ֽ���
	int toknum; // token��Ŀ
	int index; //��ǰ��ȡλ��
	vector<string> V;//�洢����
	vector<TokenSet> TokSet; // �������е�token
	string proText; // ������һ�𱣴��� proText ��
	int line; // ��ǰ��ȡ������ĵ�n��
};

