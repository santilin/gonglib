#ifndef GONGSOCIASMASTERTABLE_H
#define GONGSOCIASMASTERTABLE_H

#include <empresamastertable.h>

namespace gong {
namespace socias {

class FieldDefs
{
public:
    FieldDefs( dbMasterTable *mt ): pMasterTable( mt ) {}
private:
    dbMasterTable *pMasterTable;
};


class MasterTable: public empresa::MasterTable, public socias::FieldDefs
{
public:
    MasterTable(dbDefinition &db, const Xtring &name, const Xtring &from = Xtring::null,
                const Xtring &where = Xtring::null, const Xtring &orderby = Xtring::null)
        : empresa::MasterTable( db, name, from, where, orderby ),
          socias::FieldDefs( this ) {}
    MasterTable( dbTableDefinition *tbldef )
        : empresa::MasterTable( tbldef ), socias::FieldDefs( this ) {}
};

} // namespace gong
} // namespace socias

#endif
