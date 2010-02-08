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
#include "connection.h"
#include "client_service.h"

#include <util/logger.h>

namespace rpc
{

ClientService::ClientService(Handler *handler):
	BasicService(handler)
{
}

ClientService::~ClientService()
{
}

void ClientService::connect(std::string const &host, std::string const &port)
{
    boost::asio::ip::tcp::resolver resolver(get_io_service());
    boost::asio::ip::tcp::resolver::query query(host, port);
    boost::asio::ip::tcp::resolver::iterator endpoint_it = resolver.resolve(query);

    boost::shared_ptr<Connection> connection(new Connection(get_handler(), get_io_service()));

    boost::asio::ip::tcp::endpoint endpoint = *endpoint_it++;
    connection->socket().async_connect(endpoint,
        boost::bind(&ClientService::handle_connect, this, boost::asio::placeholders::error, connection, endpoint_it));
}

void ClientService::handle_connect(const boost::system::error_code& error, boost::shared_ptr<Connection> const &connection,
    boost::asio::ip::tcp::resolver::iterator endpoint_it)
{
    if (error)
    {
        if (endpoint_it != boost::asio::ip::tcp::resolver::iterator())
        {
            connection->socket().close();

            boost::asio::ip::tcp::endpoint endpoint = *endpoint_it++;
            connection->socket().async_connect(endpoint,
                boost::bind(&ClientService::handle_connect, this, boost::asio::placeholders::error, connection, endpoint_it));
        }
        else
        {
            get_handler()->on_error(connection, error);
        }

        return;
    }

    connection->start();
    get_handler()->on_connect(connection);
}

}
