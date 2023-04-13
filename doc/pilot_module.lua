local pilot = {}
pilot.__index = pilot

function pilot:setup(config)
  self.config = config
end

function pilot:set_init_hook(init_hook)
  self.init_hook = init_hook
end

function pilot:set_start_hook(start_hook)
  self.start_hook = start_hook
end

function pilot:set_step_hook(step_hook)
  self.step_hook = step_hook
end

function pilot:init()
    self.init_hook()
end

function pilot:start()
    self.start_hook()
end

function pilot:stop()
    self.step_hook()
end

function pilot:get_node(node_path)
    return {}
end

function pilot:parse_config()
    print("Start parsing")
end

function pilot:recurs_print_table(tbl, depth, count)
    count = count or 0;
    depth = depth or 5;

    if (depth == 0) then
        print(string.rep(' ', count).."...");
        return;
    end

    if (count == 0) then
        print(" ");
    end

    for key, value in pairs(tbl) do
        if (key) then
			if (type(key) == "number" or type(key) == "string") then
				key = string.format("[\"%s\"]", key);

				if (type(value) == "table") then
					if (next(value)) then
						print(string.rep(' ', count)..key.." = {");
						self:recurs_print_table(value, depth - 1, count + 4);
						print(string.rep(' ', count).."},");
					else
						print(string.rep(' ', count)..key.." = {},");
					end
				else
					if (type(value) == "string") then
						value = string.format("\"%s\"", value);
					else
						value = tostring(value)
					end

					print(string.rep(' ', count)..key.." = "..value..",");
				end
			else
				if (type(key) == "function") then
					print(string.rep(' ', count).."function = "..value..",");
				end
			end
        end
    end

    if (count == 0) then
        print(" ");
    end
end

function pilot:print_config(depth)
    self:recurs_print_table(self.config, depth, nil)
end

return pilot