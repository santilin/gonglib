/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de albaranes de venta
// FIELD Fecha date - cabecera
// FIELD TipoDoc_ID Reference(TipoDoc,Codigo,Nombre) - cabecera
// FIELD Contador int - cabecera
// FIELD Numero string - cabecera
// FIELD Facturado bool - cabecera
// FIELD Cliente_ID Reference(Cliente,Codigo,RazonSocial,dbRecord::InsertIfNotFound) - cabecera2
// FIELD IVADetallado comboint - cabecera2
// FIELD FormaPago_ID Reference(pagos::FormaPago,Codigo,Nombre,dbRecord::InsertIfNotFound) - cabecera2
// FIELD Agente_ID Reference(Agente,Codigo,RazonSocial,dbRecord::InsertIfNotFound) - cabecera3
// FIELD AlbaranVentaDet FrmEditRecDetail
// FIELD NoFacturable bool - noaddrightNoFacturable
// FIELD SumaImportes money - noaddrightSumaImportes
// FIELD DtoP100 double - noaddrightDtoP100
// FIELD Descuento money - noaddrightDescuento
// FIELD BaseImponible money - noaddrightBaseImponible
// FIELD RecargoEquivalencia money - noaddrightRecargoEquivalencia
// FIELD IVA money - noaddrightIVA
// FIELD Total money - noaddrightTotal
// FIELD Cobros money - noaddrightCobros
// FIELD Entrega money - noaddrightEntrega
// FIELD Resto money - noaddrightResto
// FIELD Proyecto_ID Reference(empresa::Proyecto,Codigo,Nombre) tabPagos proyecto if(empresa::ModuleInstance->usaProyectos())
// FIELD DesgloseIVA string tabPagos desgloseiva
// FIELD CuentaPago_ID Reference(CuentaTesoreria,Codigo,Nombre) tabPagos pago MODULE_INCLUDED(Tesoreria)
// FIELD CuentaPago_ID Reference(Cuenta,Cuenta,Descripcion) tabPagos pago MODULE_INCLUDED(Contab)
// FIELD DocumentoPago string tabPagos pago
// FIELD FechaPago date tabPagos pago
// FIELD Notas text tabPagos notas
// TYPE FrmEditRecMaster factu::AlbaranVenta Albaran Venta
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITALBARANVENTA_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "factufrmeditalbaranventa.h"
/*>>>>>FRMEDITALBARANVENTA_INCLUDES*/
#include "factumodule.h"
#include "factufrmpagar.h"
#include "factufldivadetallado.h"

