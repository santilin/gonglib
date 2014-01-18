/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de facturas de venta
// FIELD Fecha date - cabecera
// FIELD TipoDoc_ID Reference(TipoDoc,Codigo,Nombre) - cabecera
// FIELD Contador int - cabecera
// FIELD Numero string - cabecera
// FIELD Cliente_ID Reference(Cliente,Codigo,RazonSocial,dbApplication::InsertIfNotFound) - cabecera2
// FIELD IVADetallado comboint - cabecera2
// FIELD FormaPago_ID Reference(pagos::FormaPago,Codigo,Nombre,dbApplication::InsertIfNotFound) - cabecera3
// FIELD Agente_ID Reference(Agente,Codigo,RazonSocial,dbApplication::InsertIfNotFound) - cabecera3
// FIELD FacturaVentaDet FrmEditRecDetail
// FIELD SumaImportes money - noaddrightSumaImportes
// FIELD DtoP100 double - noaddrightDtoP100
// FIELD Descuento money - noaddrightDescuento
// FIELD BaseImponible money - noaddrightBaseImponible
// FIELD RecargoEquivalencia money - noaddrightRecargoEquivalencia
// FIELD IVA money - noaddrightIVA
// FIELD Total money - noaddrightTotal
// FIELD EntregaAlbaranes money - noaddrightEntregaAlbaranes
// FIELD Cobros money - noaddrightCobros
// FIELD Entrega money - noaddrightEntrega
// FIELD Resto money - noaddrightResto
// FIELD Proyecto_ID Reference(empresa::Proyecto,Codigo,Nombre) tabPagos proyecto if(empresa::ModuleInstance->usaProyectos())
// FIELD FechaIVA date tabPagos desgloseiva
// FIELD DesgloseIVA string tabPagos desgloseiva
// FIELD DocumentoPago string tabPagos pago
// FIELD FechaPago date tabPagos pago
// FIELD CuentaPago_ID Reference(Cuenta,Cuenta,Descripcion) tabPagos pago MODULE_INCLUDED(Contab)
// FIELD Notas text tabPagos notas
// TYPE FrmEditRecMaster factu::FacturaVenta Factura Venta
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITFACTURAVENTA_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "factufrmeditfacturaventa.h"
/*>>>>>FRMEDITFACTURAVENTA_INCLUDES*/
#include <empresamodule.h>
#include "factumodule.h"
#include "facturecalbaranventa.h"
#include "factufrmpagar.h"

