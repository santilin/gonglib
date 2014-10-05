#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "factufrmcopiardocumento.h"
#include <empresamodule.h>
#include <facturecalbaranventa.h>
#include <facturecfacturaventa.h>
#include <facturecalbarancompra.h>
#include <facturecfacturacompra.h>

namespace gong {
namespace factu {

FrmCopiarDocumento::FrmCopiarDocumento(QWidget* parent, const char* name, WidgetFlags f)
    : FrmCustom( parent, name, f)
{
    pushAccept->setVisible(false);
    setTitle( _("Copiar documentos de otras empresas o ejercicios") );
    pSearchEmpresa = addSearchField( 0, "EMPRESA", "CODIGO", "NOMBRE" );
    pEditEjercicio = addInput( 0, _("Ejercicio"), empresa::ModuleInstance->getEjercicio(), "INTEGER" );
//	chkRevisar = addCheckBox( 0, _("Revisar los pedidos generados uno por uno"), true );
//	validate( comboDesde, 0);
    pButtonCopiaAlbaranVenta = addButton(0, DBAPP->getTableDescSingular("ALBARANVENTA"), "copiaralbaranventa");
    pButtonCopiaAlbaranCompra = addButton(0, DBAPP->getTableDescSingular("ALBARANCOMPRA"), "copiaralbarancompra");
    pButtonCopiaFacturaVenta = addButton(0, DBAPP->getTableDescSingular("FACTURAVENTA"), "copiarfacturaventa");
    pButtonCopiaFacturaCompra = addButton(0, DBAPP->getTableDescSingular("FACTURACOMPRA"), "copiarfacturacompra");
    pFocusWidget = pSearchEmpresa->getEditCode();
}

void FrmCopiarDocumento::validate_input( QWidget *sender, bool *isvalid )
{
    FrmCustom::validate_input( sender, isvalid );
    bool dummy_is_valid = false;
    if( isvalid == 0 )
        isvalid = &dummy_is_valid;
    List<dbRecordID> &empresa_ids = pSearchEmpresa->getRecordIDs();
    if( empresa_ids.size() != 1 ) {
        msgError(this, _("Por favor, elige una empresa" ) );
        return;
    }
    if( sender && pEditEjercicio->toInt() == 0 ) {
        msgError(this, _("Por favor, introduce un ejercicio") );
        *isvalid = false;
        return;
    }
    if( empresa_ids[0] == empresa::ModuleInstance->getRecEmpresa()->getRecordID()
            && pEditEjercicio->toInt() == empresa::ModuleInstance->getEjercicio() ) {
        msgError(this, _("Por favor, elige una empresa y ejercicio distintos al actual" ) );
        return;
    }
    if( sender == pButtonCopiaAlbaranVenta ) {
        copiaAlbaranVenta();
    } else if( sender == pButtonCopiaFacturaVenta ) {
        copiaFacturaVenta();
    } else if( sender == pButtonCopiaAlbaranCompra ) {
        copiaAlbaranCompra();
    } else if( sender == pButtonCopiaFacturaCompra ) {
        copiaFacturaCompra();
    }
}


void FrmCopiarDocumento::accept()
{
}

void FrmCopiarDocumento::copiaAlbaranCompra()
{
    RecAlbaranCompra *orig_record = static_cast<RecAlbaranCompra *>( DBAPP->createRecord("ALBARANCOMPRA") );
    List<dbRecordID> &empresa_ids = pSearchEmpresa->getRecordIDs();
    orig_record->getFilters().clear();
    orig_record->addStructuralFilter( "ALBARANCOMPRA.EMPRESA_ID=" + orig_record->getConnection()->toSQL( empresa_ids[0] )
                                      + " AND ALBARANCOMPRA.EJERCICIO=" + orig_record->getConnection()->toSQL( pEditEjercicio->toInt() ) );
    orig_record->setValue( "EMPRESA_ID", empresa_ids[0] );
    orig_record->setValue( "EJERCICIO", pEditEjercicio->toInt() );
    dbRecordID orig_id = DBAPP->choose( static_cast<FrmEditRec *>(0), orig_record, 0, dbApplication::readOnly );
    if( orig_id == 0 )
        return;
    dbRecordList *detalles = orig_record->getListAlbaranCompraDet();
    for( dbRecordList::const_iterator it = detalles->begin();
            it != detalles->end(); ++it ) {
        dbRecord *detalle = *it;
        detalle->setNew( true );
    }
    // Se pone el setNew después de leer los detalles porque si no no los lee porque ID es cero
    orig_record->setNew( true );
    orig_record->setValue( "EMPRESA_ID", empresa::ModuleInstance->getRecEmpresa()->getRecordID() );
    orig_record->setValue( "EJERCICIO", empresa::ModuleInstance->getEjercicio() );
    orig_record->setValue( "FACTURADO", false );
    orig_record->getFilters().clear();
    orig_record->addStructuralFilter(
        "ALBARANCOMPRA.EMPRESA_ID=" + orig_record->getConnection()->toSQL(
            empresa::ModuleInstance->getRecEmpresa()->getRecordID() )
        + " AND ALBARANCOMPRA.EJERCICIO=" + orig_record->getConnection()->toSQL(
            empresa::ModuleInstance->getEjercicio() ) );
    DBAPP->getMainWindow()->createClient( DBAPP->createEditForm( 0, orig_record, 0, DataTable::updating,
                                          dbApplication::EditFlags( dbApplication::deleteRecord | dbApplication::simpleEdition ) ) );
}


void FrmCopiarDocumento::copiaFacturaCompra()
{
    RecFacturaCompra *orig_record = static_cast<RecFacturaCompra *>( DBAPP->createRecord("FACTURACOMPRA") );
    List<dbRecordID> &empresa_ids = pSearchEmpresa->getRecordIDs();
    orig_record->getFilters().clear();
    orig_record->addStructuralFilter( "FACTURACOMPRA.EMPRESA_ID=" + orig_record->getConnection()->toSQL( empresa_ids[0] )
                                      + " AND FACTURACOMPRA.EJERCICIO=" + orig_record->getConnection()->toSQL( pEditEjercicio->toInt() ) );
    orig_record->setValue( "EMPRESA_ID", empresa_ids[0] );
    orig_record->setValue( "EJERCICIO", pEditEjercicio->toInt() );
    dbRecordID orig_id = DBAPP->choose( static_cast<FrmEditRec *>(0), orig_record, 0, dbApplication::readOnly );
    if( orig_id == 0 )
        return;
    dbRecordList *detalles = orig_record->getListFacturaCompraDet();
    for( dbRecordList::const_iterator it = detalles->begin();
            it != detalles->end(); ++it ) {
        dbRecord *detalle = *it;
        detalle->setNew( true );
    }
    // Se pone el setNew después de leer los detalles porque si no no los lee porque ID es cero
    orig_record->setNew( true );
    orig_record->setValue( "EMPRESA_ID", empresa::ModuleInstance->getRecEmpresa()->getRecordID() );
    orig_record->setValue( "EJERCICIO", empresa::ModuleInstance->getEjercicio() );
    orig_record->getFilters().clear();
    orig_record->addStructuralFilter(
        "FACTURACOMPRA.EMPRESA_ID=" + orig_record->getConnection()->toSQL(
            empresa::ModuleInstance->getRecEmpresa()->getRecordID() )
        + " AND FACTURACOMPRA.EJERCICIO=" + orig_record->getConnection()->toSQL(
            empresa::ModuleInstance->getEjercicio() ) );
    DBAPP->getMainWindow()->createClient( DBAPP->createEditForm( 0, orig_record, 0, DataTable::updating,
                                          dbApplication::EditFlags( dbApplication::deleteRecord | dbApplication::simpleEdition ) ) );
}

void FrmCopiarDocumento::copiaAlbaranVenta()
{
    RecAlbaranVenta *orig_record = static_cast<RecAlbaranVenta *>( DBAPP->createRecord("ALBARANVENTA") );
    List<dbRecordID> &empresa_ids = pSearchEmpresa->getRecordIDs();
    orig_record->getFilters().clear();
    orig_record->addStructuralFilter( "ALBARANVENTA.EMPRESA_ID=" + orig_record->getConnection()->toSQL( empresa_ids[0] )
                                      + " AND ALBARANVENTA.EJERCICIO=" + orig_record->getConnection()->toSQL( pEditEjercicio->toInt() ) );
    orig_record->setValue( "EMPRESA_ID", empresa_ids[0] );
    orig_record->setValue( "EJERCICIO", pEditEjercicio->toInt() );
    dbRecordID orig_id = DBAPP->choose( static_cast<FrmEditRec *>(0), orig_record, 0, dbApplication::readOnly );
    if( orig_id == 0 )
        return;
    dbRecordList *detalles = orig_record->getListAlbaranVentaDet();
    for( dbRecordList::const_iterator it = detalles->begin();
            it != detalles->end(); ++it ) {
        dbRecord *detalle = *it;
        detalle->setNew( true );
    }
    // Se pone el setNew después de leer los detalles porque si no no los lee porque ID es cero
    orig_record->setNew( true );
    orig_record->setValue( "EMPRESA_ID", empresa::ModuleInstance->getRecEmpresa()->getRecordID() );
    orig_record->setValue( "EJERCICIO", empresa::ModuleInstance->getEjercicio() );
    orig_record->setValue( "FACTURADO", false );
    orig_record->getFilters().clear();
    orig_record->addStructuralFilter(
        "ALBARANVENTA.EMPRESA_ID=" + orig_record->getConnection()->toSQL(
            empresa::ModuleInstance->getRecEmpresa()->getRecordID() )
        + " AND ALBARANVENTA.EJERCICIO=" + orig_record->getConnection()->toSQL(
            empresa::ModuleInstance->getEjercicio() ) );
    DBAPP->getMainWindow()->createClient( DBAPP->createEditForm( 0, orig_record, 0, DataTable::updating,
                                          dbApplication::EditFlags( dbApplication::deleteRecord | dbApplication::simpleEdition ) ) );
}



void FrmCopiarDocumento::copiaFacturaVenta()
{
    RecFacturaVenta *orig_record = static_cast<RecFacturaVenta *>( DBAPP->createRecord("FACTURAVENTA") );
    List<dbRecordID> &empresa_ids = pSearchEmpresa->getRecordIDs();
    orig_record->getFilters().clear();
    orig_record->addStructuralFilter( "FACTURAVENTA.EMPRESA_ID=" + orig_record->getConnection()->toSQL( empresa_ids[0] )
                                      + " AND FACTURAVENTA.EJERCICIO=" + orig_record->getConnection()->toSQL( pEditEjercicio->toInt() ) );
    orig_record->setValue( "EMPRESA_ID", empresa_ids[0] );
    orig_record->setValue( "EJERCICIO", pEditEjercicio->toInt() );
    dbRecordID orig_id = DBAPP->choose( static_cast<FrmEditRec *>(0), orig_record, 0, dbApplication::readOnly );
    if( orig_id == 0 )
        return;
    dbRecordList *detalles = orig_record->getListFacturaVentaDet();
    for( dbRecordList::const_iterator it = detalles->begin();
            it != detalles->end(); ++it ) {
        dbRecord *detalle = *it;
        detalle->setNew( true );
    }
    // Se pone el setNew después de leer los detalles porque si no no los lee porque ID es cero
    orig_record->setNew( true );
    orig_record->setValue( "EMPRESA_ID", empresa::ModuleInstance->getRecEmpresa()->getRecordID() );
    orig_record->setValue( "EJERCICIO", empresa::ModuleInstance->getEjercicio() );
    orig_record->getFilters().clear();
    orig_record->addStructuralFilter(
        "FACTURAVENTA.EMPRESA_ID=" + orig_record->getConnection()->toSQL(
            empresa::ModuleInstance->getRecEmpresa()->getRecordID() )
        + " AND FACTURAVENTA.EJERCICIO=" + orig_record->getConnection()->toSQL(
            empresa::ModuleInstance->getEjercicio() ) );
    DBAPP->getMainWindow()->createClient( DBAPP->createEditForm( 0, orig_record, 0, DataTable::updating,
                                          dbApplication::EditFlags( dbApplication::deleteRecord | dbApplication::simpleEdition ) ) );
}

} // namespace factu
} // namespace gong


