/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de albaranes de compra
// FIELD Fecha date - cabecera
// FIELD TipoDoc_ID Reference(TipoDoc,Codigo,Nombre) - cabecera
// FIELD Numero string - cabecera
// FIELD IVADetallado comboint - cabecera
// FIELD Facturado bool - cabecera
// FIELD Proveedora_ID Reference(Proveedora,Codigo,RazonSocial,dbApplication::InsertIfNotFound) - cabecera2
// FIELD FormaPago_ID Reference(pagos::FormaPago,Codigo,Nombre,dbApplication::InsertIfNotFound) - cabecera2
// FIELD Contador int - cabecera2
// FIELD AlbaranCompraDet FrmEditRecDetail
// FIELD NoFacturable bool - noaddrightNoFacturable
// FIELD SumaImportes money - noaddrightSumaImportes
// FIELD DtoP100 double - noaddrightDtoP100
// FIELD Descuento money - noaddrightDescuento
// FIELD BaseImponible money - noaddrightBaseImponible
// FIELD RecargoEquivalencia money - noaddrightRecargoEquivalencia
// FIELD IVA money - noaddrightIVA
// FIELD Resto money - noaddrightResto
// FIELD Total money - noaddrightTotal
// FIELD Pagos money - noaddrightPagos
// FIELD Entrega money - noaddrightEntrega
// FIELD Proyecto_ID Reference(empresa::Proyecto,Codigo,Nombre) tabPagos proyecto if(empresa::ModuleInstance->usaProyectos())
// FIELD DesgloseIVA string tabPagos desgloseiva
// FIELD DocumentoPago string tabPagos pago
// FIELD FechaPago date tabPagos pago
// FIELD CuentaPago_ID Reference(CuentaTesoreria,Codigo,Nombre) tabPagos pago MODULE_INCLUDED(Tesoreria)
// FIELD CuentaPago_ID Reference(Cuenta,Cuenta,Descripcion) tabPagos pago MODULE_INCLUDED(Contab)
// FIELD Notas text tabPagos notas
// TYPE FrmEditRecMaster factu::AlbaranCompra Albaran Compra
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITALBARANCOMPRA_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "factufrmeditalbarancompra.h"
/*>>>>>FRMEDITALBARANCOMPRA_INCLUDES*/
#include "factumodule.h"
#include "factufrmpagar.h"
#include "factufldivadetallado.h"

