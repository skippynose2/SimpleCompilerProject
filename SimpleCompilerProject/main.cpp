#include <iostream>
#include "SymbolRow.h";
#include <vector>
#include <fstream>
#include <iterator>
#include <sstream>
#include <stack>
using namespace std;


//Creating the 3 variables
int CurrentData = 99;
int CurrentPointer = 0;
int CurrentSYM = 0;


std::vector<std::string> tempCharHolder;
vector<string> tempCharHolderEqaution{};
// Vector to temp hold the SML code that will be eventually written to a flie
vector <int> tempSMLHolder;

// Vector holding the go to things
vector <int> UnResolvedSymbols;


//Creating the SymbolTable
vector <SymbolRow> SymbolTable;




//Functions that are needed to split string by whitespaces
template <typename Out>
void split(const std::string& s, char delim, Out result) {
	std::istringstream iss(s);
	std::string item;
	while (std::getline(iss, item, delim)) {
		*result++ = item;
	}
}

std::vector<std::string> split(const std::string& s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, std::back_inserter(elems));
	return elems;
}




// Converting infix to postfix old way of doing it
// -------------------------------------------------------------------------
//Function to return precedence of operators
int prec(char c) {
	if (c == '^')
		return 3;
	else if (c == '/' || c == '*')
		return 2;
	else if (c == '+' || c == '-')
		return 1;
	else
		return -1;
}

// The main function to convert infix expression
//to postfix expression
string infixToPostfix(string s) {

	stack<char> st; //For stack operations, we are using C++ built in stack
	string result;

	for (int i = 0; i < s.length(); i++) {
		char c = s[i];

		// If the scanned character is
		// an operand, add it to output string.
		if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
			result = result + c + " ";

		// If the scanned character is an
		// ‘(‘, push it to the stack.
		else if (c == '(')
			st.push('(');

		// If the scanned character is an ‘)’,
		// pop and to output string from the stack
		// until an ‘(‘ is encountered.
		else if (c == ')') {
			while (st.top() != '(')
			{
				result = result + st.top() + " ";
				st.pop();
			}
			st.pop();
		}

		//If an operator is scanned
		else {
			while (!st.empty() && prec(s[i]) <= prec(st.top())) {
				result = result + st.top() + " ";
				st.pop();
			}
			st.push(c);
		}
	}

	// Pop all the remaining elements from the stack
	while (!st.empty()) {
		result = result + st.top() + " ";
		st.pop();
	}
	return result;
}

// End of converting infix to post fix
// ---------------------------------------------------------------------------------






void SymTbl(string sym, string type, int loc) {
	//array of reserve words rem, input print it does not need to be in the table
	// array of acceptable charaters a-z all lower case
	// put the letter in for the sym and put type as "L" and loc = CurrentData
	//Add the symbol to the symbol tables
	// "20" or "x"
	SymbolTable.push_back(SymbolRow(sym, type, loc));
}



int Generate_SML_Command(int OpCode, int location) {

	string s1 = to_string(OpCode);
	string s2 = to_string(location);


	if (location < 10) {
		if (location == -1) {
			s2 = "";
		}
		else {
			s2 = "0" + to_string(location);
		}
	}


	string SML = s1 + s2;

	int code = stoi(SML);

	return code;
}


int Generate_SML_Command_For_GoTo(int OpCode, string location, vector<SymbolRow> row) {
	string opLocation;
	// First find location in table and store into a variable
	for (int i = 0; i < SymbolTable.size(); i++) {
		if (row[i].getSym() == location) {
			opLocation = to_string(row[i].getLoc());
		}
	}

	// turn op location into string and if less than 10 concate it with 0
	if (stoi(opLocation) < 10) {
		opLocation = "0" + opLocation;
	}
	
	string s1 = to_string(OpCode);

	string SML = s1 + opLocation;

	int code = stoi(SML);

	return code;

}


bool SymInTable(vector<SymbolRow> row, string sym) {
	for (int i = 0; i < SymbolTable.size(); i++) {
		if (row[i].getSym() == sym) {
			return true;
		}
	}
	return false;
}

