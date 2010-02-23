//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <d3d9.h>
#include <vector>
#include "mesh_shader.h"
#include "mesh_draw_call.h"
#include "animated_model.h"
#include "renderer.h"
#include "scheduler.h"
#include "animated_mesh.h"

namespace graphic
{

AnimatedMesh::AnimatedMesh(Model *model_ptr):
    PnjwtStaticMesh(model_ptr)
{
}

MeshPtr AnimatedMesh::shallow_copy() const
{
	return MeshPtr(new AnimatedMesh(*this));
}

AnimatedModel *AnimatedMesh::get_model()
{
    return (AnimatedModel *) Mesh::get_model();
}

void AnimatedMesh::draw(Joint *joint_ptr)
{
	math::matrix<4,4> world_matrix = joint_ptr->matrix * get_model()->get_renderer()->get_world_matrix();

    std::vector<math::matrix<4,4> > joint_matrix_array;
    joint_matrix_array.reserve(joint_index_transform.size());

    for (joint_index_transform_type::const_iterator it = joint_index_transform.begin();
         it != joint_index_transform.end(); it++)
    {
		math::matrix<4,4> t = bind_shape_matrix *
			inverted_bind_matrices[*it] *
			get_model()->joints[*it]->matrix;

        joint_matrix_array.push_back(t);
    }

	for (triangles_list_type::const_iterator it = triangles_list.begin(); it != triangles_list.end(); it++)
	{
		boost::shared_ptr<MeshDrawCall> draw_call(new MeshDrawCall(get_model()->get_renderer(), shader_));

		{
			math::obb<3> bounds(bounds_);
			bounds.transform(joint_ptr->matrix);
			draw_call->set_obb(bounds);
		}

		draw_call->set_vertex_buffer(vertex_buffer_);
		draw_call->set_index_buffer(index_buffer_);
		draw_call->set_material(it->material_ptr);
		draw_call->set_minimal_vertex_index(it->minimal_vertex_index);
		draw_call->set_vertices_count(it->vertices_count);
		draw_call->set_first_index_offset(it->first_index_offset);
		draw_call->set_triangles_count(it->triangles_count);
		draw_call->set_world_matrix(world_matrix);
		draw_call->set_joint_matrix_array(joint_matrix_array);
		draw_call->commit();

		get_model()->get_scheduler()->add(draw_call);
	}
}

}
