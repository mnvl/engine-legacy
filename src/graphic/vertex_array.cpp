
#include <luabind/luabind.hpp>
#include <exception/directx_error.h>
#include "renderer.h"
#include "vertex_array.h"

namespace graphic
{

VertexBuffer::VertexBuffer(boost::shared_ptr<Renderer> const &renderer, size_t size,
	bool dynamic, bool writeonly)
:
	renderer_(renderer),
	clone_(size),
	usage_(0),
	pool_(dynamic ? D3DPOOL_DEFAULT : D3DPOOL_MANAGED)
{
	if(dynamic) usage_ |= D3DUSAGE_DYNAMIC;
	if(writeonly) usage_ |= D3DUSAGE_WRITEONLY;

	IDirect3DVertexBuffer9 *buffer;
	HRESULT hr = renderer_->get_device()->CreateVertexBuffer(size, usage_, 0, pool_, &buffer, 0);
	if(FAILED(hr)) throw exception::directx_error(hr);

	buffer_.reset(buffer);
	buffer->Release();
}

VertexBuffer::~VertexBuffer()
{
}

const void *VertexBuffer::lock() const
{
	return &clone_[0];
}

void VertexBuffer::unlock() const
{
}

void *VertexBuffer::lock(bool discard)
{
	return &clone_[0];
}

void VertexBuffer::unlock()
{
	void *ptr = 0;

	HRESULT hr = buffer_->Lock(0, 0, (void **) &ptr, D3DLOCK_DISCARD);
	if(FAILED(hr)) throw exception::directx_error(hr);

	memcpy(ptr, &clone_[0], clone_.size());

	hr = buffer_->Unlock();
	if(FAILED(hr)) throw exception::directx_error(hr);
}

void VertexBuffer::handle_device_lost()
{
	buffer_.reset();
}

void VertexBuffer::restore_after_device_lost()
{
	IDirect3DVertexBuffer9 *buffer;
	HRESULT hr = renderer_->get_device()->CreateVertexBuffer(clone_.size(), usage_, 0, pool_, &buffer, 0);
	if(FAILED(hr)) throw exception::directx_error(hr);

	buffer_.reset(buffer);
	buffer->Release();

	// unlock will restore contents of buffer
	lock();
	unlock();
}

template<>
IDirect3DVertexDeclaration9 *VertexArrayPT::get_vertex_decl() const
{
	return renderer_->get_shared_resources()->get_vertex_decl<vertex_pt>();
}

template<>
size_t VertexArrayPT::get_vertex_size() const {
	return sizeof(vertex_pt);
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
		class_<VertexBuffer, Resource, boost::shared_ptr<VertexBuffer> >("VertexBuffer"),

		class_<VertexArrayPT, VertexBuffer, boost::shared_ptr<VertexArrayPT> >("VertexArrayPT")
		.def("lock", (vertex_pt *(VertexArrayPT::*)(bool)) &VertexArrayPT::lock)
		.def("unlock", (void (VertexArrayPT::*)()) &VertexArrayPT::unlock)
		.def("at", (vertex_pt *(VertexArrayPT::*)(size_t)) &VertexArrayPT::at),

		class_<VertexArrayPNT, VertexBuffer, boost::shared_ptr<VertexArrayPNT> >("VertexArrayPNT")
		.def("lock", (vertex_pnt *(VertexArrayPNT::*)(bool)) &VertexArrayPNT::lock)
		.def("unlock", (void (VertexArrayPNT::*)()) &VertexArrayPNT::unlock)
		.def("at", (vertex_pnt *(VertexArrayPNT::*)(size_t)) &VertexArrayPNT::at),

		class_<VertexArrayPNJWT, VertexBuffer, boost::shared_ptr<VertexArrayPNJWT> >("VertexArrayPNJWT")
		.def("lock", (vertex_pnjwt *(VertexArrayPNJWT::*)(bool)) &VertexArrayPNJWT::lock)
		.def("unlock", (void (VertexArrayPNJWT::*)()) &VertexArrayPNJWT::unlock)
		.def("at", (vertex_pnjwt *(VertexArrayPNJWT::*)(size_t)) &VertexArrayPNJWT::at)
	];
}

}
