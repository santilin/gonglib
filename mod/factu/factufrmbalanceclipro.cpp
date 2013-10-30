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


FrmBalanceCliPro::FrmBalanceCliPro(QWidget * parent, const char * name,
                                   WidgetFlags fl)
    : FrmCustom(parent, name, fl)
{
    if ( !name )
        setName( "FrmBalanceCliPro" );
    Xtring titulo = _("Balances de clientes y proveedoras");
    setTitle( titulo );
    pEditTitulo = addInput( 0, "Título", titulo );
    XtringList comprasoventas;
    comprasoventas << "Clientes" << "Proveedoras";
    pComboCliOPro = addComboBox( 0, "Clientes o proveedoras", comprasoventas );
    pDateRange = addDateRangeBox( 0, "Fecha de las transacciones",
                                  Date( empresa::ModuleInstance->getEjercicio(), 1, 1 ),
                                  Date( empresa::ModuleInstance->getEjercicio(), 12, 31 ) );
    pSearchTipoDoc = addMultipleSearchField( 0, "TIPODOC", "CODIGO", "NOMBRE" );
    pSearchCliente = addMultipleSearchField( 0, "CLIENTE", "CODIGO", "RAZONSOCIAL" );
    pSearchProveedora = addMultipleSearchField( 0, "PROVEEDORA", "CODIGO", "RAZONSOCIAL" );
    XtringList agrupar_por;
    agrupar_por << "No agrupar" << "Cliente o proveedora";
    pComboAgruparPor = addComboBox( 0, "Agrupar  por", agrupar_por );
    pCheckIncCobros = addCheckBox( 0, "Incluir cobros", false );
    pCheckResumido = addCheckBox( 0, "Resumido", false );
    pCheckTodasLasEmpresas = addCheckBox(0, "Todas las empresas", false );
    pCheckGranTotal = addCheckBox( 0, "Incluir gran total", true );
    pCheckIncNotas = addCheckBox( 0, "Incluir notas", false );
    pCheckCSV = addCheckBox( 0, _("Salida a CSV, OpenOffice Calc, ..."), false );
}


