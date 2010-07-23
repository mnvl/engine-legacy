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

#include <stdlib.h>
#include <util/logger.h>

namespace util
{

inline std::string get_sources_path()
{
	#if defined(ENGINE_ROOT)
		LOG_DEBUG("ENGINE_ROOT = " << ENGINE_ROOT);
		return std::string(ENGINE_ROOT) + "/src/";
	#endif

	LOG_WARN("ENGINE_ROOT is not defined, defaulting to current directory");
	return std::string();
}

inline std::string get_graphic_sources_path()
{
	std::string const &sources_path = get_sources_path();
	return sources_path.empty() ? std::string() : sources_path + "graphic/";
}

inline std::string get_particle_system_sources_path()
{
	std::string const &sources_path = get_sources_path();
	return sources_path.empty() ? std::string() : sources_path + "particle_system/";
}

}
