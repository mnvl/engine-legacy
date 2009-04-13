#pragma once

#include <string>
#include <xercesc/util/XMLString.hpp>
#include <boost/scoped_array.hpp>

namespace engine {
namespace util {

inline std::string xerces_lcp_transcode(XMLCh const *str) {
	boost::scoped_array<char> buffer(xercesc::XMLString::transcode(str));
	return std::string(buffer.get());
}

inline std::basic_string<XMLCh> xerces_lcp_transcode(char const *str) {
	boost::scoped_array<XMLCh> buffer(xercesc::XMLString::transcode(str));
	return std::basic_string<XMLCh>(buffer.get());
}

}
}
