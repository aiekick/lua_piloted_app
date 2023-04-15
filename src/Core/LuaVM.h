#pragma once

#include <luajit/src/lua.hpp>

class LuaVM
{
private:
	lua_State* m_LuaStatePtr = nullptr;

public:
	bool init();
	void unit();

private:
    lua_State* CreateLuaState();
    void DestroyLuaState(lua_State* vlua_State_ptr);
};
