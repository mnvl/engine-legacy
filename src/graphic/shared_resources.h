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

#include <d3d9.h>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/intrusive_ptr.hpp>
#include <util/boost_com_intrusive_ptr.h>
#include "vertex.h"

namespace graphic
{

class Font;
namespace shader { class Shader; }
class Material;
class Renderer;

class SharedResources {
public:
    SharedResources(Renderer * const renderer_ptr);
    ~SharedResources();

    template<class T>
    IDirect3DVertexDeclaration9 *get_vertex_decl() const;

    template<>
    IDirect3DVertexDeclaration9 *get_vertex_decl<VERTEX_PT>() const
    {
        return pt_decl_.get();
    }

    template<>
    IDirect3DVertexDeclaration9 *get_vertex_decl<VERTEX_PTBNT>() const
    {
        return ptbnt_decl_.get();
    }

    template<>
    IDirect3DVertexDeclaration9 *get_vertex_decl<VERTEX_PNT>() const
    {
        return pnt_decl_.get();
    }

    template<>
    IDirect3DVertexDeclaration9 *get_vertex_decl<VERTEX_PNJWT>() const
    {
        return pnjwt_decl_.get();
    }

	boost::shared_ptr<Font> get_default_font();
	boost::shared_ptr<Material> get_default_material();
	boost::shared_ptr<shader::Shader> get_animated_model_shader();

private:
	Renderer * const renderer_ptr_;

    boost::intrusive_ptr<IDirect3DVertexDeclaration9> pt_decl_;
    boost::intrusive_ptr<IDirect3DVertexDeclaration9> pnt_decl_;
    boost::intrusive_ptr<IDirect3DVertexDeclaration9> ptbnt_decl_;
    boost::intrusive_ptr<IDirect3DVertexDeclaration9> pnjwt_decl_;

	boost::weak_ptr<Font> default_font_;
	boost::weak_ptr<Material> default_material_;
	boost::weak_ptr<shader::Shader> animated_model_shader_ptr_;
};

}
