#pragma once

#include <rpc/buffer.h>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

namespace drive
{
namespace client
{

class ConnectionHandler: public boost::noncopyable
{
public:
	ConnectionHandler();
	~ConnectionHandler();

	void connect(std::string const &host, std::string const &port);
	void disconnect();

	bool is_connected() const;
	bool have_error() const;
	std::string error_message() const;

	bool get_message(boost::uint16_t &id, rpc::BufferPtr &buffer);

private:
	class Impl;
	boost::scoped_ptr<Impl> impl_;
};

}
}
