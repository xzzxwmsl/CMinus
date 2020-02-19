#include "pch.h"
#include "SCAN.h"
#include<vector>
#include<fstream>
#include<string>
#include<cstring>

using namespace std;

SCAN::SCAN()
{
	//Location = "C:\\Users\\Яђе§жа\\Desktop\\pro.txt";
	Location = "pro.txt";
	V.clear();
}


SCAN::~SCAN()
{
	V.clear();
}

void SCAN::getLocation() {
	cout << "input loction\n";
    cin >> Location;
}

vector<string> SCAN::getText() {
	ifstream inFile;
	inFile.open(Location.data());
	if (inFile.is_open()) {
		string s;
		while (getline(inFile,s)) {
			s += "\n";
			V.push_back(s);
		}
	}
	else cout << "Error,NOT FOUND"<<endl;

	return V;
}
