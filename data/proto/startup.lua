
local window = platform.Window()
window:create("defence")

local renderer = graphic.Renderer(window)

local model = graphic.StaticModel(renderer)

local loader = graphic.ColladaLoader("pyramide4.dae", model, renderer)
loader:load()

model:set_animation_time(0)

local camera = graphic.Camera()

camera:set_perspective_projection(3.14/2, 4/3, 1, 1000)

local x = 0

while window:run_message_loop() do
	renderer:begin_frame()

	camera:set_eye_lookat_up(math.vec3(5 * math.sin(x), 0, 5 * math.cos(x)), math.vec3(0, 0, 0), math.vec3(0, 1, 0))

	renderer:set_view_projection_from_camera(camera)
	model:draw()

	renderer:finish_frame()

	x = x + 0.001
end
