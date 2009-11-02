#pragma once

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
		Message(Logger *Logger_ptr, level_t level, char const *filename,
			int line, char const *pretty_function);
		~Message();

		std::ostream &stream() const
		{
			return stream_;
		}

	private:
		Logger *Logger_ptr_;
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

	void enable_output_debug_string();
	void enable_stdout();
	void enable_stderr();

private:
	typedef std::vector<OutputPtr> outputs_t;

	static char const *get_level_text(level_t level);
	outputs_t outputs_;
};

#define LOG_MESSAGE(L, M) { util::Logger::Message(&util::Logger::instance(), util::Logger::L, __FILE__, __LINE__, __FUNCTION__).stream() << M; }
#define LOG_DEBUG(M)    LOG_MESSAGE(LEVEL_DEBUG, M)
#define LOG_INFO(M)     LOG_MESSAGE(LEVEL_INFO, M)
#define LOG_WARNING(M)  LOG_MESSAGE(LEVEL_WARNING, M)
#define LOG_ERROR(M)    LOG_MESSAGE(LEVEL_ERROR, M)
#define LOG_CRITICAL(M) LOG_MESSAGE(LEVEL_CRITICAL, M)

}
