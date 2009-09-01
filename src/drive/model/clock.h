#pragma once

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

namespace drive
{
namespace model
{

class Clock: public boost::noncopyable
{
public:
	Clock();
	~Clock();

	long millisecs() const;
	void set_delta(long delta);

private:
	class Impl;
	boost::scoped_ptr<Impl> impl_;
	long delta_;
};

}
}
