#pragma once
//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2012 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <string>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <boost/shared_array.hpp>
#include <boost/lexical_cast.hpp>

namespace util
{

inline std::string xerces_lcp_transcode(XMLCh const *str) {
	char *buffer = xercesc::XMLString::transcode(str);
	std::string result(buffer);
	xercesc::XMLString::release(&buffer);
	return result;
}

inline std::string xerces_lcp_transcode(std::wstring const &str) {
	return xerces_lcp_transcode(str.c_str());
}

inline std::wstring
xerces_lcp_transcode(char const *str) {
	XMLCh *buffer = xercesc::XMLString::transcode(str);
	std::wstring result(buffer);
	xercesc::XMLString::release(&buffer);
	return result;
}

inline std::wstring
xerces_lcp_transcode(std::string const &str) {
	return xerces_lcp_transcode(str.c_str());
}

template<class T> inline T
xerces_get_attribute(xercesc::DOMNode const *node_ptr, std::wstring const &attribute_name, T const &default_value = T())
{
	xercesc::DOMNode *attibute_ptr = node_ptr->getAttributes()->getNamedItem(attribute_name.c_str());
	return attibute_ptr ? boost::lexical_cast<T>(attibute_ptr->getTextContent()) : default_value;
}

inline std::wstring
xerces_get_text(xercesc::DOMNode const *node_ptr) {
	return node_ptr->getTextContent();
}

}

