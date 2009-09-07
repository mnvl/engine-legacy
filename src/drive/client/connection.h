#pragma once

#include <rpc/buffer.h>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/tuple/tuple.hpp>

namespace drive
{
namespace client
{

class Connection: public boost::noncopyable
{
public:
	Connection();
	~Connection();

	void connect(std::string const &host, std::string const &port);
	void disconnect();

	bool is_connected() const;
	bool have_error() const;
	std::string error_message() const;

	boost::tuple<boost::uint16_t, rpc::BufferPtr> get_message();

private:
	class Impl;
	boost::scoped_ptr<Impl> impl_;
};

}
}
