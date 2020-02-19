
//

#include "pch.h"
#include"Lexical.h"
#include"SCAN.h"
#include"Parse.h"
#include"Output.h"
#include<string>
#include<vector>
using namespace std;
int main()
{
	SCAN scan;
	Lexical lexical(scan.getText()); // 初始化词法分析器
	cout<<"X";
	lexical.doLexical();//进行词法分析
	cout<<"!"; 
    lexical.showPro();
	Parse parse(lexical.getTokenSet());
	parse.printTree(parse.parse());
	//parse.parse();
	
	//lexical.showResult();//展示结果

	//Output out(lexical); //将之输出到out.txt

}

