
#define BOOST_TEST_MODULE test_proto
#include <boost/test/included/unit_test.hpp>
#include <drive/proto/service.pb.h>

// just to test linking against proto library
BOOST_AUTO_TEST_SUITE (test_proto)

BOOST_AUTO_TEST_CASE (test_handshake)
{
	drive::proto::service::handshake hs;
	hs.set_protocol_version(100);
}

BOOST_AUTO_TEST_SUITE_END()
