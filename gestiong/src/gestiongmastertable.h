#ifndef GESTIONGMASTERTABLE_H
#define GESTIONGMASTERTABLE_H

#include <gongdbmastertable.h>

namespace gong {
namespace gestiong {

class MasterTable: public gong::dbMasterTable
{
public:
    MasterTable(dbDefinition &db, const gong::Xtring& name,
                const gong::Xtring &from = gong::Xtring(), const gong::Xtring& where = gong::Xtring(),
                const gong::Xtring& orderby= gong::Xtring() )
        : dbMasterTable(new dbTableDefinition(db, name, from, where, orderby ))
    {}
    MasterTable( dbTableDefinition *tbldef ): dbMasterTable( tbldef ) {}
};

} // namespace gestiong
} // namespace gong

#endif // GESTIONGMASTERTABLE

