
#include "table.h"

namespace script
{

table::table(descr const *table_descr):
	descr_(table_descr),
	fields_(table_descr ? table_descr->nfields() : 0)
{

}

table::~table()
{
}

}
