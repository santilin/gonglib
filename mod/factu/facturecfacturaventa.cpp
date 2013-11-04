/*<<<<<COPYLEFT*/
/** @file facturecfacturaventa.cpp Registro de facturas de venta
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
// COPYLEFT Registro de facturas de venta
// MEMBER init
// MEMBER save
// MEMBER remove
// MEMBER toString
// RELATION pagos::FormaPago
// RELATION empresa::Proyecto
// RELATION TipoDoc
// RELATION Cliente
// RELATION Agente
// RELATION FacturaVentaDet Detalles
// INTERFACE public factu::IPagableFactura
// INTERFACE public factu::IIVADesglosable
// INTERFACE public factu::IAsentableFactura #ifdef|HAVE_CONTABMODULE
// INTERFACE public factu::ITotalizableRecord
// TYPE dbRecord factu::FacturaVenta
/*>>>>>MODULE_INFO*/

/*<<<<<FACTURAVENTA_INCLUDES*/
#include "facturecfacturaventa.h"
/*>>>>>FACTURAVENTA_INCLUDES*/
#include <empresamodule.h>
#include <factumodule.h>

#ifdef HAVE_CONTABMODULE
#include <contabmodule.h>
#include <contabrecasiento.h>
#endif

namespace gong {
namespace factu {

/*<<<<<FACTURAVENTA_INIT*/
void RecFacturaVenta::init()
{
/*>>>>>FACTURAVENTA_INIT*/
    addStructuralFilter( "FACTURAVENTA.EMPRESA_ID=" + getConnection()->toSQL( empresa::ModuleInstance->getEmpresaID() ) );
    addStructuralFilter( "FACTURAVENTA.EJERCICIO=" + getConnection()->toSQL( empresa::ModuleInstance->getEjercicio() ) );
}


/*<<<<<FACTURAVENTA_RELATIONS*/
pagos::RecFormaPago *RecFacturaVenta::getRecFormaPago() const
{
	return static_cast<pagos::RecFormaPago*>(findRelatedRecord("FACTURAVENTA.FORMAPAGO_ID"));
}

empresa::RecProyecto *RecFacturaVenta::getRecProyecto() const
{
	return static_cast<empresa::RecProyecto*>(findRelatedRecord("FACTURAVENTA.PROYECTO_ID"));
}

RecTipoDoc *RecFacturaVenta::getRecTipoDoc() const
{
	return static_cast<RecTipoDoc*>(findRelatedRecord("FACTURAVENTA.TIPODOC_ID"));
}

RecCliente *RecFacturaVenta::getRecCliente() const
{
	return static_cast<RecCliente*>(findRelatedRecord("FACTURAVENTA.CLIENTE_ID"));
}

RecAgente *RecFacturaVenta::getRecAgente() const
{
	return static_cast<RecAgente*>(findRelatedRecord("FACTURAVENTA.AGENTE_ID"));
}

RecFacturaVentaDet *RecFacturaVenta::getRecFacturaVentaDet( int facturaventadet ) const
{
	return static_cast<RecFacturaVentaDet*>(findRelationByRelatedTable("FACTURAVENTADET" )->getRelatedRecord( facturaventadet));
}

dbRecordList *RecFacturaVenta::getListFacturaVentaDet() const
{
	return findRelationByRelatedTable( "FACTURAVENTADET" )->getRelatedRecordList();
}
/*>>>>>FACTURAVENTA_RELATIONS*/


/*<<<<<FACTURAVENTA_TOSTRING*/
Xtring RecFacturaVenta::toString(int format, const RegExp &includedFields) const
{
	Xtring result;
/*>>>>>FACTURAVENTA_TOSTRING*/
    if( format == TOSTRING_CODE_AND_DESC_WITH_TABLENAME ) {
        result = dbRecord::toString( TOSTRING_CODE_AND_DESC_WITH_TABLENAME );
        if( !getRecCliente()->getValue("RAZONSOCIAL").toString().isEmpty() )
            result += ", " + getRecCliente()->getValue("RAZONSOCIAL").toString();
    } else
        result = dbRecord::toString(format, includedFields);
    return result;
}


/*<<<<<FACTURAVENTA_SAVE*/
bool RecFacturaVenta::save(bool saverelated) throw( dbError )
{
/*>>>>>FACTURAVENTA_SAVE*/
#ifdef HAVE_PAGOSMODULE
    actRestoFactura();
#endif
    bool ret = dbRecord::save(saverelated);
    if( ret && saverelated ) {
#ifdef HAVE_PAGOSMODULE
        delCobros();
        genCobros();
#endif
#ifdef HAVE_CONTABMODULE
        if( contab::ModuleInstance->isContabActive() ) {
            bool supervisar = contab::ModuleInstance->getModuleSetting( "SUPERVISAR_ASIENTOS" ).toBool();
            regenAsiento( supervisar );
        }
#endif
    }
    return ret;
}

/*<<<<<FACTURAVENTA_REMOVE*/
bool RecFacturaVenta::remove() throw( dbError )
{
/*>>>>>FACTURAVENTA_REMOVE*/
    bool ret = dbRecord::remove();
    if( ret ) {
#ifdef HAVE_PAGOSMODULE
        delCobros();
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

