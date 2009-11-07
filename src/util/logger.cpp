
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

Logger::Message::Message(Logger *Logger_ptr, level_t level, char const *filename, int line, char const *pretty_function):
Logger_ptr_(Logger_ptr),
filename_(filename),
line_(line),
pretty_function_(pretty_function)
{
	stream_ << boost::posix_time::microsec_clock::local_time() << '\t'
			<< get_level_text(level) << '\t';
}

Logger::Message::~Message()
{
	stream_ << "\t["
			<< filename_ << ':' << line_ << ' '
			<< pretty_function_ << "]\n";

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
