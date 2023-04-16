#include "LuaVastModule.h"

#include <sol/types.hpp>
#include <ctools/cTools.h>

void LuaVastModule::create_lua_vast_module(sol::state& v_state)
{
    // it seems, there is no method to ensure vState is valid..
    
    // chargement du module lua vast
    v_state.new_usertype<LuaVastModule>("vast_module",
        "setup", &LuaVastModule::setup,
        "set_init_hook", &LuaVastModule::set_init_hook,
        "init", &LuaVastModule::init,
        "set_start_hook", &LuaVastModule::set_start_hook,
        "start", &LuaVastModule::start,
        "set_step_hook", &LuaVastModule::set_step_hook,
        "step", &LuaVastModule::step,
        "set_stop_hook", &LuaVastModule::set_stop_hook,
        "stop", &LuaVastModule::stop,
        "print_project", &LuaVastModule::print_project);
}

void LuaVastModule::setup(sol::table v_project)
{
    m_project_table = v_project;
}

void LuaVastModule::set_init_hook(sol::function v_init_hook_function)
{

}

void LuaVastModule::init()
{

}

void LuaVastModule::set_start_hook(sol::function v_start_hook_function)
{

}

void LuaVastModule::start()
{

}

void LuaVastModule::set_step_hook(sol::function v_stop_hook_function)
{

}

void LuaVastModule::step()
{

}

void LuaVastModule::set_stop_hook(sol::function v_stop_hook_function)
{

}

void LuaVastModule::stop()
{

}

void LuaVastModule::print_project(const uint32_t& v_depth)
{
    try
    {
        recurs_print_table(m_project_table, v_depth);
    }
    catch (const sol::error& e)
    {
        std::cout << "setup err : " << e.what() << std::endl;
    }
}

/////////////////////////////////////////////////////////////
//// PRIVATE ////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

void LuaVastModule::recurs_print_table(sol::table& v_table, const uint32_t& v_depth, const uint32_t& v_space_count)
{
    auto current_space_string = std::string(v_space_count, ' ');

    if (v_depth == 0)
    {
        std::cout << current_space_string << "..." << std::endl;
        return;
    }

    if (v_space_count == 0)
    {
        std::cout << " " << std::endl;
    }
    
    for (auto& item : v_table)
    {
        auto& key = item.first;
        auto& value = item.second;

        const auto& key_type = key.get_type();
        const auto& value_type = value.get_type();

        if (key_type == sol::type::number ||
            key_type == sol::type::string)
        {
            std::string key_as_string;
            if (key_type == sol::type::string)
            {
                key_as_string = key.as<std::string>();
            }
            else
            {
                key_as_string = ct::toStr(key.as<double>());
            }

            if (value_type == sol::type::table)
            {
                sol::table value_as_table = value.as<sol::table>();
                if (!value_as_table.empty())
                {
                    std::cout << current_space_string << "[\"" << key_as_string << "\"] = {" << std::endl;

                    recurs_print_table(value_as_table, v_depth - 1U, v_space_count + 4U);

                    std::cout << current_space_string << "}," << std::endl;
                }
                else
                {
                    std::cout << current_space_string << " = {}," << std::endl;
                }
            }
            else
            {
                std::string value_as_string;

                if (value_type == sol::type::string)
                {
                    value_as_string = value.as<std::string>();
                }
                else if (value_type == sol::type::number)
                {
                    value_as_string = ct::toStr(value.as<double>());
                }
                else if (value_type == sol::type::boolean)
                {
                    value_as_string = ct::toStr(value.as<bool>());
                }

                auto value_string = 
                    (value.get_type() == sol::type::string) ? 
                        ct::toStr("\"%s\"", value_as_string.c_str()) : 
                        value_as_string;

                std::cout << current_space_string << "[\"" << key_as_string << "\"] = " << value_string << "," << std::endl;
            }
        }
        else if (key_type == sol::type::function)
        {
            std::string value_as_string = value.as<std::string>();
            std::cout << current_space_string << "function = " << value_as_string << "," << std::endl;
        }
        else
        {
            CTOOL_DEBUG_BREAK;
        }
    }

    if (v_space_count == 0)
    {
        std::cout << " " << std::endl;
    }
}

void LuaVastModule::recurs_load_project(sol::table& v_table, const uint32_t& v_depth)
{
    if (v_depth == 0)
    {
        return;
    }

    // on va iterer le 1er niveai
    // comme les tables sont des hash map
    // l'ordre affiché n'est pas l'ordre parsé
    // on ne tombera pas directement sur la key class
    // du coup on va :
    // soaver la sol::table
    // iterer jusqu'a tomber sur la cle class
    // creer le node du bon type avec le factory et lui passer en param la sol::table
    for (auto& item : v_table)
    {
        auto& key = item.first;
        auto& value = item.second;

        const auto& key_type = key.get_type();
        const auto& value_type = value.get_type();

        if (key_type == sol::type::string &&
            value_type == sol::type::table)
        {
            sol::table value_as_table = value.as<sol::table>();
            if (!value_as_table.empty())
            {
                for (auto& members : value_as_table)
                {
                    auto& member_key = item.first;
                    auto& member_value = item.second;

                    const auto& member_key_type = key.get_type();
                    const auto& member_value_type = value.get_type();

                    if (member_key_type == sol::type::string)
                    {
                        auto key_as_string = member_key.as<std::string>();
                        if (key_as_string == "class")
                        {
                            if (member_value_type == sol::type::string)
                            {
                                auto value_as_string = value.as<std::string>();

                                //NodeFactory::create_node(value_as_string, value_as_table);
                            }
                        }
                    }
                }
            }
        }
    }
}
