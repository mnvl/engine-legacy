
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
	for name in string.gmatch(path, "(%w+).lua$") do
		return name
	end

	error(string.format("could not get module name from path `%s'", path))
end

local import = function(path)
	local name = get_module_name(path)

	do
		local package = imported[name]
		if package then return package end
	end

	local chunk, error_message = loadfile(path)

	if not chunk then
		error(string.format("could not locate package `%s' from path `%s': %s", name, path, error_message))
    end

	imported[name] = make_package_stub(name)

	local package = chunk()
	imported[name] = package

	return package
end

declare('import', import)