namespace gong {
namespace factu {

/*<<<<<FRMEDITALBARANCOMPRA_CONSTRUCTOR*/
FrmEditAlbaranCompra::FrmEditAlbaranCompra(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditAlbaranCompra" );
/*>>>>>FRMEDITALBARANCOMPRA_CONSTRUCTOR*/
    /*<<<<<FRMEDITALBARANCOMPRA_INIT_CONTROLS*/
	QHBoxLayout *cabeceraLayout = new QHBoxLayout(0, 0, 6, "cabeceraLayout");
	QHBoxLayout *cabecera2Layout = new QHBoxLayout(0, 0, 6, "cabecera2Layout");
	QHBoxLayout *albarancompradetLayout = new QHBoxLayout(0, 0, 6, "albarancompradetLayout");
	QHBoxLayout *rightNoFacturableLayout = new QHBoxLayout(0, 0, 6, "rightNoFacturableLayout");
	QHBoxLayout *rightSumaImportesLayout = new QHBoxLayout(0, 0, 6, "rightSumaImportesLayout");
	QHBoxLayout *rightDtoP100Layout = new QHBoxLayout(0, 0, 6, "rightDtoP100Layout");
	QHBoxLayout *rightDescuentoLayout = new QHBoxLayout(0, 0, 6, "rightDescuentoLayout");
	QHBoxLayout *rightBaseImponibleLayout = new QHBoxLayout(0, 0, 6, "rightBaseImponibleLayout");
	QHBoxLayout *rightRecargoEquivalenciaLayout = new QHBoxLayout(0, 0, 6, "rightRecargoEquivalenciaLayout");
	QHBoxLayout *rightIVALayout = new QHBoxLayout(0, 0, 6, "rightIVALayout");
	QHBoxLayout *rightRestoLayout = new QHBoxLayout(0, 0, 6, "rightRestoLayout");
	QHBoxLayout *rightTotalLayout = new QHBoxLayout(0, 0, 6, "rightTotalLayout");
	QHBoxLayout *rightPagosLayout = new QHBoxLayout(0, 0, 6, "rightPagosLayout");
	QHBoxLayout *rightEntregaLayout = new QHBoxLayout(0, 0, 6, "rightEntregaLayout");
	showTabs(true);
	QWidget *tabPagos = new QWidget( pTabWidget, "tabPagos" );
	QVBoxLayout *tabPagosLayout = new QVBoxLayout(tabPagos, 11, 6, "tabPagosLayout");
	QHBoxLayout *proyectoLayout = new QHBoxLayout(0, 0, 6, "proyectoLayout");
	QHBoxLayout *desgloseivaLayout = new QHBoxLayout(0, 0, 6, "desgloseivaLayout");
	QHBoxLayout *pagoLayout = new QHBoxLayout(0, 0, 6, "pagoLayout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
	editFecha = addEditField( pControlsFrame, "ALBARANCOMPRA", "FECHA", cabeceraLayout );

	searchTipoDocCodigo = addSearchField( pControlsFrame, "TIPODOC_ID", "TIPODOC", "CODIGO", "NOMBRE", cabeceraLayout );
	pushTipoDocCodigo = searchTipoDocCodigo->getButton();
	connect( pushTipoDocCodigo, SIGNAL( clicked() ), this, SLOT( pushTipoDocCodigo_clicked() ) );
	editTipoDocCodigo = searchTipoDocCodigo->getEditCode();
	editTipoDocNombre = searchTipoDocCodigo->getEditDesc();
	editNumero = addEditField( pControlsFrame, "ALBARANCOMPRA", "NUMERO", cabeceraLayout );
	comboIVADetallado = addComboIntField( pControlsFrame, "ALBARANCOMPRA", "IVADETALLADO", cabeceraLayout );
	checkFacturado = addCheckField( pControlsFrame, "ALBARANCOMPRA", "FACTURADO", cabeceraLayout );

	searchProveedoraCodigo = addSearchField( pControlsFrame, "PROVEEDORA_ID", "PROVEEDORA", "CODIGO", "RAZONSOCIAL", cabecera2Layout );
	pushProveedoraCodigo = searchProveedoraCodigo->getButton();
	connect( pushProveedoraCodigo, SIGNAL( clicked() ), this, SLOT( pushProveedoraCodigo_clicked() ) );
	editProveedoraCodigo = searchProveedoraCodigo->getEditCode();
	editProveedoraRazonSocial = searchProveedoraCodigo->getEditDesc();

	searchFormaPagoCodigo = addSearchField( pControlsFrame, "FORMAPAGO_ID", "FORMAPAGO", "CODIGO", "NOMBRE", cabecera2Layout );
	pushFormaPagoCodigo = searchFormaPagoCodigo->getButton();
	connect( pushFormaPagoCodigo, SIGNAL( clicked() ), this, SLOT( pushFormaPagoCodigo_clicked() ) );
	editFormaPagoCodigo = searchFormaPagoCodigo->getEditCode();
	editFormaPagoNombre = searchFormaPagoCodigo->getEditDesc();
	editContador = addEditField( pControlsFrame, "ALBARANCOMPRA", "CONTADOR", cabecera2Layout );

// frmDetails: AlbaranCompraDet
	QFrame *albarancompradetFrame = new QFrame(this);
	albarancompradetFrame->setFrameStyle(QFrame::Sunken | QFrame::Panel);
	albarancompradetFrame->setLineWidth(4);
	QVBoxLayout *albarancompradetFrameLayout = new QVBoxLayout(albarancompradetFrame);
	RecAlbaranCompraDet *albarancompradet = getRecAlbaranCompra()->getRecAlbaranCompraDet(-1);
	dbViewDefinitionDict albarancompradetviews;
	DBAPP->getDatabase()->getViewsForTable( albarancompradet, albarancompradetviews );
	dbRecordListDataModel *albarancompradetdm =
		new dbRecordListDataModel(getRecAlbaranCompra()->getListAlbaranCompraDet(), albarancompradet, albarancompradetviews, "1=0");
	pFrmAlbaranCompraDet = static_cast<FrmEditAlbaranCompraDet *>(
		DBAPP->createEditDetailForm(this,
		-1, albarancompradet, "ALBARANCOMPRADET",albarancompradetdm, DataTable::browsing,
		static_cast<dbApplication::EditFlags>(dbApplication::embedded + dbApplication::tableEditEvent ),
		pControlsFrame) );
	pFrmAlbaranCompraDet->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding);
	albarancompradetFrameLayout->addWidget( pFrmAlbaranCompraDet );
	albarancompradetLayout->addWidget(albarancompradetFrame);
	checkNoFacturable = addCheckField( pControlsFrame, "ALBARANCOMPRA", "NOFACTURABLE", rightNoFacturableLayout );
	editSumaImportes = addEditField( pControlsFrame, "ALBARANCOMPRA", "SUMAIMPORTES", rightSumaImportesLayout );
	editDtoP100 = addEditField( pControlsFrame, "ALBARANCOMPRA", "DTOP100", rightDtoP100Layout );
	editDescuento = addEditField( pControlsFrame, "ALBARANCOMPRA", "DESCUENTO", rightDescuentoLayout );
	editBaseImponible = addEditField( pControlsFrame, "ALBARANCOMPRA", "BASEIMPONIBLE", rightBaseImponibleLayout );
	editRecargoEquivalencia = addEditField( pControlsFrame, "ALBARANCOMPRA", "RECARGOEQUIVALENCIA", rightRecargoEquivalenciaLayout );
	editIVA = addEditField( pControlsFrame, "ALBARANCOMPRA", "IVA", rightIVALayout );
	editResto = addEditField( pControlsFrame, "ALBARANCOMPRA", "RESTO", rightRestoLayout );
	editTotal = addEditField( pControlsFrame, "ALBARANCOMPRA", "TOTAL", rightTotalLayout );
	editPagos = addEditField( pControlsFrame, "ALBARANCOMPRA", "PAGOS", rightPagosLayout );
	editEntrega = addEditField( pControlsFrame, "ALBARANCOMPRA", "ENTREGA", rightEntregaLayout );

if(empresa::ModuleInstance->usaProyectos()){
	searchProyectoCodigo = addSearchField( tabPagos, "PROYECTO_ID", "PROYECTO", "CODIGO", "NOMBRE", proyectoLayout );
	pushProyectoCodigo = searchProyectoCodigo->getButton();
	connect( pushProyectoCodigo, SIGNAL( clicked() ), this, SLOT( pushProyectoCodigo_clicked() ) );
	editProyectoCodigo = searchProyectoCodigo->getEditCode();
	editProyectoNombre = searchProyectoCodigo->getEditDesc();
}
	editDesgloseIVA = addEditField( tabPagos, "ALBARANCOMPRA", "DESGLOSEIVA", desgloseivaLayout );
	editDocumentoPago = addEditField( tabPagos, "ALBARANCOMPRA", "DOCUMENTOPAGO", pagoLayout );
	editFechaPago = addEditField( tabPagos, "ALBARANCOMPRA", "FECHAPAGO", pagoLayout );

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
	editNotas = addTextField( tabPagos, "ALBARANCOMPRA", "NOTAS", notasLayout );
	pControlsLayout->addLayout( cabeceraLayout );
	pControlsLayout->addLayout( cabecera2Layout );
	pControlsLayout->addLayout( albarancompradetLayout );
	alignLayout( rightNoFacturableLayout, false);
	alignLayout( rightSumaImportesLayout, false);
	alignLayout( rightDtoP100Layout, false);
	alignLayout( rightDescuentoLayout, false);
	alignLayout( rightBaseImponibleLayout, false);
	alignLayout( rightRecargoEquivalenciaLayout, false);
	alignLayout( rightIVALayout, false);
	alignLayout( rightRestoLayout, false);
	alignLayout( rightTotalLayout, false);
	alignLayout( rightPagosLayout, false);
	alignLayout( rightEntregaLayout, false);
	tabPagosLayout->addLayout( proyectoLayout );
	tabPagosLayout->addLayout( desgloseivaLayout );
	tabPagosLayout->addLayout( pagoLayout );
	tabPagosLayout->addLayout( notasLayout );
/*>>>>>FRMEDITALBARANCOMPRA_INIT_CONTROLS*/
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
    gridlayout->addLayout( rightPagosLayout, 2, 1 );
    gridlayout->addLayout( rightEntregaLayout, 2, 2 );
    gridlayout->addLayout( rightRestoLayout, 2, 3 );
    pControlsLayout->addLayout( gridlayout );

    pushPagar = new QPushButton( this );
    connect( pushPagar, SIGNAL( clicked() ), this, SLOT( slotPagar() ) );
    gridlayout->addWidget( pushPagar, 2, 0 );

    checkFacturado->setMustBeReadOnly( true );
    editContador->setMustBeReadOnly( true );
    editSumaImportes->setMustBeReadOnly( true );
    editBaseImponible->setMustBeReadOnly( true );
    editRecargoEquivalencia->setMustBeReadOnly( true );
    editIVA->setMustBeReadOnly( true );
    editPagos->setMustBeReadOnly( true );
    editDescuento->setMustBeReadOnly( true );
    editResto->setMustBeReadOnly( true );
    editProveedoraCodigo->setWidthInChars(8);
    editFormaPagoCodigo->setWidthInChars(8);
    editFormaPagoNombre->setWidthInChars(15);
    editNumero->setWidthInChars(20);
    editNotas->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Minimum);
    if( empresa::ModuleInstance->getRecEmpresa()->getValue("RECARGOEQUIVALENCIA").toBool() == false ) {
        editRecargoEquivalencia->getLabel()->setVisible( false );
        editRecargoEquivalencia->setVisible( false );
    }
    mHasPagos = false;
}

void FrmEditAlbaranCompra::scatterFields()
{
    if( isDuplicating() && mIsFirstScatter ) {
        getRecAlbaranCompra()->setValue( "NUMERO", "" );
        getRecAlbaranCompra()->setValue( "CONTADOR", 0 );
        getRecAlbaranCompra()->setValue( "FACTURADO", false );
        getRecAlbaranCompra()->setValue( "ENTREGA", Money(0.0) );
        getRecAlbaranCompra()->setValue( "PAGOS", Money(0.0) );
        getRecAlbaranCompra()->setValue( "RESTO", getRecAlbaranCompra()->getValue( "TOTAL") );
        getRecAlbaranCompra()->setValue( "FECHAPAGO", Date() );
        getRecAlbaranCompra()->setValue( "DOCUMENTOPAGO", 0 );
#if defined (HAVE_CONTABMODULE) || defined (HAVE_TESORERIAMODULE)
        getRecAlbaranCompra()->setValue( "APUNTE_ID", 0 );
        getRecCuentaPago()->clear( false );
#endif
    }
    /*<<<<<FRMEDITALBARANCOMPRA_SCATTER*/
	editFecha->setText(getRecAlbaranCompra()->getValue("FECHA").toDate());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editFecha;
	editNumero->setText(getRecAlbaranCompra()->getValue("NUMERO").toString());
	comboIVADetallado->setCurrentItemByValue(getRecAlbaranCompra()->getValue("IVADETALLADO").toInt());
	checkFacturado->setChecked(getRecAlbaranCompra()->getValue("FACTURADO").toBool());
	editContador->setText(getRecAlbaranCompra()->getValue("CONTADOR").toInt());
	checkNoFacturable->setChecked(getRecAlbaranCompra()->getValue("NOFACTURABLE").toBool());
	editSumaImportes->setText(getRecAlbaranCompra()->getValue("SUMAIMPORTES").toMoney());
	editDtoP100->setText(getRecAlbaranCompra()->getValue("DTOP100").toDouble());
	editDescuento->setText(getRecAlbaranCompra()->getValue("DESCUENTO").toMoney());
	editBaseImponible->setText(getRecAlbaranCompra()->getValue("BASEIMPONIBLE").toMoney());
	editRecargoEquivalencia->setText(getRecAlbaranCompra()->getValue("RECARGOEQUIVALENCIA").toMoney());
	editIVA->setText(getRecAlbaranCompra()->getValue("IVA").toMoney());
	editResto->setText(getRecAlbaranCompra()->getValue("RESTO").toMoney());
	editTotal->setText(getRecAlbaranCompra()->getValue("TOTAL").toMoney());
	editPagos->setText(getRecAlbaranCompra()->getValue("PAGOS").toMoney());
	editEntrega->setText(getRecAlbaranCompra()->getValue("ENTREGA").toMoney());
	editDesgloseIVA->setText(getRecAlbaranCompra()->getValue("DESGLOSEIVA").toString());
	editDocumentoPago->setText(getRecAlbaranCompra()->getValue("DOCUMENTOPAGO").toString());
	editFechaPago->setText(getRecAlbaranCompra()->getValue("FECHAPAGO").toDate());
	editNotas->setText(getRecAlbaranCompra()->getValue("NOTAS").toString());
	scatterTipoDoc();
	scatterProveedora();
	scatterFormaPago();
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
/*>>>>>FRMEDITALBARANCOMPRA_SCATTER*/
	if( mIsFirstScatter ) {
		if( isInserting() ) {
			if( !isDuplicating() && editFecha->toDate().isNull() )
				editFecha->setText( ModuleInstance->getWorkingDate() );
			empresa::EmpresaModule *em = ModuleInstance->getEmpresaModule();
			if( em && em->getRecEmpresa() ) {
				if( em->getRecEmpresa()->getValue("RECARGOEQUIVALENCIA").toBool() )
					comboIVADetallado->setCurrentItemByValue( FldIVADetallado::con_recargo );
			}
			if( isDuplicating() ) {
				editTipoDocCodigo->setJustEdited( true );
				validateFields( editTipoDocCodigo, 0 );
			}
		} else if( isUpdating() ) {
			pFocusWidget = pFrmAlbaranCompraDet;
		}
		pFrmAlbaranCompraDet->addDetailIfNeeded();
		if( editContador->toInt() == 0 )
			editContador->setText( empresa::ModuleInstance->getMaxContador() );
		searchProveedoraCodigo->setMustBeReadOnly( mHasPagos );
		searchFormaPagoCodigo->setMustBeReadOnly( mHasPagos );
		editFecha->setMustBeReadOnly( mHasPagos );
		editDtoP100->setMustBeReadOnly( mHasPagos );
		editTotal->setMustBeReadOnly( mHasPagos );
		editEntrega->setMustBeReadOnly( mHasPagos );
		pushPagar->setVisible( !mHasPagos );
//    	scatterFormaPago(); // Para cambiar el texto del botón pagar después de actualizar los totales
		validateFields( comboIVADetallado, 0 ); // Para mostrar u ocultar el recargo de equivalencia
	}
}

void FrmEditAlbaranCompra::gatherFields()
{
    /*<<<<<FRMEDITALBARANCOMPRA_GATHER*/
	getRecAlbaranCompra()->setValue( "FECHA", editFecha->toDate());
	getRecAlbaranCompra()->setValue( "TIPODOC_ID", getRecTipoDoc()->getRecordID() );
	getRecAlbaranCompra()->setValue( "NUMERO", editNumero->toString());
	getRecAlbaranCompra()->setValue( "IVADETALLADO", comboIVADetallado->getCurrentItemValue());
	getRecAlbaranCompra()->setValue( "FACTURADO", checkFacturado->isChecked());
	getRecAlbaranCompra()->setValue( "PROVEEDORA_ID", getRecProveedora()->getRecordID() );
	getRecAlbaranCompra()->setValue( "FORMAPAGO_ID", getRecFormaPago()->getRecordID() );
	getRecAlbaranCompra()->setValue( "CONTADOR", editContador->toInt());
	getRecAlbaranCompra()->setValue( "NOFACTURABLE", checkNoFacturable->isChecked());
	getRecAlbaranCompra()->setValue( "SUMAIMPORTES", editSumaImportes->toMoney());
	getRecAlbaranCompra()->setValue( "DTOP100", editDtoP100->toDouble());
	getRecAlbaranCompra()->setValue( "DESCUENTO", editDescuento->toMoney());
	getRecAlbaranCompra()->setValue( "BASEIMPONIBLE", editBaseImponible->toMoney());
	getRecAlbaranCompra()->setValue( "RECARGOEQUIVALENCIA", editRecargoEquivalencia->toMoney());
	getRecAlbaranCompra()->setValue( "IVA", editIVA->toMoney());
	getRecAlbaranCompra()->setValue( "RESTO", editResto->toMoney());
	getRecAlbaranCompra()->setValue( "TOTAL", editTotal->toMoney());
	getRecAlbaranCompra()->setValue( "PAGOS", editPagos->toMoney());
	getRecAlbaranCompra()->setValue( "ENTREGA", editEntrega->toMoney());
if(empresa::ModuleInstance->usaProyectos()){
	getRecAlbaranCompra()->setValue( "PROYECTO_ID", getRecProyecto()->getRecordID() );
}
	getRecAlbaranCompra()->setValue( "DESGLOSEIVA", editDesgloseIVA->toString());
	getRecAlbaranCompra()->setValue( "DOCUMENTOPAGO", editDocumentoPago->toString());
	getRecAlbaranCompra()->setValue( "FECHAPAGO", editFechaPago->toDate());
#ifdef HAVE_TESORERIAMODULE
if( ModuleInstance->getTesoreriaModule() ) {
	getRecAlbaranCompra()->setValue( "CUENTAPAGO_ID", getRecCuentaPago()->getRecordID() );
}
#endif
#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	getRecAlbaranCompra()->setValue( "CUENTAPAGO_ID", getRecCuentaPago()->getRecordID() );
}
#endif
	getRecAlbaranCompra()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITALBARANCOMPRA_GATHER*/
    ModuleInstance->setWorkingDate( editFecha->toDate() );
}


