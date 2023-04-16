#pragma once

#include <cstdint>
#include <sol/sol.hpp>

class LuaVastModule
{
private:
    sol::table m_project_table;

public:
    static void create_lua_vast_module(sol::state& v_state);

public:
    void setup(sol::table v_project);

    void set_init_hook(sol::function v_init_hook_function);
    void init();

    void set_start_hook(sol::function v_start_hook_function);
    void start();

    void set_step_hook(sol::function v_stop_hook_function);
    void step();

    void set_stop_hook(sol::function v_stop_hook_function);
    void stop();

    void print_project(const uint32_t& v_depth);

private:

    /// <summary>
    /// will print the table in recursif
    /// </summary>
    /// <param name="vTable">the table to print</param>
    /// <param name="vDepth">the max depth for avoid infinite recursion</param>
    /// <param name="vSpaceCount">the sapce count for print indents</param>
    void recurs_print_table(sol::table& v_table, const uint32_t& v_depth = 10U, const uint32_t& v_space_count = 0U);

    /// <summary>
    /// will load the project
    /// </summary>
    /// <param name="vTable">the table to print</param>
    /// <param name="vDepth">the max depth for avoid infinite recursion</param>
    void recurs_load_project(sol::table& v_table, const uint32_t& v_depth = 10U);
};