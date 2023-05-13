#include "Headers.h"
#include "XMLNode.h"
#include "Application.h"
#include "ModuleXML.h"

XMLNode XMLNode::FindChildBreadth(std::string name, uint count)
{
	XMLNode ret = *this;
	uint currentCount = 0;
	// First search. This will only look into the current node children, and won't look into the next sibling's children.
	pugi::xml_node n;

	// Check if this is the node we are looking for
	std::string nodeName = node.name();

	if (nodeName == name) 
		return ret;

	std::queue<pugi::xml_node> firstQueue;

	for (n = node.first_child(); n; n = n.next_sibling())
	{
		if (n.first_child()) firstQueue.push(n.first_child());

		std::string nodeName = n.name();
		if (nodeName == name)
		{
			if (count > currentCount)
				currentCount++;
			else
			{
				ret.node = n;
				return ret;
			}
		}
	}

	return FindChildBreadthRecursive(firstQueue, name, count, currentCount);
}

XMLNode XMLNode::FindChildDepth(std::string name)
{
	XMLNode ret = *this;

	bool foundTarget = false;

	// Check if this is the node we are looking for
	std::string nodeName = node.name();

	if (nodeName == name) return ret;

	// Check children using recursive function
	ret = FindChildDepthRecursive(node, name, foundTarget);

	if (!foundTarget) LOG("XMLNode ERROR: The child of name %s was not found.", name.c_str());

	return ret;

}

XMLNode XMLNode::FindChildBreadthRecursive(std::queue<pugi::xml_node> q, std::string name, uint count, uint currentCount)
{
	std::queue<pugi::xml_node> nextQueue; // the queue data structure that is going to be passed to the next function.

	XMLNode ret = *this; // return value. Initialized as a copy of itself

	pugi::xml_node n;

	while (!q.empty())
	{
		n = q.front(); // Get first element of the queue
		q.pop();
		if (n.first_child()) nextQueue.push(n.first_child()); // Save first child for the next recursive function.

		for (n; n; n = n.next_sibling())
		{
			std::string nodeName = n.name();
			if (nodeName == name) // Check every child of the current node 
			{
				if (currentCount++ == count)
				{
					ret.node = n; // If it is the correct child, return the XMLNode changing the current node.
					return ret;
				}
			}
			if (n.first_child()) nextQueue.push(n.first_child()); // Save first child for the next recursive function.
		}
	}

	if (nextQueue.empty()) // If the next queue is empty and we didn't return any child, log an error and return a copy of the first node.
	{
		LOG("XMLNode ERROR: The child of name %s was not found. Count: %d of %d", name.c_str(), currentCount, count);
		return ret;
	}

	return FindChildBreadthRecursive(nextQueue, name, count, currentCount);	// If the queue is not empty, we look into the next layer of children
}

XMLNode XMLNode::FindChildDepthRecursive(pugi::xml_node node, std::string name, bool& foundTarget)
{
	XMLNode ret = *this;

	pugi::xml_node n;

	// Check every child
	for (n = node.first_child(); n; n = n.next_sibling())
	{
		std::string nodeName = n.name();
		if (nodeName == name)
		{
			ret.node = n;
			foundTarget = true;
			return ret;
		}
		// If this child has a child, check it.
		if (n.first_child())
		{
			ret = FindChildDepthRecursive(n, name, foundTarget);
			if (foundTarget) return ret;
		}
	}

	return ret;
}

void XMLNode::Save(const std::string& extension)
{
	Application::Instance()->xml->Save(xmlFile, extension);
}