int FindSymLoc(vector<SymbolRow> row, string sym) {
	for (int i = 0; i < SymbolTable.size(); i++) {
		if (row[i].getSym() == sym) {
			return row[i].getLoc();
		}
	}
}


int BranchesWithNoAddress(vector <int> tempSMLHolder) {
	int key1 = 40;
	int key2 = 41;
	int key3 = 42;

	bool found = false;

	for (int i = 0; i < tempSMLHolder.size(); i++)
	{
		if ((tempSMLHolder[i] == key1) || (tempSMLHolder[i] == key2) || (tempSMLHolder[i] == key3))
		{
			std::cout << "Element present at index " << i << endl;
			found = true;
			return i;
			break;
		}
	}
}

void SecondPass(vector<int> UnResolvedSymbols, vector<int> &tempSMLHolder, vector <SymbolRow> row) {

	for (int i = 0; i < UnResolvedSymbols.size(); i++) {
		if (UnResolvedSymbols[i] != -1) {
			// Then start the process of looking for the first thing that is just a 40, 41, 42 etc
			// store that
			int index = BranchesWithNoAddress(tempSMLHolder);
			cout << "Location at index " << index << endl;

			// Then look for the value of UnResolvedSymbols in row/SymbolRow vector and get its location
			int location = FindSymLoc(SymbolTable, to_string(UnResolvedSymbols[i]));
			cout << location << endl;

			//Add location to the SML and push to the tempSMLHolder
			tempSMLHolder[index] = Generate_SML_Command(tempSMLHolder[index], location);
			cout << "The command generated " << Generate_SML_Command(tempSMLHolder[index], location) << endl;
			cout << "We running but nothing happening" << endl;
		}
	}
}



void Handle_Goto(string line, string gotoNum, vector<int> tempSMLGenerator, vector<SymbolRow> symbolTable, int BranchCommand) {

	if (stoi(gotoNum) > stoi(line)) {
		cout << "hi there" << endl;
		tempSMLHolder.push_back(Generate_SML_Command(BranchCommand, -1));
		UnResolvedSymbols.push_back(stoi(gotoNum));
		SymTbl(line, "L", -1);
		CurrentSYM += 1;
	}
	else {
		// Implement it to deal with things that are above, this first cause easier??
		tempSMLHolder.push_back(Generate_SML_Command_For_GoTo(BranchCommand, gotoNum, SymbolTable));
	}
}


bool Not_Operator(string command) {
	bool found = true;

	// Expand the list later
	const int size = 5;
	string legalVar[size] = { "*", "+", "-", "/" };
	for (int i = 0; i < size; i++) {
		if (legalVar[i] == command) {
			found = false;
			return found;
		}
	}

	return found;
}

bool NotVar(string command) {
	bool found = true;

	// Expand the list later
	const int size = 5;
	string legalVar[size] = { "x", "y", "z", "b", "a"};
	for (int i = 0; i < size; i++) {
		if (legalVar[i] == command) {
			found = false;
			return found;
		}
	}

	return found;
}

