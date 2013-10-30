#ifndef _CONTAB_MASTERTABLE_H
#define _CONTAB_MASTERTABLE_H

#include <gongdbmastertable.h>
#include <empresamastertable.h>
#include "contabfldcuenta.h"


namespace gong {
namespace contab {

class FieldDefs
{
public:
    FieldDefs( dbMasterTable *mt ): pMasterTable(mt) {}
private:
    dbMasterTable *pMasterTable;
};

class MasterTable: public empresa::MasterTable, public contab::FieldDefs
{
public:
    MasterTable(dbDefinition &db, const Xtring &name, const Xtring &from = Xtring::null,
                const Xtring &where = Xtring::null, const Xtring &orderby = Xtring::null)
        : empresa::MasterTable( db, name, from, where, orderby ),
          contab::FieldDefs( this )
    {}
    MasterTable( dbTableDefinition *tbldef )
        : empresa::MasterTable( tbldef ), contab::FieldDefs( this ) {}

};

} // namespace contab
} // namespace gong

#endif
