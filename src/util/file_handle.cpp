//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2011 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <boost/shared_ptr.hpp>
#include <luabind/luabind.hpp>
#include <util/file_handle.h>

namespace util
{

FileHandle::FileHandle():
	handle_(-1)
{
}

FileHandle::FileHandle(int handle):
	handle_(handle)
{
}

FileHandle::FileHandle(std::string const &filename, int flags, int mode):
	handle_(-1)
{
	open(filename, flags, mode);
}

FileHandle::FileHandle(FileHandle const &rhs):
	handle_(rhs.handle_)
{
}

FileHandle::~FileHandle() {
	if(is_open()) close();
}

void FileHandle::set_handle(int handle)
{
	if(is_open()) throw std::logic_error("set_handle() called on already opened file");

	handle_ = handle;
	filename_.erase();
}

void FileHandle::open(std::string const &filename, int flags, int mode)
{
	if(is_open()) throw std::logic_error("open() called on already opened file");

	handle_ = ::open(filename.c_str(), flags | O_BINARY, mode);
	if(handle_ < 0) throw error(std::string("error opening file ") + filename, errno);

	filename_ = filename;
}

bool FileHandle::is_open() const {
	return handle_ >= 0;
}

void FileHandle::close() {
	if(is_open())
	{
		::close(handle_);
		handle_ = -1;
	}
}

void FileHandle::read(void *buffer, size_t length) {
	if(!is_open()) throw std::logic_error("file handle is not open but read() is called");

	int bytes_read = ::read(handle_, buffer, length);

	if(bytes_read < 0)
		throw error("error reading from file " + filename_, errno);

	if(bytes_read != length)
		throw std::logic_error("end of file reached while reading from file " + filename_);
}

void FileHandle::write(void const *buffer, size_t length) {
	if(!is_open()) throw std::logic_error("file handle is not open but write() is called");
	long bytes_written = ::write(handle_, buffer, length);
	if(bytes_written != length) throw error("error writing to file " + filename_, errno);
}

FileHandle::offset_t FileHandle::lseek(offset_t offset, int origin) {
	if(!is_open())
		throw std::logic_error("file handle is not open but lseek() is called");

	offset_t current_offset = ::lseek(handle_, offset, origin);

	if(current_offset < 0)
		throw error("error seeking in file " + filename_, errno);

	return current_offset;
}

FileHandle::stat_t FileHandle::stat() const {
    stat_t st;
    if(fstat(handle_, &st) < 0)
			throw error("error retreiving stats for " + filename_, errno);

	return st;
}

void FileHandle::read_all(std::vector<char> &buffer)
{
    buffer.resize(stat().st_size);
    read(&buffer[0], buffer.size());
}

void FileHandle::bind(lua_State *L)
{
	using namespace luabind;

	module_(L, "util")
	[
		class_<FileHandle, boost::shared_ptr<FileHandle> >("FileHandle")
		.enum_("open_mode")
		[
			value("O_RDONLY", O_RDONLY),
			value("O_WRONLY", O_WRONLY),
			value("O_CREAT", O_CREAT)
		]
		.enum_("open_mode")
		[
			value("S_IREAD", S_IREAD),
			value("S_IWRITE", S_IWRITE)
		]
		.def(constructor<std::string const &, int, int>())
		.def("close", &close)
	];

}

}