void If_Statement_Generator(string line_num, string var1, string relop, string var2, string gotoLine, vector<int> &tempSMLHolder, vector<SymbolRow> SymbolTable) {
	// If statement for ==
	if (relop == "==") {
		cout << "Relop cout" << endl;

		if ((SymInTable(SymbolTable, var1) == true) && (SymInTable(SymbolTable, var2) == true)) {
			tempSMLHolder.push_back(Generate_SML_Command(20, FindSymLoc(SymbolTable, var1)));
			CurrentPointer += 1;
			tempSMLHolder.push_back(Generate_SML_Command(31, FindSymLoc(SymbolTable, var2)));
			CurrentPointer += 1;
			Handle_Goto(line_num, gotoLine, tempSMLHolder, SymbolTable, 42);
			CurrentPointer += 1;
		}
		else {
			cout << "Error, variable does not exist" << endl;
		}
	}

	// If statement for <
	if (relop == "<") {
		if ((SymInTable(SymbolTable, var1) == true) && (SymInTable(SymbolTable, var2) == true)) {
			tempSMLHolder.push_back(Generate_SML_Command(20, FindSymLoc(SymbolTable, var1)));
			CurrentPointer += 1;
			tempSMLHolder.push_back(Generate_SML_Command(31, FindSymLoc(SymbolTable, var2)));
			CurrentPointer += 1;
			Handle_Goto(line_num, gotoLine, tempSMLHolder, SymbolTable, 41);
			CurrentPointer += 1;
		}
		else {
			cout << "Error, variable does not exist" << endl;
		}
	}

	// If statement for >
	if (relop == ">" || relop == ">=") {
		if ((SymInTable(SymbolTable, var1) == true) && (SymInTable(SymbolTable, var2) == true)) {
			tempSMLHolder.push_back(Generate_SML_Command(20, FindSymLoc(SymbolTable, var1)));
			CurrentPointer += 1;
			tempSMLHolder.push_back(Generate_SML_Command(31, FindSymLoc(SymbolTable, var2)));
			CurrentPointer += 1;
			Handle_Goto(line_num, gotoLine, tempSMLHolder, SymbolTable, 41);
			CurrentPointer += 1;
			Handle_Goto(line_num, gotoLine, tempSMLHolder, SymbolTable, 40);
			CurrentPointer += 1;
			Handle_Goto(line_num, gotoLine, tempSMLHolder, SymbolTable, 42);
			CurrentPointer += 1;
		}
		else {
			cout << "Error, variable does not exist" << endl;
		}
	}

	// If statement for <=
	if (relop == "<=") {
		if ((SymInTable(SymbolTable, var1) == true) && (SymInTable(SymbolTable, var2) == true)) {
			tempSMLHolder.push_back(Generate_SML_Command(20, FindSymLoc(SymbolTable, var1)));
			CurrentPointer += 1;
			tempSMLHolder.push_back(Generate_SML_Command(31, FindSymLoc(SymbolTable, var2)));
			CurrentPointer += 1;
			Handle_Goto(line_num, gotoLine, tempSMLHolder, SymbolTable, 41);
			CurrentPointer += 1;
			Handle_Goto(line_num, gotoLine, tempSMLHolder, SymbolTable, 42);
			CurrentPointer += 1;
		}
		else {
			cout << "Error, variable does not exist" << endl;
		}
	}

	// If statment for !=
	if (relop == "!=") {
		if ((SymInTable(SymbolTable, var1) == true) && (SymInTable(SymbolTable, var2) == true)) {
			tempSMLHolder.push_back(Generate_SML_Command(20, FindSymLoc(SymbolTable, var1)));
			CurrentPointer += 1;
			tempSMLHolder.push_back(Generate_SML_Command(31, FindSymLoc(SymbolTable, var2)));
			CurrentPointer += 1;
			Handle_Goto(line_num, gotoLine, tempSMLHolder, SymbolTable, 41);
			CurrentPointer += 1;
			Handle_Goto(line_num, gotoLine, tempSMLHolder, SymbolTable, 40);
			CurrentPointer += 1;
		}
		else {
			cout << "Error, variable does not exist" << endl;
		}
	}
}


int Operator(string symbol) {
	if (symbol == "*") {
		return 0;
	}

	if (symbol == "/") {
		return 1;
	}

	if (symbol == "+") {
		return 2;
	}

	if (symbol == "-") {
		return 3;
	}

	return -1;

}


