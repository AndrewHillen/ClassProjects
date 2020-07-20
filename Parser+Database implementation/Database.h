#ifndef DATABASE_H
#define DATABASE_H
#include <map>
#include "Relation.h"


class Database
{
private:
	std::map<std::string, Relation> relations;



public:
	Database() {};


	void InsertRelation(std::string name, Relation newRelation)
	{
		relations.insert(std::pair<std::string, Relation>(name, newRelation));
	}

	void FactTuples(std::string name, Tuple tuple)
	{
		if (relations.find(name) != relations.end())
		{
			relations.find(name)->second.AddTuple(tuple);
		}
	}

	std::map<std::string, Relation> RetrieveRelations()
	{
		return relations;
	}

	void ReplaceRelations(std::map<std::string, Relation> r)
	{
		relations = r;
	}

};



#endif
