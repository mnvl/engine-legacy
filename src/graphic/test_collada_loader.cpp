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
#include <util/get_sources_path.h>
#include "memory_model.h"
#include "collada_loader.h"

namespace
{

struct Fixture
{
    Fixture()
    {
        xercesc::XMLPlatformUtils::Initialize();
    }

    ~Fixture()
    {
        xercesc::XMLPlatformUtils::Terminate();
    }
};

}

BOOST_AUTO_TEST_SUITE(collada_loader)

BOOST_AUTO_TEST_CASE(plane)
{
	Fixture fixture;

	std::auto_ptr<graphic::MemoryModel> model_ptr(new graphic::MemoryModel(graphic::RendererPtr(), boost::shared_ptr<graphic::Scheduler>()));

    graphic::ColladaLoader loader(model_ptr.get());
	loader.load("test_collada_loader_plane.xml");
	loader.build_model();

    BOOST_ASSERT (model_ptr->meshes.size() == 1);
    BOOST_ASSERT (model_ptr->joints.size() == 1);
    BOOST_ASSERT (model_ptr->animations.size() == 0);

    BOOST_ASSERT (model_ptr->joints[0]->mesh_ptr.lock() == model_ptr->meshes[0]);

    graphic::MemoryMesh const *mesh_ptr = (graphic::MemoryMesh const *) model_ptr->meshes.front().get();
    BOOST_ASSERT (mesh_ptr->get_vertex_buffer().size() == 4);
    BOOST_ASSERT (mesh_ptr->get_index_buffer().size() == 6);

    BOOST_ASSERT ((mesh_ptr->get_vertex_buffer()[0].position - math::vec<3>(-0.5f, 0.0f,  0.5f)).length_sq() < math::EPSILON);
    BOOST_ASSERT ((mesh_ptr->get_vertex_buffer()[1].position - math::vec<3>( 0.5f, 0.0f,  0.5f)).length_sq() < math::EPSILON);
    BOOST_ASSERT ((mesh_ptr->get_vertex_buffer()[2].position - math::vec<3>(-0.5f, 0.0f, -0.5f)).length_sq() < math::EPSILON);
    BOOST_ASSERT ((mesh_ptr->get_vertex_buffer()[3].position - math::vec<3>( 0.5f, 0.0f, -0.5f)).length_sq() < math::EPSILON);

    BOOST_ASSERT (mesh_ptr->get_index_buffer()[0] == 0);
    BOOST_ASSERT (mesh_ptr->get_index_buffer()[1] == 1);
    BOOST_ASSERT (mesh_ptr->get_index_buffer()[2] == 2);
    BOOST_ASSERT (mesh_ptr->get_index_buffer()[3] == 2);
    BOOST_ASSERT (mesh_ptr->get_index_buffer()[4] == 1);
    BOOST_ASSERT (mesh_ptr->get_index_buffer()[5] == 3);
}

BOOST_AUTO_TEST_CASE(joint)
{
	Fixture fixture;

    std::auto_ptr<graphic::MemoryModel> model_ptr(new graphic::MemoryModel(graphic::RendererPtr(), boost::shared_ptr<graphic::Scheduler>()));

    graphic::ColladaLoader loader(model_ptr.get());
	loader.load("test_collada_loader_joint.xml");
	loader.build_model();

    BOOST_ASSERT (model_ptr->meshes.size() == 1);
    BOOST_ASSERT (model_ptr->joints.size() == 2);
    BOOST_ASSERT (model_ptr->animations.size() == 1);

    BOOST_ASSERT (!model_ptr->joints[0]->parent.lock());
    BOOST_ASSERT (!model_ptr->joints[1]->parent.lock());

    BOOST_ASSERT (model_ptr->joints[0]->mesh_ptr.lock() == model_ptr->meshes[0]);

    graphic::MemoryMesh *mesh_ptr = (graphic::MemoryMesh *) model_ptr->meshes.front().get();

    model_ptr->set_animation_time(0);
    std::vector<graphic::MemoryMesh::vertex_type> vertices1;
    mesh_ptr->transform_vertices(model_ptr->joints[0].get(), vertices1);

    BOOST_ASSERT (vertices1.size() == 3);
    BOOST_ASSERT (abs(vertices1[0].position.y) < 0.1f);

    model_ptr->set_animation_time(1);
    std::vector<graphic::MemoryMesh::vertex_type> vertices2;
    mesh_ptr->transform_vertices(model_ptr->joints[0].get(), vertices2);

    BOOST_ASSERT (vertices2.size() == 3);
    BOOST_ASSERT (abs(vertices2[0].position.y - 1.0f) < 0.1f);
}

