#ifndef _FACTU_MASTERTABLE_H
#define _FACTU_MASTERTABLE_H

#include "config.h"
#include <gongdbmastertable.h>
#include <empresamastertable.h>
#ifdef HAVE_CONTABMODULE
#include <contabmastertable.h>
#endif
#include "factufldivadetallado.h"

namespace gong {
namespace factu {

class FieldDefs
{
public:
    FieldDefs( dbMasterTable *mt ): pMasterTable( mt ) {}

    dbFieldDefinition *addFieldEuro4Dec( const Xtring &name ) {
        return pMasterTable->addFieldMoney( name, 12, 4 )->setStyle( "MONEY4" );
    }
    dbFieldDefinition *addFieldPercent4Dec( const Xtring &name ) {
        return pMasterTable->addFieldFloat( name, dbFieldDefinition::NOTNULL )
               ->setStyle( "PERCENT4" );
    }
    void addFieldsTotalesMaster( bool addentrega, bool addotrospagos ) {
        addFieldEuro4Dec( "SUMAIMPORTES" );
        addFieldPercent4Dec( "DTOP100" );
        addFieldEuro4Dec( "DESCUENTO" );
        addFieldEuro4Dec( "BASEIMPONIBLE" );
        addFieldEuro4Dec( "IVA" );
        addFieldEuro4Dec( "RECARGOEQUIVALENCIA" );
        _addFieldEuro( "TOTAL" );
        if( addentrega ) {
            _addFieldEuro( "ENTREGA" );
            _addFieldEuro( "RESTO" );
            if( addotrospagos ) {
                pMasterTable->addFieldDate( "FECHAPAGO" );
                pMasterTable->addFieldString( "DOCUMENTOPAGO", 50 );
#ifdef HAVE_CONTABMODULE 
                pMasterTable->addFieldOne2OneRelation( "CUENTAPAGO_ID", "CUENTA.ID" )->setCanBeNull(true);
#elif defined( HAVE_TESORERIAMODULE )
                pMasterTable->addFieldOne2OneRelation( "CUENTAPAGO_ID", "CUENTATESORERIA.ID" )->setCanBeNull(true);
#else				
                pMasterTable->addFieldString( "CUENTAPAGO", 20 );
#endif
            }
        }
    }
    void addFieldsDesgloseIVA() {
        pMasterTable->addFieldString( "DESGLOSEIVA", 200 )->setReadOnly( true );
    }
    void addFieldsComunesDetalles( bool compras ) {
        pMasterTable->addFieldInt( "NLINEA" );
        pMasterTable->addFieldFloat( "CANTIDAD" );
        pMasterTable->addFieldOne2OneRelation( "ARTICULO_ID", "ARTICULO.ID", true );
        if ( compras ) {
            addFieldEuro4Dec( "COSTESINIVA" );
            _addFieldEuro( "COSTE" );
        } else {
            addFieldEuro4Dec( "PVPSINIVA" );
            _addFieldEuro( "PVP" );
        }
        pMasterTable->addFieldPercent( "DTOP100" );
        addFieldEuro4Dec( "IMPORTE" );
        addFieldEuro4Dec( "IMPORTECONIVA" );
        pMasterTable->addFieldOne2OneRelation( "TIPOIVA_ID", "TIPOIVA.ID", true );
        pMasterTable->addFieldText( "DESCRIPCION" );
    }

private:
    dbFieldDefinition *_addFieldEuro( const Xtring &name ) {
        return pMasterTable->addFieldMoney( name, 12, 2 )->setStyle( "MONEY" );
    }
    dbMasterTable *pMasterTable;
};

class MasterTable: public empresa::MasterTable, public factu::FieldDefs
#ifdef HAVE_CONTABMODULE
    , public contab::FieldDefs
#endif
{
public:
    MasterTable( dbDefinition &db, const Xtring &name, const Xtring &from = Xtring::null,
                 const Xtring &where = Xtring::null, const Xtring &orderby = Xtring::null )
        : empresa::MasterTable( new dbTableDefinition( db, name,
                                from, where, orderby ) ),
        factu::FieldDefs( this )
#ifdef HAVE_CONTABMODULE
        , contab::FieldDefs( this )
#endif
    {}
    MasterTable( dbTableDefinition *tbldef )
        : empresa::MasterTable( tbldef ), factu::FieldDefs( this )
#ifdef HAVE_CONTABMODULE
        , contab::FieldDefs( this )
#endif
    {}
};

} // namespace factu
} // namespace gong

#endif
