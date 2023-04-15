local vast = vast_module.new();

vast:setup({
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

vast:init()
vast:start()

vast:print_config(10)
