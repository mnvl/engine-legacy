#pragma once

#include <stdlib.h>
#include <util/logger.h>

namespace util
{

inline std::string get_sources_path()
{
	char const *ENGINE_SOURCE_DIR = getenv("ENGINE_SOURCE_DIR");

	if (ENGINE_SOURCE_DIR)
	{
		LOG_DEBUG("ENGINE_SOURCE_DIR = " << ENGINE_SOURCE_DIR);
		return std::string(ENGINE_SOURCE_DIR) + "/src/";
	}

	LOG_WARNING("ENGINE_SOURCE_DIR is not defined, defaulting to current directory");
	return std::string();
}

inline std::string get_graphic_sources_path()
{
	std::string const &sources_path = get_sources_path();
	return sources_path.empty() ? std::string() : sources_path + "graphic/";
}

}
