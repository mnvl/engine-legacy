
#include <lauxlib.h>
#include <windows.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "logger.h"

namespace util
{

Logger::Output::~Output()
{
}

namespace
{

class DebugStringOutput: public Logger::Output
{
public:
	void print_line(char const *message)
	{
		OutputDebugString(message);
	}
};

class StdoutOutput: public Logger::Output
{
public:
	void print_line(char const *message)
	{
		fwrite(message, 1, strlen(message), stdout);
	}
};

class StderrOutput: public Logger::Output
{
public:
	void print_line(char const *message)
	{
		fwrite(message, 1, strlen(message), stderr);
	}
};

}

char const *Logger::get_level_text(level_t level)
{
	switch (level) {
	case LEVEL_DEBUG: return "DEBUG";
	case LEVEL_INFO: return "INFO";
	case LEVEL_NOTICE: return "NOTICE";
	case LEVEL_WARNING: return "WARNING";
	case LEVEL_ERROR: return "ERROR";
	case LEVEL_CRITICAL: return "CRITICAL";
	default: return "UNKNOWN";
	}
}

Logger::Message::Message(Logger *logger_ptr, level_t level, char const *filename, int line, char const *pretty_function):
logger_ptr_(logger_ptr),
filename_(filename),
line_(line),
pretty_function_(pretty_function)
{
	stream_ << boost::posix_time::microsec_clock::local_time() << '\t'
			<< get_level_text(level) << '\t';
}

Logger::Message::~Message()
{
	if (logger_ptr_->is_source_position_enabled())
	{
		stream_ << "\t["
				<< filename_ << ':' << line_ << ' '
				<< pretty_function_ << "]\n";
	}
	else
	{
		stream_ << '\n';
	}

	logger_ptr_->print_line(stream_.str().c_str());
}

Logger::Logger():
	source_position_enabled_(false)
{
	add_debug_string_output();
	add_stderr_output();
}

Logger::~Logger()
{
}

Logger &Logger::instance()
{
	static Logger singleton;
	return singleton;
}

void Logger::print_line(char const *str)
{
	for (outputs_t::iterator it = outputs_.begin(); it != outputs_.end(); it++)
	{
		(*it)->print_line(str);
	}
}

void Logger::add_debug_string_output()
{
	outputs_.push_back(OutputPtr(new DebugStringOutput()));
}

void Logger::add_stdout_output()
{
	outputs_.push_back(OutputPtr(new StdoutOutput()));
}

void Logger::add_stderr_output()
{
	outputs_.push_back(OutputPtr(new StderrOutput()));
}

void Logger::enable_source_position()
{
	source_position_enabled_ = true;
}

void Logger::disable_source_position()
{
	source_position_enabled_ = false;
}

bool Logger::is_source_position_enabled() const
{
	return source_position_enabled_;
}

namespace
{

int lua_message(lua_State *L, Logger::level_t level)
{
	lua_Debug d;
	lua_getstack(L, 1, &d);
	lua_getinfo(L, "Sln", &d);

	Logger::Message message(&Logger::instance(), level, d.short_src, d.currentline, d.name);
	message.stream() << lua_tostring(L, -1);

	lua_pop(L, 1);

	return 0;
}

int lua_debug(lua_State *L)
{
	return lua_message(L, Logger::LEVEL_DEBUG);
}

luaL_reg const log_functions[] = {
	{"debug", lua_debug},
	{0, 0}
};

}

void Logger::bind(lua_State *L)
{
	luaL_register(L, "log", log_functions);
}

}
