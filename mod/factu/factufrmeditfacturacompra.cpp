/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de facturas de compra
// FIELD Fecha date - cabecera
// FIELD TipoDoc_ID Reference(TipoDoc,Codigo,Nombre) - cabecera
// FIELD Numero string - cabecera
// FIELD Contador int - cabecera
// FIELD Proveedora_ID Reference(Proveedora,Codigo,RazonSocial,dbRecord::InsertIfNotFound) - cabecera2
// FIELD IVADetallado comboint - cabecera2
// FIELD FormaPago_ID Reference(pagos::FormaPago,Codigo,Nombre,dbRecord::InsertIfNotFound) - cabecera3
// FIELD Agente_ID Reference(Agente,Codigo,RazonSocial,dbRecord::InsertIfNotFound) - cabecera3
// FIELD FacturaCompraDet FrmEditRecDetail
// FIELD SumaImportes money - noaddrightSumaImportes
// FIELD DtoP100 double - noaddrightDtoP100
// FIELD Descuento money - noaddrightDescuento
// FIELD BaseImponible money - noaddrightBaseImponible
// FIELD RecargoEquivalencia money - noaddrightRecargoEquivalencia
// FIELD IVA money - noaddrightIVA
// FIELD Total money - noaddrightTotal
// FIELD Pagos money - noaddrightPagos
// FIELD EntregaAlbaranes money - noaddrightEntregaAlbaranes
// FIELD Resto money - noaddrightResto
// FIELD Entrega money - noaddrightEntrega
// FIELD Proyecto_ID Reference(empresa::Proyecto,Codigo,Nombre) tabPagos proyecto if(empresa::ModuleInstance->usaProyectos())
// FIELD DesgloseIVA string tabPagos desgloseiva
// FIELD DocumentoPago string tabPagos pago
// FIELD FechaPago date tabPagos pago
// FIELD CuentaPago_ID Reference(CuentaTesoreria,Codigo,Nombre) tabPagos cuentapago MODULE_INCLUDED(Tesoreria)
// FIELD CuentaPago_ID Reference(Cuenta,Cuenta,Descripcion) tabPagos cuentapago MODULE_INCLUDED(Contab)
// FIELD Notas text tabPagos notas
// TYPE FrmEditRecMaster factu::FacturaCompra Factura Compra
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITFACTURACOMPRA_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "factufrmeditfacturacompra.h"
/*>>>>>FRMEDITFACTURACOMPRA_INCLUDES*/
#include "factumodule.h"
#include "factufrmpagar.h"
#include "facturecalbarancompra.h"

