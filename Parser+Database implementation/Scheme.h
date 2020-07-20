#ifndef SCHEME_H
#define SCHEME_H

#include <vector>
#include <iostream>
#include <set>

class Scheme
{
private:

	std::vector<std::string> names;

public:
	Scheme() {};

	void AddName(std::string s)
	{
		names.push_back(s);
	}


	std::vector<std::string> GetNames()
	{
		return names;
	}

};







#endif