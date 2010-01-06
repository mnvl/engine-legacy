//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#pragma once

#include <d3d9.h>
#include <boost/cstdint.hpp>
#include <util/boost_com_intrusive_ptr.h>
#include <math/vec.h>
#include <math/matrix.h>
#include <graphic/vertex.h>
#include <graphic/model.h>

namespace graphic
{

class Renderer;

class HeightfieldLoader {
public:
    class load_error: public std::runtime_error {
	public:
		load_error(std::string const &message):
		runtime_error(message)
		{
		}
	};

    HeightfieldLoader(std::string const &filename, Model *model_ptr,
		boost::shared_ptr<Renderer> const &renderer_ptr = boost::shared_ptr<Renderer>());
	~HeightfieldLoader();

    template<class T>
	void load_from_raw(math::matrix<4, 4> const &tf, size_t nrows, size_t ncols,
		math::vec<3> const &scale, std::string const &texture_name);

private:
	std::string filename_;
	Model *model_ptr_;
	boost::shared_ptr<Renderer> const renderer_ptr_;
};

}
