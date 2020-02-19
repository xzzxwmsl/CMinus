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
	//���������ն���

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
	while ((currentTok != INT) && (currentTok != VOID) && (currentTok != ENDFILE))//����������INT||VOID��ʼ
	{
		cout << "����������" << endl;
		currentTok = getToken();
		if (currentTok == ENDFILE)
			break;
	}
	while (currentTok == INT || currentTok == VOID)//����������INT||VOID��ʼ
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
				p->sibling = q;//����Ϊ�ֵܽڵ�
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
	TreeNode* t = NULL;//�ڵ�����
	TreeNode* p = NULL;//����
	TreeNode* q = NULL;//ID
	TreeNode* s = NULL;//�±�
	TreeNode* a = NULL;//����Ԫ��=������+�±�
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
		case LPAREN://�Ǻ�������
			t = newTree(FunK);
			t->child[0] = p;//�ӽڵ�0������
			t->child[1] = q;//�ӽڵ�1�Ǻ�����
			match(LPAREN);//ƥ��������
			t->child[2] = params();//�ֽڵ�2�Ǻ��������б�
			match(RPAREN);//ƥ��������
			t->child[3] = compound_stmt();//�ӽڵ�3������
			break;
		case LBRACKET://����������
			t = newTree(Var_DeclK);//����
			a = newTree(Arry_ElementK);//����Ԫ��
			t->child[0] = p;// �ӽڵ�0������
			t->child[1] = a;//�ӽڵ�1������Ԫ��
			match(LBRACKET);//ƥ�������� 
			s = newTree(ConstK);//����
			s->attr.val = atoi(tokT2String(currentTok).c_str());
			match(NUM);//ƥ������
			a->child[0] = q;
			a->child[1] = s;
			match(RBRACKET);
			match(SEMI);//ƥ�������źͷֺ�
			break;
		case SEMI://�ֺţ��Ǳ�������
			t = newTree(Var_DeclK);
			t->child[0] = p;
			t->child[1] = q;//����=����+ID
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




TreeNode* Parse::params()//���������������
{
	TreeNode* t = newTree(ParamsK);
	TreeNode* p = NULL;
	if (currentTok == VOID)
	{
		p = newTree(VoidK);
		match(VOID);
		if (currentTok == RPAREN)//�����ţ�������Ϊ Type funName ( void )
		{
			if (t != NULL)
				t->child[0] = p;
		}
		else //������Ϊ Type funName(void id, [����])
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
	while (currentTok==COMMA)//����������INT||VOID��ʼ
	{
		match(COMMA);//ƥ�䶺��
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
				p->sibling = q;//����Ϊ�ֵܽڵ�
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
			if (currentTok == LBRACKET)//����
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
				q->sibling = p;//���Ӳ��е��ֵܽڵ�
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
				p->sibling = q;//�����ֵܽڵ�
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
	else if (currentTok == LBRACE) { //������
		t = compound_stmt();
	}
	else if (currentTok == ID) {
		t = expression_stmt();
	}
	else {
		currentTok = getToken();//�����ϣ���ȡ�¸�token
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
{  //�˴�����д���ο��ı��˵Ĵ���
	TreeNode * t = var();
	if (t == NULL) // ������ ID ��ͷ��ֻ���� simple_expression ���
	{
		t = simple_expression(t);
	}
	else // �� ID ��ͷ�������Ǹ�ֵ��䣬�� simple_expression �е� var �� call���͵����

	{
	TreeNode * p = NULL;
	if (currentTok == ASSIGN) // ��ֵ���
	{
	p = newTree(AssignK);
	match(ASSIGN);
	p->child[0] = t; //��ֵ������ΪID
	p->child[1] = expression();//���ʽ
	return p;
	}
	else //simple_expression �е� var �� call ���͵����
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
	if (currentTok == LBRACKET) {//����
		match(LBRACKET);
		k = expression();
		match(RBRACKET);
		t = newTree(Arry_ElementK);
		t->child[0] = p;
		t->child[1] = k;
	}
	else {
		t = p;//���Ǳ�����
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
	if (k != NULL) //kΪ������ var, ����Ϊ call ��	var
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
			else // ������������㣬������һ��
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
					p->sibling = q;//�ֵܽڵ�
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
