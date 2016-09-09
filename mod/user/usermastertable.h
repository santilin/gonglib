#ifndef _USER_MASTERTABLE_H
#define _USER_MASTERTABLE_H

#include <gongdbmastertable.h>

namespace gong {
namespace user {

class MasterTable: public dbMasterTable
{
public:
    MasterTable(dbDefinition &db, const Xtring &name,
                const Xtring &from = Xtring::null,
                const Xtring &where = Xtring::null, const Xtring &orderby = Xtring::null)
        : dbMasterTable( new dbTableDefinition( db, name, from, where, orderby ) ) {}
    MasterTable( dbTableDefinition *tbldef )
        : dbMasterTable( tbldef ) {}
    dbFieldDefinition *addFieldNotas(const Xtring &name="NOTAS",
                                     dbFieldDefinition::Flags flags = dbFieldDefinition::NONE) {
        return addFieldText(name, flags);
    }
};

} // namespace pagos
} // namespace gong

#endif // _PAGOS_MASTERTABLE_H
