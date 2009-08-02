#pragma once

#include <boost/intrusive_ptr.hpp>
#include <util/logger.h>
#include <util/boost_com_intrusive_ptr.h>
#include <exception/directx_error.h>
#include <graphic/renderer.h>

namespace graphic
{

class Shader {
public:
    Shader(Renderer *renderer_ptr):
        renderer_ptr_(renderer_ptr)
    {
    }

    void load_from_file(std::string const &filename)
    {
        HRESULT hr;

        LOG_INFO("loading shader from file " << filename);

        ID3DXEffect *effect_ptr;
        ID3DXBuffer *compilation_errors_ptr;
        hr = D3DXCreateEffectFromFile(renderer_ptr_->get_device(), filename.c_str(), 0, 0, D3DXSHADER_DEBUG, 0,
            &effect_ptr, &compilation_errors_ptr);

        if (compilation_errors_ptr && compilation_errors_ptr->GetBufferSize() > 0)
        {
            LOG_WARNING("compilation messages:");
            util::logger::instance().write((char const *) compilation_errors_ptr->GetBufferPointer(), compilation_errors_ptr->GetBufferSize());
            compilation_errors_ptr->Release();
        }

        if (FAILED(hr)) throw exception::directx_error(hr);

        effect_ptr_.reset(effect_ptr);
    }

    ID3DXEffect *get() const
    {
        return effect_ptr_.get();
    }

private:
    Renderer *renderer_ptr_;
    boost::intrusive_ptr<ID3DXEffect> effect_ptr_;
};

}
