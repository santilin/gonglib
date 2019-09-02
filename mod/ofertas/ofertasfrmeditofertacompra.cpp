/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de albaranes de compra
// FIELD Fecha date - cabecera
// FIELD TipoDoc_ID Reference(factu::TipoDoc,Codigo,Nombre) - cabecera
// FIELD Numero string - cabecera
// FIELD IVADetallado comboint - cabecera
// FIELD Proveedora_ID Reference(factu::Proveedora,Codigo,RazonSocial,dbRecord::InsertIfNotFound) - cabecera2
// FIELD FormaPago_ID Reference(pagos::FormaPago,Codigo,Nombre,dbRecord::InsertIfNotFound) - cabecera2
// FIELD Contador int - cabecera2
// FIELD OfertaCompraDet FrmEditRecDetail
// FIELD SumaImportes money - noaddrightSumaImportes
// FIELD DtoP100 double - noaddrightDtoP100
// FIELD Descuento money - noaddrightDescuento
// FIELD BaseImponible money - noaddrightBaseImponible
// FIELD Pagos money - noaddrightPagos
// FIELD RecargoEquivalencia money - noaddrightRecargoEquivalencia
// FIELD IVA money - noaddrightIVA
// FIELD Entrega money - noaddrightEntrega
// FIELD Resto money - noaddrightResto
// FIELD Total money - noaddrightTotal
// FIELD DesgloseIVA string tabPagos desgloseiva
// FIELD Proyecto_ID Reference(empresa::Proyecto,Codigo,Nombre) tabPagos proyecto if(empresa::ModuleInstance->usaProyectos())
// FIELD CuentaPago_ID Reference(CuentaOferta,Codigo,Nombre) tabPagos cuentapago MODULE_INCLUDED(Tesoreria)
// FIELD DocumentoPago string tabPagos pago
// FIELD FechaPago date tabPagos pago
// FIELD Notas text tabPagos notas
// TYPE FrmEditRecMaster ofertas::OfertaCompra Albaran Compra
/*>>>>>MODULE_INFO*/

/*<<<<<COPYLEFT*/
/** @file ofertasfrmeditofertacompra.cpp Fichero de edición de albaranes de compra
 * GongLib project. (L) Santiago Capel Torres, 2003-2011
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
/*>>>>>COPYLEFT*/

/*<<<<<FRMEDITOFERTACOMPRA_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "ofertasfrmeditofertacompra.h"
/*>>>>>FRMEDITOFERTACOMPRA_INCLUDES*/
#include <factufldivadetallado.h>
#include <factufrmpagar.h>
#include <facturecalbarancompra.h>
#include "ofertasmodule.h"

