
local model = graphic.StaticModel(Main.get_renderer())
local loader = graphic.ColladaLoader("pyramide4.dae", model, Main.get_renderer())
loader:load()