namespace gong {
namespace factu {

/*<<<<<FRMEDITALBARANVENTA_CONSTRUCTOR*/
FrmEditAlbaranVenta::FrmEditAlbaranVenta(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditAlbaranVenta" );
/*>>>>>FRMEDITALBARANVENTA_CONSTRUCTOR*/
    /*<<<<<FRMEDITALBARANVENTA_INIT_CONTROLS*/
	QHBoxLayout *cabeceraLayout = new QHBoxLayout(0, 0, 6, "cabeceraLayout");
	QHBoxLayout *cabecera2Layout = new QHBoxLayout(0, 0, 6, "cabecera2Layout");
	QHBoxLayout *cabecera3Layout = new QHBoxLayout(0, 0, 6, "cabecera3Layout");
	QHBoxLayout *albaranventadetLayout = new QHBoxLayout(0, 0, 6, "albaranventadetLayout");
	QHBoxLayout *rightNoFacturableLayout = new QHBoxLayout(0, 0, 6, "rightNoFacturableLayout");
	QHBoxLayout *rightSumaImportesLayout = new QHBoxLayout(0, 0, 6, "rightSumaImportesLayout");
	QHBoxLayout *rightDtoP100Layout = new QHBoxLayout(0, 0, 6, "rightDtoP100Layout");
	QHBoxLayout *rightDescuentoLayout = new QHBoxLayout(0, 0, 6, "rightDescuentoLayout");
	QHBoxLayout *rightBaseImponibleLayout = new QHBoxLayout(0, 0, 6, "rightBaseImponibleLayout");
	QHBoxLayout *rightRecargoEquivalenciaLayout = new QHBoxLayout(0, 0, 6, "rightRecargoEquivalenciaLayout");
	QHBoxLayout *rightIVALayout = new QHBoxLayout(0, 0, 6, "rightIVALayout");
	QHBoxLayout *rightTotalLayout = new QHBoxLayout(0, 0, 6, "rightTotalLayout");
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
	editFecha = addEditField( pControlsFrame, "ALBARANVENTA", "FECHA", cabeceraLayout );

	searchTipoDocCodigo = addSearchField( pControlsFrame, "TIPODOC_ID", "TIPODOC", "CODIGO", "NOMBRE", cabeceraLayout );
	pushTipoDocCodigo = searchTipoDocCodigo->getButton();
	connect( pushTipoDocCodigo, SIGNAL( clicked() ), this, SLOT( pushTipoDocCodigo_clicked() ) );
	editTipoDocCodigo = searchTipoDocCodigo->getEditCode();
	editTipoDocNombre = searchTipoDocCodigo->getEditDesc();
	editContador = addEditField( pControlsFrame, "ALBARANVENTA", "CONTADOR", cabeceraLayout );
	editNumero = addEditField( pControlsFrame, "ALBARANVENTA", "NUMERO", cabeceraLayout );
	checkFacturado = addCheckField( pControlsFrame, "ALBARANVENTA", "FACTURADO", cabeceraLayout );

	searchClienteCodigo = addSearchField( pControlsFrame, "CLIENTE_ID", "CLIENTE", "CODIGO", "RAZONSOCIAL", cabecera2Layout );
	pushClienteCodigo = searchClienteCodigo->getButton();
	connect( pushClienteCodigo, SIGNAL( clicked() ), this, SLOT( pushClienteCodigo_clicked() ) );
	editClienteCodigo = searchClienteCodigo->getEditCode();
	editClienteRazonSocial = searchClienteCodigo->getEditDesc();
	comboIVADetallado = addComboIntField( pControlsFrame, "ALBARANVENTA", "IVADETALLADO", cabecera2Layout );

	searchFormaPagoCodigo = addSearchField( pControlsFrame, "FORMAPAGO_ID", "FORMAPAGO", "CODIGO", "NOMBRE", cabecera2Layout );
	pushFormaPagoCodigo = searchFormaPagoCodigo->getButton();
	connect( pushFormaPagoCodigo, SIGNAL( clicked() ), this, SLOT( pushFormaPagoCodigo_clicked() ) );
	editFormaPagoCodigo = searchFormaPagoCodigo->getEditCode();
	editFormaPagoNombre = searchFormaPagoCodigo->getEditDesc();

	searchAgenteCodigo = addSearchField( pControlsFrame, "AGENTE_ID", "AGENTE", "CODIGO", "RAZONSOCIAL", cabecera3Layout );
	pushAgenteCodigo = searchAgenteCodigo->getButton();
	connect( pushAgenteCodigo, SIGNAL( clicked() ), this, SLOT( pushAgenteCodigo_clicked() ) );
	editAgenteCodigo = searchAgenteCodigo->getEditCode();
	editAgenteRazonSocial = searchAgenteCodigo->getEditDesc();

// frmDetails: AlbaranVentaDet
	QFrame *albaranventadetFrame = new QFrame(this);
	albaranventadetFrame->setFrameStyle(QFrame::Sunken | QFrame::Panel);
	albaranventadetFrame->setLineWidth(4);
	QVBoxLayout *albaranventadetFrameLayout = new QVBoxLayout(albaranventadetFrame);
	RecAlbaranVentaDet *albaranventadet = getRecAlbaranVenta()->getRecAlbaranVentaDet(-1);
	dbViewDefinitionDict albaranventadetviews;
	DBAPP->getDatabase()->getViewsForTable( albaranventadet, albaranventadetviews );
	dbRecordListDataModel *albaranventadetdm =
		new dbRecordListDataModel(getRecAlbaranVenta()->getListAlbaranVentaDet(), albaranventadet, albaranventadetviews, "1=0");
	pFrmAlbaranVentaDet = static_cast<FrmEditAlbaranVentaDet *>(
		DBAPP->createEditDetailForm(this,
		-1, albaranventadet, "ALBARANVENTADET",albaranventadetdm, DataTable::browsing,
		static_cast<dbApplication::EditFlags>(dbApplication::embedded + dbApplication::tableEditEvent ),
		pControlsFrame) );
	pFrmAlbaranVentaDet->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding);
	albaranventadetFrameLayout->addWidget( pFrmAlbaranVentaDet );
	albaranventadetLayout->addWidget(albaranventadetFrame);
	checkNoFacturable = addCheckField( pControlsFrame, "ALBARANVENTA", "NOFACTURABLE", rightNoFacturableLayout );
	editSumaImportes = addEditField( pControlsFrame, "ALBARANVENTA", "SUMAIMPORTES", rightSumaImportesLayout );
	editDtoP100 = addEditField( pControlsFrame, "ALBARANVENTA", "DTOP100", rightDtoP100Layout );
	editDescuento = addEditField( pControlsFrame, "ALBARANVENTA", "DESCUENTO", rightDescuentoLayout );
	editBaseImponible = addEditField( pControlsFrame, "ALBARANVENTA", "BASEIMPONIBLE", rightBaseImponibleLayout );
	editRecargoEquivalencia = addEditField( pControlsFrame, "ALBARANVENTA", "RECARGOEQUIVALENCIA", rightRecargoEquivalenciaLayout );
	editIVA = addEditField( pControlsFrame, "ALBARANVENTA", "IVA", rightIVALayout );
	editTotal = addEditField( pControlsFrame, "ALBARANVENTA", "TOTAL", rightTotalLayout );
	editCobros = addEditField( pControlsFrame, "ALBARANVENTA", "COBROS", rightCobrosLayout );
	editEntrega = addEditField( pControlsFrame, "ALBARANVENTA", "ENTREGA", rightEntregaLayout );
	editResto = addEditField( pControlsFrame, "ALBARANVENTA", "RESTO", rightRestoLayout );

if(empresa::ModuleInstance->usaProyectos()){
	searchProyectoCodigo = addSearchField( tabPagos, "PROYECTO_ID", "PROYECTO", "CODIGO", "NOMBRE", proyectoLayout );
	pushProyectoCodigo = searchProyectoCodigo->getButton();
	connect( pushProyectoCodigo, SIGNAL( clicked() ), this, SLOT( pushProyectoCodigo_clicked() ) );
	editProyectoCodigo = searchProyectoCodigo->getEditCode();
	editProyectoNombre = searchProyectoCodigo->getEditDesc();
}
	editDesgloseIVA = addEditField( tabPagos, "ALBARANVENTA", "DESGLOSEIVA", desgloseivaLayout );
	editDocumentoPago = addEditField( tabPagos, "ALBARANVENTA", "DOCUMENTOPAGO", pagoLayout );
	editFechaPago = addEditField( tabPagos, "ALBARANVENTA", "FECHAPAGO", pagoLayout );

#ifdef HAVE_TESORERIAMODULE
if( ModuleInstance->getTesoreriaModule() ) {
	searchCuentaPagoCodigo = addSearchField( tabPagos, "CUENTAPAGO_ID", "CUENTATESORERIA", "CODIGO", "NOMBRE", pagoLayout );
	pushCuentaPagoCodigo = searchCuentaPagoCodigo->getButton();
	connect( pushCuentaPagoCodigo, SIGNAL( clicked() ), this, SLOT( pushCuentaPagoCodigo_clicked() ) );
	editCuentaPagoCodigo = searchCuentaPagoCodigo->getEditCode();
	editCuentaPagoNombre = searchCuentaPagoCodigo->getEditDesc();
}
#endif

#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	searchCuentaPagoCuenta = addSearchField( tabPagos, "CUENTAPAGO_ID", "CUENTA", "CUENTA", "DESCRIPCION", pagoLayout );
	pushCuentaPagoCuenta = searchCuentaPagoCuenta->getButton();
	connect( pushCuentaPagoCuenta, SIGNAL( clicked() ), this, SLOT( pushCuentaPagoCuenta_clicked() ) );
	editCuentaPagoCuenta = searchCuentaPagoCuenta->getEditCode();
	editCuentaPagoDescripcion = searchCuentaPagoCuenta->getEditDesc();
}
#endif
	editNotas = addTextField( tabPagos, "ALBARANVENTA", "NOTAS", notasLayout );
	pControlsLayout->addLayout( cabeceraLayout );
	pControlsLayout->addLayout( cabecera2Layout );
	pControlsLayout->addLayout( cabecera3Layout );
	pControlsLayout->addLayout( albaranventadetLayout );
	alignLayout( rightNoFacturableLayout, false);
	alignLayout( rightSumaImportesLayout, false);
	alignLayout( rightDtoP100Layout, false);
	alignLayout( rightDescuentoLayout, false);
	alignLayout( rightBaseImponibleLayout, false);
	alignLayout( rightRecargoEquivalenciaLayout, false);
	alignLayout( rightIVALayout, false);
	alignLayout( rightTotalLayout, false);
	alignLayout( rightCobrosLayout, false);
	alignLayout( rightEntregaLayout, false);
	alignLayout( rightRestoLayout, false);
	tabPagosLayout->addLayout( proyectoLayout );
	tabPagosLayout->addLayout( desgloseivaLayout );
	tabPagosLayout->addLayout( pagoLayout );
	tabPagosLayout->addLayout( notasLayout );
/*>>>>>FRMEDITALBARANVENTA_INIT_CONTROLS*/
    pTabWidget->insertTab( tabPagos, toGUI( _( "&Extra" ) ) );

