/*<<<<<COPYLEFT*/
/** @file facturecfacturacompra.h Registro de facturas de compra
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */
/*>>>>>COPYLEFT*/
/*<<<<<FACTURACOMPRA_PREAMBLE*/
#ifndef _FACTU_FACTURECFACTURACOMPRA_H
#define _FACTU_FACTURECFACTURACOMPRA_H
/*>>>>>FACTURACOMPRA_PREAMBLE*/

#include "config.h"
/*<<<<<FACTURACOMPRA_INCLUDES*/
#include <dbappdbapplication.h>
#include <gongregexp.h>
#include "pagosrecformapago.h"
#include "empresarecproyecto.h"
#include "facturectipodoc.h"
#include "facturecproveedora.h"
#include "facturecagente.h"
#include "facturecfacturacompradet.h"
/*>>>>>FACTURACOMPRA_INCLUDES*/
#include "factuipagables.h"
#include "factuiivadesglosable.h"
#include "factuitotalizablerecord.h"

#ifdef HAVE_CONTABMODULE
#include <contabrecasiento.h>
#include "factuiasentables.h"
#endif

namespace gong {
namespace factu {

/*<<<<<FACTURACOMPRA_CONSTRUCTOR*/
class RecFacturaCompra: public dbRecord,
    public factu::IPagableFactura,
    public factu::IIVADesglosable,
#ifdef HAVE_CONTABMODULE
    public factu::IAsentableFactura,
#endif
    public factu::ITotalizableRecord

{
public:
    RecFacturaCompra(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
        : dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("FACTURACOMPRA"), recid, user)
/*>>>>>FACTURACOMPRA_CONSTRUCTOR*/
        , IPagableFactura( this, IPagableRecord::pagos )
        , IIVADesglosable( this, getListFacturaCompraDet() )
#ifdef HAVE_CONTABMODULE
        , IAsentableFactura( this, IAsentableFactura::facturacompra )
#endif
        , ITotalizableRecord( this, getListFacturaCompraDet(), compra )
    {
        addSemanticProperty( "COMPRA" );
        addSemanticProperty( "FACTURA" );
    }
    /*<<<<<FACTURACOMPRA_MEMBERS*/
    void init();
    virtual bool save(bool saverelated) throw( dbError ); // from dbRecord
    virtual bool remove() throw( dbError ); // from dbRecord
    Xtring toString(int format, const RegExp &includedFields=RegExp()) const;
    /*>>>>>FACTURACOMPRA_MEMBERS*/
public:
    /*<<<<<FACTURACOMPRA_RELATIONS*/
    pagos::RecFormaPago *getRecFormaPago() const;
    empresa::RecProyecto *getRecProyecto() const;
    RecTipoDoc *getRecTipoDoc() const;
    RecProveedora *getRecProveedora() const;
    RecAgente *getRecAgente() const;
    RecFacturaCompraDet *getRecFacturaCompraDet( int nfacturacompradet = -1 ) const;
    dbRecordList *getListFacturaCompraDet() const;
    /*>>>>>FACTURACOMPRA_RELATIONS*/
}; // end class

/*<<<<<FACTURACOMPRA_POSTAMBLE*/
} // namespace factu
} // namespace gong
#endif // FACTU_FACTURECFACTURACOMPRA_H
/*>>>>>FACTURACOMPRA_POSTAMBLE*/
