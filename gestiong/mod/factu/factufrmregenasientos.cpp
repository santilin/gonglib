#include "config.h"

#ifdef HAVE_CONTABMODULE

#include <dbappdbapplication.h>
#include <dbappfrmeditrecmaster.h>
#include <dbappmainwindow.h>
#include <empresamodule.h>
#include "factufrmregenasientos.h"
#include "facturecalbaranventa.h"
#include "facturecalbarancompra.h"
#include "facturecfacturaventa.h"
#include "facturecfacturacompra.h"
#include "pagosrecpago.h"
#include "pagosreccobro.h"

namespace gong {
namespace factu {

const char *osd_title = "Regenerando asientos de contabilidad";
const char *osd_content = "%d de %s procesados";
const char *osd_final = "%d asientos de %s regenerados";

FrmRegenAsientos::FrmRegenAsientos(QWidget* parent, const char* name, WidgetFlags f)
    : FrmCustom( parent, name, f)
{
    setTitle( _("Regeneración de asientos de contabilidad") );
    chkAlbaranesCompra = addCheckBox( 0, DBAPP->getTableDescPlural( "ALBARANCOMPRA" ), false );
    chkFacturasCompra = addCheckBox( 0, DBAPP->getTableDescPlural( "FACTURACOMPRA" ), false );
    chkAlbaranesVenta = addCheckBox( 0, DBAPP->getTableDescPlural( "ALBARANVENTA" ), false );
    chkFacturasVenta = addCheckBox( 0, DBAPP->getTableDescPlural( "FACTURAVENTA" ), false );
    chkPagos = addCheckBox( 0, DBAPP->getTableDescPlural( "PAGO" ), false );
    chkCobros = addCheckBox( 0, DBAPP->getTableDescPlural( "COBRO" ), false );
    Date ini = Date(empresa::ModuleInstance->getEjercicio(), 1, 1 );
    Date fin = Date(empresa::ModuleInstance->getEjercicio(), 12, 31 );
    pDateRange = addDateRangeBox( 0, _("Fecha de los documentos:"), ini, fin);
    chkRevisar = addCheckBox( 0, _("Revisar los asientos generados uno por uno"), true );
}

void FrmRegenAsientos::validate_input( QWidget *sender, bool *isvalid )
{
    FrmCustom::validate_input( sender, isvalid );
}

void FrmRegenAsientos::accept()
{
    if( pDateRange->getDateFrom().getYear() != empresa::ModuleInstance->getEjercicio() ) {
        msgError( this, _( "La fecha de inicio debe de ser de este ejercicio" ) );
        return;
    }
    if( pDateRange->getDateTo().getYear() != empresa::ModuleInstance->getEjercicio() ) {
        msgError( this, _( "La fecha de fin debe de ser de este ejercicio" ) );
        return;
    }
    if( chkAlbaranesCompra->isChecked() )
        regenAlbaranesCompra();
    if( chkFacturasCompra->isChecked() )
        regenFacturasCompra();
    if( chkAlbaranesVenta->isChecked() )
        regenAlbaranesVenta();
    if( chkFacturasVenta->isChecked() )
        regenFacturasVenta();
    if( chkPagos->isChecked() )
        regenPagos();
}

Xtring FrmRegenAsientos::genSelect(const Xtring& tablename)
{
    Xtring sql = "SELECT ID FROM " + tablename;
    Xtring cond;
    if( !pDateRange->getDateFrom().isNull() )
        cond += tablename + ".FECHA >= " + DBAPP->getConnection()->toSQL( pDateRange->getDateTimeFrom() );
    if( !pDateRange->getDateTo().isNull() ) {
        if( !cond.isEmpty() )
            cond += " AND " ;
        cond += tablename + ".FECHA <= " + DBAPP->getConnection()->toSQL( pDateRange->getDateTimeTo() );
    }
    if( !cond.isEmpty() )
        cond = " WHERE " + cond;
    return sql + cond;
}


void FrmRegenAsientos::regenAlbaranesCompra()
{
    Xtring sql = genSelect( "ALBARANCOMPRA" );
    dbResultSet *rs = DBAPP->getConnection()->select ( sql ) ;
    uint nregen = 0;
    while( rs->next() ) {
        dbRecordID id = rs->toInt(0);
        RecAlbaranCompra *record = static_cast<RecAlbaranCompra *>( DBAPP->createRecord( "ALBARANCOMPRA" ) );
        if( record->read( id ) )
            if( record->regenAsiento( chkRevisar->isChecked() ) )
                nregen ++;
        if( rs->getRowNumber() % 5 == 0 ) {
            DBAPP->showOSD( _( osd_title ),
                            Xtring::printf( _( osd_content ), rs->getRowNumber() + 1,
                                            DBAPP->getTableDescPlural("ALBARANCOMPRA").c_str() ) );
        }
    }
    delete rs;
    DBAPP->showOSD( osd_title,
                    Xtring::printf( _( osd_final ), nregen,
                                    DBAPP->getTableDescPlural("ALBARANCOMPRA").c_str() ) );
}

void FrmRegenAsientos::regenAlbaranesVenta()
{
    Xtring sql = genSelect( "ALBARANVENTA" );
    dbResultSet *rs = DBAPP->getConnection()->select ( sql ) ;
    uint nregen = 0;
    while( rs->next() ) {
        dbRecordID id = rs->toInt(0);
        RecAlbaranVenta *record = static_cast<RecAlbaranVenta *>( DBAPP->createRecord( "ALBARANVENTA" ) );
        if( record->read( id ) )
            if( record->regenAsiento( chkRevisar->isChecked() ) )
                nregen ++;
        if( rs->getRowNumber() % 5 == 0 ) {
            DBAPP->showOSD( _( osd_title ),
                            Xtring::printf( _( osd_content ), rs->getRowNumber() + 1,
                                            DBAPP->getTableDescPlural("ALBARANVENTA").c_str() ) );
        }
    }
    delete rs;
}

#if 0
void FrmRegenAsientos::regenCobros()
{
    Xtring sql = genSelect( "COBRO" );
    dbResultSet *rs = DBAPP->getConnection()->select ( sql ) ;
    uint nregen = 0;
    for( uint row = 0; row < rs->getRowCount(); ++ row ) {
        dbRecordID id = rs->at(row).toInt(0);
        pagos::RecCobro *record = static_cast<pagos::RecCobro *>( DBAPP->createRecord( "COBRO" ) );
        if( record->read( id ) )
            if( record->regenAsiento( chkRevisar->isChecked() ) )
                nregen ++;
        if( row % 5 == 0 ) {
            DBAPP->showOSD( _( osd_title ),
                            Xtring::printf( _( osd_content ), row + 1,
                                            DBAPP->getTableDescPlural("COBRO").c_str() ) );
        }
    }
}
#endif

void FrmRegenAsientos::regenFacturasCompra()
{
    Xtring sql = genSelect( "FACTURACOMPRA" );
    dbResultSet *rs = DBAPP->getConnection()->select ( sql ) ;
    uint nregen = 0;
    while( rs->next() ) {
        dbRecordID id = rs->toInt(0);
        RecFacturaCompra *record = static_cast<RecFacturaCompra *>( DBAPP->createRecord( "FACTURACOMPRA" ) );
        if( record->read( id ) )
            if( record->regenAsiento( chkRevisar->isChecked() ) )
                nregen ++;
        if( rs->getRowNumber() % 5 == 0 ) {
            DBAPP->showOSD( _( osd_title ),
                            Xtring::printf( _( osd_content ), rs->getRowNumber() + 1,
                                            DBAPP->getTableDescPlural("FACTURACOMPRA").c_str() ) );
        }
    }
    delete rs;
}

void FrmRegenAsientos::regenFacturasVenta()
{
    Xtring sql = genSelect( "FACTURAVENTA" );
    dbResultSet *rs = DBAPP->getConnection()->select ( sql ) ;
    uint nregen = 0;
    while( rs->next() ) {
        dbRecordID id = rs->toInt(0);
        RecFacturaVenta *record = static_cast<RecFacturaVenta *>( DBAPP->createRecord( "FACTURAVENTA" ) );
        if( record->read( id ) )
            if( record->regenAsiento( chkRevisar->isChecked() ) )
                nregen ++;
        if( rs->getRowNumber() % 5 == 0 ) {
            DBAPP->showOSD( _( osd_title ),
                            Xtring::printf( _( osd_content ), rs->getRowNumber() + 1,
                                            DBAPP->getTableDescPlural("FACTURAVENTA").c_str() ) );
        }
    }
    delete rs;
}

void FrmRegenAsientos::regenPagos()
{
#if 0
    Xtring sql = genSelect( "PAGO" );
    dbResultSet *rs = DBAPP->getConnection()->select ( sql ) ;
    uint nregen = 0;
    for( uint row = 0; row < rs->getRowCount(); ++ row ) {
        dbRecordID id = rs->at(row).toInt(0);
        pagos::RecPago *record = static_cast<pagos::RecPago *>( DBAPP->createRecord( "PAGO" ) );
        if( record->read( id ) )
            if( record->regenAsiento( chkRevisar->isChecked() ) )
                nregen ++;
        if( row % 5 == 0 ) {
            DBAPP->showOSD( _( osd_title ),
                            Xtring::printf( _( osd_content ), row + 1,
                                            DBAPP->getTableDescPlural("PAGO").c_str() ) );
        }
    }
#endif
}

} // namespace factu
} // namespace gong

#endif // HAVE_CONTABMODULE

