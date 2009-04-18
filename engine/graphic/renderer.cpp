
#include <d3dx9.h>
#include <engine/exception/directx_error.h>
#include <engine/graphic/camera.h>
#include <engine/graphic/renderer.h>

namespace engine {
namespace graphic {

renderer::renderer(system::window *window_ptr, int width, int height, bool fullscreen):
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
}

void renderer::begin_frame()
{
	HRESULT hr;

	hr = device_ptr_->Clear(0, 0, D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	if (FAILED(hr)) throw exception::directx_error(hr);

	hr = device_ptr_->BeginScene();
	if (FAILED(hr)) throw exception::directx_error(hr);
}

void renderer::finish_frame()
{
	HRESULT hr;

	hr = device_ptr_->EndScene();
	if (FAILED(hr)) throw exception::directx_error(hr);

	hr = device_ptr_->Present(0, 0, 0, 0);
	if (FAILED(hr)) throw exception::directx_error(hr);
}

void renderer::select(camera const &c)
{
	device_ptr_->SetTransform(D3DTS_VIEW, (D3DMATRIX const *) &c.get_view_matrix());
	device_ptr_->SetTransform(D3DTS_PROJECTION, (D3DMATRIX const *) &c.get_projection_matrix());
}

}
}
