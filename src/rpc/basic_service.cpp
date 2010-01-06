//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <boost/bind.hpp>
#include <util/logger.h>
#include "basic_service.h"

namespace rpc
{

BasicService::BasicService(Handler *handler):
    handler_(handler)
{
}

BasicService::~BasicService()
{
}

void BasicService::start(size_t nthreads)
{
	for (size_t i = 0; i < nthreads; ++i)
	{
		threads_.create_thread(boost::bind(&boost::asio::io_service::run, &io_service_));
	}
}

void BasicService::stop()
{
	io_service_.stop();
}

void BasicService::wait()
{
	threads_.join_all();
}

}
