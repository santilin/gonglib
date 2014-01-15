/*<<<<<MODULE_INFO*/
// COPYLEFT Registro de detalles de facturas de venta
// MEMBER beforeSaveOrDeleteRelated
// RELATION Articulo
// RELATION empresa::TipoIVA
// TYPE dbRecord factu::FacturaVentaDet
/*>>>>>MODULE_INFO*/

/*<<<<<FACTURAVENTADET_INCLUDES*/
#include "facturecfacturaventadet.h"
/*>>>>>FACTURAVENTADET_INCLUDES*/
#include "facturecfacturaventa.h"

namespace gong {
namespace factu {

/*<<<<<FACTURAVENTADET_BEFORESAVEORDELETERELATED*/
void RecFacturaVentaDet::beforeSaveOrDeleteRelated(dbRecord *master, bool saving)
{
/*>>>>>FACTURAVENTADET_BEFORESAVEORDELETERELATED*/
    actStocks( master, saving );
    actAlbaranes( master, saving );
}

bool RecFacturaVentaDet::actAlbaranes(dbRecord *facturaventa, bool saving) const
{
    dbRecordID alb_id = getValue( "ALBARANVENTA_ID" ).toInt();
    if( alb_id != 0 ) {
        Xtring sql = "UPDATE ALBARANVENTA SET FACTURADO=" + getConnection()->toSQL( saving )
                     + " WHERE ID=" + getConnection()->toSQL( alb_id );
        getConnection()->exec( sql );
    }
    return true;
}

bool RecFacturaVentaDet::actStocks( dbRecord *facturaventa, bool saving ) const
{
    double cantidad;
    if( static_cast<RecFacturaVenta *>(facturaventa)->getRecTipoDoc()->getValue("ACTUALIZASTOCKS").toBool() )
        cantidad = getValue( "CANTIDAD" ).toDouble();
    else
        cantidad = 0.0;
    if( getValue( "ALBARANVENTA_ID" ).toInt() == 0 ) {
        // Las líneas que proceden de albaranes no cambian el stock, ya lo hicieron en el albarán
        getRecArticulo()->actStocks(saving, false /*venta*/, cantidad);
    }
    return true;
}

/*<<<<<FACTURAVENTADET_RELATIONS*/
RecArticulo *RecFacturaVentaDet::getRecArticulo() const
{
	return static_cast<RecArticulo*>(findRelatedRecord("ARTICULO_ID"));
}

empresa::RecTipoIVA *RecFacturaVentaDet::getRecTipoIVA() const
{
	return static_cast<empresa::RecTipoIVA*>(findRelatedRecord("TIPOIVA_ID"));
}

/*>>>>>FACTURAVENTADET_RELATIONS*/

} // namespace factu
} // namespace gong
