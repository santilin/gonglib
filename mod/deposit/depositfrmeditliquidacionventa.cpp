/*<<<<<COPYLEFT*/
/** @file depositfrmeditliquidacionventa.cpp Fichero de edición de liquidaciones de ventas
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */
/*>>>>>COPYLEFT*/
/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de liquidaciones de ventas
// FIELD Fecha date - cabecera
// FIELD TipoDoc_ID Reference(factu::TipoDoc,Codigo,Nombre) - cabecera
// FIELD Numero string - cabecera
// FIELD IVADetallado comboint - cabecera
// FIELD Contador int - cabecera
// FIELD Cliente_ID Reference(factu::Cliente,Codigo,RazonSocial,dbRecord::InsertIfNotFound) - cabecera2
// FIELD Agente_ID Reference(factu::Agente,Codigo,RazonSocial,dbRecord::InsertIfNotFound) - cabecera2
// FIELD LiquidacionVentaDet FrmEditRecDetail
// FIELD SumaImportes money - noaddrightSumaImportes
// FIELD DtoP100 double - noaddrightDtoP100
// FIELD Descuento money - noaddrightDescuento
// FIELD BaseImponible money - noaddrightBaseImponible
// FIELD IVA money - noaddrightIVA
// FIELD RecargoEquivalencia money - noaddrightRecargoEquivalencia
// FIELD Total money - noaddrightTotal
// FIELD Notas text
// TYPE FrmEditRecMaster deposit::LiquidacionVenta Pedido Venta
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITLIQUIDACIONVENTA_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "depositfrmeditliquidacionventa.h"
/*>>>>>FRMEDITLIQUIDACIONVENTA_INCLUDES*/
#include <depositmodule.h>
#include <factufldivadetallado.h>

