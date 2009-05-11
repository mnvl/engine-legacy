
#include <boost/date_time/posix_time/posix_time.hpp>
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

void logger::set_file(std::string const &filename)
{
	file_.open(filename.c_str(), O_WRONLY | O_CREAT);
}

void logger::write(std::string const &str)
{
	file_.write(str.c_str(), str.size());
}

}