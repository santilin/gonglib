#ifndef DEPOSITMASTERTABLE_H
#define DEPOSITMASTERTABLE_H

#include <factumastertable.h>

namespace gong {
namespace deposit {

class MasterTable: public factu::MasterTable
{
public:
    MasterTable( dbDefinition &db, const Xtring &name, const Xtring &from = Xtring::null,
                       const Xtring &where = Xtring::null, const Xtring &orderby = Xtring::null )
            : factu::MasterTable( db, name, from, where, orderby ) {}
    MasterTable( dbTableDefinition *tbldef )
            : factu::MasterTable( tbldef ) {}

};


} // namespace deposit
} // namespace gong


#endif // DEPOSITMASTERTABLE
