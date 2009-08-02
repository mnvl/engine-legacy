
#include <boost/asio.hpp>
#include <boost/test/included/unit_test.hpp>
#include <iosfwd>
#include <server/header.h>
#include <server/handler.h>
#include <server/connection.h>
#include <server/service.h>

namespace
{

char const HOST[] = "127.0.0.1";
char const SERVICE[] = "17171";

struct echo_fixture: public server::Handler
{
	server::Service service;
	bool completed;

	echo_fixture():
		service(this),
		completed(false)
    {
		service.start(HOST, SERVICE, 4);
	}

	~echo_fixture()
	{
		service.stop();
		service.wait();
	}

	void process(server::Connection *connection_ptr, boost::uint16_t id, server::Buffer &in)
	{
		std::vector<char> message(in.read_bytes_available());
		if (message.size()) in.read(&message[0], message.size());
		for (size_t j = 0; j < message.size(); j++) BOOST_ASSERT(message[j] == (char) j);

		boost::shared_ptr<server::Buffer> out(new server::Buffer());

		{
			server::header h;
			h.id = id;
			h.len = sizeof(h) + message.size();
			out->write(&h, sizeof(h));
		}

		if (message.size()) out->write(&message[0], message.size());

		connection_ptr->write(out);

		completed = true;
	}

	void error(server::Connection *connection_ptr, boost::system::error_code const &error)
	{
		BOOST_ASSERT (completed);
	}
};

}

void test_echo()
{
	static size_t const MESSAGE_SIZE[] = {
		0,
		1,
		41,
		2650,
		4091,
		4092,
		4093,
		4095,
		4096,
		4097,
		20123,
		30132,
		40213,
		50312,
		65530,
		65531
	};

	echo_fixture fixture;

	for (size_t i = 0; i < sizeof(MESSAGE_SIZE) / sizeof(MESSAGE_SIZE[0]); i++)
	{
		std::vector<char> message(MESSAGE_SIZE[i]);
		for (size_t j = 0; j < message.size(); j++) message[j] = (char) j;

		boost::asio::ip::tcp::iostream stream(HOST, SERVICE);

		{
			server::header header;
			header.id = i;
			header.len = message.size() + sizeof(header);
			stream.write((char const *) &header, sizeof(header));
		}

		if (message.size())
		{
			stream.write(&message[0], message.size());
		}

		std::vector<char> reply(message.size());

		{
			server::header h;
			stream.read((char *) &h, sizeof(h));

			BOOST_ASSERT (h.id == i);
			BOOST_ASSERT (h.len == sizeof(h) + message.size());
		}

		if (message.size()) stream.read(&reply[0], message.size());

		BOOST_ASSERT (message == reply);
	}
}

boost::unit_test::test_suite*
init_unit_test_suite(int argc, char *argv[])
{
	boost::unit_test::framework::master_test_suite().p_name.value = "test_service";
	boost::unit_test::framework::master_test_suite().add(BOOST_TEST_CASE(&test_echo), 0, /* timeout */ 10 );

    return 0;
}
