//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2012 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <log4cplus/configurator.h>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

namespace
{

struct Dummy
{
	Dummy()
	{
		log4cplus::BasicConfigurator::doConfigure();
	}
};

Dummy dummy;

}
