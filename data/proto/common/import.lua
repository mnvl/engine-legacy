
local imported = {}

local make_stub = function(name)
	local stub = {}
	local meta = {
		__index = function(_, index)
			error(string.format("member `%s' is accessed before package `%s' is fully imported or package have no such member", index, name))
		end,
		__newindex = function(_, index, _)
			error(string.format("member `%s' is assigned a value before package `%s' is fully imported or package have no such member", index, name))
		end,
	}
	setmetatable(stub, meta)
	return stub
end

local get_name = function(path)
	for name in string.gmatch(path, "(%w+).lua$") do
		return name
	end

	error(string.format("could not get module name from path `%s'", path))
end

local import = function(path)
	local name = get_name(path)

	do
		local package = imported[name]
		if package then return package end
	end

	local chunk, error_message = loadfile(path)

	if not chunk then
		error(string.format("could not locate package `%s' from path `%s': %s", name, path, error_message))
    end

	local package = make_stub(name)
	imported[name] = package

	local exports = chunk()

	for k, v in pairs(exports) do
		rawset(package, k, v)
	end

	return package
end

declare('import', import)