// Implementation of let equation handler
void EquationHandler(string equation, vector<int>& tempSMLHolder, vector<SymbolRow> SymbolTable, int memLocation) {


	string s = infixToPostfix(equation);

	string space_delimiter = " ";
	vector<string> EquationSymbol{};

	size_t pos = 0;
	while ((pos = s.find(space_delimiter)) != string::npos) {
		EquationSymbol.push_back(s.substr(0, pos));
		s.erase(0, pos + space_delimiter.length());
	}

	int stackpointer = CurrentData;
	for (int i = 0; i < EquationSymbol.size(); i++) {
		cout << EquationSymbol[i] << endl;
		//cout << equation << endl;
		// Check to see if it is a variable or number or operator and push on stack accordingly
		if (Not_Operator(EquationSymbol[i]) == false) {
			// pop stuff and perform operations
			cout << "evaluvating operator" << endl;
			switch (Operator(EquationSymbol[i]))
			{
			case 0:
				stackpointer += 1;
				tempSMLHolder.push_back(Generate_SML_Command(20, stackpointer));
				stackpointer += 1;
				tempSMLHolder.push_back(Generate_SML_Command(33, stackpointer));
				tempSMLHolder.push_back(Generate_SML_Command(21, stackpointer));
				stackpointer -= 1;
				break;
			case 1:
				stackpointer += 1;
				tempSMLHolder.push_back(Generate_SML_Command(20, stackpointer));
				stackpointer += 1;
				tempSMLHolder.push_back(Generate_SML_Command(32, stackpointer));
				tempSMLHolder.push_back(Generate_SML_Command(21, stackpointer));
				stackpointer -= 1;
				break;
			case 2:
				stackpointer += 1;
				tempSMLHolder.push_back(Generate_SML_Command(20, stackpointer));
				stackpointer += 1;
				tempSMLHolder.push_back(Generate_SML_Command(30, stackpointer));
				tempSMLHolder.push_back(Generate_SML_Command(21, stackpointer));
				stackpointer -= 1;
				break;
			case 3:
				stackpointer += 1;
				tempSMLHolder.push_back(Generate_SML_Command(20, stackpointer));
				stackpointer += 1;
				tempSMLHolder.push_back(Generate_SML_Command(31, stackpointer));
				tempSMLHolder.push_back(Generate_SML_Command(21, stackpointer));
				stackpointer -= 1;
				break;
			default:
				break;
			}
		}
		else {
			// If number push it on stack, meaning put it in the mem location where stackpointer is
			if (NotVar(EquationSymbol[i]) == true) {
				tempSMLHolder.push_back(Generate_SML_Command(22, stoi(EquationSymbol[i])));
				CurrentPointer += 1;
				tempSMLHolder.push_back(Generate_SML_Command(21, stackpointer));
				CurrentPointer += 1;
				CurrentData -= 1;
				stackpointer -= 1;
			}
			else {
				// If a variable, check if var is in symtbl, if so
				//then load the variable value into the current stack pointer
				tempSMLHolder.push_back(Generate_SML_Command(20, FindSymLoc(SymbolTable, EquationSymbol[i])));
				CurrentPointer += 1;
				tempSMLHolder.push_back(Generate_SML_Command(21, stackpointer));
				CurrentPointer += 1;
				CurrentData -= 1;
				stackpointer -= 1;
			}
		}
	}

	//Load value in whereever the stack pointer is located and put that value in mem of the original var of the let stmt
	tempSMLHolder.push_back(Generate_SML_Command(20, (stackpointer + 1)));
	CurrentPointer += 1;
	tempSMLHolder.push_back(Generate_SML_Command(21, memLocation));
	CurrentPointer += 1;
	
}





