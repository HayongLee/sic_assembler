#pragma once

#include "OpTable.h"
#include "SymTab.h"

#include <string>
#include <list>

using namespace std;

class InstructionStruct
{
public:
	InstructionStruct(string* words, map<string, operation *> optable);
	void calAddress();
	static void initSizeInst();

	string label;

	string op;
	int opcode;

	string operand;
	
	int instruction_format;
	int address;

	bool is_comment = false;
	string comment;

	static int startAddress;
	static int currentAddress;
	static string sourceName;
	static list<int> sizeInst;

	~InstructionStruct();
};

std::ostream& operator<<(std::ostream &strm, const InstructionStruct &a);
