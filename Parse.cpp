#include "pch.h"
#include "Parse.h"
#include<string.h>
using namespace std;
Parse::Parse(vector<TokenSet>  tokS) {
	tokenSet = tokS;
	for (vector<TokenSet> ::iterator i = tokS.begin(); i != tokS.end(); i++) {
		cout << (*i).tokenName << " " << (*i).tokenType << " " << (*i).tokT << endl;
		tokenQueue.push((*i).tokT);
		tokenName.push((*i).tokenName);
	}
	indentno = 0;
	index = 0;
}

Parse::~Parse() {
	while (!tokenQueue.empty()) {
		tokenQueue.pop();
	}
	while (!tokenName.empty()){
		tokenName.pop();
	}
	tokenSet.clear();
	//销毁类后清空队列

}

TokenType Parse::getToken() {
	TokenType tokT = tokenSet[index++].tokT;
	return tokT;
}

void Parse::showToken() {
	int i = tokenQueue.size();
	while (i-- > 0) {
		cout << tokenQueue.front();
		printTokenType(tokenQueue.front()); cout << endl;
		tokenQueue.pop();
	}
}

string Parse::tokT2String(TokenType expected) {
	string s="";
	switch (expected)
	{
	case IF:    s = "IF"; break;
	case ELSE:  s = "ELSE"; break;
	case INT:   s = "INT"; break;
	case RETURN:  s = "RETURN"; break;
	case VOID: s = "VOID"; break;
	case WHILE: s = "WHILE"; break;
	case NUM:s = "NUMM"; break;
	case ID:s = "ID"; break;
	case ASSIGN: s = "ASSIGN "; break;
	case LT: s = "LT"; break;
	case LE:s = "LE"; break;
	case GT: s = "GT"; break;
	case GE:s = "GE"; break;
	case EQ: s = "EQ"; break;
	case NEQ: s = "NEQ"; break;
	case LPAREN: s = "LPAREN"; break;
	case RPAREN: s = "RPAREN"; break;
	case LBUFFER: s = "LBUFFER"; break;
	case RBUFFER: s = "RBUFFER"; break;
	case LBRACKET: s = "LBRACKET"; break;
	case RBRACKET: s = "RBRACKET"; break;
	case LBRACE: s = "LBRACE"; break;
	case RBRACE: s = "RBRACE"; break;
	case SEMI: s = "SEMI"; break;
	case COMMA: s = "COMMA"; break;
	case PLUS: s = "PLUS"; break;
	case MINUS:s = "MINUS"; break;
	case MUL:s = "MUL"; break;
	case DIV: s = "DIV"; break;
	case ENDFILE: s = "ENDFILE"; break;
	case ERROR:s = "ERROR"; break;
	default:s = "UNKNOW";
	}
	return s;
}

void Parse::printTokenType(TokenType expected) {
	cout << tokT2String(expected);
}

TreeNode* Parse::newTree(ExpKind kind) {

	TreeNode * T = (TreeNode *)malloc(2*sizeof(TreeNode));
	if (T == NULL) cout << "ERRO" << endl;
	else {
		for (int i = 0; i < CHILDNUM; i++) T->child[i] = NULL;
		T->sibling = NULL;
		T->nodekind = ExpK;
		T->kind.exp = kind;
	}
	return T;
}



TreeNode* Parse::newTree(StmtKind kind) {
	TreeNode * t = (TreeNode *)malloc(sizeof(TreeNode));
	int i;
	if (t == NULL) cout << "ERRO" << endl;
	else {
		for (i = 0; i < CHILDNUM; i++) t->child[i] = NULL;
		t->sibling = NULL;
		t->nodekind = StmtK;
		t->kind.stmt = kind;
	}
	return t;
}

void Parse::match(TokenType tokT)
{
	if (currentTok == tokT) currentTok = getToken();
}

TreeNode* Parse::parse() {
	TreeNode* t;
	currentTok = getToken();
	t = declaration_list();
	if (currentTok != ENDFILE) {
		cout << "ERROR END" << endl;
	}
	return t;
}

