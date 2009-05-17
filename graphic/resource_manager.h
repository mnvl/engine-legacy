#pragma once

#include <d3d9.h>
#include <boost/scoped_ptr.hpp>
#include <boost/intrusive_ptr.hpp>
#include <util/boost_com_intrusive_ptr.h>
#include <graphic/vertex.h>

namespace graphic
{

class font;
class material;
class renderer;
class shader;

class resource_manager {
public:
    resource_manager(renderer *renderer_ptr);
    ~resource_manager();

    font *get_default_font() const { return default_font_.get(); }
	material *get_default_material() const { return default_material_.get(); }

    template<class T>
    IDirect3DVertexDeclaration9 *get_vertex_decl() const;

    template<>
    IDirect3DVertexDeclaration9 *get_vertex_decl<vertex_pnt>() const
    {
        return pnt_decl_.get();
    }

    template<>
    IDirect3DVertexDeclaration9 *get_vertex_decl<vertex_pnjwt>() const
    {
        return pnjwt_decl_.get();
    }

private:
    renderer *renderer_ptr_;

	boost::scoped_ptr<font> default_font_;
	boost::scoped_ptr<material> default_material_;

    boost::intrusive_ptr<IDirect3DVertexDeclaration9> pnt_decl_;
    boost::intrusive_ptr<IDirect3DVertexDeclaration9> pnjwt_decl_;

    boost::scoped_ptr<shader> animated_model_shader_ptr_;
};

}