void FrmEditAlbaranCompra::scatterTipoDoc()
{
    /*<<<<<FRMEDITALBARANCOMPRA_SCATTER_TIPODOC*/
	editTipoDocCodigo->setText( getRecTipoDoc()->getValue("CODIGO") );
	editTipoDocNombre->setText( getRecTipoDoc()->getValue("NOMBRE") );
/*>>>>>FRMEDITALBARANCOMPRA_SCATTER_TIPODOC*/
    if( editTipoDocCodigo->isJustEdited() ) {
        checkNoFacturable->setChecked( getRecTipoDoc()->getValue("NOFACTURABLE").toBool() );
        if( getRecTipoDoc()->getValue("IVADetallado").toInt() != 0 )
            comboIVADetallado->setCurrentItemByValue( getRecTipoDoc()->getValue("IVADetallado").toInt() );
        if( editFormaPagoCodigo->toString().isEmpty() )
            searchFormaPagoCodigo->setValue( getRecTipoDoc()->getValue("FORMAPAGO.CODIGO").toInt() );
    }
}

void FrmEditAlbaranCompra::pushTipoDocCodigo_clicked()
{
    /*<<<<<FRMEDITALBARANCOMPRA_PUSH_TIPODOC_CODIGO_CLICKED*/
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
/*>>>>>FRMEDITALBARANCOMPRA_PUSH_TIPODOC_CODIGO_CLICKED*/
}

