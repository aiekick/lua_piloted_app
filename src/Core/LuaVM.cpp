#include "LuaVM.h"
#include <sol/sol.hpp>

///////////////////////////////////////////////
//// INIT / UNIT //////////////////////////////
///////////////////////////////////////////////

bool LuaVM::init()
{
    m_LuaStatePtr = CreateLuaState();
    if (m_LuaStatePtr)
    {
        return true;
    }
	return false;
}

void LuaVM::unit()
{
    if (m_LuaStatePtr)
    {
        DestroyLuaState(m_LuaStatePtr);
    }
}

///////////////////////////////////////////////
//// PRIVATE //////////////////////////////////
///////////////////////////////////////////////

lua_State* LuaVM::CreateLuaState()
{
    auto lua_state_ptr = luaL_newstate();
    if (lua_state_ptr)
    {
        luaJIT_setmode(lua_state_ptr, 0, LUAJIT_MODE_ENGINE | LUAJIT_MODE_ON);

        luaL_openlibs(lua_state_ptr); // lua access to basic libraries

        // register custom functions
        /*lua_register(lua_state_ptr, "print", lua_int_print_args);
        lua_register(lua_state_ptr, "LogInfo", Lua_void_LogInfo_string);
        lua_register(lua_state_ptr, "LogWarning", Lua_void_LogWarning_string);
        lua_register(lua_state_ptr, "LogError", Lua_void_LogError_string);
        lua_register(lua_state_ptr, "SetInfos", Lua_void_SetInfos_string);
        lua_register(lua_state_ptr, "SetRowBufferName", Lua_void_SetRowBufferName_string);
        lua_register(lua_state_ptr, "SetFunctionForEachRow", Lua_void_SetFunctionForEachRow_string);
        lua_register(lua_state_ptr, "SetFunctionForEndFile", Lua_void_SetFunctionForEndFile_string);
        lua_register(lua_state_ptr, "GetRowIndex", Lua_int_GetRowIndex_void);
        lua_register(lua_state_ptr, "GetRowCount", Lua_int_GetRowCount_void);
        lua_register(lua_state_ptr, "AddSignalValue", Lua_void_AddSignalValue_category_name_date_value);
        lua_register(lua_state_ptr, "AddSignalStartZone", Lua_void_AddSignalStartZone_category_name_date_string);
        lua_register(lua_state_ptr, "AddSignalEndZone", Lua_void_AddSignalEndZone_category_name_date_string);
        lua_register(lua_state_ptr, "AddSignalTag", Lua_void_AddSignalTag_date_color_name_help);
        lua_register(lua_state_ptr, "AddSignalStatus", Lua_void_AddSignalStatus_category_name_date_string);*/
    }

    return lua_state_ptr;
}

void LuaVM::DestroyLuaState(lua_State* vlua_state_ptr)
{
    lua_close(vlua_state_ptr);
    vlua_state_ptr = nullptr;
}