/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de presupuestos de venta
// FIELD Fecha date - cabecera
// FIELD TipoDoc_ID Reference(TipoDoc,Codigo,Nombre) - cabecera
// FIELD Contador int - cabecera
// FIELD Numero string - cabecera
// FIELD Cliente_ID Reference(Cliente,Codigo,RazonSocial,dbApplication::InsertIfNotFound) - cabecera2
// FIELD IVADetallado comboint - cabecera
// FIELD Agente_ID Reference(Agente,Codigo,RazonSocial,dbApplication::InsertIfNotFound) - cabecera2
// FIELD PresupuestoVentaDet FrmEditRecDetail
// FIELD SumaImportes money - noaddrightSumaImportes
// FIELD DtoP100 double - noaddrightDtoP100
// FIELD Descuento money - noaddrightDescuento
// FIELD BaseImponible money - noaddrightBaseImponible
// FIELD RecargoEquivalencia money - noaddrightRecargoEquivalencia
// FIELD IVA money - noaddrightIVA
// FIELD Total money - noaddrightTotal
// FIELD DesgloseIVA string - desgloseiva
// FIELD Notas text
// TYPE FrmEditRecMaster factu::PresupuestoVenta Presupuesto Venta
/*>>>>>MODULE_INFO*/

#include <gonggettext.h>
/*<<<<<FRMEDITPRESUPUESTOVENTA_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "factufrmeditpresupuestoventa.h"
/*>>>>>FRMEDITPRESUPUESTOVENTA_INCLUDES*/
#include "factumodule.h"
#include "factufldivadetallado.h"

