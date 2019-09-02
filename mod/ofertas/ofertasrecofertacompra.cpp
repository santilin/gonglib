/*<<<<<COPYLEFT*/
/** @file ofertasrecofertacompra.cpp Registro de ofertas de compra
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
// COPYLEFT Registro de ofertas de compra
// MEMBER init
// MEMBER save
// MEMBER remove
// MEMBER toString
// RELATION CuentaOferta #ifdef|HAVE_TESORERIAMODULE
// RELATION factu::TipoDoc
// RELATION factu::Proveedora
// RELATION factu::Agente
// RELATION pagos::FormaPago
// RELATION empresa::Proyecto
// RELATION OfertaCompraDet Detalles
// INTERFACE public ofertas::IPagableOferta
// INTERFACE public factu::IIVADesglosable
// INTERFACE public factu::ITotalizableRecord
// INTERFACE public tesoreria::IApuntableRecord #ifdef|HAVE_TESORERIAMODULE
// TYPE dbRecord ofertas::OfertaCompra
/*>>>>>MODULE_INFO*/

#include "ofertasmodule.h"
/*<<<<<OFERTACOMPRA_INCLUDES*/
#include "ofertasrecofertacompra.h"
/*>>>>>OFERTACOMPRA_INCLUDES*/

namespace gong {
namespace ofertas {

/*<<<<<OFERTACOMPRA_INIT*/
void RecOfertaCompra::init()
{
/*>>>>>OFERTACOMPRA_INIT*/
    addStructuralFilter( "OFERTACOMPRA.EMPRESA_ID=" + getConnection()->toSQL( empresa::ModuleInstance->getEmpresaID() ) );
    addStructuralFilter( "OFERTACOMPRA.EJERCICIO=" + getConnection()->toSQL( empresa::ModuleInstance->getEjercicio() ) );
}

/*<<<<<OFERTACOMPRA_RELATIONS$s/CUENTAOFERTA/CUENTAPAGO/$*/
#ifdef HAVE_TESORERIAMODULE
RecCuentaOferta *RecOfertaCompra::getRecCuentaOferta() const
{
	return static_cast<RecCuentaOferta*>(findRelatedRecord("CUENTAPAGO_ID"));
}

#endif
factu::RecTipoDoc *RecOfertaCompra::getRecTipoDoc() const
{
	return static_cast<factu::RecTipoDoc*>(findRelatedRecord("TIPODOC_ID"));
}

factu::RecProveedora *RecOfertaCompra::getRecProveedora() const
{
	return static_cast<factu::RecProveedora*>(findRelatedRecord("PROVEEDORA_ID"));
}

factu::RecAgente *RecOfertaCompra::getRecAgente() const
{
	return static_cast<factu::RecAgente*>(findRelatedRecord("AGENTE_ID"));
}

pagos::RecFormaPago *RecOfertaCompra::getRecFormaPago() const
{
	return static_cast<pagos::RecFormaPago*>(findRelatedRecord("FORMAPAGO_ID"));
}

empresa::RecProyecto *RecOfertaCompra::getRecProyecto() const
{
	return static_cast<empresa::RecProyecto*>(findRelatedRecord("PROYECTO_ID"));
}

RecOfertaCompraDet *RecOfertaCompra::getRecOfertaCompraDet( int ofertacompradet ) const
{
	return static_cast<RecOfertaCompraDet*>(findRelationByRelatedTable("OFERTACOMPRADET" )->getRelatedRecord( ofertacompradet));
}

dbRecordList *RecOfertaCompra::getListOfertaCompraDet() const
{
	return findRelationByRelatedTable( "OFERTACOMPRADET" )->getRelatedRecordList();
}
/*>>>>>OFERTACOMPRA_RELATIONS*/

/*<<<<<OFERTACOMPRA_TOSTRING*/
Xtring RecOfertaCompra::toString(int format, const Xtring &includedFields) const
{
	Xtring result;
/*>>>>>OFERTACOMPRA_TOSTRING*/
    if( format == TOSTRING_CODE_AND_DESC_WITH_TABLENAME ) {
        result = dbRecord::toString( TOSTRING_CODE_AND_DESC_WITH_TABLENAME );
        if( !getRecProveedora()->getValue("RAZONSOCIAL").toString().isEmpty() )
            result += ", " + getRecProveedora()->getValue("RAZONSOCIAL").toString();
    } else
        result = dbRecord::toString(format, includedFields);
    return result;
}

/*<<<<<OFERTACOMPRA_SAVE*/
bool RecOfertaCompra::save(bool validate, bool saverelated = true)/* throw( dbError ) */
{
/*>>>>>OFERTACOMPRA_SAVE*/
    if( getValue( "CONTADOR" ).toInt() == 0 ) {
        setValue( "CONTADOR", ModuleInstance->getMaxContador() );
	}
	if( saverelated ) {
#ifdef HAVE_PAGOSMODULE
		if( ModuleInstance->getPagosModule() ) {
			actRestoFactura();
		}
#endif
	}
    bool ret = dbRecord::save(validate, saverelated);
    if( ret && saverelated ) {
#ifdef HAVE_PAGOSMODULE
		if( ModuleInstance->getPagosModule() ) {
			delPagos( false );
			genPagos();
		}
#endif
#ifdef HAVE_TESORERIAMODULE
		if( ModuleInstance->getTesoreriaModule() ) {
            bool supervisar = tesoreria::ModuleInstance->getModuleSetting( "SUPERVISAR_APUNTES" ).toBool();
            regenApunte( supervisar );
        }
#endif
    }
    if( ret )
        DBAPP->showStickyOSD( toString( TOSTRING_CODE_AND_DESC_WITH_TABLENAME ),
			Xtring::printf( _("Contador: %d"), getValue( "CONTADOR" ).toInt() ) );
    return ret;
}

/*<<<<<OFERTACOMPRA_REMOVE*/
bool RecOfertaCompra::remove()/* throw( dbError ) */
{
/*>>>>>OFERTACOMPRA_REMOVE*/
    bool ret = dbRecord::remove();
    if( ret ) {
#ifdef HAVE_PAGOSMODULE
		if( ModuleInstance->getPagosModule() ) {
			delPagos( true );
		}
#endif
#ifdef HAVE_TESORERIAMODULE
		if( ModuleInstance->getTesoreriaModule() ) {
            delete borraApunte( false /*no regenerando*/);
        }
#endif
    }
    return ret;
}

} // namespace ofertas
} // namespace gong