namespace gong {
namespace factu {

/*<<<<<FRMEDITFACTURACOMPRA_CONSTRUCTOR*/
FrmEditFacturaCompra::FrmEditFacturaCompra(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditFacturaCompra" );
/*>>>>>FRMEDITFACTURACOMPRA_CONSTRUCTOR*/
    /*<<<<<FRMEDITFACTURACOMPRA_INIT_CONTROLS*/
	QHBoxLayout *cabeceraLayout = new QHBoxLayout(0, 0, 6, "cabeceraLayout");
	QHBoxLayout *cabecera2Layout = new QHBoxLayout(0, 0, 6, "cabecera2Layout");
	QHBoxLayout *cabecera3Layout = new QHBoxLayout(0, 0, 6, "cabecera3Layout");
	QHBoxLayout *facturacompradetLayout = new QHBoxLayout(0, 0, 6, "facturacompradetLayout");
	QHBoxLayout *rightSumaImportesLayout = new QHBoxLayout(0, 0, 6, "rightSumaImportesLayout");
	QHBoxLayout *rightDtoP100Layout = new QHBoxLayout(0, 0, 6, "rightDtoP100Layout");
	QHBoxLayout *rightDescuentoLayout = new QHBoxLayout(0, 0, 6, "rightDescuentoLayout");
	QHBoxLayout *rightBaseImponibleLayout = new QHBoxLayout(0, 0, 6, "rightBaseImponibleLayout");
	QHBoxLayout *rightRecargoEquivalenciaLayout = new QHBoxLayout(0, 0, 6, "rightRecargoEquivalenciaLayout");
	QHBoxLayout *rightIVALayout = new QHBoxLayout(0, 0, 6, "rightIVALayout");
	QHBoxLayout *rightTotalLayout = new QHBoxLayout(0, 0, 6, "rightTotalLayout");
	QHBoxLayout *rightPagosLayout = new QHBoxLayout(0, 0, 6, "rightPagosLayout");
	QHBoxLayout *rightEntregaAlbaranesLayout = new QHBoxLayout(0, 0, 6, "rightEntregaAlbaranesLayout");
	QHBoxLayout *rightRestoLayout = new QHBoxLayout(0, 0, 6, "rightRestoLayout");
	QHBoxLayout *rightEntregaLayout = new QHBoxLayout(0, 0, 6, "rightEntregaLayout");
	showTabs(true);
	QWidget *tabPagos = new QWidget( pTabWidget, "tabPagos" );
	QVBoxLayout *tabPagosLayout = new QVBoxLayout(tabPagos, 11, 6, "tabPagosLayout");
	QHBoxLayout *proyectoLayout = new QHBoxLayout(0, 0, 6, "proyectoLayout");
	QHBoxLayout *desgloseivaLayout = new QHBoxLayout(0, 0, 6, "desgloseivaLayout");
	QHBoxLayout *pagoLayout = new QHBoxLayout(0, 0, 6, "pagoLayout");
	QHBoxLayout *cuentapagoLayout = new QHBoxLayout(0, 0, 6, "cuentapagoLayout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
	editFecha = addEditField( pControlsFrame, "FACTURACOMPRA", "FECHA", cabeceraLayout );

	searchTipoDocCodigo = addSearchField( pControlsFrame, "TIPODOC_ID", "TIPODOC", "CODIGO", "NOMBRE", cabeceraLayout );
	pushTipoDocCodigo = searchTipoDocCodigo->getButton();
	connect( pushTipoDocCodigo, SIGNAL( clicked() ), this, SLOT( pushTipoDocCodigo_clicked() ) );
	editTipoDocCodigo = searchTipoDocCodigo->getEditCode();
	editTipoDocNombre = searchTipoDocCodigo->getEditDesc();
	editNumero = addEditField( pControlsFrame, "FACTURACOMPRA", "NUMERO", cabeceraLayout );
	editContador = addEditField( pControlsFrame, "FACTURACOMPRA", "CONTADOR", cabeceraLayout );

	searchProveedoraCodigo = addSearchField( pControlsFrame, "PROVEEDORA_ID", "PROVEEDORA", "CODIGO", "RAZONSOCIAL", cabecera2Layout );
	pushProveedoraCodigo = searchProveedoraCodigo->getButton();
	connect( pushProveedoraCodigo, SIGNAL( clicked() ), this, SLOT( pushProveedoraCodigo_clicked() ) );
	editProveedoraCodigo = searchProveedoraCodigo->getEditCode();
	editProveedoraRazonSocial = searchProveedoraCodigo->getEditDesc();
	comboIVADetallado = addComboIntField( pControlsFrame, "FACTURACOMPRA", "IVADETALLADO", cabecera2Layout );

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

// frmDetails: FacturaCompraDet
	QFrame *facturacompradetFrame = new QFrame(this);
	facturacompradetFrame->setFrameStyle(QFrame::Sunken | QFrame::Panel);
	facturacompradetFrame->setLineWidth(4);
	QVBoxLayout *facturacompradetFrameLayout = new QVBoxLayout(facturacompradetFrame);
	RecFacturaCompraDet *facturacompradet = getRecFacturaCompra()->getRecFacturaCompraDet(-1);
	dbViewDefinitionDict facturacompradetviews;
	DBAPP->getDatabase()->getViewsForTable( facturacompradet, facturacompradetviews );
	dbRecordListDataModel *facturacompradetdm =
		new dbRecordListDataModel(getRecFacturaCompra()->getListFacturaCompraDet(), facturacompradet, facturacompradetviews, "1=0");
	pFrmFacturaCompraDet = static_cast<FrmEditFacturaCompraDet *>(
		DBAPP->createEditDetailForm(this,
		-1, facturacompradet, "FACTURACOMPRADET",facturacompradetdm, DataTable::browsing,
		static_cast<dbApplication::EditFlags>(dbApplication::embedded + dbApplication::tableEditEvent ),
		pControlsFrame) );
	pFrmFacturaCompraDet->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding);
	facturacompradetFrameLayout->addWidget( pFrmFacturaCompraDet );
	facturacompradetLayout->addWidget(facturacompradetFrame);
	editSumaImportes = addEditField( pControlsFrame, "FACTURACOMPRA", "SUMAIMPORTES", rightSumaImportesLayout );
	editDtoP100 = addEditField( pControlsFrame, "FACTURACOMPRA", "DTOP100", rightDtoP100Layout );
	editDescuento = addEditField( pControlsFrame, "FACTURACOMPRA", "DESCUENTO", rightDescuentoLayout );
	editBaseImponible = addEditField( pControlsFrame, "FACTURACOMPRA", "BASEIMPONIBLE", rightBaseImponibleLayout );
	editRecargoEquivalencia = addEditField( pControlsFrame, "FACTURACOMPRA", "RECARGOEQUIVALENCIA", rightRecargoEquivalenciaLayout );
	editIVA = addEditField( pControlsFrame, "FACTURACOMPRA", "IVA", rightIVALayout );
	editTotal = addEditField( pControlsFrame, "FACTURACOMPRA", "TOTAL", rightTotalLayout );
	editPagos = addEditField( pControlsFrame, "FACTURACOMPRA", "PAGOS", rightPagosLayout );
	editEntregaAlbaranes = addEditField( pControlsFrame, "FACTURACOMPRA", "ENTREGAALBARANES", rightEntregaAlbaranesLayout );
	editResto = addEditField( pControlsFrame, "FACTURACOMPRA", "RESTO", rightRestoLayout );
	editEntrega = addEditField( pControlsFrame, "FACTURACOMPRA", "ENTREGA", rightEntregaLayout );

if(empresa::ModuleInstance->usaProyectos()){
	searchProyectoCodigo = addSearchField( tabPagos, "PROYECTO_ID", "PROYECTO", "CODIGO", "NOMBRE", proyectoLayout );
	pushProyectoCodigo = searchProyectoCodigo->getButton();
	connect( pushProyectoCodigo, SIGNAL( clicked() ), this, SLOT( pushProyectoCodigo_clicked() ) );
	editProyectoCodigo = searchProyectoCodigo->getEditCode();
	editProyectoNombre = searchProyectoCodigo->getEditDesc();
}
	editDesgloseIVA = addEditField( tabPagos, "FACTURACOMPRA", "DESGLOSEIVA", desgloseivaLayout );
	editDocumentoPago = addEditField( tabPagos, "FACTURACOMPRA", "DOCUMENTOPAGO", pagoLayout );
	editFechaPago = addEditField( tabPagos, "FACTURACOMPRA", "FECHAPAGO", pagoLayout );

#ifdef HAVE_TESORERIAMODULE
if( ModuleInstance->getTesoreriaModule() ) {
	searchCuentaPagoCodigo = addSearchField( tabPagos, "CUENTAPAGO_ID", "CUENTATESORERIA", "CODIGO", "NOMBRE", cuentapagoLayout );
	pushCuentaPagoCodigo = searchCuentaPagoCodigo->getButton();
	connect( pushCuentaPagoCodigo, SIGNAL( clicked() ), this, SLOT( pushCuentaPagoCodigo_clicked() ) );
	editCuentaPagoCodigo = searchCuentaPagoCodigo->getEditCode();
	editCuentaPagoNombre = searchCuentaPagoCodigo->getEditDesc();
}
#endif

#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	searchCuentaPagoCuenta = addSearchField( tabPagos, "CUENTAPAGO_ID", "CUENTA", "CUENTA", "DESCRIPCION", cuentapagoLayout );
	pushCuentaPagoCuenta = searchCuentaPagoCuenta->getButton();
	connect( pushCuentaPagoCuenta, SIGNAL( clicked() ), this, SLOT( pushCuentaPagoCuenta_clicked() ) );
	editCuentaPagoCuenta = searchCuentaPagoCuenta->getEditCode();
	editCuentaPagoDescripcion = searchCuentaPagoCuenta->getEditDesc();
}
#endif
	editNotas = addTextField( tabPagos, "FACTURACOMPRA", "NOTAS", notasLayout );
	pControlsLayout->addLayout( cabeceraLayout );
	pControlsLayout->addLayout( cabecera2Layout );
	pControlsLayout->addLayout( cabecera3Layout );
	pControlsLayout->addLayout( facturacompradetLayout );
	alignLayout( rightSumaImportesLayout, false);
	alignLayout( rightDtoP100Layout, false);
	alignLayout( rightDescuentoLayout, false);
	alignLayout( rightBaseImponibleLayout, false);
	alignLayout( rightRecargoEquivalenciaLayout, false);
	alignLayout( rightIVALayout, false);
	alignLayout( rightTotalLayout, false);
	alignLayout( rightPagosLayout, false);
	alignLayout( rightEntregaAlbaranesLayout, false);
	alignLayout( rightRestoLayout, false);
	alignLayout( rightEntregaLayout, false);
	tabPagosLayout->addLayout( proyectoLayout );
	tabPagosLayout->addLayout( desgloseivaLayout );
	tabPagosLayout->addLayout( pagoLayout );
	tabPagosLayout->addLayout( cuentapagoLayout );
	tabPagosLayout->addLayout( notasLayout );
/*>>>>>FRMEDITFACTURACOMPRA_INIT_CONTROLS*/
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
    gridlayout->addLayout( rightPagosLayout, 2, 1 );
    gridlayout->addLayout( rightEntregaLayout, 2, 2 );
    gridlayout->addLayout( rightRestoLayout, 2, 3 );
    pControlsLayout->addLayout( gridlayout );