BOOST_AUTO_TEST_CASE(animation)
{
	Fixture fixture;

    std::auto_ptr<graphic::MemoryModel> model_ptr(new graphic::MemoryModel(graphic::RendererPtr(), boost::shared_ptr<graphic::Scheduler>()));
    graphic::ColladaLoader loader(model_ptr.get());
	loader.load("test_collada_loader_animation.xml");
	loader.build_model();

    BOOST_ASSERT (model_ptr->meshes.size() == 1);
    BOOST_ASSERT (model_ptr->joints.size() == 4);
    BOOST_ASSERT (model_ptr->animations.size() == 1);

    BOOST_ASSERT (!model_ptr->joints[0]->parent.lock());
    BOOST_ASSERT (!model_ptr->joints[1]->parent.lock());
    BOOST_ASSERT (model_ptr->joints[2]->parent.lock() == model_ptr->joints[1]);
    BOOST_ASSERT (model_ptr->joints[3]->parent.lock() == model_ptr->joints[2]);

    BOOST_ASSERT (model_ptr->joints[0]->childs.size() == 0);

    BOOST_ASSERT (model_ptr->joints[1]->childs.size() == 1);
    BOOST_ASSERT (model_ptr->joints[1]->childs[0].lock() == model_ptr->joints[2]);

    BOOST_ASSERT (model_ptr->joints[2]->childs.size() == 1);
    BOOST_ASSERT (model_ptr->joints[2]->childs[0].lock() == model_ptr->joints[3]);

    BOOST_ASSERT (model_ptr->joints[3]->childs.size() == 0);

    graphic::MemoryMesh *mesh_ptr = (graphic::MemoryMesh *) model_ptr->meshes.front().get();
    BOOST_ASSERT (model_ptr->joints[0]->mesh_ptr.lock().get() == mesh_ptr);

    model_ptr->set_animation_time(0);
    std::vector<graphic::MemoryMesh::vertex_type> vertices1;
    mesh_ptr->transform_vertices(model_ptr->joints[0].get(), vertices1);

    model_ptr->set_animation_time(1);
    std::vector<graphic::MemoryMesh::vertex_type> vertices2;
    mesh_ptr->transform_vertices(model_ptr->joints[0].get(), vertices2);

    BOOST_ASSERT((vertices1[0].position - math::vec<3>(-1.0f, 0.85f, 1.0f)).length_sq() < 0.1f);
    BOOST_ASSERT((vertices2[0].position - math::vec<3>(-1.0f, 0.85f, 1.0f)).length_sq() < 0.1f);

    BOOST_ASSERT((vertices1[3].position - math::vec<3>(1.0f, 1.6f, -1.0f)).length_sq() < 0.1f);
    BOOST_ASSERT((vertices2[3].position - math::vec<3>(2.0f, 0.0f, -0.3f)).length_sq() < 0.1f);
}

