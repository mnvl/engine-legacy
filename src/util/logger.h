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

#include <luabind/lua_include.hpp>
#include <vector>
#include <sstream>
#include <boost/shared_ptr.hpp>

namespace util
{

class Logger {
public:
	enum level_t
	{
		LEVEL_DEBUG = 1,
		LEVEL_INFO = 2,
		LEVEL_NOTICE = 3,
		LEVEL_WARNING = 4,
		LEVEL_ERROR = 5,
		LEVEL_CRITICAL = 6
	};

	class Message {
	public:
		Message(Logger *logger_ptr, level_t level, char const *filename,
			int line, char const *pretty_function);
		~Message();

		std::ostream &stream() const
		{
			return stream_;
		}

	private:
		Logger *logger_ptr_;
		mutable std::stringstream stream_;

		char const * const filename_;
		int const line_;
		char const * const pretty_function_;
	};

	class Output {
	public:
		virtual ~Output();
		virtual void print_line(char const *str) = 0;
	};

	typedef boost::shared_ptr<Output> OutputPtr;

	Logger();
	~Logger();

	static Logger &instance();

	void print_line(char const *str);

	void add_debug_string_output();
	void add_stdout_output();
	void add_stderr_output();

	void enable_source_position();
	void disable_source_position();
	bool is_source_position_enabled() const;

	static void bind(lua_State *L);

private:
	typedef std::vector<OutputPtr> outputs_t;

	static char const *get_level_text(level_t level);

	outputs_t outputs_;
	bool source_position_enabled_;
};

#define LOG_MESSAGE(L, M) { util::Logger::Message(&util::Logger::instance(), util::Logger::L, __FILE__, __LINE__, __FUNCTION__).stream() << M; }
#define LOG_DEBUG(M)    LOG_MESSAGE(LEVEL_DEBUG, M)
#define LOG_INFO(M)     LOG_MESSAGE(LEVEL_INFO, M)
#define LOG_WARNING(M)  LOG_MESSAGE(LEVEL_WARNING, M)
#define LOG_ERROR(M)    LOG_MESSAGE(LEVEL_ERROR, M)
#define LOG_CRITICAL(M) LOG_MESSAGE(LEVEL_CRITICAL, M)

}
