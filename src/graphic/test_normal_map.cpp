//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
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
#include "renderer.h"

BOOST_AUTO_TEST_SUITE(normal_map)

BOOST_AUTO_TEST_CASE(basic)
{
	std::string const &src = util::get_graphic_sources_path();

	boost::shared_ptr<platform::Window> window(new platform::Window("test normal_map basic"));
	boost::shared_ptr<graphic::Renderer> renderer(new graphic::Renderer(window));

	renderer->postconstruct();
	renderer->reset_state();

	boost::shared_ptr<graphic::Material> material(new graphic::Material(renderer));
	material->set_diffuse_texture(src + "test_white.png");
	material->set_normal_map_texture(src + "test_particle.png");

	boost::shared_ptr<graphic::VertexArrayPTBNT> vertex_array = renderer->create_vertex_array<graphic::vertex_ptbnt>(6);

	{
		graphic::vertex_ptbnt *v = vertex_array->lock(true);

		v->position  .set(0, 0, 0);
		v->tangent   .set(1, 0, 0);
		v->binormal  .set(0, 1, 0);
		v->normal    .set(0, 0, 1);
		v->texcoords .set(0, 0);
		++v;

		v->position  .set(1, 1, 0);
		v->tangent   .set(1, 0, 0);
		v->binormal  .set(0, 1, 0);
		v->normal    .set(0, 0, 1);
		v->texcoords .set(1, 1);
		++v;

		v->position  .set(1, 0, 0);
		v->tangent   .set(1, 0, 0);
		v->binormal  .set(0, 1, 0);
		v->normal    .set(0, 0, 1);
		v->texcoords .set(1, 0);
		++v;

		v[0] = v[-3];
		++v;

		v->position  .set(0, 1, 0);
		v->tangent   .set(1, 0, 0);
		v->binormal  .set(0, 1, 0);
		v->normal    .set(0, 0, 1);
		v->texcoords .set(0, 1);
		++v;

		v[0] = v[-4];

		vertex_array->unlock();
	}

	graphic::Camera camera;
	camera.set_perspective_projection(math::PI / 2, renderer->get_screen_width() / renderer->get_screen_height(), 0, 2);
	camera.set_translation_rotation(math::vec<3>(0, 0, 1), math::vec<3>(math::PI / 3, 0, math::PI / 6));

	for (int i = 0; i < 1000 && window->run_message_loop(); i++)
	{
		renderer->disable_all_tests();

		renderer->begin_frame();
		renderer->set_camera(camera);
		material->select();
		renderer->draw_triangles(vertex_array.get(), 6);
		renderer->finish_frame();
	}
}

BOOST_AUTO_TEST_SUITE_END()
