#include <gonggettext.h>
#include "ofertasfrmestadcompraventa.h"

namespace gong {
namespace ofertas {


FrmEstadCompraVenta::FrmEstadCompraVenta(QWidget * parent, WidgetFlags fl)
	: factu::FrmEstadCompraVenta( parent, fl )
{
	pCheckIncOfertas = addCheckBox( 0, _("Incluir ofertas"), false );
}

Xtring FrmEstadCompraVenta::getExtraSQL(ExtraSQLWhat what, const Xtring &signo_ventas, bool usa_prov_articulo, 
		const Xtring &compras_where, const Xtring &ventas_where,										
		const List<dbRecordID> &proveedora_ids, const List<dbRecordID> &cliente_ids,
		const List<dbRecordID> &agente_ids, const List<dbRecordID> &formapago_ids,
		const List<dbRecordID> &articulo_ids, const List<dbRecordID> &familia_ids,
		const List<dbRecordID> &tipodoc_ids)
{
	Xtring ret;
	if (!pCheckIncOfertas )
		return Xtring::null;
	if( what == AnotherUnionAll ) {
		if( ESTAD_VENTAS ) {
			ret += "		UNION ALL"
				"	      SELECT " + signo_ventas + "SUM(OVD.CANTIDAD) AS CANTIDAD, " + signo_ventas + "SUM(OVD.IMPORTE) AS IMPORTE, " + signo_ventas + "SUM(OVD.IMPORTECONIVA) AS IMPORTECONIVA, OVD.PVPSINIVA AS PRECIOSINIVA, OVD.PVP AS PRECIOCONIVA, OVD.DTOP100, OVD.TIPOIVA_ID, TIPOIVA.CODIGO AS TIPOIVA_CODIGO, OVD.ARTICULO_ID, OV.FECHA, 'Ov' AS TIPO, OV.NUMERO, TIPODOC.NOMBRE AS TIPODOC, CLIENTE.CODIGO AS RAZONSOCIAL_CODIGO, CLIENTE.RAZONSOCIAL AS RAZONSOCIAL, CLIENTE.ID AS RAZONSOCIAL_ID, TIPODOC.ID AS TIPODOC_ID, OVD.NOTAS AS NOTAS"
				"			FROM OFERTAVENTADET OVD"
				"				INNER JOIN OFERTAVENTA OV ON OVD.OFERTAVENTA_ID=OV.ID";
			if( usa_prov_articulo ) {
				ret += " INNER JOIN ARTICULO ON OVD.ARTICULO_ID=ARTICULO.ID AND ARTICULO.PROVEEDORA_ID IN("
						+ proveedora_ids.join(",") + ")";
			}
			ret +=
				"				INNER JOIN TIPODOC ON TIPODOC.ID = OV.TIPODOC_ID"
				"				INNER JOIN TIPOIVA ON TIPOIVA.ID = OVD.TIPOIVA_ID"
				"				INNER JOIN CLIENTE ON OV.CLIENTE_ID = CLIENTE.ID";
			if( agente_ids.size() )
				ret += "				INNER JOIN AGENTE ON OV.AGENTE_ID = AGENTE.ID";
			if( formapago_ids.size() )
				ret += "				INNER JOIN FORMAPAGO ON OV.FORMAPAGO_ID = FORMAPAGO.ID";
			if( !ventas_where.isEmpty() )
				ret += " WHERE " + Xtring(ventas_where).replace("AV.","OV.").replace("AVD.","OVD.");
			ret +=
				"			GROUP BY OVD.ID, OVD.PVPSINIVA, OVD.DTOP100, OVD.TIPOIVA_ID";
		}
		if( ESTAD_COMPRAS ) {
			ret += "		UNION ALL"
				"	      SELECT SUM(OCD.CANTIDAD) AS CANTIDAD, SUM(OCD.IMPORTE) AS IMPORTE, SUM(OCD.IMPORTECONIVA) AS IMPORTECONIVA, OCD.COSTESINIVA AS PRECIOSINIVA, OCD.COSTE AS PRECIOCONIVA, OCD.DTOP100, OCD.TIPOIVA_ID, TIPOIVA.CODIGO AS TIPOIVA_CODIGO, OCD.ARTICULO_ID, OC.FECHA, 'Oc' AS TIPO, OC.NUMERO, TIPODOC.NOMBRE AS TIPODOC, PROVEEDORA.CODIGO AS RAZONSOCIAL_CODIGO, PROVEEDORA.RAZONSOCIAL AS RAZONSOCIAL, PROVEEDORA.ID AS RAZONSOCIAL_ID, TIPODOC.ID AS TIPODOC_ID, OCD.NOTAS AS NOTAS"
				"			FROM OFERTACOMPRADET OCD"
				"				INNER JOIN OFERTACOMPRA OC ON OCD.OFERTACOMPRA_ID=OC.ID";
			if( usa_prov_articulo ) {
				ret += " INNER JOIN ARTICULO ON OCD.ARTICULO_ID=ARTICULO.ID AND ARTICULO.PROCEEDORA_ID IN("
						+ proveedora_ids.join(",") + ")";
			}
			ret +=
				"				INNER JOIN TIPODOC ON TIPODOC.ID = OC.TIPODOC_ID"
				"				INNER JOIN TIPOIVA ON TIPOIVA.ID = OCD.TIPOIVA_ID"
				"				INNER JOIN PROVEEDORA ON OC.PROVEEDORA_ID = PROVEEDORA.ID";
			if( agente_ids.size() )
				ret += "				INNER JOIN AGENTE ON OC.AGENTE_ID = AGENTE.ID";
			if( formapago_ids.size() )
				ret += "				INNER JOIN FORMAPAGO ON OC.FORMAPAGO_ID = FORMAPAGO.ID";
			if( !compras_where.isEmpty() )
				ret += " WHERE " + Xtring(compras_where).replace("AC.","OC.").replace("ACD.","OCD.");
			ret +=
				"			GROUP BY OCD.ID, OCD.COSTESINIVA, OCD.DTOP100, OCD.TIPOIVA_ID";
		}
		
	}
	return ret;
}


Xtring FrmEstadCompraVenta::createRTK(const Xtring &_template, Xtring &from, Xtring &where,
									  const Xtring &_compras_where, const Xtring &_ventas_where,
									  Xtring &titulo)
{
	Xtring rtk;
	if( pCheckResumido->isOn() )
		rtk = factu::FrmEstadCompraVenta::createRTK( "estadcompraventa_resumido.inc", from,
													 where, _compras_where, _ventas_where, titulo);
	else
		rtk = factu::FrmEstadCompraVenta::createRTK( "estadcompraventa.inc", from,
													 where, _compras_where, _ventas_where, titulo);
	if( pCheckIncOfertas->isOn() )
		rtk = "#define INCOFERTAS\n" + rtk;
	return rtk;
}

} // namespace ofertas
} // namespace gong
