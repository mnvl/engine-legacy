
local load_collada = function(model, filename)
	local loader = graphic.ColladaLoader("pyramide4.dae", model, renderer)
	loader:load()

	model:set_animation_time(0)
end

local load_static = function(renderer, filename)
	local model = graphic.StaticModel(renderer)
	load_collada(model, filename)
	return model
end

return {
	load_static = load_static
}
