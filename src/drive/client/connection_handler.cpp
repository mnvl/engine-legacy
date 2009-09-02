
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

class ConnectionHandler::Impl
{
public:
	Impl(ConnectionHandler *handler):
		handler_(handler),
		service_(handler)
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

private:
	ConnectionHandler *handler_;
	rpc::ClientService service_;
};

ConnectionHandler::ConnectionHandler():
	impl_(new Impl(this))
{
}

ConnectionHandler::~ConnectionHandler()
{
}

void ConnectionHandler::connect(std::string const &host, std::string const &port)
{
	impl_->connect(host, port);
	impl_->start();
}

void ConnectionHandler::stop()
{
	impl_->stop();
	impl_->wait();
}

void ConnectionHandler::on_connect(boost::shared_ptr<rpc::Connection> const &connection_ptr)
{
	boost::system::error_code dummy_error_code;
	LOG_DEBUG("connected to " << connection_ptr->socket().remote_endpoint(dummy_error_code));

	proto::service::handshake message;
	message.set_protocol_version(1);
	connection_ptr->write(proto::SERVICE_HANDSHAKE, message);
}

void ConnectionHandler::on_read(boost::shared_ptr<rpc::Connection> const &connection_ptr, boost::uint16_t id, rpc::Buffer &buf)
{
	switch (id)
	{
	case proto::SERVICE_HANDSHAKE_REPLY:
	{
		proto::service::handshake_reply reply;
		reply.ParseFromArray(buf.read_ptr(), buf.read_bytes_available());

		if (!reply.is_supported()) {
			LOG_ERROR("protocol version is not supported by server");
			throw CloseConnectionException();
		}

		LOG_DEBUG("handshake completed");

		break;
	}

	default:
		LOG_ERROR("invalid message " << id);
		throw CloseConnectionException();
	}
}

void ConnectionHandler::on_error(boost::shared_ptr<rpc::Connection> const &connection_ptr, boost::system::error_code const &error)
{
	boost::system::error_code dummy_error_code;
	LOG_ERROR("error on " << connection_ptr->socket().remote_endpoint(dummy_error_code) << ": " << error);
}

}
}