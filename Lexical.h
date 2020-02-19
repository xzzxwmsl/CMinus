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
	void showPro();//打印文本代码
	string getPro();//读取程序
	TokenSet getToken();//读取Token
	vector<TokenSet> getTokenSet();//返回TokenSet(用于语法分析)
	string tokToName(TokenType, string);//讲token转化为相应字符串名
	void doLexical();//执行词法分析
	void showResult();//展示词法分析结果
	TokenType idCase(string);//当token为ID时，检查其是否为保留字
private:
	int len; // 程序字节数
	int toknum; // token数目
	int index; //当前读取位置
	vector<string> V;//存储代码
	vector<TokenSet> TokSet; // 保存所有的token
	string proText; // 将程序一起保存在 proText 中
	int line; // 当前读取到程序的第n行
};

