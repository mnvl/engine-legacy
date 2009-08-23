
#include <boost/date_time/posix_time/posix_time.hpp>
#include <util/Logger.h>

namespace util
{

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

Logger::message::message(Logger *Logger_ptr, level_t level, char const *filename,
	int line, char const *pretty_function):
Logger_ptr_(Logger_ptr)
{
	stream_ << boost::posix_time::microsec_clock::local_time() << '\t'
			<< get_level_text(level) << '\t'
			<< filename << ':' << line << '\t'
			<< pretty_function << '\t';
}

Logger::message::~message()
{
	stream_ << '\n';

	Logger_ptr_->write(stream_.str());
}

Logger::Logger()
{
    set_stderr();
}

Logger::~Logger()
{
}

Logger &Logger::instance()
{
	static Logger singleton;
	return singleton;
}

void Logger::set_stdout()
{
	file_.set_handle(1);
}

void Logger::set_stderr()
{
    file_.set_handle(2);
}

void Logger::set_file(std::string const &filename)
{
	file_.open(filename.c_str(), O_WRONLY | O_CREAT);
}

void Logger::write(char const *buffer, size_t length)
{
    file_.write(buffer, length);
}

void Logger::write(std::string const &str)
{
	file_.write(str.c_str(), str.size());
}

}