    QGridLayout *gridlayout = new QGridLayout();
    gridlayout->addLayout( rightNoFacturableLayout, 0, 0 );
    gridlayout->addLayout( rightSumaImportesLayout, 0, 1 );
    gridlayout->addLayout( rightDtoP100Layout, 0, 2 );
    gridlayout->addLayout( rightDescuentoLayout, 0, 3 );
    gridlayout->addLayout( rightBaseImponibleLayout, 1, 0 );
    gridlayout->addLayout( rightIVALayout, 1, 1 );
    gridlayout->addLayout( rightRecargoEquivalenciaLayout, 1, 2 );
    gridlayout->addLayout( rightTotalLayout, 1, 3 );
    gridlayout->addLayout( rightCobrosLayout, 2, 1 );
    gridlayout->addLayout( rightEntregaLayout, 2, 2 );
    gridlayout->addLayout( rightRestoLayout, 2, 3 );
    pControlsLayout->addLayout( gridlayout );

    pushCobrar = new QPushButton( this );
    connect( pushCobrar, SIGNAL( clicked() ), this, SLOT( slotCobrar() ) );
    gridlayout->addWidget( pushCobrar, 2, 0 );

    editNumero->setMustBeReadOnly( true );
    checkFacturado->setMustBeReadOnly( true );
    editSumaImportes->setMustBeReadOnly( true );
    editBaseImponible->setMustBeReadOnly( true );
    editRecargoEquivalencia->setMustBeReadOnly( true );
    editIVA->setMustBeReadOnly( true );
    editCobros->setMustBeReadOnly( true );
    editResto->setMustBeReadOnly( true );
    editDescuento->setMustBeReadOnly( true );
    editClienteCodigo->setWidthInChars(8);
    editAgenteCodigo->setWidthInChars(8);
    editFormaPagoCodigo->setWidthInChars(8);
    editNotas->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Minimum);
    mHasCobros = false;
}

