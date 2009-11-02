
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
		fputs(message, stdout);
	}
};

class StderrOutput: public Logger::Output
{
public:
	void print_line(char const *message)
	{
		fputs(message, stderr);
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

Logger::Message::Message(Logger *Logger_ptr, level_t level, char const *filename,
	int line, char const *pretty_function):
Logger_ptr_(Logger_ptr)
{
	stream_ << boost::posix_time::microsec_clock::local_time() << '\t'
			<< get_level_text(level) << '\t'
			<< filename << ':' << line << '\t'
			<< pretty_function << '\t';
}

Logger::Message::~Message()
{
	Logger_ptr_->print_line(stream_.str().c_str());
}

Logger::Logger()
{
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

void Logger::enable_output_debug_string()
{
	outputs_.push_back(OutputPtr(new DebugStringOutput()));
}

void Logger::enable_stdout()
{
	outputs_.push_back(OutputPtr(new StdoutOutput()));
}

void Logger::enable_stderr()
{
	outputs_.push_back(OutputPtr(new StderrOutput()));
}

}
