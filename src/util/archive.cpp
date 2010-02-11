//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include "archive.h"

namespace util
{

void Archive::read_directory()
{
	chunks_.clear();

	ARCHIVE_HEADER header;
	file_.lseek(0, SEEK_SET);
	file_.read(&header, sizeof(ARCHIVE_HEADER));

	if (header.version != VERSION)
	{
		throw std::runtime_error("archive with invalid version");
	}

	if (header.directory_layout.length == 0)
	{
		return;
	}

	std::vector<char> buffer(header.directory_layout.length);
	file_.lseek(header.directory_layout.offset, SEEK_SET);
	file_.read(&buffer[0], buffer.size());

	char *cursor = &buffer[0];
	size_t bytes_left = buffer.size();

	while (bytes_left > 0)
	{
		std::string name;

		while (bytes_left > 0 && *cursor)
		{
			name += *cursor++;
			--bytes_left;
		}

		if (bytes_left > 0 && *cursor == 0)
		{
			--bytes_left;
			++cursor;
		}
		else
		{
			throw std::runtime_error("invalid directory record");
		}

		CHUNK_LAYOUT chunk_layout;

		if (bytes_left >= sizeof(CHUNK_LAYOUT))
		{
			std::copy(cursor, cursor + sizeof(CHUNK_LAYOUT), (char *) &chunk_layout);

			cursor += sizeof(CHUNK_LAYOUT);
			bytes_left -= sizeof(CHUNK_LAYOUT);
		}
		else
		{
			throw std::runtime_error("invalid directory record");
		}

		chunks_.insert(std::make_pair(name, chunk_layout));
	}
}

void ArchiveReader::open(std::string const &filename)
{
	file_.open(filename, O_RDONLY);
	read_directory();
}

void ArchiveReader::read_data(std::string const &key, std::vector<char> &buffer)
{
	chunks_t::const_iterator it = chunks_.find(key);
	if (it == chunks_.end()) throw std::runtime_error("can't find chunk " + key);

	buffer.resize(it->second.length);
	file_.lseek(it->second.offset, SEEK_SET);
	file_.read(&buffer[0], buffer.size());
}

luabind::object ArchiveReader::read_string(lua_State *L, std::string const &key)
{
	std::vector<char> buffer;
	read_data(key, buffer);

	lua_pushlstring(L, &buffer[0], buffer.size());

	return luabind::object(luabind::from_stack(L, -1));
}

void ArchiveReader::close()
{
	file_.close();
}

void ArchiveWriter::create(std::string const &filename)
{
	file_.open(filename, O_WRONLY | O_CREAT | O_TRUNC);

	ARCHIVE_HEADER header;
	header.version = VERSION;
	header.directory_layout.offset = 0;
	header.directory_layout.length = 0;
	file_.write(&header, sizeof(ARCHIVE_HEADER));
}

void ArchiveWriter::open(std::string const &filename)
{
	file_.open(filename, O_RDWR);
	read_directory();
}

void ArchiveWriter::write_data(std::string const &key, void const *data, size_t len)
{
	CHUNK_LAYOUT chunk_layout;
	chunk_layout.offset = file_.tell();
	chunk_layout.length = len;

	file_.write(data, len);

	if (!chunks_.insert(std::make_pair(key, chunk_layout)).second)
	{
		throw std::runtime_error("archive already contains file with key " + key);
	}
}

void ArchiveWriter::write_file(std::string const &key, FileHandle &file_handle)
{
	std::vector<char> buffer;
	file_handle.read_all(buffer);
	write_data(key, &buffer[0], buffer.size());
}

void ArchiveWriter::write_file(std::string const &key, std::string const &path)
{
	FileHandle file(path, O_RDONLY);
	write_file(key, file);
}

void ArchiveWriter::close()
{
	write_directory();
	file_.close();
}

void ArchiveWriter::write_directory()
{
	ARCHIVE_HEADER header;
	header.version = VERSION;
	header.directory_layout.offset = file_.tell();
	header.directory_layout.length = 0;

	for (chunks_t::const_iterator it = chunks_.begin(); it != chunks_.end(); it++)
	{
		file_.write(it->first.c_str(), it->first.size() + 1);
		file_.write(&it->second, sizeof(CHUNK_LAYOUT));
	}

	header.directory_layout.length = file_.tell() - header.directory_layout.offset;
	file_.lseek(0, SEEK_SET);
	file_.write(&header, sizeof(ARCHIVE_HEADER));
}

}