void FrmEditAlbaranVenta::scatterFields()
{
    if( isDuplicating() && mIsFirstScatter ) {
        getRecAlbaranVenta()->setValue( "NUMERO", "" );
        getRecAlbaranVenta()->setValue( "CONTADOR", 0 );
        getRecAlbaranVenta()->setValue( "FACTURADO", false );
        getRecAlbaranVenta()->setValue( "ENTREGA", Money(0.0) );
        getRecAlbaranVenta()->setValue( "COBROS", Money(0.0) );
        getRecAlbaranVenta()->setValue( "RESTO", getRecAlbaranVenta()->getValue( "TOTAL") );
        getRecAlbaranVenta()->setValue( "FECHAPAGO", Date() );
        getRecAlbaranVenta()->setValue( "DOCUMENTOPAGO", Xtring::null );
#ifdef HAVE_CONTABMODULE
        getRecAlbaranVenta()->setValue( "ASIENTO_ID", 0 );
        getRecCuentaPago()->clear( false );
#elif defined(HAVE_TESORERIAMODULE)
        getRecAlbaranVenta()->setValue( "APUNTE_ID", 0 );
        getRecCuentaPago()->clear( false );
#endif
    }
    /*<<<<<FRMEDITALBARANVENTA_SCATTER*/
	editFecha->setText(getRecAlbaranVenta()->getValue("FECHA").toDate());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editFecha;
	editContador->setText(getRecAlbaranVenta()->getValue("CONTADOR").toInt());
	editNumero->setText(getRecAlbaranVenta()->getValue("NUMERO").toString());
	checkFacturado->setChecked(getRecAlbaranVenta()->getValue("FACTURADO").toBool());
	comboIVADetallado->setCurrentItemByValue(getRecAlbaranVenta()->getValue("IVADETALLADO").toInt());
	checkNoFacturable->setChecked(getRecAlbaranVenta()->getValue("NOFACTURABLE").toBool());
	editSumaImportes->setText(getRecAlbaranVenta()->getValue("SUMAIMPORTES").toMoney());
	editDtoP100->setText(getRecAlbaranVenta()->getValue("DTOP100").toDouble());
	editDescuento->setText(getRecAlbaranVenta()->getValue("DESCUENTO").toMoney());
	editBaseImponible->setText(getRecAlbaranVenta()->getValue("BASEIMPONIBLE").toMoney());
	editRecargoEquivalencia->setText(getRecAlbaranVenta()->getValue("RECARGOEQUIVALENCIA").toMoney());
	editIVA->setText(getRecAlbaranVenta()->getValue("IVA").toMoney());
	editTotal->setText(getRecAlbaranVenta()->getValue("TOTAL").toMoney());
	editCobros->setText(getRecAlbaranVenta()->getValue("COBROS").toMoney());
	editEntrega->setText(getRecAlbaranVenta()->getValue("ENTREGA").toMoney());
	editResto->setText(getRecAlbaranVenta()->getValue("RESTO").toMoney());
	editDesgloseIVA->setText(getRecAlbaranVenta()->getValue("DESGLOSEIVA").toString());
	editDocumentoPago->setText(getRecAlbaranVenta()->getValue("DOCUMENTOPAGO").toString());
	editFechaPago->setText(getRecAlbaranVenta()->getValue("FECHAPAGO").toDate());
	editNotas->setText(getRecAlbaranVenta()->getValue("NOTAS").toString());
	scatterTipoDoc();
	scatterCliente();
	scatterFormaPago();
	scatterAgente();
if(empresa::ModuleInstance->usaProyectos()){
	scatterProyecto();
}
#ifdef HAVE_TESORERIAMODULE
#ifdef HAVE_TESORERIAMODULE
if( ModuleInstance->getTesoreriaModule() ) {
	scatterCuentaPago();
}
#endif
#endif
#ifdef HAVE_CONTABMODULE
#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	scatterCuentaPago();
}
#endif
#endif
/*>>>>>FRMEDITALBARANVENTA_SCATTER*/
	if( mIsFirstScatter ) {
		if( isInserting() ) {
			if( !isDuplicating() && editFecha->toDate().isNull() )
				editFecha->setText( ModuleInstance->getWorkingDate() );
			if( isDuplicating() ) {
				editTipoDocCodigo->setJustEdited( true );
				validateFields( editTipoDocCodigo, 0 );
			}
		} else if( isUpdating() ) {
			pFocusWidget = pFrmAlbaranVentaDet;
		}
		pFrmAlbaranVentaDet->addDetailIfNeeded();
		// Si estamos duplicando o copiando desde cualquier otro documento y no tenemos contador, generarlo, pero si es una 
		// inserción normal, no generarlo hasta que se meta el tipo de documento
		if( isInserting() && editContador->toInt() == 0 && editTipoDocCodigo->toInt() != 0 && isFirstScatter() ) 
			genNumeroDocumento();
		searchClienteCodigo->setMustBeReadOnly( mHasCobros );
		searchFormaPagoCodigo->setMustBeReadOnly( mHasCobros );
		editFecha->setMustBeReadOnly( mHasCobros );
		editDtoP100->setMustBeReadOnly( mHasCobros );
		editTotal->setMustBeReadOnly( mHasCobros );
		editEntrega->setMustBeReadOnly( mHasCobros );
		pushCobrar->setVisible( !mHasCobros );
		scatterFormaPago(); // Para cambiar el texto del botón pagar después de actualizar los totales
		validateFields( comboIVADetallado, 0 ); // Para mostrar u ocultar el recargo de equivalencia
	}
}

