#ifndef PARAMETER_H
#define PARAMETER_H
#include <sstream>
#include <iostream>

class Parameter
{
private:
	std::string param;
	bool isConstant;


public:
	Parameter(std::string s, bool b = false) : param(s), isConstant(b) {}

	std::string ToString()
	{
		std::ostringstream out;
		out << param;

		return out.str();
	}

	std::string RetrieveName()
	{
		return param;
	}

	bool checkConstant()
	{
		return isConstant;
	}


};

















#endif