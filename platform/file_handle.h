#pragma once

#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <string>
#include <stdexcept>

namespace platform {

class file_handle {
public:
	class error: public std::runtime_error {
	public:
		error(std::string const &message, int error_number):
		runtime_error(message + ": " + strerror(error_number))
		{
		}
	};

	typedef size_t size_type;
	typedef long offset_type;

	file_handle();
	explicit file_handle(int handle);
	file_handle(char const *filename, int flags, int mode = S_IREAD | S_IWRITE);
	file_handle(file_handle const &rhs);
	~file_handle();

	void open(char const *filename, int flags, int mode = S_IREAD | S_IWRITE);
	void close();

	bool is_open() const;

	void read(void *buffer, size_type length);
	void write(void const *buffer, size_type length);
	void lseek(offset_type offset, int origin);

	void read_string(std::string &string);
	void write_string(std::string const &string);

	size_type get_file_size() const;

private:
	int handle_;
	std::string filename_;
};

}
