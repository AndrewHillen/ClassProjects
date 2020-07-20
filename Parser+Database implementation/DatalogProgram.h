#ifndef DATALOGPROGRAM_H
#define DATALOGPROGRAM_H
#include "Rule.h"
#include <set>


class DatalogProgram
{
private:
	std::vector<Predicate> schemes;
	std::vector<Predicate> facts;
	std::vector<Rule_> rules;
	std::vector<Predicate> queries;

public:
	DatalogProgram(std::vector<Predicate> s = {}, std::vector<Predicate> f = {}, std::vector<Rule_> r = {}, std::vector<Predicate> q = {}) :
		schemes(s), facts(f), rules(r), queries(q) {}

	void SetSchemes(std::vector<Predicate> s)
	{
		schemes = s;
	}
	void SetFacts(std::vector<Predicate> f)
	{
		facts = f;
	}
	void SetRules(std::vector<Rule_> r)
	{
		rules = r;
	}
	void SetQueries(std::vector<Predicate> q)
	{
		queries = q;
	}

	std::vector<Predicate>RetrieveSchemes()
	{
		return schemes;
	}
	std::vector<Predicate>RetrieveFacts()
	{
		return facts;
	}
	std::vector<Predicate>RetrieveQueries()
	{
		return queries;
	}
	std::vector<Rule_>RetrieveRules()
	{
		return rules;
	}


	void ToString()
	{
		std::cout << "Success!\n";
		std::cout << "Schemes(" << schemes.size() << "):\n";
		for (unsigned int i = 0; i < schemes.size(); i++)
		{
			std::cout << "  " << schemes.at(i).ToString() << std::endl;
		}
		std::set<std::string> domains;
		std::vector<Parameter> paramTemp;
		std::cout << "Facts(" << facts.size() << "):\n";
		for (unsigned int i = 0; i < facts.size(); i++)
		{
			std::cout << "  " << facts.at(i).ToString() << ".\n";
			paramTemp = facts.at(i).JustParams();
			for (unsigned int j = 0; j < paramTemp.size(); j++)
			{
				domains.insert(paramTemp.at(j).ToString());
			}
		}

		std::cout << "Rules(" << rules.size() << "):\n";
		for (unsigned int i = 0; i < rules.size(); i++)
		{
			std::cout << "  " << rules.at(i).ToString() << ".\n";
		}

		std::cout << "Queries(" << queries.size() << "):\n";
		for (unsigned int i = 0; i < queries.size(); i++)
		{
			std::cout << "  " << queries.at(i).ToString() << "?\n";
		}

		std::cout << "Domain(" << domains.size() << "):\n";
		for (auto iter = domains.begin(); iter != domains.end(); iter++)
		{
			std::cout << "  " << *iter << "\n";
		}
	}



};




#endif