void FrmEditAlbaranVenta::gatherFields()
{
    /*<<<<<FRMEDITALBARANVENTA_GATHER*/
	getRecAlbaranVenta()->setValue( "FECHA", editFecha->toDate());
	getRecAlbaranVenta()->setValue( "TIPODOC_ID", getRecTipoDoc()->getRecordID() );
	getRecAlbaranVenta()->setValue( "CONTADOR", editContador->toInt());
	getRecAlbaranVenta()->setValue( "NUMERO", editNumero->toString());
	getRecAlbaranVenta()->setValue( "FACTURADO", checkFacturado->isChecked());
	getRecAlbaranVenta()->setValue( "CLIENTE_ID", getRecCliente()->getRecordID() );
	getRecAlbaranVenta()->setValue( "IVADETALLADO", comboIVADetallado->getCurrentItemValue());
	getRecAlbaranVenta()->setValue( "FORMAPAGO_ID", getRecFormaPago()->getRecordID() );
	getRecAlbaranVenta()->setValue( "AGENTE_ID", getRecAgente()->getRecordID() );
	getRecAlbaranVenta()->setValue( "NOFACTURABLE", checkNoFacturable->isChecked());
	getRecAlbaranVenta()->setValue( "SUMAIMPORTES", editSumaImportes->toMoney());
	getRecAlbaranVenta()->setValue( "DTOP100", editDtoP100->toDouble());
	getRecAlbaranVenta()->setValue( "DESCUENTO", editDescuento->toMoney());
	getRecAlbaranVenta()->setValue( "BASEIMPONIBLE", editBaseImponible->toMoney());
	getRecAlbaranVenta()->setValue( "RECARGOEQUIVALENCIA", editRecargoEquivalencia->toMoney());
	getRecAlbaranVenta()->setValue( "IVA", editIVA->toMoney());
	getRecAlbaranVenta()->setValue( "TOTAL", editTotal->toMoney());
	getRecAlbaranVenta()->setValue( "COBROS", editCobros->toMoney());
	getRecAlbaranVenta()->setValue( "ENTREGA", editEntrega->toMoney());
	getRecAlbaranVenta()->setValue( "RESTO", editResto->toMoney());
if(empresa::ModuleInstance->usaProyectos()){
	getRecAlbaranVenta()->setValue( "PROYECTO_ID", getRecProyecto()->getRecordID() );
}
	getRecAlbaranVenta()->setValue( "DESGLOSEIVA", editDesgloseIVA->toString());
	getRecAlbaranVenta()->setValue( "DOCUMENTOPAGO", editDocumentoPago->toString());
	getRecAlbaranVenta()->setValue( "FECHAPAGO", editFechaPago->toDate());
#ifdef HAVE_TESORERIAMODULE
if( ModuleInstance->getTesoreriaModule() ) {
	getRecAlbaranVenta()->setValue( "CUENTAPAGO_ID", getRecCuentaPago()->getRecordID() );
}
#endif
#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	getRecAlbaranVenta()->setValue( "CUENTAPAGO_ID", getRecCuentaPago()->getRecordID() );
}
#endif
	getRecAlbaranVenta()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITALBARANVENTA_GATHER*/
    ModuleInstance->setWorkingDate( editFecha->toDate() );
}

/*<<<<<FRMEDITALBARANVENTA_CABECERA_GENNUMDOC*/
void FrmEditAlbaranVenta::genNumeroDocumento()
{
		editContador->setText( ModuleInstance->getMaxContador( "ALBARANVENTA",
			getRecord()->getValue("EMPRESA_ID").toInt(),
			ModuleInstance->getEmpresaModule()->getEjercicio(),
			getRecTipoDoc()->getValue("SERIE").toString() ) );
		editContador->setJustEdited( true );
		validateFields( editContador, 0 );
/*>>>>>FRMEDITALBARANVENTA_CABECERA_GENNUMDOC*/
}

void FrmEditAlbaranVenta::scatterTipoDoc()
{
    /*<<<<<FRMEDITALBARANVENTA_SCATTER_TIPODOC*/
	editTipoDocCodigo->setText( getRecTipoDoc()->getValue("CODIGO") );
	editTipoDocNombre->setText( getRecTipoDoc()->getValue("NOMBRE") );
/*>>>>>FRMEDITALBARANVENTA_SCATTER_TIPODOC*/
    if( editTipoDocCodigo->isJustEdited() ) {
        if( editAgenteCodigo->toString().isEmpty() )
            searchAgenteCodigo->setValue( getRecTipoDoc()->getRecAgente()->getValue("CODIGO") );
        checkNoFacturable->setChecked( getRecTipoDoc()->getValue("NOFACTURABLE").toBool() );
        if( getRecTipoDoc()->getValue("IVADetallado").toInt() != 0 )
            comboIVADetallado->setCurrentItemByValue( getRecTipoDoc()->getValue("IVADetallado").toInt() );
        if( editFormaPagoCodigo->toString().isEmpty() )
            searchFormaPagoCodigo->setValue( getRecTipoDoc()->getValue("FORMAPAGO.CODIGO").toInt() );
        genNumeroDocumento();
    }
}