namespace gong {
namespace factu {

/*<<<<<FRMEDITFACTURAVENTA_CONSTRUCTOR*/
FrmEditFacturaVenta::FrmEditFacturaVenta(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditFacturaVenta" );
/*>>>>>FRMEDITFACTURAVENTA_CONSTRUCTOR*/
    /*<<<<<FRMEDITFACTURAVENTA_INIT_CONTROLS*/
	QHBoxLayout *cabeceraLayout = new QHBoxLayout(0, 0, 6, "cabeceraLayout");
	QHBoxLayout *cabecera2Layout = new QHBoxLayout(0, 0, 6, "cabecera2Layout");
	QHBoxLayout *cabecera3Layout = new QHBoxLayout(0, 0, 6, "cabecera3Layout");
	QHBoxLayout *facturaventadetLayout = new QHBoxLayout(0, 0, 6, "facturaventadetLayout");
	QHBoxLayout *rightSumaImportesLayout = new QHBoxLayout(0, 0, 6, "rightSumaImportesLayout");
	QHBoxLayout *rightDtoP100Layout = new QHBoxLayout(0, 0, 6, "rightDtoP100Layout");
	QHBoxLayout *rightDescuentoLayout = new QHBoxLayout(0, 0, 6, "rightDescuentoLayout");
	QHBoxLayout *rightBaseImponibleLayout = new QHBoxLayout(0, 0, 6, "rightBaseImponibleLayout");
	QHBoxLayout *rightRecargoEquivalenciaLayout = new QHBoxLayout(0, 0, 6, "rightRecargoEquivalenciaLayout");
	QHBoxLayout *rightIVALayout = new QHBoxLayout(0, 0, 6, "rightIVALayout");
	QHBoxLayout *rightTotalLayout = new QHBoxLayout(0, 0, 6, "rightTotalLayout");
	QHBoxLayout *rightEntregaAlbaranesLayout = new QHBoxLayout(0, 0, 6, "rightEntregaAlbaranesLayout");
	QHBoxLayout *rightCobrosLayout = new QHBoxLayout(0, 0, 6, "rightCobrosLayout");
	QHBoxLayout *rightEntregaLayout = new QHBoxLayout(0, 0, 6, "rightEntregaLayout");
	QHBoxLayout *rightRestoLayout = new QHBoxLayout(0, 0, 6, "rightRestoLayout");
	showTabs(true);
	QWidget *tabPagos = new QWidget( pTabWidget, "tabPagos" );
	QVBoxLayout *tabPagosLayout = new QVBoxLayout(tabPagos, 11, 6, "tabPagosLayout");
	QHBoxLayout *proyectoLayout = new QHBoxLayout(0, 0, 6, "proyectoLayout");
	QHBoxLayout *desgloseivaLayout = new QHBoxLayout(0, 0, 6, "desgloseivaLayout");
	QHBoxLayout *pagoLayout = new QHBoxLayout(0, 0, 6, "pagoLayout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
	editFecha = addEditField( pControlsFrame, "FACTURAVENTA", "FECHA", cabeceraLayout );

	searchTipoDocCodigo = addSearchField( pControlsFrame, "TIPODOC_ID", "TIPODOC", "CODIGO", "NOMBRE", cabeceraLayout );
	pushTipoDocCodigo = searchTipoDocCodigo->getButton();
	connect( pushTipoDocCodigo, SIGNAL( clicked() ), this, SLOT( pushTipoDocCodigo_clicked() ) );
	editTipoDocCodigo = searchTipoDocCodigo->getEditCode();
	editTipoDocNombre = searchTipoDocCodigo->getEditDesc();
	editContador = addEditField( pControlsFrame, "FACTURAVENTA", "CONTADOR", cabeceraLayout );
	editNumero = addEditField( pControlsFrame, "FACTURAVENTA", "NUMERO", cabeceraLayout );

	searchClienteCodigo = addSearchField( pControlsFrame, "CLIENTE_ID", "CLIENTE", "CODIGO", "RAZONSOCIAL", cabecera2Layout );
	pushClienteCodigo = searchClienteCodigo->getButton();
	connect( pushClienteCodigo, SIGNAL( clicked() ), this, SLOT( pushClienteCodigo_clicked() ) );
	editClienteCodigo = searchClienteCodigo->getEditCode();
	editClienteRazonSocial = searchClienteCodigo->getEditDesc();
	comboIVADetallado = addComboIntField( pControlsFrame, "FACTURAVENTA", "IVADETALLADO", cabecera2Layout );

	searchFormaPagoCodigo = addSearchField( pControlsFrame, "FORMAPAGO_ID", "FORMAPAGO", "CODIGO", "NOMBRE", cabecera3Layout );
	pushFormaPagoCodigo = searchFormaPagoCodigo->getButton();
	connect( pushFormaPagoCodigo, SIGNAL( clicked() ), this, SLOT( pushFormaPagoCodigo_clicked() ) );
	editFormaPagoCodigo = searchFormaPagoCodigo->getEditCode();
	editFormaPagoNombre = searchFormaPagoCodigo->getEditDesc();

	searchAgenteCodigo = addSearchField( pControlsFrame, "AGENTE_ID", "AGENTE", "CODIGO", "RAZONSOCIAL", cabecera3Layout );
	pushAgenteCodigo = searchAgenteCodigo->getButton();
	connect( pushAgenteCodigo, SIGNAL( clicked() ), this, SLOT( pushAgenteCodigo_clicked() ) );
	editAgenteCodigo = searchAgenteCodigo->getEditCode();
	editAgenteRazonSocial = searchAgenteCodigo->getEditDesc();

// frmDetails: FacturaVentaDet
	QFrame *facturaventadetFrame = new QFrame(this);
	facturaventadetFrame->setFrameStyle(QFrame::Sunken | QFrame::Panel);
	facturaventadetFrame->setLineWidth(4);
	QVBoxLayout *facturaventadetFrameLayout = new QVBoxLayout(facturaventadetFrame);
	RecFacturaVentaDet *facturaventadet = getRecFacturaVenta()->getRecFacturaVentaDet(-1);
	dbViewDefinitionsList facturaventadetviews;
	DBAPP->getDatabase()->getViewsForTable( facturaventadet, facturaventadetviews );
	dbRecordListDataModel *facturaventadetdm =
		new dbRecordListDataModel(getRecFacturaVenta()->getListFacturaVentaDet(), facturaventadet, facturaventadetviews, "1=0");
	pFrmFacturaVentaDet = static_cast<FrmEditFacturaVentaDet *>(
		DBAPP->createEditDetailForm(this,
		-1, facturaventadet, "FACTURAVENTADET",facturaventadetdm, DataTable::browsing,
		static_cast<dbApplication::EditFlags>(dbApplication::embedded + dbApplication::tableEditEvent ),
		pControlsFrame) );
	pFrmFacturaVentaDet->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding);
	facturaventadetFrameLayout->addWidget( pFrmFacturaVentaDet );
	facturaventadetLayout->addWidget(facturaventadetFrame);
	editSumaImportes = addEditField( pControlsFrame, "FACTURAVENTA", "SUMAIMPORTES", rightSumaImportesLayout );
	editDtoP100 = addEditField( pControlsFrame, "FACTURAVENTA", "DTOP100", rightDtoP100Layout );
	editDescuento = addEditField( pControlsFrame, "FACTURAVENTA", "DESCUENTO", rightDescuentoLayout );
	editBaseImponible = addEditField( pControlsFrame, "FACTURAVENTA", "BASEIMPONIBLE", rightBaseImponibleLayout );
	editRecargoEquivalencia = addEditField( pControlsFrame, "FACTURAVENTA", "RECARGOEQUIVALENCIA", rightRecargoEquivalenciaLayout );
	editIVA = addEditField( pControlsFrame, "FACTURAVENTA", "IVA", rightIVALayout );
	editTotal = addEditField( pControlsFrame, "FACTURAVENTA", "TOTAL", rightTotalLayout );
	editEntregaAlbaranes = addEditField( pControlsFrame, "FACTURAVENTA", "ENTREGAALBARANES", rightEntregaAlbaranesLayout );
	editCobros = addEditField( pControlsFrame, "FACTURAVENTA", "COBROS", rightCobrosLayout );
	editEntrega = addEditField( pControlsFrame, "FACTURAVENTA", "ENTREGA", rightEntregaLayout );
	editResto = addEditField( pControlsFrame, "FACTURAVENTA", "RESTO", rightRestoLayout );

if(empresa::ModuleInstance->usaProyectos()){
	searchProyectoCodigo = addSearchField( tabPagos, "PROYECTO_ID", "PROYECTO", "CODIGO", "NOMBRE", proyectoLayout );
	pushProyectoCodigo = searchProyectoCodigo->getButton();
	connect( pushProyectoCodigo, SIGNAL( clicked() ), this, SLOT( pushProyectoCodigo_clicked() ) );
	editProyectoCodigo = searchProyectoCodigo->getEditCode();
	editProyectoNombre = searchProyectoCodigo->getEditDesc();
}
	editFechaIVA = addEditField( tabPagos, "FACTURAVENTA", "FECHAIVA", desgloseivaLayout );
	editDesgloseIVA = addEditField( tabPagos, "FACTURAVENTA", "DESGLOSEIVA", desgloseivaLayout );
	editDocumentoPago = addEditField( tabPagos, "FACTURAVENTA", "DOCUMENTOPAGO", pagoLayout );
	editFechaPago = addEditField( tabPagos, "FACTURAVENTA", "FECHAPAGO", pagoLayout );

#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	searchCuentaPagoCuenta = addSearchField( tabPagos, "CUENTAPAGO_ID", "CUENTA", "CUENTA", "DESCRIPCION", pagoLayout );
	pushCuentaPagoCuenta = searchCuentaPagoCuenta->getButton();
	connect( pushCuentaPagoCuenta, SIGNAL( clicked() ), this, SLOT( pushCuentaPagoCuenta_clicked() ) );
	editCuentaPagoCuenta = searchCuentaPagoCuenta->getEditCode();
	editCuentaPagoDescripcion = searchCuentaPagoCuenta->getEditDesc();
}
#endif
	editNotas = addTextField( tabPagos, "FACTURAVENTA", "NOTAS", notasLayout );
	pControlsLayout->addLayout( cabeceraLayout );
	pControlsLayout->addLayout( cabecera2Layout );
	pControlsLayout->addLayout( cabecera3Layout );
	pControlsLayout->addLayout( facturaventadetLayout );
	alignLayout( rightSumaImportesLayout, false);
	alignLayout( rightDtoP100Layout, false);
	alignLayout( rightDescuentoLayout, false);
	alignLayout( rightBaseImponibleLayout, false);
	alignLayout( rightRecargoEquivalenciaLayout, false);
	alignLayout( rightIVALayout, false);
	alignLayout( rightTotalLayout, false);
	alignLayout( rightEntregaAlbaranesLayout, false);
	alignLayout( rightCobrosLayout, false);
	alignLayout( rightEntregaLayout, false);
	alignLayout( rightRestoLayout, false);
	tabPagosLayout->addLayout( proyectoLayout );
	tabPagosLayout->addLayout( desgloseivaLayout );
	tabPagosLayout->addLayout( pagoLayout );
	tabPagosLayout->addLayout( notasLayout );
/*>>>>>FRMEDITFACTURAVENTA_INIT_CONTROLS*/
    pTabWidget->insertTab( tabPagos, toGUI( _( "&Extra" ) ) );