BOOST_AUTO_TEST_CASE(materials)
{
	Fixture fixture;

    std::auto_ptr<graphic::MemoryModel> model_ptr(new graphic::MemoryModel(graphic::RendererPtr(), boost::shared_ptr<graphic::Scheduler>()));

    graphic::ColladaLoader loader(model_ptr.get());
	loader.load("test_collada_loader_materials.xml");
	loader.build_model();

    BOOST_ASSERT (model_ptr->meshes.size() == 2);
    BOOST_ASSERT (model_ptr->joints.size() == 2);
    BOOST_ASSERT (model_ptr->animations.size() == 0);

    BOOST_ASSERT (model_ptr->joints[0]->mesh_ptr.lock() == model_ptr->meshes[0]);
    BOOST_ASSERT (model_ptr->joints[1]->mesh_ptr.lock() == model_ptr->meshes[1]);

    graphic::Material const &m0 = *model_ptr->materials[0];
    BOOST_ASSERT (m0.get_name() == "blinn1");
    BOOST_ASSERT (m0.get_diffuse_texture()->get_filename().find("file2.tga") != std::string::npos);

    graphic::Material const &m1 = *model_ptr->materials[1];
    BOOST_ASSERT (m1.get_name() == "blinn2");
    BOOST_ASSERT (m1.get_diffuse_texture()->get_filename().find("file4.tga") != std::string::npos);

    graphic::Material const &m2 = *model_ptr->materials[2];
    BOOST_ASSERT (m2.get_name() == "lambert1");
    BOOST_ASSERT (m2.get_diffuse_texture()->get_filename().find("file1.tga") != std::string::npos);

    graphic::Material const &m3 = *model_ptr->materials[3];
    BOOST_ASSERT (m3.get_name() == "phong1");
    BOOST_ASSERT (m3.get_diffuse_texture()->get_filename().find("file3.tga") != std::string::npos);

    graphic::Material const &m4 = *model_ptr->materials[4];
    BOOST_ASSERT (m4.get_name() == "phong2");
    BOOST_ASSERT (m4.get_diffuse_texture()->get_filename().find("file5.tga") != std::string::npos);

    {
        graphic::MemoryMesh const *mesh1_ptr = (graphic::MemoryMesh const *) model_ptr->meshes[0].get();
        BOOST_ASSERT (mesh1_ptr->get_vertex_buffer().size() == 9);
        BOOST_ASSERT (mesh1_ptr->get_index_buffer().size() == 9);

        graphic::Mesh::Triangles const &t0 = mesh1_ptr->triangles_list[0];
        BOOST_ASSERT (t0.material_ptr->get_name() == "lambert1");
        BOOST_ASSERT (t0.triangles_count == 1);
        BOOST_ASSERT (t0.first_index_offset == 0);
        BOOST_ASSERT (t0.minimal_vertex_index == 0);
        BOOST_ASSERT (t0.vertices_count == 3);

        graphic::Mesh::Triangles const &t1 = mesh1_ptr->triangles_list[1];
        BOOST_ASSERT (t1.material_ptr->get_name() == "phong1");
        BOOST_ASSERT (t1.triangles_count == 1);
        BOOST_ASSERT (t1.first_index_offset == 3);
        BOOST_ASSERT (t1.minimal_vertex_index == 3);
        BOOST_ASSERT (t1.vertices_count == 3);

        graphic::Mesh::Triangles const &t2 = mesh1_ptr->triangles_list[2];
        BOOST_ASSERT (t2.material_ptr->get_name() == "blinn1");
        BOOST_ASSERT (t2.triangles_count == 1);
        BOOST_ASSERT (t2.first_index_offset == 6);
        BOOST_ASSERT (t2.minimal_vertex_index == 6);
        BOOST_ASSERT (t2.vertices_count == 3);
    }

    {
        graphic::MemoryMesh const *mesh2_ptr = (graphic::MemoryMesh const *) model_ptr->meshes[1].get();
        BOOST_ASSERT (mesh2_ptr->get_vertex_buffer().size() == 9);
        BOOST_ASSERT (mesh2_ptr->get_index_buffer().size() == 9);

        graphic::Mesh::Triangles const &t0 = mesh2_ptr->triangles_list[0];
        BOOST_ASSERT (t0.material_ptr->get_name() == "phong2");
        BOOST_ASSERT (t0.triangles_count == 1);
        BOOST_ASSERT (t0.first_index_offset == 0);
        BOOST_ASSERT (t0.minimal_vertex_index == 0);
        BOOST_ASSERT (t0.vertices_count == 3);

        graphic::Mesh::Triangles const &t1 = mesh2_ptr->triangles_list[1];
        BOOST_ASSERT (t1.material_ptr->get_name() == "blinn2");
        BOOST_ASSERT (t1.triangles_count == 1);
        BOOST_ASSERT (t1.first_index_offset == 3);
        BOOST_ASSERT (t1.minimal_vertex_index == 3);
        BOOST_ASSERT (t1.vertices_count == 3);

        graphic::Mesh::Triangles const &t2 = mesh2_ptr->triangles_list[2];
        BOOST_ASSERT (t2.material_ptr->get_name() == "lambert1");
        BOOST_ASSERT (t2.triangles_count == 1);
        BOOST_ASSERT (t2.first_index_offset == 6);
        BOOST_ASSERT (t2.minimal_vertex_index == 6);
        BOOST_ASSERT (t2.vertices_count == 3);
    }
}

