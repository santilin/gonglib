/*<<<<<COPYLEFT*/
/** @file facturecalbarancompra.cpp Registro de albaranes de compra
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
// COPYLEFT Registro de albaranes de compra
// MEMBER init
// MEMBER save
// MEMBER remove
// MEMBER toString
// RELATION TipoDoc
// RELATION Proveedora
// RELATION Agente
// RELATION pagos::FormaPago
// RELATION empresa::Proyecto
// RELATION AlbaranCompraDet Detalles
// INTERFACE public factu::IPagableAlbaran
// INTERFACE public factu::IIVADesglosable
// INTERFACE public factu::IAsentableFactura #ifdef|HAVE_CONTABMODULE
// INTERFACE public factu::ITotalizableRecord
// TYPE dbRecord factu::AlbaranCompra
/*>>>>>MODULE_INFO*/

/*<<<<<ALBARANCOMPRA_INCLUDES*/
#include "facturecalbarancompra.h"
/*>>>>>ALBARANCOMPRA_INCLUDES*/
#include <empresamodule.h>
#ifdef HAVE_CONTABMODULE
#include <contabrecasiento.h>
#include <contabmodule.h>
#endif
#include "factumodule.h"

namespace gong {
namespace factu {

/*<<<<<ALBARANCOMPRA_INIT*/
void RecAlbaranCompra::init()
{
    /*>>>>>ALBARANCOMPRA_INIT*/
    addStructuralFilter( "ALBARANCOMPRA.EMPRESA_ID=" + getConnection()->toSQL( empresa::ModuleInstance->getEmpresaID() ) );
    addStructuralFilter( "ALBARANCOMPRA.EJERCICIO=" + getConnection()->toSQL( empresa::ModuleInstance->getEjercicio() ) );
}

/*<<<<<ALBARANCOMPRA_RELATIONS*/
RecTipoDoc *RecAlbaranCompra::getRecTipoDoc() const
{
    return static_cast<RecTipoDoc*>(findRelatedRecord("ALBARANCOMPRA.TIPODOC_ID"));
}

RecProveedora *RecAlbaranCompra::getRecProveedora() const
{
    return static_cast<RecProveedora*>(findRelatedRecord("ALBARANCOMPRA.PROVEEDORA_ID"));
}

RecAgente *RecAlbaranCompra::getRecAgente() const
{
    return static_cast<RecAgente*>(findRelatedRecord("ALBARANCOMPRA.AGENTE_ID"));
}

pagos::RecFormaPago *RecAlbaranCompra::getRecFormaPago() const
{
    return static_cast<pagos::RecFormaPago*>(findRelatedRecord("ALBARANCOMPRA.FORMAPAGO_ID"));
}

empresa::RecProyecto *RecAlbaranCompra::getRecProyecto() const
{
    return static_cast<empresa::RecProyecto*>(findRelatedRecord("ALBARANCOMPRA.PROYECTO_ID"));
}

RecAlbaranCompraDet *RecAlbaranCompra::getRecAlbaranCompraDet( int albarancompradet ) const
{
    return static_cast<RecAlbaranCompraDet*>(findRelationByRelatedTable("ALBARANCOMPRADET" )->getRelatedRecord( albarancompradet));
}

dbRecordList *RecAlbaranCompra::getListAlbaranCompraDet() const
{
    return findRelationByRelatedTable( "ALBARANCOMPRADET" )->getRelatedRecordList();
}
/*>>>>>ALBARANCOMPRA_RELATIONS*/

/*<<<<<ALBARANCOMPRA_TOSTRING*/
Xtring RecAlbaranCompra::toString(int format, const RegExp &includedFields) const
{
    Xtring result;
    /*>>>>>ALBARANCOMPRA_TOSTRING*/
    if( format == TOSTRING_CODE_AND_DESC_WITH_TABLENAME ) {
        result = dbRecord::toString( TOSTRING_CODE_AND_DESC_WITH_TABLENAME );
        if( !getRecProveedora()->getValue("RAZONSOCIAL").toString().isEmpty() )
            result += ", " + getRecProveedora()->getValue("RAZONSOCIAL").toString();
    } else
        result = dbRecord::toString(format, includedFields);
    return result;
}

/*<<<<<ALBARANCOMPRA_SAVE*/
bool RecAlbaranCompra::save(bool saverelated) throw( dbError )
{
    /*>>>>>ALBARANCOMPRA_SAVE*/
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

/*<<<<<ALBARANCOMPRA_REMOVE*/
bool RecAlbaranCompra::remove() throw( dbError )
{
    /*>>>>>ALBARANCOMPRA_REMOVE*/
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
