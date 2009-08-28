#pragma once

#include <boost/scoped_ptr.hpp>
#include <rpc/server_service.h>
#include "connection_handler.h"

namespace chloe
{

class Global
{
public:
	Global();
	~Global();

	void Start();
	void Stop();
	void Wait();

	static Global &instance();

private:
	boost::scoped_ptr<ConnectionHandler> connection_handler_;
	boost::scoped_ptr<rpc::ServerService> service_;
};

}
