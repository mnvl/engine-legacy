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

#include <log4cplus/logger.h>
#include <luabind/lua_include.hpp>

#define LOG_DEBUG(M)    LOG4CPLUS_DEBUG(log4cplus::Logger::getRoot(), M)
#define LOG_INFO(M)	LOG4CPLUS_INFO(log4cplus::Logger::getRoot(), M)
#define LOG_WARN(M)	LOG4CPLUS_WARN(log4cplus::Logger::getRoot(), M)
#define LOG_ERROR(M)    LOG4CPLUS_ERROR(log4cplus::Logger::getRoot(), M)

namespace util
{

void bind_logger(lua_State *L);

}
