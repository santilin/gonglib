/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de albaranes de venta
// FIELD Fecha date - cabecera
// FIELD TipoDoc_ID Reference(factu::TipoDoc,Codigo,Nombre) - cabecera
// FIELD Contador int - cabecera
// FIELD Numero string - cabecera
// FIELD Cliente_ID Reference(factu::Cliente,Codigo,RazonSocial,dbRecord::InsertIfNotFound) - cabecera2
// FIELD IVADetallado comboint - cabecera2
// FIELD FormaPago_ID Reference(pagos::FormaPago,Codigo,Nombre,dbRecord::InsertIfNotFound) - cabecera3
// FIELD Agente_ID Reference(factu::Agente,Codigo,RazonSocial,dbRecord::InsertIfNotFound) - cabecera3
// FIELD OfertaVentaDet FrmEditRecDetail
// FIELD SumaImportes money - noaddrightSumaImportes
// FIELD DtoP100 double - noaddrightDtoP100
// FIELD Descuento money - noaddrightDescuento
// FIELD BaseImponible money - noaddrightBaseImponible
// FIELD Cobros money - noaddrightCobros
// FIELD RecargoEquivalencia money - noaddrightRecargoEquivalencia
// FIELD IVA money - noaddrightIVA
// FIELD Entrega money - noaddrightEntrega
// FIELD Resto money - noaddrightResto
// FIELD Total money - noaddrightTotal
// FIELD Proyecto_ID Reference(empresa::Proyecto,Codigo,Nombre) tabPagos proyecto if(empresa::ModuleInstance->usaProyectos())
// FIELD DesgloseIVA string tabPagos desgloseiva
// FIELD DocumentoPago string tabPagos pago
// FIELD FechaPago date tabPagos pago
// FIELD CuentaPago_ID Reference(CuentaOferta,Codigo,Nombre) tabPagos cuentapago MODULE_INCLUDED(Tesoreria)
// FIELD Notas text tabPagos notas
// TYPE FrmEditRecMaster ofertas::OfertaVenta Albaran Venta
/*>>>>>MODULE_INFO*/

/*<<<<<COPYLEFT*/
/** @file ofertasfrmeditalbaranventa.cpp Fichero de edición de albaranes de venta
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

/*<<<<<FRMEDITOFERTAVENTA_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "ofertasfrmeditofertaventa.h"
/*>>>>>FRMEDITOFERTAVENTA_INCLUDES*/
#include <facturectipodoc.h>
#include <factufrmpagar.h>
#include <factufldivadetallado.h>
#include <facturecalbaranventa.h>
#include <ofertasmodule.h>

