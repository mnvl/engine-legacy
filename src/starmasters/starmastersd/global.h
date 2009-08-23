#pragma once

#include <boost/scoped_ptr.hpp>
#include <server/service.h>
#include "connection_handler.h"

namespace starmastersd
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
	boost::scoped_ptr<server::Service> service_;
};

}