    pushPagar = new QPushButton( this );
    connect( pushPagar, SIGNAL( clicked() ), this, SLOT( slotPagar() ) );
    rightEntregaAlbaranesLayout->insertWidget(0, pushPagar );

    editContador->setMustBeReadOnly( true );
    editSumaImportes->setMustBeReadOnly( true );
    editBaseImponible->setMustBeReadOnly( true );
    editRecargoEquivalencia->setMustBeReadOnly( true );
    editEntregaAlbaranes->setMustBeReadOnly( true );
    editPagos->setMustBeReadOnly( true );
    editIVA->setMustBeReadOnly( true );
    editDescuento->setMustBeReadOnly( true );
    editResto->setMustBeReadOnly( true );
    editProveedoraCodigo->setWidthInChars(8);
    editTipoDocCodigo->setWidthInChars(8);
    editFormaPagoCodigo->setWidthInChars(8);
	editFormaPagoNombre->setWidthInChars(10);
	editAgenteCodigo->setWidthInChars(8);
	editAgenteRazonSocial->setWidthInChars(10);
    editNotas->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Minimum);
	empresa::EmpresaModule *em = ModuleInstance->getEmpresaModule();
	if( em && em->getRecEmpresa() && !em->getRecEmpresa()->getValue("RECARGOEQUIVALENCIA").toBool() ) {
        editRecargoEquivalencia->getLabel()->setVisible( false );
        editRecargoEquivalencia->setVisible( false );
		mRecargoEquivalencia = false;
    } else {
		mRecargoEquivalencia = true;
	}
    mHasPagos = false;
}

