#pragma once

#include "clock.h"

namespace chloe
{
namespace model
{

class Model
{
public:
	static Model &instance();

	static Clock &clock()
	{
		return instance().clock_;
	}

private:
	Clock clock_;
};

}
}
