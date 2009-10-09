
local Main = import "lib/main.lua"

local load_collada = function(model, filename)
	local loader = graphic.ColladaLoader("pyramide4.dae", model, Main.get_renderer())
	loader:load()

	model:set_animation_time(0)
end

local load_static = function(filename)
	local model = graphic.StaticModel(Main.get_renderer())
	load_collada(model, filename)
	return model
end

return {
	load_static = load_static
}
