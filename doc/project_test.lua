package.path = "C:\\Gamedev\\lua\\lua_piloted_app\\doc\\?.lua;" .. package.path
--print(package.path)

local pilot = require("pilot_module")

pilot:setup({
	dataProvider={
		class=FileDataProvider,
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
    test={
        class=ScreenText,
        [function(obj,data)
            obj:setText("State = "..tostring(data))
        end]="dataProvider.event",
    },
})

pilot:init()
pilot:start()

pilot:print_config(10)
