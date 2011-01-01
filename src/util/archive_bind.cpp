//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2011 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <boost/shared_ptr.hpp>
#include <luabind/luabind.hpp>
#include "archive.h"

namespace util
{

void ArchiveReader::bind(lua_State *L)
{
	using namespace luabind;

	module_(L, "util")
	[
		class_<ArchiveReader, boost::shared_ptr<ArchiveReader> >("ArchiveReader")
		.def(constructor<>())
		.def("open", &ArchiveReader::open)
		.def("read_string", &ArchiveReader::read_string)
		.def("close", &ArchiveReader::close)
	];
}

void ArchiveWriter::bind(lua_State *L)
{
	using namespace luabind;

	module_(L, "util")
	[
		class_<ArchiveWriter, boost::shared_ptr<ArchiveWriter> >("ArchiveWriter")
		.def(constructor<>())
		.def("create", &ArchiveWriter::create)
		.def("write_file", (void (ArchiveWriter::*)(std::string const &, std::string const &)) &ArchiveWriter::write_file)
		.def("close", &ArchiveWriter::close)
	];
}

}
