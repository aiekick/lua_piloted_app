#pragma once

#include <sol/sol.hpp>

class LuaVM
{
private:
	sol::state m_Lua;

public:
	bool init();
	void unit();

private:
	void create_lua_vast_module();
};
