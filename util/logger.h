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

		class message: public std::stringstream {
		public:
			message(logger *logger_ptr, level_t level, char const *filename,
				int line, char const *pretty_function);
			~message();

		private:
			logger *logger_ptr_;
		};

		logger();
		~logger();

		static logger &instance();

		void set_file(std::string const &filename);
		void write(message const &msg);

	private:
		platform::file_handle file_;

		static char const *get_level_text(level_t level);
	};
}