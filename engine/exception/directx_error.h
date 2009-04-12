#pragma once

#include <windows.h>
#include <dxerr.h>
#include <exception>

namespace engine {
namespace exception {

class directx_error: public std::exception {
private:
	HRESULT hresult_;

public:
	directx_error(HRESULT hr) throw(): hresult_(hr) {}
	~directx_error() throw() {}
	char const *what() const throw() { return DXGetErrorDescription(hresult_); }
};

}
}