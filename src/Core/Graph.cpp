#include "Graph.h"

void Graph::addNode(std::shared_ptr<LuaNode> vLuaNodePtr) {
	if (vLuaNodePtr) {
		m_LuaNodes.emplace(vLuaNodePtr);
	}
}

void Graph::clear() {
	m_LuaNodes.clear();
}