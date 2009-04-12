
#include <boost/scoped_ptr.hpp>
#include <boost/scoped_array.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/TransService.hpp>
#include <engine/util/xerces_lcp_transcoder.h>

namespace engine {
namespace util {

std::string xerces_lcp_transcode(XMLCh const *str) {
	static boost::scoped_ptr<xercesc::XMLLCPTranscoder> transcoder(
		xercesc::XMLPlatformUtils::fgTransService->
			makeNewLCPTranscoder(xercesc::XMLPlatformUtils::fgMemoryManager));

	boost::scoped_array<char> ansi_string(transcoder->transcode(str));
	return std::string(ansi_string.get());
}

}
}