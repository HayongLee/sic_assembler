#pragma once
#include <string>
#include <map>

using namespace std;

class symbol {
private:
	int address;
	string label;
public:
	symbol(int, string);
	int getAddress();
	string getLabel();
};