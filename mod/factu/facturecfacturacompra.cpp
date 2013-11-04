/*<<<<<COPYLEFT*/
/** @file facturecfacturacompra.cpp Registro de facturas de compra
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */
/*>>>>>COPYLEFT*/
/*<<<<<MODULE_INFO*/
// COPYLEFT Registro de facturas de compra
// MEMBER init
// MEMBER save
// MEMBER remove
// MEMBER toString
// RELATION pagos::FormaPago
// RELATION empresa::Proyecto
// RELATION TipoDoc
// RELATION Proveedora
// RELATION Agente
// RELATION FacturaCompraDet Detalles
// INTERFACE public factu::IPagableFactura
// INTERFACE public factu::IIVADesglosable
// INTERFACE public factu::IAsentableFactura #ifdef|HAVE_CONTABMODULE
// INTERFACE public factu::ITotalizableRecord
// TYPE dbRecord factu::FacturaCompra
/*>>>>>MODULE_INFO*/

/*<<<<<FACTURACOMPRA_INCLUDES*/
#include "facturecfacturacompra.h"
/*>>>>>FACTURACOMPRA_INCLUDES*/
#ifdef HAVE_CONTABMODULE
#include <contabmodule.h>
#include <contabrecasiento.h>
#include <contabreccuenta.h>
#endif
#include <empresamodule.h>
#include "factumodule.h"

namespace gong {
namespace factu {

/*<<<<<FACTURACOMPRA_INIT*/
void RecFacturaCompra::init()
{
/*>>>>>FACTURACOMPRA_INIT*/
    addStructuralFilter( "FACTURACOMPRA.EMPRESA_ID=" + getConnection()->toSQL( empresa::ModuleInstance->getEmpresaID() ) );
    addStructuralFilter( "FACTURACOMPRA.EJERCICIO=" + getConnection()->toSQL( empresa::ModuleInstance->getEjercicio() ) );
}

/*<<<<<FACTURACOMPRA_RELATIONS*/
pagos::RecFormaPago *RecFacturaCompra::getRecFormaPago() const
{
	return static_cast<pagos::RecFormaPago*>(findRelatedRecord("FACTURACOMPRA.FORMAPAGO_ID"));
}

empresa::RecProyecto *RecFacturaCompra::getRecProyecto() const
{
	return static_cast<empresa::RecProyecto*>(findRelatedRecord("FACTURACOMPRA.PROYECTO_ID"));
}

RecTipoDoc *RecFacturaCompra::getRecTipoDoc() const
{
	return static_cast<RecTipoDoc*>(findRelatedRecord("FACTURACOMPRA.TIPODOC_ID"));
}

RecProveedora *RecFacturaCompra::getRecProveedora() const
{
	return static_cast<RecProveedora*>(findRelatedRecord("FACTURACOMPRA.PROVEEDORA_ID"));
}

RecAgente *RecFacturaCompra::getRecAgente() const
{
	return static_cast<RecAgente*>(findRelatedRecord("FACTURACOMPRA.AGENTE_ID"));
}

RecFacturaCompraDet *RecFacturaCompra::getRecFacturaCompraDet( int facturacompradet ) const
{
	return static_cast<RecFacturaCompraDet*>(findRelationByRelatedTable("FACTURACOMPRADET" )->getRelatedRecord( facturacompradet));
}

dbRecordList *RecFacturaCompra::getListFacturaCompraDet() const
{
	return findRelationByRelatedTable( "FACTURACOMPRADET" )->getRelatedRecordList();
}
/*>>>>>FACTURACOMPRA_RELATIONS*/

/*<<<<<FACTURACOMPRA_TOSTRING*/
Xtring RecFacturaCompra::toString(int format, const RegExp &includedFields) const
{
	Xtring result;
/*>>>>>FACTURACOMPRA_TOSTRING*/
    if( format == TOSTRING_CODE_AND_DESC_WITH_TABLENAME ) {
        result = dbRecord::toString( TOSTRING_CODE_AND_DESC_WITH_TABLENAME );
        if( !getRecProveedora()->getValue("RAZONSOCIAL").toString().isEmpty() )
            result += ", " + getRecProveedora()->getValue("RAZONSOCIAL").toString();
    } else
        result = dbRecord::toString(format, includedFields);
    return result;
}

/*<<<<<FACTURACOMPRA_SAVE*/
bool RecFacturaCompra::save(bool saverelated) throw( dbError )
{
/*>>>>>FACTURACOMPRA_SAVE*/
    if( getValue( "CONTADOR" ).toInt() == 0 )
        setValue( "CONTADOR", empresa::ModuleInstance->getMaxContador() );
#ifdef HAVE_PAGOSMODULE
    actRestoFactura();
#endif
    bool ret = dbRecord::save(saverelated);
    if( ret && saverelated ) {
#ifdef HAVE_PAGOSMODULE
        delPagos();
        genPagos();
#endif
#ifdef HAVE_CONTABMODULE
        if( contab::ModuleInstance->isContabActive() ) {
            bool supervisar = contab::ModuleInstance->getModuleSetting( "SUPERVISAR_ASIENTOS" ).toBool();
            regenAsiento( supervisar );
        }
#endif
    }
    if( ret )
        DBAPP->showStickyOSD( toString( TOSTRING_CODE_AND_DESC_WITH_TABLENAME ),
                              Xtring::printf( _("Contador: %d"), getValue( "CONTADOR" ).toInt() ) );
    return ret;
}

/*<<<<<FACTURACOMPRA_REMOVE*/
bool RecFacturaCompra::remove() throw( dbError )
{
/*>>>>>FACTURACOMPRA_REMOVE*/
    bool ret = dbRecord::remove();
    if( ret ) {
#ifdef HAVE_PAGOSMODULE
        delPagos();
#endif
#ifdef HAVE_CONTABMODULE
        if( contab::ModuleInstance->isContabActive() )
            delete borraAsiento();
#endif
    }
    return ret;
}

} // namespace factu
} // namespace gong
