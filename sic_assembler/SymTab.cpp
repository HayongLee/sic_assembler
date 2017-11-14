#include "SymTab.h"

symbol::symbol(int address, string label) {
	this->address = address;
	this->label = label;
}

int symbol::getAddress() {
	return this->address;
}

string symbol::getLabel() {
	return this->label;
}