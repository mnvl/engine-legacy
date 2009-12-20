
#include <boost/test/unit_test.hpp>
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

std::string sources_directory;

void test_plane()
{
	Fixture dummy;

    std::auto_ptr<graphic::MemoryModel> model_ptr(new graphic::MemoryModel());

    graphic::ColladaLoader loader(model_ptr.get());
	loader.load(sources_directory + "/test_collada_loader_plane.xml");
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

void test_joint()
{
	Fixture dummy;

    std::auto_ptr<graphic::MemoryModel> model_ptr(new graphic::MemoryModel());

    graphic::ColladaLoader loader(model_ptr.get());
	loader.load(sources_directory + "/test_collada_loader_joint.xml");
	loader.build_model();

    BOOST_ASSERT (model_ptr->meshes.size() == 1);
    BOOST_ASSERT (model_ptr->joints.size() == 2);
    BOOST_ASSERT (model_ptr->animations.size() == 1);

    BOOST_ASSERT (!model_ptr->joints[0]->parent.lock());
    BOOST_ASSERT (!model_ptr->joints[1]->parent.lock());

    BOOST_ASSERT (model_ptr->joints[0]->mesh_ptr.lock() == model_ptr->meshes[0]);

    graphic::MemoryMesh *mesh_ptr = (graphic::MemoryMesh *) model_ptr->meshes.front().get();

    model_ptr->set_animation_time(0);
    std::vector<graphic::MemoryMesh::vertex_t> vertices1;
    mesh_ptr->transform_vertices(model_ptr->joints[0].get(), vertices1);

    BOOST_ASSERT (vertices1.size() == 3);
    BOOST_ASSERT (abs(vertices1[0].position.y) < 0.1f);

    model_ptr->set_animation_time(1);
    std::vector<graphic::MemoryMesh::vertex_t> vertices2;
    mesh_ptr->transform_vertices(model_ptr->joints[0].get(), vertices2);

    BOOST_ASSERT (vertices2.size() == 3);
    BOOST_ASSERT (abs(vertices2[0].position.y - 1.0f) < 0.1f);
}

void test_animation()
{
	Fixture dummy;

    std::auto_ptr<graphic::MemoryModel> model_ptr(new graphic::MemoryModel());
    graphic::ColladaLoader loader(model_ptr.get());
	loader.load(sources_directory + "/test_collada_loader_animation.xml");
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
    std::vector<graphic::MemoryMesh::vertex_t> vertices1;
    mesh_ptr->transform_vertices(model_ptr->joints[0].get(), vertices1);

    model_ptr->set_animation_time(1);
    std::vector<graphic::MemoryMesh::vertex_t> vertices2;
    mesh_ptr->transform_vertices(model_ptr->joints[0].get(), vertices2);

    BOOST_ASSERT((vertices1[0].position - math::vec<3>(-1.0f, 0.85f, 1.0f)).length_sq() < 0.1f);
    BOOST_ASSERT((vertices2[0].position - math::vec<3>(-1.0f, 0.85f, 1.0f)).length_sq() < 0.1f);

    BOOST_ASSERT((vertices1[3].position - math::vec<3>(1.0f, 1.6f, -1.0f)).length_sq() < 0.1f);
    BOOST_ASSERT((vertices2[3].position - math::vec<3>(2.0f, 0.0f, -0.3f)).length_sq() < 0.1f);
}

void test_materials()
{
	Fixture dummy;

    std::auto_ptr<graphic::MemoryModel> model_ptr(new graphic::MemoryModel());

    graphic::ColladaLoader loader(model_ptr.get());
	loader.load(sources_directory + "/test_collada_loader_materials.xml");
	loader.build_model();

    BOOST_ASSERT (model_ptr->meshes.size() == 2);
    BOOST_ASSERT (model_ptr->joints.size() == 2);
    BOOST_ASSERT (model_ptr->animations.size() == 0);

    BOOST_ASSERT (model_ptr->joints[0]->mesh_ptr.lock() == model_ptr->meshes[0]);
    BOOST_ASSERT (model_ptr->joints[1]->mesh_ptr.lock() == model_ptr->meshes[1]);

    graphic::Material const &m0 = *model_ptr->materials[0];
    BOOST_ASSERT (m0.get_name() == "blinn1");
    BOOST_ASSERT (m0.get_texture_name().find("file2.tga") != std::string::npos);

    graphic::Material const &m1 = *model_ptr->materials[1];
    BOOST_ASSERT (m1.get_name() == "blinn2");
    BOOST_ASSERT (m1.get_texture_name().find("file4.tga") != std::string::npos);

    graphic::Material const &m2 = *model_ptr->materials[2];
    BOOST_ASSERT (m2.get_name() == "lambert1");
    BOOST_ASSERT (m2.get_texture_name().find("file1.tga") != std::string::npos);

    graphic::Material const &m3 = *model_ptr->materials[3];
    BOOST_ASSERT (m3.get_name() == "phong1");
    BOOST_ASSERT (m3.get_texture_name().find("file3.tga") != std::string::npos);

    graphic::Material const &m4 = *model_ptr->materials[4];
    BOOST_ASSERT (m4.get_name() == "phong2");
    BOOST_ASSERT (m4.get_texture_name().find("file5.tga") != std::string::npos);

    {
        graphic::MemoryMesh const *mesh1_ptr = (graphic::MemoryMesh const *) model_ptr->meshes[0].get();
        BOOST_ASSERT (mesh1_ptr->get_vertex_buffer().size() == 9);
        BOOST_ASSERT (mesh1_ptr->get_index_buffer().size() == 9);

        graphic::Mesh::triangles const &t0 = mesh1_ptr->triangles_list[0];
        BOOST_ASSERT (t0.material_ptr->get_name() == "lambert1");
        BOOST_ASSERT (t0.triangles_count == 1);
        BOOST_ASSERT (t0.first_index_offset == 0);
        BOOST_ASSERT (t0.minimal_vertex_index == 0);
        BOOST_ASSERT (t0.vertices_count == 3);

        graphic::Mesh::triangles const &t1 = mesh1_ptr->triangles_list[1];
        BOOST_ASSERT (t1.material_ptr->get_name() == "phong1");
        BOOST_ASSERT (t1.triangles_count == 1);
        BOOST_ASSERT (t1.first_index_offset == 3);
        BOOST_ASSERT (t1.minimal_vertex_index == 3);
        BOOST_ASSERT (t1.vertices_count == 3);

        graphic::Mesh::triangles const &t2 = mesh1_ptr->triangles_list[2];
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

        graphic::Mesh::triangles const &t0 = mesh2_ptr->triangles_list[0];
        BOOST_ASSERT (t0.material_ptr->get_name() == "phong2");
        BOOST_ASSERT (t0.triangles_count == 1);
        BOOST_ASSERT (t0.first_index_offset == 0);
        BOOST_ASSERT (t0.minimal_vertex_index == 0);
        BOOST_ASSERT (t0.vertices_count == 3);

        graphic::Mesh::triangles const &t1 = mesh2_ptr->triangles_list[1];
        BOOST_ASSERT (t1.material_ptr->get_name() == "blinn2");
        BOOST_ASSERT (t1.triangles_count == 1);
        BOOST_ASSERT (t1.first_index_offset == 3);
        BOOST_ASSERT (t1.minimal_vertex_index == 3);
        BOOST_ASSERT (t1.vertices_count == 3);

        graphic::Mesh::triangles const &t2 = mesh2_ptr->triangles_list[2];
        BOOST_ASSERT (t2.material_ptr->get_name() == "lambert1");
        BOOST_ASSERT (t2.triangles_count == 1);
        BOOST_ASSERT (t2.first_index_offset == 6);
        BOOST_ASSERT (t2.minimal_vertex_index == 6);
        BOOST_ASSERT (t2.vertices_count == 3);
    }
}
}

boost::unit_test::test_suite*
init_unit_test_suite(int argc, char *argv[])
{
	BOOST_ASSERT (argc == 2);

	sources_directory = argv[1];
	boost::unit_test::framework::master_test_suite().p_name.value = "test_collada_loader";
	boost::unit_test::framework::master_test_suite().add(BOOST_TEST_CASE(&test_plane), 0, 0);
	boost::unit_test::framework::master_test_suite().add(BOOST_TEST_CASE(&test_joint), 0, 0);
	boost::unit_test::framework::master_test_suite().add(BOOST_TEST_CASE(&test_animation), 0, 0);
	boost::unit_test::framework::master_test_suite().add(BOOST_TEST_CASE(&test_materials), 0, 0);

    return 0;
}