void FrmEditAlbaranCompra::scatterProveedora()
{
    /*<<<<<FRMEDITALBARANCOMPRA_SCATTER_PROVEEDORA*/
	editProveedoraCodigo->setText( getRecProveedora()->getValue("CODIGO") );
	editProveedoraRazonSocial->setText( getRecProveedora()->getValue("RAZONSOCIAL") );
/*>>>>>FRMEDITALBARANCOMPRA_SCATTER_PROVEEDORA*/
    /*<<<<<FRMEDITALBARANCOMPRA_CABECERA_SCATTER_PROVEEDORA*/
	if( editProveedoraCodigo->isJustEdited() ) {
		if( getRecProveedora()->getRecFormaPago()->getValue("CODIGO").toInt() != 0 )
			searchFormaPagoCodigo->setValue( getRecProveedora()->getRecFormaPago()->getValue("CODIGO") );
		if( getRecProveedora()->getValue("IVADetallado").toInt() != 0 )
			comboIVADetallado->setCurrentItemByValue( getRecProveedora()->getValue("IVADetallado").toInt() );
	}
/*>>>>>FRMEDITALBARANCOMPRA_CABECERA_SCATTER_PROVEEDORA*/
}

void FrmEditAlbaranCompra::pushProveedoraCodigo_clicked()
{
    /*<<<<<FRMEDITALBARANCOMPRA_PUSH_PROVEEDORA_CODIGO_CLICKED*/
	char action = mControlKeyPressed;
	if( !isEditing() || searchProveedoraCodigo->mustBeReadOnly() )
		action = 'E';
	switch( action ) {
		case 'F':
		case '\0':
			editProveedoraCodigo->setJustEdited( false );
			editProveedoraCodigo->setCancelling();
			if( DBAPP->choose(this, getRecProveedora(), 0, dbApplication::editNone, this ) ) {
				setEdited(true);
				scatterProveedora();
				editProveedoraCodigo->setJustEdited( true );
				setWiseFocus(editProveedoraCodigo);
			}
			break;
		case 'M':
			{
				if( getRecProveedora()->getRecordID() ) {
					editProveedoraCodigo->setJustEdited( false );
					if( DBAPP->editRecord(this,
							getRecProveedora(), 0, DataTable::updating,
							dbApplication::simpleEdition, this ) ) {
						editProveedoraCodigo->setJustEdited( true );
						scatterProveedora();
					}
				setWiseFocus(editProveedoraCodigo);
				}
			}
			break;
		case 'E':
			{
				if( getRecProveedora()->getRecordID() != 0 ) {
					editProveedoraCodigo->setJustEdited( false );
					DBAPP->getMainWindow()->createClient( DBAPP->createEditForm(this, getRecProveedora(),
						0, DataTable::selecting, dbApplication::simpleEdition, this ) );
				}
			}
			break;
		case 'A':
			{
				RecProveedora *tmprec = static_cast<RecProveedora *>(DBAPP->createRecord( "Proveedora" ));
				editProveedoraCodigo->setJustEdited( false );
				tmprec->clear( true ); // set default values
				DBAPP->setCodeNotFound( editProveedoraCodigo->toString() );
				if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
					dbApplication::simpleEdition, this ) ) {
					editProveedoraCodigo->setJustEdited( true );
					getRecProveedora()->copyRecord( tmprec );
					scatterProveedora();
				}
				setWiseFocus(editProveedoraCodigo);
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITALBARANCOMPRA_PUSH_PROVEEDORA_CODIGO_CLICKED*/
}

