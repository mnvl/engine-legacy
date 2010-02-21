#pragma once
//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <boost/noncopyable.hpp>
#include <math/obb.h>

namespace graphic
{

class Shader;
class VertexBuffer;
class IndexBuffer;
class Material;
class Camera;

class DrawCall: public boost::noncopyable {
public:
	DrawCall(boost::shared_ptr<Renderer> const &renderer);
	virtual ~DrawCall();

	void set_obb(math::obb<3> const &bounds);
	void set_vertex_buffer(boost::shared_ptr<VertexBuffer> const &vertex_buffer);
	void set_index_buffer(boost::shared_ptr<IndexBuffer> const &index_buffer);
	void set_material(boost::shared_ptr<Material> const &material);
	void set_minimal_vertex_index(size_t minimal_vertex_index);
	void set_vertices_count(size_t vertices_count);
	void set_first_index_offset(size_t first_index_offset);
	void set_triangles_count(size_t triangles_count);
	void set_world_matrix(math::matrix<4,4> const &world_matrix);
	void set_camera(boost::shared_ptr<Camera> const &camera);
	void commit();

	virtual void execute();

protected:
	boost::shared_ptr<Renderer> const renderer_;
	math::obb<3> bounds_;
	boost::shared_ptr<VertexBuffer> vertex_buffer_;
	boost::shared_ptr<IndexBuffer> index_buffer_;
	boost::shared_ptr<Material> material_;
	size_t minimal_vertex_index_, vertices_count_, first_index_offset_, triangles_count_;
	math::matrix<4,4> world_matrix_, view_matrix_, projection_matrix_;
};

}
