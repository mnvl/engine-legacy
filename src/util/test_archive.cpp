//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <boost/test/unit_test.hpp>
#include "logger.h"
#include "archive.h"

BOOST_AUTO_TEST_SUITE(archive)

BOOST_AUTO_TEST_CASE(writer_reader)
{
	{
		util::ArchiveWriter writer;
		writer.create("test.archive");

		writer.write_data("one", "hello");
		writer.write_data("two", "world");

		writer.close();
	}
}

BOOST_AUTO_TEST_SUITE_END()
