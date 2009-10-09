
local window = platform.Window()
window:create("defence")

local renderer = graphic.Renderer(window)

local model = graphic.StaticModel(renderer)

local loader = graphic.ColladaLoader("pyramide4.dae", model, renderer)
loader:load()

local camera = graphic.Camera()
camera:set_orthographic_projection(1.0, 1.0, 1.0, 1000.0)
camera:set_eye_lookat_up(math.vec3(10, 0, 0), math.vec3(0, 0, 0), math.vec3(0, 1, 0))

while window:run_message_loop() do
	renderer:begin_frame()

	renderer:set_view_projection_from_camera(camera)
	model:draw()

	renderer:finish_frame()
end