    QGridLayout *gridlayout = new QGridLayout();
    gridlayout->addLayout( rightSumaImportesLayout, 0, 1 );
    gridlayout->addLayout( rightDtoP100Layout, 0, 2 );
    gridlayout->addLayout( rightDescuentoLayout, 0, 3 );
    gridlayout->addLayout( rightBaseImponibleLayout, 1, 0 );
    gridlayout->addLayout( rightIVALayout, 1, 1 );
    gridlayout->addLayout( rightRecargoEquivalenciaLayout, 1, 2 );
    gridlayout->addLayout( rightTotalLayout, 1, 3 );
    gridlayout->addLayout( rightEntregaAlbaranesLayout, 2, 0 );
    gridlayout->addLayout( rightCobrosLayout, 2, 1 );
    gridlayout->addLayout( rightEntregaLayout, 2, 2 );
    gridlayout->addLayout( rightRestoLayout, 2, 3 );
    pControlsLayout->addLayout( gridlayout );

    pushCobrar = new QPushButton( this );
    connect( pushCobrar, SIGNAL( clicked() ), this, SLOT( slotCobrar() ) );
    rightEntregaAlbaranesLayout->insertWidget(0, pushCobrar);

    editNumero->setMustBeReadOnly( true );
    editSumaImportes->setMustBeReadOnly( true );
    editBaseImponible->setMustBeReadOnly( true );
    editRecargoEquivalencia->setMustBeReadOnly( true );
    editEntregaAlbaranes->setMustBeReadOnly( true );
    editCobros->setMustBeReadOnly( true );
    editIVA->setMustBeReadOnly( true );
    editDescuento->setMustBeReadOnly( true );
    editResto->setMustBeReadOnly( true );
    editClienteCodigo->setWidthInChars(8);
    editAgenteCodigo->setWidthInChars(8);
    editFormaPagoCodigo->setWidthInChars(8);
    editNotas->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Minimum);
    mHasCobros = false;
}

void FrmEditFacturaVenta::scatterFields()
{
    if( isDuplicating() ) {
        getRecFacturaVenta()->setValue( "NUMERO", "" );
        getRecFacturaVenta()->setValue( "CONTADOR", 0 );
        getRecFacturaVenta()->setValue( "ENTREGAALBARANES", 0 );
        getRecFacturaVenta()->setValue( "ENTREGA", Money(0.0) );
        getRecFacturaVenta()->setValue( "COBROS", Money(0.0) );
        getRecFacturaVenta()->setValue( "RESTO", getRecFacturaVenta()->getValue( "TOTAL") );
        getRecFacturaVenta()->setValue( "FECHAPAGO", Date() );
        getRecFacturaVenta()->setValue( "FECHA", Date::currentDate() );
        getRecFacturaVenta()->setValue( "DOCUMENTOPAGO", 0 );
        getRecFacturaVenta()->setValue( "FECHAIVA", 0 );
#ifdef HAVE_CONTABMODULE
        getRecFacturaVenta()->setValue( "ASIENTO_ID", 0 );
        getRecCuentaPago()->clear( false );
#endif
        // No duplicar los albaranes facturados
        dbRecordList *reclst = getRecFacturaVenta()->getListFacturaVentaDet();
        for ( int i = reclst->size() - 1; i >= 0; --i ) {
            RecFacturaVentaDet *detalle = static_cast<RecFacturaVentaDet *>( reclst->at( i ) );
            if( detalle->getValue( "ALBARANVENTA_ID").toInt() != 0 ) {
                reclst->removeRecord( i );
                delete detalle;
            }
        }
    }
    /*<<<<<FRMEDITFACTURAVENTA_SCATTER*/
	editFecha->setText(getRecFacturaVenta()->getValue("FECHA").toDate());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editFecha;
	editContador->setText(getRecFacturaVenta()->getValue("CONTADOR").toInt());
	editNumero->setText(getRecFacturaVenta()->getValue("NUMERO").toString());
	comboIVADetallado->setCurrentItemByValue(getRecFacturaVenta()->getValue("IVADETALLADO").toInt());
	editSumaImportes->setText(getRecFacturaVenta()->getValue("SUMAIMPORTES").toMoney());
	editDtoP100->setText(getRecFacturaVenta()->getValue("DTOP100").toDouble());
	editDescuento->setText(getRecFacturaVenta()->getValue("DESCUENTO").toMoney());
	editBaseImponible->setText(getRecFacturaVenta()->getValue("BASEIMPONIBLE").toMoney());
	editRecargoEquivalencia->setText(getRecFacturaVenta()->getValue("RECARGOEQUIVALENCIA").toMoney());
	editIVA->setText(getRecFacturaVenta()->getValue("IVA").toMoney());
	editTotal->setText(getRecFacturaVenta()->getValue("TOTAL").toMoney());
	editEntregaAlbaranes->setText(getRecFacturaVenta()->getValue("ENTREGAALBARANES").toMoney());
	editCobros->setText(getRecFacturaVenta()->getValue("COBROS").toMoney());
	editEntrega->setText(getRecFacturaVenta()->getValue("ENTREGA").toMoney());
	editResto->setText(getRecFacturaVenta()->getValue("RESTO").toMoney());
	editFechaIVA->setText(getRecFacturaVenta()->getValue("FECHAIVA").toDate());
	editDesgloseIVA->setText(getRecFacturaVenta()->getValue("DESGLOSEIVA").toString());
	editDocumentoPago->setText(getRecFacturaVenta()->getValue("DOCUMENTOPAGO").toString());
	editFechaPago->setText(getRecFacturaVenta()->getValue("FECHAPAGO").toDate());
	editNotas->setText(getRecFacturaVenta()->getValue("NOTAS").toString());
	scatterTipoDoc();
	scatterCliente();
	scatterFormaPago();
	scatterAgente();
if(empresa::ModuleInstance->usaProyectos()){
	scatterProyecto();
}
#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	scatterCuentaPago();
}
#endif
/*>>>>>FRMEDITFACTURAVENTA_SCATTER*/
    if( isInserting() ) {
        if( !isDuplicating() && editFecha->toDate().isNull() )
            editFecha->setText( ModuleInstance->getWorkingDate() );
        if( isDuplicating() ) {
            editTipoDocCodigo->setJustEdited( true );
            validateFields( editTipoDocCodigo, 0 );
        }
    } else if( isUpdating() ) {
        pFocusWidget = pFrmFacturaVentaDet;
    }
    pFrmFacturaVentaDet->addDetailIfNeeded();
    searchClienteCodigo->setMustBeReadOnly( mHasCobros );
    searchFormaPagoCodigo->setMustBeReadOnly( mHasCobros );
    editFecha->setMustBeReadOnly( mHasCobros );
    editDtoP100->setMustBeReadOnly( mHasCobros );
    editTotal->setMustBeReadOnly( mHasCobros );
    editEntrega->setMustBeReadOnly( mHasCobros );
    pushCobrar->setVisible( !mHasCobros );
    scatterFormaPago(); // Para cambiar el texto del botón pagar después de actualizar los totales
    if( editFechaIVA->toDate().isNull() )
        editFechaIVA->setText( editFecha->toDate() );
    validateFields( comboIVADetallado, 0 ); // Para mostrar u ocultar el recargo de equivalencia
}

