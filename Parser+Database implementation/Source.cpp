#include <iostream>
#include <fstream>
#include "Parser.h"
#include "Scanner.h"
#include "Database.h"
#include "Interperator.h"

using namespace std;


int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		cerr << "Missing input file\n";
		return 1;
	}

	Scanner scanner(argv[1]);
	string b = scanner.tester();
	vector<Token> myTokens;

	myTokens = scanner.RetrieveVector();
	Parser myParser(myTokens);

	myParser.Begin();

	DatalogProgram dProgram = myParser.RetrieveDatalog();

	Interperator myInterperator(dProgram);
	myInterperator.PopulateSchemes();
	myInterperator.FactTuples();
	myInterperator.EvaluateRules();
	myInterperator.AllQueries();


}