TreeNode* Parse::declaration_list()
{
	TreeNode* t = declaration();
	TreeNode* p = t;
	while ((currentTok != INT) && (currentTok != VOID) && (currentTok != ENDFILE))//程序声明以INT||VOID开始
	{
		cout << "非类型声明" << endl;
		currentTok = getToken();
		if (currentTok == ENDFILE)
			break;
	}
	while (currentTok == INT || currentTok == VOID)//程序声明以INT||VOID开始
	{
		TreeNode* q;
		q = declaration();
		if (q != NULL)
		{
			if (t == NULL)
			{
				t = p = q;
			}
			else
			{
				p->sibling = q;//连接为兄弟节点
				p = q;
			}
		}
	}
	return t;
}

char * Parse::copyString(const char * s) {
	int n;
	char * t;
	if (s == NULL) return NULL;
	n = strlen(s) + 1;
	t =(char*) malloc(n);
	strcpy(t, s);
	return t;
}

TreeNode* Parse::declaration()
{
	TreeNode* t = NULL;//节点类型
	TreeNode* p = NULL;//类型
	TreeNode* q = NULL;//ID
	TreeNode* s = NULL;//下标
	TreeNode* a = NULL;//数组元素=数组名+下标
	if (currentTok == INT)
	{
		p = newTree(IntK);
		match(INT);
	}
	else if (currentTok == VOID)
	{
		p = newTree(VoidK);
		match(VOID);
	}
	else
	{
		cout << "TYPE ERROR" << endl;
	}
	if (p != NULL && currentTok == ID)
	{
		q = newTree(IDK);
		if (q != NULL) {
			q->attr.name = copyString(tokenSet[index-1].tokenName.c_str());
		}//tokT2String(currentTok);
		match(ID);

		switch (currentTok) {
		case LPAREN://是函数声明
			t = newTree(FunK);
			t->child[0] = p;//子节点0是类型
			t->child[1] = q;//子节点1是函数名
			match(LPAREN);//匹配左括号
			t->child[2] = params();//字节点2是函数声明列表
			match(RPAREN);//匹配右括号
			t->child[3] = compound_stmt();//子节点3是语句块
			break;
		case LBRACKET://是数组声明
			t = newTree(Var_DeclK);//类型
			a = newTree(Arry_ElementK);//数组元素
			t->child[0] = p;// 子节点0是类型
			t->child[1] = a;//子节点1是数组元素
			match(LBRACKET);//匹配左方括号 
			s = newTree(ConstK);//常数
			s->attr.val = atoi(tokT2String(currentTok).c_str());
			match(NUM);//匹配数字
			a->child[0] = q;
			a->child[1] = s;
			match(RBRACKET);
			match(SEMI);//匹配右括号和分号
			break;
		case SEMI://分号，是变量声明
			t = newTree(Var_DeclK);
			t->child[0] = p;
			t->child[1] = q;//变量=类型+ID
			match(SEMI);
			break;
		default:
			cout << "ERROR" << endl;
		}

	}
	else
	{
		cout << "ERROR" << endl;
	}
	return t;
}




TreeNode* Parse::params()//函数声明参数类别
{
	TreeNode* t = newTree(ParamsK);
	TreeNode* p = NULL;
	if (currentTok == VOID)
	{
		p = newTree(VoidK);
		match(VOID);
		if (currentTok == RPAREN)//右括号，函数形为 Type funName ( void )
		{
			if (t != NULL)
				t->child[0] = p;
		}
		else //函数形为 Type funName(void id, [……])
		{
			t->child[0] = param_list(p);
		}
	}
	else if (currentTok == INT)
	{
		t->child[0] = param_list(p);
	}
	else
	{
		cout << "ERROR" << endl;
	}
	return t;
}


