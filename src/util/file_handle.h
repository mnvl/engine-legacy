#pragma once
//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#if defined(WIN32)
#include <io.h>
#endif

#if defined(__linux__)
#include <sys/types.h>
#define O_BINARY 0
#endif

#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <string>
#include <vector>
#include <stdexcept>

#pragma warning(disable: 4996)

namespace util {

class FileHandle {
public:
	class error: public std::runtime_error {
	public:
		error(std::string const &message, int error_number):
		runtime_error(message + ": " + strerror(error_number))
		{
		}
	};

	typedef long offset_t;
    typedef struct stat stat_t;

	FileHandle();
	explicit FileHandle(int handle);
	FileHandle(std::string const &filename, int flags, int mode = S_IREAD | S_IWRITE);
	FileHandle(FileHandle const &rhs);
	~FileHandle();

	void set_handle(int handle);
	void open(std::string const &filename, int flags, int mode = S_IREAD | S_IWRITE);
	void close();

	bool is_open() const;

	void read(void *buffer, size_t length);
	void write(void const *buffer, size_t length);
	offset_t lseek(offset_t offset, int origin);
	offset_t tell() { return lseek(0, SEEK_CUR); }

	void read_string(std::string &str);
	void write_string(std::string const &str);

    stat_t stat() const;

    void read_all(std::vector<char> &buffer);

private:
	int handle_;
	std::string filename_;
};

}
