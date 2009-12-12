
#include <luabind/luabind.hpp>
#include <exception/directx_error.h>
#include "renderer.h"
#include "vertex_array.h"

namespace graphic
{

VertexBuffer::VertexBuffer(boost::shared_ptr<Renderer> const &renderer, size_t size,
	bool dynamic, bool writeonly)
:
	renderer_(renderer)
{
	DWORD usage = 0;
	if(dynamic) usage |= D3DUSAGE_DYNAMIC;
	if(writeonly) usage |= D3DUSAGE_WRITEONLY;

	D3DPOOL pool = dynamic ? D3DPOOL_DEFAULT : D3DPOOL_MANAGED;

	IDirect3DVertexBuffer9 *buffer;
	HRESULT hr = renderer->get_device()->CreateVertexBuffer(size, usage, 0, pool, &buffer, 0);
	if(FAILED(hr)) throw exception::directx_error(hr);

	buffer_.reset(buffer);
	buffer->Release();
}

VertexBuffer::~VertexBuffer()
{
}

void *VertexBuffer::lock()
{
	HRESULT hr = buffer_->Lock(0, 0, (void **) &begin_, 0);
	if(FAILED(hr)) throw exception::directx_error(hr);

	return begin_;
}

const void *VertexBuffer::lock() const
{
	HRESULT hr = buffer_->Lock(0, 0, (void **) &begin_, D3DLOCK_READONLY);
	if(FAILED(hr)) throw exception::directx_error(hr);

	return begin_;
}

void VertexBuffer::unlock() const
{
	HRESULT hr = buffer_->Unlock();
	if(FAILED(hr)) throw exception::directx_error(hr);
}

template<>
IDirect3DVertexDeclaration9 *VertexArrayP2T::get_vertex_decl() const
{
	return renderer_->get_shared_resources()->get_vertex_decl<vertex_p2t>();
}

template<>
size_t VertexArrayP2T::get_vertex_size() const {
	return sizeof(vertex_p2t);
}

template<>
IDirect3DVertexDeclaration9 *VertexArrayPNT::get_vertex_decl() const
{
	return renderer_->get_shared_resources()->get_vertex_decl<vertex_pnt>();
}

template<>
size_t VertexArrayPNT::get_vertex_size() const {
	return sizeof(vertex_pnt);
}

template<>
IDirect3DVertexDeclaration9 *VertexArrayPNJWT::get_vertex_decl() const
{
	return renderer_->get_shared_resources()->get_vertex_decl<vertex_pnjwt>();
}

template<>
size_t VertexArrayPNJWT::get_vertex_size() const {
	return sizeof(vertex_pnjwt);
}

void bind_vertex_array(lua_State *L)
{
	using namespace luabind;

	module_(L, "graphic")
	[
		class_<VertexBuffer, boost::shared_ptr<VertexBuffer> >("VertexBuffer"),

		class_<VertexArrayP2T, VertexBuffer, boost::shared_ptr<VertexArrayP2T> >("VertexArrayP2T")
		.def(constructor<boost::shared_ptr<Renderer> const &, size_t, bool, bool>())
		.def("lock", (vertex_p2t *(VertexArrayP2T::*)()) &VertexArrayP2T::lock)
		.def("unlock", &VertexArrayP2T::unlock)
		.def("at", (vertex_p2t *(VertexArrayP2T::*)(size_t)) &VertexArrayP2T::at),

		class_<VertexArrayPNT, VertexBuffer, boost::shared_ptr<VertexArrayPNT> >("VertexArrayPNT")
		.def(constructor<boost::shared_ptr<Renderer> const &, size_t, bool, bool>())
		.def("lock", (vertex_pnt *(VertexArrayPNT::*)()) &VertexArrayPNT::lock)
		.def("unlock", &VertexArrayPNT::unlock)
		.def("at", (vertex_pnt *(VertexArrayPNT::*)(size_t)) &VertexArrayPNT::at),

		class_<VertexArrayPNJWT, VertexBuffer, boost::shared_ptr<VertexArrayPNJWT> >("VertexArrayPNJWT")
		.def(constructor<boost::shared_ptr<Renderer> const &, size_t, bool, bool>())
		.def("lock", (vertex_pnjwt *(VertexArrayPNJWT::*)()) &VertexArrayPNJWT::lock)
		.def("unlock", &VertexArrayPNJWT::unlock)
		.def("at", (vertex_pnjwt *(VertexArrayPNJWT::*)(size_t)) &VertexArrayPNJWT::at)
	];
}

}
