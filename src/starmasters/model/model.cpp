
#include "model.h"

namespace starmasters
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
