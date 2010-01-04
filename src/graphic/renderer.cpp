
#include <d3dx9.h>
#include <boost/bind.hpp>
#include <exception/directx_error.h>
#include <platform/timer.h>
#include <util/logger.h>
#include "camera.h"
#include "font.h"
#include "material.h"
#include "vertex_array.h"
#include "renderer.h"

namespace graphic
{

Renderer::Renderer(boost::shared_ptr<platform::Window> const &window, int width, int height, bool fullscreen):
	window_ptr_(window),
	screen_width_(width),
	screen_height_(height),
	fullscreen_(fullscreen)
{
	if (!screen_width_ || !screen_height_)
	{
		RECT rc;
		GetClientRect(window->get_handle(), &rc);

		screen_width_ = rc.right - rc.left;
		screen_height_ = rc.bottom - rc.top;

		LOG_INFO("no width or height specified, taking dimensions from window client dimensions "
			<< screen_width_ << 'x' << screen_height_);
	}

	{
		IDirect3D9 *direct3d_ptr = Direct3DCreate9(D3D_SDK_VERSION);
		direct3d_ptr_.reset(direct3d_ptr);
		direct3d_ptr->Release();
	}

	{
		IDirect3DDevice9 *device_ptr;

		pp_.BackBufferWidth = screen_width_;
		pp_.BackBufferHeight = screen_height_;
		pp_.BackBufferCount = 1;
		pp_.BackBufferFormat = D3DFMT_X8R8G8B8;
		pp_.MultiSampleType = D3DMULTISAMPLE_NONE;
		pp_.MultiSampleQuality = 0;
		pp_.SwapEffect = D3DSWAPEFFECT_FLIP;
		pp_.hDeviceWindow = window_ptr_->get_handle();
		pp_.Windowed = !fullscreen_;
		pp_.EnableAutoDepthStencil = TRUE;
		pp_.AutoDepthStencilFormat = D3DFMT_D24S8;
		pp_.Flags = D3DPRESENTFLAG_DEVICECLIP;
		pp_.FullScreen_RefreshRateInHz = 0;
		pp_.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

		HRESULT hr = direct3d_ptr_->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
			window_ptr_->get_handle(), D3DCREATE_HARDWARE_VERTEXPROCESSING, &pp_, &device_ptr);

		if (FAILED(hr))
		{
			LOG_ERROR("failed to create direct3d device with hr = " << std::hex << hr
				<< ", trying sw vertex processing");

			hr = direct3d_ptr_->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
				window_ptr_->get_handle(), D3DCREATE_SOFTWARE_VERTEXPROCESSING, &pp_, &device_ptr);
		}

		if (FAILED(hr)) throw exception::directx_error(hr);

		device_ptr_.reset(device_ptr);
		device_ptr->Release();
	}
}

Renderer::~Renderer() {
}

void Renderer::postconstruct()
{
    shared_resources_.reset(new SharedResources(this));
	resources_directory_.reset(new ResourcesDirectory());
	window_ptr_->signal_resize.connect(boost::bind(&Renderer::thunk_resize, boost::weak_ptr<Renderer>(shared_from_this())));
}

void Renderer::begin_frame()
{
	HRESULT hr;

	hr = device_ptr_->Clear(0, 0, D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	if (FAILED(hr)) throw exception::directx_error(hr);

	hr = device_ptr_->BeginScene();
	if (FAILED(hr)) throw exception::directx_error(hr);
}

void Renderer::finish_frame()
{
	HRESULT hr;

	hr = device_ptr_->EndScene();
	if (FAILED(hr)) throw exception::directx_error(hr);

	hr = device_ptr_->Present(0, 0, 0, 0);
	if (FAILED(hr)) throw exception::directx_error(hr);

	/*
	double current_time = platform::Timer::get_current_time();
	last_frame_time_ = current_time - frame_start_time_;
	while (!frame_times_.empty() && (current_time - frame_times_.front()) > 1.0) frame_times_.pop_front();
	*/
}

void Renderer::set_view_projection_from_camera(Camera const &c)
{
    HRESULT hr;

	view_matrix_ = c.get_view_matrix();
	hr = device_ptr_->SetTransform(D3DTS_VIEW, (D3DMATRIX const *) &c.get_view_matrix());
	if (FAILED(hr)) throw exception::directx_error(hr);

	projection_matrix_ = c.get_projection_matrix();
	hr = device_ptr_->SetTransform(D3DTS_PROJECTION, (D3DMATRIX const *) &c.get_projection_matrix());
	if (FAILED(hr)) throw exception::directx_error(hr);
}

void Renderer::set_world_matrix(math::matrix<4,4> const &tf)
{
	world_matrix_ = tf;

    HRESULT hr = device_ptr_->SetTransform(D3DTS_WORLD, (D3DMATRIX const *) &tf);
	if (FAILED(hr)) throw exception::directx_error(hr);
}

void Renderer::draw_triangles(VertexBuffer *buffer, size_t triangles_count)
{
	draw_triangles(buffer, 0, triangles_count);
}

void Renderer::draw_triangles(VertexBuffer *buffer, size_t first_vertex, size_t triangles_count)
{
	return;

	if (triangles_count == 0) return;

	HRESULT hr;

	hr = device_ptr_->SetVertexDeclaration(buffer->get_vertex_decl());
	if (FAILED(hr)) throw exception::directx_error(hr);

    hr = device_ptr_->SetStreamSource(0, buffer->get(), 0, buffer->get_vertex_size());
	if (FAILED(hr)) throw exception::directx_error(hr);

	hr = device_ptr_->DrawPrimitive(D3DPT_TRIANGLELIST, first_vertex, triangles_count);
	if (FAILED(hr)) throw exception::directx_error(hr);
}

void Renderer::disable_all_tests()
{
	HRESULT hr;

	hr = device_ptr_->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	if (FAILED(hr)) throw exception::directx_error(hr);

	hr = device_ptr_->SetRenderState(D3DRS_LIGHTING, FALSE);
	if (FAILED(hr)) throw exception::directx_error(hr);

	hr = device_ptr_->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	if (FAILED(hr)) throw exception::directx_error(hr);
}

void Renderer::handle_resize()
{
	if (!fullscreen_)
	{
		RECT rc;
		GetClientRect(window_ptr_->get_handle(), &rc);

		screen_width_ = rc.right - rc.left;
		screen_height_ = rc.bottom - rc.top;

		LOG_INFO("window resized, new client dimensions are " << screen_width_ << 'x' << screen_height_);

		pp_.BackBufferWidth = screen_width_;
		pp_.BackBufferHeight = screen_height_;

		LOG_INFO("preparing all resources to Direct3D device reset...");
		resources_directory_->handle_device_lost();

		LOG_INFO("resetting Direct3D device...");
		HRESULT hr = get_device()->Reset(&pp_);
		if (FAILED(hr)) throw exception::directx_error(hr);

		LOG_INFO("restoring all resources...");
		resources_directory_->restore_after_device_lost();
	}
}

void Renderer::thunk_resize(boost::weak_ptr<Renderer> renderer)
{
	if (boost::shared_ptr<Renderer> self = renderer.lock())
	{
		self->handle_resize();
	}
}

}

