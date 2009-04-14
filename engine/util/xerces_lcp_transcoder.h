#pragma once

#include <string>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <boost/scoped_array.hpp>

namespace engine {
namespace util {

inline std::string xerces_lcp_transcode(XMLCh const *str) {
	// TODO: free using XMLString::release
	boost::scoped_array<char> buffer(xercesc::XMLString::transcode(str));
	return std::string(buffer.get());
}

inline std::basic_string<XMLCh> xerces_lcp_transcode(char const *str) {
	// TODO: free using XMLString::release
	boost::scoped_array<XMLCh> buffer(xercesc::XMLString::transcode(str));
	return std::basic_string<XMLCh>(buffer.get());
}

inline std::basic_string<XMLCh> xerces_lcp_transcode(std::string const &str) {
	return xerces_lcp_transcode(str.c_str());
}

inline std::string xerces_get_attribute(xercesc::DOMNode const *node_ptr, std::string const &attribute_name) {
	return xerces_lcp_transcode(node_ptr->getAttributes()->getNamedItem
		(xerces_lcp_transcode(attribute_name).c_str())->getTextContent());
}

inline std::string xerces_get_text(xercesc::DOMNode const *node_ptr) {
	return xerces_lcp_transcode(node_ptr->getTextContent());
}

}
}
