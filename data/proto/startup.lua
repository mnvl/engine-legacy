
local window = platform.Window()
window:create("defence")

local renderer = graphic.Renderer(window)

local model = graphic.StaticModel(renderer)
local loader = graphic.ColladaLoader("pyramide4.dae", model, renderer)
loader:load()

while window:run_message_loop() do
	renderer:begin_frame()

	renderer:finish_frame()
end
