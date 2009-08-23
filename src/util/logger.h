#pragma once

#include <sstream>
#include <util/file_handle.h>

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

	class message {
	public:
		message(Logger *Logger_ptr, level_t level, char const *filename,
			int line, char const *pretty_function);
		~message();

		std::ostream &stream() const
		{
			return stream_;
		}

	private:
		Logger *Logger_ptr_;
		mutable std::stringstream stream_;
	};

	Logger();
	~Logger();

	static Logger &instance();

	void set_stdout();
    void set_stderr();
	void set_file(std::string const &filename);

    void write(char const *buffer, size_t length);
	void write(std::string const &str);

private:
	util::FileHandle file_;

	static char const *get_level_text(level_t level);
};

#define LOG_MESSAGE(L, M) { util::Logger::message(&util::Logger::instance(), util::Logger::L, __FILE__, __LINE__, __FUNCTION__).stream() << M; }
#define LOG_DEBUG(M)    LOG_MESSAGE(LEVEL_DEBUG, M)
#define LOG_INFO(M)     LOG_MESSAGE(LEVEL_INFO, M)
#define LOG_WARNING(M)  LOG_MESSAGE(LEVEL_WARNING, M)
#define LOG_ERROR(M)    LOG_MESSAGE(LEVEL_ERROR, M)
#define LOG_CRITICAL(M) LOG_MESSAGE(LEVEL_CRITICAL, M)

}