TreeNode* Parse::param_list(TreeNode* k)
{
	TreeNode* t = param(k);
	TreeNode* p = t;
	k = NULL;
	while (currentTok==COMMA)//程序声明以INT||VOID开始
	{
		match(COMMA);//匹配逗号
		TreeNode* q=NULL;
		q = param(k);
		if (q != NULL)
		{
			if (t == NULL)
			{
				t = p = q;
			}
			else
			{
				p->sibling = q;//连接为兄弟节点
				p = q;
			}
		}
	}
	return t;
}


TreeNode* Parse::param(TreeNode* k)
{
	TreeNode* t = newTree(ParamK);
	TreeNode* p = NULL;
	TreeNode* q = NULL;

	if (k == NULL && currentTok == VOID)
	{
		p = newTree(VoidK);
		match(VOID);
	}
	else if (k == NULL && currentTok == INT)
	{
		p = newTree(IntK);
		match(INT);
	}
	else if (k != NULL)
	{
		p = k;
	}
	if (p != NULL)
	{
		t->child[0] = p;
		if (currentTok == ID)
		{
			q = newTree(IDK);
			q->attr.name = copyString(tokenSet[index - 1].tokenName.c_str());
			t->child[1] = q;
			match(ID);
		}
		if (currentTok == LBRACKET && (t->child[1] != NULL))
		{
			match(LBRACKET);
			t->child[2] = newTree(IDK);
			match(RBRACKET);
		}
		else
		{
			return t;
		}
	}
	else
	{
		;
	}
	return t;
}


TreeNode* Parse::compound_stmt()
{
	TreeNode* t = newTree(CompK);
	match(LBRACE);//match {
	t->child[0] = local_declarations();
	t->child[1] = statement_list();
	match(RBRACE);//match }
	return t;
}


TreeNode* Parse::local_declarations()
{
	TreeNode * t = NULL;
	TreeNode * q = NULL;
	TreeNode * p = NULL;
	while (currentTok == INT || currentTok == VOID)
	{
		p = newTree(Var_DeclK);
		TreeNode * q1=NULL;
		if (currentTok == INT)
		{
			q1 = newTree(IntK);
			match(INT);
		}
		else if (currentTok == VOID)
		{
		    q1 = newTree(VoidK);
			match(VOID);
		}
		p->child[0] = q1;
		TreeNode * q2;
		if ((p != NULL) && (currentTok == ID))
		{
			q2= newTree(IDK);
			q2->attr.name = copyString(tokenSet[index - 1].tokenName.c_str());
			p->child[1] = q2;
			match(ID); 
			if (currentTok == LBRACKET)//数组
			{
				TreeNode * q3 = newTree(Var_DeclK);
				p->child[3] = q3;
				match(LBRACKET);
				match(RBRACKET);
				match(SEMI);
			}
			else if (currentTok == SEMI)
			{
				match(SEMI);
			}
			else
			{
				match(SEMI);
			}
		}
		if (p != NULL)
		{
			if (t == NULL)
				t = q = p;
			else
			{
				q->sibling = p;//增加并列的兄弟节点
				q = p;
			}
		}
	}
	return t;
}


TreeNode* Parse::statement_list()
{
	TreeNode * t = statement();
	TreeNode * p = t;
	while ( currentTok == IF ||  currentTok == LBRACKET ||currentTok == ID ||  currentTok == WHILE  ||
		currentTok == RETURN ||  currentTok == SEMI  ||  currentTok == LPAREN|| currentTok == NUM)
	{
		TreeNode * q;
		q = statement();
		if (q != NULL)
		{
			if (t == NULL)
			{
				t = p = q;
			}
			else
			{
				p->sibling = q;//增加兄弟节点
				p = q;
			}
		}
	}
	return t;
}


TreeNode* Parse::statement()
{
	TreeNode* t = NULL;
	if (currentTok == IF) {
		t = selection_stmt();
	}
	else if (currentTok == WHILE) {
		t = iteration_stmt();
	}
	else if (currentTok == RETURN) {
		t = return_stmt();
	}
	else if (currentTok == LBRACE) { //左花括号
		t = compound_stmt();
	}
	else if (currentTok == ID) {
		t = expression_stmt();
	}
	else {
		currentTok = getToken();//不符合，读取下个token
	}
	return t;
}


