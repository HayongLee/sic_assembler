#include "InstructionStruct.h"
#include <iostream>
#include <sstream>

int InstructionStruct::currentAddress = 0;
int InstructionStruct::startAddress = 0;
string InstructionStruct::sourceName = "0";
list<int> InstructionStruct::sizeInst = {};

InstructionStruct::InstructionStruct(string* words, map<string, operation *> optable)
{
	if (words[3] != "") { is_comment = true; comment = words[3]; } // when read line is a comment.
	else {
		this->label = words[0];
		this->op = words[1];
		this->opcode = optable[words[1]]->getOpcode();
		this->instruction_format = optable[words[1]]->getFormat();
		this->operand = words[2];
	}
}

void InstructionStruct::calAddress()
{
	if (this->opcode == 0xFA) //start
	{
		InstructionStruct::sourceName = this->label;
		InstructionStruct::startAddress = stoi(this->operand, nullptr, 16);
		InstructionStruct::currentAddress = InstructionStruct::startAddress;
	}
	else if (this->is_comment) {} // if it is comment pass this procedure
	else if (find(begin(sizeInst), end(sizeInst), this->opcode) == end(sizeInst))
	{
		this->address = this->currentAddress;
		this->currentAddress += 0x3;
	}
	else
	{
		if (this->opcode == 0xFB) //resb
		{
			this->address = this->currentAddress;
			this->currentAddress += 0x1 * stoi(this->operand);
		}
		else if (this->opcode == 0xFC) //resw
		{
			this->address = this->currentAddress;
			this->currentAddress += 0x3 * stoi(this->operand);
		}
		else if (this->opcode == 0xFD) //word
		{
			this->address = this->currentAddress;
			this->currentAddress += 0x3;
		}
		else // byte
		{
			this->address = this->currentAddress;
			
			// c''
			if (this->operand.find("c") != string::npos) 
			{
				string temp = this->operand.substr(this->operand.find("'")+1, this->operand.size() - this->operand.find("'")-2);
				this->currentAddress += this->operand.substr(this->operand.find("'"), 
							this->operand.size() - this->operand.find("'")).size() - 2; // 2 byte per letter
				
				// string to hex
				ostringstream tmp_str;
				string result = "";
				for (int i = 0; i < temp.size(); i++) {
					int tmp_int = (char)temp[i];
					tmp_str << std::hex << tmp_int;
					result += tmp_str.str();
					tmp_str.str("");
				}

				this->operand = result; // save result(string -> int (hex) -> string)
				
			}
			// x''
			else if (this->operand.find("x") != string::npos)
			{
				string temp = this->operand.substr(this->operand.find("'")+1, 
								this->operand.size() - this->operand.find("'")-2);
				this->currentAddress += temp.size() / 2; // a byte per letter
				this->operand = temp;
			}
		}
	}
}
void InstructionStruct::initSizeInst()
{
	InstructionStruct::sizeInst.push_back(0xFB); // RESB
	InstructionStruct::sizeInst.push_back(0xFC); // RESW
	InstructionStruct::sizeInst.push_back(0xFD); // WORD
	InstructionStruct::sizeInst.push_back(0xFE); // BYTE
}


std::ostream& operator<<(std::ostream &strm, const InstructionStruct &a) {
	return strm << "label=" << a.label << endl << "opcode="<<a.opcode<< endl<<"operand="<<a.operand<<endl<<"address="<<hex<<a.address<<endl;
}

InstructionStruct::~InstructionStruct()
{
}
