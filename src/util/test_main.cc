//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2012 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <time.h>
#include <stdlib.h>

namespace
{

struct Dummy
{
	Dummy()
	{
		srand((unsigned) time(0));
	}
};

Dummy dummy;
}

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
