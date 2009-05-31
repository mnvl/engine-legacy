#pragma once

#include <d3d9.h>
#include <boost/cstdint.hpp>
#include <util/boost_com_intrusive_ptr.h>
#include <math/vec.h>
#include <math/matrix.h>
#include <graphic/vertex.h>
#include <graphic/model.h>

namespace graphic
{

class renderer;

class heightfield_loader {
public:
    class load_error: public std::runtime_error {
	public:
		load_error(std::string const &message):
		runtime_error(message)
		{
		}
	};

    heightfield_loader(std::string const &filename, model *model_ptr, renderer *renderer_ptr = 0);
	~heightfield_loader();

    template<class T>
	void load_from_raw(math::matrix<4, 4> const &tf, size_t nrows, size_t ncols,
		math::vec<3> const &scale, std::string const &texture_name);

private:
	std::string filename_;
	model *model_ptr_;
	renderer *renderer_ptr_;
};

}
