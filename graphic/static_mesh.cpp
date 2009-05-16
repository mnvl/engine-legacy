
#include <exception/directx_error.h>
#include <graphic/static_model.h>
#include <graphic/static_mesh.h>

namespace graphic
{

static_mesh::static_mesh(static_model *model_ptr):
mesh(model_ptr)
{
}

static_mesh::~static_mesh()
{
}

static_model *static_mesh::get_model()
{
	return (static_model *) model_ptr_;
}

void static_mesh::draw()
{
}

void static_mesh::set_vertices_count(size_t count)
{
}

size_t static_mesh::get_vertices_count()
{
	return 0;
}

void static_mesh::set_indices_count(size_t count)
{
}

void static_mesh::lock()
{
}

void static_mesh::set_positions(math::vec<3> const *buffer)
{
}

void static_mesh::set_normals(math::vec<3> const *buffer)
{
}

void static_mesh::set_texcoords(math::vec<2> const *buffer)
{
}

void static_mesh::set_joint_indices(math::vec<4, unsigned> const *buffer)
{
}

void static_mesh::set_joint_weights(math::vec<4> const *buffer)
{
}

void static_mesh::set_face_indices(unsigned *buffer)
{
}

void static_mesh::unlock()
{
}

}
