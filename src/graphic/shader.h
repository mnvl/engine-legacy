#pragma once
//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2012 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <vector>
#include <boost/noncopyable.hpp>
#include <math/vec.h>
#include <math/matrix.h>
#include "forward.h"

namespace graphic
{

class Shader: public boost::noncopyable {
public:
	class Parameter: public boost::noncopyable {
	public:
		virtual ~Parameter() {}
		virtual void set(math::vec<2> const &v) = 0;
		virtual void set(math::vec<3> const &v) = 0;
		virtual void set(math::vec<4> const &v) = 0;
		virtual void set(math::matrix<4, 4> const &m) = 0;
		virtual void set(std::vector<math::matrix<4,4> > const &value) = 0;
		virtual void set(TexturePtr const &t) = 0;
		virtual void set(void const *ptr, size_t size) = 0;
	};

	typedef boost::shared_ptr<Parameter> ParameterPtr;

	typedef void *handle_type;

	virtual ~Shader();

	virtual void add_preprocessor_macro(std::string const &key, std::string const &value = std::string()) = 0;

    virtual void load_from_file(std::string const &filename) = 0;
    virtual void load_from_buffer(char const *buffer, size_t len) = 0;

	virtual ParameterPtr get_parameter(std::string const &name) = 0;

protected:
    Shader();
};

}
