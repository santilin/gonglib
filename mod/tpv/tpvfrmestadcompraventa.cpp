#include <gonggettext.h>
#include "tpvmodule.h"
#include "tpvfrmestadcompraventa.h"
#include "tpvrecsesiontpv.h"

namespace gong {
namespace tpv {


FrmEstadCompraVenta::FrmEstadCompraVenta(QWidget * parent, WidgetFlags fl)
	: factu::FrmEstadCompraVenta( parent, fl )
{
	pEditTitulo->setText( _("Arqueo de caja") );
	setTitle( _("Arqueo de caja") );
	pSearchSesionTPV = addMultipleSearchField( 0, "SESIONTPV", "CODIGO", "INICIO" );
	RecSesionTPV *sesion = RecSesionTPV::getCurrentSesion(
		ModuleInstance->getConnection(), ModuleInstance->getNumeroTPV() );
	if( sesion->getRecordID() != 0 )
		pSearchSesionTPV->setValue( sesion->getValue( "CODIGO" ).toInt() );
	delete sesion;
	pComboComprasOVentas->setCurrentIndex( 2 );
}

Xtring FrmEstadCompraVenta::getExtraSQL( ExtraSQLWhat what )
{
	Xtring ret;
	switch( what ) {
		case VentasAlbaranesWhere:
		{
			List<dbRecordID> &sesiontpv_ids = pSearchSesionTPV->getRecordIDs();
			if( sesiontpv_ids.size() )
				ret = "(ST.ID IN(" + sesiontpv_ids.join(",") + "))";
		}
			break;
		case VentasAlbaranesFrom:
		{
			List<dbRecordID> &sesiontpv_ids = pSearchSesionTPV->getRecordIDs();
			if( sesiontpv_ids.size() )
				ret = " INNER JOIN SESIONTPV ST ON ST.ID=AV.SESIONTPV_ID";
		}
			break;
		case AnotherUnionAll:
		{
			List<dbRecordID> &sesiontpv_ids = pSearchSesionTPV->getRecordIDs();
			if( sesiontpv_ids.size() )
				ret = " UNION ALL ( SELECT 1 AS CANTIDAD, 0 AS IMPORTE, CAJAINICIAL AS IMPORTECONIVA, 0 AS PRECIOSINIVA, CAJAINICIAL AS PRECIOCONIVA, 0, 0, 0 AS TIPOIVA_CODIGO, 0, ST.INICIO AS FECHA, '00' AS TIPO, 0 AS NUMERO, 'CAJA INICIAL' AS TIPODOC, 0 AS RAZONSOCIAL_CODIGO, '' AS RAZONSOCIAL, 0 AS RAZONSOCIAL_ID, 0 AS TIPODOC_ID, '' AS NOTAS"
					" FROM SESIONTPV ST WHERE ST.ID IN(" + sesiontpv_ids.join(",") + "))";
			break;
		}
		case VentasFacturasWhere:
			ret = "1=0"; // No incluir facturas
			break;
		case VentasFacturasFrom:
		case ComprasAlbaranesWhere:
		case ComprasFacturasWhere:
		case ComprasAlbaranesFrom:
		case ComprasFacturasFrom:
			break;
	}
	return ret;
}

} // namespace tpv
} // namespace gong
