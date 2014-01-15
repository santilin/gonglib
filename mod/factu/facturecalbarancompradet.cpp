/*<<<<<MODULE_INFO*/
// COPYLEFT Registro de detalles de albaranes de compra
// MEMBER beforeSaveOrDeleteRelated
// RELATION Articulo
// RELATION empresa::TipoIVA
// TYPE dbRecord factu::AlbaranCompraDet
/*>>>>>MODULE_INFO*/

/*<<<<<ALBARANCOMPRADET_INCLUDES*/
#include "facturecalbarancompradet.h"
/*>>>>>ALBARANCOMPRADET_INCLUDES*/
#include "facturecalbarancompra.h"

namespace gong {
namespace factu {

/*<<<<<ALBARANCOMPRADET_RELATIONS*/
RecArticulo *RecAlbaranCompraDet::getRecArticulo() const
{
	return static_cast<RecArticulo*>(findRelatedRecord("ARTICULO_ID"));
}

empresa::RecTipoIVA *RecAlbaranCompraDet::getRecTipoIVA() const
{
	return static_cast<empresa::RecTipoIVA*>(findRelatedRecord("TIPOIVA_ID"));
}

/*>>>>>ALBARANCOMPRADET_RELATIONS*/

/*<<<<<ALBARANCOMPRADET_BEFORESAVEORDELETERELATED*/
void RecAlbaranCompraDet::beforeSaveOrDeleteRelated(dbRecord *master, bool saving)
{
/*>>>>>ALBARANCOMPRADET_BEFORESAVEORDELETERELATED*/
    actStocks( master, saving );
}

bool RecAlbaranCompraDet::actStocks( dbRecord *albarancompra, bool savingdetail ) const
{
    if( static_cast<RecAlbaranCompra *>(albarancompra)->getRecTipoDoc()->getValue("ACTUALIZASTOCKS").toBool() )
        getRecArticulo()->actStocks(savingdetail, true /*compra*/, getValue( "CANTIDAD" ).toDouble());
    return true;
}

} // namespace factu
} // namespace gong
