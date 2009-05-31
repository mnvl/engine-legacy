
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
void load_from_raw(size_t nrows, size_t ncols, math::vec<3> const &scale)
{
    std::vector<char> buffer;
    platform::file_handle(filename_).read_all(buffer);

    mesh *mesh_ptr = model_ptr_->create_mesh();

    T *get = (T *) &buffer[0];

    size_t cells_count = ;

    std::vector<math::vec<3> > positions;
    std::vector<math::vec<3> > normals;
    std::vector<math::vec<2> > texcoords;
    std::vector<unsigned> indices;

    positions.resize(nrows * ncols);
    texcoords.resize(positions.size());
    indices.reserve((nrows - 1) * (ncols - 1) * 2 * 3);

    for (size_t row = 0, i = 0; row < nros; row++)
    {
        for (size_t col = 0; col < ncols; col++, i++)
        {
            positions[i] = math::vec<3>(row * scale.x / nrows,
                *get * scale.y, col * scale.z / ncols);

            texcoords[i] = math::vec<2>(math::scalar(row) / math::scalar(nrows),
                math::scalar(col) / math::scalar(ncols));
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

    mesh_ptr->set_vertices_count(cells_count);
    mesh_ptr->set_indices_count(indices.size());
    mesh_ptr->lock();
    mesh_ptr->set_positions(&positions[0]);
    mesh_ptr->set_texcoords(&texcoords[0]);
    mesh_ptr->set_face_indices(&indices);
    mesh_ptr->unlock();
}

}
