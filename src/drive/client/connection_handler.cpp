
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <rpc/handler.h>
#include <rpc/connection.h>
#include <rpc/client_service.h>
#include <util/logger.h>
#include <drive/proto/id.pb.h>
#include <drive/proto/service.pb.h>
#include "connection_handler.h"

namespace drive
{
namespace client
{

class ConnectionHandler::Impl:
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

	bool get_message(boost::uint16_t &id, rpc::BufferPtr &buffer)
	{
		boost::unique_lock<boost::mutex> lock(mutex_);
		if (incoming_messages_.empty()) return false;

		message_t &message = incoming_messages_.front();

		id = message.first;
		buffer = message.second;

		incoming_messages_.pop_front();

		return true;
	}

private:
	typedef std::pair<boost::uint16_t, rpc::BufferPtr> message_t;

	mutable boost::mutex mutex_;
	rpc::ClientService service_;
	bool is_connected_, have_error_;
	std::string error_message_;
	std::deque<message_t> incoming_messages_;

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
		incoming_messages_.push_back(message_t(id, rpc::BufferPtr(new rpc::Buffer(buf))));
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

ConnectionHandler::ConnectionHandler()
{
}

ConnectionHandler::~ConnectionHandler()
{
}

void ConnectionHandler::connect(std::string const &host, std::string const &port)
{
	impl_.reset(new Impl());
	impl_->connect(host, port);
	impl_->start();
}

void ConnectionHandler::disconnect()
{
	impl_->stop();
	impl_->wait();
	impl_.reset();
}

bool ConnectionHandler::is_connected() const
{
	return impl_ && impl_->is_connected();
}

bool ConnectionHandler::have_error() const
{
	return impl_->have_error();
}

std::string ConnectionHandler::error_message() const
{
	return impl_->error_message();
}

bool ConnectionHandler::get_message(boost::uint16_t &id, rpc::BufferPtr &buffer)
{
	return impl_->get_message(id, buffer);
}

}
}