namespace gong {
namespace deposit {

/*<<<<<FRMEDITLIQUIDACIONVENTA_CONSTRUCTOR*/
FrmEditLiquidacionVenta::FrmEditLiquidacionVenta(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditLiquidacionVenta" );
/*>>>>>FRMEDITLIQUIDACIONVENTA_CONSTRUCTOR*/
/*<<<<<FRMEDITLIQUIDACIONVENTA_INIT_CONTROLS*/
	QHBoxLayout *cabeceraLayout = new QHBoxLayout(0, 0, 6, "cabeceraLayout");
	QHBoxLayout *cabecera2Layout = new QHBoxLayout(0, 0, 6, "cabecera2Layout");
	QHBoxLayout *liquidacionventadetLayout = new QHBoxLayout(0, 0, 6, "liquidacionventadetLayout");
	QHBoxLayout *rightSumaImportesLayout = new QHBoxLayout(0, 0, 6, "rightSumaImportesLayout");
	QHBoxLayout *rightDtoP100Layout = new QHBoxLayout(0, 0, 6, "rightDtoP100Layout");
	QHBoxLayout *rightDescuentoLayout = new QHBoxLayout(0, 0, 6, "rightDescuentoLayout");
	QHBoxLayout *rightBaseImponibleLayout = new QHBoxLayout(0, 0, 6, "rightBaseImponibleLayout");
	QHBoxLayout *rightIVALayout = new QHBoxLayout(0, 0, 6, "rightIVALayout");
	QHBoxLayout *rightRecargoEquivalenciaLayout = new QHBoxLayout(0, 0, 6, "rightRecargoEquivalenciaLayout");
	QHBoxLayout *rightTotalLayout = new QHBoxLayout(0, 0, 6, "rightTotalLayout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
	editFecha = addEditField( pControlsFrame, "LIQUIDACIONVENTA", "FECHA", cabeceraLayout );

	searchTipoDocCodigo = addSearchField( pControlsFrame, "TIPODOC_ID", "TIPODOC", "CODIGO", "NOMBRE", cabeceraLayout );
	pushTipoDocCodigo = searchTipoDocCodigo->getButton();
	connect( pushTipoDocCodigo, SIGNAL( clicked() ), this, SLOT( pushTipoDocCodigo_clicked() ) );
	editTipoDocCodigo = searchTipoDocCodigo->getEditCode();
	editTipoDocNombre = searchTipoDocCodigo->getEditDesc();
	editNumero = addEditField( pControlsFrame, "LIQUIDACIONVENTA", "NUMERO", cabeceraLayout );
	comboIVADetallado = addComboIntField( pControlsFrame, "LIQUIDACIONVENTA", "IVADETALLADO", cabeceraLayout );
	editContador = addEditField( pControlsFrame, "LIQUIDACIONVENTA", "CONTADOR", cabeceraLayout );

	searchClienteCodigo = addSearchField( pControlsFrame, "CLIENTE_ID", "CLIENTE", "CODIGO", "RAZONSOCIAL", cabecera2Layout );
	pushClienteCodigo = searchClienteCodigo->getButton();
	connect( pushClienteCodigo, SIGNAL( clicked() ), this, SLOT( pushClienteCodigo_clicked() ) );
	editClienteCodigo = searchClienteCodigo->getEditCode();
	editClienteRazonSocial = searchClienteCodigo->getEditDesc();

	searchAgenteCodigo = addSearchField( pControlsFrame, "AGENTE_ID", "AGENTE", "CODIGO", "RAZONSOCIAL", cabecera2Layout );
	pushAgenteCodigo = searchAgenteCodigo->getButton();
	connect( pushAgenteCodigo, SIGNAL( clicked() ), this, SLOT( pushAgenteCodigo_clicked() ) );
	editAgenteCodigo = searchAgenteCodigo->getEditCode();
	editAgenteRazonSocial = searchAgenteCodigo->getEditDesc();

// frmDetails: LiquidacionVentaDet
	QFrame *liquidacionventadetFrame = new QFrame(this);
	liquidacionventadetFrame->setFrameStyle(QFrame::Sunken | QFrame::Panel);
	liquidacionventadetFrame->setLineWidth(4);
	QVBoxLayout *liquidacionventadetFrameLayout = new QVBoxLayout(liquidacionventadetFrame);
	RecLiquidacionVentaDet *liquidacionventadet = getRecLiquidacionVenta()->getRecLiquidacionVentaDet(-1);
	dbViewDefinitionDict liquidacionventadetviews;
	DBAPP->getDatabase()->getViewsForTable( liquidacionventadet, liquidacionventadetviews );
	dbRecordListDataModel *liquidacionventadetdm =
		new dbRecordListDataModel(getRecLiquidacionVenta()->getListLiquidacionVentaDet(), liquidacionventadet, liquidacionventadetviews, "1=0");
	pFrmLiquidacionVentaDet = static_cast<FrmEditLiquidacionVentaDet *>(
		DBAPP->createEditDetailForm(this,
		-1, liquidacionventadet, "LIQUIDACIONVENTADET",liquidacionventadetdm, DataTable::browsing,
		static_cast<dbApplication::EditFlags>(dbApplication::embedded + dbApplication::tableEditEvent ),
		pControlsFrame) );
	pFrmLiquidacionVentaDet->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding);
	liquidacionventadetFrameLayout->addWidget( pFrmLiquidacionVentaDet );
	liquidacionventadetLayout->addWidget(liquidacionventadetFrame);
	editSumaImportes = addEditField( pControlsFrame, "LIQUIDACIONVENTA", "SUMAIMPORTES", rightSumaImportesLayout );
	editDtoP100 = addEditField( pControlsFrame, "LIQUIDACIONVENTA", "DTOP100", rightDtoP100Layout );
	editDescuento = addEditField( pControlsFrame, "LIQUIDACIONVENTA", "DESCUENTO", rightDescuentoLayout );
	editBaseImponible = addEditField( pControlsFrame, "LIQUIDACIONVENTA", "BASEIMPONIBLE", rightBaseImponibleLayout );
	editIVA = addEditField( pControlsFrame, "LIQUIDACIONVENTA", "IVA", rightIVALayout );
	editRecargoEquivalencia = addEditField( pControlsFrame, "LIQUIDACIONVENTA", "RECARGOEQUIVALENCIA", rightRecargoEquivalenciaLayout );
	editTotal = addEditField( pControlsFrame, "LIQUIDACIONVENTA", "TOTAL", rightTotalLayout );
	editNotas = addTextField( pControlsFrame, "LIQUIDACIONVENTA", "NOTAS", notasLayout );
	pControlsLayout->addLayout( cabeceraLayout );
	pControlsLayout->addLayout( cabecera2Layout );
	pControlsLayout->addLayout( liquidacionventadetLayout );
	alignLayout( rightSumaImportesLayout, false);
	alignLayout( rightDtoP100Layout, false);
	alignLayout( rightDescuentoLayout, false);
	alignLayout( rightBaseImponibleLayout, false);
	alignLayout( rightIVALayout, false);
	alignLayout( rightRecargoEquivalenciaLayout, false);
	alignLayout( rightTotalLayout, false);
	pControlsLayout->addLayout( notasLayout );
/*>>>>>FRMEDITLIQUIDACIONVENTA_INIT_CONTROLS*/

	QGridLayout *gridlayout = new QGridLayout();
	gridlayout->addLayout( rightSumaImportesLayout, 0, 1 );
	gridlayout->addLayout( rightDtoP100Layout, 0, 2 );
	gridlayout->addLayout( rightDescuentoLayout, 0, 3 );
	gridlayout->addLayout( rightBaseImponibleLayout, 1, 0 );
	gridlayout->addLayout( rightIVALayout, 1, 1 );
	gridlayout->addLayout( rightRecargoEquivalenciaLayout, 1, 2 );
	gridlayout->addLayout( rightTotalLayout, 1, 3 );
	pControlsLayout->addLayout( gridlayout );

	editContador->setMustBeReadOnly( true );
	editSumaImportes->setMustBeReadOnly( true );
	editBaseImponible->setMustBeReadOnly( true );
	editRecargoEquivalencia->setMustBeReadOnly( true );
	editIVA->setMustBeReadOnly( true );
	editNumero->setWidthInChars( 15 );
 	editClienteCodigo->setWidthInChars(8);
 	editAgenteCodigo->setWidthInChars(8);
	editTipoDocCodigo->setWidthInChars(8);
	editNotas->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Minimum);
}

void FrmEditLiquidacionVenta::scatterFields()
{
	if( isDuplicating() ) {
		getRecLiquidacionVenta()->setValue( "CONTADOR", 0 );
	}
/*<<<<<FRMEDITLIQUIDACIONVENTA_SCATTER*/
	editFecha->setText(getRecLiquidacionVenta()->getValue("FECHA").toDate());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editFecha;
	editNumero->setText(getRecLiquidacionVenta()->getValue("NUMERO").toString());
	comboIVADetallado->setCurrentItemByValue(getRecLiquidacionVenta()->getValue("IVADETALLADO").toInt());
	editContador->setText(getRecLiquidacionVenta()->getValue("CONTADOR").toInt());
	editSumaImportes->setText(getRecLiquidacionVenta()->getValue("SUMAIMPORTES").toMoney());
	editDtoP100->setText(getRecLiquidacionVenta()->getValue("DTOP100").toDouble());
	editDescuento->setText(getRecLiquidacionVenta()->getValue("DESCUENTO").toMoney());
	editBaseImponible->setText(getRecLiquidacionVenta()->getValue("BASEIMPONIBLE").toMoney());
	editIVA->setText(getRecLiquidacionVenta()->getValue("IVA").toMoney());
	editRecargoEquivalencia->setText(getRecLiquidacionVenta()->getValue("RECARGOEQUIVALENCIA").toMoney());
	editTotal->setText(getRecLiquidacionVenta()->getValue("TOTAL").toMoney());
	editNotas->setText(getRecLiquidacionVenta()->getValue("NOTAS").toString());
	scatterTipoDoc();
	scatterCliente();
	scatterAgente();
/*>>>>>FRMEDITLIQUIDACIONVENTA_SCATTER*/
	if( isInserting() ) {
		if( !isDuplicating() && editFecha->toDate().isNull() )
			editFecha->setText( factu::ModuleInstance->getWorkingDate() );
		if( isDuplicating() ) {
			editTipoDocCodigo->setJustEdited( true );
			validateFields( editTipoDocCodigo, 0 );
		}
	} else if( isUpdating() ) {
		pFocusWidget = pFrmLiquidacionVentaDet;
	}
	pFrmLiquidacionVentaDet->addDetailIfNeeded();
	if( editContador->toInt() == 0 )
		editContador->setText( empresa::ModuleInstance->getMaxContador() );
}


void FrmEditLiquidacionVenta::gatherFields()
{
    /*<<<<<FRMEDITLIQUIDACIONVENTA_GATHER*/
	getRecLiquidacionVenta()->setValue( "FECHA", editFecha->toDate());
	getRecLiquidacionVenta()->setValue( "TIPODOC_ID", getRecTipoDoc()->getRecordID() );
	getRecLiquidacionVenta()->setValue( "NUMERO", editNumero->toString());
	getRecLiquidacionVenta()->setValue( "IVADETALLADO", comboIVADetallado->getCurrentItemValue());
	getRecLiquidacionVenta()->setValue( "CONTADOR", editContador->toInt());
	getRecLiquidacionVenta()->setValue( "CLIENTE_ID", getRecCliente()->getRecordID() );
	getRecLiquidacionVenta()->setValue( "AGENTE_ID", getRecAgente()->getRecordID() );
	getRecLiquidacionVenta()->setValue( "SUMAIMPORTES", editSumaImportes->toMoney());
	getRecLiquidacionVenta()->setValue( "DTOP100", editDtoP100->toDouble());
	getRecLiquidacionVenta()->setValue( "DESCUENTO", editDescuento->toMoney());
	getRecLiquidacionVenta()->setValue( "BASEIMPONIBLE", editBaseImponible->toMoney());
	getRecLiquidacionVenta()->setValue( "IVA", editIVA->toMoney());
	getRecLiquidacionVenta()->setValue( "RECARGOEQUIVALENCIA", editRecargoEquivalencia->toMoney());
	getRecLiquidacionVenta()->setValue( "TOTAL", editTotal->toMoney());
	getRecLiquidacionVenta()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITLIQUIDACIONVENTA_GATHER*/
	factu::ModuleInstance->setWorkingDate( editFecha->toDate() );
}

void FrmEditLiquidacionVenta::scatterTipoDoc()
{
/*<<<<<FRMEDITLIQUIDACIONVENTA_SCATTER_TIPODOC*/
	editTipoDocCodigo->setText( getRecTipoDoc()->getValue("CODIGO") );
	editTipoDocNombre->setText( getRecTipoDoc()->getValue("NOMBRE") );
/*>>>>>FRMEDITLIQUIDACIONVENTA_SCATTER_TIPODOC*/
	if( editTipoDocCodigo->isJustEdited() ) {
		if( editAgenteCodigo->toString().isEmpty() ) {
			editAgenteCodigo->setText( getRecTipoDoc()->getRecAgente()->getValue("CODIGO") );
			editAgenteCodigo->setJustEdited( true );
			validate( editAgenteCodigo, 0 );
		}
		if( getRecTipoDoc()->getValue("IVADetallado").toInt() != 0 )
			comboIVADetallado->setCurrentItemByValue( getRecTipoDoc()->getValue("IVADetallado").toInt() );
	}
}

void FrmEditLiquidacionVenta::pushTipoDocCodigo_clicked()
{
/*<<<<<FRMEDITLIQUIDACIONVENTA_PUSH_TIPODOC_CODIGO_CLICKED*/
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
/*>>>>>FRMEDITLIQUIDACIONVENTA_PUSH_TIPODOC_CODIGO_CLICKED*/
}

void FrmEditLiquidacionVenta::scatterCliente()
{
/*<<<<<FRMEDITLIQUIDACIONVENTA_SCATTER_CLIENTE*/
	editClienteCodigo->setText( getRecCliente()->getValue("CODIGO") );
	editClienteRazonSocial->setText( getRecCliente()->getValue("RAZONSOCIAL") );
/*>>>>>FRMEDITLIQUIDACIONVENTA_SCATTER_CLIENTE*/
/*<<<<<FRMEDITLIQUIDACIONVENTA_CABECERA_SCATTER_CLIENTE*/
	if( editClienteCodigo->isJustEdited() ) {
		if( getRecCliente()->getValue("IVADetallado").toInt() != 0 )
			comboIVADetallado->setCurrentItemByValue( getRecCliente()->getValue("IVADetallado").toInt() );
		if( getRecCliente()->getRecAgente()->getRecordID() != 0 )
			searchAgenteCodigo->setValue( getRecCliente()->getRecAgente()->getValue("CODIGO") );
	}
/*>>>>>FRMEDITLIQUIDACIONVENTA_CABECERA_SCATTER_CLIENTE*/
}

void FrmEditLiquidacionVenta::pushClienteCodigo_clicked()
{
    /*<<<<<FRMEDITLIQUIDACIONVENTA_PUSH_CLIENTE_CODIGO_CLICKED*/
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
/*>>>>>FRMEDITLIQUIDACIONVENTA_PUSH_CLIENTE_CODIGO_CLICKED*/
}

void FrmEditLiquidacionVenta::scatterAgente()
{
/*<<<<<FRMEDITLIQUIDACIONVENTA_SCATTER_AGENTE*/
	editAgenteCodigo->setText( getRecAgente()->getValue("CODIGO") );
	editAgenteRazonSocial->setText( getRecAgente()->getValue("RAZONSOCIAL") );
/*>>>>>FRMEDITLIQUIDACIONVENTA_SCATTER_AGENTE*/
}
void FrmEditLiquidacionVenta::pushAgenteCodigo_clicked()
{
    /*<<<<<FRMEDITLIQUIDACIONVENTA_PUSH_AGENTE_CODIGO_CLICKED*/
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
/*>>>>>FRMEDITLIQUIDACIONVENTA_PUSH_AGENTE_CODIGO_CLICKED*/
}


void FrmEditLiquidacionVenta::specialControlKeyPressed( QWidget *sender, char key )
{
    /*<<<<<FRMEDITLIQUIDACIONVENTA_SPECIALACTION*/
	mControlKeyPressed = key;
	FrmEditRecMaster::specialControlKeyPressed(sender,key); // calls the behaviors
	if( sender == editTipoDocCodigo )
		pushTipoDocCodigo_clicked();
	if( sender == editClienteCodigo )
		pushClienteCodigo_clicked();
	if( sender == editAgenteCodigo )
		pushAgenteCodigo_clicked();
	mControlKeyPressed = '\0';
/*>>>>>FRMEDITLIQUIDACIONVENTA_SPECIALACTION*/
}

void FrmEditLiquidacionVenta::validateFields( QWidget *sender, bool *isvalid, ValidResult *ir )
{
    /*<<<<<FRMEDITLIQUIDACIONVENTA_VALIDATE*/
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
	if( focusWidget() != pushAgenteCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editAgenteCodigo, editAgenteRazonSocial,
		getRecAgente(), "CODIGO", "RAZONSOCIAL", Xtring::null, dbRecord::SeekCodeFlags( dbRecord::InsertIfNotFound )) )
		scatterAgente();
/*>>>>>FRMEDITLIQUIDACIONVENTA_VALIDATE*/
/*<<<<<FRMEDITLIQUIDACIONVENTA_CABECERA_VALIDATE*/
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
/*>>>>>FRMEDITLIQUIDACIONVENTA_CABECERA_VALIDATE*/
	if( !sender && isInserting() ) {
		int contador = empresa::ModuleInstance->getMaxContador();
		if( contador > editContador->toInt() ) {
			editContador->setText( contador );
			validresult->addWarning( Xtring::printf(
				_("El contador ha cambiado durante la edición de este registro. El nuevo contador es %d"),
				contador ), "CONTADOR" );
		}
	}
	if( !sender || (
		(sender == editNumero && !editNumero->toString().isEmpty())
			|| (sender == editClienteCodigo && !editClienteCodigo->toString().isEmpty() ) )
		) {
		dbRecordID prov_id = getRecCliente()->getRecordID();
		if( getRecord()->existsAnother(
			"CLIENTE_ID=" + getRecord()->getConnection()->toSQL( prov_id )
			+ " AND NUMERO=" + getRecord()->getConnection()->toSQL( editNumero->toString() ) ) ) {
			*isvalid = false;
			validresult->addError( Xtring::printf(_("Ya existe %s de %s con este valor para '%s'"),
					DBAPP->getTableDescSingular( getRecord()->getTableName(), "una" ).c_str(),
					DBAPP->getTableDescSingular( "CLIENTE", "esta" ).c_str(),
					DBAPP->getDatabase()->findFieldDefinition( "LIQUIDACIONCOMPRA.NUMERO" )->getCaption().c_str()),
					"NUMERO" );
		}
	}
	if ( !ir ) {
        showValidMessages( isvalid, *validresult, sender );
        delete validresult;
    }
}

