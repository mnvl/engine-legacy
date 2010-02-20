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

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "vertex_declarations.h"
#include "shader.h"

namespace graphic
{

class Font;
class Shader;
class Material;
class Renderer;

class SharedResources {
public:
    SharedResources(Renderer * const renderer_ptr);
    ~SharedResources();

    template<class V>
    IDirect3DVertexDeclaration9 *get_vertex_decl()
	{
		return vertex_declarations_.get<V>();
	}

	boost::shared_ptr<Font> get_default_font();
	boost::shared_ptr<Material> get_default_material();
	boost::shared_ptr<Shader> get_animated_model_shader();

private:
	Renderer * const renderer_ptr_;

	VertexDeclarations vertex_declarations_;

	boost::weak_ptr<Font> default_font_;
	boost::weak_ptr<Material> default_material_;
	boost::weak_ptr<Shader> animated_model_shader_ptr_;
};

}
