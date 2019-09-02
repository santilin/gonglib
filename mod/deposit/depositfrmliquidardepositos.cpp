#include <empresamodule.h>
#include "depositrecliquidacionventa.h"
#include "depositfrmliquidardepositos.h"

namespace gong {
namespace deposit {

FrmLiquidarDepositos::FrmLiquidarDepositos( Tipo tipo, QWidget* parent, WidgetFlags f)
	: FrmCustom( parent, "FrmLiquidarDepositos", f), mTipo( tipo )
{
	if( tipo == compras )
		setTitle( _("Liquidar depósitos de las proveedoras") );
	else
		setTitle( _("Liquidar depósitos a las clientes") );
	Date ini = Date(empresa::ModuleInstance->getEjercicio(), 1, 1 );
	Date fin = Date(empresa::ModuleInstance->getEjercicio(), 12, 31 );
	pDateRange = addDateRangeBox( 0, _("Fecha de los depósitos:"), ini, fin);
	if( tipo == compras )
		pSearchTercero = addMultipleSearchField( 0, "PROVEEDORA", "CODIGO", "RAZONSOCIAL" );
	else
		pSearchTercero = addMultipleSearchField( 0, "CLIENTE", "CODIGO", "RAZONSOCIAL" );
	pSearchTipoDocCond = addMultipleSearchField( 0, "TIPODOC", "CODIGO", "NOMBRE" );
	pSearchTipoDocCond->setText( _("Tipo(s) de documento(s) de los depósitos"));
	pDateRange->getEditDateFrom()->setFocus();
}

void FrmLiquidarDepositos::accept()
{
	Xtring where;
	List<dbRecordID> &terceros_ids = pSearchTercero->getRecordIDs();
	if( mTipo == compras ) {
		if( terceros_ids.size() ) {
			where += "(PROVEEDORA_ID IN(" + terceros_ids.join(",") + "))";
		} else {
			msgError(this, _("Selecciona al menos una proveedora") );
			return;
		}
	} else {
		if( terceros_ids.size() ) {
			where += "(CLIENTE_ID IN(" + terceros_ids.join(",") + "))";
		} else {
			msgError(this, _("Selecciona al menos una cliente") );
			return;
		}
	}
	List<dbRecordID> &tipodoc_ids = pSearchTipoDocCond->getRecordIDs();
	if( tipodoc_ids.size() ) {
		if( !where.isEmpty() )
			where+="AND";
		where += "(TIPODOC_ID IN(" + tipodoc_ids.join(",") + "))";
	}
	DBAPP->waitCursor( true );
	if( mTipo == ventas ) {
		try {
			for( List<dbRecordID>::const_iterator it = terceros_ids.begin();
				it != terceros_ids.end(); ++ it ) {
				DBAPP->waitCursor( true );
				RecLiquidacionVenta *liqventa = genLiquidacionVenta( *it, where );
				if ( liqventa ) {
					DBAPP->editRecord( 0, liqventa, 0, DataTable::inserting );
				}
			}
		} catch ( std::exception &e ) {
			FrmBase::msgError( this, e.what() );
		}
	}
	DBAPP->resetCursor();
}

RecLiquidacionVenta *FrmLiquidarDepositos::genLiquidacionVenta(
	dbRecordID cliente_id,	const Xtring &cond )
{
	RecLiquidacionVenta *liquidacion = 0;
	Xtring sql = "SELECT AVD.ID, SUM(CANTIDAD), ARTICULO_ID, SUM(IMPORTE), SUM(IMPORTECONIVA), AVD.TIPOIVA_ID, AVD.DTOP100, AVD.PVP, AVD.PVPSINIVA";
	Xtring from =
		" FROM ALBARANVENTA AV INNER JOIN ALBARANVENTADET AVD ON AVD.ALBARANVENTA_ID=AV.ID"
		" INNER JOIN ARTICULO A ON A.ID = AVD.ARTICULO_ID";
	Xtring where = cond;
	if( !pDateRange->getDateFrom().isNull() ) {
		if( !where.isEmpty() )
			where += " AND ";
		where += "AV.FECHA >= " + DBAPP->getConnection()->toSQL( pDateRange->getDateTimeFrom() );
	}
	if( !pDateRange->getDateTo().isNull() ) {
		if( !where.isEmpty() )
			where += " AND ";
		where += "AV.FECHA <= " + DBAPP->getConnection()->toSQL( pDateRange->getDateTimeTo() );
	}
	if( !where.isEmpty() )
		where = " WHERE " + where;
	// Hay que agrupar por el pvpsiniva para que salgan distintas líneas si los precios difieren
	Xtring groupby = " GROUP BY A.ID, AVD.PVP, AVD.DTOP100, AVD.TIPOIVA_ID"
		" ORDER BY A.NOMBRE";
	dbResultSet *rsventas = DBAPP->getConnection()->select( sql + from + where + groupby );
	liquidacion = static_cast<RecLiquidacionVenta *>
		( DBAPP->createRecord( "LIQUIDACIONVENTA" ) );
	_GONG_DEBUG_ASSERT( liquidacion );
	liquidacion->clear( true );
	liquidacion->setValue( "CLIENTE_ID", cliente_id );
	liquidacion->getRecCliente()->read( cliente_id );
	if( rsventas->getRowCount() > 0 ) {
		liquidacion->setValue( "FECHA", Date::currentDate() );
		while( rsventas->next() ) {
			double cantidad = rsventas->toDouble( 1 );
			dbRecordID articulo_id = rsventas->toUInt( 2 );
			Money importe = Money(rsventas->toDouble( 3 ), 4);
			Money importeconiva = rsventas->toMoney( 4 );
			int tipoiva_id = rsventas->toInt( 5 );
			double dtop100 = rsventas->toDouble( 6 );
			Money pvp = rsventas->toMoney( 7 );
			Money pvpsiniva = Money(rsventas->toDouble( 8 ), 4);
			RecLiquidacionVentaDet *liquid_det = static_cast<RecLiquidacionVentaDet *>
					( DBAPP->createRecord( "LIQUIDACIONVENTADET" ) );
			liquid_det->setValue( "NLINEA", (long long int )(rsventas->getRowCount() + 1L) );
			liquid_det->setValue( "CANTIDAD", cantidad );
			liquid_det->setValue( "ARTICULO_ID", articulo_id );
			liquid_det->setValue( "PVPSINIVA", pvpsiniva );
			liquid_det->setValue( "PVP", pvp );
			liquid_det->setValue( "IMPORTE", importe );
			liquid_det->setValue( "IMPORTECONIVA", importeconiva );
			liquid_det->setValue( "TIPOIVA_ID", tipoiva_id );
			liquid_det->setValue( "DTOP100", dtop100 );
			liquid_det->setNew( false );
			liquid_det->readRelated( true );
			liquid_det->setNew( true );
			liquidacion->getListLiquidacionVentaDet()->addRecord( liquid_det );
		}
		delete rsventas;
	} else {
		FrmBase::msgError( this, Xtring::printf( _("No se han encontrado depósitos de %s %s"),
								   DBAPP->getTableDescSingular("CLIENTE", "la").c_str(),
								   liquidacion->getRecCliente()->getValue("RAZONSOCIAL").toString().c_str()) );
	}
    return liquidacion;
}

} // namespace deposit
} // namespace gong