void FrmEditFacturaVenta::gatherFields()
{
    /*<<<<<FRMEDITFACTURAVENTA_GATHER*/
	getRecFacturaVenta()->setValue( "FECHA", editFecha->toDate());
	getRecFacturaVenta()->setValue( "TIPODOC_ID", getRecTipoDoc()->getRecordID() );
	getRecFacturaVenta()->setValue( "CONTADOR", editContador->toInt());
	getRecFacturaVenta()->setValue( "NUMERO", editNumero->toString());
	getRecFacturaVenta()->setValue( "CLIENTE_ID", getRecCliente()->getRecordID() );
	getRecFacturaVenta()->setValue( "IVADETALLADO", comboIVADetallado->getCurrentItemValue());
	getRecFacturaVenta()->setValue( "FORMAPAGO_ID", getRecFormaPago()->getRecordID() );
	getRecFacturaVenta()->setValue( "AGENTE_ID", getRecAgente()->getRecordID() );
	getRecFacturaVenta()->setValue( "SUMAIMPORTES", editSumaImportes->toMoney());
	getRecFacturaVenta()->setValue( "DTOP100", editDtoP100->toDouble());
	getRecFacturaVenta()->setValue( "DESCUENTO", editDescuento->toMoney());
	getRecFacturaVenta()->setValue( "BASEIMPONIBLE", editBaseImponible->toMoney());
	getRecFacturaVenta()->setValue( "RECARGOEQUIVALENCIA", editRecargoEquivalencia->toMoney());
	getRecFacturaVenta()->setValue( "IVA", editIVA->toMoney());
	getRecFacturaVenta()->setValue( "TOTAL", editTotal->toMoney());
	getRecFacturaVenta()->setValue( "ENTREGAALBARANES", editEntregaAlbaranes->toMoney());
	getRecFacturaVenta()->setValue( "COBROS", editCobros->toMoney());
	getRecFacturaVenta()->setValue( "ENTREGA", editEntrega->toMoney());
	getRecFacturaVenta()->setValue( "RESTO", editResto->toMoney());
if(empresa::ModuleInstance->usaProyectos()){
	getRecFacturaVenta()->setValue( "PROYECTO_ID", getRecProyecto()->getRecordID() );
}
	getRecFacturaVenta()->setValue( "FECHAIVA", editFechaIVA->toDate());
	getRecFacturaVenta()->setValue( "DESGLOSEIVA", editDesgloseIVA->toString());
	getRecFacturaVenta()->setValue( "DOCUMENTOPAGO", editDocumentoPago->toString());
	getRecFacturaVenta()->setValue( "FECHAPAGO", editFechaPago->toDate());
#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	getRecFacturaVenta()->setValue( "CUENTAPAGO_ID", getRecCuentaPago()->getRecordID() );
}
#endif
	getRecFacturaVenta()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITFACTURAVENTA_GATHER*/
    ModuleInstance->setWorkingDate( editFecha->toDate() );
}

/*<<<<<FRMEDITFACTURAVENTA_CABECERA_GENNUMDOC*/
void FrmEditFacturaVenta::genNumeroDocumento()
{
		editContador->setText( ModuleInstance->getMaxContador( "FACTURAVENTA",
			getRecord()->getValue("EMPRESA_ID").toInt(),
			ModuleInstance->getEmpresaModule()->getEjercicio(),
			getRecTipoDoc()->getValue("SERIE").toString() ) );
		editContador->setJustEdited( true );
		validateFields( editContador, 0 );
/*>>>>>FRMEDITFACTURAVENTA_CABECERA_GENNUMDOC*/
}


void FrmEditFacturaVenta::scatterTipoDoc()
{
    /*<<<<<FRMEDITFACTURAVENTA_SCATTER_TIPODOC*/
	editTipoDocCodigo->setText( getRecTipoDoc()->getValue("CODIGO") );
	editTipoDocNombre->setText( getRecTipoDoc()->getValue("NOMBRE") );
/*>>>>>FRMEDITFACTURAVENTA_SCATTER_TIPODOC*/
    if( editTipoDocCodigo->isJustEdited() ) {
        if( editFormaPagoCodigo->toString().isEmpty() )
            searchFormaPagoCodigo->setValue( getRecTipoDoc()->getValue("FORMAPAGO.CODIGO").toInt() );
        if( editAgenteCodigo->toString().isEmpty() )
            searchAgenteCodigo->setValue( getRecTipoDoc()->getRecAgente()->getValue("CODIGO") );
        if( getRecTipoDoc()->getValue("IVADetallado").toInt() != 0 )
            comboIVADetallado->setCurrentItemByValue( getRecTipoDoc()->getValue("IVADetallado").toInt() );
        genNumeroDocumento();
    }
}

void FrmEditFacturaVenta::pushTipoDocCodigo_clicked()
{
    /*<<<<<FRMEDITFACTURAVENTA_PUSH_TIPODOC_CODIGO_CLICKED*/
	char action = mControlKeyPressed;
	if( !isEditing() || searchTipoDocCodigo->mustBeReadOnly() )
		action = 'E';
	switch( action ) {
		case 'F':
		case '\0':
			editTipoDocCodigo->setJustEdited( false );
			editTipoDocCodigo->setCancelling();
			if( DBAPP->choose(this, getRecTipoDoc(), 0, dbApplication::editNone, this ) ) {
				setEdited(true);
				scatterTipoDoc();
				editTipoDocCodigo->setJustEdited( true );
				setWiseFocus(editTipoDocCodigo);
			}
			break;
		case 'M':
			{
				if( getRecTipoDoc()->getRecordID() ) {
					editTipoDocCodigo->setJustEdited( false );
					if( DBAPP->editRecord(this,
							getRecTipoDoc(), 0, DataTable::updating,
							dbApplication::simpleEdition, this ) ) {
						editTipoDocCodigo->setJustEdited( true );
						scatterTipoDoc();
					}
				setWiseFocus(editTipoDocCodigo);
				}
			}
			break;
		case 'E':
			{
				if( getRecTipoDoc()->getRecordID() != 0 ) {
					editTipoDocCodigo->setJustEdited( false );
					DBAPP->getMainWindow()->createClient( DBAPP->createEditForm(this, getRecTipoDoc(),
						0, DataTable::selecting, dbApplication::simpleEdition, this ) );
				}
			}
			break;
		case 'A':
			{
				RecTipoDoc *tmprec = static_cast<RecTipoDoc *>(DBAPP->createRecord( "TipoDoc" ));
				editTipoDocCodigo->setJustEdited( false );
				tmprec->clear( true ); // set default values
				DBAPP->setCodeNotFound( editTipoDocCodigo->toString() );
				if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
					dbApplication::simpleEdition, this ) ) {
					editTipoDocCodigo->setJustEdited( true );
					getRecTipoDoc()->copyRecord( tmprec );
					scatterTipoDoc();
				}
				setWiseFocus(editTipoDocCodigo);
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITFACTURAVENTA_PUSH_TIPODOC_CODIGO_CLICKED*/
}


