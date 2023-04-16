#pragma once

#include <col/sol.hpp>

class LuaNode
{
private:
	sol::table m_NodeTable;
	
public:
	virtual ~LuaNode() = default;
	virtual buildNode(const sol::table vTable) = 0;
};