/*<<<<<FRMEDITLIQUIDACIONVENTA_CABECERA_UPDATEFROMDETAILS*/
void FrmEditLiquidacionVenta::updateFromDetails( class FrmEditRecDetail *detailfrm )
{
	actTotales();
	numeraLineas();
/*>>>>>FRMEDITLIQUIDACIONVENTA_CABECERA_UPDATEFROMDETAILS*/
}

/*<<<<<FRMEDITLIQUIDACIONVENTA_CABECERA_NUMERALINEAS*/
void FrmEditLiquidacionVenta::numeraLineas()
{
	dbRecordList *reclst = getRecLiquidacionVenta()->getListLiquidacionVentaDet();
	for ( unsigned int i = 0; i < reclst->size(); i++ ) {
		RecLiquidacionVentaDet *detalle = static_cast<RecLiquidacionVentaDet *>( reclst->at( i ) );
		if( !detalle->isEmpty() ) // No numerar detalles vacíos 
			detalle->setValue( "NLINEA", i+1 );
	}
/*>>>>>FRMEDITLIQUIDACIONVENTA_CABECERA_NUMERALINEAS*/
}

/*<<<<<FRMEDITLIQUIDACIONVENTA_CABECERA_ACTTOTALES*/
void FrmEditLiquidacionVenta::actTotales()
{
	getRecLiquidacionVenta()->gather( this );
	getRecLiquidacionVenta()->actTotales();
	getRecLiquidacionVenta()->scatter( this );
/*>>>>>FRMEDITLIQUIDACIONVENTA_CABECERA_ACTTOTALES*/
}

/*<<<<<FRMEDITLIQUIDACIONVENTA_FIN*/
} // namespace deposit
} // namespace gong
/*>>>>>FRMEDITLIQUIDACIONVENTA_FIN*/

#if 0
/*<<<<<FRMEDITLIQUIDACIONVENTA_CABECERA_GENNUMDOC*/
void FrmEditLiquidacionVenta::genNumeroDocumento()
{
		editContador->setText( ModuleInstance->getMaxContador( "LIQUIDACIONVENTA",
			getRecord()->getValue("EMPRESA_ID").toInt(),
			ModuleInstance->getEmpresaModule()->getEjercicio(),
			getRecTipoDoc()->getValue("SERIE").toString() ) );
		editContador->setJustEdited( true );
		validateFields( editContador, 0 );
/*>>>>>FRMEDITLIQUIDACIONVENTA_CABECERA_GENNUMDOC*/
}
#endif
