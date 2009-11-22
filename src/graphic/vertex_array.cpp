
#include <luabind/luabind.hpp>
#include <exception/directx_error.hpp>
#include "renderer.h"
#include "vertex_array.hpp"

namespace graphic
{

VertexBuffer::VertexBuffer(Renderer *renderer, size_t size, bool dynamic, bool writeonly)
{
	DWORD usage = 0;
	if(dynamic) usage |= D3DUSAGE_DYNAMIC;
	if(writeonly) usage |= D3DUSAGE_WRITEONLY;

	D3DPOOL pool = dynamic ? D3DPOOL_DEFAULT : D3DPOOL_MANAGED;

	IDirect3DVertexBuffer9 *buffer;
	HRESULT hr = renderer->get_device()->CreateVertexBuffer(size, usage, vertex_format, pool, &buffer, 0);
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

void bind_vertex_array(lua_State *L)
{
	using namespace luabind;

	module_(L, "graphic")
	[
		class_<VertexArrayPNT>("VertexArrayPNT")
		.def(constructor<Renderer *, size_t, bool, bool>())
		.def("lock", (vertex_pnt *(*)()) &VertexArrayPNT::lock)
		.def("unlock", &VertexArrayPNT::unlock)
		.def("at", &VertexArrayPNT::at),

		class_<VertexArrayPNJWT>("VertexArrayPNJWT")
		.def(constructor<Renderer *, size_t, bool, bool>())
		.def("lock", (vertex_pnjwt *(*)()) &VertexArrayPNJWT::lock)
		.def("unlock", &VertexArrayPNJWT::unlock)
		.def("at", &VertexArrayPNJWT::at)
	];
}

}
