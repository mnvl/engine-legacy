
#include <d3d9.h>
#include <vector>
#include <graphic/shader.h>
#include <graphic/animated_model.h>
#include <graphic/renderer.h>
#include <graphic/animated_mesh.h>

namespace graphic
{

animated_mesh::animated_mesh(model *model_ptr):
    pnjwt_static_mesh(model_ptr)
{
}

animated_model *animated_mesh::get_model()
{
    return (animated_model *) mesh::get_model();
}

void animated_mesh::draw(joint *joint_ptr)
{
    HRESULT hr;

    renderer *renderer_ptr = get_model()->get_renderer();
    resource_manager *resource_manager_ptr = renderer_ptr->get_resource_manager();
	IDirect3DDevice9 *device_ptr = renderer_ptr->get_device();
    shader *shader_ptr = resource_manager_ptr->get_animated_model_shader();

	math::matrix<4,4> view_projection_matrix = renderer_ptr->get_view_matrix() * renderer_ptr->get_projection_matrix();
	shader_ptr->get()->SetMatrix("view_projection_matrix", (D3DXMATRIX const *) &view_projection_matrix);

    std::vector<math::matrix<4,4> > joint_matrix_array;
    joint_matrix_array.reserve(joint_index_transform.size());

    for (joint_index_transform_type::const_iterator it = joint_index_transform.begin();
         it != joint_index_transform.end(); it++)
    {
		math::matrix<4,4> t = bind_shape_matrix *
			inverted_bind_matrices[*it] *
			get_model()->joints[*it].matrix;

        joint_matrix_array.push_back(t);
    }

    hr = shader_ptr->get()->SetMatrixArray("joint_matrix_array", (D3DXMATRIX const *) &joint_matrix_array[0], joint_matrix_array.size());
    if (FAILED(hr)) throw exception::directx_error(hr);

	device_ptr->SetVertexDeclaration(resource_manager_ptr->get_vertex_decl<vertex_t>());
    device_ptr->SetStreamSource(0, vertex_buffer_.get(), 0, sizeof(vertex_t));

    device_ptr->SetIndices(index_buffer_.get());

    UINT passes_count;
    hr = shader_ptr->get()->Begin(&passes_count, D3DXFX_DONOTSAVESTATE);
    if (FAILED(hr)) throw exception::directx_error(hr);

    for(UINT pass = 0; pass < passes_count; pass++ )
    {
        hr = shader_ptr->get()->BeginPass(pass);
        if (FAILED(hr)) throw exception::directx_error(hr);

        draw_triangles_list();

        hr = shader_ptr->get()->EndPass();
        if (FAILED(hr)) throw exception::directx_error(hr);
    }

    hr = shader_ptr->get()->End();
    if (FAILED(hr)) throw exception::directx_error(hr);

    hr = device_ptr->SetVertexShader(0);
    if (FAILED(hr)) throw exception::directx_error(hr);
}

}
