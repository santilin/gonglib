#ifndef _CONTACTOS_MASTERTABLE_H
#define _CONTACTOS_MASTERTABLE_H

#include <gongdbfieldemail.h>
#include <gongdbmastertable.h>
#include "contactosfldcif.h"

namespace gong {
namespace contactos {

class FieldDefs
{
public:
    FieldDefs( dbMasterTable *mt ): pMasterTable(mt) {}

    dbFieldDefinition *addFieldNotas(const Xtring &name="NOTAS",
                                     dbFieldDefinition::Flags flags = dbFieldDefinition::NONE)
    {
        return pMasterTable->addFieldText(name, flags);
    }
    dbFieldDefinition *addFieldTelefono(const Xtring &name="TELEFONO",
                                        dbFieldDefinition::Flags flags = dbFieldDefinition::NONE)
    {
        return pMasterTable->addFieldText(name, flags);
    }
    dbFieldDefinition *addFieldDireccion(const Xtring &name="DIRECCION",
                                         dbFieldDefinition::Flags flags = dbFieldDefinition::NONE)
    {
        return pMasterTable->addFieldString(name, 100, flags);
    }
    dbFieldDefinition *addFieldCodigoPostal(const Xtring &name="CP",
                                            dbFieldDefinition::Flags flags = dbFieldDefinition::NONE)
    {
        return pMasterTable->addFieldString(name, 10, flags);
    }
    dbFieldDefinition *addFieldLocalidad(const Xtring &name="LOCALIDAD",
                                         dbFieldDefinition::Flags flags = dbFieldDefinition::NONE)
    {
        return pMasterTable->addFieldString(name, 80, flags);
    }
    dbFieldDefinition *addFieldProvincia(const Xtring &name="PROVINCIA",
                                         dbFieldDefinition::Flags flags = dbFieldDefinition::NONE)
    {
        return pMasterTable->addFieldString(name, 30, flags);
    }
    dbFieldDefinition *addFieldPais(const Xtring &name="PAIS",
                                    dbFieldDefinition::Flags flags = dbFieldDefinition::NONE)
    {
        return pMasterTable->addFieldString(name, 30, flags);
    }

private:
    dbMasterTable *pMasterTable;
};


class MasterTable: public dbMasterTable, public FieldDefs
{
public:
    MasterTable(dbDefinition &db, const Xtring &name,
                const Xtring &from = Xtring::null,
                const Xtring &where = Xtring::null, const Xtring &orderby = Xtring::null)
        : dbMasterTable( new dbTableDefinition( db, name, from, where, orderby ) ),
          FieldDefs( this )
    {}
    MasterTable( dbTableDefinition *tbldef )
        : dbMasterTable( tbldef ), FieldDefs( this ) {}
};

} // namespace contactos
} // namespace gong

#endif
