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
#include "font.h"
#include "renderer.h"

BOOST_AUTO_TEST_SUITE(window_resize)

BOOST_AUTO_TEST_CASE(basic)
{
	boost::shared_ptr<platform::Window> window(new platform::Window("test_window_resize"));
	boost::shared_ptr<graphic::Renderer> renderer(new graphic::Renderer(window));

	for (int i = 0; i < 10 && window->run_message_loop(); i++)
	{
		if (i == 5) MoveWindow(window->get_handle(), 100, 100, 900, 700, TRUE);

		renderer->begin_frame();
		renderer->finish_frame();
	}
}

BOOST_AUTO_TEST_CASE(with_postconstruct)
{
	boost::shared_ptr<platform::Window> window(new platform::Window("test_window_resize"));

	boost::shared_ptr<graphic::Renderer> renderer(new graphic::Renderer(window));
	renderer->postconstruct();

	for (int i = 0; i < 10 && window->run_message_loop(); i++)
	{
		if (i == 5) MoveWindow(window->get_handle(), 100, 100, 900, 700, TRUE);

		renderer->begin_frame();
		renderer->finish_frame();
	}
}

BOOST_AUTO_TEST_CASE(with_postconstruct_and_vertex_array)
{
	boost::shared_ptr<platform::Window> window(new platform::Window("test_window_resize"));

	boost::shared_ptr<graphic::Renderer> renderer(new graphic::Renderer(window));
	renderer->postconstruct();

	boost::shared_ptr<graphic::VertexArrayPT> vertex_array1;
	boost::shared_ptr<graphic::VertexArrayPT> vertex_array2;
	boost::shared_ptr<graphic::VertexArrayPT> vertex_array3;
	boost::shared_ptr<graphic::VertexArrayPT> vertex_array4;

	for (int i = 0; i < 20 && window->run_message_loop(); i++)
	{
		if (i == 5)
		{
			vertex_array1 = renderer->create_vertex_array<graphic::VERTEX_PT>(100, false, false);
			vertex_array2 = renderer->create_vertex_array<graphic::VERTEX_PT>(100, false, true);
			vertex_array3 = renderer->create_vertex_array<graphic::VERTEX_PT>(100, true, false);
			vertex_array4 = renderer->create_vertex_array<graphic::VERTEX_PT>(100, true, true);
		}

		if (i == 15)
		{
			MoveWindow(window->get_handle(), 100, 100, 900, 700, TRUE);
		}

		renderer->begin_frame();
		renderer->finish_frame();
	}
}

BOOST_AUTO_TEST_CASE(with_postconstruct_and_index_array)
{
	boost::shared_ptr<platform::Window> window(new platform::Window("test_window_resize"));

	boost::shared_ptr<graphic::Renderer> renderer(new graphic::Renderer(window));
	renderer->postconstruct();

	boost::shared_ptr<graphic::IndexArray16> index_array1;
	boost::shared_ptr<graphic::IndexArray16> index_array2;
	boost::shared_ptr<graphic::IndexArray16> index_array3;
	boost::shared_ptr<graphic::IndexArray16> index_array4;

	for (int i = 0; i < 20 && window->run_message_loop(); i++)
	{
		if (i == 5)
		{
			index_array1 = renderer->create_index_array<graphic::index_t>(100, false, false);
			index_array2 = renderer->create_index_array<graphic::index_t>(100, false, true);
			index_array3 = renderer->create_index_array<graphic::index_t>(100, true, false);
			index_array4 = renderer->create_index_array<graphic::index_t>(100, true, true);
		}

		if (i == 15)
		{
			MoveWindow(window->get_handle(), 100, 100, 900, 700, TRUE);
		}

		renderer->begin_frame();
		renderer->finish_frame();
	}
}

BOOST_AUTO_TEST_CASE(with_postconstruct_and_texture)
{
	boost::shared_ptr<platform::Window> window(new platform::Window("test_window_resize"));

	boost::shared_ptr<graphic::Renderer> renderer(new graphic::Renderer(window));
	renderer->postconstruct();

	boost::shared_ptr<graphic::Texture> texture;

	for (int i = 0; i < 20 && window->run_message_loop(); i++)
	{
		if (i == 5)
		{
			texture = renderer->create_texture(util::get_graphic_sources_path() + "test_white.png");
		}

		if (i == 15)
		{
			MoveWindow(window->get_handle(), 100, 100, 900, 700, TRUE);
		}

		renderer->begin_frame();
		renderer->finish_frame();
	}
}

BOOST_AUTO_TEST_CASE(with_postconstruct_and_font)
{
	boost::shared_ptr<platform::Window> window(new platform::Window("test_window_resize"));

	boost::shared_ptr<graphic::Renderer> renderer(new graphic::Renderer(window));
	renderer->postconstruct();

	boost::shared_ptr<graphic::Font> font;

	for (int i = 0; i < 20 && window->run_message_loop(); i++)
	{
		if (i == 5)
		{
			font = renderer->get_shared_resources()->get_default_font();
		}

		if (i == 15)
		{
			MoveWindow(window->get_handle(), 100, 100, 900, 700, TRUE);
		}

		renderer->begin_frame();

		if (font)
		{
			font->draw(math::vec<2>(0, 0), math::vec<2>(100, 100), "hi!",
				graphic::Font::FLAG_ALIGN_LEFT, math::vec<3>(1,1,1));
		}

		renderer->finish_frame();
	}
}

BOOST_AUTO_TEST_SUITE_END()
