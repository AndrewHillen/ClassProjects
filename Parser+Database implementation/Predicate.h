#ifndef PREDICATE_H
#define PREDICATE_H
#include <vector>
#include "Parameter.h"

class Predicate
{
private:
	std::string pred;
	std::vector<Parameter> params;



public:
	Predicate(std::string s = "", std::vector<Parameter> v = {}) : pred(s), params(v) {}


	std::string ToString()
	{
		std::ostringstream out;

		out << pred << "(" << params.at(0).ToString();
		for (unsigned int i = 1; i < params.size(); i++)
		{
			out << "," << params.at(i).ToString();
		}
		out << ")";

		return out.str();
	}

	std::vector<Parameter> JustParams()
	{
		return params;
	}
	std::string RetrieveName()
	{
		return pred;
	}

};


#endif
