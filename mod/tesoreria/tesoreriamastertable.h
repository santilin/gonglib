#ifndef TESORERIAMASTERTABLE_H
#define TESORERIAMASTERTABLE_H

#include <empresamastertable.h>
#include "tesoreriafldpedircampo.h"

namespace gong {
namespace tesoreria {
	
	
class MasterTable: public empresa::MasterTable
{
public:
    MasterTable( dbDefinition &db, const Xtring &name, const Xtring &from = Xtring::null,
                 const Xtring &where = Xtring::null, const Xtring &orderby = Xtring::null )
        : empresa::MasterTable( db, name, from, where, orderby ) {}
    MasterTable( dbTableDefinition *tbldef )
        : empresa::MasterTable( tbldef ) {}
};	

} // namespace tesoreria
} // namespace gong

#endif // TESORERIAMASTERTABLE_H
