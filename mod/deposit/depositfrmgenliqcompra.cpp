#include <dbappdbapplication.h>
#include <dbappfrmeditrecmaster.h>
#include <dbappmainwindow.h>
#include <empresamodule.h>
#include <facturecalbaranventa.h>
#include "depositfrmgenliqcompra.h"
#include "depositrecliquidacioncompra.h"

namespace gong {
namespace deposit {

FrmGenLiqCompra::FrmGenLiqCompra(QWidget* parent, WidgetFlags f)
	: FrmCustom( parent, "FrmGenLiqCompra", f)
{
	setTitle( _("Liquidación de depósitos de proveedoras a partir de nuestras ventas") );
	Date ini = Date(empresa::ModuleInstance->getEjercicio(), 1, 1 );
	Date fin = Date(empresa::ModuleInstance->getEjercicio(), 12, 31 );
	pDateRange = addDateRangeBox( 0, _("Fecha de las ventas:"), ini, fin);
	pSearchProveedora = addMultipleSearchField( 0, "PROVEEDORA", "CODIGO", "RAZONSOCIAL" );
	pSearchTipoDocCond = addMultipleSearchField( 0, "TIPODOC", "CODIGO", "NOMBRE" );
	pSearchTipoDocCond->setText(_("Tipo(s) de documento(s) de las ventas") );
	pSearchTipoDocLiquidacion = addSearchField( 0, "TIPODOC", "CODIGO", "NOMBRE" );
	pSearchTipoDocLiquidacion->setText(_("Tipo de documento a poner en las liquidaciones") );
	pCheckReliquidar = addCheckBox(0, _("Reliquidar las ventas"), false );
	pCheckMarcarVentasLiquidadas = addCheckBox(0, _("Marcar las ventas como liquidadas"), false );
}

void FrmGenLiqCompra::accept()
{
	Xtring where;
	List<dbRecordID> &proveedora_ids = pSearchProveedora->getRecordIDs();
	if( proveedora_ids.size() ) {
		where += "(PROVEEDORA_ID IN(" + proveedora_ids.join(",") + "))";
	} else {
		msgError(this, _("Selecciona al menos una proveedora") );
		return;
	}
	List<dbRecordID> &tipodoc_ids = pSearchTipoDocCond->getRecordIDs();;
	if( tipodoc_ids.size() ) {
		if( !where.isEmpty() )
			where+="AND";
		where += "(TIPODOC_ID IN(" + tipodoc_ids.join(",") + "))";
	}
	dbRecordID tipodocliq_id = pSearchTipoDocLiquidacion->getRecord()->getRecordID();
	if( tipodocliq_id == 0 ) {
		msgError(this, _("Selecciona un tipo de documento para la liquidación") );
		return;
	}
	if( !pCheckReliquidar->isOn() ) {
		if( !where.isEmpty() )
			where+="AND";
		where += "(AVD.LIQUIDACIONCOMPRA_ID IS NULL)";
	}
	factu::RecAlbaranVenta *albaranventa = static_cast<factu::RecAlbaranVenta *>(DBAPP->createRecord( "ALBARANVENTA" ));
	where = albaranventa->getFilter( "WHERE", where ).replace("ALBARANVENTA.", "AV.");
	DBAPP->waitCursor( true );
    try {
		List<dbRecordID> avdet_ids;
		for( List<dbRecordID>::const_iterator it = proveedora_ids.begin();
			it != proveedora_ids.end(); ++ it ) {
			avdet_ids.clear();
			RecLiquidacionCompra *liquidacion
				= genLiquidacionCompra( *it, tipodocliq_id, avdet_ids, where );
			if ( liquidacion ) {
				if( DBAPP->editRecord( 0, liquidacion, 0, DataTable::inserting ) ) {
					if( pCheckMarcarVentasLiquidadas->isOn() ) {
						Xtring sql_exec = "UPDATE ALBARANVENTADET SET LIQUIDACIONCOMPRA_ID="
							+ liquidacion->getConnection()->toSQL( liquidacion->getRecordID() )
							+ " WHERE ID IN(" + avdet_ids.join(",") + ")";
						if( liquidacion->getConnection()->exec( sql_exec ) )
							DBAPP->showStickyOSD(getTitle(), Xtring::printf( _("%d %s actualizados"),
														avdet_ids.size(), DBAPP->getTableDescPlural("ALBARANVENTADET").c_str() ) );
						else
							msgError( this, Xtring::printf( _("No se han podido actualizar los %s"),
															DBAPP->getTableDescPlural("ALBARANVENTADET").c_str() ) );
					}
				}
				delete liquidacion;
			}
		}
    } catch ( std::exception &e ) {
        FrmBase::msgError( this, e.what() );
    }
	DBAPP->resetCursor();
}

RecLiquidacionCompra *FrmGenLiqCompra::genLiquidacionCompra( dbRecordID proveedora_id,
				dbRecordID tipodocliq_id, List<dbRecordID> &avdet_ids, const Xtring &cond )
{
	RecLiquidacionCompra *liquidacion = 0;
	Xtring sql = "SELECT AVD.ID, SUM(CANTIDAD), ARTICULO_ID, SUM(IMPORTELIQUIDACION), SUM(ROUND(IMPORTELIQUIDACIONIVA,2)), AVD.TIPOIVA_ID";
	Xtring from =
		" FROM ALBARANVENTA AV INNER JOIN ALBARANVENTADET AVD ON AVD.ALBARANVENTA_ID=AV.ID"
		" INNER JOIN ARTICULO A ON A.ID = AVD.ARTICULO_ID";
	Xtring where = cond;
	if( !pDateRange->getDateFrom().isNull() )
		where += " AND AV.FECHA >= " + DBAPP->getConnection()->toSQL( pDateRange->getDateTimeFrom() );
	if( !pDateRange->getDateTo().isNull() )
		where += " AND AV.FECHA <= " + DBAPP->getConnection()->toSQL( pDateRange->getDateTimeTo() );
	// Hay que agrupar por el pvpsiniva para que salgan distintas líneas si los precios difieren
	Xtring groupby = " GROUP BY A.ID, ROUND(IMPORTELIQUIDACION/CANTIDAD,2), AVD.TIPOIVA_ID"
		" ORDER BY A.NOMBRE";
	dbResultSet *rsventas = DBAPP->getConnection()->select( sql + from + where + groupby );
	if( rsventas->getRowCount() > 0 ) {
		liquidacion = static_cast<RecLiquidacionCompra *>
            ( DBAPP->createRecord( "LIQUIDACIONCOMPRA" ) );
		if ( liquidacion ) {
			liquidacion->clear( true );
			liquidacion->setValue( "PROVEEDORA_ID", proveedora_id );
			liquidacion->getRecProveedora()->read( proveedora_id );
			liquidacion->setValue( "TIPODOC_ID", tipodocliq_id );
			liquidacion->getRecTipoDoc()->read( tipodocliq_id );
			liquidacion->setValue( "FECHA", Date::currentDate() );
			while( rsventas->next() ) {
				double cantidad = rsventas->toDouble( 1 );
				dbRecordID articulo_id = rsventas->toInt( 2 );
				Money importeliquidacion = Money(rsventas->toDouble( 3 ), 4);
				Money importeliquidacioniva = Money(rsventas->toDouble( 4 ), 2);
				int tipoiva_id = rsventas->toInt( 5 );
				Money costesiniva = importeliquidacion / cantidad;
				Money coste = importeliquidacioniva / cantidad;
				RecLiquidacionCompraDet *liquid_det = static_cast<RecLiquidacionCompraDet *>
						( DBAPP->createRecord( "LIQUIDACIONCOMPRADET" ) );
				liquid_det->setValue( "NLINEA", (long long int)(rsventas->getRowCount() + 1L) );
				liquid_det->setValue( "CANTIDAD", cantidad );
				liquid_det->setValue( "ARTICULO_ID", articulo_id );
				liquid_det->setValue( "COSTESINIVA", costesiniva );
				liquid_det->setValue( "COSTE", coste );
				liquid_det->setValue( "IMPORTE", importeliquidacion );
				liquid_det->setValue( "IMPORTECONIVA", importeliquidacioniva );
				liquid_det->setValue( "TIPOIVA_ID", tipoiva_id );
				liquid_det->setNew( false );
				liquid_det->readRelated( true );
				liquid_det->setNew( true );
				liquidacion->getListLiquidacionCompraDet()->addRecord( liquid_det );
			}
		}
		delete rsventas;
		// Segunda consulta para obtener todos los ids, sin agrupar
		rsventas = DBAPP->getConnection()->select( "SELECT AVD.ID " + from + where );
		while( rsventas->next() ) {
			dbRecordID avdet_id = rsventas->toInt( 0 );
			avdet_ids.push_back( avdet_id );
		}
	} else {
		FrmBase::msgError( this, _("No se han encontrado ventas de esta proveedora." ) );
	}
	delete rsventas;
    return liquidacion;
}


} // namespace deposit
} // namespace gong


