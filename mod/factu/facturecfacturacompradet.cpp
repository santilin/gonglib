/*<<<<<MODULE_INFO*/
// COPYLEFT Registro de detalles de facturas de compra
// MEMBER beforeSaveOrDeleteRelated
// RELATION Articulo
// RELATION empresa::TipoIVA
// TYPE dbRecord factu::FacturaCompraDet
/*>>>>>MODULE_INFO*/

/*<<<<<FACTURACOMPRADET_INCLUDES*/
#include "facturecfacturacompradet.h"
/*>>>>>FACTURACOMPRADET_INCLUDES*/
#include "facturecfacturacompra.h"

namespace gong {
namespace factu {

bool RecFacturaCompraDet::actAlbaranes(dbRecord *facturacompra, bool saving) const
{
    dbRecordID alb_id = getValue( "ALBARANCOMPRA_ID" ).toInt();
    if( alb_id != 0 ) {
        Xtring sql = "UPDATE ALBARANCOMPRA SET FACTURADO=" + getConnection()->toSQL( saving )
                     + " WHERE ID=" + getConnection()->toSQL( alb_id );
        getConnection()->exec( sql );
    }
    return true;
}

bool RecFacturaCompraDet::actStocks( dbRecord *facturacompra, bool saving ) const
{
    double cantidad;
    if( static_cast<RecFacturaCompra *>(facturacompra)->getRecTipoDoc()->getValue("ACTUALIZASTOCKS").toBool() )
        cantidad = getValue( "CANTIDAD" ).toDouble();
    else
        cantidad = 0.0;
    getRecArticulo()->actStocks( saving, true /*compra*/, cantidad);
    return true;
}


/*<<<<<FACTURACOMPRADET_RELATIONS*/
RecArticulo *RecFacturaCompraDet::getRecArticulo() const
{
	return static_cast<RecArticulo*>(findRelatedRecord("ARTICULO_ID"));
}

empresa::RecTipoIVA *RecFacturaCompraDet::getRecTipoIVA() const
{
	return static_cast<empresa::RecTipoIVA*>(findRelatedRecord("TIPOIVA_ID"));
}

/*>>>>>FACTURACOMPRADET_RELATIONS*/

/*<<<<<FACTURACOMPRADET_BEFORESAVEORDELETERELATED*/
void RecFacturaCompraDet::beforeSaveOrDeleteRelated(dbRecord *master, bool saving)
{
/*>>>>>FACTURACOMPRADET_BEFORESAVEORDELETERELATED*/
    actStocks( master, saving );
    actAlbaranes( master, saving );
}

} // namespace factu
} // namespace gong
