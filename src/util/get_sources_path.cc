#include <stdlib.h>
#include <util/logger.h>
#include "get_sources_path.h"

namespace util
{

std::string get_sources_path()
{
#if defined(_DEBUG)
#if defined(ENGINE_ROOT)
	LOG_DEBUG("ENGINE_ROOT = " << ENGINE_ROOT);
	return std::string(ENGINE_ROOT) + "/src/";
#endif

	LOG_WARN("ENGINE_ROOT is not defined, defaulting to current directory");
	return std::string();
#else
	return std::string();
#endif
}

std::string get_graphic_sources_path()
{
	std::string const &sources_path = get_sources_path();
	return sources_path.empty() ? std::string() : sources_path + "graphic/";
}

std::string get_particle_system_sources_path()
{
	std::string const &sources_path = get_sources_path();
	return sources_path.empty() ? std::string() : sources_path + "particle_system/";
}

}
