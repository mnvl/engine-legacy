
local declared = {}

local declare = function(name, value)
	rawset(_G, name, value)
	declared[name] = true
end

_G.declare = declare

setmetatable(_G, {
	__newindex = function (t, n, v)
		if not declared[n] then
			error(string.format("attempt to write to undeclared variable `%s'", n), 2)
		else
			rawset(t, n, v)
		end
	end,

	__index = function (_, n)
		if not declared[n] then
			error(string.format("attempt to read undeclared variable `%s'", n), 2)
		else
			return nil
		end
	end,
})
