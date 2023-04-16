#pragma once

#include <sol/sol.hpp>
#include "NodeFactory.h"
#include "Graph.h"

class LuaVM
{
private:
	sol::state m_Lua;
	std::shared_ptr<NodeFactory> m_NodeFactoryPtr = nullptr;
	std::shared_ptr<Graph> m_GraphPtr = nullptr;

public:
	bool init();
	void unit();

	std::weak_ptr<NodeFactory> getNodeFactory() {
		return m_NodeFactoryPtr;
	}

	std::weak_ptr<Graph> getGraph() {
		return m_GraphPtr;
	}
};
