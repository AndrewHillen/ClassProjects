#ifndef DFSTREE_H
#define DFSTREE_H
#include "Node_.h"
#include <vector>
#include <set>

class DFSTree
{
private:
	bool fixedPoint = false;
	std::vector<NodeID> components;
public:
	DFSTree() {};

	void InsertNode(NodeID n)
	{
		components.push_back(n);
	}
	void ReOrderNodes()
    {
        std::set<NodeID> reOrder;
        for(unsigned int i = 0; i < components.size(); i++)
        {
            reOrder.insert(components.at(i));
        }
        unsigned int count = 0;
        for(auto it : reOrder)
        {
            components.at(count) = it;
            count++;
        }
    }

	std::vector<NodeID> GetVector()
	{
		return components;
	}

	void DoFixedPoint()
	{
		fixedPoint = true;
	}

	bool CheckFixed()
	{
		return fixedPoint;
	}

	unsigned int size()
	{
		return components.size();
	}



};



#endif
