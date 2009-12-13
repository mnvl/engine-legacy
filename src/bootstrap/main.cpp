
#include <crtdbg.h>
#include <windows.h>
#include <lualib.h>
#include <xercesc/util/PlatformUtils.hpp>
#include <luabind/luabind.hpp>
#include <luabind/detail/pcall.hpp>
#include <graphic/bind.h>
#include <platform/timer.h>
#include <platform/window.h>
#include <platform/input.h>
#include <platform/keyboard.h>
#include <platform/mouse.h>
#include <math/vec.h>
#include <math/matrix.h>
#include <util/logger.h>
#include <util/lua_backtrace.h>

class Main
{
public:
    Main():
		L_(lua_open())
	{
		xercesc::XMLPlatformUtils::Initialize();

		luaL_openlibs(L_);
		luabind::open(L_);

		graphic::bind(L_);

		platform::Timer::bind(L_);
		platform::Window::bind(L_);

		math::bind_vec(L_);
        math::bind_matrix(L_);

		util::Logger::bind(L_);

		platform::Input::bind(L_);
		platform::Keyboard::bind(L_);
		platform::Mouse::bind(L_);
	}

    ~Main()
	{
		lua_close(L_);

        xercesc::XMLPlatformUtils::Terminate();
	}

	void run(char const *filename)
	{
		int result = luaL_loadfile(L_, filename);
		if (result == 0) result = luabind::detail::pcall(L_, 0, 0);

		switch (result)
		{
		case LUA_ERRSYNTAX:
			throw std::runtime_error("syntax eror");

		case LUA_ERRRUN:
			throw std::runtime_error(lua_tostring(L_, -1));

		default:
			throw std::runtime_error("unknown error");
		}
	}

private:
    lua_State *L_;
};


INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpstrCmdLine, INT nShowCmd)
{
    try
    {
		luabind::set_pcall_callback(util::make_lua_backtrace);

		util::Logger::instance().enable_source_position();

		Main().run(lpstrCmdLine);
    }
    catch (std::exception const &error)
    {
        LOG_ERROR("exception caught: " << error.what());
        return 1;
    }
    catch (...)
    {
        LOG_ERROR("unknown exception caught");
        return 1;
    }

    return 0;
}
