
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOM.hpp>
#include <engine/exception/winapi_error.h>
#include <engine/system/window.h>
#include <engine/graphic/renderer.h>

// REMOVE:
#include <engine/graphic/collada_importer.h>

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpstrCmdLine, INT nShowCmd) {
	try
	{
		{
			char buffer[1024];
			if (!GetModuleFileName(0, buffer, sizeof(buffer))) throw engine::exception::winapi_error();
		}

		xercesc::XMLPlatformUtils::Initialize();

		engine::system::window main_window("engine");
		engine::graphic::renderer renderer(&main_window);

		engine::graphic::collada_importer ci2("d:\\home\\bublik.dae");
		ci2.load();

		engine::graphic::collada_importer ci3("d:\\home\\cf_shooter_cast.dae");
		ci3.load();

		while (true)
		{
			main_window.run_message_loop();
			renderer.tick();
		}

		xercesc::XMLPlatformUtils::Terminate();
	}
	catch(std::exception const &error)
	{
		MessageBox(0, error.what(), 0, MB_OK);
	}
	catch (xercesc::XMLException const &error) {
		char* message = xercesc::XMLString::transcode(error.getMessage());
		MessageBox(0, message, 0, MB_OK);
        xercesc::XMLString::release(&message);
    }
	catch (xercesc::DOMException const &error) {
		char* message = xercesc::XMLString::transcode(error.msg);
		MessageBox(0, message, 0, MB_OK);
        xercesc::XMLString::release(&message);
    }
}