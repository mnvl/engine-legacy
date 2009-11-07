
#include <d3d9.h>
#include <vector>
#include <graphic/shader.h>
#include <graphic/animated_model.h>
#include <graphic/renderer.h>
#include <graphic/animated_mesh.h>

namespace graphic
{

AnimatedMesh::AnimatedMesh(Model *model_ptr):
    PnjwtStaticMesh(model_ptr)
{
}

AnimatedModel *AnimatedMesh::get_model()
{
    return (AnimatedModel *) Mesh::get_model();
}

void AnimatedMesh::draw(Joint *joint_ptr)
{
    HRESULT hr;

    Renderer *renderer_ptr = get_model()->get_renderer();
    ResourceManager *resource_manager_ptr = renderer_ptr->get_resource_manager();
	IDirect3DDevice9 *device_ptr = renderer_ptr->get_device();
    Shader *shader_ptr = resource_manager_ptr->get_animated_model_shader();

	math::matrix<4,4> view_projection_matrix =
		joint_ptr->matrix *
        renderer_ptr->get_world_matrix() *
        renderer_ptr->get_view_matrix() *
        renderer_ptr->get_projection_matrix();

	hr = shader_ptr->get()->SetMatrix("view_projection_matrix", (D3DXMATRIX const *) &view_projection_matrix);
	if (FAILED(hr)) throw exception::directx_error(hr);

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

	if (joint_matrix_array.empty())
	{
	    hr = shader_ptr->get()->SetMatrixArray("joint_matrix_array", (D3DXMATRIX const *) 0, 0);
	}
	else
	{
	    hr = shader_ptr->get()->SetMatrixArray("joint_matrix_array", (D3DXMATRIX const *) &joint_matrix_array[0], joint_matrix_array.size());
	}

	if (FAILED(hr)) throw exception::directx_error(hr);

	hr = device_ptr->SetVertexDeclaration(resource_manager_ptr->get_vertex_decl<vertex_t>());
	if (FAILED(hr)) throw exception::directx_error(hr);

    hr = device_ptr->SetStreamSource(0, vertex_buffer_.get(), 0, sizeof(vertex_t));
	if (FAILED(hr)) throw exception::directx_error(hr);

    hr = device_ptr->SetIndices(index_buffer_.get());
	if (FAILED(hr)) throw exception::directx_error(hr);

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
