//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#pragma once

#include <boost/enable_shared_from_this.hpp>
#include "basic_service.h"

namespace rpc
{

class Connection;

class ServerService:
		public BasicService,
		public boost::enable_shared_from_this<ServerService>
{
public:
	ServerService(Handler *handler);
	~ServerService();

	void start(std::string const &host_name, std::string const &service_name, size_t nthreads);

private:
	boost::asio::ip::tcp::acceptor acceptor_;

	void schedule_accept();
	void handle_accept(boost::shared_ptr<Connection> const &connection_ptr,
		boost::system::error_code const &error);
};

}
