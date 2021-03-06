#include <fstream>
#include <gongfileutils.h>
#include <dbappreport.h>
#include <empresamodule.h>
#include <factumodule.h>
#include "factufrmbalanceclipro.h"


/**
 * Sobre niveles y sumas en RTK
 *
 * En este informe, si se incluyen los cobros, el select contiene campos de las facturas y
 * campos de los cobros. Si una factura tiene más de un cobro, se obtienen tantas filas
 * como cobros para cada factura, de forma que se repite la factura.
 * En esta situación, al sumar, se suman varias veces los totales de las facturas.
 * Para evitar esto, utilizamos distinctsum, que es una suma que se hace efectiva solamente cuando
 * la sección que contiene el objeto cambia de valor, de este modo se suma una sola vez por sección.
 * Para que funcione, la sección que contiene este objeto tiene que tener value="valor de la sección".
 * Para el report_footer, también funciona así, pero no entiendo muy bien por qué
 *
*/

namespace gong {
namespace factu {

#define BALANCE_PROVEEDORAS (pComboCliOPro->currentIndex() == 1 )
#define BALANCE_CLIENTES (pComboCliOPro->currentIndex() == 0 )


FrmBalanceCliPro::FrmBalanceCliPro(QWidget * parent, WidgetFlags fl)
    : FrmReportConds(_("Balances de clientes y proveedoras"), parent, fl)
{
    setObjectName( "FrmBalanceCliPro" );
    XtringList comprasoventas;
    comprasoventas << "Clientes" << "Proveedoras";
    pComboCliOPro = addComboBoxXtring(true, 0, "Clientes o proveedoras", comprasoventas );
    pDateRange = addDateRangeBox( 0, "Fecha de las transacciones",
                                  Date( empresa::ModuleInstance->getEjercicio()-1, 1, 1 ),
                                  Date( empresa::ModuleInstance->getEjercicio(), 12, 31 ) );
    pSearchTipoDoc = addMultipleSearchField( 0, "TIPODOC", "CODIGO", "NOMBRE" );
    pSearchCliente = addMultipleSearchField( 0, "CLIENTE", "CODIGO", "RAZONSOCIAL" );
    pSearchProveedora = addMultipleSearchField( 0, "PROVEEDORA", "CODIGO", "RAZONSOCIAL" );
    XtringList agrupar_por;
    agrupar_por << "No agrupar" << "Cliente o proveedora";
    pComboAgruparPor = addComboBoxXtring( true, 0, "Agrupar  por", agrupar_por );
    pCheckIncCobros = addCheckBox( 0, "Incluir cobros", false );
    pCheckTodasLasEmpresas = addCheckBox(0, "Todas las empresas", false );
    pCheckGranTotal = addCheckBox( 0, "Incluir gran total", true );
    pCheckIncNotas = addCheckBox( 0, "Incluir notas", false );
}


Xtring FrmBalanceCliPro::createRTK(const Xtring &_template,
                                   Xtring &from, Xtring &where, Xtring &titulo)
{
    Xtring compras_where, ventas_where;
    titulo = pEditTitulo->toString();
    Xtring defines;
    if( pCheckGranTotal->isOn() )
        defines += "#define GRAN_TOTAL\n";
    if( pCheckIncNotas->isOn() )
        defines += "#define NOTAS\n";
    if( pCheckResumido->isOn() )
        defines += "#define RESUMIDO\n";
//     if( pCheckCSV->isOn() )
//         defines += "#define CSV\n";
    bool inc_cobros = false;
    if( pCheckIncCobros->isOn() ) {
        inc_cobros = true;
        defines += "#define COBROS\n";
    }
    bool agrupar_razonsocial = ( pComboAgruparPor->currentIndex() == 1 );
    if( agrupar_razonsocial ) {
// 		titulo += " por razón social";
        defines += "#define AGRUPAR\n";
        defines += "#define AGRUPAR_POR_CLIPRO\n";
    }
    if( !pDateRange->getDateFrom().isNull() )
        where += "AND(VT.FECHA>=" + ModuleInstance->getConnection()->toSQL( pDateRange->getDateFrom() ) + ")";
    if( !pDateRange->getDateTo().isNull() ) {
        where += "AND(VT.FECHA<=" + ModuleInstance->getConnection()->toSQL( pDateRange->getDateTimeTo() ) + ")";
    }
    List<dbRecordID> &proveedora_ids = pSearchProveedora->getRecordIDs();
    if( proveedora_ids.size() ) {
        compras_where = "PROVEEDORA.ID IN(" + proveedora_ids.join(",") + ")";
    }
    List<dbRecordID> &cliente_ids = pSearchCliente->getRecordIDs();
    if( cliente_ids.size() ) {
        ventas_where = "CLIENTE.ID IN(" + cliente_ids.join(",") + ")";
    }
    List<dbRecordID> &tipodoc_ids = pSearchTipoDoc->getRecordIDs();
    if( tipodoc_ids.size() ) {
        where += "AND(VT.TIPODOC_ID IN(" + tipodoc_ids.join(",") + "))";
    }
    if( !pCheckTodasLasEmpresas->isOn() ) {
        if( !compras_where.isEmpty() )
            compras_where += " AND ";
        compras_where += "AV.EMPRESA_ID="
                         + DBAPP->getConnection()->toSQL(empresa::ModuleInstance->getRecEmpresa()->getRecordID());
        if( !ventas_where.isEmpty() )
            ventas_where += " AND ";
        ventas_where += "AV.EMPRESA_ID="
                        + DBAPP->getConnection()->toSQL(empresa::ModuleInstance->getRecEmpresa()->getRecordID());
    }

    from.clear();
    Xtring signo_compras;
    if( BALANCE_PROVEEDORAS ) {
        Xtring pagos_albaranes_select, pagos_facturas_select;
        from +=
            "		SELECT AV.SUMAIMPORTES, AV.BASEIMPONIBLE, AV.IVA, AV.TOTAL AS TOTAL, AV.ENTREGA + AV.PAGOS AS PAGADO, AV.RESTO, AV.FECHA, 'Av' AS TIPO, AV.NUMERO, TIPODOC.NOMBRE AS TIPODOC, PROVEEDORA.RAZONSOCIAL AS RAZONSOCIAL, PROVEEDORA.ID AS RAZONSOCIAL_ID, TIPODOC.ID AS TIPODOC_ID, AV.NOTAS"
            "			FROM ALBARANCOMPRA AV"
            "				INNER JOIN TIPODOC ON TIPODOC.ID = AV.TIPODOC_ID"
            "				INNER JOIN PROVEEDORA ON AV.PROVEEDORA_ID = PROVEEDORA.ID"
            " 			WHERE AV.FACTURADO=0 AND TIPODOC.DEPOSITO=0";
        if( !compras_where.isEmpty() )
            from += " AND " + compras_where;
        from +=
            "		UNION ALL"
            "			SELECT -AV.SUMAIMPORTES, -AV.BASEIMPONIBLE, -AV.IVA, -AV.TOTAL AS TOTAL, -AV.ENTREGA - AV.PAGOS AS PAGADO, -AV.RESTO, AV.FECHA, 'Av' AS TIPO, AV.NUMERO, TIPODOC.NOMBRE AS TIPODOC, PROVEEDORA.RAZONSOCIAL AS RAZONSOCIAL, PROVEEDORA.ID AS RAZONSOCIAL_ID, TIPODOC.ID AS TIPODOC_ID, AV.NOTAS"
            "			FROM ALBARANCOMPRA AV"
            "				INNER JOIN TIPODOC ON TIPODOC.ID = AV.TIPODOC_ID"
            "				INNER JOIN PROVEEDORA ON AV.PROVEEDORA_ID = PROVEEDORA.ID"
            " 			WHERE AV.FACTURADO=0 AND TIPODOC.ESDEPOSITO=0";
        if( !compras_where.isEmpty() )
            from += " AND " + compras_where;
        from +=
            "		UNION ALL"
            "			SELECT FV.SUMAIMPORTES, FV.BASEIMPONIBLE, FV.IVA, FV.TOTAL, FV.ENTREGA + FV.ENTREGAALBARANES + FV.PAGOS AS PAGADO, FV.RESTO, FV.FECHA, 'Fv' AS TIPO, FV.NUMERO, TIPODOC.NOMBRE AS TIPODOC, PROVEEDORA.RAZONSOCIAL AS RAZONSOCIAL, PROVEEDORA.ID AS RAZONSOCIAL_ID, TIPODOC.ID AS TIPODOC_ID, FV.NOTAS"
            + pagos_facturas_select +
            "			FROM FACTURACOMPRA FV"
            "				INNER JOIN TIPODOC ON TIPODOC.ID = FV.TIPODOC_ID"
            "				INNER JOIN PROVEEDORA ON FV.PROVEEDORA_ID = PROVEEDORA.ID";
        if( !compras_where.isEmpty() )
            from += " WHERE " + compras_where.replace("AV.","FV.");
        from = "PROVEEDORA RS INNER JOIN (" + from + ") VT ON VT.RAZONSOCIAL_ID=RS.ID";
    } else if( BALANCE_CLIENTES ) {
        Xtring cobros_albaranes_select, cobros_facturas_select;
        from +=
            "		SELECT -AV.TOTAL AS TOTAL, AV.FECHA, 'Av' AS TIPO, AV.NUMERO, TIPODOC.NOMBRE AS TIPODOC, CLIENTE.RAZONSOCIAL AS RAZONSOCIAL, CLIENTE.ID AS RAZONSOCIAL_ID, TIPODOC.ID AS TIPODOC_ID, AV.NOTAS"
            "			FROM ALBARANVENTA AV"
            "				INNER JOIN TIPODOC ON TIPODOC.ID = AV.TIPODOC_ID"
            "				INNER JOIN CLIENTE ON AV.CLIENTE_ID = CLIENTE.ID"
            " 			WHERE AV.FACTURADO=0 AND TIPODOC.ESDEPOSITO=0";
        if( !ventas_where.isEmpty() )
            from += " AND " + ventas_where;
        from +=
            "	UNION ALL"
            "		SELECT -FV.TOTAL AS TOTAL, FV.FECHA, 'Fv' AS TIPO, FV.NUMERO, TIPODOC.NOMBRE AS TIPODOC, CLIENTE.RAZONSOCIAL AS RAZONSOCIAL, CLIENTE.ID AS RAZONSOCIAL_ID, TIPODOC.ID AS TIPODOC_ID, FV.NOTAS"
            "		FROM FACTURAVENTA FV"
            "			INNER JOIN TIPODOC ON TIPODOC.ID = FV.TIPODOC_ID"
            "			INNER JOIN CLIENTE ON FV.CLIENTE_ID = CLIENTE.ID";
        if( !ventas_where.isEmpty() )
            from += " WHERE " + Xtring(ventas_where).replace("AV.","FV.");
        from +=
            "		UNION ALL"
            "		SELECT AV.ENTREGA, AV.FECHA, 'Av' AS TIPO, AV.NUMERO, 'Entrega' AS TIPODOC, CLIENTE.RAZONSOCIAL AS RAZONSOCIAL, CLIENTE.ID AS RAZONSOCIAL_ID, TIPODOC.ID AS TIPODOC_ID, AV.NOTAS"
            "			FROM ALBARANVENTA AV"
            "				INNER JOIN TIPODOC ON TIPODOC.ID = AV.TIPODOC_ID"
            "				INNER JOIN CLIENTE ON AV.CLIENTE_ID = CLIENTE.ID"
            " 			WHERE AV.FACTURADO=0 AND TIPODOC.ESDEPOSITO=0 AND AV.ENTREGA!=0";
        if( !ventas_where.isEmpty() )
            from += " AND " + ventas_where;
        from +=
            "		UNION ALL"
            "		SELECT FV.ENTREGA, FV.FECHA, 'Fv' AS TIPO, FV.NUMERO, 'Entrega' AS TIPODOC, CLIENTE.RAZONSOCIAL AS RAZONSOCIAL, CLIENTE.ID AS RAZONSOCIAL_ID, TIPODOC.ID AS TIPODOC_ID, FV.NOTAS"
            "			FROM FACTURAVENTA FV"
            "				INNER JOIN TIPODOC ON TIPODOC.ID = FV.TIPODOC_ID"
            "				INNER JOIN CLIENTE ON FV.CLIENTE_ID = CLIENTE.ID"
            "			WHERE FV.ENTREGA!=0";
        if( !ventas_where.isEmpty() )
            from += " WHERE " + Xtring(ventas_where).replace("AV.","FV.");

        from = "CLIENTE RS INNER JOIN (" + from + ") VT ON VT.RAZONSOCIAL_ID=RS.ID";
    }

    if( where.size() )
        where = where.mid(3);

    Xtring paths = DBAPP->getReportsPath( true );
    Xtring filename;
    Xtring rtkstring = readRTK( _template );
    if( rtkstring.isEmpty() )
        return Xtring();
    Xtring orderby;
    if( inc_cobros ) {
        orderby = "VT.FACTURA_ID,VT.FECHAPAGO";
    } else {
        orderby = "VT.FECHA";
    }
    if( agrupar_razonsocial )
        orderby = "RS.CODIGO,RS.ID," + orderby;
    rtkstring += "\n\tINPUT default {\n"
                 "\t\tfrom = \"" + from + "\";\n"
                 "\t\torderby = \"" + orderby + "\";\n"
                 "\t\twhere = \"" + where + "\";\n"
                 "\t}\n"
                 "}";
    rtkstring = defines + rtkstring;
//    FrmBase::msgOkLarge(this, "", from );
    return rtkstring;
}


void FrmBalanceCliPro::accept()
{
#if 0
    List<dbRecordID> &cliente_ids = findRecordIDs("CLIENTE", "CODIGO");
    List<dbRecordID> &proveedora_ids = findRecordIDs("PROVEEDORA", "CODIGO");
    if( proveedora_ids.size() && cliente_ids.size() && ! ESTAD_PROVEEDORAS ) {
        FrmBase::msgOk(this, getTitle(),
                       _("Has seleccionado clientes y proveedoras, así que el informe se hará de compras y ventas") );
        pComboCliOPro->setCurrentIndex( 0 );
    }
    if( proveedora_ids.size() ) {
        if( ESTAD_SOLOVENTAS ) {
            FrmBase::msgOk(this, getTitle(),
                           _("Has seleccionado proveedoras en un listado solo de ventas. No se tendrá en cuenta a las proveedoras.") );
        }
    }
    if( cliente_ids.size() ) {
        if( ESTAD_SOLOCOMPRAS ) {
            FrmBase::msgOk(this, getTitle(),
                           _("Has seleccionado clientes en un listado solo de compras. No se tendrá en cuenta a las clientes.") );
        }
    }
#endif
#ifdef HAVE_RTKMODULE
    Xtring from, where, titulo;
    Xtring _template = "balanceclipro";
    if( pCheckIncCobros->isOn() )
        _template += "_cobros";
    Xtring rtkstring = createRTK( _template, from, where, titulo);
    if( !rtkstring.isEmpty() ) {
        AppReport *report = new AppReport(*DBAPP, ModuleInstance->getConnection());
        report->readString( rtkstring.c_str() );
        report->setParameterValue( "EMPRESA", empresa::ModuleInstance->getNombreEmpresa() );
        Dictionary<Variant> properties;
        properties.insert( "TITLE", titulo );
        report->print(RTK_Screen, properties, Xtring(), Xtring(),
                      DBAPP->getAppSetting( "RTK.LANDSCAPE" ).toBool() ? Landscape : DefaultOrientation, false);
    }
#else
    FrmBase::msgOk(this, Xtring("Error"), "Lo siento, en este ordenador no se puede hacer informes" );
#endif
}

} // namespace factu
} // namespace gong

