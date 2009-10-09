
dofile("common/declare.lua")
dofile("common/import.lua")

local Model = import "lib/model.lua"
local Main = import "lib/main.lua"

Main.init()

local model = Model.load_static("pyramide4.lua")

local camera = graphic.Camera()

camera:set_perspective_projection(3.14/2, 4/3, 1, 1000)

local x = 0

while Main.get_window():run_message_loop() do
	Main.get_renderer():begin_frame()

	camera:set_eye_lookat_up(math.vec3(5 * math.sin(x), 0, 5 * math.cos(x)), math.vec3(0, 0, 0), math.vec3(0, 1, 0))

	Main.get_renderer():set_view_projection_from_camera(camera)
	model:draw()

	Main.get_renderer():finish_frame()

	x = x + 0.01
end
