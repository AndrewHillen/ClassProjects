#ifndef INTERPERATOR_H
#define INTERPERATOR_H
#include "DatalogProgram.h"
#include "Database.h"
#include "Graph.h"


class Interperator
{
private:
	DatalogProgram dProgram;
	Database myDatabase;
	Graph_ myGraph;

public:
	Interperator(DatalogProgram d) : dProgram(d) {};

	void PopulateSchemes()
	{
		std::vector<Predicate> schemes = dProgram.RetrieveSchemes();

		for (unsigned int i = 0; i < schemes.size(); i++)
		{
			std::string name = schemes.at(i).RetrieveName();
			//std::cout << name << std::endl;
			std::vector<Parameter> theScheme = schemes.at(i).JustParams();
			Scheme newScheme;
			for (unsigned int j = 0; j < theScheme.size(); j++)
			{
				std::string paramName = theScheme.at(j).RetrieveName();
				newScheme.AddName(paramName);
				//std::cout << paramName << ", ";
			}
			//std::cout << std::endl;
			Relation newRelation(name, newScheme);

			myDatabase.InsertRelation(name, newRelation);
		}

	}

	void FactTuples()
	{
		std::vector<Predicate> facts = dProgram.RetrieveFacts();


		for (unsigned int i = 0; i < facts.size(); i++)
		{
			std::string name = facts.at(i).RetrieveName();
			//std::cout << name << std::endl;
			std::vector<Parameter> tupleScheme = facts.at(i).JustParams();
			Tuple newTuple;
			for (unsigned int j = 0; j < tupleScheme.size(); j++)
			{
				std::string paramName = tupleScheme.at(j).RetrieveName();
				newTuple.push_back(paramName);
				//std::cout << paramName << ", ";
			}
			//std::cout << std::endl;
			
			myDatabase.FactTuples(name, newTuple);

		}
	}
	void BuildGraph(std::vector<Rule_> rules)
	{
		std::map<NodeID, Node_> tempMap;
		for (unsigned int i = 0; i < rules.size(); i++)
		{
			Predicate headPred = rules.at(i).GetHead();
			std::vector<Predicate> childPreds = rules.at(i).GetPreds();
			for (unsigned int j = 0; j < childPreds.size(); j++)
			{
				for (unsigned int k = 0; k < rules.size(); k++)
				{
					Predicate subHeadPred = rules.at(k).GetHead();
					if (childPreds.at(j).RetrieveName() == subHeadPred.RetrieveName())
					{
						if (tempMap.find(i) != tempMap.end())
						{
							tempMap.find(i)->second.Insert(k);
						}
						else
						{
							Node_ tempNode;
							tempNode.Insert(k);
							tempMap.insert(std::pair<NodeID, Node_>(i, tempNode));
						}
					}
				}
			}
			if (tempMap.find(i) == tempMap.end())
			{
				Node_ tempNode;
				tempMap.insert(std::pair<NodeID, Node_>(i, tempNode));
			}
		}

		std::cout << "Dependency Graph\n";
		for (auto it : tempMap)
		{
			std::cout << "R" << it.first << ": ";
			std::cout << it.second.ToString() << "\n";
			myGraph.Insert(it.first, it.second);
		}
		std::cout << "\n";


	}
	std::vector<DFSTree> BuildForest()
	{
		std::map<NodeID, Node_> reverseMap = myGraph.GetReverse();
		Graph_ reverseGraph;
		for (auto mItem : reverseMap)
		{
			reverseGraph.Insert(mItem.first, mItem.second);
		}
		reverseGraph.RunDFS();
		std::vector<NodeID> reversePostOrder = reverseGraph.ReversePostOrderVector();

		std::vector<DFSTree> forest = myGraph.GetSCCs(reversePostOrder);


		return forest;
	}

