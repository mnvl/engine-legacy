#pragma once
//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2011 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <string>
#include <d3d9.h>
#include <d3dx9.h>
#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <util/boost_com_intrusive_ptr.h>
#include <boost/noncopyable.hpp>
#include "forward.h"

namespace graphic
{

class Shader: public boost::noncopyable {
public:
    Shader(RendererPtr const &renderer_ptr);
	virtual ~Shader();

    void load_from_file(std::string const &filename);
    void load_from_buffer(char const *buffer, size_t len);

    ID3DXEffect *get() const
    {
        return effect_.get();
    }

	virtual void postconstruct();

	void handle_lost_device();
	void handle_reset_device();

protected:
	D3D9RendererPtr const renderer_;
    boost::intrusive_ptr<ID3DXEffect> effect_;

	static void log_compilation_errors(ID3DXBuffer *compilation_errors_ptr);
};

}