void FrmEditFacturaCompra::scatterFields()
{
    if( isDuplicating() && mIsFirstScatter ) {
        getRecFacturaCompra()->setValue( "NUMERO", "" );
        getRecFacturaCompra()->setValue( "CONTADOR", 0 );
        getRecFacturaCompra()->setValue( "ENTREGAALBARANES", 0 );
        getRecFacturaCompra()->setValue( "ENTREGA", Money(0.0) );
        getRecFacturaCompra()->setValue( "PAGOS", Money(0.0) );
        getRecFacturaCompra()->setValue( "RESTO", getRecFacturaCompra()->getValue( "TOTAL") );
        getRecFacturaCompra()->setValue( "FECHAPAGO", Date() );
        getRecFacturaCompra()->setValue( "DOCUMENTOPAGO", 0 );
#ifdef HAVE_CONTABMODULE
        getRecFacturaCompra()->setValue( "ASIENTO_ID", 0 );
        getRecCuentaPago()->clear( false );
#endif
#ifdef HAVE_TESORERIAMODULE
        getRecFacturaCompra()->setValue( "APUNTE_ID", 0 );
        getRecCuentaPago()->clear( false );
#endif
        // No duplicar los albaranes facturados
        dbRecordList *reclst = getRecFacturaCompra()->getListFacturaCompraDet();
        for ( int i = reclst->size() - 1; i >= 0; --i ) {
            RecFacturaCompraDet *detalle = static_cast<RecFacturaCompraDet *>( reclst->at( i ) );
            if( detalle->getValue( "ALBARANCOMPRA_ID").toInt() != 0 ) {
                reclst->removeRecord( i );
                delete detalle;
            }
        }
    }
/*<<<<<FRMEDITFACTURACOMPRA_SCATTER*/
	editFecha->setText(getRecFacturaCompra()->getValue("FECHA").toDate());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editFecha;
	editNumero->setText(getRecFacturaCompra()->getValue("NUMERO").toString());
	editContador->setText(getRecFacturaCompra()->getValue("CONTADOR").toInt());
	comboIVADetallado->setCurrentItemByValue(getRecFacturaCompra()->getValue("IVADETALLADO").toInt());
	editSumaImportes->setText(getRecFacturaCompra()->getValue("SUMAIMPORTES").toMoney());
	editDtoP100->setText(getRecFacturaCompra()->getValue("DTOP100").toDouble());
	editDescuento->setText(getRecFacturaCompra()->getValue("DESCUENTO").toMoney());
	editBaseImponible->setText(getRecFacturaCompra()->getValue("BASEIMPONIBLE").toMoney());
	editRecargoEquivalencia->setText(getRecFacturaCompra()->getValue("RECARGOEQUIVALENCIA").toMoney());
	editIVA->setText(getRecFacturaCompra()->getValue("IVA").toMoney());
	editTotal->setText(getRecFacturaCompra()->getValue("TOTAL").toMoney());
	editPagos->setText(getRecFacturaCompra()->getValue("PAGOS").toMoney());
	editEntregaAlbaranes->setText(getRecFacturaCompra()->getValue("ENTREGAALBARANES").toMoney());
	editResto->setText(getRecFacturaCompra()->getValue("RESTO").toMoney());
	editEntrega->setText(getRecFacturaCompra()->getValue("ENTREGA").toMoney());
	editDesgloseIVA->setText(getRecFacturaCompra()->getValue("DESGLOSEIVA").toString());
	editDocumentoPago->setText(getRecFacturaCompra()->getValue("DOCUMENTOPAGO").toString());
	editFechaPago->setText(getRecFacturaCompra()->getValue("FECHAPAGO").toDate());
	editNotas->setText(getRecFacturaCompra()->getValue("NOTAS").toString());
	scatterTipoDoc();
	scatterProveedora();
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
/*>>>>>FRMEDITFACTURACOMPRA_SCATTER*/
	if( mIsFirstScatter ) {
		if( isInserting() ) {
			if( !isDuplicating() && editFecha->toDate().isNull() )
				editFecha->setText( ModuleInstance->getWorkingDate() );
			if( isDuplicating() ) {
				editTipoDocCodigo->setJustEdited( true );
				validateFields( editTipoDocCodigo, 0 );
			}
			if( mustRecargoEquivalencia() && comboIVADetallado->getCurrentItemValue() == 0 )
				comboIVADetallado->setCurrentItemByValue( FldIVADetallado::con_recargo );
		} else if( isUpdating() ) {
			pFocusWidget = pFrmFacturaCompraDet;
		}
		pFrmFacturaCompraDet->addDetailIfNeeded();
		// Si estamos duplicando o copiando desde cualquier otro documento y no tenemos contador, generarlo, pero si es una 
		// inserción normal, no generarlo hasta que se meta el tipo de documento
		if( isInserting() && editContador->toInt() == 0 && editTipoDocCodigo->toInt() != 0 && isFirstScatter() ) 
			editContador->setText( empresa::ModuleInstance->getMaxContador() );		searchProveedoraCodigo->setMustBeReadOnly( mHasPagos );
		searchFormaPagoCodigo->setMustBeReadOnly( mHasPagos );
		editFecha->setMustBeReadOnly( mHasPagos );
		editDtoP100->setMustBeReadOnly( mHasPagos );
		editTotal->setMustBeReadOnly( mHasPagos );
		editEntrega->setMustBeReadOnly( mHasPagos );
		pushPagar->setVisible( !mHasPagos );
		scatterFormaPago(); // Para cambiar el texto del botón pagar después de actualizar los totales
		validateFields( comboIVADetallado, 0 ); // Para mostrar u ocultar el recargo de equivalencia
	}
}

