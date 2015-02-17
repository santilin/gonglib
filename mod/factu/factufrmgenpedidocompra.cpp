#include <dbappdbapplication.h>
#include <dbappfrmeditrecmaster.h>
#include <dbappmainwindow.h>
#include <empresamodule.h>
#include "factufrmgenpedidocompra.h"
#include "facturecpedidoventa.h"
#include "facturecpedidocompra.h"
#include "factumastertable.h"


namespace gong {
namespace factu {

XtringList FrmGenPedidoCompra::mOpcionesDesde;

FrmGenPedidoCompra::FrmGenPedidoCompra(QWidget* parent, const char* name, WidgetFlags f)
    : FrmCustom( parent, name, f)
{
    setTitle( _("Generación de pedidos de compra") );
    mOpcionesDesde << "Pedidos de clientes" << "Stocks de los artículos";
    comboDesde = addComboBoxXtring( false, 0, "Origen de los pedidos a proveedores:", mOpcionesDesde );
    Date ini = Date(empresa::ModuleInstance->getEjercicio(), 1, 1 );
    Date fin = Date(empresa::ModuleInstance->getEjercicio(), 12, 31 );
    pDateRange = addDateRangeBox( 0, "Fecha de los pedidos de las clientes:", ini, fin);
    comboEstadoPedidosProveedores = addComboIntField( 0, _("Poner el estado de los pedidos de las proveedoras a:"),
                                    "PEDIDOCOMPRA", "ESTADOPEDIDO", "NOMBRE" );
    pSearchTipoDoc = addMultipleSearchField( 0, "TIPODOC", "CODIGO", "NOMBRE" );
    pSearchTipoDoc->setText( _("Tipo de documento de los pedidos generados") );
    // Solo para pedidos a partir de pedidos de clientes
    comboEstadoPedidosClientes = addComboIntField( 0, _("Poner el estado de los pedidos de las clientes a:"),
                                 "PEDIDOVENTA", "ESTADOPEDIDO", "NOMBRE", _("No cambiar") );
    chkAgruparPorProveedora = addCheckBox( 0, _("Agrupar por proveedora"), false);
    chkRevisar = addCheckBox( 0, _("Revisar los pedidos generados uno por uno"), true );
    validate_input( comboDesde, 0);
}

void FrmGenPedidoCompra::validate_input( QWidget *sender, bool *isvalid )
{
    FrmCustom::validate_input( sender, isvalid );
    if( comboDesde->currentIndex() == 0 ) { // Pedidos de clientes
        comboEstadoPedidosClientes->setVisible( true );
        chkAgruparPorProveedora->setVisible( true );
    } else {
        comboEstadoPedidosClientes->setVisible( false );
        chkAgruparPorProveedora->setVisible( false );
    }
}

void FrmGenPedidoCompra::accept()
{
    if( comboDesde->currentIndex() == 0 ) { // Pedidos de clientes
        List<dbRecordID> &tipodoc_ids = pSearchTipoDoc->getRecordIDs();
        if( tipodoc_ids.size() == 0 ) {
            msgError(this, _("Por favor, elige al menos un tipo de documento") );
            return;
        }
        preparaDesdePedidosClientes();
    } else {
        preparaDesdeStockArticulos();
    }
}

void FrmGenPedidoCompra::preparaDesdePedidosClientes()
{
    DBAPP->waitCursor(true);
    try {
        DBAPP->showOSD("Elige el/los pedidos de venta", "para generar los pedidos de compras" );
        RecPedidoVenta *tmprec = static_cast<RecPedidoVenta *>( DBAPP->createRecord( "PEDIDOVENTA" ) );
        mPedidoVentaIds.clear();
        Xtring filter;
        if( pDateRange->getDateFrom().isValid() )
            filter = "PEDIDOVENTA.FECHA >= " + DBAPP->getConnection()->toSQL( pDateRange->getDateTimeFrom() );
        if( pDateRange->getDateTo().isValid() ) {
            if( !filter.isEmpty() )
                filter += " AND " ;
            filter += "PEDIDOVENTA.FECHA <= " + DBAPP->getConnection()->toSQL( pDateRange->getDateTimeTo() );
        }
        RecPedidoVentaDet *pvdet = static_cast<RecPedidoVentaDet *>( DBAPP->createRecord( "PEDIDOVENTADET" ) );
        dbViewDefinitionDict pvview;
        DBAPP->getDatabase()->getViewsForTable( "PEDIDOVENTA", pvview);
        dbRecordDataModel *pvrdm = new dbRecordDataModel( pvdet, pvview, filter);
        dbRecordID pedidoventa_id = DBAPP->chooseMulti( mPedidoVentaIds,
                                    static_cast<FrmEditRecMaster*>(0), tmprec, pvrdm );
        int npedidos = 0;
        if ( pedidoventa_id != 0 ) {
            DBAPP->showOSD( "Elige los artículos", "para generar los pedidos de compras");
            RecPedidoVentaDet *pvdet = static_cast<RecPedidoVentaDet *>( DBAPP->createRecord( "PEDIDOVENTADET" ) );
            dbViewDefinitionDict pvdtview;
            DBAPP->getDatabase()->getViewsByName( "PEDIDOVENTADET._POR_PROVEEDORA", pvdtview);
            dbRecordDataModel *pvdtrdm = new dbRecordDataModel( pvdet, pvdtview, "PEDIDOVENTA.ID IN(" + mPedidoVentaIds.join(",") + ")");
            mPedidoVentaDetIds.clear();
            DBAPP->chooseMulti( mPedidoVentaDetIds, static_cast<FrmEditRec*>(0), pvdet, pvdtrdm, dbApplication::readOnly );
            if( mPedidoVentaDetIds.size() ) {
                npedidos = genDesdePedidosClientes( chkAgruparPorProveedora->isOn() );
            }
            if( npedidos == 0 ) {
                msgOk( this, "No se ha generado ningún pedido" );
            } else {
                msgOk( this, Xtring::printf( "Se han generado %d pedidos a proveedoras", npedidos ) );
            }
        }
    } catch ( std::exception &e ) {
        FrmBase::msgError( this, e.what() );
    }
    DBAPP->resetCursor();
}

int FrmGenPedidoCompra::genDesdePedidosClientes( bool agrupar_por_proveedor )
{
    int npedidos = 0;
    DBAPP->waitCursor( true );
    dbRecordID tipodoc_id = pSearchTipoDoc->getRecord()->getRecordID();
    Xtring sql = "SELECT ARTICULO.PROVEEDORA_ID, PEDIDOVENTADET.CANTIDAD, ARTICULO.ID, ARTICULO.COSTESINIVA, ARTICULO.COSTE, ARTICULO.TIPOIVA_ID, CLIENTE.RAZONSOCIAL, PEDIDOVENTA.NUMERO, PEDIDOVENTA_ID, PEDIDOVENTADET.DESCRIPCION "
                 " FROM PEDIDOVENTADET INNER JOIN PEDIDOVENTA ON PEDIDOVENTADET.PEDIDOVENTA_ID=PEDIDOVENTA.ID INNER JOIN ARTICULO ON ARTICULO.ID=PEDIDOVENTADET.ARTICULO_ID LEFT JOIN CLIENTE ON PEDIDOVENTA.CLIENTE_ID = CLIENTE.ID"
                 " WHERE PEDIDOVENTADET.ID IN(" + mPedidoVentaDetIds.join(",") + ")";
    if( pDateRange->getDateFrom().isValid() )
        sql += " AND PEDIDOVENTA.FECHA >= " + DBAPP->getConnection()->toSQL( pDateRange->getDateTimeFrom() );
    if( pDateRange->getDateTo().isValid() )
        sql += " AND PEDIDOVENTA.FECHA <= " + DBAPP->getConnection()->toSQL( pDateRange->getDateTimeTo() );
    if( agrupar_por_proveedor )
        sql += " ORDER BY ARTICULO.PROVEEDORA_ID, ARTICULO.CODIGO";
    else
        sql += " ORDER BY CLIENTE.CODIGO, CLIENTE.ID, ARTICULO.PROVEEDORA_ID, ARTICULO.CODIGO";
    dbRecordID grupo_id = 0, old_grupo_id = 0;
    int nlinea = 1;
    Xtring referencia_pedido, cliente_razonsocial, cliente_numeropedido;
    dbResultSet *rsdetalles = DBAPP->getConnection()->select( sql );
    RecPedidoCompra *pedidocompra = static_cast<RecPedidoCompra *>( DBAPP->createRecord( "PEDIDOCOMPRA" ) );
    pedidocompra->clear( true ); // set custom devault values
    for ( uint i = 0; i <= rsdetalles->getRowCount(); ++i ) {
        /// TODO usar rsdetalles->position()
        if( i != rsdetalles->getRowCount() )
            grupo_id = rsdetalles->getValue(i, 0).toInt();
        if( i == rsdetalles->getRowCount() || (grupo_id != old_grupo_id && old_grupo_id != 0 )) {
            pedidocompra->setValue("PROVEEDORA_ID", old_grupo_id );
            pedidocompra->getRecProveedora()->read( old_grupo_id );
            if( !agrupar_por_proveedor )
                pedidocompra->setValue( "REFERENCIA", Xtring(cliente_razonsocial + "/" + cliente_numeropedido) );
            pedidocompra->setValue( "TIPODOC_ID", tipodoc_id );
            pedidocompra->getRecTipoDoc()->copyRecord( pSearchTipoDoc->getRecord() );
            pedidocompra->setValue( "ESTADOPEDIDO", comboEstadoPedidosProveedores->getCurrentItemValue() );
            FrmEditRecMaster *frmpedido = static_cast<FrmEditRecMaster *>(DBAPP->createEditForm( 0, pedidocompra,
                                          0, DataTable::inserting, dbApplication::simpleEdition ));
            LineEdit *le = static_cast<LineEdit *>(frmpedido->findControl( "TIPODOC.CODIGO" ) );
            if( le )
                le->setJustEdited(true);
            bool supervisar = chkRevisar->isOn();
            if( !supervisar )
                supervisar = !frmpedido->showAndSave();
            if( supervisar )
                frmpedido->showModalFor( 0, true, true );
            bool aceptado = !frmpedido->wasCancelled();
            delete frmpedido;
            if( !aceptado ) {
                if( msgYesNo(this, getTitle(), _("El pedido no se ha guardado. ¿Quieres continuar?") ) == false ) {
                    goto _fin;
                }
            } else {
                npedidos ++;
            }
            referencia_pedido.clear();
            if( i == rsdetalles->getRowCount() )
                break;
            nlinea = 1;
            pedidocompra->clear( true ); // set custom devault values
            pedidocompra->setNew( true );
        }
        old_grupo_id = grupo_id;
        double cantidad = rsdetalles->getValue(i,1).toDouble();
        dbRecordID articulo_id = rsdetalles->getValue(i,2).toUInt();
        Money costesiniva = rsdetalles->getValue(i,3).toMoney();
        Money coste = rsdetalles->getValue(i,4).toMoney();
        dbRecordID tipoiva_id = rsdetalles->getValue(i,5).toInt();
        cliente_razonsocial = rsdetalles->getValue(i,6).toString();
        cliente_numeropedido = rsdetalles->getValue(i,7).toString();
        dbRecordID pedidoventa_id = rsdetalles->getValue(i,8).toInt();
        Xtring descripcion = rsdetalles->getValue(i,9).toString();
        if( !mPedidosVentaACambiarEstado.contains( pedidoventa_id ) )
            mPedidosVentaACambiarEstado.push_back( pedidoventa_id );
        RecPedidoCompraDet *pedidocompra_det = static_cast<RecPedidoCompraDet *>
                                               ( DBAPP->createRecord( "PEDIDOCOMPRADET" ) );
        pedidocompra_det->setValue( "NLINEA", nlinea );
        pedidocompra_det->setValue( "CANTIDAD", cantidad );
        pedidocompra_det->setValue( "ARTICULO_ID", articulo_id );
        pedidocompra_det->getRecArticulo()->read( articulo_id );
        pedidocompra_det->setValue( "COSTESINIVA", costesiniva );
        pedidocompra_det->setValue( "COSTE", coste );
        pedidocompra_det->setValue( "TIPOIVA_ID", tipoiva_id );
        pedidocompra_det->setValue( "IMPORTE", cantidad * costesiniva.toDouble() );
        pedidocompra_det->setValue( "IMPORTECONIVA", cantidad * coste.toDouble() );
        pedidocompra_det->setValue( "DESCRIPCION", descripcion );
        if( agrupar_por_proveedor ) {
            Xtring referencia_pedido = Xtring::printf("%s %s %s %s",
                                       DBAPP->getDatabase()->findTableDefinition("PEDIDOVENTA")->getDescSingular().c_str(),
                                       cliente_numeropedido.c_str(),
                                       DBAPP->getTableDescSingular("CLIENTE", "de la").c_str(),
                                       cliente_razonsocial.c_str() );
            pedidocompra_det->setValue( "NOTAS", referencia_pedido );
        }
        pedidocompra_det->setNew( true );
        pedidocompra->getListPedidoCompraDet()->addRecord( pedidocompra_det );
        nlinea ++;
    }
    delete rsdetalles;
_fin:
    cambiaEstadoPedidosClientes();
    DBAPP->resetCursor();
    return npedidos;
}

void FrmGenPedidoCompra::preparaDesdeStockArticulos()
{
    msgError( this, _("Esta opción aún no está disponible") );
}


int FrmGenPedidoCompra::genDesdeStockArticulos()
{
    return 0;
}

void FrmGenPedidoCompra::cambiaEstadoPedidosClientes()
{
    int estadopedido = comboEstadoPedidosClientes->getCurrentItemValue();
    if( estadopedido != -1 ) {
        DBAPP->getConnection()->exec(
            "UPDATE PEDIDOVENTA SET ESTADO=" + DBAPP->getConnection()->toSQL( estadopedido )
            + " WHERE PEDIDOVENTA.ID IN( " + mPedidosVentaACambiarEstado.join(",") + ")" );
    }
}


} // namespace factu
} // namespace gong


