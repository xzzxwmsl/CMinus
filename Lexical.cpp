#include "pch.h"
#include "Lexical.h"
#include<vector>
#include<string>
using namespace std;
Lexical::Lexical(vector<string> v) {
	V.clear();
	TokSet.clear();
	V = v;
	line = 1;
	index = 0;
	toknum = 0;

	// 将程序文本存储在 proText 中
	string s;
	for (vector<string>::iterator i = V.begin(); i != V.end(); i++) {
		s += (*i);
	}
	proText = s;
	char eof = EOF;
	proText += eof; //将EOF加入程序string中
	// 读取长度
	len = s.length();
}

Lexical::~Lexical() {
	V.clear();
	TokSet.clear();
}

string Lexical::getPro() {
	return proText;
}

void Lexical::showPro() {
	for (vector<string>::iterator i = V.begin(); i != V.end(); i++) cout << *i;
}

string Lexical::tokToName(TokenType tok,string s) {
	string reversed = "Reversed Words ";
	string normalID = "ID,Name = ";
	string other = " ";
	string e="Error ";
	string num = "NUM ";
	string eof = "ENDFILE";
	if (tok == ID) {
		for (int i = 0; i < 6; i++) {
			if (s == reversedWords[i].tokName) return reversed;
		}
		return normalID;
	}
	else if (tok == ERROR) {
		return e;
	}
	else if (tok == NUM) {
		return num;
	}
	else if (tok == ENDFILE) {
		return eof;
	}
	else {
		return "";
	}
}

TokenSet Lexical::getToken() {
	string name;
	TokenType tok = ID;
	State state = START;
	bool save;
	while (state != DONE) {
		char c = proText[index];
		save = true;
		switch (state)
		{
		case START:
			if (isalpha(c)) {
				state = INLETTER;
			}
			else if (isdigit(c)) {
				state = INNUM;
			}
			else if (c == '>') {
				state = INGE;
			}
			else if (c == '<') {
				state = INLE;
			}
			else if (c == '!') {
				state = INNE;
			}
			else if (c == '=') {
				state = INEQ;
			}
			else if (c == '\n' || c == '\t' || c == ' ') {
				if (c == '\n') line++; //说明换行了
				save = false;
			}
			else if (c == '/') {
				state = INLBUFFER;
			}
			else {
				state = DONE;
				switch (c) {
				case '+':
					tok = PLUS;
					break;
				case '-':
					tok = MINUS;
					break;
				case '*':
					tok = MUL;
					break;
				case '(':
					tok = LPAREN;
					break;
				case ')':
					tok = RPAREN;
					break;
				case '[':
					tok = LBRACKET;
					break;
				case ']':
					tok = RBRACKET;
					break;
				case '{':
					tok = LBRACE;
					break;
				case '}':
					tok = RBRACE;
					break;
				case ',':
					tok = COMMA;
					break;
				case ';':
					tok = SEMI;
					break;
				case EOF:
					save = false;
					tok = ENDFILE;
					break;
				default:
					tok = ERROR;
					break;
				}
			}
				break;

		case INLETTER:
			if (!isalpha(c)) {
				tok = ID;
				state = DONE;
				save = false;
				index--;
			}
			break;
		case INNUM:
			if (!isdigit(c)) {
				tok = NUM;
				state = DONE;
				save = false;
				index--;
			}
			break;

		case INEQ:
			if (c == '=') {
				tok = EQ;
				state = DONE;
			}
			else {
				tok = ASSIGN;
				save = false;
				state = DONE;
				index--;
			}
			break;

		case INLE:
			if (c == '=') {
				tok = LE;
				state = DONE;
			}
			else {
				tok = LT;
				save = false;
				state = DONE;
				index--;
			}
			break;
		case INGE:
			if (c == '=') {
				tok = GE;
				state = DONE;
			}
			else {
				tok = GT;
				state = DONE;
				save = false;
				index--;
			}
			break;

		case INNE:
			if (c == '=') {
				tok = NEQ;
				state = DONE;
			}
			else {
				tok = ERROR;
				state = DONE;
				save = false;
				index--;
			}
			break;

		case INLBUFFER:
			if (c == '*') {
				name = ""; // 说明是注释符，删去之前的 '/'
				state = INCOMMENT;
				save = false;
			}
			else if (c == EOF) {
				state = DONE;
				tok = ENDFILE;
			}
			else {
				state = DONE;
				tok = DIV;
			}
			break;

		case INCOMMENT:
			save = false;
			if (c == '*') {
				state = INRBUFFER;
			}
			else if (c == EOF) {
				state = DONE;
				tok = ENDFILE;
			}
			break;

		case INRBUFFER:
			save = false;
			if (c == '/') {
				state = START;
			}
			else if (c == '*') {
				state = INRBUFFER;
			}
			else if (c == EOF) {
				state = DONE;
				tok = ENDFILE;
			}
			else {
				state = INCOMMENT;
			}
			break;

		case DONE:
			break;

		default:
			cout << "ERROR IN: " << line << " line"<<endl;
			state = DONE;
			tok = ERROR;
			break;

		//end 
		}//switch end


		if (save) {
			name += c;
		}
		index++;


	}

	//end while
	string x = tokToName(tok, name);//得到tok的字符串形式
	if(tok==ID) tok = idCase(name); //当tok类型为ID时，检查其是否为保留字
	currentTok = {
		tok,x,name,line
	};

	return currentTok;

}

void Lexical::doLexical() {
	while (index <= len) {
		TokSet.push_back(getToken());
		//index--;
	}
}

void Lexical::showResult() {
	for (vector<TokenSet>::iterator i = TokSet.begin(); i != TokSet.end(); i++) {
		cout << (*i).line << "\t" << (*i).tokenType << " " << (*i).tokenName << endl;
	}
}

vector<TokenSet> Lexical::getTokenSet() {
	return TokSet;
}

TokenType Lexical::idCase(string s) {
	for (int i = 0; i < 6; i++) {
		if (s == reversedWords[i].tokName) return reversedWords[i].tok;
	}
	return TokenType::ID;
}