void SML_Generator(vector<string> Command) {
	// All start by looking at command [1] to see keyword
	// If key word rem, then look at Command[0] and add that to symbol table
	if (Command[1] == "rem") {
		SymTbl(Command[0], "L", CurrentPointer);
		CurrentSYM += 1;
		UnResolvedSymbols.push_back(-1);
	}

	// If key word input, then look at Command[0] and add that to symbol table with instruction pointer and SML
	// than look at Command [2] and add to symbol table
	if (Command[1] == "input") {
		// Make sure there is no other x or whatever var is in the symtbl
		if (!SymInTable(SymbolTable, Command[2])) {
			// Generate SML code and then add the line number and var to symtbl
			SymTbl(Command[0], "L", CurrentPointer);
			CurrentPointer += 1;
			CurrentSYM += 1;
			UnResolvedSymbols.push_back(-1);
			//Adding var to symtbl
			tempSMLHolder.push_back(Generate_SML_Command(10, CurrentData));
			SymTbl(Command[2], "V", CurrentData);
			CurrentData -= 1;
			CurrentSYM += 1;
			UnResolvedSymbols.push_back(-1);
		}
		else {
			cout << "Variable already declared" << endl;
		}
	}

	// If keyword print, look at at Command[0] and add that to instruction table with the updated instruction pointer and SML
	if (Command[1] == "print") {
		if (!SymInTable(SymbolTable, Command[2])) {
			cout << "Var you trying to get " << Command[2] << " does not exist" << endl;
		}
		else {
			// Find the symtbl row and generate SML
			tempSMLHolder.push_back(Generate_SML_Command(11, FindSymLoc(SymbolTable, Command[2])));
			SymTbl(Command[0], "L", CurrentPointer);
			CurrentPointer += 1;
			UnResolvedSymbols.push_back(-1);
		}
	}


	if (Command[1] == "goto") {

		cout << stoi(Command[2]) << endl;

		Handle_Goto(Command[0], Command[2], tempSMLHolder, SymbolTable, 40);
		SymTbl(Command[0], "L", -1);
		CurrentSYM += 1;
		CurrentData += 1;
	}

	if (Command[1] == "if") {
		if (NotVar(Command[4]) == true) {
			// Redoing handeling literals
			cout << "Literal Statement in here " << endl;
			tempSMLHolder.push_back(Generate_SML_Command(22, stoi(Command[4])));
			CurrentPointer += 1;
			tempSMLHolder.push_back(Generate_SML_Command(21, CurrentData));
			CurrentPointer += 1;
			SymTbl(Command[4], "C", CurrentData);
			CurrentData -= 1;
			CurrentSYM += 1;
			/*tempSMLHolder.push_back(stoi(Command[4]));
			CurrentPointer += 1;*/
		}
		cout << "If statement relop" << endl;
		If_Statement_Generator(Command[0], Command[2], Command[3], Command[4], Command[6], tempSMLHolder, SymbolTable);
	}

	if (Command[1] == "end") {
		tempSMLHolder.push_back(Generate_SML_Command(43, 00));
		SymTbl(Command[0], "L", CurrentPointer);
		CurrentPointer += 1;
		CurrentSYM += 1;
	}

	if (Command[1] == "let") {
		// Implement let statement here. Start with literals and work your way up


		cout << "-----------------------------------" << endl;


		//Check to see if variable actually exists 
		if (SymInTable(SymbolTable, Command[2]) == true) {
			EquationHandler(Command[4], tempSMLHolder, SymbolTable, FindSymLoc(SymbolTable, Command[2]));
		}
		else {
			// Genereate variable and put it in both symtbl and at current data location
			SymTbl(Command[2], "V", CurrentData);
			CurrentData -= 1;
			CurrentSYM += 1;
			UnResolvedSymbols.push_back(-1);
			EquationHandler(Command[4], tempSMLHolder, SymbolTable, FindSymLoc(SymbolTable, Command[2]));
		}
	}

	for (int z = 0; z < tempSMLHolder.size(); z++) {
		cout << tempSMLHolder[z] << endl;
	}

	cout << "---------------------------------------" << endl;

}


void SML_Writer(string command) {
	// Break up the command by whitespace
	tempCharHolder.clear();
	tempCharHolder = split(command, ' ');
	SML_Generator(tempCharHolder);
}


int main() {

	ifstream file("Text.txt");

	// Reads it line by line and returns the line pass this into SML code generator function, a function which has the line parser and table adder
	if (file.is_open()) {
		std::string line;
		while (std::getline(file, line)) {
			SML_Writer(line);
		}

		file.close();
	}



	for (int x = 0; x < UnResolvedSymbols.size(); x++) {
		cout << UnResolvedSymbols[x] << endl;
	}

	SecondPass(UnResolvedSymbols, tempSMLHolder, SymbolTable);

	cout << "---------------------------------------" << endl;

	for (int z = 0; z < tempSMLHolder.size(); z++) {
		cout << tempSMLHolder[z] << endl;
	}


	//Giant if statement we read between the white spaces
	//Read 20 and put in table
	//read input or command trigger if function write the SML code to output to SML file
	// read the actual symbol if there is another

	// We use what the above outputted throw into a switch case and go from there

	return 0;
}