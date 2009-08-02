#pragma once

#include <d3d9.h>
#include <boost/scoped_ptr.hpp>
#include <boost/intrusive_ptr.hpp>
#include <util/boost_com_intrusive_ptr.h>
#include <graphic/vertex.h>

namespace graphic
{

class Font;
class Material;
class Renderer;
class Shader;

class ResourceManager {
public:
    ResourceManager(Renderer *renderer_ptr);
    ~ResourceManager();

    Font *get_default_font() const { return default_font_.get(); }
	Material *get_default_material() const { return default_material_.get(); }

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

    Shader *get_animated_model_shader() const
    {
        return animated_model_shader_ptr_.get();
    }

private:
    Renderer *renderer_ptr_;

	boost::scoped_ptr<Font> default_font_;
	boost::scoped_ptr<Material> default_material_;

    boost::intrusive_ptr<IDirect3DVertexDeclaration9> pnt_decl_;
    boost::intrusive_ptr<IDirect3DVertexDeclaration9> pnjwt_decl_;

    boost::scoped_ptr<Shader> animated_model_shader_ptr_;
};

}
