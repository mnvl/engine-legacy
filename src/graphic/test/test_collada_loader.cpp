
#define BOOST_TEST_MODULE test_collada_loader
#include <boost/test/included/unit_test.hpp>
#include <graphic/memory_model.h>
#include <graphic/collada_loader.h>

BOOST_AUTO_TEST_SUITE (test_collada_loader)

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

BOOST_FIXTURE_TEST_CASE (test_plane, Fixture)
{
    std::auto_ptr<graphic::memory_model> model_ptr(new graphic::memory_model());
    graphic::collada_loader("test_collada_loader_plane.xml", model_ptr.get()).load();

    BOOST_ASSERT (model_ptr->meshes.size() == 1);
    BOOST_ASSERT (model_ptr->joints.size() == 1);
    BOOST_ASSERT (model_ptr->animations.size() == 0);

    BOOST_ASSERT (model_ptr->joints[0].mesh_ptr == &model_ptr->meshes[0]);

    graphic::memory_mesh const *mesh_ptr = (graphic::memory_mesh const *) &model_ptr->meshes.front();
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

BOOST_FIXTURE_TEST_CASE (test_joint, Fixture)
{
    std::auto_ptr<graphic::memory_model> model_ptr(new graphic::memory_model());
    graphic::collada_loader("test_collada_loader_joint.xml", model_ptr.get()).load();

    BOOST_ASSERT (model_ptr->meshes.size() == 1);
    BOOST_ASSERT (model_ptr->joints.size() == 2);
    BOOST_ASSERT (model_ptr->animations.size() == 1);

    BOOST_ASSERT (model_ptr->joints[0].parent == 0);
    BOOST_ASSERT (model_ptr->joints[1].parent == 0);

    BOOST_ASSERT (model_ptr->joints[0].mesh_ptr == (graphic::mesh *) &model_ptr->meshes[0]);

    graphic::memory_mesh *mesh_ptr = (graphic::memory_mesh *) &model_ptr->meshes.front();

    model_ptr->set_animation_time(0);
    std::vector<graphic::memory_mesh::vertex_t> vertices1;
    mesh_ptr->transform_vertices(&model_ptr->joints[0], vertices1);

    BOOST_ASSERT (vertices1.size() == 3);
    BOOST_ASSERT (abs(vertices1[0].position.y) < 0.1f);

    model_ptr->set_animation_time(1);
    std::vector<graphic::memory_mesh::vertex_t> vertices2;
    mesh_ptr->transform_vertices(&model_ptr->joints[0], vertices2);

    BOOST_ASSERT (vertices2.size() == 3);
    BOOST_ASSERT (abs(vertices2[0].position.y - 1.0f) < 0.1f);
}

BOOST_FIXTURE_TEST_CASE (test_animation, Fixture)
{
    std::auto_ptr<graphic::memory_model> model_ptr(new graphic::memory_model());
    graphic::collada_loader("test_collada_loader_animation.xml", model_ptr.get()).load();

    BOOST_ASSERT (model_ptr->meshes.size() == 1);
    BOOST_ASSERT (model_ptr->joints.size() == 4);
    BOOST_ASSERT (model_ptr->animations.size() == 1);

    BOOST_ASSERT (model_ptr->joints[0].parent == 0);
    BOOST_ASSERT (model_ptr->joints[1].parent == 0);
    BOOST_ASSERT (model_ptr->joints[2].parent == &model_ptr->joints[1]);
    BOOST_ASSERT (model_ptr->joints[3].parent == &model_ptr->joints[2]);

    BOOST_ASSERT (model_ptr->joints[0].childs.size() == 0);

    BOOST_ASSERT (model_ptr->joints[1].childs.size() == 1);
    BOOST_ASSERT (model_ptr->joints[1].childs[0] == &model_ptr->joints[2]);

    BOOST_ASSERT (model_ptr->joints[2].childs.size() == 1);
    BOOST_ASSERT (model_ptr->joints[2].childs[0] == &model_ptr->joints[3]);

    BOOST_ASSERT (model_ptr->joints[3].childs.size() == 0);

    graphic::memory_mesh *mesh_ptr = (graphic::memory_mesh *) &model_ptr->meshes.front();
    BOOST_ASSERT (model_ptr->joints[0].mesh_ptr == mesh_ptr);

    model_ptr->set_animation_time(0);
    std::vector<graphic::memory_mesh::vertex_t> vertices1;
    mesh_ptr->transform_vertices(&model_ptr->joints[0], vertices1);

    model_ptr->set_animation_time(1);
    std::vector<graphic::memory_mesh::vertex_t> vertices2;
    mesh_ptr->transform_vertices(&model_ptr->joints[0], vertices2);

    BOOST_ASSERT((vertices1[0].position - math::vec<3>(-1.0f, 0.85f, 1.0f)).length_sq() < 0.1f);
    BOOST_ASSERT((vertices2[0].position - math::vec<3>(-1.0f, 0.85f, 1.0f)).length_sq() < 0.1f);

    BOOST_ASSERT((vertices1[3].position - math::vec<3>(1.0f, 1.6f, -1.0f)).length_sq() < 0.1f);
    BOOST_ASSERT((vertices2[3].position - math::vec<3>(2.0f, 0.0f, -0.3f)).length_sq() < 0.1f);
}

BOOST_FIXTURE_TEST_CASE (test_materials, Fixture)
{
    std::auto_ptr<graphic::memory_model> model_ptr(new graphic::memory_model());
    graphic::collada_loader("test_collada_loader_materials.xml", model_ptr.get()).load();

    BOOST_ASSERT (model_ptr->meshes.size() == 2);
    BOOST_ASSERT (model_ptr->joints.size() == 2);
    BOOST_ASSERT (model_ptr->animations.size() == 0);

    BOOST_ASSERT (model_ptr->joints[0].mesh_ptr == &model_ptr->meshes[0]);
    BOOST_ASSERT (model_ptr->joints[1].mesh_ptr == &model_ptr->meshes[1]);

    graphic::material const &m0 = model_ptr->materials[0];
    BOOST_ASSERT (m0.get_name() == "blinn1");
    BOOST_ASSERT (m0.get_texture_name() == "file2.tga");

    graphic::material const &m1 = model_ptr->materials[1];
    BOOST_ASSERT (m1.get_name() == "blinn2");
    BOOST_ASSERT (m1.get_texture_name() == "file4.tga");

    graphic::material const &m2 = model_ptr->materials[2];
    BOOST_ASSERT (m2.get_name() == "lambert1");
    BOOST_ASSERT (m2.get_texture_name() == "file1.tga");

    graphic::material const &m3 = model_ptr->materials[3];
    BOOST_ASSERT (m3.get_name() == "phong1");
    BOOST_ASSERT (m3.get_texture_name() == "file3.tga");

    graphic::material const &m4 = model_ptr->materials[4];
    BOOST_ASSERT (m4.get_name() == "phong2");
    BOOST_ASSERT (m4.get_texture_name() == "file5.tga");

    {
        graphic::memory_mesh const *mesh1_ptr = (graphic::memory_mesh const *) &model_ptr->meshes[0];
        BOOST_ASSERT (mesh1_ptr->get_vertex_buffer().size() == 9);
        BOOST_ASSERT (mesh1_ptr->get_index_buffer().size() == 9);

        graphic::mesh::triangles const &t0 = mesh1_ptr->triangles_list[0];
        BOOST_ASSERT (t0.material_ptr->get_name() == "lambert1");
        BOOST_ASSERT (t0.triangles_count == 1);
        BOOST_ASSERT (t0.first_index_offset == 0);
        BOOST_ASSERT (t0.minimal_vertex_index == 0);
        BOOST_ASSERT (t0.vertices_count == 3);

        graphic::mesh::triangles const &t1 = mesh1_ptr->triangles_list[1];
        BOOST_ASSERT (t1.material_ptr->get_name() == "phong1");
        BOOST_ASSERT (t1.triangles_count == 1);
        BOOST_ASSERT (t1.first_index_offset == 3);
        BOOST_ASSERT (t1.minimal_vertex_index == 3);
        BOOST_ASSERT (t1.vertices_count == 3);

        graphic::mesh::triangles const &t2 = mesh1_ptr->triangles_list[2];
        BOOST_ASSERT (t2.material_ptr->get_name() == "blinn1");
        BOOST_ASSERT (t2.triangles_count == 1);
        BOOST_ASSERT (t2.first_index_offset == 6);
        BOOST_ASSERT (t2.minimal_vertex_index == 6);
        BOOST_ASSERT (t2.vertices_count == 3);
    }

    {
        graphic::memory_mesh const *mesh2_ptr = (graphic::memory_mesh const *) &model_ptr->meshes[1];
        BOOST_ASSERT (mesh2_ptr->get_vertex_buffer().size() == 9);
        BOOST_ASSERT (mesh2_ptr->get_index_buffer().size() == 9);

        graphic::mesh::triangles const &t0 = mesh2_ptr->triangles_list[0];
        BOOST_ASSERT (t0.material_ptr->get_name() == "phong2");
        BOOST_ASSERT (t0.triangles_count == 1);
        BOOST_ASSERT (t0.first_index_offset == 0);
        BOOST_ASSERT (t0.minimal_vertex_index == 0);
        BOOST_ASSERT (t0.vertices_count == 3);

        graphic::mesh::triangles const &t1 = mesh2_ptr->triangles_list[1];
        BOOST_ASSERT (t1.material_ptr->get_name() == "blinn2");
        BOOST_ASSERT (t1.triangles_count == 1);
        BOOST_ASSERT (t1.first_index_offset == 3);
        BOOST_ASSERT (t1.minimal_vertex_index == 3);
        BOOST_ASSERT (t1.vertices_count == 3);

        graphic::mesh::triangles const &t2 = mesh2_ptr->triangles_list[2];
        BOOST_ASSERT (t2.material_ptr->get_name() == "lambert1");
        BOOST_ASSERT (t2.triangles_count == 1);
        BOOST_ASSERT (t2.first_index_offset == 6);
        BOOST_ASSERT (t2.minimal_vertex_index == 6);
        BOOST_ASSERT (t2.vertices_count == 3);
    }
}

BOOST_AUTO_TEST_SUITE_END()
