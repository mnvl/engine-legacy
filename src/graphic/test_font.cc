//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2012 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <SDL.h>
#include <boost/test/unit_test.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <util/get_sources_path.h>
#include "test_fixture.h"
#include "font.h"
#include "material.h"
#include "vertex_array.h"
#include "index_array.h"
#include "uber_draw_call.h"
#include "d3d9_renderer.h"

BOOST_AUTO_TEST_SUITE(font)

BOOST_FIXTURE_TEST_CASE(make_draw_call, graphic::TestFixture)
{
	graphic::RendererPtr renderer(new graphic::D3D9Renderer());
	renderer->postconstruct();

	graphic::FontPtr font(new graphic::Font(renderer, "arial.ttf", 72));

	boost::shared_ptr<graphic::Camera> camera(new graphic::Camera());
	camera->set_orthographic_projection(2, 2 * math::scalar(renderer->get_screen_height()) / renderer->get_screen_width(), 0, 2);
	camera->set_eye_lookat_up(math::vec<3>(0.5f, 0.5f, 1), math::vec<3>(0.5f, 0.5f, 0), math::vec<3>(0, 1, 0));

	graphic::DrawCallPtr draw_call = font->make_draw_call(math::vec<3>(0, 1, 0), math::vec<3>(1, 1, 0),
		math::vec<3>(1, 0, 0), math::vec<3>(0, 0, 0), "hello", math::vec<4>(1, 1, 1, 0), math::vec<4>(0, 0.2, 0.2, 0));
	draw_call->set_camera(camera);
	draw_call->commit();

	boost::posix_time::ptime start_time(boost::posix_time::microsec_clock::local_time());
	math::scalar delta;
	math::scalar const DURATION = 5e6;

	while ((delta = math::scalar((boost::posix_time::microsec_clock::local_time() - start_time).total_microseconds())) < DURATION)
	{
		graphic::TestFixture::process_events();

		renderer->begin_frame();

		draw_call->execute();

		renderer->finish_frame();
	}
}

BOOST_AUTO_TEST_SUITE_END()
