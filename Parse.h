#pragma once
#include"pch.h"
#include<queue>
class Parse
{
public:
	Parse(vector<TokenSet>);
	~Parse();
	TokenType getToken();
	void showToken();
	TreeNode* newTree(ExpKind);
	TreeNode* newTree(StmtKind);//重载建树函数
	//语法结构
	TreeNode* parse();
	TreeNode* declaration_list();
	TreeNode* declaration();
	//TreeNode* var_declaration();
	//TreeNode* type_specifier();
	//TreeNode* fun_declaration();
	TreeNode* params();
	TreeNode* param_list(TreeNode*);
	TreeNode* param(TreeNode*);
	TreeNode* compound_stmt();
	TreeNode* local_declarations();
	TreeNode* statement_list();
	TreeNode* statement();
	TreeNode* expression_stmt();
	TreeNode* selection_stmt();
	TreeNode* iteration_stmt();
	TreeNode* return_stmt();
	TreeNode* expression();
	TreeNode* var();
	TreeNode* simple_expression(TreeNode * p);
	TreeNode* additive_expression(TreeNode * p);
	TreeNode* term(TreeNode * p);
	TreeNode* factor(TreeNode * p);
	TreeNode* call(TreeNode * p);
	TreeNode* args();
	//工具
	void match(TokenType tokT);
	void printTokenType(TokenType);
	string tokT2String(TokenType);
	char* copyString(const char *s);
	void printSpaces();
	void printTree(TreeNode*);
private:
	vector<TokenSet> tokenSet;
	queue<TokenType> tokenQueue;
	queue<string> tokenName;//记录ID token的名字
	TokenType currentTok;//当前处理的token
	int indentno;
	int index;//tokenSet的位置
};

