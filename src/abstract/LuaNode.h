#pragma once

#include <sol/sol.hpp>

class LuaNode
{
private:
	sol::table m_NodeTable;
	
public:
	virtual ~LuaNode() = default;
	virtual bool buildNode(const sol::table& vTable) = 0;
};