
#include <windows.h>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOM.hpp>

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <luabind/luabind.hpp>
#include <platform/window.h>
#include <graphic/renderer.h>
#include <util/logger.h>

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpstrCmdLine, INT nShowCmd)
{
	try
	{
		util::logger::instance().set_file_by_executable_name();
		LOG_INFO("executable have been built on " << __TIMESTAMP__);

		xercesc::XMLPlatformUtils::Initialize();

        lua_State *L = luaL_newstate();
        luaL_openlibs(L);
        luabind::open(L);

        {
            using namespace luabind;

            module(L, "platform")
            [
                class_<platform::window>("window")
                .def(constructor<char const *>())
                .def("run_message_loop", &platform::window::run_message_loop)
            ];

            module(L, "graphic")
            [
                class_<graphic::renderer>("renderer")
                .def(constructor<platform::window *>())
                .def("begin_frame", &graphic::renderer::begin_frame)
                .def("finish_frame", &graphic::renderer::finish_frame)
            ];
        }

        if (luaL_dofile(L, "main.lua") != 0)
        {
            throw std::runtime_error(lua_tostring(L, -1));
        }

        lua_close(L);

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