namespace gong {
namespace factu {

/*<<<<<FRMEDITPRESUPUESTOVENTA_CONSTRUCTOR*/
FrmEditPresupuestoVenta::FrmEditPresupuestoVenta(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditPresupuestoVenta" );
/*>>>>>FRMEDITPRESUPUESTOVENTA_CONSTRUCTOR*/
    /*<<<<<FRMEDITPRESUPUESTOVENTA_INIT_CONTROLS*/
	QHBoxLayout *cabeceraLayout = new QHBoxLayout(0, 0, 6, "cabeceraLayout");
	QHBoxLayout *cabecera2Layout = new QHBoxLayout(0, 0, 6, "cabecera2Layout");
	QHBoxLayout *presupuestoventadetLayout = new QHBoxLayout(0, 0, 6, "presupuestoventadetLayout");
	QHBoxLayout *rightSumaImportesLayout = new QHBoxLayout(0, 0, 6, "rightSumaImportesLayout");
	QHBoxLayout *rightDtoP100Layout = new QHBoxLayout(0, 0, 6, "rightDtoP100Layout");
	QHBoxLayout *rightDescuentoLayout = new QHBoxLayout(0, 0, 6, "rightDescuentoLayout");
	QHBoxLayout *rightBaseImponibleLayout = new QHBoxLayout(0, 0, 6, "rightBaseImponibleLayout");
	QHBoxLayout *rightRecargoEquivalenciaLayout = new QHBoxLayout(0, 0, 6, "rightRecargoEquivalenciaLayout");
	QHBoxLayout *rightIVALayout = new QHBoxLayout(0, 0, 6, "rightIVALayout");
	QHBoxLayout *rightTotalLayout = new QHBoxLayout(0, 0, 6, "rightTotalLayout");
	QHBoxLayout *desgloseivaLayout = new QHBoxLayout(0, 0, 6, "desgloseivaLayout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
	editFecha = addEditField( pControlsFrame, "PRESUPUESTOVENTA", "FECHA", cabeceraLayout );

	searchTipoDocCodigo = addSearchField( pControlsFrame, "TIPODOC_ID", "TIPODOC", "CODIGO", "NOMBRE", cabeceraLayout );
	pushTipoDocCodigo = searchTipoDocCodigo->getButton();
	connect( pushTipoDocCodigo, SIGNAL( clicked() ), this, SLOT( pushTipoDocCodigo_clicked() ) );
	editTipoDocCodigo = searchTipoDocCodigo->getEditCode();
	editTipoDocNombre = searchTipoDocCodigo->getEditDesc();
	editContador = addEditField( pControlsFrame, "PRESUPUESTOVENTA", "CONTADOR", cabeceraLayout );
	editNumero = addEditField( pControlsFrame, "PRESUPUESTOVENTA", "NUMERO", cabeceraLayout );

	searchClienteCodigo = addSearchField( pControlsFrame, "CLIENTE_ID", "CLIENTE", "CODIGO", "RAZONSOCIAL", cabecera2Layout );
	pushClienteCodigo = searchClienteCodigo->getButton();
	connect( pushClienteCodigo, SIGNAL( clicked() ), this, SLOT( pushClienteCodigo_clicked() ) );
	editClienteCodigo = searchClienteCodigo->getEditCode();
	editClienteRazonSocial = searchClienteCodigo->getEditDesc();
	comboIVADetallado = addComboIntField( pControlsFrame, "PRESUPUESTOVENTA", "IVADETALLADO", cabeceraLayout );

	searchAgenteCodigo = addSearchField( pControlsFrame, "AGENTE_ID", "AGENTE", "CODIGO", "RAZONSOCIAL", cabecera2Layout );
	pushAgenteCodigo = searchAgenteCodigo->getButton();
	connect( pushAgenteCodigo, SIGNAL( clicked() ), this, SLOT( pushAgenteCodigo_clicked() ) );
	editAgenteCodigo = searchAgenteCodigo->getEditCode();
	editAgenteRazonSocial = searchAgenteCodigo->getEditDesc();

// frmDetails: PresupuestoVentaDet
	QFrame *presupuestoventadetFrame = new QFrame(this);
	presupuestoventadetFrame->setFrameStyle(QFrame::Sunken | QFrame::Panel);
	presupuestoventadetFrame->setLineWidth(4);
	QVBoxLayout *presupuestoventadetFrameLayout = new QVBoxLayout(presupuestoventadetFrame);
	RecPresupuestoVentaDet *presupuestoventadet = getRecPresupuestoVenta()->getRecPresupuestoVentaDet(-1);
	dbViewDefinitionDict presupuestoventadetviews;
	DBAPP->getDatabase()->getViewsForTable( presupuestoventadet, presupuestoventadetviews );
	dbRecordListDataModel *presupuestoventadetdm =
		new dbRecordListDataModel(getRecPresupuestoVenta()->getListPresupuestoVentaDet(), presupuestoventadet, presupuestoventadetviews, "1=0");
	pFrmPresupuestoVentaDet = static_cast<FrmEditPresupuestoVentaDet *>(
		DBAPP->createEditDetailForm(this,
		-1, presupuestoventadet, "PRESUPUESTOVENTADET",presupuestoventadetdm, DataTable::browsing,
		static_cast<dbApplication::EditFlags>(dbApplication::embedded + dbApplication::tableEditEvent ),
		pControlsFrame) );
	pFrmPresupuestoVentaDet->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding);
	presupuestoventadetFrameLayout->addWidget( pFrmPresupuestoVentaDet );
	presupuestoventadetLayout->addWidget(presupuestoventadetFrame);
	editSumaImportes = addEditField( pControlsFrame, "PRESUPUESTOVENTA", "SUMAIMPORTES", rightSumaImportesLayout );
	editDtoP100 = addEditField( pControlsFrame, "PRESUPUESTOVENTA", "DTOP100", rightDtoP100Layout );
	editDescuento = addEditField( pControlsFrame, "PRESUPUESTOVENTA", "DESCUENTO", rightDescuentoLayout );
	editBaseImponible = addEditField( pControlsFrame, "PRESUPUESTOVENTA", "BASEIMPONIBLE", rightBaseImponibleLayout );
	editRecargoEquivalencia = addEditField( pControlsFrame, "PRESUPUESTOVENTA", "RECARGOEQUIVALENCIA", rightRecargoEquivalenciaLayout );
	editIVA = addEditField( pControlsFrame, "PRESUPUESTOVENTA", "IVA", rightIVALayout );
	editTotal = addEditField( pControlsFrame, "PRESUPUESTOVENTA", "TOTAL", rightTotalLayout );
	editDesgloseIVA = addEditField( pControlsFrame, "PRESUPUESTOVENTA", "DESGLOSEIVA", desgloseivaLayout );
	editNotas = addTextField( pControlsFrame, "PRESUPUESTOVENTA", "NOTAS", notasLayout );
	pControlsLayout->addLayout( cabeceraLayout );
	pControlsLayout->addLayout( cabecera2Layout );
	pControlsLayout->addLayout( presupuestoventadetLayout );
	alignLayout( rightSumaImportesLayout, false);
	alignLayout( rightDtoP100Layout, false);
	alignLayout( rightDescuentoLayout, false);
	alignLayout( rightBaseImponibleLayout, false);
	alignLayout( rightRecargoEquivalenciaLayout, false);
	alignLayout( rightIVALayout, false);
	alignLayout( rightTotalLayout, false);
	pControlsLayout->addLayout( desgloseivaLayout );
	pControlsLayout->addLayout( notasLayout );
/*>>>>>FRMEDITPRESUPUESTOVENTA_INIT_CONTROLS*/

    QGridLayout *gridlayout = new QGridLayout();
    gridlayout->addLayout( rightSumaImportesLayout, 0, 1 );
    gridlayout->addLayout( rightDtoP100Layout, 0, 2 );
    gridlayout->addLayout( rightDescuentoLayout, 0, 3 );
    gridlayout->addLayout( rightBaseImponibleLayout, 1, 0 );
    gridlayout->addLayout( rightIVALayout, 1, 1 );
    gridlayout->addLayout( rightRecargoEquivalenciaLayout, 1, 2 );
    gridlayout->addLayout( rightTotalLayout, 1, 3 );
    pControlsLayout->addLayout( gridlayout );

    editNumero->setMustBeReadOnly( true );
    editSumaImportes->setMustBeReadOnly( true );
    editBaseImponible->setMustBeReadOnly( true );
    editRecargoEquivalencia->setMustBeReadOnly( true );
    editIVA->setMustBeReadOnly( true );
    editNumero->setWidthInChars( 15 );
    editTipoDocCodigo->setWidthInChars(8);
    editClienteCodigo->setWidthInChars(8);
    editAgenteCodigo->setWidthInChars(8);
    editNotas->setHeightInLines(3);
    editNotas->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Minimum);
}

void FrmEditPresupuestoVenta::scatterFields()
{
    /*<<<<<FRMEDITPRESUPUESTOVENTA_SCATTER*/
	editFecha->setText(getRecPresupuestoVenta()->getValue("FECHA").toDate());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editFecha;
	editContador->setText(getRecPresupuestoVenta()->getValue("CONTADOR").toInt());
	editNumero->setText(getRecPresupuestoVenta()->getValue("NUMERO").toString());
	comboIVADetallado->setCurrentItemByValue(getRecPresupuestoVenta()->getValue("IVADETALLADO").toInt());
	editSumaImportes->setText(getRecPresupuestoVenta()->getValue("SUMAIMPORTES").toMoney());
	editDtoP100->setText(getRecPresupuestoVenta()->getValue("DTOP100").toDouble());
	editDescuento->setText(getRecPresupuestoVenta()->getValue("DESCUENTO").toMoney());
	editBaseImponible->setText(getRecPresupuestoVenta()->getValue("BASEIMPONIBLE").toMoney());
	editRecargoEquivalencia->setText(getRecPresupuestoVenta()->getValue("RECARGOEQUIVALENCIA").toMoney());
	editIVA->setText(getRecPresupuestoVenta()->getValue("IVA").toMoney());
	editTotal->setText(getRecPresupuestoVenta()->getValue("TOTAL").toMoney());
	editDesgloseIVA->setText(getRecPresupuestoVenta()->getValue("DESGLOSEIVA").toString());
	editNotas->setText(getRecPresupuestoVenta()->getValue("NOTAS").toString());
	scatterTipoDoc();
	scatterCliente();
	scatterAgente();
/*>>>>>FRMEDITPRESUPUESTOVENTA_SCATTER*/
    if( isInserting() ) {
        if( !isDuplicating() && editFecha->toDate().isNull() )
            editFecha->setText( ModuleInstance->getWorkingDate() );
        if( isDuplicating() ) {
            editTipoDocCodigo->setJustEdited( true );
            validateFields( editTipoDocCodigo, 0 );
        }
        pFocusWidget = editFecha;
    } else if( isEditing() ) {
        pFocusWidget = pFrmPresupuestoVentaDet;
    }
    pFrmPresupuestoVentaDet->addDetailIfNeeded();
    validateFields( comboIVADetallado, 0 ); // Para mostrar u ocultar el recargo de equivalencia
}

void FrmEditPresupuestoVenta::gatherFields()
{
    /*<<<<<FRMEDITPRESUPUESTOVENTA_GATHER*/
	getRecPresupuestoVenta()->setValue( "FECHA", editFecha->toDate());
	getRecPresupuestoVenta()->setValue( "TIPODOC_ID", getRecTipoDoc()->getRecordID() );
	getRecPresupuestoVenta()->setValue( "CONTADOR", editContador->toInt());
	getRecPresupuestoVenta()->setValue( "NUMERO", editNumero->toString());
	getRecPresupuestoVenta()->setValue( "CLIENTE_ID", getRecCliente()->getRecordID() );
	getRecPresupuestoVenta()->setValue( "IVADETALLADO", comboIVADetallado->getCurrentItemValue());
	getRecPresupuestoVenta()->setValue( "AGENTE_ID", getRecAgente()->getRecordID() );
	getRecPresupuestoVenta()->setValue( "SUMAIMPORTES", editSumaImportes->toMoney());
	getRecPresupuestoVenta()->setValue( "DTOP100", editDtoP100->toDouble());
	getRecPresupuestoVenta()->setValue( "DESCUENTO", editDescuento->toMoney());
	getRecPresupuestoVenta()->setValue( "BASEIMPONIBLE", editBaseImponible->toMoney());
	getRecPresupuestoVenta()->setValue( "RECARGOEQUIVALENCIA", editRecargoEquivalencia->toMoney());
	getRecPresupuestoVenta()->setValue( "IVA", editIVA->toMoney());
	getRecPresupuestoVenta()->setValue( "TOTAL", editTotal->toMoney());
	getRecPresupuestoVenta()->setValue( "DESGLOSEIVA", editDesgloseIVA->toString());
	getRecPresupuestoVenta()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITPRESUPUESTOVENTA_GATHER*/
    ModuleInstance->setWorkingDate( editFecha->toDate() );
}

void FrmEditPresupuestoVenta::scatterCliente()
{
    /*<<<<<FRMEDITPRESUPUESTOVENTA_SCATTER_CLIENTE*/
	editClienteCodigo->setText( getRecCliente()->getValue("CODIGO") );
	editClienteRazonSocial->setText( getRecCliente()->getValue("RAZONSOCIAL") );
/*>>>>>FRMEDITPRESUPUESTOVENTA_SCATTER_CLIENTE*/
    /*<<<<<FRMEDITPRESUPUESTOVENTA_CABECERA_SCATTER_CLIENTE*/
	if( editClienteCodigo->isJustEdited() ) {
		if( getRecCliente()->getValue("IVADetallado").toInt() != 0 )
			comboIVADetallado->setCurrentItemByValue( getRecCliente()->getValue("IVADetallado").toInt() );
		if( getRecCliente()->getRecAgente()->getRecordID() != 0 )
			searchAgenteCodigo->setValue( getRecCliente()->getRecAgente()->getValue("CODIGO") );
	}
/*>>>>>FRMEDITPRESUPUESTOVENTA_CABECERA_SCATTER_CLIENTE*/
}

void FrmEditPresupuestoVenta::pushClienteCodigo_clicked()
{
    /*<<<<<FRMEDITPRESUPUESTOVENTA_PUSH_CLIENTE_CODIGO_CLICKED*/
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
/*>>>>>FRMEDITPRESUPUESTOVENTA_PUSH_CLIENTE_CODIGO_CLICKED*/
}

void FrmEditPresupuestoVenta::scatterAgente()
{
    /*<<<<<FRMEDITPRESUPUESTOVENTA_SCATTER_AGENTE*/
	editAgenteCodigo->setText( getRecAgente()->getValue("CODIGO") );
	editAgenteRazonSocial->setText( getRecAgente()->getValue("RAZONSOCIAL") );
/*>>>>>FRMEDITPRESUPUESTOVENTA_SCATTER_AGENTE*/
}
void FrmEditPresupuestoVenta::pushAgenteCodigo_clicked()
{
    /*<<<<<FRMEDITPRESUPUESTOVENTA_PUSH_AGENTE_CODIGO_CLICKED*/
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
/*>>>>>FRMEDITPRESUPUESTOVENTA_PUSH_AGENTE_CODIGO_CLICKED*/
}
void FrmEditPresupuestoVenta::specialControlKeyPressed( QWidget *sender, char key )
{
    /*<<<<<FRMEDITPRESUPUESTOVENTA_SPECIALACTION*/
	mControlKeyPressed = key;
	FrmEditRecMaster::specialControlKeyPressed(sender,key); // calls the behaviors
	if( sender == editTipoDocCodigo )
		pushTipoDocCodigo_clicked();
	if( sender == editClienteCodigo )
		pushClienteCodigo_clicked();
	if( sender == editAgenteCodigo )
		pushAgenteCodigo_clicked();
	mControlKeyPressed = '\0';
/*>>>>>FRMEDITPRESUPUESTOVENTA_SPECIALACTION*/
}

void FrmEditPresupuestoVenta::scatterTipoDoc()
{
    /*<<<<<FRMEDITPRESUPUESTOVENTA_SCATTER_TIPODOC*/
	editTipoDocCodigo->setText( getRecTipoDoc()->getValue("CODIGO") );
	editTipoDocNombre->setText( getRecTipoDoc()->getValue("NOMBRE") );
/*>>>>>FRMEDITPRESUPUESTOVENTA_SCATTER_TIPODOC*/
    if( editTipoDocCodigo->isJustEdited() ) {
        if( editAgenteCodigo->toString().isEmpty() )
            searchAgenteCodigo->setValue( getRecTipoDoc()->getRecAgente()->getValue("CODIGO") );
        if( getRecTipoDoc()->getValue("IVADetallado").toInt() != 0 )
            comboIVADetallado->setCurrentItemByValue( getRecTipoDoc()->getValue("IVADetallado").toInt() );
        editContador->setText( ModuleInstance->getMaxContador( "PRESUPUESTOVENTA",
                               getRecord()->getValue("EMPRESA_ID").toInt(),
                               0, getRecTipoDoc()->getValue("SERIE").toString() ) );
        editContador->setJustEdited( true );
        validate( editContador, 0 );
    }
}

void FrmEditPresupuestoVenta::pushTipoDocCodigo_clicked()
{
    /*<<<<<FRMEDITPRESUPUESTOVENTA_PUSH_TIPODOC_CODIGO_CLICKED*/
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
/*>>>>>FRMEDITPRESUPUESTOVENTA_PUSH_TIPODOC_CODIGO_CLICKED*/
}


void FrmEditPresupuestoVenta::validateFields( QWidget *sender, bool *isvalid, ValidResult *ir )
{
    /*<<<<<FRMEDITPRESUPUESTOVENTA_VALIDATE*/
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
	if( focusWidget() != pushAgenteCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editAgenteCodigo, editAgenteRazonSocial,
		getRecAgente(), "CODIGO", "RAZONSOCIAL", Xtring::null, dbApplication::SeekCodeFlags( dbApplication::InsertIfNotFound )) )
		scatterAgente();
/*>>>>>FRMEDITPRESUPUESTOVENTA_VALIDATE*/
    /*<<<<<FRMEDITPRESUPUESTOVENTA_CABECERA_VALIDATE*/
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
/*>>>>>FRMEDITPRESUPUESTOVENTA_CABECERA_VALIDATE*/
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
    if ( !ir ) {
        showValidMessages( isvalid, *validresult, sender );
        delete validresult;
    }
}

