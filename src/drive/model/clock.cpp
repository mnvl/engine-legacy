
#include <boost/date_time/posix_time/posix_time.hpp>
#include "clock.h"

namespace drive
{
namespace model
{

class Clock::Impl
{
public:
	Impl():
		epoch_(boost::posix_time::microsec_clock::local_time())
	{
	}

	boost::posix_time::time_duration time_since_epoch() const
	{
		return boost::posix_time::microsec_clock::local_time() - epoch_;
	}

private:
	boost::posix_time::ptime epoch_;
};

Clock::Clock():
	impl_(new Impl()),
	delta_(0)
{
}

Clock::~Clock()
{
}

long Clock::millisecs() const
{
	return long(impl_->time_since_epoch().total_milliseconds()) + delta_;
}

void Clock::set_delta(long delta)
{
	delta_ = delta;
}

}
}
