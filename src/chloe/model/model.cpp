
#include "model.h"

namespace chloe
{
namespace model
{

Model &Model::Instance()
{
	static Model model;
	return model;
}

}
}