namespace gong {
namespace ofertas {

/*<<<<<FRMEDITOFERTAVENTA_CONSTRUCTOR*/
FrmEditOfertaVenta::FrmEditOfertaVenta(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditOfertaVenta" );
/*>>>>>FRMEDITOFERTAVENTA_CONSTRUCTOR*/
/*<<<<<FRMEDITOFERTAVENTA_INIT_CONTROLS*/
	QHBoxLayout *cabeceraLayout = new QHBoxLayout(0, 0, 6, "cabeceraLayout");
	QHBoxLayout *cabecera2Layout = new QHBoxLayout(0, 0, 6, "cabecera2Layout");
	QHBoxLayout *cabecera3Layout = new QHBoxLayout(0, 0, 6, "cabecera3Layout");
	QHBoxLayout *ofertaventadetLayout = new QHBoxLayout(0, 0, 6, "ofertaventadetLayout");
	QHBoxLayout *rightSumaImportesLayout = new QHBoxLayout(0, 0, 6, "rightSumaImportesLayout");
	QHBoxLayout *rightDtoP100Layout = new QHBoxLayout(0, 0, 6, "rightDtoP100Layout");
	QHBoxLayout *rightDescuentoLayout = new QHBoxLayout(0, 0, 6, "rightDescuentoLayout");
	QHBoxLayout *rightBaseImponibleLayout = new QHBoxLayout(0, 0, 6, "rightBaseImponibleLayout");
	QHBoxLayout *rightCobrosLayout = new QHBoxLayout(0, 0, 6, "rightCobrosLayout");
	QHBoxLayout *rightRecargoEquivalenciaLayout = new QHBoxLayout(0, 0, 6, "rightRecargoEquivalenciaLayout");
	QHBoxLayout *rightIVALayout = new QHBoxLayout(0, 0, 6, "rightIVALayout");
	QHBoxLayout *rightEntregaLayout = new QHBoxLayout(0, 0, 6, "rightEntregaLayout");
	QHBoxLayout *rightRestoLayout = new QHBoxLayout(0, 0, 6, "rightRestoLayout");
	QHBoxLayout *rightTotalLayout = new QHBoxLayout(0, 0, 6, "rightTotalLayout");
	showTabs(true);
	QWidget *tabPagos = new QWidget( pTabWidget, "tabPagos" );
	QVBoxLayout *tabPagosLayout = new QVBoxLayout(tabPagos, 11, 6, "tabPagosLayout");
	QHBoxLayout *proyectoLayout = new QHBoxLayout(0, 0, 6, "proyectoLayout");
	QHBoxLayout *desgloseivaLayout = new QHBoxLayout(0, 0, 6, "desgloseivaLayout");
	QHBoxLayout *pagoLayout = new QHBoxLayout(0, 0, 6, "pagoLayout");
	QHBoxLayout *cuentapagoLayout = new QHBoxLayout(0, 0, 6, "cuentapagoLayout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
	editFecha = addEditField( pControlsFrame, "OFERTAVENTA", "FECHA", cabeceraLayout );

	searchTipoDocCodigo = addSearchField( pControlsFrame, "TIPODOC_ID", "TIPODOC", "CODIGO", "NOMBRE", cabeceraLayout );
	pushTipoDocCodigo = searchTipoDocCodigo->getButton();
	connect( pushTipoDocCodigo, SIGNAL( clicked() ), this, SLOT( pushTipoDocCodigo_clicked() ) );
	editTipoDocCodigo = searchTipoDocCodigo->getEditCode();
	editTipoDocNombre = searchTipoDocCodigo->getEditDesc();
	editContador = addEditField( pControlsFrame, "OFERTAVENTA", "CONTADOR", cabeceraLayout );
	editNumero = addEditField( pControlsFrame, "OFERTAVENTA", "NUMERO", cabeceraLayout );

	searchClienteCodigo = addSearchField( pControlsFrame, "CLIENTE_ID", "CLIENTE", "CODIGO", "RAZONSOCIAL", cabecera2Layout );
	pushClienteCodigo = searchClienteCodigo->getButton();
	connect( pushClienteCodigo, SIGNAL( clicked() ), this, SLOT( pushClienteCodigo_clicked() ) );
	editClienteCodigo = searchClienteCodigo->getEditCode();
	editClienteRazonSocial = searchClienteCodigo->getEditDesc();
	comboIVADetallado = addComboIntField( pControlsFrame, "OFERTAVENTA", "IVADETALLADO", cabecera2Layout );

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

// frmDetails: OfertaVentaDet
	QFrame *ofertaventadetFrame = new QFrame(this);
	ofertaventadetFrame->setFrameStyle(QFrame::Sunken | QFrame::Panel);
	ofertaventadetFrame->setLineWidth(4);
	QVBoxLayout *ofertaventadetFrameLayout = new QVBoxLayout(ofertaventadetFrame);
	RecOfertaVentaDet *ofertaventadet = getRecOfertaVenta()->getRecOfertaVentaDet(-1);
	dbViewDefinitionDict ofertaventadetviews;
	DBAPP->getDatabase()->getViewsForTable( ofertaventadet, ofertaventadetviews );
	dbRecordListDataModel *ofertaventadetdm =
		new dbRecordListDataModel(getRecOfertaVenta()->getListOfertaVentaDet(), ofertaventadet, ofertaventadetviews, "1=0");
	pFrmOfertaVentaDet = static_cast<FrmEditOfertaVentaDet *>(
		DBAPP->createEditDetailForm(this,
		-1, ofertaventadet, "OFERTAVENTADET",ofertaventadetdm, DataTable::browsing,
		static_cast<dbApplication::EditFlags>(dbApplication::embedded + dbApplication::tableEditEvent ),
		pControlsFrame) );
	pFrmOfertaVentaDet->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding);
	ofertaventadetFrameLayout->addWidget( pFrmOfertaVentaDet );
	ofertaventadetLayout->addWidget(ofertaventadetFrame);
	editSumaImportes = addEditField( pControlsFrame, "OFERTAVENTA", "SUMAIMPORTES", rightSumaImportesLayout );
	editDtoP100 = addEditField( pControlsFrame, "OFERTAVENTA", "DTOP100", rightDtoP100Layout );
	editDescuento = addEditField( pControlsFrame, "OFERTAVENTA", "DESCUENTO", rightDescuentoLayout );
	editBaseImponible = addEditField( pControlsFrame, "OFERTAVENTA", "BASEIMPONIBLE", rightBaseImponibleLayout );
	editCobros = addEditField( pControlsFrame, "OFERTAVENTA", "COBROS", rightCobrosLayout );
	editRecargoEquivalencia = addEditField( pControlsFrame, "OFERTAVENTA", "RECARGOEQUIVALENCIA", rightRecargoEquivalenciaLayout );
	editIVA = addEditField( pControlsFrame, "OFERTAVENTA", "IVA", rightIVALayout );
	editEntrega = addEditField( pControlsFrame, "OFERTAVENTA", "ENTREGA", rightEntregaLayout );
	editResto = addEditField( pControlsFrame, "OFERTAVENTA", "RESTO", rightRestoLayout );
	editTotal = addEditField( pControlsFrame, "OFERTAVENTA", "TOTAL", rightTotalLayout );

if(empresa::ModuleInstance->usaProyectos()){
	searchProyectoCodigo = addSearchField( tabPagos, "PROYECTO_ID", "PROYECTO", "CODIGO", "NOMBRE", proyectoLayout );
	pushProyectoCodigo = searchProyectoCodigo->getButton();
	connect( pushProyectoCodigo, SIGNAL( clicked() ), this, SLOT( pushProyectoCodigo_clicked() ) );
	editProyectoCodigo = searchProyectoCodigo->getEditCode();
	editProyectoNombre = searchProyectoCodigo->getEditDesc();
}
	editDesgloseIVA = addEditField( tabPagos, "OFERTAVENTA", "DESGLOSEIVA", desgloseivaLayout );
	editDocumentoPago = addEditField( tabPagos, "OFERTAVENTA", "DOCUMENTOPAGO", pagoLayout );
	editFechaPago = addEditField( tabPagos, "OFERTAVENTA", "FECHAPAGO", pagoLayout );

#ifdef HAVE_TESORERIAMODULE
if( ModuleInstance->getTesoreriaModule() ) {
	searchCuentaPagoCodigo = addSearchField( tabPagos, "CUENTAPAGO_ID", "CUENTAOFERTA", "CODIGO", "NOMBRE", cuentapagoLayout );
	pushCuentaPagoCodigo = searchCuentaPagoCodigo->getButton();
	connect( pushCuentaPagoCodigo, SIGNAL( clicked() ), this, SLOT( pushCuentaPagoCodigo_clicked() ) );
	editCuentaPagoCodigo = searchCuentaPagoCodigo->getEditCode();
	editCuentaPagoNombre = searchCuentaPagoCodigo->getEditDesc();
}
#endif
	editNotas = addTextField( tabPagos, "OFERTAVENTA", "NOTAS", notasLayout );
	pControlsLayout->addLayout( cabeceraLayout );
	pControlsLayout->addLayout( cabecera2Layout );
	pControlsLayout->addLayout( cabecera3Layout );
	pControlsLayout->addLayout( ofertaventadetLayout );
	alignLayout( rightSumaImportesLayout, false);
	alignLayout( rightDtoP100Layout, false);
	alignLayout( rightDescuentoLayout, false);
	alignLayout( rightBaseImponibleLayout, false);
	alignLayout( rightCobrosLayout, false);
	alignLayout( rightRecargoEquivalenciaLayout, false);
	alignLayout( rightIVALayout, false);
	alignLayout( rightEntregaLayout, false);
	alignLayout( rightRestoLayout, false);
	alignLayout( rightTotalLayout, false);
	tabPagosLayout->addLayout( proyectoLayout );
	tabPagosLayout->addLayout( desgloseivaLayout );
	tabPagosLayout->addLayout( pagoLayout );
	tabPagosLayout->addLayout( cuentapagoLayout );
	tabPagosLayout->addLayout( notasLayout );
/*>>>>>FRMEDITOFERTAVENTA_INIT_CONTROLS*/
    pTabWidget->insertTab( tabPagos, toGUI( _( "&Extra" ) ) );

	QGridLayout *gridlayout = new QGridLayout();
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

	pushCobrar = new PushButton( this, Xtring::null );
	connect( pushCobrar, SIGNAL( clicked() ), this, SLOT( slotCobrar() ) );
	gridlayout->addWidget( pushCobrar, 2, 0 );

	pushPasarAAlbaran = new PushButton( this, "A albarán" );
	connect( pushPasarAAlbaran, SIGNAL( clicked() ), this, SLOT( slotPasarAAlbaran() ) );
	pButtonsLayout->addWidget( pushPasarAAlbaran );

	editNumero->setMustBeReadOnly( true );
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

void FrmEditOfertaVenta::scatterFields()
{
	if( isDuplicating() ) {
		getRecOfertaVenta()->setValue( "NUMERO", "" );
		getRecOfertaVenta()->setValue( "CONTADOR", 0 );
		getRecOfertaVenta()->setValue( "ENTREGA", Money(0.0) );
		getRecOfertaVenta()->setValue( "COBROS", Money(0.0) );
		getRecOfertaVenta()->setValue( "RESTO", getRecOfertaVenta()->getValue( "TOTAL") );
		getRecOfertaVenta()->setValue( "FECHAPAGO", Date() );
		getRecOfertaVenta()->setValue( "DOCUMENTOPAGO", Xtring::null );
	}
/*<<<<<FRMEDITOFERTAVENTA_SCATTER*/
	editFecha->setText(getRecOfertaVenta()->getValue("FECHA").toDate());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editFecha;
	editContador->setText(getRecOfertaVenta()->getValue("CONTADOR").toInt());
	editNumero->setText(getRecOfertaVenta()->getValue("NUMERO").toString());
	comboIVADetallado->setCurrentItemByValue(getRecOfertaVenta()->getValue("IVADETALLADO").toInt());
	editSumaImportes->setText(getRecOfertaVenta()->getValue("SUMAIMPORTES").toMoney());
	editDtoP100->setText(getRecOfertaVenta()->getValue("DTOP100").toDouble());
	editDescuento->setText(getRecOfertaVenta()->getValue("DESCUENTO").toMoney());
	editBaseImponible->setText(getRecOfertaVenta()->getValue("BASEIMPONIBLE").toMoney());
	editCobros->setText(getRecOfertaVenta()->getValue("COBROS").toMoney());
	editRecargoEquivalencia->setText(getRecOfertaVenta()->getValue("RECARGOEQUIVALENCIA").toMoney());
	editIVA->setText(getRecOfertaVenta()->getValue("IVA").toMoney());
	editEntrega->setText(getRecOfertaVenta()->getValue("ENTREGA").toMoney());
	editResto->setText(getRecOfertaVenta()->getValue("RESTO").toMoney());
	editTotal->setText(getRecOfertaVenta()->getValue("TOTAL").toMoney());
	editDesgloseIVA->setText(getRecOfertaVenta()->getValue("DESGLOSEIVA").toString());
	editDocumentoPago->setText(getRecOfertaVenta()->getValue("DOCUMENTOPAGO").toString());
	editFechaPago->setText(getRecOfertaVenta()->getValue("FECHAPAGO").toDate());
	editNotas->setText(getRecOfertaVenta()->getValue("NOTAS").toString());
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
/*>>>>>FRMEDITOFERTAVENTA_SCATTER*/
	if( isInserting() ) {
		if( !isDuplicating() && editFecha->toDate().isNull() )
			editFecha->setText( Date::currentDate() );
		if( isDuplicating() ) {
			editTipoDocCodigo->setJustEdited( true );
			validateFields( editTipoDocCodigo, 0 );
		}
	} else if( isUpdating() ) {
		pFocusWidget = pFrmOfertaVentaDet;
	}
	pFrmOfertaVentaDet->addDetailIfNeeded();
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

void FrmEditOfertaVenta::gatherFields()
{
/*<<<<<FRMEDITOFERTAVENTA_GATHER*/
	getRecOfertaVenta()->setValue( "FECHA", editFecha->toDate());
	getRecOfertaVenta()->setValue( "TIPODOC_ID", getRecTipoDoc()->getRecordID() );
	getRecOfertaVenta()->setValue( "CONTADOR", editContador->toInt());
	getRecOfertaVenta()->setValue( "NUMERO", editNumero->toString());
	getRecOfertaVenta()->setValue( "CLIENTE_ID", getRecCliente()->getRecordID() );
	getRecOfertaVenta()->setValue( "IVADETALLADO", comboIVADetallado->getCurrentItemValue());
	getRecOfertaVenta()->setValue( "FORMAPAGO_ID", getRecFormaPago()->getRecordID() );
	getRecOfertaVenta()->setValue( "AGENTE_ID", getRecAgente()->getRecordID() );
	getRecOfertaVenta()->setValue( "SUMAIMPORTES", editSumaImportes->toMoney());
	getRecOfertaVenta()->setValue( "DTOP100", editDtoP100->toDouble());
	getRecOfertaVenta()->setValue( "DESCUENTO", editDescuento->toMoney());
	getRecOfertaVenta()->setValue( "BASEIMPONIBLE", editBaseImponible->toMoney());
	getRecOfertaVenta()->setValue( "COBROS", editCobros->toMoney());
	getRecOfertaVenta()->setValue( "RECARGOEQUIVALENCIA", editRecargoEquivalencia->toMoney());
	getRecOfertaVenta()->setValue( "IVA", editIVA->toMoney());
	getRecOfertaVenta()->setValue( "ENTREGA", editEntrega->toMoney());
	getRecOfertaVenta()->setValue( "RESTO", editResto->toMoney());
	getRecOfertaVenta()->setValue( "TOTAL", editTotal->toMoney());
if(empresa::ModuleInstance->usaProyectos()){
	getRecOfertaVenta()->setValue( "PROYECTO_ID", getRecProyecto()->getRecordID() );
}
	getRecOfertaVenta()->setValue( "DESGLOSEIVA", editDesgloseIVA->toString());
	getRecOfertaVenta()->setValue( "DOCUMENTOPAGO", editDocumentoPago->toString());
	getRecOfertaVenta()->setValue( "FECHAPAGO", editFechaPago->toDate());
#ifdef HAVE_TESORERIAMODULE
if( ModuleInstance->getTesoreriaModule() ) {
	getRecOfertaVenta()->setValue( "CUENTAPAGO_ID", getRecCuentaPago()->getRecordID() );
}
#endif
	getRecOfertaVenta()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITOFERTAVENTA_GATHER*/
}

/*<<<<<FRMEDITOFERTAVENTA_CABECERA_GENNUMDOC$s/ModuleInstance->/ModuleInstance->getFactuModule()->/$*/
void FrmEditOfertaVenta::genNumeroDocumento()
{
		editContador->setText( ModuleInstance->getFactuModule()->getMaxContador( "OFERTAVENTA",
			getRecord()->getValue("EMPRESA_ID").toInt(),
			ModuleInstance->getFactuModule()->getEmpresaModule()->getEjercicio(),
			getRecTipoDoc()->getValue("SERIE").toString() ) );
		editContador->setJustEdited( true );
		validateFields( editContador, 0 );
/*>>>>>FRMEDITOFERTAVENTA_CABECERA_GENNUMDOC*/
}

void FrmEditOfertaVenta::scatterTipoDoc()
{
/*<<<<<FRMEDITOFERTAVENTA_SCATTER_TIPODOC*/
	editTipoDocCodigo->setText( getRecTipoDoc()->getValue("CODIGO") );
	editTipoDocNombre->setText( getRecTipoDoc()->getValue("NOMBRE") );
/*>>>>>FRMEDITOFERTAVENTA_SCATTER_TIPODOC*/
	if( editTipoDocCodigo->isJustEdited() ) {
		if( editAgenteCodigo->toString().isEmpty() )
			searchAgenteCodigo->setValue( getRecTipoDoc()->getRecAgente()->getValue("CODIGO") );
		if( getRecTipoDoc()->getValue("IVADetallado").toInt() != 0 )
			comboIVADetallado->setCurrentItemByValue( getRecTipoDoc()->getValue("IVADetallado").toInt() );
		if( editFormaPagoCodigo->toString().isEmpty() )
			searchFormaPagoCodigo->setValue( getRecTipoDoc()->getValue("FORMAPAGO.CODIGO").toInt() );
		genNumeroDocumento();
	}
}

void FrmEditOfertaVenta::pushTipoDocCodigo_clicked()
{
/*<<<<<FRMEDITOFERTAVENTA_PUSH_TIPODOC_CODIGO_CLICKED*/
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
/*>>>>>FRMEDITOFERTAVENTA_PUSH_TIPODOC_CODIGO_CLICKED*/
}

void FrmEditOfertaVenta::scatterCliente()
{
/*<<<<<FRMEDITOFERTAVENTA_SCATTER_CLIENTE*/
	editClienteCodigo->setText( getRecCliente()->getValue("CODIGO") );
	editClienteRazonSocial->setText( getRecCliente()->getValue("RAZONSOCIAL") );
/*>>>>>FRMEDITOFERTAVENTA_SCATTER_CLIENTE*/
/*<<<<<FRMEDITOFERTAVENTA_CABECERA_SCATTER_CLIENTE*/
	if( editClienteCodigo->isJustEdited() ) {
		if( getRecCliente()->getRecFormaPago()->getValue("CODIGO").toInt() != 0 )
			searchFormaPagoCodigo->setValue( getRecCliente()->getRecFormaPago()->getValue("CODIGO") );
		if( getRecCliente()->getValue("IVADetallado").toInt() != 0 )
			comboIVADetallado->setCurrentItemByValue( getRecCliente()->getValue("IVADetallado").toInt() );
		if( getRecCliente()->getRecAgente()->getRecordID() != 0 )
			searchAgenteCodigo->setValue( getRecCliente()->getRecAgente()->getValue("CODIGO") );
	}
/*>>>>>FRMEDITOFERTAVENTA_CABECERA_SCATTER_CLIENTE*/
}

void FrmEditOfertaVenta::pushClienteCodigo_clicked()
{
/*<<<<<FRMEDITOFERTAVENTA_PUSH_CLIENTE_CODIGO_CLICKED*/
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
				factu::RecCliente *tmprec = static_cast<factu::RecCliente *>(DBAPP->createRecord( "Cliente" ));
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
/*>>>>>FRMEDITOFERTAVENTA_PUSH_CLIENTE_CODIGO_CLICKED*/
}

void FrmEditOfertaVenta::scatterFormaPago()
{
/*<<<<<FRMEDITOFERTAVENTA_SCATTER_FORMAPAGO*/
	editFormaPagoCodigo->setText( getRecFormaPago()->getValue("CODIGO") );
	editFormaPagoNombre->setText( getRecFormaPago()->getValue("NOMBRE") );
/*>>>>>FRMEDITOFERTAVENTA_SCATTER_FORMAPAGO*/
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

void FrmEditOfertaVenta::pushFormaPagoCodigo_clicked()
{
/*<<<<<FRMEDITOFERTAVENTA_PUSH_FORMAPAGO_CODIGO_CLICKED*/
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
/*>>>>>FRMEDITOFERTAVENTA_PUSH_FORMAPAGO_CODIGO_CLICKED*/
}

void FrmEditOfertaVenta::scatterAgente()
{
/*<<<<<FRMEDITOFERTAVENTA_SCATTER_AGENTE*/
	editAgenteCodigo->setText( getRecAgente()->getValue("CODIGO") );
	editAgenteRazonSocial->setText( getRecAgente()->getValue("RAZONSOCIAL") );
/*>>>>>FRMEDITOFERTAVENTA_SCATTER_AGENTE*/
}
void FrmEditOfertaVenta::pushAgenteCodigo_clicked()
{
/*<<<<<FRMEDITOFERTAVENTA_PUSH_AGENTE_CODIGO_CLICKED*/
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
				factu::RecAgente *tmprec = static_cast<factu::RecAgente *>(DBAPP->createRecord( "Agente" ));
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
/*>>>>>FRMEDITOFERTAVENTA_PUSH_AGENTE_CODIGO_CLICKED*/
}

void FrmEditOfertaVenta::scatterProyecto()
{
/*<<<<<FRMEDITOFERTAVENTA_SCATTER_PROYECTO*/
	editProyectoCodigo->setText( getRecProyecto()->getValue("CODIGO") );
	editProyectoNombre->setText( getRecProyecto()->getValue("NOMBRE") );
/*>>>>>FRMEDITOFERTAVENTA_SCATTER_PROYECTO*/
}
void FrmEditOfertaVenta::pushProyectoCodigo_clicked()
{
/*<<<<<FRMEDITOFERTAVENTA_PUSH_PROYECTO_CODIGO_CLICKED*/
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
/*>>>>>FRMEDITOFERTAVENTA_PUSH_PROYECTO_CODIGO_CLICKED*/
}

#ifdef HAVE_TESORERIAMODULE
void FrmEditOfertaVenta::scatterCuentaPago()
{
/*<<<<<FRMEDITOFERTAVENTA_SCATTER_CUENTAPAGO*/
	editCuentaPagoCodigo->setText( getRecCuentaPago()->getValue("CODIGO") );
	editCuentaPagoNombre->setText( getRecCuentaPago()->getValue("NOMBRE") );
/*>>>>>FRMEDITOFERTAVENTA_SCATTER_CUENTAPAGO*/
}
#endif

void FrmEditOfertaVenta::pushCuentaPagoCodigo_clicked()
{
#ifdef HAVE_TESORERIAMODULE
/*<<<<<FRMEDITOFERTAVENTA_PUSH_CUENTAPAGO_CODIGO_CLICKED*/
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
/*>>>>>FRMEDITOFERTAVENTA_PUSH_CUENTAPAGO_CODIGO_CLICKED*/
#endif
}


void FrmEditOfertaVenta::specialControlKeyPressed(QWidget *sender, char key)
{
/*<<<<<FRMEDITOFERTAVENTA_SPECIALACTION*/
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
	mControlKeyPressed = '\0';
/*>>>>>FRMEDITOFERTAVENTA_SPECIALACTION*/
}


void FrmEditOfertaVenta::validateFields(QWidget *sender, bool *isvalid, ValidResult *ir)
{
/*<<<<<FRMEDITOFERTAVENTA_VALIDATE*/
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
/*>>>>>FRMEDITOFERTAVENTA_VALIDATE*/
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
/*<<<<<FRMEDITOFERTAVENTA_CABECERA_VALIDATE*/
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
/*>>>>>FRMEDITOFERTAVENTA_CABECERA_VALIDATE*/
    if( !sender && editContador->toInt() == 0 ) {
		genNumeroDocumento();
		gatherFields();
	}
	if( sender == editContador || (sender == editAgenteCodigo && editAgenteCodigo->isJustEdited()) ) {
		editNumero->setText( factu::RecTipoDoc::formatNumDocumento(
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
	if( !ir ) {
		showValidMessages(isvalid, *validresult, sender);
		delete validresult;
	}
}

/*<<<<<FRMEDITOFERTAVENTA_CABECERA_UPDATEFROMDETAILS*/
void FrmEditOfertaVenta::updateFromDetails( class FrmEditRecDetail *detailfrm )
{
	actTotales();
	numeraLineas();
/*>>>>>FRMEDITOFERTAVENTA_CABECERA_UPDATEFROMDETAILS*/
}

/*<<<<<FRMEDITOFERTAVENTA_CABECERA_NUMERALINEAS*/
void FrmEditOfertaVenta::numeraLineas()
{
	dbRecordList *reclst = getRecOfertaVenta()->getListOfertaVentaDet();
	for ( unsigned int i = 0; i < reclst->size(); i++ ) {
		RecOfertaVentaDet *detalle = static_cast<RecOfertaVentaDet *>( reclst->at( i ) );
		if( !detalle->isEmpty() ) // No numerar detalles vacíos 
			detalle->setValue( "NLINEA", i+1 );
	}
/*>>>>>FRMEDITOFERTAVENTA_CABECERA_NUMERALINEAS*/
}

/*<<<<<FRMEDITOFERTAVENTA_CABECERA_ACTTOTALES*/
void FrmEditOfertaVenta::actTotales()
{
	getRecOfertaVenta()->gather( this );
	getRecOfertaVenta()->actTotales();
	getRecOfertaVenta()->scatter( this );
/*>>>>>FRMEDITOFERTAVENTA_CABECERA_ACTTOTALES*/
}

bool FrmEditOfertaVenta::canBeginEdit(FrmEditRec::EditMode newmode)
{
	mHasCobros = false;
	mOldTotal = getRecOfertaVenta()->getValue( "TOTAL" ).toMoney();
	if( newmode == DataTable::updating || newmode == DataTable::deleting ) {
		if( getRecOfertaVenta()->hasCobros( -1, pagos::PagosModule::ReciboPendiente ) ) {
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

void FrmEditOfertaVenta::slotCobrar()
{
	if( editEntrega->toDouble() != 0.0 ) {
		editResto->setText( editResto->toMoney() + editEntrega->toMoney() );
		editEntrega->setText( 0.0 );
		editFechaPago->setText( "" );
		editDocumentoPago->setText( "" );
		pushCobrar->setText( _("&Entrega") );
	} else {
		Date fechapago = editFechaPago->toDate();
		if( fechapago.isNull() )
			fechapago = editFecha->toDate();
		Xtring docpago = _("Cobro alb. " + editNumero->toString() );
		Money pago = editEntrega->toMoney();
		if( pago == 0.0 )
			pago = editTotal->toMoney();
		bool has_contab = false;
		Xtring cuentapago;
		if( cuentapago.isEmpty() )
			cuentapago = empresa::ModuleInstance->getRecEmpresa()->getValue("CUENTACAJA").toString();
		factu::FrmPagar *pr = new factu::FrmPagar( has_contab, pago, fechapago,
			cuentapago, docpago, 0, "pagarOfertaVenta" );
		pr->showModalFor( this, true, true );
		if( !pr->wasCancelled() ) {
			editEntrega->setText( pr->getImporte() );
			editResto->setText( editResto->toMoney() - pr->getImporte() );
			editFechaPago->setText( pr->getFechaPago() );
			editDocumentoPago->setText( pr->getDocumentoPago() );
			if( editTotal->toDouble() != 0.0 )
				pushCobrar->setText( _("&Borrar entrega") );
		}
		delete pr;
	}
}


void FrmEditOfertaVenta::slotPasarAAlbaran()
{
	dbRecordID id = getTableRecordID();
	if( id == 0 )
		return;
	RecOfertaVenta *orig_record = static_cast<RecOfertaVenta*>(DBAPP->createRecord("OFERTAVENTA"));
	if( !orig_record->read(id) )
		return;
	DBAPP->waitCursor(true);
	dbRecordList *detalles = orig_record->getListOfertaVentaDet();
	factu::RecAlbaranVenta *new_albaran_venta = static_cast<factu::RecAlbaranVenta *>( DBAPP->createRecord("ALBARANVENTA") );
	new_albaran_venta->copyRecord( orig_record );
	new_albaran_venta->setValue("CONTADOR",0);
	for( dbRecordList::const_iterator it = detalles->begin(); it != detalles->end(); ++it ) {
		dbRecord *detalle = *it;
		factu::RecAlbaranVentaDet *new_detalle = static_cast<factu::RecAlbaranVentaDet *>( DBAPP->createRecord("ALBARANVENTADET") );
		new_detalle->copyRecord( detalle );
		new_detalle->setNew( true );
		new_albaran_venta->getListAlbaranVentaDet()->addRecord( new_detalle );
	}
	FrmEditRec *frmeditalbaran = DBAPP->createEditForm(0, new_albaran_venta,
														  0, DataTable::inserting, dbApplication::simpleEdition);
	DBAPP->resetCursor();
	if( !frmeditalbaran->showAndSave() )
		frmeditalbaran->showModalFor( 0, true, true );
	if( frmeditalbaran->isSaved() ) {
		ofertas::ModuleInstance->copiaCobros(orig_record, new_albaran_venta);
		Xtring rec_desc = DBAPP->getTableDescSingular( orig_record->getTableName(), "la" );
		if( FrmBase::msgYesNo("Copiar a albarán",
									Xtring::printf("%s se ha guardado perfectamente.\n¿Quieres borrar la oferta de venta original?", rec_desc.c_str())) ) {
			if( orig_record->remove() ) {
				DBAPP->showOSD(Xtring::printf("%s original se ha borrado correctamente", rec_desc.c_str()),"");
				refresh();
			} else {
				FrmBase::msgError(Xtring::printf("Error borrando %s original", rec_desc.c_str()), "");
			}
		}
	}
	delete frmeditalbaran;
	delete new_albaran_venta;
	delete orig_record;
}

void FrmEditOfertaVenta::updateStatus(bool callbehaviors)
{
	FrmEditRecMaster::updateStatus(false);
	pushPasarAAlbaran->hide();
	if( !isEditing() && getDataTable()->getDataModel()->getRowCount()> 0) {
		pushPasarAAlbaran->show();
	}
}


/*<<<<<FRMEDITOFERTAVENTA_FIN*/
} // namespace ofertas
} // namespace gong
/*>>>>>FRMEDITOFERTAVENTA_FIN*/
