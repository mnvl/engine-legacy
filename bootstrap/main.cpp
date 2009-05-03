
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOM.hpp>
#include <exception/winapi_error.h>
#include <system/window.h>
#include <graphic/renderer.h>

// REMOVE:
#include <graphic/collada_loader.h>
#include <graphic/camera.h>

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpstrCmdLine, INT nShowCmd)
{
	try
	{
		{
			char buffer[1024];
			if (!GetModuleFileName(0, buffer, sizeof(buffer))) throw engine::exception::winapi_error();
		}

		xercesc::XMLPlatformUtils::Initialize();

		engine::system::window main_window("engine");
		engine::graphic::renderer renderer(&main_window);

		engine::graphic::collada_loader ci2("d:\\home\\1.dae");
		//engine::graphic::collada_loader ci2("e:\\home\\zxc.dae");
		//engine::graphic::collada_loader ci2("e:\\home\\zzz.dae");
		ci2.load();

		engine::graphic::camera cam;
		math::scalar angle = 0;

		engine::graphic::memory_model &m = const_cast<engine::graphic::memory_model &>(ci2.get_model());

		while (true)
		{
			main_window.run_message_loop();

			renderer.begin_frame();
			cam.set_position(math::vec<3>(5000 * cos(angle), 0, 5000 * sin(angle)), math::vec<3>(0, 0, 0), math::vec<3>(0, 1, 0));
			renderer.select(cam);
			m.draw_wireframe(&renderer, 0);
			renderer.finish_frame();

			angle += 0.01f;
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