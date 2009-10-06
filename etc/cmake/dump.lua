
local dump_character = function(ch)
	if ch >= 0x20 and ch < 0x80 then
		return string.format('\'%c\'', ch)
	else
		return string.format('0x%02x', ch)
	end
end

if #arg ~= 0 and #arg ~= 2 then
    error('usage: ' .. arg[0] .. ' [<input-file-name> <output-file-name>]')
end

local input_file_name, output_file_name = unpack(arg)

local input_data = (input_file_name and assert(io.open(input_file_name, 'rb')) or io.stdin):read("*all")
local input_length = string.len(input_data)

local output_file = output_file_name and assert(io.open(output_file_name, 'wb')) or io.stdout

for index = 1, input_length do
	if index % 16 == 1 then
	   output_file:write(string.format('\t/* 0x%08x */\t', index - 1))
	end

	output_file:write(dump_character(string.byte(input_data, index)))

	if index ~= input_length then
	   output_file:write(', ')
	end

	if index % 16 == 0 then
	   output_file:write('\n')
	end
end

output_file:write('\n')