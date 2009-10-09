#pragma once

#include <set>
#include <map>
#include <vector>
#include <stdexcept>
#include <xercesc/dom/DOM.hpp>
#include <luabind/lua_include.hpp>
#include <math/vec.h>
#include <graphic/model.h>

namespace graphic {

class Renderer;

class ColladaLoader {
public:
	class load_error: public std::runtime_error {
	public:
		load_error(std::string const &message):
		runtime_error(message)
		{
		}
	};

	ColladaLoader(std::string const &filename, Model *model_ptr, Renderer *renderer_ptr = 0);
	~ColladaLoader();

	void load();

    static void bind(lua_State *L);

private:
	std::string filename_;
	Model *model_ptr_;
	Renderer *renderer_ptr_;
};

}