void FrmEditFacturaVenta::scatterCliente()
{
    /*<<<<<FRMEDITFACTURAVENTA_SCATTER_CLIENTE*/
	editClienteCodigo->setText( getRecCliente()->getValue("CODIGO") );
	editClienteRazonSocial->setText( getRecCliente()->getValue("RAZONSOCIAL") );
/*>>>>>FRMEDITFACTURAVENTA_SCATTER_CLIENTE*/
    /*<<<<<FRMEDITFACTURAVENTA_CABECERA_SCATTER_CLIENTE*/
	if( editClienteCodigo->isJustEdited() ) {
		if( getRecCliente()->getRecFormaPago()->getValue("CODIGO").toInt() != 0 )
			searchFormaPagoCodigo->setValue( getRecCliente()->getRecFormaPago()->getValue("CODIGO") );
		if( getRecCliente()->getValue("IVADetallado").toInt() != 0 )
			comboIVADetallado->setCurrentItemByValue( getRecCliente()->getValue("IVADetallado").toInt() );
		if( getRecCliente()->getRecAgente()->getRecordID() != 0 )
			searchAgenteCodigo->setValue( getRecCliente()->getRecAgente()->getValue("CODIGO") );
	}
/*>>>>>FRMEDITFACTURAVENTA_CABECERA_SCATTER_CLIENTE*/
}

void FrmEditFacturaVenta::pushClienteCodigo_clicked()
{
    /*<<<<<FRMEDITFACTURAVENTA_PUSH_CLIENTE_CODIGO_CLICKED*/
	char action = mControlKeyPressed;
	if( !isEditing() || searchClienteCodigo->mustBeReadOnly() )
		action = 'E';
	switch( action ) {
		case 'F':
		case '\0':
			editClienteCodigo->setJustEdited( false );
			editClienteCodigo->setCancelling();
			if( DBAPP->choose(this, getRecCliente(), 0, dbApplication::editNone, this ) ) {
				setEdited(true);
				scatterCliente();
				editClienteCodigo->setJustEdited( true );
				setWiseFocus(editClienteCodigo);
			}
			break;
		case 'M':
			{
				if( getRecCliente()->getRecordID() ) {
					editClienteCodigo->setJustEdited( false );
					if( DBAPP->editRecord(this,
							getRecCliente(), 0, DataTable::updating,
							dbApplication::simpleEdition, this ) ) {
						editClienteCodigo->setJustEdited( true );
						scatterCliente();
					}
				setWiseFocus(editClienteCodigo);
				}
			}
			break;
		case 'E':
			{
				if( getRecCliente()->getRecordID() != 0 ) {
					editClienteCodigo->setJustEdited( false );
					DBAPP->getMainWindow()->createClient( DBAPP->createEditForm(this, getRecCliente(),
						0, DataTable::selecting, dbApplication::simpleEdition, this ) );
				}
			}
			break;
		case 'A':
			{
				RecCliente *tmprec = static_cast<RecCliente *>(DBAPP->createRecord( "Cliente" ));
				editClienteCodigo->setJustEdited( false );
				tmprec->clear( true ); // set default values
				DBAPP->setCodeNotFound( editClienteCodigo->toString() );
				if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
					dbApplication::simpleEdition, this ) ) {
					editClienteCodigo->setJustEdited( true );
					getRecCliente()->copyRecord( tmprec );
					scatterCliente();
				}
				setWiseFocus(editClienteCodigo);
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITFACTURAVENTA_PUSH_CLIENTE_CODIGO_CLICKED*/
}

void FrmEditFacturaVenta::scatterFormaPago()
{
    /*<<<<<FRMEDITFACTURAVENTA_SCATTER_FORMAPAGO*/
	editFormaPagoCodigo->setText( getRecFormaPago()->getValue("CODIGO") );
	editFormaPagoNombre->setText( getRecFormaPago()->getValue("NOMBRE") );
/*>>>>>FRMEDITFACTURAVENTA_SCATTER_FORMAPAGO*/
    if( getRecFormaPago()->getValue( "TIPOFORMAPAGO" ).toInt() == pagos::RecFormaPago::Contado
            || getRecFormaPago()->getValue( "TIPOFORMAPAGO" ).toInt() == pagos::RecFormaPago::SeIgnora ) {
        pushCobrar->setVisible( false );
        editEntrega->setReadOnly( true );
    } else {
        pushCobrar->setVisible( true );
        editEntrega->setReadOnly( false );
    }
    actTotales();
    if( editEntrega->toDouble() != 0.0 && editTotal->toDouble() != 0.0 )
        pushCobrar->setText( _("&Borrar entrega") );
    else
        pushCobrar->setText( _("&Entrega") );
}

void FrmEditFacturaVenta::pushFormaPagoCodigo_clicked()
{
    /*<<<<<FRMEDITFACTURAVENTA_PUSH_FORMAPAGO_CODIGO_CLICKED*/
	char action = mControlKeyPressed;
	if( !isEditing() || searchFormaPagoCodigo->mustBeReadOnly() )
		action = 'E';
	switch( action ) {
		case 'F':
		case '\0':
			editFormaPagoCodigo->setJustEdited( false );
			editFormaPagoCodigo->setCancelling();
			if( DBAPP->choose(this, getRecFormaPago(), 0, dbApplication::editNone, this ) ) {
				setEdited(true);
				scatterFormaPago();
				editFormaPagoCodigo->setJustEdited( true );
				setWiseFocus(editFormaPagoCodigo);
			}
			break;
		case 'M':
			{
				if( getRecFormaPago()->getRecordID() ) {
					editFormaPagoCodigo->setJustEdited( false );
					if( DBAPP->editRecord(this,
							getRecFormaPago(), 0, DataTable::updating,
							dbApplication::simpleEdition, this ) ) {
						editFormaPagoCodigo->setJustEdited( true );
						scatterFormaPago();
					}
				setWiseFocus(editFormaPagoCodigo);
				}
			}
			break;
		case 'E':
			{
				if( getRecFormaPago()->getRecordID() != 0 ) {
					editFormaPagoCodigo->setJustEdited( false );
					DBAPP->getMainWindow()->createClient( DBAPP->createEditForm(this, getRecFormaPago(),
						0, DataTable::selecting, dbApplication::simpleEdition, this ) );
				}
			}
			break;
		case 'A':
			{
				pagos::RecFormaPago *tmprec = static_cast<pagos::RecFormaPago *>(DBAPP->createRecord( "FormaPago" ));
				editFormaPagoCodigo->setJustEdited( false );
				tmprec->clear( true ); // set default values
				DBAPP->setCodeNotFound( editFormaPagoCodigo->toString() );
				if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
					dbApplication::simpleEdition, this ) ) {
					editFormaPagoCodigo->setJustEdited( true );
					getRecFormaPago()->copyRecord( tmprec );
					scatterFormaPago();
				}
				setWiseFocus(editFormaPagoCodigo);
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITFACTURAVENTA_PUSH_FORMAPAGO_CODIGO_CLICKED*/
}

void FrmEditFacturaVenta::scatterAgente()
{
    /*<<<<<FRMEDITFACTURAVENTA_SCATTER_AGENTE*/
	editAgenteCodigo->setText( getRecAgente()->getValue("CODIGO") );
	editAgenteRazonSocial->setText( getRecAgente()->getValue("RAZONSOCIAL") );
/*>>>>>FRMEDITFACTURAVENTA_SCATTER_AGENTE*/
}

