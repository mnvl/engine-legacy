#pragma once

#include <vector>
#include <util/file_handle.h>

namespace util
{

class Serializer {
public:
	Serializer(FileHandle *file_handle):
		file_handle_(file_handle)
	{
	}

	~Serializer()
	{
	}

	template<class T>
	Serializer &pod(T const &data)
	{
		file_handle_->write(&data, sizeof(T));
		return *this;
	}

	template<class T>
	Serializer &pod(std::vector<T> const &data)
	{
		pod(data.size());

		if (data.size())
		{
			file_handle_->write(&data[0], data.size() * sizeof(T));
		}

		return *this;
	}

private:
	FileHandle *file_handle_;
};

}

