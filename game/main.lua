
main = {
    window = nil,
    renderer = nil,
    camera = nil,
}

function main:new()
    o = {}
    setmetatable(o, self)
    self.__index = self
    return o
end

function main:run()
    self.window = platform.window("engine")
    self.renderer = graphic.renderer(self.window)
    self.camera = graphic.camera()

    self.camera:set_perspective_projection(1.0, 4/3, 1.0, 1000.0)
    self.camera:set_position(math.vec3f(10, 1, 0), math.vec3f(0, 1, 0), math.vec3f(0, 1, 0))

    ground = graphic.static_model(self.renderer)
    ground:load_from_collada("ground.dae")
    ground:set_animation_time(0)

    while self.window:run_message_loop() do
	self.renderer:set_view_projection_from_camera(self.camera)
	self.renderer:begin_frame()
	ground:draw()
	self.renderer:finish_frame()
    end
end

dofile("game_object.lua")
main:new():run()
