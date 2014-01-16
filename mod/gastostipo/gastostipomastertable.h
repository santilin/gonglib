#ifndef GASTOSMASTERTABLE_H
#define GASTOSMASTERTABLE_H

#include <empresamastertable.h>

namespace gong {
namespace gastostipo {

class MasterTable: public empresa::MasterTable
{
public:
    MasterTable( dbDefinition &db, const Xtring &name, const Xtring &from = Xtring::null,
                 const Xtring &where = Xtring::null, const Xtring &orderby = Xtring::null )
        : empresa::MasterTable( db, name, from, where, orderby ) {}
    MasterTable( dbTableDefinition *tbldef )
        : empresa::MasterTable( tbldef ) {}
};

} // namespace gastos
} // namespace gong

#endif // GASTOSMASTERTABLE_H
