#include "LuaVM.h"
#include "LuaVastModule.h"
#include <ctools/FileHelper.h>

#include <nodes/DataProviders/FileDataProvider.h>
#include <nodes/DataConverters/DataConvertTest.h>

///////////////////////////////////////////////
//// INIT / UNIT //////////////////////////////
///////////////////////////////////////////////

bool LuaVM::init()
{
    bool res = true;

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

        // register nodes 
        m_NodeFactoryPtr = NodeFactory::create();
        if (m_NodeFactoryPtr)
        {
            m_NodeFactoryPtr->register_node<FileDataProvider>("FileDataProvider");
            m_NodeFactoryPtr->register_node<DataConvertTest>("DataConvertTest");
        }
        else
        {
            return false;
        }

        // graph container
        m_GraphPtr = Graph::create();
        if (!m_GraphPtr)
        {
            return false;
        }

        // load lua vast module
        LuaVastModule::create_lua_vast_module(m_Lua, this);

        // test with custom projet structure
        auto result_test = m_Lua.safe_script(u8R"(
vast:setup({
	dataProvider={
		class="FileDataProvider",
        file="doc/log_sample.log",
        format = {
            sim_time={
                name="sim_time",
                type="double",
                count=1,
                default_value=0,
            },
            epoch_time={
                name="epoch_time",
                type="double",
                count=1,
                default_value=0,
            },
            render_time={
                name="render_time",
                type="double",
                count=1,
                default_value=0,
            },
        },
	},
    converterTest={
        class="DataConvertTest",
        render_time_test="dataProvider.render_time"
    },
    screenText={
        class="ScreenText",
        [function(obj,data)
            print("Render_time : "..tostring(data).." ms")
        end]="dataProvider.render_time",
        [function(obj,data)
            print("Render_time : "..tostring(data).." f/s")
        end]="converterTest.render_time_test",
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
    if (m_NodeFactoryPtr)
    {
        m_NodeFactoryPtr->clear();
        m_NodeFactoryPtr.reset();
    }

    if (m_GraphPtr)
    {
        m_GraphPtr->clear();
        m_GraphPtr.reset();
    }
}

///////////////////////////////////////////////
//// PRIVATE //////////////////////////////////
///////////////////////////////////////////////
