/*<<<<<COPYLEFT*/
/** @file ofertasrecofertaventa.cpp Registro de ofertas de venta
 * GongLib project. (L) Santiago Capel Torres, 2003-2011
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
/*>>>>>COPYLEFT*/
/*<<<<<MODULE_INFO*/
// COPYLEFT Registro de ofertas de venta
// MEMBER init
// MEMBER save
// MEMBER remove
// MEMBER toString
// RELATION CuentaOferta #ifdef|HAVE_TESORERIAMODULE
// RELATION factu::TipoDoc
// RELATION factu::Cliente
// RELATION factu::Agente
// RELATION pagos::FormaPago
// RELATION empresa::Proyecto
// RELATION OfertaVentaDet Detalles
// INTERFACE public ofertas::IPagableOferta
// INTERFACE public factu::IIVADesglosable
// INTERFACE public factu::ITotalizableRecord
// INTERFACE public tesoreria::IApuntableRecord #ifdef|HAVE_TESORERIAMODULE
// TYPE dbRecord ofertas::OfertaVenta
/*>>>>>MODULE_INFO*/

/*<<<<<OFERTAVENTA_INCLUDES*/
#include "ofertasrecofertaventa.h"
/*>>>>>OFERTAVENTA_INCLUDES*/
#include <empresamodule.h>
#include <factuipagables.h>
#include <factuiivadesglosable.h>
#include <factuitotalizablerecord.h>

namespace gong {
namespace ofertas {

/*<<<<<OFERTAVENTA_INIT*/
void RecOfertaVenta::init()
{
/*>>>>>OFERTAVENTA_INIT*/
    addStructuralFilter( "OFERTAVENTA.EMPRESA_ID=" + getConnection()->toSQL( empresa::ModuleInstance->getEmpresaID() ) );
    addStructuralFilter( "OFERTAVENTA.EJERCICIO=" + getConnection()->toSQL( empresa::ModuleInstance->getEjercicio() ) );
}

/*<<<<<OFERTAVENTA_RELATIONS$s/CUENTAOFERTA/CUENTAPAGO/$*/
#ifdef HAVE_TESORERIAMODULE
RecCuentaOferta *RecOfertaVenta::getRecCuentaOferta() const
{
	return static_cast<RecCuentaOferta*>(findRelatedRecord("CUENTAPAGO_ID"));
}

#endif
factu::RecTipoDoc *RecOfertaVenta::getRecTipoDoc() const
{
	return static_cast<factu::RecTipoDoc*>(findRelatedRecord("TIPODOC_ID"));
}

factu::RecCliente *RecOfertaVenta::getRecCliente() const
{
	return static_cast<factu::RecCliente*>(findRelatedRecord("CLIENTE_ID"));
}

factu::RecAgente *RecOfertaVenta::getRecAgente() const
{
	return static_cast<factu::RecAgente*>(findRelatedRecord("AGENTE_ID"));
}

pagos::RecFormaPago *RecOfertaVenta::getRecFormaPago() const
{
	return static_cast<pagos::RecFormaPago*>(findRelatedRecord("FORMAPAGO_ID"));
}

empresa::RecProyecto *RecOfertaVenta::getRecProyecto() const
{
	return static_cast<empresa::RecProyecto*>(findRelatedRecord("PROYECTO_ID"));
}

RecOfertaVentaDet *RecOfertaVenta::getRecOfertaVentaDet( int ofertaventadet ) const
{
	return static_cast<RecOfertaVentaDet*>(findRelationByRelatedTable("OFERTAVENTADET" )->getRelatedRecord( ofertaventadet));
}

dbRecordList *RecOfertaVenta::getListOfertaVentaDet() const
{
	return findRelationByRelatedTable( "OFERTAVENTADET" )->getRelatedRecordList();
}
/*>>>>>OFERTAVENTA_RELATIONS*/

/*<<<<<OFERTAVENTA_TOSTRING*/
Xtring RecOfertaVenta::toString(int format, const Xtring &includedFields) const
{
	Xtring result;
/*>>>>>OFERTAVENTA_TOSTRING*/
    if( format == TOSTRING_CODE_AND_DESC_WITH_TABLENAME ) {
        result = dbRecord::toString( TOSTRING_CODE_AND_DESC_WITH_TABLENAME );
        if( !getRecCliente()->getValue("RAZONSOCIAL").toString().isEmpty() )
            result += ", " + getRecCliente()->getValue("RAZONSOCIAL").toString();
    } else
        result = dbRecord::toString(format, includedFields);
    return result;
}

/*<<<<<OFERTAVENTA_SAVE*/
bool RecOfertaVenta::save(bool validate, bool saverelated = true)/* throw( dbError ) */
{
/*>>>>>OFERTAVENTA_SAVE*/
#ifdef HAVE_PAGOSMODULE
		if( DBAPP->findModule("pagos") )
			actRestoFactura();
#endif
    bool ret = dbRecord::save(validate, saverelated);
    if( ret && saverelated ) {
#ifdef HAVE_PAGOSMODULE
		if( DBAPP->findModule("pagos") ) {
			delCobros( false );
			genCobros();
		}
#endif
#ifdef HAVE_TESORERIAMODULE
        if( DBAPP->findModule("tesoreria") ) {
            bool supervisar = tesoreria::ModuleInstance->getModuleSetting( "SUPERVISAR_APUNTES" ).toBool();
            regenApunte( supervisar );
        }
#endif
    }
    return ret;
}

/*<<<<<OFERTAVENTA_REMOVE*/
bool RecOfertaVenta::remove()/* throw( dbError ) */
{
/*>>>>>OFERTAVENTA_REMOVE*/
    bool ret = dbRecord::remove();
    if( ret ) {
#ifdef HAVE_PAGOSMODULE
		if( DBAPP->findModule("pagos") ) 
			delCobros( true );
#endif
#ifdef HAVE_TESORERIAMODULE
        if( DBAPP->findModule("tesoreria") ) {
            delete borraApunte( false /*no regenerando*/);
        }
#endif
    }
    return ret;
}

} // namespace ofertas
} // namespace gong