namespace gong {
namespace ofertas {

/*<<<<<FRMEDITOFERTACOMPRA_CONSTRUCTOR*/
FrmEditOfertaCompra::FrmEditOfertaCompra(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditOfertaCompra" );
/*>>>>>FRMEDITOFERTACOMPRA_CONSTRUCTOR*/
/*<<<<<FRMEDITOFERTACOMPRA_INIT_CONTROLS*/
	QHBoxLayout *cabeceraLayout = new QHBoxLayout(0, 0, 6, "cabeceraLayout");
	QHBoxLayout *cabecera2Layout = new QHBoxLayout(0, 0, 6, "cabecera2Layout");
	QHBoxLayout *ofertacompradetLayout = new QHBoxLayout(0, 0, 6, "ofertacompradetLayout");
	QHBoxLayout *rightSumaImportesLayout = new QHBoxLayout(0, 0, 6, "rightSumaImportesLayout");
	QHBoxLayout *rightDtoP100Layout = new QHBoxLayout(0, 0, 6, "rightDtoP100Layout");
	QHBoxLayout *rightDescuentoLayout = new QHBoxLayout(0, 0, 6, "rightDescuentoLayout");
	QHBoxLayout *rightBaseImponibleLayout = new QHBoxLayout(0, 0, 6, "rightBaseImponibleLayout");
	QHBoxLayout *rightPagosLayout = new QHBoxLayout(0, 0, 6, "rightPagosLayout");
	QHBoxLayout *rightRecargoEquivalenciaLayout = new QHBoxLayout(0, 0, 6, "rightRecargoEquivalenciaLayout");
	QHBoxLayout *rightIVALayout = new QHBoxLayout(0, 0, 6, "rightIVALayout");
	QHBoxLayout *rightEntregaLayout = new QHBoxLayout(0, 0, 6, "rightEntregaLayout");
	QHBoxLayout *rightRestoLayout = new QHBoxLayout(0, 0, 6, "rightRestoLayout");
	QHBoxLayout *rightTotalLayout = new QHBoxLayout(0, 0, 6, "rightTotalLayout");
	showTabs(true);
	QWidget *tabPagos = new QWidget( pTabWidget, "tabPagos" );
	QVBoxLayout *tabPagosLayout = new QVBoxLayout(tabPagos, 11, 6, "tabPagosLayout");
	QHBoxLayout *desgloseivaLayout = new QHBoxLayout(0, 0, 6, "desgloseivaLayout");
	QHBoxLayout *proyectoLayout = new QHBoxLayout(0, 0, 6, "proyectoLayout");
	QHBoxLayout *cuentapagoLayout = new QHBoxLayout(0, 0, 6, "cuentapagoLayout");
	QHBoxLayout *pagoLayout = new QHBoxLayout(0, 0, 6, "pagoLayout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
	editFecha = addEditField( pControlsFrame, "OFERTACOMPRA", "FECHA", cabeceraLayout );

	searchTipoDocCodigo = addSearchField( pControlsFrame, "TIPODOC_ID", "TIPODOC", "CODIGO", "NOMBRE", cabeceraLayout );
	pushTipoDocCodigo = searchTipoDocCodigo->getButton();
	connect( pushTipoDocCodigo, SIGNAL( clicked() ), this, SLOT( pushTipoDocCodigo_clicked() ) );
	editTipoDocCodigo = searchTipoDocCodigo->getEditCode();
	editTipoDocNombre = searchTipoDocCodigo->getEditDesc();
	editNumero = addEditField( pControlsFrame, "OFERTACOMPRA", "NUMERO", cabeceraLayout );
	comboIVADetallado = addComboIntField( pControlsFrame, "OFERTACOMPRA", "IVADETALLADO", cabeceraLayout );

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
	editContador = addEditField( pControlsFrame, "OFERTACOMPRA", "CONTADOR", cabecera2Layout );

// frmDetails: OfertaCompraDet
	QFrame *ofertacompradetFrame = new QFrame(this);
	ofertacompradetFrame->setFrameStyle(QFrame::Sunken | QFrame::Panel);
	ofertacompradetFrame->setLineWidth(4);
	QVBoxLayout *ofertacompradetFrameLayout = new QVBoxLayout(ofertacompradetFrame);
	RecOfertaCompraDet *ofertacompradet = getRecOfertaCompra()->getRecOfertaCompraDet(-1);
	dbViewDefinitionDict ofertacompradetviews;
	DBAPP->getDatabase()->getViewsForTable( ofertacompradet, ofertacompradetviews );
	dbRecordListDataModel *ofertacompradetdm =
		new dbRecordListDataModel(getRecOfertaCompra()->getListOfertaCompraDet(), ofertacompradet, ofertacompradetviews, "1=0");
	pFrmOfertaCompraDet = static_cast<FrmEditOfertaCompraDet *>(
		DBAPP->createEditDetailForm(this,
		-1, ofertacompradet, "OFERTACOMPRADET",ofertacompradetdm, DataTable::browsing,
		static_cast<dbApplication::EditFlags>(dbApplication::embedded + dbApplication::tableEditEvent ),
		pControlsFrame) );
	pFrmOfertaCompraDet->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding);
	ofertacompradetFrameLayout->addWidget( pFrmOfertaCompraDet );
	ofertacompradetLayout->addWidget(ofertacompradetFrame);
	editSumaImportes = addEditField( pControlsFrame, "OFERTACOMPRA", "SUMAIMPORTES", rightSumaImportesLayout );
	editDtoP100 = addEditField( pControlsFrame, "OFERTACOMPRA", "DTOP100", rightDtoP100Layout );
	editDescuento = addEditField( pControlsFrame, "OFERTACOMPRA", "DESCUENTO", rightDescuentoLayout );
	editBaseImponible = addEditField( pControlsFrame, "OFERTACOMPRA", "BASEIMPONIBLE", rightBaseImponibleLayout );
	editPagos = addEditField( pControlsFrame, "OFERTACOMPRA", "PAGOS", rightPagosLayout );
	editRecargoEquivalencia = addEditField( pControlsFrame, "OFERTACOMPRA", "RECARGOEQUIVALENCIA", rightRecargoEquivalenciaLayout );
	editIVA = addEditField( pControlsFrame, "OFERTACOMPRA", "IVA", rightIVALayout );
	editEntrega = addEditField( pControlsFrame, "OFERTACOMPRA", "ENTREGA", rightEntregaLayout );
	editResto = addEditField( pControlsFrame, "OFERTACOMPRA", "RESTO", rightRestoLayout );
	editTotal = addEditField( pControlsFrame, "OFERTACOMPRA", "TOTAL", rightTotalLayout );
	editDesgloseIVA = addEditField( tabPagos, "OFERTACOMPRA", "DESGLOSEIVA", desgloseivaLayout );

if(empresa::ModuleInstance->usaProyectos()){
	searchProyectoCodigo = addSearchField( tabPagos, "PROYECTO_ID", "PROYECTO", "CODIGO", "NOMBRE", proyectoLayout );
	pushProyectoCodigo = searchProyectoCodigo->getButton();
	connect( pushProyectoCodigo, SIGNAL( clicked() ), this, SLOT( pushProyectoCodigo_clicked() ) );
	editProyectoCodigo = searchProyectoCodigo->getEditCode();
	editProyectoNombre = searchProyectoCodigo->getEditDesc();
}

#ifdef HAVE_TESORERIAMODULE
if( ModuleInstance->getTesoreriaModule() ) {
	searchCuentaPagoCodigo = addSearchField( tabPagos, "CUENTAPAGO_ID", "CUENTAOFERTA", "CODIGO", "NOMBRE", cuentapagoLayout );
	pushCuentaPagoCodigo = searchCuentaPagoCodigo->getButton();
	connect( pushCuentaPagoCodigo, SIGNAL( clicked() ), this, SLOT( pushCuentaPagoCodigo_clicked() ) );
	editCuentaPagoCodigo = searchCuentaPagoCodigo->getEditCode();
	editCuentaPagoNombre = searchCuentaPagoCodigo->getEditDesc();
}
#endif
	editDocumentoPago = addEditField( tabPagos, "OFERTACOMPRA", "DOCUMENTOPAGO", pagoLayout );
	editFechaPago = addEditField( tabPagos, "OFERTACOMPRA", "FECHAPAGO", pagoLayout );
	editNotas = addTextField( tabPagos, "OFERTACOMPRA", "NOTAS", notasLayout );
	pControlsLayout->addLayout( cabeceraLayout );
	pControlsLayout->addLayout( cabecera2Layout );
	pControlsLayout->addLayout( ofertacompradetLayout );
	alignLayout( rightSumaImportesLayout, false);
	alignLayout( rightDtoP100Layout, false);
	alignLayout( rightDescuentoLayout, false);
	alignLayout( rightBaseImponibleLayout, false);
	alignLayout( rightPagosLayout, false);
	alignLayout( rightRecargoEquivalenciaLayout, false);
	alignLayout( rightIVALayout, false);
	alignLayout( rightEntregaLayout, false);
	alignLayout( rightRestoLayout, false);
	alignLayout( rightTotalLayout, false);
	tabPagosLayout->addLayout( desgloseivaLayout );
	tabPagosLayout->addLayout( proyectoLayout );
	tabPagosLayout->addLayout( cuentapagoLayout );
	tabPagosLayout->addLayout( pagoLayout );
	tabPagosLayout->addLayout( notasLayout );
/*>>>>>FRMEDITOFERTACOMPRA_INIT_CONTROLS*/
    pTabWidget->insertTab( tabPagos, toGUI( _( "&Extra" ) ) );

	QGridLayout *gridlayout = new QGridLayout();
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

	pushPagar = new PushButton( this, Xtring::null );
	connect( pushPagar, SIGNAL( clicked() ), this, SLOT( slotPagar() ) );
	gridlayout->addWidget( pushPagar, 2, 0 );

	pushPasarAAlbaran = new PushButton( this, "A albarán" );
	connect( pushPasarAAlbaran, SIGNAL( clicked() ), this, SLOT( slotPasarAAlbaran() ) );
	pButtonsLayout->addWidget( pushPasarAAlbaran );

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

void FrmEditOfertaCompra::scatterFields()
{
	if( isDuplicating() ) {
		getRecOfertaCompra()->setValue( "NUMERO", "" );
		getRecOfertaCompra()->setValue( "CONTADOR", 0 );
		getRecOfertaCompra()->setValue( "ENTREGA", Money(0.0) );
		getRecOfertaCompra()->setValue( "PAGOS", Money(0.0) );
		getRecOfertaCompra()->setValue( "RESTO", getRecOfertaCompra()->getValue( "TOTAL") );
		getRecOfertaCompra()->setValue( "FECHAPAGO", Date() );
		getRecOfertaCompra()->setValue( "DOCUMENTOPAGO", 0 );
	}
/*<<<<<FRMEDITOFERTACOMPRA_SCATTER*/
	editFecha->setText(getRecOfertaCompra()->getValue("FECHA").toDate());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editFecha;
	editNumero->setText(getRecOfertaCompra()->getValue("NUMERO").toString());
	comboIVADetallado->setCurrentItemByValue(getRecOfertaCompra()->getValue("IVADETALLADO").toInt());
	editContador->setText(getRecOfertaCompra()->getValue("CONTADOR").toInt());
	editSumaImportes->setText(getRecOfertaCompra()->getValue("SUMAIMPORTES").toMoney());
	editDtoP100->setText(getRecOfertaCompra()->getValue("DTOP100").toDouble());
	editDescuento->setText(getRecOfertaCompra()->getValue("DESCUENTO").toMoney());
	editBaseImponible->setText(getRecOfertaCompra()->getValue("BASEIMPONIBLE").toMoney());
	editPagos->setText(getRecOfertaCompra()->getValue("PAGOS").toMoney());
	editRecargoEquivalencia->setText(getRecOfertaCompra()->getValue("RECARGOEQUIVALENCIA").toMoney());
	editIVA->setText(getRecOfertaCompra()->getValue("IVA").toMoney());
	editEntrega->setText(getRecOfertaCompra()->getValue("ENTREGA").toMoney());
	editResto->setText(getRecOfertaCompra()->getValue("RESTO").toMoney());
	editTotal->setText(getRecOfertaCompra()->getValue("TOTAL").toMoney());
	editDesgloseIVA->setText(getRecOfertaCompra()->getValue("DESGLOSEIVA").toString());
	editDocumentoPago->setText(getRecOfertaCompra()->getValue("DOCUMENTOPAGO").toString());
	editFechaPago->setText(getRecOfertaCompra()->getValue("FECHAPAGO").toDate());
	editNotas->setText(getRecOfertaCompra()->getValue("NOTAS").toString());
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
/*>>>>>FRMEDITOFERTACOMPRA_SCATTER*/
	if( isInserting() ) {
		if( !isDuplicating() && editFecha->toDate().isNull() )
			editFecha->setText( Date::currentDate() );
		empresa::EmpresaModule *em = static_cast<empresa::EmpresaModule *>(DBAPP->findModule("empresa"));
		if( em && em->getRecEmpresa() ) {
			if( em->getRecEmpresa()->getValue("RECARGOEQUIVALENCIA").toBool() )
				comboIVADetallado->setCurrentItemByValue( factu::FldIVADetallado::con_recargo );
		}
		if( isDuplicating() ) {
			editTipoDocCodigo->setJustEdited( true );
			validateFields( editTipoDocCodigo, 0 );
		}
	} else if( isUpdating() ) {
		pFocusWidget = pFrmOfertaCompraDet;
	}
	pFrmOfertaCompraDet->addDetailIfNeeded();
	searchProveedoraCodigo->setMustBeReadOnly( mHasPagos );
	searchFormaPagoCodigo->setMustBeReadOnly( mHasPagos );
	editFecha->setMustBeReadOnly( mHasPagos );
	editDtoP100->setMustBeReadOnly( mHasPagos );
	editTotal->setMustBeReadOnly( mHasPagos );
	editEntrega->setMustBeReadOnly( mHasPagos );
	pushPagar->setVisible( !mHasPagos );
	scatterFormaPago(); // Para cambiar el texto del botón pagar después de actualizar los totales
	validateFields( comboIVADetallado, 0 ); // Para mostrar u ocultar el recargo de equivalencia
}

void FrmEditOfertaCompra::gatherFields()
{
/*<<<<<FRMEDITOFERTACOMPRA_GATHER*/
	getRecOfertaCompra()->setValue( "FECHA", editFecha->toDate());
	getRecOfertaCompra()->setValue( "TIPODOC_ID", getRecTipoDoc()->getRecordID() );
	getRecOfertaCompra()->setValue( "NUMERO", editNumero->toString());
	getRecOfertaCompra()->setValue( "IVADETALLADO", comboIVADetallado->getCurrentItemValue());
	getRecOfertaCompra()->setValue( "PROVEEDORA_ID", getRecProveedora()->getRecordID() );
	getRecOfertaCompra()->setValue( "FORMAPAGO_ID", getRecFormaPago()->getRecordID() );
	getRecOfertaCompra()->setValue( "CONTADOR", editContador->toInt());
	getRecOfertaCompra()->setValue( "SUMAIMPORTES", editSumaImportes->toMoney());
	getRecOfertaCompra()->setValue( "DTOP100", editDtoP100->toDouble());
	getRecOfertaCompra()->setValue( "DESCUENTO", editDescuento->toMoney());
	getRecOfertaCompra()->setValue( "BASEIMPONIBLE", editBaseImponible->toMoney());
	getRecOfertaCompra()->setValue( "PAGOS", editPagos->toMoney());
	getRecOfertaCompra()->setValue( "RECARGOEQUIVALENCIA", editRecargoEquivalencia->toMoney());
	getRecOfertaCompra()->setValue( "IVA", editIVA->toMoney());
	getRecOfertaCompra()->setValue( "ENTREGA", editEntrega->toMoney());
	getRecOfertaCompra()->setValue( "RESTO", editResto->toMoney());
	getRecOfertaCompra()->setValue( "TOTAL", editTotal->toMoney());
	getRecOfertaCompra()->setValue( "DESGLOSEIVA", editDesgloseIVA->toString());
if(empresa::ModuleInstance->usaProyectos()){
	getRecOfertaCompra()->setValue( "PROYECTO_ID", getRecProyecto()->getRecordID() );
}
#ifdef HAVE_TESORERIAMODULE
if( ModuleInstance->getTesoreriaModule() ) {
	getRecOfertaCompra()->setValue( "CUENTAPAGO_ID", getRecCuentaPago()->getRecordID() );
}
#endif
	getRecOfertaCompra()->setValue( "DOCUMENTOPAGO", editDocumentoPago->toString());
	getRecOfertaCompra()->setValue( "FECHAPAGO", editFechaPago->toDate());
	getRecOfertaCompra()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITOFERTACOMPRA_GATHER*/
}

void FrmEditOfertaCompra::scatterTipoDoc()
{
/*<<<<<FRMEDITOFERTACOMPRA_SCATTER_TIPODOC*/
	editTipoDocCodigo->setText( getRecTipoDoc()->getValue("CODIGO") );
	editTipoDocNombre->setText( getRecTipoDoc()->getValue("NOMBRE") );
/*>>>>>FRMEDITOFERTACOMPRA_SCATTER_TIPODOC*/
	if( editTipoDocCodigo->isJustEdited() ) {
		if( getRecTipoDoc()->getValue("IVADetallado").toInt() != 0 )
			comboIVADetallado->setCurrentItemByValue( getRecTipoDoc()->getValue("IVADetallado").toInt() );
		if( editFormaPagoCodigo->toString().isEmpty() )
			searchFormaPagoCodigo->setValue( getRecTipoDoc()->getValue("FORMAPAGO.CODIGO").toInt() );
	}
}

void FrmEditOfertaCompra::pushTipoDocCodigo_clicked()
{
/*<<<<<FRMEDITOFERTACOMPRA_PUSH_TIPODOC_CODIGO_CLICKED*/
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
				factu::RecTipoDoc *tmprec = static_cast<factu::RecTipoDoc *>(DBAPP->createRecord( "TipoDoc" ));
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
/*>>>>>FRMEDITOFERTACOMPRA_PUSH_TIPODOC_CODIGO_CLICKED*/
}

void FrmEditOfertaCompra::scatterProveedora()
{
/*<<<<<FRMEDITOFERTACOMPRA_SCATTER_PROVEEDORA*/
	editProveedoraCodigo->setText( getRecProveedora()->getValue("CODIGO") );
	editProveedoraRazonSocial->setText( getRecProveedora()->getValue("RAZONSOCIAL") );
/*>>>>>FRMEDITOFERTACOMPRA_SCATTER_PROVEEDORA*/
/*<<<<<FRMEDITOFERTACOMPRA_CABECERA_SCATTER_PROVEEDORA*/
	if( editProveedoraCodigo->isJustEdited() ) {
		if( getRecProveedora()->getRecFormaPago()->getValue("CODIGO").toInt() != 0 )
			searchFormaPagoCodigo->setValue( getRecProveedora()->getRecFormaPago()->getValue("CODIGO") );
		if( getRecProveedora()->getValue("IVADetallado").toInt() != 0 )
			comboIVADetallado->setCurrentItemByValue( getRecProveedora()->getValue("IVADetallado").toInt() );
	}
/*>>>>>FRMEDITOFERTACOMPRA_CABECERA_SCATTER_PROVEEDORA*/
}

void FrmEditOfertaCompra::pushProveedoraCodigo_clicked()
{
/*<<<<<FRMEDITOFERTACOMPRA_PUSH_PROVEEDORA_CODIGO_CLICKED*/
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
				factu::RecProveedora *tmprec = static_cast<factu::RecProveedora *>(DBAPP->createRecord( "Proveedora" ));
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
/*>>>>>FRMEDITOFERTACOMPRA_PUSH_PROVEEDORA_CODIGO_CLICKED*/
}

void FrmEditOfertaCompra::scatterFormaPago()
{
/*<<<<<FRMEDITOFERTACOMPRA_SCATTER_FORMAPAGO*/
	editFormaPagoCodigo->setText( getRecFormaPago()->getValue("CODIGO") );
	editFormaPagoNombre->setText( getRecFormaPago()->getValue("NOMBRE") );
/*>>>>>FRMEDITOFERTACOMPRA_SCATTER_FORMAPAGO*/
	if( getRecFormaPago()->getValue( "TIPOFORMAPAGO" ).toInt() == pagos::RecFormaPago::Contado
		|| getRecFormaPago()->getValue( "TIPOFORMAPAGO" ).toInt() == pagos::RecFormaPago::SeIgnora ) {
		pushPagar->setVisible( false );
		editEntrega->setReadOnly( true );
	} else {
		pushPagar->setVisible( true );
		editEntrega->setReadOnly( false );
	}
	actTotales();
	if( editEntrega->toDouble() != 0.0 && editTotal->toDouble() != 0.0 )
		pushPagar->setText( _("&Borrar entrega") );
	else
		pushPagar->setText( _("&Entrega") );
}

void FrmEditOfertaCompra::pushFormaPagoCodigo_clicked()
{
/*<<<<<FRMEDITOFERTACOMPRA_PUSH_FORMAPAGO_CODIGO_CLICKED*/
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
/*>>>>>FRMEDITOFERTACOMPRA_PUSH_FORMAPAGO_CODIGO_CLICKED*/
}

void FrmEditOfertaCompra::scatterProyecto()
{
/*<<<<<FRMEDITOFERTACOMPRA_SCATTER_PROYECTO*/
	editProyectoCodigo->setText( getRecProyecto()->getValue("CODIGO") );
	editProyectoNombre->setText( getRecProyecto()->getValue("NOMBRE") );
/*>>>>>FRMEDITOFERTACOMPRA_SCATTER_PROYECTO*/
}
void FrmEditOfertaCompra::pushProyectoCodigo_clicked()
{
/*<<<<<FRMEDITOFERTACOMPRA_PUSH_PROYECTO_CODIGO_CLICKED*/
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
/*>>>>>FRMEDITOFERTACOMPRA_PUSH_PROYECTO_CODIGO_CLICKED*/
}

#ifdef HAVE_TESORERIAMODULE
void FrmEditOfertaCompra::scatterCuentaPago()
{
/*<<<<<FRMEDITOFERTACOMPRA_SCATTER_CUENTAPAGO*/
	editCuentaPagoCodigo->setText( getRecCuentaPago()->getValue("CODIGO") );
	editCuentaPagoNombre->setText( getRecCuentaPago()->getValue("NOMBRE") );
/*>>>>>FRMEDITOFERTACOMPRA_SCATTER_CUENTAPAGO*/
}
#endif

void FrmEditOfertaCompra::pushCuentaPagoCodigo_clicked()
{
#ifdef HAVE_TESORERIAMODULE
/*<<<<<FRMEDITOFERTACOMPRA_PUSH_CUENTAPAGO_CODIGO_CLICKED*/
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
/*>>>>>FRMEDITOFERTACOMPRA_PUSH_CUENTAPAGO_CODIGO_CLICKED*/
#endif
}


void FrmEditOfertaCompra::specialControlKeyPressed(QWidget *sender, char key)
{
/*<<<<<FRMEDITOFERTACOMPRA_SPECIALACTION*/
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
	mControlKeyPressed = '\0';
/*>>>>>FRMEDITOFERTACOMPRA_SPECIALACTION*/
}

void FrmEditOfertaCompra::validateFields(QWidget *sender, bool *isvalid, ValidResult *ir)
{
/*<<<<<FRMEDITOFERTACOMPRA_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	if( !sender && !pRecord->validate( ValidResult::editing) ) {
		validresult->append( pRecord->getErrors() );
		*isvalid = false;
	}
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
/*>>>>>FRMEDITOFERTACOMPRA_VALIDATE*/
	if( sender == editEntrega && editEntrega->isJustEdited() ) {
		if( editEntrega->toDouble() == 0.0 ) {
			editFechaPago->setText( Xtring::null );
			editDocumentoPago->setText( "" );
		} else {
			if( editFechaPago->toString().isEmpty() ) {
				editFechaPago->setText( editFecha->toDate() );
				editDocumentoPago->setText( _("Pago ofe. " + editNumero->toString() ) );
			}
		}
		actTotales();
	}
/*<<<<<FRMEDITOFERTACOMPRA_CABECERA_VALIDATE*/
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
/*>>>>>FRMEDITOFERTACOMPRA_CABECERA_VALIDATE*/
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
	if( mHasPagos && mOldTotal != editTotal->toMoney() ) {
		validresult->addError( Xtring::printf( _("No puedes modificar el total de %s porque tiene pagos ya pagados"),
			DBAPP->getTableDescSingular( pRecord->getTableName(), "esta" ).c_str() ), "TIPODOC_ID" );
		*isvalid = false;
	}
	if( !ir ) {
		showValidMessages(isvalid, *validresult, sender);
		delete validresult;
	}
}

/*<<<<<FRMEDITOFERTACOMPRA_CABECERA_UPDATEFROMDETAILS*/
void FrmEditOfertaCompra::updateFromDetails( class FrmEditRecDetail *detailfrm )
{
	actTotales();
	numeraLineas();
/*>>>>>FRMEDITOFERTACOMPRA_CABECERA_UPDATEFROMDETAILS*/
}

/*<<<<<FRMEDITOFERTACOMPRA_CABECERA_NUMERALINEAS*/
void FrmEditOfertaCompra::numeraLineas()
{
	dbRecordList *reclst = getRecOfertaCompra()->getListOfertaCompraDet();
	for ( unsigned int i = 0; i < reclst->size(); i++ ) {
		RecOfertaCompraDet *detalle = static_cast<RecOfertaCompraDet *>( reclst->at( i ) );
		if( !detalle->isEmpty() ) // No numerar detalles vacíos 
			detalle->setValue( "NLINEA", i+1 );
	}
/*>>>>>FRMEDITOFERTACOMPRA_CABECERA_NUMERALINEAS*/
}

bool FrmEditOfertaCompra::canBeginEdit(DataTable::EditMode newmode)
{
	mHasPagos = false;
	mOldTotal = getRecOfertaCompra()->getValue( "TOTAL" ).toMoney();
	if( newmode == DataTable::updating || newmode == DataTable::deleting ) {
		if( getRecOfertaCompra()->hasPagos( -1, pagos::PagosModule::ReciboPendiente ) ) {
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

void FrmEditOfertaCompra::slotPagar()
{
	if( editEntrega->toDouble() != 0 ) {
		editResto->setText( editResto->toMoney() + editEntrega->toMoney() );
		editEntrega->setText( 0.0 );
		editFechaPago->setText( "" );
		editDocumentoPago->setText( "" );
		pushPagar->setText( _("&Entrega") );
	} else {
		Date fechapago = editFechaPago->toDate();
		if( fechapago.isNull() )
			fechapago = editFecha->toDate();
		Xtring docpago = _("Pago alb. " + editNumero->toString() );
		Money pago = editEntrega->toMoney();
		if( pago == 0.0 )
			pago = editTotal->toMoney();
		bool has_contab = false;
		Xtring cuentapago;
		if( cuentapago.isEmpty() )
			cuentapago = empresa::ModuleInstance->getRecEmpresa()->getValue("CUENTACAJA").toString();
		factu::FrmPagar *pr = new factu::FrmPagar( has_contab, pago, fechapago, cuentapago, docpago, 0, "FrmPagar" );
		pr->showModalFor( this, true, true );
		if( !pr->wasCancelled() ) {
			editEntrega->setText( pr->getImporte() );
			editResto->setText( editResto->toMoney() - pr->getImporte() );
			editFechaPago->setText( pr->getFechaPago() );
			editDocumentoPago->setText( pr->getDocumentoPago() );
			if( editTotal->toDouble() == 0.0 )
				pushPagar->setText( _("&Borrar entrega") );
		}
		delete pr;
	}
}

/*<<<<<FRMEDITOFERTACOMPRA_CABECERA_GENNUMDOC$s/ModuleInstance->/ModuleInstance->getFactuModule()->/$*/
void FrmEditOfertaCompra::genNumeroDocumento()
{
		editContador->setText( ModuleInstance->getFactuModule()->getMaxContador( "OFERTACOMPRA",
			getRecord()->getValue("EMPRESA_ID").toInt(),
			ModuleInstance->getFactuModule()->getEmpresaModule()->getEjercicio(),
			getRecTipoDoc()->getValue("SERIE").toString() ) );
		editContador->setJustEdited( true );
		validateFields( editContador, 0 );
/*>>>>>FRMEDITOFERTACOMPRA_CABECERA_GENNUMDOC*/
}

/*<<<<<FRMEDITOFERTACOMPRA_CABECERA_ACTTOTALES*/
void FrmEditOfertaCompra::actTotales()
{
	getRecOfertaCompra()->gather( this );
	getRecOfertaCompra()->actTotales();
	getRecOfertaCompra()->scatter( this );
/*>>>>>FRMEDITOFERTACOMPRA_CABECERA_ACTTOTALES*/
}

void FrmEditOfertaCompra::slotPasarAAlbaran()
{
	dbRecordID id = getTableRecordID();
	if( id == 0 )
		return;
	RecOfertaCompra *orig_record = static_cast<RecOfertaCompra*>(DBAPP->createRecord("OFERTACOMPRA"));
	if( !orig_record->read(id) )
		return;
	DBAPP->waitCursor(true);
	dbRecordList *detalles = orig_record->getListOfertaCompraDet();
	factu::RecAlbaranCompra *new_albaran_compra = static_cast<factu::RecAlbaranCompra *>( DBAPP->createRecord("ALBARANCOMPRA") );
	new_albaran_compra->copyRecord( orig_record );
	new_albaran_compra->setValue("CONTADOR", 0);
	for( dbRecordList::const_iterator it = detalles->begin(); it != detalles->end(); ++it ) {
		dbRecord *detalle = *it;
		factu::RecAlbaranCompraDet *new_detalle = static_cast<factu::RecAlbaranCompraDet *>( DBAPP->createRecord("ALBARANCOMPRADET") );
		new_detalle->copyRecord( detalle );
		new_detalle->setNew( true );
		new_albaran_compra->getListAlbaranCompraDet()->addRecord( new_detalle );
	}
	FrmEditRec *frmeditalbaran = DBAPP->createEditForm(0, new_albaran_compra,
														  0, DataTable::inserting, dbApplication::simpleEdition);
	DBAPP->resetCursor();
	if( !frmeditalbaran->showAndSave() )
		frmeditalbaran->showModalFor( 0, true, true );
	if( frmeditalbaran->isSaved() ) {
		ofertas::ModuleInstance->copiaCobros(orig_record, new_albaran_compra);
		Xtring rec_desc = DBAPP->getTableDescSingular( orig_record->getTableName(), "la" );
		if( FrmBase::msgYesNo("Copiar a albarán",
									Xtring::printf("%s se ha guardado perfectamente.\n¿Quieres borrar la oferta de compra original?", rec_desc.c_str())) ) {
			if( orig_record->remove() ) {
				DBAPP->showOSD(Xtring::printf("%s original se ha borrado correctamente", rec_desc.c_str()),"");
				refresh();
			} else {
				FrmBase::msgError(Xtring::printf("Error borrando %s original", rec_desc.c_str()), "");
			}
		}
	}
	delete frmeditalbaran;
	delete new_albaran_compra;
	delete orig_record;
}

void FrmEditOfertaCompra::updateStatus(bool callbehaviors)
{
	FrmEditRecMaster::updateStatus(callbehaviors);
	pushPasarAAlbaran->hide();
	if( !isEditing() && getDataTable()->getDataModel()->getRowCount()> 0) {
		pushPasarAAlbaran->show();
	}
}



/*<<<<<FRMEDITOFERTACOMPRA_FIN*/
} // namespace ofertas
} // namespace gong
/*>>>>>FRMEDITOFERTACOMPRA_FIN*/
