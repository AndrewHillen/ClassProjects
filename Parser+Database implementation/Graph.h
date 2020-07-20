#ifndef GRAPH_H
#define GRAPH_H
#include <map>
#include "Node_.h"
#include "DFSTree.h"

class Graph_
{
private:
	std::map<NodeID, Node_> myGraph;
	unsigned int postOrder = 1;


public:
	Graph_() {};

	void Insert(NodeID id, Node_ n)
	{
		myGraph.insert(std::pair<NodeID, Node_>(id, n));
	}
	std::vector<NodeID> ReversePostOrderVector()
	{
		std::vector<NodeID> reversePostOrder;
		for (unsigned int i = 0; i < myGraph.size(); i++)
		{
			NodeID temp = 0;
			reversePostOrder.push_back(temp);
		}
		for (auto mItem : myGraph)
		{
			unsigned int index = myGraph.size() - mItem.second.GetPostOrder();
			reversePostOrder[index] = mItem.first;
		}
		return reversePostOrder;
	}

	void RunDFS()
	{
		for (auto mItem : myGraph)
		{
			DFSTree dummyTree;
			DFS(mItem.first, myGraph, dummyTree);
		}
	}
	std::vector<DFSTree> GetSCCs(std::vector<NodeID> reverseNodes)
	{
		std::vector<DFSTree> forest;
		for (unsigned int i = 0; i < reverseNodes.size(); i++)
		{
			DFSTree tempTree;
			DFS(reverseNodes.at(i), myGraph, tempTree);
			if (tempTree.size() > 0)
			{
				forest.push_back(tempTree);
			}
		}
		return forest;
	}

	void DFS(NodeID id, std::map<NodeID, Node_> &graph, DFSTree &tree)
	{
		if (graph.find(id)->second.AlreadyVisited())
		{
			return;
		}
		graph.find(id)->second.VisitNode();
		std::set<NodeID> adjNodes = graph.find(id)->second.GetAdjList();
		tree.InsertNode(id);
		for (auto sItem : adjNodes)
		{
			std::vector<NodeID> componentVector = tree.GetVector();
			for (unsigned int i = 0; i < componentVector.size(); i++)
			{
				if (componentVector.at(i) == sItem)
				{
					tree.DoFixedPoint();
				}
			}
			if (myGraph.find(sItem) == myGraph.end())
			{
				return;
			}
			if (!graph.find(sItem)->second.AlreadyVisited())
			{
				DFS(graph.find(sItem)->first, graph, tree);
			}
		}
		graph.find(id)->second.SetPostOrder(postOrder);
		postOrder++;
		return;

	}

	std::map<NodeID, Node_> GetReverse()
	{
		std::map<NodeID, Node_> reverseGraph;
		for (auto mItem : myGraph)
		{
			std::set<NodeID> adjNodes = mItem.second.GetAdjList();
			for (auto sItem : adjNodes)
			{
				if (reverseGraph.find(sItem) != reverseGraph.end())
				{
					reverseGraph.find(sItem)->second.Insert(mItem.first);
				}
				else
				{
					Node_ tempNode;
					tempNode.Insert(mItem.first);
					reverseGraph.insert(std::pair<NodeID, Node_>(sItem, tempNode));

				}
			}
			if (reverseGraph.find(mItem.first) == reverseGraph.end())
			{
				Node_ tempNode;
				reverseGraph.insert(std::pair<NodeID, Node_>(mItem.first, tempNode));
			}
		}
		return reverseGraph;
	}


};



#endif
