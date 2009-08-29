#pragma once

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

namespace chloe
{
namespace model
{

class Clock: public boost::noncopyable
{
public:
	Clock();
	~Clock();

	long millisecs() const;

private:
	class Impl;
	boost::scoped_ptr<Impl> impl_;
};

}
}
