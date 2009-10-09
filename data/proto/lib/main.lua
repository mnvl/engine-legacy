
local window
local renderer

local init = function()
	window = platform.Window()
	window:create("defence")

	renderer = graphic.Renderer(window)
end

local get_window = function()
	return window
end

local get_renderer = function()
	return renderer
end

return {
	init = init,
	get_window = get_window,
	get_renderer = get_renderer,
}
