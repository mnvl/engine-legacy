
local imported = {}

local make_package_stub = function(name)
	local stub = {}
	local stub_meta = {
		__index = function(_, index)
			error(string.format("member `%s' is accessed before package `%s' is fully imported", index, name))
		end,
		__newindex = function(_, index, _)
			error(string.format("member `%s' is assigned a value before package `%s' is fully imported", index, name))
		end,
	}
	setmetatable(stub, stub_meta)
	return stub
end

local get_module_name = function(path)
	local name

	for str in string.gmatch(path, "(%w+).lua$") do
		if name then error(string.format("multiple candidates for module name in path `%s'", path)) end
		name = str
	end

	if not name then error(string.format("could not get module name from path `%s'", path)) end

	return name
end

return function(path)
	local name = get_module_name(path)

	do
		local package = imported[name]
		if package then return package end
	end

	local chunk = assert(loadfile(path), string.format("could not locate package `%s'", name))

	imported[name] = make_package_stub(name)

	local package = chunk()
	imported[name] = package

	return package
end
