#pragma once

#include <sstream>
#include <platform/file_handle.h>

namespace util
{

class logger {
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

	class message {
	public:
		message(logger *logger_ptr, level_t level, char const *filename,
			int line, char const *pretty_function);
		~message();

		std::ostream &stream() const
		{
			return stream_;
		}

	private:
		logger *logger_ptr_;
		mutable std::stringstream stream_;
	};

	logger();
	~logger();

	static logger &instance();

	void set_file(std::string const &filename);
	void write(std::string const &str);

private:
	platform::file_handle file_;

	static char const *get_level_text(level_t level);
};

#define LOG_MESSAGE(L, M) { util::logger::message(&util::logger::instance(), util::logger::L, __FILE__, __LINE__, __FUNCTION__).stream() << M; }
#define LOG_DEBUG(M) LOG_MESSAGE(LEVEL_DEBUG, M)
#define LOG_INFO(M) LOG_MESSAGE(LEVEL_INFO, M)

}