void FrmEditFacturaCompra::gatherFields()
{
    /*<<<<<FRMEDITFACTURACOMPRA_GATHER*/
	getRecFacturaCompra()->setValue( "FECHA", editFecha->toDate());
	getRecFacturaCompra()->setValue( "TIPODOC_ID", getRecTipoDoc()->getRecordID() );
	getRecFacturaCompra()->setValue( "NUMERO", editNumero->toString());
	getRecFacturaCompra()->setValue( "CONTADOR", editContador->toInt());
	getRecFacturaCompra()->setValue( "PROVEEDORA_ID", getRecProveedora()->getRecordID() );
	getRecFacturaCompra()->setValue( "IVADETALLADO", comboIVADetallado->getCurrentItemValue());
	getRecFacturaCompra()->setValue( "FORMAPAGO_ID", getRecFormaPago()->getRecordID() );
	getRecFacturaCompra()->setValue( "AGENTE_ID", getRecAgente()->getRecordID() );
	getRecFacturaCompra()->setValue( "SUMAIMPORTES", editSumaImportes->toMoney());
	getRecFacturaCompra()->setValue( "DTOP100", editDtoP100->toDouble());
	getRecFacturaCompra()->setValue( "DESCUENTO", editDescuento->toMoney());
	getRecFacturaCompra()->setValue( "BASEIMPONIBLE", editBaseImponible->toMoney());
	getRecFacturaCompra()->setValue( "RECARGOEQUIVALENCIA", editRecargoEquivalencia->toMoney());
	getRecFacturaCompra()->setValue( "IVA", editIVA->toMoney());
	getRecFacturaCompra()->setValue( "TOTAL", editTotal->toMoney());
	getRecFacturaCompra()->setValue( "PAGOS", editPagos->toMoney());
	getRecFacturaCompra()->setValue( "ENTREGAALBARANES", editEntregaAlbaranes->toMoney());
	getRecFacturaCompra()->setValue( "RESTO", editResto->toMoney());
	getRecFacturaCompra()->setValue( "ENTREGA", editEntrega->toMoney());
if(empresa::ModuleInstance->usaProyectos()){
	getRecFacturaCompra()->setValue( "PROYECTO_ID", getRecProyecto()->getRecordID() );
}
	getRecFacturaCompra()->setValue( "DESGLOSEIVA", editDesgloseIVA->toString());
	getRecFacturaCompra()->setValue( "DOCUMENTOPAGO", editDocumentoPago->toString());
	getRecFacturaCompra()->setValue( "FECHAPAGO", editFechaPago->toDate());
#ifdef HAVE_TESORERIAMODULE
if( ModuleInstance->getTesoreriaModule() ) {
	getRecFacturaCompra()->setValue( "CUENTAPAGO_ID", getRecCuentaPago()->getRecordID() );
}
#endif
#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	getRecFacturaCompra()->setValue( "CUENTAPAGO_ID", getRecCuentaPago()->getRecordID() );
}
#endif
	getRecFacturaCompra()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITFACTURACOMPRA_GATHER*/
    ModuleInstance->setWorkingDate( editFecha->toDate() );
}

