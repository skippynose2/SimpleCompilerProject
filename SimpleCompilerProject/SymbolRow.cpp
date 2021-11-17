#include "SymbolRow.h";
#include <iostream>

SymbolRow::SymbolRow (string InputSym, string InputType, int InputLoc) {
	SetSym(InputSym);
	SetType(InputType);
	SetLoc(InputLoc);
}

// Writing the mutator methods
void SymbolRow::SetSym(string InputSym) {
	sym = InputSym;
}

void SymbolRow::SetType(string InputType) {
	type = InputType;
}

void SymbolRow::SetLoc(int InputLoc) {
	loc = InputLoc;
}

//Accessor methods
string SymbolRow::getSym() {
	return sym;
}

string SymbolRow::getType() {
	return type;
}

int SymbolRow::getLoc() {
	return loc;
}