void FrmEditFacturaVenta::pushAgenteCodigo_clicked()
{
    /*<<<<<FRMEDITFACTURAVENTA_PUSH_AGENTE_CODIGO_CLICKED*/
	char action = mControlKeyPressed;
	if( !isEditing() || searchAgenteCodigo->mustBeReadOnly() )
		action = 'E';
	switch( action ) {
		case 'F':
		case '\0':
			editAgenteCodigo->setJustEdited( false );
			editAgenteCodigo->setCancelling();
			if( DBAPP->choose(this, getRecAgente(), 0, dbApplication::editNone, this ) ) {
				setEdited(true);
				scatterAgente();
				editAgenteCodigo->setJustEdited( true );
				setWiseFocus(editAgenteCodigo);
			}
			break;
		case 'M':
			{
				if( getRecAgente()->getRecordID() ) {
					editAgenteCodigo->setJustEdited( false );
					if( DBAPP->editRecord(this,
							getRecAgente(), 0, DataTable::updating,
							dbApplication::simpleEdition, this ) ) {
						editAgenteCodigo->setJustEdited( true );
						scatterAgente();
					}
				setWiseFocus(editAgenteCodigo);
				}
			}
			break;
		case 'E':
			{
				if( getRecAgente()->getRecordID() != 0 ) {
					editAgenteCodigo->setJustEdited( false );
					DBAPP->getMainWindow()->createClient( DBAPP->createEditForm(this, getRecAgente(),
						0, DataTable::selecting, dbApplication::simpleEdition, this ) );
				}
			}
			break;
		case 'A':
			{
				RecAgente *tmprec = static_cast<RecAgente *>(DBAPP->createRecord( "Agente" ));
				editAgenteCodigo->setJustEdited( false );
				tmprec->clear( true ); // set default values
				DBAPP->setCodeNotFound( editAgenteCodigo->toString() );
				if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
					dbApplication::simpleEdition, this ) ) {
					editAgenteCodigo->setJustEdited( true );
					getRecAgente()->copyRecord( tmprec );
					scatterAgente();
				}
				setWiseFocus(editAgenteCodigo);
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITFACTURAVENTA_PUSH_AGENTE_CODIGO_CLICKED*/
}

void FrmEditFacturaVenta::scatterCuentaPago()
{
#ifdef HAVE_CONTABMODULE
#define getRecCuenta getRecCuentaPago
    /*<<<<<FRMEDITFACTURAVENTA_SCATTER_CUENTAPAGO*/
	editCuentaPagoCuenta->setText( getRecCuentaPago()->getValue("CUENTA") );
	editCuentaPagoDescripcion->setText( getRecCuentaPago()->getValue("DESCRIPCION") );
/*>>>>>FRMEDITFACTURAVENTA_SCATTER_CUENTAPAGO*/
#undef getRecCuentaPago
#endif
}
void FrmEditFacturaVenta::pushCuentaPagoCuenta_clicked()
{
#ifdef HAVE_CONTABMODULE
    /*<<<<<FRMEDITFACTURAVENTA_PUSH_CUENTAPAGO_CUENTA_CLICKED*/
	char action = mControlKeyPressed;
	if( !isEditing() || searchCuentaPagoCuenta->mustBeReadOnly() )
		action = 'E';
	switch( action ) {
		case 'F':
		case '\0':
			editCuentaPagoCuenta->setJustEdited( false );
			editCuentaPagoCuenta->setCancelling();
			if( DBAPP->choose(this, getRecCuentaPago(), 0, dbApplication::editNone, this ) ) {
				setEdited(true);
				scatterCuentaPago();
				editCuentaPagoCuenta->setJustEdited( true );
				setWiseFocus(editCuentaPagoCuenta);
			}
			break;
		case 'M':
			{
				if( getRecCuentaPago()->getRecordID() ) {
					editCuentaPagoCuenta->setJustEdited( false );
					if( DBAPP->editRecord(this,
							getRecCuentaPago(), 0, DataTable::updating,
							dbApplication::simpleEdition, this ) ) {
						editCuentaPagoCuenta->setJustEdited( true );
						scatterCuentaPago();
					}
				setWiseFocus(editCuentaPagoCuenta);
				}
			}
			break;
		case 'E':
			{
				if( getRecCuentaPago()->getRecordID() != 0 ) {
					editCuentaPagoCuenta->setJustEdited( false );
					DBAPP->getMainWindow()->createClient( DBAPP->createEditForm(this, getRecCuentaPago(),
						0, DataTable::selecting, dbApplication::simpleEdition, this ) );
				}
			}
			break;
		case 'A':
			{
				RecCuentaPago *tmprec = static_cast<RecCuentaPago *>(DBAPP->createRecord( "CuentaPago" ));
				editCuentaPagoCuenta->setJustEdited( false );
				tmprec->clear( true ); // set default values
				DBAPP->setCodeNotFound( editCuentaPagoCuenta->toString() );
				if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
					dbApplication::simpleEdition, this ) ) {
					editCuentaPagoCuenta->setJustEdited( true );
					getRecCuentaPago()->copyRecord( tmprec );
					scatterCuentaPago();
				}
				setWiseFocus(editCuentaPagoCuenta);
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITFACTURAVENTA_PUSH_CUENTAPAGO_CUENTA_CLICKED*/
#endif
}


void FrmEditFacturaVenta::scatterProyecto()
{
    /*<<<<<FRMEDITFACTURAVENTA_SCATTER_PROYECTO*/
	editProyectoCodigo->setText( getRecProyecto()->getValue("CODIGO") );
	editProyectoNombre->setText( getRecProyecto()->getValue("NOMBRE") );
/*>>>>>FRMEDITFACTURAVENTA_SCATTER_PROYECTO*/
}
void FrmEditFacturaVenta::pushProyectoCodigo_clicked()
{
    /*<<<<<FRMEDITFACTURAVENTA_PUSH_PROYECTO_CODIGO_CLICKED*/
	char action = mControlKeyPressed;
	if( !isEditing() || searchProyectoCodigo->mustBeReadOnly() )
		action = 'E';
	switch( action ) {
		case 'F':
		case '\0':
			editProyectoCodigo->setJustEdited( false );
			editProyectoCodigo->setCancelling();
			if( DBAPP->choose(this, getRecProyecto(), 0, dbApplication::editNone, this ) ) {
				setEdited(true);
				scatterProyecto();
				editProyectoCodigo->setJustEdited( true );
				setWiseFocus(editProyectoCodigo);
			}
			break;
		case 'M':
			{
				if( getRecProyecto()->getRecordID() ) {
					editProyectoCodigo->setJustEdited( false );
					if( DBAPP->editRecord(this,
							getRecProyecto(), 0, DataTable::updating,
							dbApplication::simpleEdition, this ) ) {
						editProyectoCodigo->setJustEdited( true );
						scatterProyecto();
					}
				setWiseFocus(editProyectoCodigo);
				}
			}
			break;
		case 'E':
			{
				if( getRecProyecto()->getRecordID() != 0 ) {
					editProyectoCodigo->setJustEdited( false );
					DBAPP->getMainWindow()->createClient( DBAPP->createEditForm(this, getRecProyecto(),
						0, DataTable::selecting, dbApplication::simpleEdition, this ) );
				}
			}
			break;
		case 'A':
			{
				empresa::RecProyecto *tmprec = static_cast<empresa::RecProyecto *>(DBAPP->createRecord( "Proyecto" ));
				editProyectoCodigo->setJustEdited( false );
				tmprec->clear( true ); // set default values
				DBAPP->setCodeNotFound( editProyectoCodigo->toString() );
				if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
					dbApplication::simpleEdition, this ) ) {
					editProyectoCodigo->setJustEdited( true );
					getRecProyecto()->copyRecord( tmprec );
					scatterProyecto();
				}
				setWiseFocus(editProyectoCodigo);
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITFACTURAVENTA_PUSH_PROYECTO_CODIGO_CLICKED*/
}