void FrmEditFacturaCompra::scatterTipoDoc()
{
/*<<<<<FRMEDITFACTURACOMPRA_SCATTER_TIPODOC*/
	editTipoDocCodigo->setText( getRecTipoDoc()->getValue("CODIGO") );
	editTipoDocNombre->setText( getRecTipoDoc()->getValue("NOMBRE") );
/*>>>>>FRMEDITFACTURACOMPRA_SCATTER_TIPODOC*/
    if( editTipoDocCodigo->isJustEdited() ) {
        if( getRecTipoDoc()->getValue("IVADetallado").toInt() != 0 )
            comboIVADetallado->setCurrentItemByValue( getRecTipoDoc()->getValue("IVADetallado").toInt() );
        if( editFormaPagoCodigo->toString().isEmpty() )
            searchFormaPagoCodigo->setValue( getRecTipoDoc()->getValue("FORMAPAGO.CODIGO").toInt() );
    }
}

void FrmEditFacturaCompra::pushTipoDocCodigo_clicked()
{
    /*<<<<<FRMEDITFACTURACOMPRA_PUSH_TIPODOC_CODIGO_CLICKED*/
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
/*>>>>>FRMEDITFACTURACOMPRA_PUSH_TIPODOC_CODIGO_CLICKED*/
}

void FrmEditFacturaCompra::scatterProveedora()
{
    /*<<<<<FRMEDITFACTURACOMPRA_SCATTER_PROVEEDORA*/
	editProveedoraCodigo->setText( getRecProveedora()->getValue("CODIGO") );
	editProveedoraRazonSocial->setText( getRecProveedora()->getValue("RAZONSOCIAL") );
/*>>>>>FRMEDITFACTURACOMPRA_SCATTER_PROVEEDORA*/
    /*<<<<<FRMEDITFACTURACOMPRA_CABECERA_SCATTER_PROVEEDORA*/
	if( editProveedoraCodigo->isJustEdited() ) {
		if( getRecProveedora()->getRecFormaPago()->getValue("CODIGO").toInt() != 0 )
			searchFormaPagoCodigo->setValue( getRecProveedora()->getRecFormaPago()->getValue("CODIGO") );
		if( getRecProveedora()->getValue("IVADetallado").toInt() != 0 )
			comboIVADetallado->setCurrentItemByValue( getRecProveedora()->getValue("IVADetallado").toInt() );
	}
/*>>>>>FRMEDITFACTURACOMPRA_CABECERA_SCATTER_PROVEEDORA*/
}