	void EvaluateRules()
	{
		std::vector<Rule_> rules = dProgram.RetrieveRules();
		BuildGraph(rules);
		std::vector<DFSTree> forest = BuildForest();
		bool tuplesAdded = false;
		std::cout << "Rule Evaluation\n";

		for (unsigned int z = 0; z < forest.size(); z++)
		{
			tuplesAdded = false;
			int count = 0;
			DFSTree tree = forest.at(z);
            tree.ReOrderNodes();
			std::vector<NodeID> nodes = tree.GetVector();
			std::cout << "SCC: ";
			for (unsigned int sccIndex = 0; sccIndex < nodes.size(); sccIndex++)
			{
				std::cout << "R" << nodes.at(sccIndex);
				if (sccIndex < nodes.size() - 1)
				{
					std::cout << ",";
				}
			}
			std::cout << "\n";


			do
			{
				tuplesAdded = false;
				for (unsigned int i = 0; i < nodes.size(); i++)
				{
					unsigned int ruleIndex = nodes.at(i);
					std::cout << rules.at(ruleIndex).ToString() << ".\n";

					std::map<std::string, Relation> relations = myDatabase.RetrieveRelations();
					Predicate headPred = rules.at(ruleIndex).GetHead();
					Relation headRelation = relations.find(headPred.RetrieveName())->second;
					std::vector<std::string> renameNames = headRelation.GetScheme().GetNames();
					std::vector<Parameter> headParams = headPred.JustParams();
					std::vector<std::string> headScheme;


					for (unsigned int j = 0; j < headParams.size(); j++)
					{
						std::string headParamName = headParams.at(j).RetrieveName();
						headScheme.push_back(headParamName);
					}

					std::map<std::string, unsigned int> nameIndexes;

					std::vector<Predicate> preds = rules.at(ruleIndex).GetPreds();
					if (preds.size() > 0)
					{
						std::string tempName = preds.at(0).RetrieveName();
						std::vector<Parameter> tempParam = preds.at(0).JustParams();
						std::vector<std::string> tempScheme;
						std::vector<std::pair<std::string, unsigned int>> tempStringIndexes;

						// String handler **************************************************************************************
						for (unsigned int j = 0; j < tempParam.size(); j++)
						{
							std::string tempParamName = tempParam.at(j).RetrieveName();
							if (tempParam.at(j).checkConstant())
							{
								tempStringIndexes.push_back(std::pair<std::string, unsigned int>(tempParamName, j));
							}
							tempScheme.push_back(tempParamName);
						}

						Relation tempRelation = relations.find(tempName)->second;
						for (unsigned int j = 0; j < tempStringIndexes.size(); j++)
						{
							tempRelation = tempRelation.Select(tempStringIndexes.at(j).first, tempStringIndexes.at(j).second);

						}
						tempRelation = tempRelation.Rename(tempScheme);


						// Join happens in this loop******************************************************************************************************

						for (unsigned int k = 1; k < preds.size(); k++)
						{
							std::string name = preds.at(k).RetrieveName();
							std::vector<Parameter> theScheme = preds.at(k).JustParams();
							std::vector<std::string> newScheme;
							std::vector<std::pair<std::string, unsigned int>> stringIndexes;

							for (unsigned int j = 0; j < theScheme.size(); j++)
							{
								std::string paramName = theScheme.at(j).RetrieveName();
								if (theScheme.at(j).checkConstant())
								{
									stringIndexes.push_back(std::pair<std::string, unsigned int>(paramName, j));
								}
								newScheme.push_back(paramName);
							}
							Relation newRelation = relations.find(name)->second;
							newRelation = newRelation.Rename(newScheme);


							for (unsigned int j = 0; j < stringIndexes.size(); j++)
							{
								newRelation = newRelation.Select(stringIndexes.at(j).first, stringIndexes.at(j).second);

							}
							tempRelation = tempRelation.Join(newRelation);
						}


						std::vector<std::string> allNames = tempRelation.GetScheme().GetNames();
						for (unsigned int j = 0; j < allNames.size(); j++)
						{
							nameIndexes.insert(std::pair<std::string, unsigned int>(allNames.at(j), j));
						}
						std::vector<unsigned int> projIndexes;

						for (unsigned int j = 0; j < headScheme.size(); j++)
						{
							if (nameIndexes.find(headScheme.at(j)) != nameIndexes.end())
							{
								projIndexes.push_back(nameIndexes.find(headScheme.at(j))->second);
							}
						}

						tempRelation = tempRelation.Project(projIndexes);
						tempRelation = tempRelation.Rename(renameNames);
						tempRelation.ReplaceName(headRelation.GetName());

						if (RuleUnion(relations, tempRelation))
						{
							tuplesAdded = true;
						}
						if (!tree.CheckFixed())
						{
							tuplesAdded = false;
						}
						//rename to rule head scheme in database
					}
					//Union here?
				}
				count++;
			} while (tuplesAdded);
			

			std::cout << count << " passes: ";
			for (unsigned int sccIndex = 0; sccIndex < nodes.size(); sccIndex++)
			{
				std::cout << "R" << nodes.at(sccIndex);
				if (sccIndex < nodes.size() - 1)
				{
					std::cout << ",";
				}
			}
			std::cout << "\n";
		}
		std::cout << "\n";
	}
	bool RuleUnion(std::map<std::string, Relation> &relations, Relation newRelation)
	{
		std::string name = newRelation.GetName();
		std::set<Tuple> newTuples = newRelation.GetTuples();
		std::set<Tuple> oldRelationTuples = relations.find(name)->second.GetTuples();
		std::vector<std::string> names = newRelation.GetScheme().GetNames();
		bool tuplesAdded = false;

		for (Tuple tup : newTuples)
		{
			if (oldRelationTuples.insert(tup).second)
			{

				if (names.size() == 0) std::cout << "";

				else
				{
					std::cout << "  " << names.at(0) << "=" << tup.at(0);

					for (unsigned int i = 1; i < names.size(); i++)
					{
						std::cout << ", " << names.at(i) << "=" << tup.at(i);
					}
					std::cout << std::endl;
				}
				myDatabase.FactTuples(name, tup);
				//output tuple here
				tuplesAdded = true;
			}
		}

		return tuplesAdded;
	}