void FrmEditAlbaranVenta::pushTipoDocCodigo_clicked()
{
    /*<<<<<FRMEDITALBARANVENTA_PUSH_TIPODOC_CODIGO_CLICKED*/
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
					DBAPP->createClient( DBAPP->createEditForm(this, getRecTipoDoc(),
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
/*>>>>>FRMEDITALBARANVENTA_PUSH_TIPODOC_CODIGO_CLICKED*/
}

void FrmEditAlbaranVenta::scatterCliente()
{
    /*<<<<<FRMEDITALBARANVENTA_SCATTER_CLIENTE*/
	editClienteCodigo->setText( getRecCliente()->getValue("CODIGO") );
	editClienteRazonSocial->setText( getRecCliente()->getValue("RAZONSOCIAL") );
/*>>>>>FRMEDITALBARANVENTA_SCATTER_CLIENTE*/
    /*<<<<<FRMEDITALBARANVENTA_CABECERA_SCATTER_CLIENTE*/
	if( editClienteCodigo->isJustEdited() ) {
		if( getRecCliente()->getRecFormaPago()->getValue("CODIGO").toInt() != 0 )
			searchFormaPagoCodigo->setValue( getRecCliente()->getRecFormaPago()->getValue("CODIGO") );
		if( getRecCliente()->getValue("IVADetallado").toInt() != 0 )
			comboIVADetallado->setCurrentItemByValue( getRecCliente()->getValue("IVADetallado").toInt() );
		if( getRecCliente()->getRecAgente()->getRecordID() != 0 )
			searchAgenteCodigo->setValue( getRecCliente()->getRecAgente()->getValue("CODIGO") );
	}
/*>>>>>FRMEDITALBARANVENTA_CABECERA_SCATTER_CLIENTE*/
}

void FrmEditAlbaranVenta::pushClienteCodigo_clicked()
{
    /*<<<<<FRMEDITALBARANVENTA_PUSH_CLIENTE_CODIGO_CLICKED*/
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
					DBAPP->createClient( DBAPP->createEditForm(this, getRecCliente(),
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
/*>>>>>FRMEDITALBARANVENTA_PUSH_CLIENTE_CODIGO_CLICKED*/
}

void FrmEditAlbaranVenta::specialControlKeyPressed(QWidget *sender, char key)
{
    /*<<<<<FRMEDITALBARANVENTA_SPECIALACTION*/
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
#ifdef HAVE_TESORERIAMODULE
if( ModuleInstance->getTesoreriaModule() ) {
	if( sender == editCuentaPagoCodigo )
		pushCuentaPagoCodigo_clicked();
}
#endif
#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	if( sender == editCuentaPagoCuenta )
		pushCuentaPagoCuenta_clicked();
}
#endif
	mControlKeyPressed = '\0';
/*>>>>>FRMEDITALBARANVENTA_SPECIALACTION*/
}

void FrmEditAlbaranVenta::scatterAgente()
{
    /*<<<<<FRMEDITALBARANVENTA_SCATTER_AGENTE*/
	editAgenteCodigo->setText( getRecAgente()->getValue("CODIGO") );
	editAgenteRazonSocial->setText( getRecAgente()->getValue("RAZONSOCIAL") );
/*>>>>>FRMEDITALBARANVENTA_SCATTER_AGENTE*/
}

void FrmEditAlbaranVenta::pushAgenteCodigo_clicked()
{
    /*<<<<<FRMEDITALBARANVENTA_PUSH_AGENTE_CODIGO_CLICKED*/
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
					DBAPP->createClient( DBAPP->createEditForm(this, getRecAgente(),
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
/*>>>>>FRMEDITALBARANVENTA_PUSH_AGENTE_CODIGO_CLICKED*/
}

#ifdef HAVE_CONTABMODULE
void FrmEditAlbaranVenta::scatterCuentaPago()
{
/*<<<<<FRMEDITALBARANVENTA_SCATTER_CUENTAPAGO*/
	editCuentaPagoCuenta->setText( getRecCuentaPago()->getValue("CUENTA") );
	editCuentaPagoDescripcion->setText( getRecCuentaPago()->getValue("DESCRIPCION") );
/*>>>>>FRMEDITALBARANVENTA_SCATTER_CUENTAPAGO*/
}
#elif defined (HAVE_TESORERIAMODULE)
void FrmEditAlbaranVenta::scatterCuentaPago()
{
	editCuentaPagoCodigo->setText( getRecCuentaPago()->getValue("CODIGO") );
	editCuentaPagoNombre->setText( getRecCuentaPago()->getValue("NOMBRE") );
}
#endif

void FrmEditAlbaranVenta::pushCuentaPagoCuenta_clicked()
{
#ifdef HAVE_CONTABMODULE
    /*<<<<<FRMEDITALBARANVENTA_PUSH_CUENTAPAGO_CUENTA_CLICKED*/
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
					DBAPP->createClient( DBAPP->createEditForm(this, getRecCuentaPago(),
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
/*>>>>>FRMEDITALBARANVENTA_PUSH_CUENTAPAGO_CUENTA_CLICKED*/
#endif
}

void FrmEditAlbaranVenta::scatterProyecto()
{
    /*<<<<<FRMEDITALBARANVENTA_SCATTER_PROYECTO*/
	editProyectoCodigo->setText( getRecProyecto()->getValue("CODIGO") );
	editProyectoNombre->setText( getRecProyecto()->getValue("NOMBRE") );
/*>>>>>FRMEDITALBARANVENTA_SCATTER_PROYECTO*/
}

void FrmEditAlbaranVenta::pushProyectoCodigo_clicked()
{
    /*<<<<<FRMEDITALBARANVENTA_PUSH_PROYECTO_CODIGO_CLICKED*/
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
					DBAPP->createClient( DBAPP->createEditForm(this, getRecProyecto(),
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
/*>>>>>FRMEDITALBARANVENTA_PUSH_PROYECTO_CODIGO_CLICKED*/
}


void FrmEditAlbaranVenta::scatterFormaPago()
{
    /*<<<<<FRMEDITALBARANVENTA_SCATTER_FORMAPAGO*/
	editFormaPagoCodigo->setText( getRecFormaPago()->getValue("CODIGO") );
	editFormaPagoNombre->setText( getRecFormaPago()->getValue("NOMBRE") );
/*>>>>>FRMEDITALBARANVENTA_SCATTER_FORMAPAGO*/
	if( editFormaPagoCodigo->isJustEdited() ) {
#ifdef HAVE_CONTABMODULE
		searchCuentaPagoCuenta->setValue(getRecFormaPago()->getRecCuentaPago()->getValue("CODIGO").toString());
#elif defined( HAVE_TESORERIAMODULE )
		searchCuentaPagoCodigo->setValue(getRecFormaPago()->getValue("CUENTATESORERIA.CODIGO").toString());
#endif		
	}
    if( getRecFormaPago()->getValue( "TIPOFORMAPAGO" ).toInt() == pagos::RecFormaPago::Contado
            || getRecFormaPago()->getValue( "TIPOFORMAPAGO" ).toInt() == pagos::RecFormaPago::SeIgnora ) {
        pushCobrar->setVisible( false );
        editEntrega->setMustBeReadOnly( true );
    } else {
        pushCobrar->setVisible( true );
        editEntrega->setMustBeReadOnly( false );
    }
    actTotales();
    if( editEntrega->toDouble() != 0.0 && editTotal->toDouble() != 0.0 )
        pushCobrar->setText( _("&Borrar entrega") );
    else
        pushCobrar->setText( _("&Entrega") );
}

void FrmEditAlbaranVenta::pushFormaPagoCodigo_clicked()
{
    /*<<<<<FRMEDITALBARANVENTA_PUSH_FORMAPAGO_CODIGO_CLICKED*/
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
					DBAPP->createClient( DBAPP->createEditForm(this, getRecFormaPago(),
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
/*>>>>>FRMEDITALBARANVENTA_PUSH_FORMAPAGO_CODIGO_CLICKED*/
}

void FrmEditAlbaranVenta::pushCuentaPagoCodigo_clicked()
{
#ifdef HAVE_TESORERIAMODULE
/*<<<<<FRMEDITALBARANVENTA_PUSH_CUENTAPAGO_CODIGO_CLICKED*/
	char action = mControlKeyPressed;
	if( !isEditing() || searchCuentaPagoCodigo->mustBeReadOnly() )
		action = 'E';
	switch( action ) {
		case 'F':
		case '\0':
			editCuentaPagoCodigo->setJustEdited( false );
			editCuentaPagoCodigo->setCancelling();
			if( DBAPP->choose(this, getRecCuentaPago(), 0, dbApplication::editNone, this ) ) {
				setEdited(true);
				scatterCuentaPago();
				editCuentaPagoCodigo->setJustEdited( true );
				setWiseFocus(editCuentaPagoCodigo);
			}
			break;
		case 'M':
			{
				if( getRecCuentaPago()->getRecordID() ) {
					editCuentaPagoCodigo->setJustEdited( false );
					if( DBAPP->editRecord(this,
							getRecCuentaPago(), 0, DataTable::updating,
							dbApplication::simpleEdition, this ) ) {
						editCuentaPagoCodigo->setJustEdited( true );
						scatterCuentaPago();
					}
				setWiseFocus(editCuentaPagoCodigo);
				}
			}
			break;
		case 'E':
			{
				if( getRecCuentaPago()->getRecordID() != 0 ) {
					editCuentaPagoCodigo->setJustEdited( false );
					DBAPP->createClient( DBAPP->createEditForm(this, getRecCuentaPago(),
						0, DataTable::selecting, dbApplication::simpleEdition, this ) );
				}
			}
			break;
		case 'A':
			{
				RecCuentaPago *tmprec = static_cast<RecCuentaPago *>(DBAPP->createRecord( "CuentaPago" ));
				editCuentaPagoCodigo->setJustEdited( false );
				tmprec->clear( true ); // set default values
				DBAPP->setCodeNotFound( editCuentaPagoCodigo->toString() );
				if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
					dbApplication::simpleEdition, this ) ) {
					editCuentaPagoCodigo->setJustEdited( true );
					getRecCuentaPago()->copyRecord( tmprec );
					scatterCuentaPago();
				}
				setWiseFocus(editCuentaPagoCodigo);
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITALBARANVENTA_PUSH_CUENTAPAGO_CODIGO_CLICKED*/
#endif
}


void FrmEditAlbaranVenta::validateFields(QWidget *sender, bool *isvalid, ValidResult *ir)
{
    /*<<<<<FRMEDITALBARANVENTA_VALIDATE*/
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
		getRecCliente(), "CODIGO", "RAZONSOCIAL", Xtring::null, dbRecord::SeekCodeFlags( dbRecord::InsertIfNotFound )) )
		scatterCliente();
	if( focusWidget() != pushFormaPagoCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editFormaPagoCodigo, editFormaPagoNombre,
		getRecFormaPago(), "CODIGO", "NOMBRE", Xtring::null, dbRecord::SeekCodeFlags( dbRecord::InsertIfNotFound )) )
		scatterFormaPago();
	if( focusWidget() != pushAgenteCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editAgenteCodigo, editAgenteRazonSocial,
		getRecAgente(), "CODIGO", "RAZONSOCIAL", Xtring::null, dbRecord::SeekCodeFlags( dbRecord::InsertIfNotFound )) )
		scatterAgente();
if(empresa::ModuleInstance->usaProyectos()){
	if( focusWidget() != pushProyectoCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editProyectoCodigo, editProyectoNombre,
		getRecProyecto(), "CODIGO", "NOMBRE", Xtring::null) )
		scatterProyecto();
}
#ifdef HAVE_TESORERIAMODULE
if( ModuleInstance->getTesoreriaModule() ) {
	if( focusWidget() != pushCuentaPagoCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editCuentaPagoCodigo, editCuentaPagoNombre,
		getRecCuentaPago(), "CODIGO", "NOMBRE", Xtring::null) )
		scatterCuentaPago();
}
#endif
/*>>>>>FRMEDITALBARANVENTA_VALIDATE*/

#ifdef HAVE_CONTABMODULE
#define editCuentaCuenta editCuentaPagoCuenta
#define getRecCuenta getRecCuentaPago
#define scatterCuenta scatterCuentaPago
#include <contabvalidatecuenta.inc>
#endif

    if( !sender ) {
        if( editEntrega->toDouble() == 0.0 ) {
            editFechaPago->setText( Xtring::null );
            editDocumentoPago->setText( "" );
#if defined (HAVE_CONTABMODULE) 
            searchCuentaPagoCuenta->setValue( "" );
#elif defined (HAVE_TESORERIAMODULE)
            searchCuentaPagoCodigo->setValue( "" );
#endif
        } else {
			if( editFechaPago->toString().isEmpty()) {
                editFechaPago->setText( editFecha->toDate() );
			}
#if defined (HAVE_CONTABMODULE) 
            if( editCuentaPagoCuenta->toString().isEmpty() && getRecFormaPago()->getValue("SUBCUENTAPAGO").toString() == Xtring::null) 
#elif defined (HAVE_TESORERIAMODULE)
            if( editCuentaPagoCodigo->toString().isEmpty() && getRecFormaPago()->getValue("CUENTATESORERIA_ID").toInt() == 0) 
#endif
			{
				validresult->addError( "No se ha introducido una cuenta de pago y la forma de pago tampoco la tiene definida.\nNo se generará el apunte del cobro en tesorería.", "CUENTAPAGO_ID");
			}
		}
	}
	if( sender == editEntrega && editEntrega->isJustEdited() ) {	
        actTotales();
    }
/*<<<<<FRMEDITALBARANVENTA_CABECERA_VALIDATE*/
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
/*>>>>>FRMEDITALBARANVENTA_CABECERA_VALIDATE*/
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
    if (!ir) {
        showValidMessages(isvalid, *validresult, sender);
        delete validresult;
    }
}

/*<<<<<FRMEDITALBARANVENTA_CABECERA_UPDATEFROMDETAILS*/
void FrmEditAlbaranVenta::updateFromDetails( class FrmEditRecDetail *detailfrm )
{
	actTotales();
	numeraLineas();
/*>>>>>FRMEDITALBARANVENTA_CABECERA_UPDATEFROMDETAILS*/
}

/*<<<<<FRMEDITALBARANVENTA_CABECERA_NUMERALINEAS*/
void FrmEditAlbaranVenta::numeraLineas()
{
	dbRecordList *reclst = getRecAlbaranVenta()->getListAlbaranVentaDet();
	for ( unsigned int i = 0; i < reclst->size(); i++ ) {
		RecAlbaranVentaDet *detalle = static_cast<RecAlbaranVentaDet *>( reclst->at( i ) );
		if( !detalle->isEmpty() ) // No numerar detalles vacíos 
			detalle->setValue( "NLINEA", i+1 );
	}
/*>>>>>FRMEDITALBARANVENTA_CABECERA_NUMERALINEAS*/
}

/*<<<<<FRMEDITALBARANVENTA_CABECERA_ACTTOTALES*/
void FrmEditAlbaranVenta::actTotales()
{
	getRecAlbaranVenta()->gather( this );
	getRecAlbaranVenta()->actTotales();
	getRecAlbaranVenta()->scatter( this );
/*>>>>>FRMEDITALBARANVENTA_CABECERA_ACTTOTALES*/
}

bool FrmEditAlbaranVenta::canBeginEdit(FrmEditRec::EditMode newmode)
{
    if( getRecord()->getValue("FACTURADO").toBool()
            && ( newmode == DataTable::updating || newmode == DataTable::deleting ) ) {
        msgError(this, Xtring::printf( _("%s no se puede modificar ni borrar porque ya está facturado"),
                                       DBAPP->getTableDescSingular( pRecord->getTableName(), "Esta").c_str() ) );
        return false;
    }
    mHasCobros = false;
    mOldTotal = getRecAlbaranVenta()->getValue( "TOTAL" ).toMoney();
    if( newmode == DataTable::updating || newmode == DataTable::deleting ) {
        if( getRecAlbaranVenta()->hasCobros( -1, pagos::PagosModule::ReciboPendiente ) ) {
            mHasCobros = true;
            if( newmode == DataTable::updating ) {
                return FrmBase::msgYesNo(this,
                                         Xtring::printf( _("%s tiene cobros ya cobrados. No podrás modificar algunos datos.\n¿Quieres continuar?"),
                                                 DBAPP->getTableDescSingular( pRecord->getTableName(), "Esta").c_str() ) );
            } else { // deleting
                FrmBase::msgError(this,
                                  Xtring::printf( _("%s tiene cobros ya cobrados. Tienes que anular todos sus cobros antes de borrar."),
                                                  DBAPP->getTableDescSingular( pRecord->getTableName(), "Esta").c_str() ) );
                return false;
            }
        }
    }
    return FrmEditRecMaster::canBeginEdit(newmode);
}

void FrmEditAlbaranVenta::slotCobrar()
{
    if( editEntrega->toDouble() != 0.0 ) {
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
            pago = editTotal->toMoney();
#ifdef HAVE_CONTABMODULE
        Xtring cuentapago = editCuentaPagoCuenta->toString();
        bool has_contab = contab::ModuleInstance->isContabActive();
#elif defined (HAVE_TESORERIAMODULE)
        Xtring cuentapago = editCuentaPagoCodigo->toString();
        bool has_contab = factu::ModuleInstance->getTesoreriaModule();
#else
        bool has_contab = false;
        Xtring cuentapago;
#endif
        if( cuentapago.isEmpty() )
            cuentapago = empresa::ModuleInstance->getRecEmpresa()->getValue("CUENTACAJA").toString();
        FrmPagar *pr = new FrmPagar( has_contab, pago, fechapago,
                                     cuentapago, docpago, 0, "FrmPagarAlbaranVenta" );
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
#elif defined (HAVE_TESORERIAMODULE)
            if( has_contab ) {
                searchCuentaPagoCodigo->setValue( pr->getCuentaPago() );
			}
#endif
            if( editTotal->toDouble() != 0.0 )
                pushCobrar->setText( _("&Borrar entrega") );
        }
        delete pr;
    }
}

/*<<<<<FRMEDITALBARANVENTA_FIN*/
} // namespace factu
} // namespace gong
/*>>>>>FRMEDITALBARANVENTA_FIN*/



