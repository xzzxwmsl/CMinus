#include "pch.h"
#include "Output.h"
#include<fstream>

Output::Output(Lexical lexical)
{
	ofstream Out;
	Out.open("out.txt");
	for (vector<TokenSet>::iterator i = lexical.TokSet.begin(); i != lexical.TokSet.end(); i++) {
		Out << (*i).line << "\t" << (*i).tokenType << " " << (*i).tokenName << endl;
	}
}


Output::~Output()
{
}
