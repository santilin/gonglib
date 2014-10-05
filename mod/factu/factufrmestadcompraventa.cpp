#include <fstream>
#include <gongfileutils.h>
#include <dbappreport.h>
#include <empresamodule.h>
#include <factumodule.h>
#include "factufrmestadcompraventa.h"

namespace gong {
namespace factu {

FrmEstadCompraVenta::FrmEstadCompraVenta(QWidget * parent, WidgetFlags fl)
    : FrmReportConds( _("Estadísticas de compras y ventas"), parent, fl)
{
    setObjectName( "FrmEstadCompraVenta" );
    XtringList comprasoventas;
    comprasoventas << _("Compras y ventas") << _("Sólo compras") << _("Sólo ventas");
    pComboComprasOVentas = addComboBoxXtring(false, 0, _("Compras o ventas"), comprasoventas );
    pDateRange = addDateRangeBox( 0, _("Fecha de las compras o ventas"),
                                  Date( empresa::ModuleInstance->getEjercicio(), 1, 1 ),
                                  Date( empresa::ModuleInstance->getEjercicio(), 12, 31 ) );
    pSearchArticulo = addMultipleSearchField( 0, "ARTICULO", "CODIGO", "NOMBRE" );
    pSearchTipoDoc = addMultipleSearchField( 0, "TIPODOC", "CODIGO", "NOMBRE" );
    pSearchCliente = addMultipleSearchField( 0, "CLIENTE", "CODIGO", "RAZONSOCIAL" );
    pSearchProveedora = addMultipleSearchField( 0, "PROVEEDORA", "CODIGO", "RAZONSOCIAL" );
    pSearchFamilia = addMultipleSearchField( 0, "FAMILIA", "CODIGO", "NOMBRE" );
    pSearchAgente = addMultipleSearchField( 0, "AGENTE", "CODIGO", "RAZONSOCIAL" );
    pSearchFormaPago = addMultipleSearchField( 0, "FORMAPAGO", "CODIGO", "NOMBRE" );
    XtringList agrupar_por;
    agrupar_por << "No agrupar" << "Artículo" << "Razón social" << "Fecha (diario)" << "Documento";
    pComboAgruparPor = addComboBoxXtring( false, 0, _("Agrupar  por"), agrupar_por );
    pCheckPreciosConIVA = addCheckBox( 0, _("Precios con IVA"), true );
    XtringList albaranesofacturas;
    albaranesofacturas << _("Albaranes y facturas") << _("Sólo facturas") << _("Sólo albaranes");
    pComboAlbaranesFacturas = addComboBoxXtring( false, 0, _("Albaranes/facturas"), albaranesofacturas );
    pCheckTodasLasEmpresas = addCheckBox(0, _("Todas las empresas"), false );
    pCheckGranTotal = addCheckBox( 0, _("Incluir gran total"), true );
    pCheckIncNotas = addCheckBox( 0, _("Incluir notas"), false );
}

Xtring FrmEstadCompraVenta::createRTK(const Xtring &_template,
                                      Xtring &from, Xtring &where,
                                      const Xtring &_compras_where, const Xtring &_ventas_where,
                                      Xtring &titulo)
{
    Xtring compras_where = _compras_where;
    Xtring ventas_where = _ventas_where;
    titulo = pEditTitulo->toString();
    Xtring defines;
    if( pCheckGranTotal->isOn() )
        defines += "#define GRAN_TOTAL\n";
    if( pCheckResumido->isOn() )
        defines += "#define RESUMIDO\n";
    if( pCheckIncNotas->isOn() )
        defines += "#define NOTAS\n";
    bool precios_con_iva = pCheckPreciosConIVA->isChecked();
    if( precios_con_iva ) {
        defines += "#define PRECIOS_CON_IVA\n";
    }
    bool agrupar_articulos = ( pComboAgruparPor->currentIndex() == 1 );
    if( agrupar_articulos ) {
        defines += "#define AGRUPAR\n";
        defines += "#define AGRUPAR_POR_ARTICULO\n";
    }
    bool agrupar_razonsocial = ( pComboAgruparPor->currentIndex() == 2 );
    if( agrupar_razonsocial ) {
        defines += "#define AGRUPAR\n";
        defines += "#define AGRUPAR_POR_RAZONSOCIAL\n";
    }
    bool agrupar_fecha_diario = ( pComboAgruparPor->currentIndex() == 3 );
    if( agrupar_fecha_diario ) {
        defines += "#define AGRUPAR\n";
        defines += "#define AGRUPAR_POR_FECHA_DIARIO\n";
    }
    bool agrupar_documento = ( pComboAgruparPor->currentIndex() == 4 );
    if( agrupar_documento ) {
        defines += "#define AGRUPAR\n";
        defines += "#define AGRUPAR_POR_DOCUMENTO\n";
    }

    if( !pDateRange->getDateFrom().isNull() ) {
        if( !where.isEmpty() )
            where += " AND ";
        where += "(VT.FECHA>=" + ModuleInstance->getConnection()->toSQL( pDateRange->getDateFrom() ) + ")";
    }
    if( !pDateRange->getDateTo().isNull() ) {
        if( !where.isEmpty() )
            where += " AND ";
        where += "(VT.FECHA<=" + ModuleInstance->getConnection()->toSQL( pDateRange->getDateTimeTo() ) + ")";
    }
    List<dbRecordID> &proveedora_ids = pSearchProveedora->getRecordIDs();
    bool usa_prov_articulo = false;
    if( proveedora_ids.size() ) {
        if( ESTAD_SOLOVENTAS )
            usa_prov_articulo = true;
        if( !compras_where.isEmpty() )
            compras_where += " AND ";
        compras_where = "(PROVEEDORA.ID IN(" + proveedora_ids.join(",") + "))";
    }
    List<dbRecordID> &cliente_ids = pSearchCliente->getRecordIDs();
    if( cliente_ids.size() ) {
        if( !ventas_where.isEmpty() )
            ventas_where += " AND ";
        ventas_where = "(CLIENTE.ID IN(" + cliente_ids.join(",") + "))";
    }
    List<dbRecordID> &agente_ids = pSearchAgente->getRecordIDs();
    if( agente_ids.size() ) {
        if( !ventas_where.isEmpty() )
            ventas_where += " AND ";
        ventas_where = "(AGENTE.ID IN(" + agente_ids.join(",") + "))";
    }
    List<dbRecordID> &formapago_ids = pSearchFormaPago->getRecordIDs();
    if( formapago_ids.size() ) {
        if( !ventas_where.isEmpty() )
            ventas_where += " AND ";
        ventas_where = "(FORMAPAGO.ID IN(" + formapago_ids.join(",") + "))";
        if( !compras_where.isEmpty() )
            compras_where += " AND ";
        compras_where = "(FORMAPAGO.ID IN(" + formapago_ids.join(",") + "))";
    }
    List<dbRecordID> &articulo_ids = pSearchArticulo->getRecordIDs();
    if( articulo_ids.size() ) {
        if( !where.isEmpty() )
            where += " AND ";
        where += "(A.ID IN(" + articulo_ids.join(",") + "))";
    }
    List<dbRecordID> &familia_ids = pSearchFamilia->getRecordIDs();
    if( familia_ids.size() ) {
        if( !where.isEmpty() )
            where += " AND ";
        where += "(A.FAMILIA_ID IN(" + familia_ids.join(",") + "))";
    }
    List<dbRecordID> &tipodoc_ids = pSearchTipoDoc->getRecordIDs();
    if( tipodoc_ids.size() ) {
        if( !where.isEmpty() )
            where += " AND ";
        where += "(VT.TIPODOC_ID IN(" + tipodoc_ids.join(",") + "))";
    }
    if( !pCheckTodasLasEmpresas->isOn() ) {
        if( !compras_where.isEmpty() )
            compras_where += " AND ";
        compras_where += "AC.EMPRESA_ID="
                         + DBAPP->getConnection()->toSQL(empresa::ModuleInstance->getRecEmpresa()->getRecordID());
        if( !ventas_where.isEmpty() )
            ventas_where += " AND ";
        ventas_where += "AV.EMPRESA_ID="
                        + DBAPP->getConnection()->toSQL(empresa::ModuleInstance->getRecEmpresa()->getRecordID());
    }

    Xtring extra_compras_albaranes_where = getExtraSQL( ComprasAlbaranesWhere );
    Xtring extra_ventas_albaranes_where = getExtraSQL( VentasAlbaranesWhere );
    Xtring extra_compras_facturas_where = getExtraSQL( ComprasFacturasWhere );
    Xtring extra_ventas_facturas_where = getExtraSQL( VentasFacturasWhere );

    from.clear();
    Xtring signo_ventas;
    if( ESTAD_COMPRASYVENTAS )
        signo_ventas = "-";
    if( ESTAD_COMPRAS ) {
        if( !ESTAD_SOLOFACTURAS ) {
            from +=
                "		SELECT SUM(ACD.CANTIDAD) AS CANTIDAD, SUM(ACD.IMPORTE) AS IMPORTE, SUM(ACD.IMPORTECONIVA) AS IMPORTECONIVA, ACD.COSTESINIVA AS PRECIOSINIVA, ACD.COSTE AS PRECIOCONIVA, ACD.DTOP100, ACD.TIPOIVA_ID, TIPOIVA.CODIGO AS TIPOIVA_CODIGO, ACD.ARTICULO_ID, AC.FECHA, 'Ac' AS TIPO, AC.NUMERO, TIPODOC.NOMBRE AS TIPODOC, PROVEEDORA.CODIGO AS RAZONSOCIAL_CODIGO, PROVEEDORA.RAZONSOCIAL AS RAZONSOCIAL, PROVEEDORA.ID AS RAZONSOCIAL_ID, TIPODOC.ID AS TIPODOC_ID, ACD.NOTAS AS NOTAS"
                "			FROM ALBARANCOMPRADET ACD"
                "           INNER JOIN ALBARANCOMPRA AC ON ACD.ALBARANCOMPRA_ID=AC.ID";
            if( ESTAD_ALBARANESYFACTURAS )
                from += " AND AC.FACTURADO=0";
            from +=
                "				INNER JOIN TIPODOC ON TIPODOC.ID = AC.TIPODOC_ID"
                "				INNER JOIN TIPOIVA ON TIPOIVA.ID = ACD.TIPOIVA_ID"
                "				INNER JOIN PROVEEDORA ON AC.PROVEEDORA_ID = PROVEEDORA.ID";
            if( agente_ids.size() )
                from += "				INNER JOIN AGENTE ON AC.AGENTE_ID = AGENTE.ID";
            if( formapago_ids.size() )
                from += "				INNER JOIN FORMAPAGO ON AC.FORMAPAGO_ID = FORMAPAGO.ID";
            from += getExtraSQL( ComprasAlbaranesFrom );
            if( !compras_where.isEmpty() )
                from += " WHERE " + compras_where;
            if( !extra_compras_albaranes_where.isEmpty() ) {
                if( !compras_where.isEmpty() )
                    from += " AND ";
                from += extra_compras_albaranes_where;
            }
            from +=
                "			GROUP BY ACD.ID, ACD.COSTESINIVA, ACD.DTOP100, ACD.TIPOIVA_ID";
        }
        if( !ESTAD_SOLOALBARANES && !ESTAD_SOLOFACTURAS )
            from += "		UNION ALL";
        if( !ESTAD_SOLOALBARANES ) {
            from +=
                "	      SELECT SUM(FCD.CANTIDAD) AS CANTIDAD, SUM(FCD.IMPORTE) AS IMPORTE, SUM(FCD.IMPORTECONIVA) AS IMPORTECONIVA, FCD.COSTESINIVA AS PRECIOSINIVA, FCD.COSTE AS PRECIOCONIVA, FCD.DTOP100, FCD.TIPOIVA_ID, TIPOIVA.CODIGO AS TIPOIVA_CODIGO,FCD.ARTICULO_ID, FC.FECHA, 'Fc' AS TIPO, FC.NUMERO, TIPODOC.NOMBRE AS TIPODOC, PROVEEDORA.CODIGO AS RAZONSOCIAL_CODIGO, PROVEEDORA.RAZONSOCIAL AS RAZONSOCIAL, PROVEEDORA.ID AS RAZONSOCIAL_ID, TIPODOC.ID AS TIPODOC_ID, FCD.NOTAS AS NOTAS"
                "			FROM FACTURACOMPRADET FCD"
                "				INNER JOIN FACTURACOMPRA FC ON FCD.FACTURACOMPRA_ID=FC.ID"
                "				INNER JOIN TIPODOC ON TIPODOC.ID = FC.TIPODOC_ID"
                "				INNER JOIN TIPOIVA ON TIPOIVA.ID = FCD.TIPOIVA_ID"
                "				INNER JOIN PROVEEDORA ON FC.PROVEEDORA_ID = PROVEEDORA.ID";
            if( agente_ids.size() )
                from += "				INNER JOIN AGENTE ON FC.AGENTE_ID = AGENTE.ID";
            if( formapago_ids.size() )
                from += "				INNER JOIN FORMAPAGO ON FC.FORMAPAGO_ID = FORMAPAGO.ID";
            from += getExtraSQL( ComprasFacturasFrom );
            if( !compras_where.isEmpty() )
                from += " WHERE " + compras_where.replace("AC.","FC.");
            if( !extra_compras_facturas_where.isEmpty() ) {
                if( !compras_where.isEmpty() )
                    from += " AND ";
                from += extra_compras_facturas_where;
            }
            from +=
                "			GROUP BY FCD.ID, FCD.COSTESINIVA, FCD.DTOP100, FCD.TIPOIVA_ID";
        }
    }
    if( ESTAD_VENTAS ) {
        if( !from.isEmpty() )
            from += " UNION ALL ";
        if( !ESTAD_SOLOFACTURAS ) {
            from +=
                "		SELECT " + signo_ventas + "SUM(AVD.CANTIDAD) AS CANTIDAD, " + signo_ventas + "SUM(AVD.IMPORTE) AS IMPORTE, " + signo_ventas + "SUM(AVD.IMPORTECONIVA) AS IMPORTECONIVA, AVD.PVPSINIVA AS PRECIOSINIVA, AVD.PVP AS PRECIOCONIVA, AVD.DTOP100, AVD.TIPOIVA_ID, TIPOIVA.CODIGO AS TIPOIVA_CODIGO, AVD.ARTICULO_ID, AV.FECHA, 'Av' AS TIPO, AV.NUMERO, TIPODOC.NOMBRE AS TIPODOC, CLIENTE.CODIGO AS RAZONSOCIAL_CODIGO, CLIENTE.RAZONSOCIAL AS RAZONSOCIAL, CLIENTE.ID AS RAZONSOCIAL_ID, TIPODOC.ID AS TIPODOC_ID, AVD.NOTAS AS NOTAS"
                "			FROM ALBARANVENTADET AVD"
                "				INNER JOIN ALBARANVENTA AV ON AVD.ALBARANVENTA_ID=AV.ID";
            if( usa_prov_articulo ) {
                from += " INNER JOIN ARTICULO ON AVD.ARTICULO_ID=ARTICULO.ID AND ARTICULO.PROVEEDORA_ID IN("
                        + proveedora_ids.join(",") + ")";
            }
            if( ESTAD_ALBARANESYFACTURAS )
                from += " AND AV.FACTURADO=0";
            from +=
                "				INNER JOIN TIPODOC ON TIPODOC.ID = AV.TIPODOC_ID"
                "				INNER JOIN TIPOIVA ON TIPOIVA.ID = AVD.TIPOIVA_ID"
                "				INNER JOIN CLIENTE ON AV.CLIENTE_ID = CLIENTE.ID";
            if( agente_ids.size() )
                from += "				INNER JOIN AGENTE ON AV.AGENTE_ID = AGENTE.ID";
            if( formapago_ids.size() )
                from += "				INNER JOIN FORMAPAGO ON AV.FORMAPAGO_ID = FORMAPAGO.ID";
            from += getExtraSQL( VentasAlbaranesFrom );
            if( !ventas_where.isEmpty() )
                from += " WHERE " + ventas_where;
            if( !extra_ventas_albaranes_where.isEmpty() ) {
                if( !ventas_where.isEmpty() )
                    from += " AND ";
                from += extra_ventas_albaranes_where;
            }
            from +=
                "			GROUP BY AVD.ID, AVD.PVPSINIVA, AVD.DTOP100, AVD.TIPOIVA_ID";
        }
        if( !ESTAD_SOLOALBARANES && !ESTAD_SOLOFACTURAS )
            from += "		UNION ALL";
        if( !ESTAD_SOLOALBARANES ) {
            from +=
                "	      SELECT " + signo_ventas + "SUM(FVD.CANTIDAD) AS CANTIDAD, " + signo_ventas + "SUM(FVD.IMPORTE) AS IMPORTE, " + signo_ventas + "SUM(FVD.IMPORTECONIVA) AS IMPORTECONIVA, FVD.PVPSINIVA AS PRECIOSINIVA, FVD.PVP AS PRECIOCONIVA, FVD.DTOP100, FVD.TIPOIVA_ID, TIPOIVA.CODIGO AS TIPOIVA_CODIGO, FVD.ARTICULO_ID, FV.FECHA, 'Fv' AS TIPO, FV.NUMERO, TIPODOC.NOMBRE AS TIPODOC, CLIENTE.CODIGO AS RAZONSOCIAL_CODIGO, CLIENTE.RAZONSOCIAL AS RAZONSOCIAL, CLIENTE.ID AS RAZONSOCIAL_ID, TIPODOC.ID AS TIPODOC_ID, FVD.NOTAS AS NOTAS"
                "			FROM FACTURAVENTADET FVD"
                "				INNER JOIN FACTURAVENTA FV ON FVD.FACTURAVENTA_ID=FV.ID";
            if( usa_prov_articulo ) {
                from += " INNER JOIN ARTICULO ON FVD.ARTICULO_ID=ARTICULO.ID AND ARTICULO.PROVEEDORA_ID IN("
                        + proveedora_ids.join(",") + ")";
            }
            from +=
                "				INNER JOIN TIPODOC ON TIPODOC.ID = FV.TIPODOC_ID"
                "				INNER JOIN TIPOIVA ON TIPOIVA.ID = FVD.TIPOIVA_ID"
                "				INNER JOIN CLIENTE ON FV.CLIENTE_ID = CLIENTE.ID";
            if( agente_ids.size() )
                from += "				INNER JOIN AGENTE ON FV.AGENTE_ID = AGENTE.ID";
            if( formapago_ids.size() )
                from += "				INNER JOIN FORMAPAGO ON FV.FORMAPAGO_ID = FORMAPAGO.ID";
            from += getExtraSQL( VentasFacturasFrom );
            if( !ventas_where.isEmpty() )
                from += " WHERE " + ventas_where.replace("AV.","FV.").replace("AVD.","FVD.");
            if( !extra_ventas_facturas_where.isEmpty() ) {
                if( !ventas_where.isEmpty() )
                    from += " AND ";
                from += extra_ventas_facturas_where;
            }
            from +=
                "			GROUP BY FVD.ID, FVD.PVPSINIVA, FVD.DTOP100, FVD.TIPOIVA_ID";
        }
    }
    from += getExtraSQL( AnotherUnionAll );
    from = "ARTICULO A INNER JOIN (" + from + ") VT ON VT.ARTICULO_ID = A.ID";
// 	msgOkLarge( this, "", from );
    Xtring rtkstring = readRTK( _template );
    if( rtkstring.isEmpty() )
        return rtkstring;
    rtkstring += "\n\tINPUT default {\n"
                 "\t\tfrom = \"" + from + "\";\n"
                 "\t\twhere = \"" + where + "\";\n";
    if( agrupar_articulos )
        rtkstring += "\t\torderby = \"A.CODIGO,A.ID,VT_FECHA\";\n";
    else if( agrupar_razonsocial )
        rtkstring += "\t\torderby = \"VT.RAZONSOCIAL,VT.RAZONSOCIAL_ID,VT_FECHA\";\n";
    else if( agrupar_fecha_diario )
        rtkstring += "\t\torderby = \"VT.FECHA,A.CODIGO\";\n";
    else if( agrupar_documento )
        rtkstring += "\t\torderby = \"VT.FECHA,VT.NUMERO,VT.RAZONSOCIAL_ID,A.CODIGO\";\n";
    else
        rtkstring += "\t\torderby = \"VT_FECHA\";\n";
    rtkstring +="\t}\n";
    rtkstring += "}";
    rtkstring = defines + rtkstring;
//  	FrmBase::msgOkLarge( this, "", rtkstring );
    return rtkstring;
}


void FrmEstadCompraVenta::accept()
{
    List<dbRecordID> &cliente_ids = pSearchCliente->getRecordIDs();
    List<dbRecordID> &agente_ids = pSearchCliente->getRecordIDs();
    List<dbRecordID> &proveedora_ids = pSearchProveedora->getRecordIDs();
    if( proveedora_ids.size() && cliente_ids.size() && ! ESTAD_COMPRASYVENTAS ) {
        FrmBase::msgWarning(this,
                            _("Has seleccionado clientes y proveedoras, así que el informe se hará de compras y ventas") );
        pComboComprasOVentas->setCurrentIndex( 0 );
    }
// 	if( proveedora_ids.size() ) {
// 		if( ESTAD_SOLOVENTAS ) {
// 			FrmBase::msgOk(this, getTitle(),
// 						   _("Has seleccionado proveedoras en un listado solo de ventas. Se considerarán las proveedoras de los artículos.") );
// 		}
// 	}
    if( cliente_ids.size() || agente_ids.size() ) {
        if( ESTAD_SOLOCOMPRAS ) {
            FrmBase::msgWarning(this,
                                _("Has seleccionado clientes o agentes en un listado solo de compras. No se tendrá en cuenta a las clientes.") );
        }
    }
    Xtring from, where, titulo;
    Xtring rtkstring;
    if( pCheckResumido->isOn() )
        rtkstring = createRTK("estadcompraventa_resumido", from, where, Xtring::null, Xtring::null, titulo);
    else
        rtkstring = createRTK("estadcompraventa", from, where, Xtring::null, Xtring::null, titulo);
    if( !rtkstring.isEmpty() ) {
        AppReport *report = new AppReport(*DBAPP, ModuleInstance->getConnection());
        report->readString( rtkstring.c_str() );
        Dictionary<Variant> properties;
        properties.insert( "TITLE", titulo );
        report->print( static_cast<RTK_Output_Type>(pComboOutputs->currentItem()),
                       properties, Xtring(), Xtring(),
                       pCheckApaisado->isOn() ? Landscape : DefaultOrientation,
                       false /*chkAskParameters->isChecked()*/)			;
    }
}

#if 0
ARTICULO A INNER JOIN (
    SELECT SUM(AVD.CANTIDAD) AS CANTIDAD, SUM(AVD.IMPORTE) AS IMPORTE, SUM(AVD.IMPORTECONIVA) AS IMPORTECONIVA, AVD.COSTESINIVA AS PRECIOSINIVA, AVD.COSTE AS PRECIOCONIVA, AVD.DTOP100, AVD.TIPOIVA_ID, TIPOIVA.CODIGO AS TIPOIVA_CODIGO, AVD.ARTICULO_ID, AV.FECHA, 'Ac' AS TIPO, AV.NUMERO, TIPODOC.NOMBRE AS TIPODOC, PROVEEDORA.CODIGO AS RAZONSOCIAL_CODIGO, PROVEEDORA.RAZONSOCIAL AS RAZONSOCIAL, PROVEEDORA.ID AS RAZONSOCIAL_ID, TIPODOC.ID AS TIPODOC_ID, AVD.NOTAS AS NOTAS
    FROM ALBARANCOMPRADET AVD INNER JOIN ALBARANCOMPRA AV ON AVD.ALBARANCOMPRA_ID=AV.ID AND AV.FACTURADO=0
            INNER JOIN TIPODOC ON TIPODOC.ID = AV.TIPODOC_ID
                    INNER JOIN TIPOIVA ON TIPOIVA.ID = AVD.TIPOIVA_ID
                            INNER JOIN PROVEEDORA ON AV.PROVEEDORA_ID = PROVEEDORA.ID
                                    WHERE (PROVEEDORA.ID IN(1)) AND AV.EMPRESA_ID=1
                                            GROUP BY AVD.ID, AVD.COSTESINIVA, AVD.DTOP100, AVD.TIPOIVA_ID
    UNION ALL
    SELECT SUM(FVD.CANTIDAD) AS CANTIDAD, SUM(FVD.IMPORTE) AS IMPORTE, SUM(FVD.IMPORTECONIVA) AS IMPORTECONIVA, FVD.COSTESINIVA AS PRECIOSINIVA, FVD.COSTE AS PRECIOCONIVA, FVD.DTOP100, FVD.TIPOIVA_ID, TIPOIVA.CODIGO AS TIPOIVA_CODIGO,FVD.ARTICULO_ID, FV.FECHA, 'Fc' AS TIPO, FV.NUMERO, TIPODOC.NOMBRE AS TIPODOC, PROVEEDORA.CODIGO AS RAZONSOCIAL_CODIGO, PROVEEDORA.RAZONSOCIAL AS RAZONSOCIAL, PROVEEDORA.ID AS RAZONSOCIAL_ID, TIPODOC.ID AS TIPODOC_ID, FVD.NOTAS AS NOTAS
    FROM FACTURACOMPRADET FVD
    INNER JOIN FACTURACOMPRA FV ON FVD.FACTURACOMPRA_ID=FV.ID
            INNER JOIN TIPODOC ON TIPODOC.ID = FV.TIPODOC_ID
                    INNER JOIN TIPOIVA ON TIPOIVA.ID = FVD.TIPOIVA_ID
                            INNER JOIN PROVEEDORA ON FV.PROVEEDORA_ID = PROVEEDORA.ID
                                    WHERE (PROVEEDORA.ID IN(1)) AND FV.EMPRESA_ID=1
                                            GROUP BY FVD.ID, FVD.COSTESINIVA, FVD.DTOP100, FVD.TIPOIVA_ID
    UNION ALL
    SELECT -SUM(AVD.CANTIDAD) AS CANTIDAD, -SUM(AVD.IMPORTE) AS IMPORTE, -SUM(AVD.IMPORTECONIVA) AS IMPORTECONIVA, AVD.PVPSINIVA AS PRECIOSINIVA, AVD.PVP AS PRECIOCONIVA, AVD.DTOP100, AVD.TIPOIVA_ID, TIPOIVA.CODIGO AS TIPOIVA_CODIGO, AVD.ARTICULO_ID, AV.FECHA, 'Av' AS TIPO, AV.NUMERO, TIPODOC.NOMBRE AS TIPODOC, CLIENTE.CODIGO AS RAZONSOCIAL_CODIGO, CLIENTE.RAZONSOCIAL AS RAZONSOCIAL, CLIENTE.ID AS RAZONSOCIAL_ID, TIPODOC.ID AS TIPODOC_ID, AVD.NOTAS AS NOTAS
    FROM ALBARANVENTADET AVD
    INNER JOIN ALBARANVENTA AV ON AVD.ALBARANVENTA_ID=AV.ID AND AV.FACTURADO=0
            INNER JOIN TIPODOC ON TIPODOC.ID = AV.TIPODOC_ID
                    INNER JOIN TIPOIVA ON TIPOIVA.ID = AVD.TIPOIVA_ID
                            INNER JOIN CLIENTE ON AV.CLIENTE_ID = CLIENTE.ID
                                    WHERE (CLIENTE.ID IN(13)) AND AV.EMPRESA_ID=1
                                            GROUP BY AVD.ID, AVD.PVPSINIVA, AVD.DTOP100, AVD.TIPOIVA_ID
    UNION ALL
    SELECT -SUM(FVD.CANTIDAD) AS CANTIDAD, -SUM(FVD.IMPORTE) AS IMPORTE, -SUM(FVD.IMPORTECONIVA) AS IMPORTECONIVA, FVD.PVPSINIVA AS PRECIOSINIVA, FVD.PVP AS PRECIOCONIVA, FVD.DTOP100, FVD.TIPOIVA_ID, TIPOIVA.CODIGO AS TIPOIVA_CODIGO, FVD.ARTICULO_ID, FV.FECHA, 'Fv' AS TIPO, FV.NUMERO, TIPODOC.NOMBRE AS TIPODOC, CLIENTE.CODIGO AS RAZONSOCIAL_CODIGO, CLIENTE.RAZONSOCIAL AS RAZONSOCIAL, CLIENTE.ID AS RAZONSOCIAL_ID, TIPODOC.ID AS TIPODOC_ID, FVD.NOTAS AS NOTAS
    FROM FACTURAVENTADET FVD
    INNER JOIN FACTURAVENTA FV ON FVD.FACTURAVENTA_ID=FV.ID
            INNER JOIN TIPODOC ON TIPODOC.ID = FV.TIPODOC_ID
                    INNER JOIN TIPOIVA ON TIPOIVA.ID = FVD.TIPOIVA_ID
                            INNER JOIN CLIENTE ON FV.CLIENTE_ID = CLIENTE.ID
                                    WHERE (CLIENTE.ID IN(13)) AND FV.EMPRESA_ID=1
                                            GROUP BY FVD.ID, FVD.PVPSINIVA, FVD.DTOP100, FVD.TIPOIVA_ID
) VT ON VT.ARTICULO_ID = A.ID
#endif

#if 0
                         void FrmEstadCompraVenta::pushGrabar_clicked()
{
    Xtring from, titulo, where, valor_grupo, desc_grupo, valor_grupo3, desc_grupo3;
    Xtring rtkstring = createRTK(from, where, titulo, valor_grupo, desc_grupo, valor_grupo3, desc_grupo3);
    msgOkLarge(this, "Informe", "Este es el informe generado", rtkstring);
    return;
    QString fn = QFileDialog::getSaveFileName( QString::null, tr( "Informes de RTK (*.rtk);;Todos (*)" ), this,
                 titulo.c_str(), tr("Elige el fichero de destino") );
    if( !fn.isEmpty() ) {
        Xtring filename = fn.latin1();
        if( FileUtils::extension(filename) != "rtk" )
            filename += ".rtk";
        std::ifstream in(filename.c_str());
        if( in ) {
            if( !FrmBase::msgYesNo(this,
            Xtring::printf("El fichero %s ya existe. ¿Desea sobreescribirlo?", filename.c_str() ) ) ) {
                in.close();
                return;
            }
            in.close();
        }
        std::ofstream out(filename.c_str());
        if( !out ) {
            FrmBase::msgOk(this, "Error", strerror( errno ) );
            return;
        } else {
            DBAPP->waitCursor(true);
            rtkstring.replace( "###TITULO###", titulo );
            rtkstring.replace( "###seleccion###", where.replace("ATENCION.", "").replace(")AND(", " y ").replace("23:59:59","") );
            rtkstring.replace( "###VALOR_GRUPO###", valor_grupo);
            rtkstring.replace( "###DESCRIPCION_GRUPO###", desc_grupo);
            rtkstring.replace( "###VALOR_GRUPO3###", valor_grupo3);
            rtkstring.replace( "###DESCRIPCION_GRUPO3###", desc_grupo3);
            out << rtkstring;
            out.close();
            DBAPP->resetCursor();
            FrmBase::msgOk(this, fromGUI(this->caption()), Xtring("El informe se ha guardado correctamente"));
        }
    }
}

void FrmEstadCompraVenta::pushFilter_clicked()
{
    FrmSQLSelect *sel = new FrmSQLSelect(DBAPP->getConnection(),
                                         DBAPP->getDatabase(), this, "filtrar" );
    XtringList tables;
    tables << "ATENCION" << "PRINCESA" << "LUGARTRABAJO" << "TIPOATENCION";
    if( sel->exec(tables, editFilter->toString() ) == QDialog::Accepted )
        editFilter->setText(sel->getSqlExpression());
    delete sel;
}

#endif

} // namespace factu
} // namespace gong


