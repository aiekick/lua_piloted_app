#include "LuaVM.h"
#include "LuaVastModule.h"
#include <ctools/FileHelper.h>

///////////////////////////////////////////////
//// INIT / UNIT //////////////////////////////
///////////////////////////////////////////////

bool LuaVM::init()
{
    try 
    {
        // load lua standard libs
        m_Lua.open_libraries(sol::lib::base);
        m_Lua.open_libraries(sol::lib::package);
        m_Lua.open_libraries(sol::lib::coroutine);
        m_Lua.open_libraries(sol::lib::string);
        m_Lua.open_libraries(sol::lib::os);
        m_Lua.open_libraries(sol::lib::math);
        m_Lua.open_libraries(sol::lib::table);
        m_Lua.open_libraries(sol::lib::debug);
        m_Lua.open_libraries(sol::lib::bit32);
        m_Lua.open_libraries(sol::lib::io);
        m_Lua.open_libraries(sol::lib::ffi);
        m_Lua.open_libraries(sol::lib::jit);

        // load lua vast module
        LuaVastModule::create_lua_vast_module(m_Lua);

        // test with custom projet structure
        auto result_test = m_Lua.safe_script(u8R"(
vast:setup({
	dataProvider={
		class="FileDataProvider",
		port=1254,
		ip="0.152.12.48",
        format = {
            event={
                name="state",
                type="int",
                count=1,
                default_value=0,
            },
        },
	},
    converterTest={
        class="ConverterTest",
        eventTest="dataProvider.event"
    },
    screenText={
        class="ScreenText",
        [function(obj,data)
            obj:setText("State = "..tostring(data))
        end]="converterTest.eventTest",
    },
})

vast:init()
vast:start()

vast:print_project(10)
)");
        if (result_test.valid())
        {
            return true;
        }
    }
    catch (const sol::error& e) 
    {
        std::cout << "lua err : " << e.what() << std::endl;

        return false;
    }

	return false;
}

void LuaVM::unit()
{
    
}

///////////////////////////////////////////////
//// PRIVATE //////////////////////////////////
///////////////////////////////////////////////