void FrmEditAlbaranCompra::scatterFormaPago()
{
/*<<<<<FRMEDITALBARANCOMPRA_SCATTER_FORMAPAGO*/
	editFormaPagoCodigo->setText( getRecFormaPago()->getValue("CODIGO") );
	editFormaPagoNombre->setText( getRecFormaPago()->getValue("NOMBRE") );
/*>>>>>FRMEDITALBARANCOMPRA_SCATTER_FORMAPAGO*/
	if( editFormaPagoCodigo->isJustEdited() ) {
#ifdef HAVE_CONTABMODULE
		searchCuentaPagoCuenta->setValue(getRecFormaPago()->getRecCuentaPago()->getValue("CODIGO").toString());
#elif defined( HAVE_TESORERIAMODULE )
		searchCuentaPagoCodigo->setValue(getRecFormaPago()->getValue("CUENTATESORERIA.CODIGO").toString());
#endif		
	}
    if( getRecFormaPago()->getValue( "TIPOFORMAPAGO" ).toInt() == pagos::RecFormaPago::Contado
            || getRecFormaPago()->getValue( "TIPOFORMAPAGO" ).toInt() == pagos::RecFormaPago::SeIgnora ) {
        pushPagar->setVisible( false );
        editEntrega->setMustBeReadOnly( true );
    } else {
        pushPagar->setVisible( true );
        editEntrega->setMustBeReadOnly( false );
    }
    actTotales();
    if( editEntrega->toDouble() != 0.0 && editTotal->toDouble() != 0.0 )
        pushPagar->setText( _("&Borrar entrega") );
    else
        pushPagar->setText( _("&Entrega") );
}

