
#include <boost/lambda/lambda.hpp>
#include <exception/directx_error.h>
#include <graphic/static_model.h>

namespace graphic {

static_model::static_model(renderer *renderer_ptr):
model(renderer_ptr)
{
}

static_model::~static_model()
{
}

void static_model::build_from_memory_model(memory_model *model_ptr)
{
}

void static_model::draw()
{
	for (meshes_type::iterator it = meshes.begin(); it != meshes.end(); it++)
	{
		it->draw();
	}
}

}
