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

	file_handle file_;
	chunks_t chunks_;
};

class ArchiveWriter: public Archive
{
public:
	void create(std::string const &filename);
	void write_data(std::string const &name, void *data, size_t len);
	void close();

private:
	file_handle file_;

	void write_directory();
};

}
