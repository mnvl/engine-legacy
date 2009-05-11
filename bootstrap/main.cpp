
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOM.hpp>
#include <exception/winapi_error.h>
#include <platform/window.h>
#include <graphic/renderer.h>

// REMOVE:
#include <graphic/collada_loader.h>
#include <graphic/static_model.h>
#include <graphic/camera.h>

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpstrCmdLine, INT nShowCmd)
{
	try
	{
		{
			char buffer[1024];
			if (!GetModuleFileName(0, buffer, sizeof(buffer))) throw exception::winapi_error();
		}

		xercesc::XMLPlatformUtils::Initialize();

		platform::window main_window("engine");
		graphic::renderer renderer(&main_window);

		graphic::collada_loader ci2("e:\\home\\z1.dae");
		ci2.load();

		graphic::camera cam;
		math::scalar angle = 0;

		//boost::shared_ptr<graphic::material> mat(new graphic::material(&renderer));
		//mat->set_texture("d:\\home\\z1.bmp");

		graphic::static_model m(&renderer);
		m.build_from_memory_model(const_cast<graphic::memory_model *>(&ci2.get_model()));

		//m.meshes[0].set_material(mat);

		while (true)
		{
			main_window.run_message_loop();

			renderer.begin_frame();
			cam.set_position(math::vec<3>(10 * cos(angle), 10 * sin(angle), 0), math::vec<3>(0, 0, 0), math::vec<3>(0, 0, 1));
			renderer.select(cam);

			m.draw();
			//((graphic::memory_model &) ci2.get_model()).draw_wireframe(&renderer, 0);

			renderer.finish_frame();

			angle += 0.01f;
			if (angle > 2 * math::PI) angle = 0;
		}

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