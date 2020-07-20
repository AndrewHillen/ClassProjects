#ifndef NODE_H
#define NODE_H
#include <set>
#include <sstream>
#include <string>

typedef int NodeID;
class Node_
{
private:
	unsigned int postOrder = 0;
	std::set<NodeID> adjNodes;
	bool visited;



public:
	Node_(bool b = false) : visited(b) {};

	void Insert(NodeID n)
	{
		adjNodes.insert(n);
	}

	bool AlreadyVisited()
	{
		return visited;
	}

	void VisitNode()
	{
		visited = true;
	}
	void SetPostOrder(unsigned int n)
	{
		postOrder = n;
	}
	unsigned int GetPostOrder()
	{
		return postOrder;
	}

	std::set<NodeID> GetAdjList()
	{
		return adjNodes;
	}

	std::string ToString()
	{
		std::ostringstream out;
		std::set<NodeID>::iterator it = adjNodes.begin();
		unsigned int size = adjNodes.size();
		unsigned int count = 0;
		while (it != adjNodes.end())
		{
			count++;
			out << "R" << *it++;
			if (count < size)
			{
				out << ",";
			}
		}

		return out.str();
	}


};


#endif
