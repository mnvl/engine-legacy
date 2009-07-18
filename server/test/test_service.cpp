
#include <iosfwd>
#include <boost/test/included/unit_test.hpp>
#include <boost/asio.hpp>
#include <server/header.h>
#include <server/handler.h>
#include <server/connection.h>
#include <server/service.h>

namespace
{

char const HOST[] = "127.0.0.1";
char const SERVICE[] = "17171";

struct echo_fixture: public server::handler
{
	server::service service;

	echo_fixture():
		service(this)
    {
		service.start(HOST, SERVICE, 4);
	}

	~echo_fixture()
	{
		service.stop();
		service.wait();
	}

	void process(server::connection *connection_ptr, uint16_t id, size_t len, std::istream &is)
	{
		std::vector<char> message(len);
		is.read(&message[0], len);

		BOOST_ASSERT (!is.bad());

		boost::shared_ptr<boost::asio::streambuf> buffer(new boost::asio::streambuf(sizeof(server::header)));
		std::ostream os(buffer.get());
		os.write(&message[0], len);

		connection_ptr->write(buffer);
	}

	void error(server::connection *connection_ptr, boost::system::error_code const &error)
	{
		BOOST_ASSERT (false);
	}
};

}

void test_echo()
{
	echo_fixture fixture;

	for (size_t i = 0; i < 10; i++)
	{
		std::vector<char> message(rand() % 4096);
		for (size_t j = 0; j < message.size(); j++) message[i] = (char) rand();

		boost::asio::ip::tcp::iostream stream(HOST, SERVICE);

		{
			server::header header;
			header.id = 0;
			header.len = message.size() + sizeof(header);
			stream.write((char const *) &header, sizeof(header));
		}

		stream.write(&message[0], message.size());

		std::vector<char> reply(message.size());
		stream.read(&reply[0], message.size());

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
