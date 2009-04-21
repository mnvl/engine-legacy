
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOM.hpp>
#include <engine/exception/winapi_error.h>
#include <engine/system/window.h>
#include <engine/graphic/renderer.h>

// REMOVE:
#include <engine/graphic/collada_loader.h>
#include <engine/graphic/camera.h>

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

		engine::graphic::collada_loader ci2("d:\\home\\r2.dae");
		//engine::graphic::collada_importer ci2("d:\\home\\bublik.dae");
		ci2.load();

		engine::graphic::camera cam;
		math::scalar angle = 0;

		while (true)
		{
			main_window.run_message_loop();

			renderer.begin_frame();
			cam.set_position(math::vec<3>(500 * cos(angle), 0, 500 * sin(angle)), math::vec<3>(0, 0, 0), math::vec<3>(0, 1, 0));
			renderer.select(cam);
			//ci2.get_model().draw_wireframe(&renderer, math::periodic_clamp(angle / math::PI, 0.0f, 1.0f) * 2);
			renderer.finish_frame();

			angle += 0.01;
			if (angle > 2 * math::PI) angle = 0;
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