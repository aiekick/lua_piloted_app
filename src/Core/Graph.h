#pragma once

#include <set>
#include <string>
#include <memory>
#include <abstract/LuaNode.h>

class Graph
{
private:
	std::set<std::shared_ptr<LuaNode>> m_LuaNodes;

public:
	static std::shared_ptr<Graph> create() {
		return std::make_shared<Graph>();
	}

public:
	void addNode(std::shared_ptr<LuaNode> vLuaNodePtr);

	void clear();
};