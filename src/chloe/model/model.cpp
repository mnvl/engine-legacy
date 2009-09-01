
#include "model.h"

namespace drive
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
