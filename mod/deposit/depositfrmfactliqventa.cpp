#include <dbappdbapplication.h>
#include <dbappfrmeditrecmaster.h>
#include <dbappmainwindow.h>
#include <empresamodule.h>
#include <factureccliente.h>
#include <facturecfacturaventa.h>
#include <facturectipodoc.h>
#include "depositfrmfactliqventa.h"

namespace gong {
namespace deposit {

FrmFactLiqVenta::FrmFactLiqVenta(QWidget* parent, WidgetFlags f)
	: FrmCustom( parent, "FrmFactLiqVenta", f)
{
	setTitle( _("Generación de facturas de clientes a partir de sus liquidaciones") );
	Date ini = Date(empresa::ModuleInstance->getEjercicio(), 1, 1 );
	Date fin = Date(empresa::ModuleInstance->getEjercicio(), 12, 31 );
	pDateRange = addDateRangeBox( 0, _("Fecha de las liquidaciones:"), ini, fin);
	pDateRange->getEditDateFrom()->setFocus();
	pSearchCliente = addMultipleSearchField( 0, "CLIENTE", "CODIGO", "RAZONSOCIAL" );
	pSearchTipoDocCond = addMultipleSearchField( 0, "TIPODOC", "CODIGO", "NOMBRE" );
}

void FrmFactLiqVenta::accept()
{
	List<dbRecordID> &cliente_ids = pSearchCliente->getRecordIDs();
	Xtring where;
	if( !cliente_ids.size() ) {
		msgError(this, _("Selecciona al menos una cliente") );
		return;
	}
	List<dbRecordID> &tipodoc_ids = pSearchTipoDocCond->getRecordIDs();
	if( tipodoc_ids.size() ) {
		if( !where.isEmpty() )
			where+="AND";
		where += "(AV.TIPODOC_ID IN(" + tipodoc_ids.join(",") + "))";
	}
	DBAPP->waitCursor( true );
	if( pDateRange->getDateFrom().isValid() ) {
		if( !where.isEmpty() )
			where+="AND";
		where += "AV.FECHA >= " + DBAPP->getConnection()->toSQL( pDateRange->getDateTimeFrom() );
	}
	if( pDateRange->getDateTo().isValid() ) {
		if( !where.isEmpty() )
			where+="AND";
		where += "AV.FECHA <= " + DBAPP->getConnection()->toSQL( pDateRange->getDateTimeTo() );
	}
    try {
		for( List<dbRecordID>::const_iterator it = cliente_ids.begin();
			it != cliente_ids.end(); ++it ) {
			dbRecordID cli_id = *it;
			factu::RecFacturaVenta *facturaventa = static_cast<factu::RecFacturaVenta *>
				( DBAPP->createRecord( "FACTURAVENTA" ) );
			if ( facturaventa ) {
				facturaventa->clear( true );
				facturaventa->setValue( "CLIENTE_ID", cli_id );
				facturaventa->getRecCliente()->read( cli_id );
				facturaventa->setValue( "FECHA", Date::currentDate() );
				genFacturaVenta( facturaventa, cli_id, where );
				DBAPP->createClient( DBAPP->createEditForm( 0, facturaventa, 0, DataTable::inserting,
						dbApplication::deleteRecord ) );
			}
		}
    } catch ( std::exception &e ) {
        FrmBase::msgError( this, e.what() );
    }
	DBAPP->resetCursor();
}

void FrmFactLiqVenta::genFacturaVenta( factu::RecFacturaVenta *facturaventa,
									   dbRecordID cli_id, const Xtring &where )
{
	DBAPP->waitCursor( true );
	Xtring sql = "SELECT SUM(CANTIDAD), ARTICULO_ID, ALBARANVENTADET.PVPSINIVA, ALBARANVENTADET.PVP, ROUND(ALBARANVENTADET.DTOP100,2), SUM(ROUND(IMPORTE,2)), SUM(ROUND(IMPORTECONIVA,2)), ALBARANVENTADET.TIPOIVA_ID"
		" FROM ALBARANVENTA AV INNER JOIN ALBARANVENTADET AVD ON AVD.ALBARANVENTA_ID=AV.ID"
		" INNER JOIN ARTICULO ON ARTICULO.ID = AVD.ARTICULO_ID"
		" WHERE AV.CLIENTE_ID=" + DBAPP->getConnection()->toSQL( cli_id ) + where;
	// Hay que agrupar por el pvpsiniva para que salgan distintas líneas si los precios difieren
	sql += " GROUP BY ARTICULO.ID, ROUND(AVD.PVPSINIVA,2), ROUND(AVD.DTOP100,2), AVD.TIPOIVA_ID"
		" ORDER BY ARTICULO.NOMBRE";
	dbResultSet *rsliquidaciones = DBAPP->getConnection()->select( sql );
	while( rsliquidaciones->next() ) {
		double cantidad = rsliquidaciones->toDouble( 0 );
		if( cantidad == 0.0 )
			continue;
		factu::RecFacturaVentaDet *liquid_det = static_cast<factu::RecFacturaVentaDet *>
				( DBAPP->createRecord( "FACTURAVENTADET" ) );
		liquid_det->setValue( "NLINEA", (long long int)(rsliquidaciones->getRowNumber() + 1L) );
		liquid_det->setValue( "CANTIDAD", cantidad );
		liquid_det->setValue( "ARTICULO_ID", rsliquidaciones->toInt( 1 ) );
		liquid_det->setValue( "PVPSINIVA", rsliquidaciones->toMoney( 2 ) );
		liquid_det->setValue( "PVP", rsliquidaciones->toMoney( 3 ) );
		liquid_det->setValue( "DTOP100", rsliquidaciones->toDouble( 4 ) );
		liquid_det->setValue( "IMPORTE", rsliquidaciones->toMoney( 5 ) );
		liquid_det->setValue( "IMPORTECONIVA", rsliquidaciones->toMoney( 6 ) );
		liquid_det->setValue( "TIPOIVA_ID", rsliquidaciones->toDouble( 7 ) );
		liquid_det->setNew( false );
		liquid_det->readRelated( true );
		liquid_det->setNew( true );
		facturaventa->getListFacturaVentaDet()->addRecord( liquid_det );
	}
	DBAPP->waitCursor( false);
	if( rsliquidaciones->getRowCount() == 0 ) {
		FrmBase::msgError( this, _("No se han encontrado liquidaciones para esta cliente.") );
	}
	delete rsliquidaciones;
	if( facturaventa->getListFacturaVentaDet()->size() == 0 ) {
		FrmBase::msgWarning( this, _("Todas las liquidaciones de esta cliente están liquidadas") );
	}
}


} // namespace deposit
} // namespace gong


