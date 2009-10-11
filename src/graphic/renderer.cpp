
#include <d3dx9.h>
#include <exception/directx_error.h>
#include <platform/timer.h>
#include <graphic/camera.h>
#include <graphic/font.h>
#include <graphic/material.h>
#include <graphic/renderer.h>

namespace graphic
{

Renderer::Renderer(platform::Window *window_ptr, int width, int height, bool fullscreen):
	window_ptr_(window_ptr),
	width_(width),
	height_(height),
	fullscreen_(fullscreen)
{
	{
		IDirect3D9 *direct3d_ptr = Direct3DCreate9(D3D_SDK_VERSION);
		direct3d_ptr_.reset(direct3d_ptr);
		direct3d_ptr->Release();
	}

	{
		IDirect3DDevice9 *device_ptr;

		D3DPRESENT_PARAMETERS pp;
		pp.BackBufferWidth = width_;
		pp.BackBufferHeight = height_;
		pp.BackBufferCount = 1;
		pp.BackBufferFormat = D3DFMT_A8R8G8B8;
		pp.MultiSampleType = D3DMULTISAMPLE_NONE;
		pp.MultiSampleQuality = 0;
		pp.SwapEffect = D3DSWAPEFFECT_FLIP;
		pp.hDeviceWindow = window_ptr_->get_handle();
		pp.Windowed = !fullscreen_;
		pp.EnableAutoDepthStencil = TRUE;
		pp.AutoDepthStencilFormat = D3DFMT_D24S8;
		pp.Flags = D3DPRESENTFLAG_DEVICECLIP;
		pp.FullScreen_RefreshRateInHz = 0;
		pp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

		HRESULT hr = direct3d_ptr_->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
			window_ptr_->get_handle(), D3DCREATE_HARDWARE_VERTEXPROCESSING, &pp, &device_ptr);
		if (FAILED(hr)) throw exception::directx_error(hr);

		device_ptr_.reset(device_ptr);
		device_ptr->Release();
	}

    resource_manager_.reset(new ResourceManager(this));
}

Renderer::~Renderer() {
}

void Renderer::begin_frame()
{
	HRESULT hr;

	frame_start_time_ = platform::Timer::get_current_time();
	frame_times_.push_back(frame_start_time_);

	hr = device_ptr_->Clear(0, 0, D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	if (FAILED(hr)) throw exception::directx_error(hr);

	hr = device_ptr_->BeginScene();
	if (FAILED(hr)) throw exception::directx_error(hr);
}

void Renderer::finish_frame()
{
	HRESULT hr;

	{
		char buffer[4096];
		sprintf(buffer, "%4.0lf msecs, %3d fps", last_frame_time_ * 1000.0, frame_times_.size());

		RECT rc = { 0, 0, width_, height_ };

		get_resource_manager()->get_default_font()->get()->DrawText(0, buffer, strlen(buffer), &rc, DT_LEFT | DT_TOP, D3DCOLOR_XRGB(255, 255, 255));
	}

	hr = device_ptr_->EndScene();
	if (FAILED(hr)) throw exception::directx_error(hr);

	hr = device_ptr_->Present(0, 0, 0, 0);
	if (FAILED(hr)) throw exception::directx_error(hr);

	double current_time = platform::Timer::get_current_time();
	last_frame_time_ = current_time - frame_start_time_;
	while (!frame_times_.empty() && (current_time - frame_times_.front()) > 1.0) frame_times_.pop_front();
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

}

