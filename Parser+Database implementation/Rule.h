#ifndef RULE_H
#define RULE_H
#include "Predicate.h"
class Rule_
{
private:
	Predicate head;
	std::vector<Predicate> preds;



public:
	Rule_(Predicate p, std::vector<Predicate> v = {}) : head(p), preds(v) {}


	std::string ToString()
	{
		std::ostringstream out;

		out << head.ToString() << " :- ";
		out << preds.at(0).ToString();
		for (unsigned int i = 1; i < preds.size(); i++)
		{
			out << "," << preds.at(i).ToString();
		}

		return out.str();
	}

	Predicate GetHead()
	{
		return head;
	}

	std::vector<Predicate> GetPreds()
	{
		return preds;
	}

};















#endif