void FrmEditAlbaranCompra::pushFormaPagoCodigo_clicked()
{
    /*<<<<<FRMEDITALBARANCOMPRA_PUSH_FORMAPAGO_CODIGO_CLICKED*/
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
/*>>>>>FRMEDITALBARANCOMPRA_PUSH_FORMAPAGO_CODIGO_CLICKED*/
}



#ifdef HAVE_CONTABMODULE
void FrmEditAlbaranCompra::scatterCuentaPago()
{
/*<<<<<FRMEDITALBARANCOMPRA_SCATTER_CUENTAPAGO*/
	editCuentaPagoCuenta->setText( getRecCuentaPago()->getValue("CUENTA") );
	editCuentaPagoDescripcion->setText( getRecCuentaPago()->getValue("DESCRIPCION") );
/*>>>>>FRMEDITALBARANCOMPRA_SCATTER_CUENTAPAGO*/
}
#elif defined (HAVE_TESORERIAMODULE)
void FrmEditAlbaranCompra::scatterCuentaPago()
{
	editCuentaPagoCodigo->setText( getRecCuentaPago()->getValue("CUENTA") );
	editCuentaPagoNombre->setText( getRecCuentaPago()->getValue("DESCRIPCION") );
}
#endif

#ifdef HAVE_CONTABMODULE
void FrmEditAlbaranCompra::pushCuentaPagoCuenta_clicked()
{
/*<<<<<FRMEDITALBARANCOMPRA_PUSH_CUENTAPAGO_CUENTA_CLICKED*/
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
/*>>>>>FRMEDITALBARANCOMPRA_PUSH_CUENTAPAGO_CUENTA_CLICKED*/
}
#endif