Xtring FrmBalanceCliPro::createRTK(Xtring &from, Xtring &where, Xtring &titulo)
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
    if( pCheckCSV->isOn() )
        defines += "#define CSV\n";
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
        if( inc_cobros ) {
            pagos_albaranes_select = ", CONCAT(AV.FECHA,'A',AV.ID) AS FACTURA_ID, NULL AS DOCUMENTOPAGO, NULL AS FECHAPAGO, NULL AS COBRO_IMPORTE, NULL AS COBRO_RESTO, NULL AS CUENTA, NULL AS CUENTA_DESCRIPCION, NULL AS FORMAPAGO";
            pagos_facturas_select = ", CONCAT(FV.FECHA,'F',FV.ID) AS FACTURA_ID, PAGO.DOCUMENTOPAGO AS DOCUMENTOPAGO, PAGO.FECHAPAGO AS FECHAPAGO, PAGO.IMPORTE AS COBRO_IMPORTE, PAGO.RESTO AS COBRO_RESTO, CUENTA.CUENTA, CUENTA.DESCRIPCION AS CUENTA_DESCRIPCION, IF(PAGO.IMPORTE IS NULL,NULL,FORMAPAGO.NOMBRE) AS FORMAPAGO";
        }
        from +=
            "		SELECT AV.SUMAIMPORTES, AV.BASEIMPONIBLE, AV.IVA, AV.TOTAL AS TOTAL, AV.ENTREGA + AV.PAGOS AS PAGADO, AV.RESTO, AV.FECHA, 'Av' AS TIPO, AV.NUMERO, TIPODOC.NOMBRE AS TIPODOC, PROVEEDORA.RAZONSOCIAL AS RAZONSOCIAL, PROVEEDORA.ID AS RAZONSOCIAL_ID, TIPODOC.ID AS TIPODOC_ID, AV.NOTAS"
            + pagos_albaranes_select +
            "			FROM ALBARANCOMPRA AV"
            "				INNER JOIN TIPODOC ON TIPODOC.ID = AV.TIPODOC_ID"
            "				INNER JOIN PROVEEDORA ON AV.PROVEEDORA_ID = PROVEEDORA.ID"
            " 			WHERE AV.FACTURADO=0 AND TIPODOC.ESLIQUIDACION=0";
        if( !compras_where.isEmpty() )
            from += " AND " + compras_where;
        from +=
            "		UNION ALL"
            "			SELECT -AV.SUMAIMPORTES, -AV.BASEIMPONIBLE, -AV.IVA, -AV.TOTAL AS TOTAL, -AV.ENTREGA - AV.PAGOS AS PAGADO, -AV.RESTO, AV.FECHA, 'Av' AS TIPO, AV.NUMERO, TIPODOC.NOMBRE AS TIPODOC, PROVEEDORA.RAZONSOCIAL AS RAZONSOCIAL, PROVEEDORA.ID AS RAZONSOCIAL_ID, TIPODOC.ID AS TIPODOC_ID, AV.NOTAS"
            + pagos_albaranes_select +
            "			FROM ALBARANCOMPRA AV"
            "				INNER JOIN TIPODOC ON TIPODOC.ID = AV.TIPODOC_ID"
            "				INNER JOIN PROVEEDORA ON AV.PROVEEDORA_ID = PROVEEDORA.ID"
            " 			WHERE AV.FACTURADO=0 AND TIPODOC.ESLIQUIDACION=1";
        if( !compras_where.isEmpty() )
            from += " AND " + compras_where;
        from +=
            "		UNION ALL"
            "			SELECT FV.SUMAIMPORTES, FV.BASEIMPONIBLE, FV.IVA, FV.TOTAL, FV.ENTREGA + FV.ENTREGAALBARANES + FV.PAGOS AS PAGADO, FV.RESTO, FV.FECHA, 'Fv' AS TIPO, FV.NUMERO, TIPODOC.NOMBRE AS TIPODOC, PROVEEDORA.RAZONSOCIAL AS RAZONSOCIAL, PROVEEDORA.ID AS RAZONSOCIAL_ID, TIPODOC.ID AS TIPODOC_ID, FV.NOTAS"
            + pagos_facturas_select +
            "			FROM FACTURACOMPRA FV"
            "				INNER JOIN TIPODOC ON TIPODOC.ID = FV.TIPODOC_ID"
            "				INNER JOIN PROVEEDORA ON FV.PROVEEDORA_ID = PROVEEDORA.ID";
        if( inc_cobros ) {
            from += " LEFT JOIN PAGO ON PAGO.FACTURACOMPRA_ID = FV.ID AND PAGO.RESTO=0"
                    " LEFT JOIN CUENTA ON CUENTA.ID=PAGO.CUENTAPAGO_ID"
                    " LEFT JOIN FORMAPAGO ON FV.FORMAPAGO_ID=FORMAPAGO.ID";
        }
        if( !compras_where.isEmpty() )
            from += " WHERE " + compras_where.replace("AV.","FV.");
        from = "PROVEEDORA RS INNER JOIN (" + from + ") VT ON VT.RAZONSOCIAL_ID=RS.ID";
    } else if( BALANCE_CLIENTES ) {
        Xtring cobros_albaranes_select, cobros_facturas_select;
        if( inc_cobros ) {
            cobros_albaranes_select = ", CONCAT(AV.FECHA,'A',AV.ID) AS FACTURA_ID, NULL AS DOCUMENTOPAGO, NULL AS FECHAPAGO, NULL AS COBRO_IMPORTE, NULL AS COBRO_RESTO, NULL AS CUENTA, NULL AS CUENTA_DESCRIPCION, NULL AS FORMAPAGO";
            cobros_facturas_select = ", CONCAT(FV.FECHA,'F',FV.ID) AS FACTURA_ID, COBRO.DOCUMENTOPAGO AS DOCUMENTOPAGO, COBRO.FECHAPAGO AS FECHAPAGO, COBRO.IMPORTE AS COBRO_IMPORTE, COBRO.RESTO AS COBRO_RESTO, CUENTA.CUENTA, CUENTA.DESCRIPCION AS CUENTA_DESCRIPCION, IF(COBRO.IMPORTE IS NULL,NULL,FORMAPAGO.NOMBRE) AS FORMAPAGO";
        }
        from +=
            "		SELECT AV.SUMAIMPORTES, AV.BASEIMPONIBLE, AV.IVA, AV.TOTAL AS TOTAL, AV.ENTREGA + AV.COBROS AS PAGADO, AV.RESTO AS RESTO, AV.FECHA, 'Av' AS TIPO, AV.NUMERO, TIPODOC.NOMBRE AS TIPODOC, CLIENTE.RAZONSOCIAL AS RAZONSOCIAL, CLIENTE.ID AS RAZONSOCIAL_ID, TIPODOC.ID AS TIPODOC_ID, AV.NOTAS"
            + cobros_albaranes_select +
            "			FROM ALBARANVENTA AV"
            "				INNER JOIN TIPODOC ON TIPODOC.ID = AV.TIPODOC_ID"
            "				INNER JOIN CLIENTE ON AV.CLIENTE_ID = CLIENTE.ID"
            " 			WHERE AV.FACTURADO=0 AND TIPODOC.ESLIQUIDACION=0";
        if( !ventas_where.isEmpty() )
            from += " AND " + ventas_where;
        from +=
            "		UNION ALL"
            "			SELECT -AV.SUMAIMPORTES, -AV.BASEIMPONIBLE, -AV.IVA, -AV.TOTAL AS TOTAL, -AV.ENTREGA - AV.COBROS AS PAGADO, -AV.RESTO AS RESTO, AV.FECHA, 'Av' AS TIPO, AV.NUMERO, TIPODOC.NOMBRE AS TIPODOC, CLIENTE.RAZONSOCIAL AS RAZONSOCIAL, CLIENTE.ID AS RAZONSOCIAL_ID, TIPODOC.ID AS TIPODOC_ID, AV.NOTAS"
            + cobros_albaranes_select +
            "			FROM ALBARANVENTA AV"
            "				INNER JOIN TIPODOC ON TIPODOC.ID = AV.TIPODOC_ID"
            "				INNER JOIN CLIENTE ON AV.CLIENTE_ID = CLIENTE.ID"
            " 			WHERE AV.FACTURADO=0 AND TIPODOC.ESLIQUIDACION=1";
        if( !ventas_where.isEmpty() )
            from += " AND " + ventas_where;
        from +=
            "		UNION ALL"
            "			SELECT FV.SUMAIMPORTES, FV.BASEIMPONIBLE, FV.IVA, FV.TOTAL, FV.ENTREGA + FV.ENTREGAALBARANES + FV.COBROS AS PAGADO, FV.RESTO, FV.FECHA, 'Fv' AS TIPO, FV.NUMERO, TIPODOC.NOMBRE AS TIPODOC, CLIENTE.RAZONSOCIAL AS RAZONSOCIAL, CLIENTE.ID AS RAZONSOCIAL_ID, TIPODOC.ID AS TIPODOC_ID, FV.NOTAS"
            + cobros_facturas_select +
            "			FROM FACTURAVENTA FV"
            "				INNER JOIN TIPODOC ON TIPODOC.ID = FV.TIPODOC_ID"
            "				INNER JOIN CLIENTE ON FV.CLIENTE_ID = CLIENTE.ID";
        if( inc_cobros ) {
            from += " LEFT JOIN COBRO ON COBRO.FACTURAVENTA_ID = FV.ID AND COBRO.RESTO=0"
                    " LEFT JOIN CUENTA ON CUENTA.ID=COBRO.CUENTAPAGO_ID"
                    " LEFT JOIN FORMAPAGO ON FV.FORMAPAGO_ID=FORMAPAGO.ID";
        }
        if( !ventas_where.isEmpty() )
            from += " WHERE " + ventas_where.replace("AV.","FV.");
        from = "CLIENTE RS INNER JOIN (" + from + ") VT ON VT.RAZONSOCIAL_ID=RS.ID";
    }

    if( where.size() )
        where = where.mid(3);

    Xtring paths = DBAPP->getReportsPath( true );
    Xtring filename;
    if( inc_cobros )
        filename = FileUtils::findInPath( paths, "balanceclipro_cobros.inc", "informes/" );
    else
        filename = FileUtils::findInPath( paths, "balanceclipro.inc", "informes/" );
    Xtring rtkstring = FileUtils::readFile( filename );
    if( rtkstring.isEmpty() ) {
        FrmBase::msgError(this,
                          Xtring::printf("Imposible encontrar el fichero balanceclipro.inc en\n%s", paths.c_str() ));
        return Xtring();
    }
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
    //FrmBase::msgOkLarge(this, "Informe", "", rtkstring );
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
    Xtring rtkstring = createRTK(from, where, titulo);
    if( !rtkstring.isEmpty() ) {
        AppReport *report = new AppReport(*DBAPP);
        report->readString( rtkstring.c_str() );
        report->setParameterValue( "EMPRESA", empresa::ModuleInstance->getNombreEmpresa() );
        Dictionary<Xtring> properties;
        properties.insert( "TITLE", titulo );
        if( pCheckCSV->isOn() )
            report->print(RTK_CSV, properties, Xtring(), Xtring(),
                          DBAPP->getAppSetting( "RTK.LANDSCAPE" ).toBool() ? Landscape : DefaultOrientation, false);
        else
            report->print(RTK_Screen, properties, Xtring(), Xtring(),
                          DBAPP->getAppSetting( "RTK.LANDSCAPE" ).toBool() ? Landscape : DefaultOrientation, false);
    }
#else
    FrmBase::msgOk(this, Xtring("Error"), "Lo siento, en este ordenador no se puede hacer informes" );
#endif
}

#if 0
void FrmBalanceCliPro::pushGrabar_clicked()
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

void FrmBalanceCliPro::pushFilter_clicked()
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

} // namespace cats
} // namespace gong