	void AllQueries()
	{
		std::vector<Predicate> queries = dProgram.RetrieveQueries();
		std::vector<Relation> relations;
		std::cout << "Query Evaluation\n";

		for (unsigned int i = 0; i < queries.size(); i++)
		{
			relations.push_back(OneQuery(queries.at(i)));
		}

		for (unsigned int i = 0; i < relations.size(); i++)
		{
			std::cout << queries.at(i).ToString() << "? " ;
			if (relations.at(i).TupleCount() == 0)
			{
				std::cout << "No\n";
			}
			else
			{
				std::cout << "Yes(" << relations.at(i).TupleCount() << ")\n";
				std::cout << relations.at(i).ToString();
			}
		}
	}

	Relation OneQuery(Predicate query)
	{
		std::map<std::string, Relation> allRelations = myDatabase.RetrieveRelations();
		std::map<std::string, unsigned int> nameIndexes;
		std::string name = query.RetrieveName();
		std::vector<Parameter> params = query.JustParams();
		Relation newRelation = allRelations.find(name)->second;
		std::vector<std::string> renameNames;
		std::vector<unsigned int> projIndexes;

		for (unsigned int i = 0; i < params.size(); i++)
		{
			std::string paramName = params.at(i).RetrieveName();
			if (params.at(i).checkConstant())
			{
				newRelation = newRelation.Select(paramName, i);
			}
			else
			{
				if (nameIndexes.find(paramName) != nameIndexes.end())
				{
					newRelation = newRelation.Select(nameIndexes.find(paramName)->second, i);
				}
//If statement here will hopefully preserve order.
				if(nameIndexes.insert(std::pair<std::string, unsigned int>(paramName, i)).second)
				{
					renameNames.push_back(paramName);
					projIndexes.push_back(i);
				}
			}
		}
/*
		for (std::pair<std::string, unsigned int>obj : nameIndexes)
		{
			renameNames.push_back(obj.first);
			projIndexes.push_back(obj.second);
		}*/

		newRelation = newRelation.Project(projIndexes);
		newRelation = newRelation.Rename(renameNames);

		return newRelation;
	}


};



#endif
