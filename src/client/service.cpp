
#include <boost/bind.hpp>
#include <util/logger.h>
#include "service.h"

namespace client
{

Service::Service()
{
}

Service::~Service()
{
}

void Service::start(size_t nthreads)
{
	for (size_t i = 0; i < nthreads; ++i)
	{
		threads_.create_thread(boost::bind(&boost::asio::io_service::run, &io_service_));
	}
}

void Service::stop()
{
	io_service_.stop();
}

void Service::wait()
{
	threads_.join_all();
}

}
