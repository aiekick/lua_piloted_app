package.path = "C:\\Gamedev\\lua\\lua_piloted_app\\doc\\?.lua;" .. package.path
--print(package.path)

local pilot = require("pilot_module")

pilot:setup({
	dataProvider={
		class=UDPDataProvider,
		port=1254,
		ip="0.152.12.48",
        format = {
            sim_time={
                name="sim_time",
                type="double",
                default_value=0,
                count=1,
            },
            event={
                name="state",
                type="int",
                default_value=0,
                count=1,
            },
            positionTL={
                name="position",
                type="double",
                default_value=0,
                count=3,
            },
            attitude={
                name="attitude",
                type="double",
                default_value=0,
                count=3,
            },
        },
	},
    test={
    },
	ConvertTLtoGodot={
        class=ConvertTLtoGodot,
		time="dataObserver.sim_time",
		posTL="dataObserver.posattTL[1..3]",
		eulerTL="dataObserver.posattTL[4..6]",
		rotMatrice="R",
	},
	Commons={
		rocket_Model={
			class="Avatar",
			coord_system_name="Cartesian",
			coord_system_bind=function(data) end,
			pos="dataObserver.position",
			rot={0,0,0},
            [function(obj,data)
            end]="dataObserver.event",
			childs={
				etage1_Model={
					class="Avatar",
					pos={0,0,100}, -- relative to parent
					rot={0,0,0},
					model="scene_etage1.tscn",
                    [function(obj,data)
                    end]="dataObserver.event",
					childs={
						reactor1_Model={
							class="Avatar",
							pos={20,0,0},
							rot={0,0,0},
							model="scene_reactor.tscn",
                            [function(obj,data)
                            end]="dataObserver.event",
						},
						reactor2_Model={
							class="Avatar",
							pos={-20,0,0},
							rot={0,0,0},
							model="scene_reactor.tscn",
                            [function(obj,data)
                            end]="dataObserver.event",
						}
					}
				},
				etage2_Model={
					class="Avatar",
					pos={0,0,200}, -- relative to parent
					rot={0,0,0},
					model="scene_etage2.tscn",
                    [function(obj,data)
                    end]="dataObserver.event",
				},
				etage3_Model={
					class="Avatar",
					pos={0,0,300}, -- relative to parent
					rot={0,0,0},
					model="scene_etage3.tscn",
                    [function(obj,data)
                    end]="dataObserver.event",
				},
				satellite_Model={
					class="Avatar",
					pos={0,0,320}, -- relative to parent
					rot={0,0,0},
					model="scene_satellite_1.tscn",
                    [function(obj,data)
                    end]="dataObserver.event",
				},
			},
		},
	},
	Views={
		RocketView={
			class=View,
			pos_in_percent={50,0}, 
			size_in_pixel={200, 300},
			visible=true,
			childs={
				rocket_model={
					target="Commons.rocket_Model"
				},
				launchpad_model={
					class=Avatar,
					pos={0,0,0},
					wsg84={0,0,0}, -- Lattitude, Longitude, Altitude
					rot={0,0,0},
                    [function(obj,data)
                    end]="dataObserver.event",
					childs={
						cubemap_Ground_Close={
							class=CubeMap,
							picture="cubempa_ground_close.png",
						},
						cubemap_Ground_Far={
							class=CubeMap,
							picture="cubempa_ground_far.png",
						},
						launchpad_Mesh={
							class="Avatar",
							pos={0,0,0},
							rot={0,0,0},
							model="scene_launchpad.tscn",
                            [function(obj,data)
                            end]="dataObserver.event",
						},
					},
				},
				Earth_Model={
					class=Earth,
					highdef=true,
					sky_layers={
						--Clouds_Layer,
						Stellar_Layer={
							class=Stellar,
							visible=false,
						},
					},
				},
				widget_background={
					class=texture,
					pos={0,0},
					size={100,100},
					visible=true,
					level=foreground,
				}
			},
			Cameras={
				camera_rocket={
					class=CameraTrackball,
					target=rocket_Model,
					rot={0,0,0},
					up_vector={0,0,1},
					distance=100,
                    [function(obj,data)
                    end]="dataObserver.event",
				},
				camera_earth={
					class=CameraFixed,
					pos={0,-1e5,0},
					dir={0,1,0},
					target=Earth_Model,
                    [function(obj,data)
                    end]="dataObserver.event",
				},
				active=camera_launchpad,
                [function(obj,data)
                end]="dataObserver.event",
			},
			Widgets={
				Button_Quit={
					class=Button,
					pos_in_percent={50,0}, 
					size={100, 300}, --il faudrait comme en css pouvoir mettre du % ou du pixel
                    [function(button_id)
                    end]="on_mouse_pressed",
                    [function(button_id)
                    end]="on_mouse_released",
				},
			},
		},
		MinimapView={
			class=Minimap,
			projection=Merkator,
			pos_in_percent={0,0},
			size_in_percent={10, 10},
			visible=false,
			targets={
				{
					target="dataObserver.event",
					color={255,255,0},
				},
			},
		},
		GlobeView={
			class=View,
			pos_in_percent={10,0}, -- or pos_in_pixel
			size_in_pixel={200, 300},
			visible=false,
			childs={
				earth_light_model={
					class=Earth,
					highdef=false,
					sky_layers={
						Clouds_Layer,
					},
				},
				tracker_model={
					class=Tracker,
					primitve_type=line,
					thickness=3.0,
					targets={
						{
							target="ConvertTLtoGodot.posTL",
							color={255,255,0},
						},
						{
							target="ConvertTLtoGodot.posWDG84",
							color={255,0,255},
						},
					},
				},
			},
			Cameras={
				camera1={
					class=CameraFixed,
					pos={0,-1e5,0},
					dir={0,1,0},
					target=earth_light_model,
				},
				active=camera1,
			},
		},
		WidgetView={
			class=IHM,
			pos_in_pixel={50,0}, 
			size_in_pixel={200, 300},
			visible=true,
			Widgets={
				CheckBox_Infos={
					class=Button,
					pos_in_percent={50,0}, 
					size={100, 300}, --il faudrait comme en css pouvoir mettre du % ou du pixel
                    [function(button_id)
                    end]="on_mouse_pressed",
                    [function(button_id)
                    end]="on_mouse_released",
                    [function(key_id)
                    end]="on_key_pressed",
                    [function(key_id)
                    end]="on_key_released",
				},
				Infos_Text={
					class=TestBlock,
					size={100, 100},
					visible=false,
                    [function(obj, data)
                        if (data ~= nill) then
                            obj:setText("sim_time : "..tostring(data[0]))
                        end
                    end]="dataObserver.sim_time",
					on_update=function() end,
				},
				Button_Quit={
					class=Button,
					pos_in_percent={50,0}, 
					size={100, 300}, --il faudrait comme en css pouvoir mettre du % ou du pixel
                    [function(button_id)
                    end]="on_mouse_pressed",
                    [function(button_id)
                    end]="on_mouse_released",
				},
			},
		},
	},
})

function init_hook()
	local rocket_view = pilot:get_node("RocketView");
	if rocket_view ~= nil then
		-- rocket_view.setVisible(true);
		-- rocket_view.setActiveCamera("camera_rocket");
	end

	local reactor1_Model = pilot:get_node("RocketView.rocket_Model.etage1_Model.reactor1_Model");
	if reactor1_Model ~= nil then
		-- reactor1_Model.setEvent(121); -- allumage
		-- reactor1_Model.setVisible(true);
	end
end

function start_hook()
    
end

function step_hook()

end

pilot:set_init_hook(init_hook)
pilot:set_start_hook(start_hook)
pilot:set_step_hook(step_hook)

pilot:init()
pilot:start()

pilot:print_config(10)