void FrmEditFacturaCompra::pushProveedoraCodigo_clicked()
{
    /*<<<<<FRMEDITFACTURACOMPRA_PUSH_PROVEEDORA_CODIGO_CLICKED*/
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
					DBAPP->createClient( DBAPP->createEditForm(this, getRecProveedora(),
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
/*>>>>>FRMEDITFACTURACOMPRA_PUSH_PROVEEDORA_CODIGO_CLICKED*/
}

void FrmEditFacturaCompra::scatterFormaPago()
{
    /*<<<<<FRMEDITFACTURACOMPRA_SCATTER_FORMAPAGO*/
	editFormaPagoCodigo->setText( getRecFormaPago()->getValue("CODIGO") );
	editFormaPagoNombre->setText( getRecFormaPago()->getValue("NOMBRE") );
/*>>>>>FRMEDITFACTURACOMPRA_SCATTER_FORMAPAGO*/
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

void FrmEditFacturaCompra::pushFormaPagoCodigo_clicked()
{
    /*<<<<<FRMEDITFACTURACOMPRA_PUSH_FORMAPAGO_CODIGO_CLICKED*/
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
/*>>>>>FRMEDITFACTURACOMPRA_PUSH_FORMAPAGO_CODIGO_CLICKED*/
}

#ifdef HAVE_CONTABMODULE
void FrmEditFacturaCompra::scatterCuentaPago()
{
/*<<<<<FRMEDITFACTURACOMPRA_SCATTER_CUENTAPAGO*/
	editCuentaPagoCuenta->setText( getRecCuentaPago()->getValue("CUENTA") );
	editCuentaPagoDescripcion->setText( getRecCuentaPago()->getValue("DESCRIPCION") );
/*>>>>>FRMEDITFACTURACOMPRA_SCATTER_CUENTAPAGO*/
}
#elif defined (HAVE_TESORERIAMODULE)
void FrmEditFacturaCompra::scatterCuentaPago()
{
	editCuentaPagoCodigo->setText( getRecCuentaPago()->getValue("CODIGO") );
	editCuentaPagoNombre->setText( getRecCuentaPago()->getValue("NOMBRE") );
}
#endif

void FrmEditFacturaCompra::pushCuentaPagoCuenta_clicked()
{
#ifdef HAVE_CONTABMODULE
    /*<<<<<FRMEDITFACTURACOMPRA_PUSH_CUENTAPAGO_CUENTA_CLICKED*/
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
/*>>>>>FRMEDITFACTURACOMPRA_PUSH_CUENTAPAGO_CUENTA_CLICKED*/
#endif
}

void FrmEditFacturaCompra::pushCuentaPagoCodigo_clicked()
{
#ifdef HAVE_TESORERIAMODULE
/*<<<<<FRMEDITFACTURACOMPRA_PUSH_CUENTAPAGO_CODIGO_CLICKED*/
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
/*>>>>>FRMEDITFACTURACOMPRA_PUSH_CUENTAPAGO_CODIGO_CLICKED*/
#endif
}

void FrmEditFacturaCompra::scatterProyecto()
{
/*<<<<<FRMEDITFACTURACOMPRA_SCATTER_PROYECTO*/
	editProyectoCodigo->setText( getRecProyecto()->getValue("CODIGO") );
	editProyectoNombre->setText( getRecProyecto()->getValue("NOMBRE") );
/*>>>>>FRMEDITFACTURACOMPRA_SCATTER_PROYECTO*/
}
void FrmEditFacturaCompra::pushProyectoCodigo_clicked()
{
    /*<<<<<FRMEDITFACTURACOMPRA_PUSH_PROYECTO_CODIGO_CLICKED*/
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
/*>>>>>FRMEDITFACTURACOMPRA_PUSH_PROYECTO_CODIGO_CLICKED*/
}

void FrmEditFacturaCompra::scatterAgente()
{
/*<<<<<FRMEDITFACTURACOMPRA_SCATTER_AGENTE*/
	editAgenteCodigo->setText( getRecAgente()->getValue("CODIGO") );
	editAgenteRazonSocial->setText( getRecAgente()->getValue("RAZONSOCIAL") );
/*>>>>>FRMEDITFACTURACOMPRA_SCATTER_AGENTE*/
}
void FrmEditFacturaCompra::pushAgenteCodigo_clicked()
{
/*<<<<<FRMEDITFACTURACOMPRA_PUSH_AGENTE_CODIGO_CLICKED*/
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
/*>>>>>FRMEDITFACTURACOMPRA_PUSH_AGENTE_CODIGO_CLICKED*/
}


void FrmEditFacturaCompra::specialControlKeyPressed( QWidget *sender, char key )
{
    /*<<<<<FRMEDITFACTURACOMPRA_SPECIALACTION*/
	mControlKeyPressed = key;
	FrmEditRecMaster::specialControlKeyPressed(sender,key); // calls the behaviors
	if( sender == editTipoDocCodigo )
		pushTipoDocCodigo_clicked();
	if( sender == editProveedoraCodigo )
		pushProveedoraCodigo_clicked();
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
/*>>>>>FRMEDITFACTURACOMPRA_SPECIALACTION*/
}

void FrmEditFacturaCompra::validateFields( QWidget *sender, bool *isvalid, ValidResult *ir )
{
    /*<<<<<FRMEDITFACTURACOMPRA_VALIDATE*/
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
		getRecProveedora(), "CODIGO", "RAZONSOCIAL", Xtring::null, dbRecord::SeekCodeFlags( dbRecord::InsertIfNotFound )) )
		scatterProveedora();
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
/*>>>>>FRMEDITFACTURACOMPRA_VALIDATE*/

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
            if( editCuentaPagoCuenta->toString().isEmpty() && getRecFormaPago()->getValue("SUBCUENTAPAGO").toString() == Xtring::null) {
				validresult->addError( "No se ha introducido una cuenta de pago y la forma de pago tampoco la tiene definida.\nNo se generará el apunte del pago en tesorería.", "CUENTAPAGO_ID");
			}
#elif defined (HAVE_TESORERIAMODULE)
            if( editCuentaPagoCodigo->toString().isEmpty() && getRecFormaPago()->getValue("CUENTATESORERIA_ID").toInt() == 0) {
				validresult->addError( "No se ha introducido una cuenta de pago y la forma de pago tampoco la tiene definida.\nNo se generará el apunte del pago en tesorería.", "CUENTAPAGO_ID");
			}
#endif
		}
	}
	if( sender == editEntrega && editEntrega->isJustEdited() ) {	
        actTotales();
    }
