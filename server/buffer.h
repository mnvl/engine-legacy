#pragma once

#include <malloc.h>
#include <string.h>
#include <new>

namespace server
{

class buffer
{
public:
	buffer():
		data_((char *) malloc(size_)),
		size_(SIZE_1),
		read_(data_),
		write_(data_)
	{
		if (!data_) throw std::bad_alloc();
	}

	buffer(buffer const &rhs):
		data_((char *) malloc(rhs.size_)),
		size_(rhs.size_),
		read_(rhs.read_offset() + data_),
		write_(rhs.write_offset() + data_)
	{
		if (!data_) throw std::bad_alloc();
		memcpy(data_, rhs.data_, rhs.write_offset());
	}

	void swap(buffer &rhs)
	{
		std::swap(data_, rhs.data_);
		std::swap(size_, rhs.size_);
		std::swap(read_, rhs.read_);
		std::swap(write_, rhs.write_);
	}

	buffer &operator =(buffer const &rhs)
	{
		if (&rhs != this)
		{
			buffer(rhs).swap(*this);
		}

		return *this;
	}

	~buffer()
	{
		if (data_)
		{
			free(data_);
		}
	}

	void enlarge()
	{
		if (size_ != SIZE_1) throw std::bad_alloc();

		size_t r = read_offset();
		size_t w = write_offset();

		char *data = (char *) realloc(data_, SIZE_2);
		if (!data) throw std::bad_alloc();

		data_ = data;
		size_ = SIZE_2;
		read_ = data_ + r;
		write_ = data_ + w;
	}

	size_t read_offset() const { return read_ - data_; }
	size_t write_offset() const { return write_ - data_; }
	size_t write_bytes_available() const { return size_ - write_offset(); }

private:
	static size_t const SIZE_1 = 4096;
	static size_t const SIZE_2 = 65536;

	size_t size_;
	char *data_;
	char *read_, *write_;
};

}
