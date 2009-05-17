
#include <windows.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <exception/winapi_error.h>
#include <util/logger.h>

namespace util
{

char const *logger::get_level_text(level_t level)
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

logger::message::message(logger *logger_ptr, level_t level, char const *filename,
	int line, char const *pretty_function):
logger_ptr_(logger_ptr)
{
	stream_ << boost::posix_time::microsec_clock::local_time() << '\t'
			<< get_level_text(level) << '\t'
			<< filename << ':' << line << '\t'
			<< pretty_function << '\t';
}

logger::message::~message()
{
	stream_ << '\n';

	logger_ptr_->write(stream_.str());
}

logger::logger()
{
}

logger::~logger()
{
}

logger &logger::instance()
{
	static logger singleton;
	return singleton;
}

void logger::set_stdout()
{
	file_.set_handle(1);
}

void logger::set_file(std::string const &filename)
{
	file_.open(filename.c_str(), O_WRONLY | O_CREAT);
}

void logger::set_file_by_executable_name()
{
	char buffer[1024];
	if (!GetModuleFileName(0, buffer, sizeof(buffer))) throw exception::winapi_error();

	strcat(buffer, ".log");

	set_file(buffer);
}

void logger::write(char const *buffer, size_t length)
{
    file_.write(buffer, length);
}

void logger::write(std::string const &str)
{
	file_.write(str.c_str(), str.size());
}

}
