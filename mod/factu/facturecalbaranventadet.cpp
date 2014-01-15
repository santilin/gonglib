/*<<<<<MODULE_INFO*/
// COPYLEFT Registro de detalles de albaranes de venta
// MEMBER beforeSaveOrDeleteRelated
// RELATION Articulo
// RELATION empresa::TipoIVA
// TYPE dbRecord factu::AlbaranVentaDet
/*>>>>>MODULE_INFO*/

/*<<<<<ALBARANVENTADET_INCLUDES*/
#include "facturecalbaranventadet.h"
/*>>>>>ALBARANVENTADET_INCLUDES*/
#include "facturecalbaranventa.h"

namespace gong {
namespace factu {

/*<<<<<ALBARANVENTADET_RELATIONS*/
RecArticulo *RecAlbaranVentaDet::getRecArticulo() const
{
	return static_cast<RecArticulo*>(findRelatedRecord("ARTICULO_ID"));
}

empresa::RecTipoIVA *RecAlbaranVentaDet::getRecTipoIVA() const
{
	return static_cast<empresa::RecTipoIVA*>(findRelatedRecord("TIPOIVA_ID"));
}

/*>>>>>ALBARANVENTADET_RELATIONS*/

/*<<<<<ALBARANVENTADET_BEFORESAVEORDELETERELATED*/
void RecAlbaranVentaDet::beforeSaveOrDeleteRelated(dbRecord *master, bool saving)
{
/*>>>>>ALBARANVENTADET_BEFORESAVEORDELETERELATED*/
    actStocks( master, saving );
}

bool RecAlbaranVentaDet::actStocks( dbRecord *albaranventa, bool savingdetail ) const
{
    if( static_cast<RecAlbaranVenta *>(albaranventa)->getRecTipoDoc()->getValue("ACTUALIZASTOCKS").toBool() )
        getRecArticulo()->actStocks(savingdetail, false /*venta*/, getValue( "CANTIDAD" ).toDouble());
    return true;
}

} // namespace factu
} // namespace gong
