//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <boost/cstdint.hpp>
#include <util/file_handle.h>
#include <util/logger.h>
#include <graphic/build_smooth_normals.h>
#include <graphic/heightfield_loader.h>

namespace graphic
{

HeightfieldLoader::HeightfieldLoader(std::string const &filename, Model *model_ptr,
	boost::shared_ptr<Renderer> const &renderer_ptr)
:
    filename_(filename),
    model_(model_ptr),
    renderer_(renderer_ptr)
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
    util::FileHandle(filename_.c_str(), O_RDONLY).read_all(buffer);

	MeshPtr mesh_ptr = model_->add_mesh();

    T *get = (T *) &buffer[0];

    std::vector<GenericVertex> vertices;
    std::vector<unsigned> indices;

    vertices.resize(nrows * ncols);
    indices.reserve((nrows - 1) * (ncols - 1) * 2 * 3);

	LOG_DEBUG("building vertex buffer...");
    for (size_t row = 0, i = 0; row < nrows; row++)
    {
        for (size_t col = 0; col < ncols; col++, i++, get++)
        {
			GenericVertex *vertex = &vertices[i];
            vertex->position.set(col * scale.x / ncols, *get * scale.y, row * scale.z / nrows);
            vertex->texcoords.set(math::scalar(col) / math::scalar(ncols), math::scalar(row) / math::scalar(nrows));
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
    build_smooth_normals(vertices, indices);

	LOG_DEBUG("building mesh...");
	mesh_ptr->set_vertices(&vertices[0], GenericVertex::FORMAT_POSITION | GenericVertex::FORMAT_NORMAL |
		GenericVertex::FORMAT_TEXCOORDS, vertices.size());
    mesh_ptr->set_indices(&indices[0], indices.size());

	LOG_DEBUG("setting material and texture (" << texture_name << ")...");
    MaterialPtr material_ptr(new Material(renderer_));
    material_ptr->set_name(texture_name + " material");
    material_ptr->set_diffuse_texture(texture_name);
    model_->materials.push_back(material_ptr);

	LOG_DEBUG("adding triangle lists and joints...");
    Mesh::Triangles t;
    t.material_ptr = model_->materials.back();
    t.first_index_offset = 0;
    t.triangles_count = indices.size() / 3;
    t.minimal_vertex_index = 0;
    t.vertices_count = vertices.size();
    mesh_ptr->triangles_list.push_back(t);

	JointPtr joint_ptr = model_->add_joint();
	joint_ptr->transforms.push_back(Joint::transform("tf", tf));
	joint_ptr->mesh_ptr = mesh_ptr;

	LOG_DEBUG("heightfield imported successfully");
}

template void HeightfieldLoader::load_from_raw<boost::int16_t>(math::matrix<4,4> const &tf, size_t nrows, size_t ncols,
    math::vec<3> const &scale, std::string const &texture_name);

template void HeightfieldLoader::load_from_raw<boost::uint16_t>(math::matrix<4,4> const &tf, size_t nrows, size_t ncols,
    math::vec<3> const &scale, std::string const &texture_name);

}
