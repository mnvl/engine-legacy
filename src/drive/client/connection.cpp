
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <rpc/handler.h>
#include <rpc/connection.h>
#include <rpc/client_service.h>
#include <util/logger.h>
#include <drive/proto/id.pb.h>
#include <drive/proto/service.pb.h>
#include "connection.h"

namespace drive
{
namespace client
{

class Connection::Impl:
	public rpc::Handler,
	public boost::noncopyable
{
public:
	Impl():
		service_(this)
	{
	}

	~Impl()
	{
	}

	void connect(std::string const &host, std::string const &port)
	{
		service_.connect(host, port);
	}

	void start()
	{
		service_.start(1);
	}

	void stop()
	{
		service_.stop();
	}

	void wait()
	{
		service_.wait();
	}

	bool is_connected() const
	{
		boost::unique_lock<boost::mutex> lock(mutex_);
		return is_connected_;
	}

	bool have_error() const
	{
		boost::unique_lock<boost::mutex> lock(mutex_);
		return have_error_;
	}

	std::string error_message() const
	{
		boost::unique_lock<boost::mutex> lock(mutex_);
		return error_message_;
	}

	boost::tuple<boost::uint16_t, rpc::BufferPtr> get_message()
	{
		boost::unique_lock<boost::mutex> lock(mutex_);

		if (incoming_messages_.empty()) return boost::tuple<boost::uint16_t, rpc::BufferPtr>();

		boost::tuple<boost::uint16_t, rpc::BufferPtr> result = incoming_messages_.front();
		incoming_messages_.pop_front();
		return result;
	}

private:
	mutable boost::mutex mutex_;
	rpc::ClientService service_;
	bool is_connected_, have_error_;
	std::string error_message_;
	std::deque<boost::tuple<boost::uint16_t, rpc::BufferPtr> > incoming_messages_;

    void on_connect(boost::shared_ptr<rpc::Connection> const &connection_ptr)
	{
		boost::unique_lock<boost::mutex> lock(mutex_);

		boost::system::error_code dummy_error_code;
		LOG_DEBUG("connected to " << connection_ptr->socket().remote_endpoint(dummy_error_code));

		is_connected_ = true;
	}

	void on_read(boost::shared_ptr<rpc::Connection> const &connection_ptr, boost::uint16_t id, rpc::Buffer &buf)
	{
		boost::unique_lock<boost::mutex> lock(mutex_);
		incoming_messages_.push_back(boost::make_tuple(id, rpc::BufferPtr(new rpc::Buffer(buf))));
	}

	void on_error(boost::shared_ptr<rpc::Connection> const &connection_ptr, boost::system::error_code const &error)
	{
		boost::unique_lock<boost::mutex> lock(mutex_);

		have_error_ = true;
		error_message_ = error.message();

		boost::system::error_code dummy_error_code;
		LOG_ERROR("error on " << connection_ptr->socket().remote_endpoint(dummy_error_code) << ": " << error);
	}
};

Connection::Connection()
{
}

Connection::~Connection()
{
}

void Connection::connect(std::string const &host, std::string const &port)
{
	impl_.reset(new Impl());
	impl_->connect(host, port);
	impl_->start();
}

void Connection::disconnect()
{
	impl_->stop();
	impl_->wait();
	impl_.reset();
}

bool Connection::is_connected() const
{
	return impl_ && impl_->is_connected();
}

bool Connection::have_error() const
{
	return impl_->have_error();
}

std::string Connection::error_message() const
{
	return impl_->error_message();
}

boost::tuple<boost::uint16_t, rpc::BufferPtr> Connection::get_message()
{
	return impl_->get_message();
}

}
}