/*<<<<<FRMEDITPRESUPUESTOVENTA_CABECERA_UPDATEFROMDETAILS*/
void FrmEditPresupuestoVenta::updateFromDetails( class FrmEditRecDetail *detailfrm )
{
	actTotales();
	numeraLineas();
/*>>>>>FRMEDITPRESUPUESTOVENTA_CABECERA_UPDATEFROMDETAILS*/
}

/*<<<<<FRMEDITPRESUPUESTOVENTA_CABECERA_NUMERALINEAS*/
void FrmEditPresupuestoVenta::numeraLineas()
{
	dbRecordList *reclst = getRecPresupuestoVenta()->getListPresupuestoVentaDet();
	for ( unsigned int i = 0; i < reclst->size(); i++ ) {
		RecPresupuestoVentaDet *detalle = static_cast<RecPresupuestoVentaDet *>( reclst->at( i ) );
		if( !detalle->isEmpty() ) // No numerar detalles vacíos 
			detalle->setValue( "NLINEA", i+1 );
	}
/*>>>>>FRMEDITPRESUPUESTOVENTA_CABECERA_NUMERALINEAS*/
}

/*<<<<<FRMEDITPRESUPUESTOVENTA_CABECERA_ACTTOTALES*/
void FrmEditPresupuestoVenta::actTotales()
{
	getRecPresupuestoVenta()->gather( this );
	getRecPresupuestoVenta()->actTotales();
	getRecPresupuestoVenta()->scatter( this );
/*>>>>>FRMEDITPRESUPUESTOVENTA_CABECERA_ACTTOTALES*/
}


/*<<<<<FRMEDITPRESUPUESTOVENTA_FIN*/
} // namespace factu
} // namespace gong
/*>>>>>FRMEDITPRESUPUESTOVENTA_FIN*/

// Los presupuestos de venta no tienen empresa
#if 0
/*<<<<<FRMEDITPRESUPUESTOVENTA_CABECERA_GENNUMDOC*/
void FrmEditPresupuestoVenta::genNumeroDocumento()
{
		editContador->setText( ModuleInstance->getMaxContador( "PRESUPUESTOVENTA",
			getRecord()->getValue("EMPRESA_ID").toInt(),
			ModuleInstance->getEmpresaModule()->getEjercicio(),
			getRecTipoDoc()->getValue("SERIE").toString() ) );
		editContador->setJustEdited( true );
		validateFields( editContador, 0 );
/*>>>>>FRMEDITPRESUPUESTOVENTA_CABECERA_GENNUMDOC*/
}
#endif
