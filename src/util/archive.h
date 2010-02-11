#pragma once
//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <boost/cstdint.hpp>
#include <boost/unordered_map.hpp>
#include <luabind/lua_include.hpp>
#include "file_handle.h"

namespace util
{

class Archive
{
protected:
	static unsigned const VERSION = 0x0100;

#pragma pack(push, 1)
	struct CHUNK_LAYOUT
	{
		boost::uint32_t offset, length;
	};

	struct ARCHIVE_HEADER
	{
		boost::uint16_t version;
		CHUNK_LAYOUT directory_layout;
	};
#pragma pack(pop)

	typedef boost::unordered_map<std::string, CHUNK_LAYOUT> chunks_t;

	FileHandle file_;
	chunks_t chunks_;

	void read_directory();
};

class ArchiveReader: public Archive
{
public:
	void open(std::string const &filename);
	void read_data(std::string const &key, std::vector<char> &buffer);
	void close();
};

class ArchiveWriter: public Archive
{
public:
	void create(std::string const &filename);
	void open(std::string const &filename);
	void write_data(std::string const &key, void const *data, size_t len);
	void write_file(std::string const &key, FileHandle &file_handle);
	void write_file(std::string const &key, std::string const &path);
	void close();

	static void bind(lua_State *L);

private:
	void write_directory();
};

}