/*<<<<<FRMEDITFACTURACOMPRA_CABECERA_VALIDATE*/
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
/*>>>>>FRMEDITFACTURACOMPRA_CABECERA_VALIDATE*/
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
                                                  DBAPP->getDatabase()->findFieldDefinition( "FACTURACOMPRA.NUMERO" )->getCaption().c_str()),
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
    if( !sender ) {
		if( mustRecargoEquivalencia() ) {
			if( comboIVADetallado->getCurrentItemValue() != FldIVADetallado::con_recargo 
				&& comboIVADetallado->getCurrentItemValue() != FldIVADetallado::sin_iva ) {
			validresult->addWarning( Xtring::printf(_("%s requiere recargo de equivalencia."),
				DBAPP->getTableDescSingular("EMPRESA", "la").c_str() ),
				"IVADETALLADO" );
			}
		}
	}
    if ( !ir ) {
        showValidMessages( isvalid, *validresult, sender );
        delete validresult;
    }
}

/*<<<<<FRMEDITFACTURACOMPRA_CABECERA_UPDATEFROMDETAILS*/
void FrmEditFacturaCompra::updateFromDetails( class FrmEditRecDetail *detailfrm )
{
	actTotales();
	numeraLineas();
/*>>>>>FRMEDITFACTURACOMPRA_CABECERA_UPDATEFROMDETAILS*/
}

/*<<<<<FRMEDITFACTURACOMPRA_CABECERA_NUMERALINEAS*/
void FrmEditFacturaCompra::numeraLineas()
{
	dbRecordList *reclst = getRecFacturaCompra()->getListFacturaCompraDet();
	for ( unsigned int i = 0; i < reclst->size(); i++ ) {
		RecFacturaCompraDet *detalle = static_cast<RecFacturaCompraDet *>( reclst->at( i ) );
		if( !detalle->isEmpty() ) // No numerar detalles vacíos 
			detalle->setValue( "NLINEA", i+1 );
	}
/*>>>>>FRMEDITFACTURACOMPRA_CABECERA_NUMERALINEAS*/
}

bool FrmEditFacturaCompra::canBeginEdit(EditMode newmode)
{
    mHasPagos = false;
    mOldTotal = getRecFacturaCompra()->getValue( "TOTAL" ).toMoney();
    if( newmode == DataTable::updating || newmode == DataTable::deleting ) {
        if( getRecFacturaCompra()->hasPagos( -1, pagos::PagosModule::ReciboPendiente ) ) {
            mHasPagos = true;
            if( newmode == DataTable::updating ) {
                return FrmBase::msgYesNo(this,
                                         Xtring::printf( _("%s tiene pagos ya pagados. No podrás modificar algunos datos\n¿Quieres continuar?"),
                                                 DBAPP->getTableDescSingular( pRecord->getTableName(), "Esta").c_str() ) );
            } else { // deleting
                FrmBase::msgError(this,
                                  Xtring::printf( _("%s tiene pagos ya pagados. Tienes que anular todos sus pagos antes de borrar."),
                                                  DBAPP->getTableDescSingular( pRecord->getTableName(), "Esta").c_str() ) );
                return false;
            }
        }
    }
    return FrmEditRecMaster::canBeginEdit(newmode);
}

void FrmEditFacturaCompra::slotPagar()
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
        Xtring docpago = _("Pago fact. " + editNumero->toString() );
        Money pago = editEntrega->toMoney();
        if( pago == 0.0 )
            pago = editResto->toMoney();
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
                                     cuentapago, docpago, 0, "FrmPagarFacturaCompra" );
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

/*<<<<<FRMEDITFACTURACOMPRA_CABECERA_ACTTOTALES*/
void FrmEditFacturaCompra::actTotales()
{
	getRecFacturaCompra()->gather( this );
	getRecFacturaCompra()->actTotales();
	getRecFacturaCompra()->scatter( this );
/*>>>>>FRMEDITFACTURACOMPRA_CABECERA_ACTTOTALES*/
}

/*<<<<<FRMEDITFACTURACOMPRA_FIN*/
} // namespace factu
} // namespace gong
/*>>>>>FRMEDITFACTURACOMPRA_FIN*/

#if 0
/*<<<<<FRMEDITFACTURACOMPRA_CABECERA_GENNUMDOC*/
void FrmEditFacturaCompra::genNumeroDocumento()
{
		editContador->setText( ModuleInstance->getMaxContador( "FACTURACOMPRA",
			getRecord()->getValue("EMPRESA_ID").toInt(),
			ModuleInstance->getEmpresaModule()->getEjercicio(),
			getRecTipoDoc()->getValue("SERIE").toString() ) );
		editContador->setJustEdited( true );
		validateFields( editContador, 0 );
/*>>>>>FRMEDITFACTURACOMPRA_CABECERA_GENNUMDOC*/
}
#endif
