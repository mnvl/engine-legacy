
#include <boost/cstdint.hpp>
#include <platform/file_handle.h>
#include <graphic/build_smooth_normals.h>
#include <graphic/heightfield_loader.h>

namespace graphic
{

heightfield_loader::heightfield_loader(std::string const &filename, model *model_ptr, renderer *renderer_ptr):
    filename_(filename),
    model_ptr_(model_ptr),
    renderer_ptr_(renderer_ptr)
{
}

heightfield_loader::~heightfield_loader()
{
}

template<class T>
void heightfield_loader::load_from_raw(math::matrix<4,4> const &tf, size_t nrows, size_t ncols,
    math::vec<3> const &scale, std::string const &texture_name)
{
    std::vector<char> buffer;
    platform::file_handle(filename_.c_str(), O_RDONLY).read_all(buffer);

    mesh *mesh_ptr = model_ptr_->create_mesh();

    T *get = (T *) &buffer[0];

    std::vector<math::vec<3> > positions;
    std::vector<math::vec<3> > normals;
    std::vector<math::vec<2> > texcoords;
    std::vector<unsigned> indices;

    positions.resize(nrows * ncols);
    texcoords.resize(positions.size());
    indices.reserve((nrows - 1) * (ncols - 1) * 2 * 3);

    for (size_t row = 0, i = 0; row < nrows; row++)
    {
        for (size_t col = 0; col < ncols; col++, i++, get++)
        {
            positions[i] = math::vec<3>(col * scale.z / ncols,
                *get * scale.y, row * scale.x / nrows);

            texcoords[i] = math::vec<2>(math::scalar(col) / math::scalar(ncols),
                math::scalar(row) / math::scalar(nrows));
        }
    }

    for (size_t row = 1; row < nrows; row++)
    {
        for (size_t col = 1; col < ncols; col++)
        {
            indices.push_back((row - 1) * ncols + (col - 1));
            indices.push_back((row - 1) * ncols + (col - 0));
            indices.push_back((row - 0) * ncols + (col - 0));

            indices.push_back((row - 0) * ncols + (col - 0));
            indices.push_back((row - 0) * ncols + (col - 1));
            indices.push_back((row - 1) * ncols + (col - 1));
        }
    }

    build_smooth_normals(positions, indices, &normals);

    mesh_ptr->set_vertices_count(positions.size());
    mesh_ptr->set_indices_count(indices.size());
    mesh_ptr->lock();
    mesh_ptr->set_positions(&positions[0]);
	mesh_ptr->set_normals(&normals[0]);
    mesh_ptr->set_texcoords(&texcoords[0]);
    mesh_ptr->set_face_indices(&indices[0]);
    mesh_ptr->unlock();

    std::auto_ptr<material> material_ptr(new material(renderer_ptr_));
    material_ptr->set_name(texture_name + " material");
    material_ptr->set_texture(texture_name);
    model_ptr_->materials.push_back(material_ptr);

    mesh::triangles t;
    t.material_ptr = &model_ptr_->materials.back();
    t.first_index_offset = 0;
    t.triangles_count = indices.size() / 3;
    t.minimal_vertex_index = 0;
    t.vertices_count = positions.size();
    mesh_ptr->triangles_list.push_back(t);

	joint *joint_ptr = model_ptr_->create_joint();
	joint_ptr->transforms.push_back(joint::transform("tf", tf));
	joint_ptr->mesh_ptr = mesh_ptr;
}

template void heightfield_loader::load_from_raw<boost::int16_t>(math::matrix<4,4> const &tf, size_t nrows, size_t ncols,
    math::vec<3> const &scale, std::string const &texture_name);

template void heightfield_loader::load_from_raw<boost::uint16_t>(math::matrix<4,4> const &tf, size_t nrows, size_t ncols,
    math::vec<3> const &scale, std::string const &texture_name);

}
