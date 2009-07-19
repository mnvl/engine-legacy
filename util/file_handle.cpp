
#include <platform/file_handle.h>

namespace platform
{

file_handle::file_handle():
	handle_(-1)
{
}

file_handle::file_handle(int handle):
	handle_(handle)
{
}

file_handle::file_handle(char const *filename, int flags, int mode):
	handle_(-1)
{
	open(filename, flags, mode);
}

file_handle::file_handle(file_handle const &rhs):
	handle_(rhs.handle_)
{
}

file_handle::~file_handle() {
	if(is_open()) close();
}

void file_handle::set_handle(int handle)
{
	if(is_open()) throw std::logic_error("set_handle() called on already opened file");

	handle_ = handle;
	filename_.erase();
}

void file_handle::open(char const *filename, int flags, int mode)
{
	if(is_open()) throw std::logic_error("open() called on already opened file");

	handle_ = ::open(filename, flags | O_BINARY, mode);
	if(handle_ < 0) throw error(std::string("error opening file ") + filename, errno);

	filename_ = filename;
}

bool file_handle::is_open() const {
	return handle_ >= 0;
}

void file_handle::close() {
	if(!is_open()) throw std::logic_error("file handle is not open but close() is called");

	if (handle_ > 2) ::close(handle_);
	handle_ = -1;
}

void file_handle::read(void *buffer, size_t length) {
	if(!is_open()) throw std::logic_error("file handle is not open but read() is called");

	int bytes_read = ::read(handle_, buffer, length);

	if(bytes_read < 0)
		throw error("error reading from file " + filename_, errno);

	if(bytes_read != length)
		throw std::logic_error("end of file reached while reading from file " + filename_);
}

void file_handle::write(void const *buffer, size_t length) {
	if(!is_open()) throw std::logic_error("file handle is not open but write() is called");
	long bytes_written = ::write(handle_, buffer, length);
	if(bytes_written != length) throw error("error writing to file " + filename_, errno);
}

void file_handle::lseek(offset_type offset, int origin) {
	if(!is_open())
		throw std::logic_error("file handle is not open but lseek() is called");

	if(::lseek(handle_, offset, origin) < 0)
		throw error("error seeking in file " + filename_, errno);
}

void file_handle::read_string(std::string &str) {
	str.clear();
	for(char ch; read(&ch, sizeof(ch)), ch != 0; str += ch);
}

void file_handle::write_string(std::string const &str) {
	write(str.c_str(), str.size());
}

file_handle::stat_t file_handle::stat() const {
    stat_t st;
    if(fstat(handle_, &st) < 0)
			throw error("error retreiving stats for " + filename_, errno);

	return st;
}

void file_handle::read_all(std::vector<char> &buffer)
{
    buffer.resize(stat().st_size);
    read(&buffer[0], buffer.size());
}

}
