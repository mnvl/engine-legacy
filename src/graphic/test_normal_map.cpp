//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2011 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <boost/test/unit_test.hpp>
#include <platform/window.h>
#include <util/get_sources_path.h>
#include "material.h"
#include "mesh_shader.h"
#include "shaders_library.h"
#include "renderer.h"

BOOST_AUTO_TEST_SUITE(normal_map)

BOOST_AUTO_TEST_CASE(basic)
{
	std::string const &src = util::get_graphic_sources_path();

	boost::shared_ptr<platform::Window> window(new platform::Window("test normal_map basic"));
	graphic::RendererPtr renderer(new graphic::Renderer(window));

	renderer->postconstruct();
	renderer->reset_state();

	boost::shared_ptr<graphic::Material> material(new graphic::Material(renderer));
	material->set_diffuse_texture(src + "test_gray.png");
	material->set_normal_map_texture(src + "test_normal_map.jpeg");

	boost::shared_ptr<graphic::VertexArrayPTBNT> vertex_array = renderer->create_vertex_array<graphic::VERTEX_PTBNT>(6);

	{
		graphic::VERTEX_PTBNT *v = vertex_array->lock(true);

		v->position  .set(0, 1, 0);
		v->tangent   .set(1, 0, 0);
		v->binormal  .set(0, 1, 0);
		v->normal    .set(0, 0, 1);
		v->texcoords .set(0, 1);
		++v;

		v->position  .set(0, 0, 0);
		v->tangent   .set(1, 0, 0);
		v->binormal  .set(0, 1, 0);
		v->normal    .set(0, 0, 1);
		v->texcoords .set(0, 0);
		++v;

		v->position  .set(1, 0, 0);
		v->tangent   .set(1, 0, 0);
		v->binormal  .set(0, 1, 0);
		v->normal    .set(0, 0, 1);
		v->texcoords .set(1, 0);
		++v;

		v[0] = v[-1];
		++v;

		v->position  .set(1, 1, 0);
		v->tangent   .set(1, 0, 0);
		v->binormal  .set(0, 1, 0);
		v->normal    .set(0, 0, 1);
		v->texcoords .set(1, 1);
		++v;

		v[0] = v[-5];

		vertex_array->unlock();
	}

	boost::shared_ptr<graphic::MeshShader> shader = renderer->get_shaders_library()->get_mesh_shader();

	boost::shared_ptr<graphic::Camera> camera(new graphic::Camera());
	camera->set_orthographic_projection(2, 2 * math::scalar(renderer->get_screen_height()) / renderer->get_screen_width(), 0, 2);
	camera->set_eye_lookat_up(math::vec<3>(0.5f, 0.5f, 1), math::vec<3>(0.5f, 0.5f, 0), math::vec<3>(0, 1, 0));

	shader->tech_ptbnt->set();

	math::matrix<4,4> world_matrix;
	world_matrix.identity();

	shader->world_view_projection_matrix->set(camera->get_view_matrix() * camera->get_projection_matrix());
	shader->world_matrix->set(world_matrix);
	shader->diffuse_texture->set(material->get_diffuse_texture());
	shader->normal_map_texture->set(material->get_normal_map_texture());

	graphic::shader_parameters::POINT_LIGHTS lights;
	memset(&lights, 0, sizeof(lights));

	lights.items[0].position = math::vec<3>(0, 0, 10);
	lights.items[0].color = math::vec<3>(1, 1, 1);
	lights.items[0].inv_attenuation = 1.0f;

	lights.items[1].color = math::vec<3>(1, 1, 1);
	lights.items[1].inv_attenuation = 0.90f;

#pragma pack(push, 1)
	struct POINT_VERTEX
	{
		math::vec<3> position;
		math::vec<4> color;
	};
#pragma pack(pop)

	POINT_VERTEX point;
	point.color.set(1.0f, 1.0f, 0, 1.0f);

	HRESULT hr;

	for (int i = 0; i < 1000 && window->run_message_loop(); i++)
	{
		point.position.set(cos(math::scalar(i) / 100) * 0.5f + 0.5f, 0.5f, sin(math::scalar(i) / 100) * 0.5f);
		lights.items[1].position = point.position;
		shader->point_lights->set(lights);

		renderer->begin_frame();

		UINT passes_count;
		hr = shader->get()->Begin(&passes_count, 0);
		if (FAILED(hr)) throw exception::directx_error(hr);

		for(UINT pass = 0; pass < passes_count; pass++)
		{
			hr = shader->get()->BeginPass(pass);
			if (FAILED(hr)) throw exception::directx_error(hr);

			renderer->get_device()->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

			hr = shader->get()->EndPass();
			if (FAILED(hr)) throw exception::directx_error(hr);
		}

		hr = shader->get()->End();
		if (FAILED(hr)) throw exception::directx_error(hr);

		hr = renderer->get_device()->SetFVF(D3DFVF_XYZ|D3DFVF_DIFFUSE);
		if (FAILED(hr)) throw exception::directx_error(hr);

		float point_size = 32.0f;
		hr = renderer->get_device()->SetRenderState(D3DRS_POINTSIZE, *((DWORD*) &point_size));
		if (FAILED(hr)) throw exception::directx_error(hr);

		hr = renderer->get_device()->DrawPrimitiveUP(D3DPT_POINTLIST, 1, &point, sizeof(POINT_VERTEX));
		if (FAILED(hr)) throw exception::directx_error(hr);

		renderer->finish_frame();
	}
}

BOOST_AUTO_TEST_SUITE_END()
