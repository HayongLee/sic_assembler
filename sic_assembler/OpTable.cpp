#include "OpTable.h"

operation::operation(int opCode, string name, int instFormat){
	this->name = name;
	this->opCode = opCode;
	this->instFormat = instFormat;
}