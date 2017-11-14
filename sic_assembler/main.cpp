#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>

#include "InstructionStruct.h"

using namespace std;
static void createOPTABLE();
static void createSYMTAB(list<InstructionStruct>::iterator iter);
static string* parseCode(string line);
static void makeCodeList(string* words);

map<string, operation*> OPTABLE;
map<string, symbol*> SYMTAB;
list<InstructionStruct> codes;

int main()
{
	// create OPTABLE
	createOPTABLE();

	string fileName;
	cin >> fileName;
	ifstream is(fileName);
	if (is.is_open())
	{
		while (!is.eof())
		{
			string line;
			getline(is, line);
			string* temp = parseCode(line);
			makeCodeList(temp);
			cout << line << endl;
		}
		InstructionStruct::initSizeInst();
		list<InstructionStruct>::iterator iter;
	
		// create symbol table.
		for (iter = codes.begin(); iter != codes.end(); iter++)
		{
			iter->calAddress();
			createSYMTAB(iter);
		}

		// print result.
		cout << "Label ------- Opcode -------- Operand -------- Address" << endl;
		for (iter = codes.begin(); iter != codes.end(); iter++)
		{
			if ((*iter).is_comment) { cout << "#"<<(*iter).comment<<"."<<endl; continue; }
			cout.flags(ios::left);
			cout << setw(14) << (*iter).label <<
					setw(2) << "0x" << ( (((*iter).opcode) < 0x10)? "0":"" ) << hex << uppercase << (*iter).opcode <<
					setw(13) << " ( " + (*iter).op + " ) " <<
					setw(17) << (*iter).operand <<
					setw(15) << hex << (*iter).address << endl;
		}
		// print symbol table
		cout << "----------------------SYMBOL TABLE --------------------" << endl;
		for (auto it = SYMTAB.cbegin(); it != SYMTAB.cend(); it++) {
			cout << setw(15) << it->second->getLabel() 
				 << setw(10) << it->second->getAddress() << endl;
		}
	}
	else
	{
		cout << "file does not exist"<<endl;
	}

}

static string* parseCode(string line)
{
	string* words = new string[4];
	int line_size = line.size();
	if (line.find(".") == line.size()-1)
	{
		for (int i = 0; i < 3; i++) words[i] = "";
		words[3] = line.substr(0, line.size() - 2);
		return words;
	}
	if (line.find(" ") == 0) // no label
	{
		words[0] = "";
		line = line.substr(9, line.size() - 9);
		if (line.find(" ") != string::npos)
		{
			words[1] = line.substr(0, line.find(" "));
			line = line.substr(8, line.size());
			words[2] = line;
		}
		else
		{
			words[1] = line;
			words[2] = "";
		}

		return words;
	}
	else // there is lable
	{
		words[0] = line.substr(0, line.find(" "));
		line = line.substr(9, line.size() - 9);
		if (line.find(" ") != string::npos)
		{
			words[1] = line.substr(0, line.find(" "));
			line = line.substr(8, line.size());
			words[2] = line;
		}
		else
		{
			words[1] = line;
			words[2] = "";
		}
	}
	return words;
}

// read a line and create information about it.
void makeCodeList(string* words)
{
	InstructionStruct newLine(words, OPTABLE); // read a line and create information
	codes.push_back(newLine);
}


// read instruction from a file and push into Operation Table.
static void createOPTABLE() {
	int opcode;
	string name;
	string format;

	ifstream optab("OPTABLE");
	if (optab.is_open()) {
		while (!optab.eof()) {
			optab >> hex >> opcode >> name >> format;
			OPTABLE[name] = new operation(opcode, name, atoi(format.c_str()));
		}
		optab.close();
	}
}

// create symbol table.
// tofind address and label information with label(string).
void createSYMTAB(list<InstructionStruct>::iterator iter)
{
	if ((*iter).label != "") {
		SYMTAB[(*iter).label] = new symbol((*iter).address, (*iter).label);
	}
}