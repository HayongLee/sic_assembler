#pragma once
#include <string>
#include <map>

using namespace std;

class operation {
private:
	string name;
	int opCode;
	int instFormat;
public:
	operation(int opcode = 0, string name = "", int instFormat = 0);

	string getName() { return this->name; }
	int getOpcode() { return this->opCode; }
	int getFormat() { return this->instFormat; }
};