void FrmEditFacturaVenta::specialControlKeyPressed( QWidget *sender, char key )
{
    /*<<<<<FRMEDITFACTURAVENTA_SPECIALACTION*/
	mControlKeyPressed = key;
	FrmEditRecMaster::specialControlKeyPressed(sender,key); // calls the behaviors
	if( sender == editTipoDocCodigo )
		pushTipoDocCodigo_clicked();
	if( sender == editClienteCodigo )
		pushClienteCodigo_clicked();
	if( sender == editFormaPagoCodigo )
		pushFormaPagoCodigo_clicked();
	if( sender == editAgenteCodigo )
		pushAgenteCodigo_clicked();
if(empresa::ModuleInstance->usaProyectos()){
	if( sender == editProyectoCodigo )
		pushProyectoCodigo_clicked();
}
#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	if( sender == editCuentaPagoCuenta )
		pushCuentaPagoCuenta_clicked();
}
#endif
	mControlKeyPressed = '\0';
/*>>>>>FRMEDITFACTURAVENTA_SPECIALACTION*/
}

void FrmEditFacturaVenta::validateFields( QWidget *sender, bool *isvalid, ValidResult *ir )
{
    /*<<<<<FRMEDITFACTURAVENTA_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	if( !sender && !pRecord->isValid( ValidResult::editing, validresult ) )
			*isvalid = false;
	if( focusWidget() != pushTipoDocCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editTipoDocCodigo, editTipoDocNombre,
		getRecTipoDoc(), "CODIGO", "NOMBRE", Xtring::null) )
		scatterTipoDoc();
	if( focusWidget() != pushClienteCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editClienteCodigo, editClienteRazonSocial,
		getRecCliente(), "CODIGO", "RAZONSOCIAL", Xtring::null, dbApplication::SeekCodeFlags( dbApplication::InsertIfNotFound )) )
		scatterCliente();
	if( focusWidget() != pushFormaPagoCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editFormaPagoCodigo, editFormaPagoNombre,
		getRecFormaPago(), "CODIGO", "NOMBRE", Xtring::null, dbApplication::SeekCodeFlags( dbApplication::InsertIfNotFound )) )
		scatterFormaPago();
	if( focusWidget() != pushAgenteCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editAgenteCodigo, editAgenteRazonSocial,
		getRecAgente(), "CODIGO", "RAZONSOCIAL", Xtring::null, dbApplication::SeekCodeFlags( dbApplication::InsertIfNotFound )) )
		scatterAgente();
if(empresa::ModuleInstance->usaProyectos()){
	if( focusWidget() != pushProyectoCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editProyectoCodigo, editProyectoNombre,
		getRecProyecto(), "CODIGO", "NOMBRE", Xtring::null) )
		scatterProyecto();
}
/*>>>>>FRMEDITFACTURAVENTA_VALIDATE*/

#ifdef HAVE_CONTABMODULE
#define editCuentaCuenta editCuentaPagoCuenta
#define getRecCuenta getRecCuentaPago
#define scatterCuenta scatterCuentaPago
#include <contabvalidatecuenta.inc>
#endif

    if( sender == editEntrega && editEntrega->isJustEdited() ) {
        if( editEntrega->toDouble() == 0.0 ) {
            editFechaPago->setText( Xtring::null );
            editDocumentoPago->setText( "" );
#ifdef HAVE_CONTABMODULE
            searchCuentaPagoCuenta->setValue( "" );
#endif
        } else {
            if( editFechaPago->toString().isEmpty()
#ifdef HAVE_CONTABMODULE
                    && editCuentaPagoCuenta->toString().isEmpty()
#endif
              ) {
                editFechaPago->setText( editFecha->toDate() );
#ifdef HAVE_CONTABMODULE
                editCuentaPagoCuenta->setText( empresa::ModuleInstance->getRecEmpresa()->getValue("CUENTACAJA").toString() );
#endif
                editDocumentoPago->setText( _("Pago fact. " + editNumero->toString() ) );
            }
        }
        actTotales();
    }
    /*<<<<<FRMEDITFACTURAVENTA_CABECERA_VALIDATE*/
	if( sender == editDtoP100 && editDtoP100->isJustEdited() ) {
		editDescuento->setText( 0.0 );
		actTotales();
	}
	if( sender == comboIVADetallado ) {
		if( comboIVADetallado->getCurrentItemValue() == factu::FldIVADetallado::con_recargo ) {
			editRecargoEquivalencia->setVisible( true );
			editRecargoEquivalencia->getLabel()->setVisible( true );
		} else {
			editRecargoEquivalencia->setVisible( false );
			editRecargoEquivalencia->getLabel()->setVisible( false );
		}
		actTotales();
	}
	if( sender == editFecha || !sender ) {
		if( editFecha->toDate().getYear() != factu::ModuleInstance->getEmpresaModule()->getEjercicio() ) {
			validresult->addWarning( Xtring::printf(_("La fecha no es del ejercicio actual: %d"),
				factu::ModuleInstance->getEmpresaModule()->getEjercicio()),
				"FECHA" );
		}
	}
	if( sender == editTotal && editTotal->isJustEdited() ) {
		if( editBaseImponible->toDouble() != 0.0 ) {
			double nuevo_total = editTotal->toDouble();
			editDtoP100->setText( 0.0 );
			editDescuento->setText( 0.0 );
			actTotales();
			double orig_total = editTotal->toDouble();
			if( orig_total != nuevo_total ) {
				double orig_sumaimp = editSumaImportes->toDouble();
				double nuevo_sumaimp =  (nuevo_total * orig_sumaimp) / orig_total;
				double dtop100 = 100 - (100 * nuevo_sumaimp / orig_sumaimp);
				editDtoP100->setText( dtop100 );
				actTotales();
			}
		}
	}
	if( sender == editEntrega )
		actTotales();
/*>>>>>FRMEDITFACTURAVENTA_CABECERA_VALIDATE*/
    if( sender == editFecha )
        if( editFechaIVA->toDate().isNull() )
            editFechaIVA->setText( editFecha->toDate() );
    if( sender == editFechaIVA || sender == editFecha || !sender ) {
        if( editFechaIVA->toDate().isNull() ) {
            validresult->addError(_("La fecha del IVA no puede estar vacía."),
                                  "FECHAIVA" );
        } else {
            Date primerodetrimestre = Date( editFecha->toDate().getYear(),
                                            ((editFecha->toDate().getQuarter()-1) * 3) + 1, 1 );
            Date primerodetrimestresiguiente = (primerodetrimestre + 93).firstDayOfMonth();
            if( editFechaIVA->toDate() < primerodetrimestre ) {
                validresult->addError(_("La fecha del IVA no puede ser de un ejercicio o trimestre anterior a la fecha de la factura"),
                                      "FECHAIVA" );
                *isvalid = false;
            } else if( editFechaIVA->toDate() >= primerodetrimestresiguiente ) {
                validresult->addError(_("La fecha del IVA es de un ejercicio o trimestre posterior a la fecha de la factura."),
                                      "FECHAIVA" );
            }
        }
    }
    if( sender == editContador || (sender == editAgenteCodigo && editAgenteCodigo->isJustEdited()) ) {
        editNumero->setText( RecTipoDoc::formatNumDocumento(
                                 empresa::ModuleInstance->getRecEmpresa()->getValue( "CODIGO" ).toInt(),
                                 empresa::ModuleInstance->getEjercicio(),
                                 editContador->toInt(), editAgenteCodigo->toInt(),
                                 getRecTipoDoc()->getValue("FORMATO").toString()) );
    }
    if( !sender || sender == editContador ) {
        if( !validCodeAndDesc( ( sender == editContador ? editNumero : sender ),
                               *validresult, editNumero, 0, "NUMERO", "") )
            if( !sender )
                *isvalid = false;
    }
    if( mHasCobros && mOldTotal != editTotal->toMoney() ) {
        validresult->addError( Xtring::printf( _("No puedes modificar el total de %s porque tiene cobros ya cobrados"),
                                               DBAPP->getTableDescSingular( pRecord->getTableName(), "esta" ).c_str() ), "TIPODOC_ID" );
        *isvalid = false;
    }
    if ( !ir ) {
        showValidMessages( isvalid, *validresult, sender );
        delete validresult;
    }
}

