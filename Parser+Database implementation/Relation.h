#ifndef RELATION_H
#define RELATION_H
#include <set>
#include "Tuple.h"
#include "Scheme.h"



class Relation
{
private:
	std::string name;
	Scheme scheme;
	std::set<Tuple> tuples;



public:
	Relation(std::string n, Scheme s) : name(n), scheme(s) {};

	void AddTuple(Tuple tuple)
	{
		tuples.insert(tuple);
	}

	Relation Select(std::string parameter, unsigned int idx)
	{
		Relation newRelation(name, scheme);
		for (Tuple tup : tuples)
		{
			if (tup.at(idx) == parameter)
			{
				newRelation.AddTuple(tup);
			}
		}
		return newRelation;
	}
	Relation Select(unsigned int idx, unsigned int idx2)
	{
		Relation newRelation(name, scheme);
		for (Tuple tup : tuples)
		{
			if (tup.at(idx) == tup.at(idx2))
			{
				newRelation.AddTuple(tup);
			}
		}
		return newRelation;
	}

	Relation Project(std::vector<unsigned int> indexes)
	{
		Relation newRelation(name, scheme);
		for (Tuple tup : tuples)
		{
			Tuple newTuple;
			for (unsigned int i = 0; i < indexes.size(); i++)
			{
				newTuple.push_back(tup.at(indexes.at(i)));
			}
			newRelation.AddTuple(newTuple);
		}
		return newRelation;
	}

	Relation Rename(std::vector<std::string> names)
	{
		Scheme s;
		for (unsigned int i = 0; i < names.size(); i++)
		{
			s.AddName(names.at(i));
		}
		Relation newRelation(name, s);

		for (Tuple tup : tuples)
		{
			newRelation.AddTuple(tup);
		}
		return newRelation;
	}

	Relation Join(Relation r2)
	{
		std::vector<std::string> r1Names = scheme.GetNames();
		std::vector<std::string> newNames = scheme.GetNames();
		std::vector<std::string> r2Names = r2.GetScheme().GetNames();
		std::vector<std::pair<unsigned int, unsigned int>> indexMatches;
		std::set<std::string> r1NameSet;
		std::vector<unsigned int> otherIndexes;

		for (unsigned int i = 0; i < r1Names.size(); i++)
		{
			r1NameSet.insert(r1Names.at(i));
		}
		
		
		for (unsigned int i = 0; i < r1Names.size(); i++)
		{
			for (unsigned int j = 0; j < r2Names.size(); j++)
			{
				if (r1Names.at(i) == r2Names.at(j))
				{
					indexMatches.push_back(std::pair<unsigned int, unsigned int>(i, j));
				}
				
				if (r1NameSet.insert(r2Names.at(j)).second)
				{
					newNames.push_back(r2Names.at(j));
					otherIndexes.push_back(j);
				}
			}
		}


		std::set<Tuple> r2Tuples = r2.GetTuples();
		std::set<Tuple> newTuples;
		

		for(Tuple tup1 : tuples)
		{
			for (Tuple tup2 : r2Tuples)
			{
				bool canJoin = true;

				for (unsigned int i = 0; i < indexMatches.size(); i++)
				{
					if (tup1.at(indexMatches.at(i).first) != tup2.at(indexMatches.at(i).second))
					{
						canJoin = false;
						break;
					}
				}
				if (canJoin)
					{
						newTuples.insert(TupleJoin(tup1, tup2, otherIndexes));
					}

			}
		}

		Scheme s;
		for (unsigned int i = 0; i < newNames.size(); i++)
		{
			s.AddName(newNames.at(i));
		}

		Relation newRelation(name, s);
		
		for (Tuple tup : newTuples)
		{
			newRelation.AddTuple(tup);
		}

		return newRelation;
	}
	
	
	
	Tuple TupleJoin(Tuple tup1, Tuple tup2, std::vector<unsigned int> indexes)
	{
		for (unsigned int i = 0; i < indexes.size(); i++)
		{
			tup1.push_back(tup2.at(indexes.at(i)));
		}
		return tup1;
	}

	Scheme GetScheme()
	{
		return scheme;
	}

	std::string GetName()
	{
		return name;
	}

	void ReplaceName(std::string s)
	{
		name = s;
	}

	std::set<Tuple> GetTuples()
	{
		return tuples;
	}

	int TupleCount()
	{
		return tuples.size();
	}

	std::string ToString()
	{
		std::ostringstream out;
		std::vector<std::string> names = scheme.GetNames();
		for (Tuple tup : tuples)
		{
			if(names.size() == 0) return "";
			out << "  " << names.at(0) << "=" << tup.at(0);
			
			for (unsigned int i = 1; i < names.size(); i++)
			{
				out << ", " << names.at(i) << "=" << tup.at(i);
			}
			out << std::endl;
		}
		return out.str();
	}


};




#endif
