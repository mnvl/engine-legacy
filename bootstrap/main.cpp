
#include <windows.h>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOM.hpp>
#include <util/logger.h>

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpstrCmdLine, INT nShowCmd)
{
	try
	{
		util::logger::instance().set_file_by_executable_name();
		LOG_INFO("executable have been built on " << __TIMESTAMP__);

		xercesc::XMLPlatformUtils::Initialize();

		xercesc::XMLPlatformUtils::Terminate();
	}
	catch(std::exception const &error)
	{
		MessageBox(0, error.what(), 0, MB_OK);
		return 1;
	}
	catch (xercesc::XMLException const &error)
	{
		char* message = xercesc::XMLString::transcode(error.getMessage());
		MessageBox(0, message, 0, MB_OK);
        xercesc::XMLString::release(&message);
		return 1;
    }
	catch (xercesc::DOMException const &error)
	{
		char* message = xercesc::XMLString::transcode(error.msg);
		MessageBox(0, message, 0, MB_OK);
        xercesc::XMLString::release(&message);
		return 1;
    }

	return 0;
}
