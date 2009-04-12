
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOM.hpp>
#include <engine/system/window.h>
#include <engine/graphic/renderer.h>

// REMOVE:
#include <engine/graphic/collada_importer.h>

using namespace xercesc;

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpstrCmdLine, INT nShowCmd) {
	try
	{
	    XMLPlatformUtils::Initialize();

		engine::system::window main_window("engine");
		engine::graphic::renderer renderer(&main_window);

		engine::graphic::collada_importer ci;
		ci.load("d:\\home\\plane.dae");

		while (true)
		{
			main_window.run_message_loop();
			renderer.tick();
		}

		XMLPlatformUtils::Terminate();
	}
	catch(std::exception const &error)
	{
		MessageBox(0, error.what(), 0, MB_OK);
	}
	catch (xercesc::XMLException const &error) {
		char* message = xercesc::XMLString::transcode(error.getMessage());
		MessageBox(0, message, 0, MB_OK);
        XMLString::release(&message);
    }
	catch (xercesc::DOMException const &error) {
		char* message = xercesc::XMLString::transcode(error.msg);
		MessageBox(0, message, 0, MB_OK);
        XMLString::release(&message);
    }
}