
#include <windows.h>
#include <lualib.h>
#include <xercesc/util/PlatformUtils.hpp>
#include <luabind/luabind.hpp>
#include <graphic/bind.h>
#include <platform/window.h>
#include <util/logger.h>

class LuaStateHolder
{
public:
    LuaStateHolder():
		L_(lua_open())
	{
		xercesc::XMLPlatformUtils::Initialize();

		luaL_openlibs(L_);
		luabind::open(L_);

		graphic::bind(L_);
		platform::Window::bind(L_);
	}

    ~LuaStateHolder()
	{
		lua_close(L_);

        xercesc::XMLPlatformUtils::Terminate();
	}

	void run(char const *filename)
	{
		if (luaL_dofile(L_, filename))
		{
			throw std::runtime_error(lua_tostring(L_, 1));
		}
	}

private:
    lua_State *L_;
};


INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpstrCmdLine, INT nShowCmd)
{
    try
    {
		LuaStateHolder().run(lpstrCmdLine);
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
