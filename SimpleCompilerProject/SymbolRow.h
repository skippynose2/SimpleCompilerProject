#include <iostream>
#include <string>

using namespace std;

class SymbolRow {
public:
	//Accessor methods
	SymbolRow(string, string, int);
	string getSym();
	string getType();
	int getLoc();

	//mutator methods
	void SetSym(string);
	void SetType(string);
	void SetLoc(int);

private:
	string sym;
	string type;
	int loc;

};