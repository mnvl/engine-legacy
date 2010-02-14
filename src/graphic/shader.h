#pragma once
//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <d3dx9.h>
#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <util/boost_com_intrusive_ptr.h>

namespace graphic
{

class Renderer;

class Shader {
public:
    Shader(boost::shared_ptr<Renderer> const &renderer_ptr);
	virtual ~Shader();

    void load_from_file(std::string const &filename);
    void load_from_buffer(char const *buffer, size_t len);

    ID3DXEffect *get() const
    {
        return effect_ptr_.get();
    }

private:
	boost::shared_ptr<Renderer> const renderer_ptr_;
    boost::intrusive_ptr<ID3DXEffect> effect_ptr_;

	static void log_compilation_errors(ID3DXBuffer *compilation_errors_ptr);
};

}