TreeNode* Parse::expression_stmt()
{
	TreeNode* t = NULL;
	if (currentTok == SEMI) {
		match(SEMI);
	}
	else {
		t = expression();
		match(SEMI);
	}
	return t;
}


TreeNode* Parse::selection_stmt()
{
	TreeNode* t = newTree(IfK);
	match(IF);
	match(LPAREN);
	t->child[0] = expression();//child0 is expression
	match(RPAREN);
	t->child[1] = statement();//child1 is statement
	if (currentTok == ELSE) {
		match(ELSE);
		t->child[2] = statement();//child2 is also statement
	}
	return t;
}


TreeNode* Parse::iteration_stmt()
{
	TreeNode* t = newTree(WhileK);
	match(WHILE);
	match(LPAREN);
	t->child[0] = expression();//child0 is expression
	match(RPAREN);
	t->child[1] = statement();//child1 is statement
	return t;
}



TreeNode* Parse::return_stmt()
{
	TreeNode * t = newTree(ReturnK);
	match(RETURN);
	if (currentTok == SEMI)
	{
		match(SEMI);
	}
	else
	{
		t->child[0] = expression();
		match(SEMI);
	}
	return t;
}


TreeNode* Parse::expression()
{  //此处不会写，参考的别人的代码
	TreeNode * t = var();
	if (t == NULL) // 不是以 ID 开头，只能是 simple_expression 情况
	{
		t = simple_expression(t);
	}
	else // 以 ID 开头，可能是赋值语句，或 simple_expression 中的 var 和 call类型的情况

	{
	TreeNode * p = NULL;
	if (currentTok == ASSIGN) // 赋值语句
	{
	p = newTree(AssignK);
	match(ASSIGN);
	p->child[0] = t; //赋值语句左边为ID
	p->child[1] = expression();//表达式
	return p;
	}
	else //simple_expression 中的 var 和 call 类型的情况
	{
	t = simple_expression(t);
	}
	}
	return t;
}


TreeNode* Parse::var()
{
	TreeNode* t = NULL;
	TreeNode* p = newTree(IDK);
	TreeNode* k = NULL;
	p->attr.name = copyString(tokenSet[index - 1].tokenName.c_str());
	match(ID);
	if (currentTok == LBRACKET) {//数组
		match(LBRACKET);
		k = expression();
		match(RBRACKET);
		t = newTree(Arry_ElementK);
		t->child[0] = p;
		t->child[1] = k;
	}
	else {
		t = p;//就是变量名
	}
	return t;
}


TreeNode* Parse::simple_expression(TreeNode * k)
{
	TreeNode* t = additive_expression(k);
	k = NULL;
	if (currentTok == GT || currentTok == GE || currentTok == LT || currentTok == LE || currentTok == EQ || currentTok == NEQ) {
		TreeNode* p = newTree(OpK);
		p->attr.op = currentTok;
		p->child[0] = t;//child0 is add-exp
		t = p;
		match(currentTok);
		t->child[1] = additive_expression(k);
	}
	return t;
}


TreeNode* Parse::additive_expression(TreeNode * k)
{
	TreeNode * t = term(k);
	k = NULL;
	while ((currentTok == PLUS) || (currentTok == MINUS))
	{
		TreeNode * q = newTree(OpK);
		q->attr.op = currentTok;
		q->child[0] = t;
		match(currentTok);
		q->child[1] = term(k);
		t = q;
	}
	return t;
}


TreeNode* Parse::term(TreeNode * k)
{
	TreeNode * t = factor(k);
	k = NULL;
	while ((currentTok == MUL) || (currentTok == DIV))
	{
		TreeNode * q = newTree(OpK);
		q->attr.op = currentTok;
		q->child[0] = t;
		t = q;
		match(currentTok);
		q->child[1] = factor(k);
	}
	return t;
}