#ifdef HAVE_TESORERIAMODULE
void FrmEditAlbaranCompra::pushCuentaPagoCodigo_clicked()
{
/*<<<<<FRMEDITALBARANCOMPRA_PUSH_CUENTAPAGO_CODIGO_CLICKED*/
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
					DBAPP->getMainWindow()->createClient( DBAPP->createEditForm(this, getRecCuentaPago(),
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
/*>>>>>FRMEDITALBARANCOMPRA_PUSH_CUENTAPAGO_CODIGO_CLICKED*/
}
#endif


void FrmEditAlbaranCompra::scatterProyecto()
{
    /*<<<<<FRMEDITALBARANCOMPRA_SCATTER_PROYECTO*/
	editProyectoCodigo->setText( getRecProyecto()->getValue("CODIGO") );
	editProyectoNombre->setText( getRecProyecto()->getValue("NOMBRE") );
/*>>>>>FRMEDITALBARANCOMPRA_SCATTER_PROYECTO*/
}

void FrmEditAlbaranCompra::pushProyectoCodigo_clicked()
{
    /*<<<<<FRMEDITALBARANCOMPRA_PUSH_PROYECTO_CODIGO_CLICKED*/
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
/*>>>>>FRMEDITALBARANCOMPRA_PUSH_PROYECTO_CODIGO_CLICKED*/
}



void FrmEditAlbaranCompra::specialControlKeyPressed( QWidget *sender, char key )
{
    /*<<<<<FRMEDITALBARANCOMPRA_SPECIALACTION*/
	mControlKeyPressed = key;
	FrmEditRecMaster::specialControlKeyPressed(sender,key); // calls the behaviors
	if( sender == editTipoDocCodigo )
		pushTipoDocCodigo_clicked();
	if( sender == editProveedoraCodigo )
		pushProveedoraCodigo_clicked();
	if( sender == editFormaPagoCodigo )
		pushFormaPagoCodigo_clicked();
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
/*>>>>>FRMEDITALBARANCOMPRA_SPECIALACTION*/
}

void FrmEditAlbaranCompra::validateFields( QWidget *sender, bool *isvalid, ValidResult *ir )
{
/*<<<<<FRMEDITALBARANCOMPRA_VALIDATE*/
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
	if( focusWidget() != pushProveedoraCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editProveedoraCodigo, editProveedoraRazonSocial,
		getRecProveedora(), "CODIGO", "RAZONSOCIAL", Xtring::null, dbApplication::SeekCodeFlags( dbApplication::InsertIfNotFound )) )
		scatterProveedora();
	if( focusWidget() != pushFormaPagoCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editFormaPagoCodigo, editFormaPagoNombre,
		getRecFormaPago(), "CODIGO", "NOMBRE", Xtring::null, dbApplication::SeekCodeFlags( dbApplication::InsertIfNotFound )) )
		scatterFormaPago();
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
/*>>>>>FRMEDITALBARANCOMPRA_VALIDATE*/

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
            if( editCuentaPagoCuenta->toString().isEmpty() ) {
#elif defined (HAVE_TESORERIAMODULE)
            if( editCuentaPagoCodigo->toString().isEmpty() ) {
#endif
				validresult->addError( "No se ha introducido una cuenta de pago y la forma de pago tampoco la tiene definida", "CUENTAPAGO_ID");
			}
		}
	}
	if( sender == editEntrega && editEntrega->isJustEdited() ) {	
        actTotales();
    }
/*<<<<<FRMEDITALBARANCOMPRA_CABECERA_VALIDATE*/
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
/*>>>>>FRMEDITALBARANCOMPRA_CABECERA_VALIDATE*/
    if( !sender || (
                (sender == editNumero && !editNumero->toString().isEmpty())
                || (sender == editProveedoraCodigo && !editProveedoraCodigo->toString().isEmpty() ) )
      ) {
        dbRecordID prov_id = getRecProveedora()->getRecordID();
        if( getRecord()->existsAnother(
                    "PROVEEDORA_ID=" + getRecord()->getConnection()->toSQL( prov_id )
                    + " AND NUMERO=" + getRecord()->getConnection()->toSQL( editNumero->toString() ) ) ) {
            *isvalid = false;
            validresult->addError( Xtring::printf(_("Ya existe %s de %s con este valor para '%s'"),
                                                  DBAPP->getTableDescSingular( getRecord()->getTableName(), "una" ).c_str(),
                                                  DBAPP->getTableDescSingular( "PROVEEDORA", "esta" ).c_str(),
                                                  DBAPP->getDatabase()->findFieldDefinition( "ALBARANCOMPRA.NUMERO" )->getCaption().c_str()),
                                   "NUMERO" );
        }
    }
    if( !sender && isInserting() ) {
        int contador = empresa::ModuleInstance->getMaxContador();
        if( contador > editContador->toInt() ) {
            editContador->setText( contador );
            validresult->addWarning( Xtring::printf(
                                         _("El contador ha cambiado durante la edición de %s. El nuevo contador es %d"),
                                         DBAPP->getTableDescSingular( pRecord->getTableName(), "esta" ).c_str(), contador ), "CONTADOR" );
        }
    }
    if( mHasPagos && mOldTotal != editTotal->toMoney() ) {
        validresult->addError( Xtring::printf( _("No puedes modificar el total de %s porque tiene pagos ya pagados"),
                                               DBAPP->getTableDescSingular( pRecord->getTableName(), "esta" ).c_str() ), "TIPODOC_ID" );
        *isvalid = false;
    }
    if ( !ir ) {
        showValidMessages( isvalid, *validresult, sender );
        delete validresult;
    }
}

