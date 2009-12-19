#pragma once

#include <d3d9.h>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/intrusive_ptr.hpp>
#include <util/boost_com_intrusive_ptr.h>
#include <graphic/vertex.h>

namespace graphic
{

class Font;
class Material;
class Renderer;
class Shader;

class SharedResources {
public:
    SharedResources(Renderer * const renderer_ptr);
    ~SharedResources();

    template<class T>
    IDirect3DVertexDeclaration9 *get_vertex_decl() const;

    template<>
    IDirect3DVertexDeclaration9 *get_vertex_decl<vertex_pt>() const
    {
        return pt_decl_.get();
    }

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

	boost::shared_ptr<Font> get_default_font();
	boost::shared_ptr<Material> get_default_material();
	boost::shared_ptr<Shader> get_animated_model_shader();

private:
	Renderer * const renderer_ptr_;

    boost::intrusive_ptr<IDirect3DVertexDeclaration9> pt_decl_;
    boost::intrusive_ptr<IDirect3DVertexDeclaration9> pnt_decl_;
    boost::intrusive_ptr<IDirect3DVertexDeclaration9> pnjwt_decl_;

	boost::weak_ptr<Font> default_font_;
	boost::weak_ptr<Material> default_material_;
    boost::weak_ptr<Shader> animated_model_shader_ptr_;
};

}
