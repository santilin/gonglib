#ifndef _EMPRESA_MASTERTABLE_H
#define _EMPRESA_MASTERTABLE_H

#include <gongdbmastertable.h>
#include <contactosmastertable.h>
#include "empresafldempresaid.h"
#include "empresafldejercicio.h"
#include "empresafldcuentabanco.h"

namespace gong
{
namespace empresa {

class FieldDefs
{
public:
    FieldDefs( dbMasterTable *mt ): pMasterTable( mt ) {}

    dbFieldDefinition *addFieldCodigo( const Xtring &name = "CODIGO" ) {
        return pMasterTable->addFieldIntCode( name );
    }
    dbFieldDefinition *addFieldEuro( const Xtring &name ) {
        return pMasterTable->addFieldMoney( name, 12, 2 )->setStyle( "MONEY" );
    }
    dbFieldDefinition *addFieldFecha( const Xtring &name = "FECHA" ) {
        return pMasterTable->addFieldDate( name );
    }
    FldEmpresaID *addFieldEmpresaID( const Xtring &name = "EMPRESA_ID",
                                     dbFieldDefinition::Flags flags = dbFieldDefinition::NONE,
                                     const Xtring &reference = "EMPRESA.ID" ) {
        FldEmpresaID *fld = static_cast<FldEmpresaID *>
                            ( pMasterTable->addField( new FldEmpresaID( pMasterTable->getTableDefinition()->getName(),
                                    name, ( flags | dbFieldDefinition::INVISIBLE), reference ) ) );
#if 0
        // Ver notas en empresamodule.cpp
        Xtring reftable = dbFieldDefinition::extractTableName( reference );
        Xtring reffield = dbFieldDefinition::extractFieldName( reference );
        pMasterTable->addRelation( dbRelationDefinition::one2one,
                                   pMasterTable->getTableDefinition()->getName(),
                                   name, reftable, reffield );
#endif
        return fld;
    }

private:
    dbMasterTable *pMasterTable;
};


class MasterTable: public contactos::MasterTable, public empresa::FieldDefs
{
public:
    MasterTable( dbDefinition &db, const Xtring &name, const Xtring &from = Xtring::null,
                 const Xtring &where = Xtring::null, const Xtring &orderby = Xtring::null )
        : contactos::MasterTable( db, name, from, where, orderby ), empresa::FieldDefs( this ) {}
    MasterTable( dbTableDefinition *tbldef )
        : contactos::MasterTable( tbldef ), empresa::FieldDefs( this ) {}
};

} // namespace empresa
} // namespace gong

#endif