/*<<<<<FRMEDITALBARANCOMPRA_CABECERA_UPDATEFROMDETAILS*/
void FrmEditAlbaranCompra::updateFromDetails( class FrmEditRecDetail *detailfrm )
{
	actTotales();
	numeraLineas();
/*>>>>>FRMEDITALBARANCOMPRA_CABECERA_UPDATEFROMDETAILS*/
}

/*<<<<<FRMEDITALBARANCOMPRA_CABECERA_NUMERALINEAS*/
void FrmEditAlbaranCompra::numeraLineas()
{
	dbRecordList *reclst = getRecAlbaranCompra()->getListAlbaranCompraDet();
	for ( unsigned int i = 0; i < reclst->size(); i++ ) {
		RecAlbaranCompraDet *detalle = static_cast<RecAlbaranCompraDet *>( reclst->at( i ) );
		if( !detalle->isEmpty() ) // No numerar detalles vacíos 
			detalle->setValue( "NLINEA", i+1 );
	}
/*>>>>>FRMEDITALBARANCOMPRA_CABECERA_NUMERALINEAS*/
}

bool FrmEditAlbaranCompra::canBeginEdit(DataTable::EditMode newmode)
{
    if( getRecord()->getValue("FACTURADO").toBool()
            && ( newmode == DataTable::updating || newmode == DataTable::deleting ) ) {
        msgError(this, Xtring::printf( _("%s no se puede modificar ni borrar porque ya está facturado"),
                                       DBAPP->getTableDescSingular( pRecord->getTableName(), "Esta").c_str() ) );
        return false;
    }
    mHasPagos = false;
    mOldTotal = getRecAlbaranCompra()->getValue( "TOTAL" ).toMoney();
    if( newmode == DataTable::updating || newmode == DataTable::deleting ) {
        if( getRecAlbaranCompra()->hasPagos( -1, pagos::PagosModule::ReciboPendiente ) ) {
            mHasPagos = true;
            if( newmode == DataTable::updating ) {
                return FrmBase::msgYesNo(this,
                                         Xtring::printf( _("%s tiene pagos ya pagados. No podrás modificar algunos datos\n¿Quieres continuar?"),
                                                 DBAPP->getTableDescSingular( pRecord->getTableName(), "Esta").c_str() ) );
            } else { // deleting
                FrmBase::msgError(this,
                                  Xtring::printf( _("%s tiene pagos ya pagados. Tines que anular todos sus pagos antes de borrar."),
                                                  DBAPP->getTableDescSingular( pRecord->getTableName(), "Esta").c_str() ) );
                return false;
            }
        }
    }
    return FrmEditRecMaster::canBeginEdit(newmode);
}

void FrmEditAlbaranCompra::slotPagar()
{
    if( editEntrega->toDouble() != 0 ) {
        editResto->setText( editResto->toMoney() + editEntrega->toMoney() );
        editEntrega->setText( 0.0 );
        editFechaPago->setText( "" );
        editDocumentoPago->setText( "" );
#if defined (HAVE_CONTABMODULE) || defined(HAVE_TESORERIAMODULE)
        getRecCuentaPago()->clear( false );
        scatterCuentaPago();
#endif
        pushPagar->setText( _("&Entrega") );
    } else {
        Date fechapago = editFechaPago->toDate();
        if( fechapago.isNull() )
            fechapago = editFecha->toDate();
        Xtring docpago = _("Pago alb. " + editNumero->toString() );
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
                                     cuentapago, docpago, 0, "FrmPagar" );
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
                searchCuentaPagoCuenta->setValue( cuenta_pago );
            }
#elif defined (HAVE_TESORERIAMODULE)
            if( has_contab ) {
                searchCuentaPagoCodigo->setValue( pr->getCuentaPago() );
			}
#endif
            if( editEntrega->toDouble() != 0.0 )
                pushPagar->setText( _("&Borrar entrega") );
        }
		delete pr;
    }
}

/*<<<<<FRMEDITALBARANCOMPRA_CABECERA_ACTTOTALES*/
void FrmEditAlbaranCompra::actTotales()
{
	getRecAlbaranCompra()->gather( this );
	getRecAlbaranCompra()->actTotales();
	getRecAlbaranCompra()->scatter( this );
/*>>>>>FRMEDITALBARANCOMPRA_CABECERA_ACTTOTALES*/
}

/*<<<<<FRMEDITALBARANCOMPRA_FIN*/
} // namespace factu
} // namespace gong
/*>>>>>FRMEDITALBARANCOMPRA_FIN*/

#if 0
/*<<<<<FRMEDITALBARANCOMPRA_CABECERA_GENNUMDOC*/
void FrmEditAlbaranCompra::genNumeroDocumento()
{
		editContador->setText( ModuleInstance->getMaxContador( "ALBARANCOMPRA",
			getRecord()->getValue("EMPRESA_ID").toInt(),
			ModuleInstance->getEmpresaModule()->getEjercicio(),
			getRecTipoDoc()->getValue("SERIE").toString() ) );
		editContador->setJustEdited( true );
		validateFields( editContador, 0 );
/*>>>>>FRMEDITALBARANCOMPRA_CABECERA_GENNUMDOC*/
}
#endif