
#include <boost/cstdint.hpp>
#include <util/file_handle.h>
#include <util/logger.h>
#include <graphic/build_smooth_normals.h>
#include <graphic/heightfield_loader.h>

namespace graphic
{

HeightfieldLoader::HeightfieldLoader(std::string const &filename, Model *model_ptr, Renderer *renderer_ptr):
    filename_(filename),
    model_ptr_(model_ptr),
    renderer_ptr_(renderer_ptr)
{
}

HeightfieldLoader::~HeightfieldLoader()
{
}

template<class T>
void HeightfieldLoader::load_from_raw(math::matrix<4,4> const &tf, size_t nrows, size_t ncols,
    math::vec<3> const &scale, std::string const &texture_name)
{
	LOG_INFO("importing heightfield from raw file " << filename_ << ", nrows = "
		<< nrows << ", ncols = " << ncols);

    std::vector<char> buffer;
    util::file_handle(filename_.c_str(), O_RDONLY).read_all(buffer);

    Mesh *mesh_ptr = model_ptr_->create_mesh();

    T *get = (T *) &buffer[0];

    std::vector<math::vec<3> > positions;
    std::vector<math::vec<3> > normals;
    std::vector<math::vec<2> > texcoords;
    std::vector<unsigned> indices;

    positions.resize(nrows * ncols);
    texcoords.resize(positions.size());
    indices.reserve((nrows - 1) * (ncols - 1) * 2 * 3);

	LOG_DEBUG("building vertex buffer...");
    for (size_t row = 0, i = 0; row < nrows; row++)
    {
        for (size_t col = 0; col < ncols; col++, i++, get++)
        {
            positions[i] = math::vec<3>(col * scale.x / ncols,
                *get * scale.y, row * scale.z / nrows);

            texcoords[i] = math::vec<2>(math::scalar(col) / math::scalar(ncols),
                math::scalar(row) / math::scalar(nrows));
        }
    }

	LOG_DEBUG("building index buffer...");
    for (size_t row = 1; row < nrows; row++)
    {
        for (size_t col = 1; col < ncols; col++)
        {
			indices.push_back((row - 1) * ncols + (col - 0));
            indices.push_back((row - 1) * ncols + (col - 1));
            indices.push_back((row - 0) * ncols + (col - 0));

			indices.push_back((row - 0) * ncols + (col - 1));
            indices.push_back((row - 0) * ncols + (col - 0));
            indices.push_back((row - 1) * ncols + (col - 1));
        }
    }

	LOG_DEBUG("building smooth normals...");
#if 0
    build_smooth_normals(positions, indices, &normals);
#else
    normals.resize(positions.size());
#endif

	LOG_DEBUG("building mesh...");

    mesh_ptr->set_vertices_count(positions.size());
    mesh_ptr->set_indices_count(indices.size());
    mesh_ptr->lock();
    mesh_ptr->set_positions(&positions[0]);
	mesh_ptr->set_normals(&normals[0]);
    mesh_ptr->set_texcoords(&texcoords[0]);
    mesh_ptr->set_face_indices(&indices[0]);
    mesh_ptr->unlock();

	LOG_DEBUG("setting material and texture (" << texture_name << ")...");
    std::auto_ptr<Material> material_ptr(new Material(renderer_ptr_));
    material_ptr->set_name(texture_name + " material");
    material_ptr->set_texture(texture_name);
    model_ptr_->materials.push_back(material_ptr);

	LOG_DEBUG("adding triangle lists and joints...");
    Mesh::triangles t;
    t.material_ptr = &model_ptr_->materials.back();
    t.first_index_offset = 0;
    t.triangles_count = indices.size() / 3;
    t.minimal_vertex_index = 0;
    t.vertices_count = positions.size();
    mesh_ptr->triangles_list.push_back(t);

	Joint *joint_ptr = model_ptr_->create_joint();
	joint_ptr->transforms.push_back(Joint::transform("tf", tf));
	joint_ptr->mesh_ptr = mesh_ptr;

	LOG_DEBUG("heightfield imported successfully");
}

template void HeightfieldLoader::load_from_raw<boost::int16_t>(math::matrix<4,4> const &tf, size_t nrows, size_t ncols,
    math::vec<3> const &scale, std::string const &texture_name);

template void HeightfieldLoader::load_from_raw<boost::uint16_t>(math::matrix<4,4> const &tf, size_t nrows, size_t ncols,
    math::vec<3> const &scale, std::string const &texture_name);

}
