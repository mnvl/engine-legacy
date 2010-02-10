//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <iostream>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include "file_handle.h"
#include "archive.h"

void create_archive(std::string const &filename, int argc, char *argv[])
{
	std::cerr << "creating archive " << filename << "...\n";
	util::ArchiveWriter archive_writer;
	archive_writer.create(filename);

	while (argc > 0)
	{
		std::vector<std::string> parts;
		boost::split(parts, *argv++, boost::is_any_of("="), boost::algorithm::token_compress_on);
		--argc;

		if (parts.size() != 2) throw std::runtime_error("invalid argument " + std::string(*argv));

		std::cerr << "adding " << parts[0] << " = " << parts[1] << "...\n";
		util::FileHandle file(parts[1], O_RDONLY);
		archive_writer.write_file(parts[0], file);
	}

	archive_writer.close();
}

int main(int argc, char *argv[])
{
	if (argc <= 3)
	{
		std::cerr << "usage: " << argv[0] << " create <archive-file> <key>=<filename> <key>=<filename> ..." << std::endl;
		return 1;
	}

	try
	{
		++argv;
		--argc;

		std::string command = *argv++;
		--argc;

		std::string archive = *argv++;
		--argc;

		if (command == "create")
		{
			create_archive(archive, argc, argv);
		}
		else
		{
			throw std::runtime_error("unknown command " + command);
		}
	}
	catch(std::exception const &error)
	{
		std::cerr << "exception caught: " << error.what() << std::endl;
		return 1;
	}
	catch(...)
	{
		std::cerr << "unknown exception caught" << std::endl;
		return 1;
	}

	return 0;
}