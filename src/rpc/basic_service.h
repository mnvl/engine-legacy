#pragma once

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>

namespace rpc
{

class BasicService:
		public boost::noncopyable
{
public:
	BasicService();
	virtual ~BasicService();

    void start(size_t nthreads);
	void stop();
	void wait();

    boost::asio::io_service &get_io_service() { return io_service_; }
    boost::asio::io_service const &get_io_service() const { return io_service_; }

private:
	boost::asio::io_service io_service_;
	boost::thread_group threads_;
};

}