TreeNode* Parse::factor(TreeNode * k)
{
	TreeNode * t = NULL;
	if (k != NULL) //k为解析的 var, 可能为 call 或	var
	{
	if (currentTok == LPAREN && k->kind.stmt != Arry_ElementK) //call
	{
	t = call(k);
	}
	else
	{
	t = k;
	}
	}
	else // NO var
	{
		switch (currentTok)
		{
		case LPAREN:
			match(LPAREN);
			t = expression();
			match(RPAREN);
			break;
		case ID:
			k = var();
			if (currentTok == LPAREN && k->kind.stmt != Arry_ElementK)
			{
				t = call(k);
			}
			else // 如果是连续计算，进入这一步
			{
				t = k;
			}
			break;
		case NUM:
			t = newTree(ConstK);
			if ((t != NULL) && (currentTok == NUM))
			{
				t->attr.val = atoi(tokT2String(currentTok).c_str());
			}
			match(NUM); break;
		default:
			currentTok = getToken();
			break;
		}
	}
	return t;
}


TreeNode* Parse::call(TreeNode * k)
{
	TreeNode * t = newTree(CallK);
	if (k != NULL)
		t->child[0] = k;
	match(LPAREN);
	if (currentTok == RPAREN)
	{
		match(RPAREN);
		return t;
	}
	else if (k != NULL)
	{
		t->child[1] = args();
		match(RPAREN);
	}
	return t;
}


TreeNode* Parse::args()
{
	TreeNode * t = newTree(ArgsK);
	TreeNode * s = NULL;
	TreeNode * p = NULL;
	if (currentTok != RPAREN)
	{
		s = expression();
		p = s;
		while (currentTok == COMMA)
		{
			TreeNode * q;
			match(COMMA);
			q = expression();
			if (q != NULL)
			{
				if (s == NULL)
				{
					s = p = q;
				}
				else
				{
					p->sibling = q;//兄弟节点
					p = q;
				}
			}
		}
	}
	if (s != NULL)
	{
		t->child[0] = s;
	}
	return t;
}



void Parse::printSpaces() {
	for (int i = 0; i < indentno; i++) {
		cout << " ";
	}
}



void Parse::printTree(TreeNode* t)
{
	int i;
	indentno += 2;
	while (t!=NULL){
		printSpaces();
		if (t->nodekind == StmtK)
		{
			switch (t->kind.stmt) {
			case IfK:
				cout << "IfK" << endl;
				break;
			case WhileK:
				cout << "WhileK" << endl;
				break;
			case AssignK:
				cout << "AssignK" << endl;
				break;
			case ReturnK:
				cout << "ReturnK" << endl;
				break;
			case CallK:
				cout << "CallK" << endl;
				break;
			case ParamK:
				cout << "ParamK" << endl;
				break;
			case CompK:
				cout << "CompK" << endl;
				break;
			case Arry_ElementK:
				cout << "Arry_ElementK" << endl;
				break;
			case ArgsK:
				cout << "ArgsK" << endl;
				break;
			case VoidK:
				cout << "VoidK" << endl;
				break;
			case IntK:
				cout << "IntK" << endl;
				break;
			case Var_DeclK:
				cout << "Var_DeclK" << endl;
				break;
			case FunK:
				cout << "FunK" << endl;
				break;
			case ParamsK:
				cout << "ParamsK" << endl;
				break;
			default:
				cout << "Unknow Type" << endl;
				break;
			}
		}
		else if (t->nodekind == ExpK)
		{
			switch (t->kind.exp) {
			case OpK:
				cout << "OpK: ";
				cout << tokT2String(t->attr.op) << endl;
				break;
			case ConstK:
				cout << "ConstK: ";
				cout << t->attr.val << endl;
				break;
			case IDK:
				cout << "IdK: ";
				cout << t->attr.name << endl;
				break;
			default:
				cout << "Unknow Type" << endl;
				break;
			}
		}
		else cout << "Unknow Type" << endl;
		for (i = 0; i < CHILDNUM; i++)
			printTree(t->child[i]);
		t = t->sibling;
	}
	indentno -= 2;
}