/*<<<<<FRMEDITFACTURAVENTA_CABECERA_UPDATEFROMDETAILS*/
void FrmEditFacturaVenta::updateFromDetails( class FrmEditRecDetail *detailfrm )
{
	actTotales();
	numeraLineas();
/*>>>>>FRMEDITFACTURAVENTA_CABECERA_UPDATEFROMDETAILS*/
}

/*<<<<<FRMEDITFACTURAVENTA_CABECERA_NUMERALINEAS*/
void FrmEditFacturaVenta::numeraLineas()
{
	dbRecordList *reclst = getRecFacturaVenta()->getListFacturaVentaDet();
	for ( unsigned int i = 0; i < reclst->size(); i++ ) {
		RecFacturaVentaDet *detalle = static_cast<RecFacturaVentaDet *>( reclst->at( i ) );
		if( !detalle->isEmpty() ) // No numerar detalles vacíos 
			detalle->setValue( "NLINEA", i+1 );
	}
/*>>>>>FRMEDITFACTURAVENTA_CABECERA_NUMERALINEAS*/
}

bool FrmEditFacturaVenta::canBeginEdit(EditMode newmode)
{
    mHasCobros = false;
    mOldTotal = getRecFacturaVenta()->getValue( "TOTAL" ).toMoney();
    if( newmode == DataTable::updating || newmode == DataTable::deleting ) {
        if( getRecFacturaVenta()->hasCobros( -1, pagos::PagosModule::ReciboPendiente ) ) {
            mHasCobros = true;
            if( newmode == DataTable::updating ) {
                return FrmBase::msgYesNo(this,
                                         Xtring::printf( _("%s tiene cobros ya cobrados. No podrás modificar algunos datos.\n¿Quieres continuar?"),
                                                 DBAPP->getTableDescSingular( pRecord->getTableName(), "Esta").c_str() ) );
            } else { // deleting
                FrmBase::msgError(this,
                                  Xtring::printf( _("%s tiene cobros ya cobrados. Tines que anular todos sus cobros antes de borrar."),
                                                  DBAPP->getTableDescSingular( pRecord->getTableName(), "Esta").c_str() ) );
                return false;
            }
        }
    }
    return FrmEditRecMaster::canBeginEdit(newmode);
}

void FrmEditFacturaVenta::slotCobrar()
{
    if( editEntrega->toDouble() != 0 ) {
        editResto->setText( editResto->toMoney() + editEntrega->toMoney() );
        editEntrega->setText( 0.0 );
        editFechaPago->setText( "" );
        editDocumentoPago->setText( "" );
#ifdef HAVE_CONTABMODULE
        getRecCuentaPago()->clear( false );
        scatterCuentaPago();
#endif
        pushCobrar->setText( _("&Entrega") );
    } else {
        Date fechapago = editFechaPago->toDate();
        if( fechapago.isNull() )
            fechapago = editFecha->toDate();
        Xtring docpago = _("Cobro alb. " + editNumero->toString() );
        Money pago = editEntrega->toMoney();
        if( pago == 0.0 )
            pago = editResto->toMoney();
#ifdef HAVE_CONTABMODULE
        Xtring cuentapago = editCuentaPagoCuenta->toString();
        bool has_contab = contab::ModuleInstance->isContabActive();
#else
        bool has_contab = false;
        Xtring cuentapago;
#endif
        if( cuentapago.isEmpty() )
            cuentapago = empresa::ModuleInstance->getRecEmpresa()->getValue("CUENTACAJA").toString();
        FrmPagar *pr = new FrmPagar( has_contab, pago, fechapago,
                                     cuentapago, docpago, 0, "cobrarFacturaVenta" );
        pr->showModalFor( this, true, true );
        if( !pr->wasCancelled() ) {
            editEntrega->setText( pr->getImporte() );
            editResto->setText( editResto->toMoney() - pr->getImporte() );
            editFechaPago->setText( pr->getFechaPago() );
            editDocumentoPago->setText( pr->getDocumentoPago() );
#ifdef HAVE_CONTABMODULE
            if( has_contab ) {
                contab::Cuenta cuenta_pago = contab::Cuenta( pr->getCuentaPago(), contab::ModuleInstance->getDigitosTrabajo() );
                cuenta_pago.expandir();
                editCuentaPagoCuenta->setText( cuenta_pago );
            }
#endif
            if( editTotal->toDouble() != 0.0 )
                pushCobrar->setText( _("&Borrar entrega") );
        }
        delete pr;
    }
}

/*<<<<<FRMEDITFACTURAVENTA_CABECERA_ACTTOTALES*/
void FrmEditFacturaVenta::actTotales()
{
	getRecFacturaVenta()->gather( this );
	getRecFacturaVenta()->actTotales();
	getRecFacturaVenta()->scatter( this );
/*>>>>>FRMEDITFACTURAVENTA_CABECERA_ACTTOTALES*/
}

/*<<<<<FRMEDITFACTURAVENTA_FIN*/
} // namespace factu
} // namespace gong
/*>>>>>FRMEDITFACTURAVENTA_FIN*/

