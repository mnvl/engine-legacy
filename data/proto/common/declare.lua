
local declare = function(name, value)
	rawset(_G, name, value)
end

setmetatable(_G, {
	__newindex = function (t, n, v)
		error(string.format("attempt to write to undeclared global variable `%s'", n), 2)
	end,

	__index = function (_, n)
		error(string.format("attempt to read undeclared global variable `%s'", n), 2)
	end,
})

declare('declare', declare)
