
#include "model.h"

namespace chloe
{
namespace model
{

Model &Model::instance()
{
	static Model model;
	return model;
}

}
}
