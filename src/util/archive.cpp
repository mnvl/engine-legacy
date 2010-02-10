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

void ArchiveWriter::create(std::string const &filename)
{
	file_.open(filename, O_WRONLY);

	ARCHIVE_HEADER header;
	header.version = VERSION;
	header.directory_layout.offset = 0;
	header.directory_layout.length = 0;
	file_.write(&header, sizeof(ARCHIVE_HEADER));
}

void ArchiveWriter::write_data(std::string const &name, void *data, size_t len)
{
	CHUNK_LAYOUT chunk_layout;
	chunk_layout.offset = file_.tell();
	chunk_layout.length = len;

	file_.write(data, len);

	if (!chunks_.insert(std::make_pair(name, chunk_layout)).first)
	{
		throw std::runtime_error("archive already contains file with name " + name);
	}
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