BOOST_AUTO_TEST_CASE(tbn)
{
	Fixture fixture;

    std::auto_ptr<graphic::MemoryModel> model_ptr(new graphic::MemoryModel(graphic::RendererPtr(), boost::shared_ptr<graphic::Scheduler>()));

    graphic::ColladaLoader loader(model_ptr.get());
	loader.load("test_collada_loader_tbn.xml");
	loader.build_model();

	BOOST_ASSERT (model_ptr->meshes.size() == 1);
	
	graphic::MemoryMesh const *mesh_ptr = (graphic::MemoryMesh const *) model_ptr->meshes[0].get();
    BOOST_ASSERT (mesh_ptr->get_vertex_buffer().size() == 3);
    BOOST_ASSERT (mesh_ptr->get_index_buffer().size() == 3);

	std::vector<graphic::MemoryMesh::vertex_type> const &vertex_buffer = mesh_ptr->get_vertex_buffer();

	BOOST_ASSERT ((vertex_buffer[0].position - math::vec<3>(-0.5f, 0.0f, -0.5f)).length_sq() < math::EPSILON);
	BOOST_ASSERT ((vertex_buffer[0].normal   - math::vec<3>(0.0f, 1.0f, 0.0f)).length_sq() < math::EPSILON);
	BOOST_ASSERT ((vertex_buffer[0].tangent  - math::vec<3>(0, 0, 1)).length_sq() < math::EPSILON);
	BOOST_ASSERT ((vertex_buffer[0].binormal - math::vec<3>(1, 0, 0)).length_sq() < math::EPSILON);

	BOOST_ASSERT ((vertex_buffer[1].position - math::vec<3>(0.5f, 0.0f, 0.5f)).length_sq() < math::EPSILON);
	BOOST_ASSERT ((vertex_buffer[1].normal   - math::vec<3>(0.0f, 1.0f, 0.0f)).length_sq() < math::EPSILON);
	BOOST_ASSERT ((vertex_buffer[1].tangent  - math::vec<3>( 0.7071068f, 0, -0.7071068f)).length_sq() < math::EPSILON);
	BOOST_ASSERT ((vertex_buffer[1].binormal - math::vec<3>(-0.7071068f, 0, -0.7071068f)).length_sq() < math::EPSILON);

	BOOST_ASSERT ((vertex_buffer[2].position - math::vec<3>(0.5f, 0.0f, -0.5f)).length_sq() < math::EPSILON);
	BOOST_ASSERT ((vertex_buffer[2].normal   - math::vec<3>(0.0f, 1.0f, 0.0f)).length_sq() < math::EPSILON);
	BOOST_ASSERT ((vertex_buffer[2].tangent  - math::vec<3>(-1, 0, 0)).length_sq() < math::EPSILON);
	BOOST_ASSERT ((vertex_buffer[2].binormal - math::vec<3>(0, 0, 1)).length_sq() < math::EPSILON);

	std::vector<graphic::INDEX_U16> const &index_buffer = mesh_ptr->get_index_buffer();

	BOOST_ASSERT (index_buffer[0] == 0);
	BOOST_ASSERT (index_buffer[1] == 1);
	BOOST_ASSERT (index_buffer[2] == 2);
}

BOOST_AUTO_TEST_SUITE_END()
