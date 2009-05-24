
main = {
    window = nil,
    renderer = nil,
}

function main:new()
    o = {}
    setmetatable(o, self)
    self.__index = self
    return o
end

function main:init()
    self.window = platform.window("engine")
    self.renderer = graphic.renderer(self.window)
end

function main:run()
    while self.window:run_message_loop() do
        self.renderer:begin_frame()
        self.renderer:finish_frame()
    end
end

m = main:new()
m:init()
m:run()