// FIELD Cliente_ID Reference(Cliente,Codigo,RazonSocial)
/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de sesiones de TPV
// FIELD Codigo int - leftcodigo
// FIELD NumeroTPV int - leftcodigo
// FIELD Inicio DateTime - leftfechas
// FIELD Fin DateTime - leftfechas
// FIELD CajaInicial Money
// FIELD MesasSVG String
// FIELD Notas text
// TYPE FrmEditRecMaster tpv::SesionTPV IncCode
/*>>>>>MODULE_INFO*/

#include <gongpushbutton.h>
/*<<<<<FRMEDITSESIONTPV_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "tpvfrmeditsesiontpv.h"
/*>>>>>FRMEDITSESIONTPV_INCLUDES*/
#include <factureccliente.h>
#include "tpvmodule.h"

namespace gong {
namespace tpv {

/*<<<<<FRMEDITSESIONTPV_CONSTRUCTOR*/
FrmEditSesionTPV::FrmEditSesionTPV(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditSesionTPV" );
/*>>>>>FRMEDITSESIONTPV_CONSTRUCTOR*/
	QHBoxLayout *cliente_idLayout = new QHBoxLayout(0, 0, 6, "cliente_idLayout");
/*<<<<<FRMEDITSESIONTPV_INIT_CONTROLS*/
	QHBoxLayout *leftcodigoLayout = new QHBoxLayout(0, 0, 6, "leftcodigoLayout");
	QHBoxLayout *leftfechasLayout = new QHBoxLayout(0, 0, 6, "leftfechasLayout");
	QHBoxLayout *cajainicialLayout = new QHBoxLayout(0, 0, 6, "cajainicialLayout");
	QHBoxLayout *mesassvgLayout = new QHBoxLayout(0, 0, 6, "mesassvgLayout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
	editCodigo = addEditField( pControlsFrame, "SESIONTPV", "CODIGO", leftcodigoLayout );
	editNumeroTPV = addEditField( pControlsFrame, "SESIONTPV", "NUMEROTPV", leftcodigoLayout );
	editInicio = addEditField( pControlsFrame, "SESIONTPV", "INICIO", leftfechasLayout );
	editFin = addEditField( pControlsFrame, "SESIONTPV", "FIN", leftfechasLayout );
	editCajaInicial = addEditField( pControlsFrame, "SESIONTPV", "CAJAINICIAL", cajainicialLayout );
	editMesasSVG = addEditField( pControlsFrame, "SESIONTPV", "MESASSVG", mesassvgLayout );
	editNotas = addTextField( pControlsFrame, "SESIONTPV", "NOTAS", notasLayout );
	pControlsLayout->addLayout( leftcodigoLayout );
	alignLayout( leftcodigoLayout, true );
	pControlsLayout->addLayout( leftfechasLayout );
	alignLayout( leftfechasLayout, true );
	pControlsLayout->addLayout( cajainicialLayout );
	pControlsLayout->addLayout( mesassvgLayout );
	pControlsLayout->addLayout( notasLayout );
/*>>>>>FRMEDITSESIONTPV_INIT_CONTROLS*/
	pRecCliente = static_cast<RecCliente *>( DBAPP->createRecord( "CLIENTE" ) );
	searchClienteCodigo = addSearchField( pControlsFrame, "CLIENTE_ID", "CLIENTE", "CODIGO", "RAZONSOCIAL", cliente_idLayout );
	pushClienteCodigo = searchClienteCodigo->getButton();
	connect( pushClienteCodigo, SIGNAL( clicked() ), this, SLOT( pushClienteCodigo_clicked() ) );
	editClienteCodigo = searchClienteCodigo->getEditCode();
	editClienteRazonSocial = searchClienteCodigo->getEditDesc();
	pControlsLayout->addLayout( cliente_idLayout );

	editFin->setMustBeReadOnly( true );
	QHBoxLayout *botonesLayout = new QHBoxLayout(0, 0, 6, "codigoLayout");
	pushReabrir = new PushButton( pControlsFrame, _("Reabrir") );
	connect( pushReabrir, SIGNAL( clicked() ), this, SLOT( slotPushReabrir() ) );
	botonesLayout->addWidget( pushReabrir );
	pushCerrar = new PushButton( pControlsFrame, _("Cerrar") );
	connect( pushCerrar, SIGNAL( clicked() ), this, SLOT( slotPushCerrar() ) );
	botonesLayout->addWidget( pushCerrar );
	pControlsLayout->addLayout( botonesLayout );
	pushZ = new PushButton( this, _("Z") );
	connect( pushZ, SIGNAL( clicked() ), this, SLOT( slotPushZ() ) );
	pButtonsLayout->insertWidget( 0, pushZ );
}

FrmEditSesionTPV::~FrmEditSesionTPV()
{
	delete pRecCliente;
}

void FrmEditSesionTPV::scatterFields()
{
/*<<<<<FRMEDITSESIONTPV_SCATTER*/
	editCodigo->setText(getRecSesionTPV()->getValue("CODIGO").toInt());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editCodigo;
	editNumeroTPV->setText(getRecSesionTPV()->getValue("NUMEROTPV").toInt());
	editInicio->setText(getRecSesionTPV()->getValue("INICIO").toDateTime());
	editFin->setText(getRecSesionTPV()->getValue("FIN").toDateTime());
	editCajaInicial->setText(getRecSesionTPV()->getValue("CAJAINICIAL").toMoney());
	editMesasSVG->setText(getRecSesionTPV()->getValue("MESASSVG").toString());
	editNotas->setText(getRecSesionTPV()->getValue("NOTAS").toString());
	if( isInserting() && editCodigo->toInt() == 0 ) {
		editCodigo->setText( getRecord()->selectNextInt( "CODIGO" ) );
	}
/*>>>>>FRMEDITSESIONTPV_SCATTER*/
	if( isInserting() || editClienteCodigo->toInt() == 0 ) {
		searchClienteCodigo->setValue( ModuleInstance->getModuleSetting("CODIGO_CLIENTE" ).toInt() );
	}
	scatterCliente();
	if( editNumeroTPV->toInt() == 0 )
		editNumeroTPV->setText( ModuleInstance->getModuleSetting("NUMERO_TPV" ).toInt() );
	if( isInserting() && editInicio->toDate().isNull() )
		editInicio->setText( DateTime::currentDateTime() );
	pFocusWidget = editInicio;
}

void FrmEditSesionTPV::gatherFields()
{
/*<<<<<FRMEDITSESIONTPV_GATHER*/
	getRecSesionTPV()->setValue( "CODIGO", editCodigo->toInt());
	getRecSesionTPV()->setValue( "NUMEROTPV", editNumeroTPV->toInt());
	getRecSesionTPV()->setValue( "INICIO", editInicio->toDateTime());
	getRecSesionTPV()->setValue( "FIN", editFin->toDateTime());
	getRecSesionTPV()->setValue( "CAJAINICIAL", editCajaInicial->toMoney());
	getRecSesionTPV()->setValue( "MESASSVG", editMesasSVG->toString());
	getRecSesionTPV()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITSESIONTPV_GATHER*/
	getRecSesionTPV()->setValue( "CLIENTE_ID", getRecCliente()->getRecordID() );
}

void FrmEditSesionTPV::validateFields(QWidget *sender, bool *isvalid, ValidResult *ir)
{
/*<<<<<FRMEDITSESIONTPV_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	if( !sender && !pRecord->validate( ValidResult::editing) ) {
		validresult->append( pRecord->getErrors() );
		*isvalid = false;
	}
/*>>>>>FRMEDITSESIONTPV_VALIDATE*/
	if( focusWidget() != pushClienteCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editClienteCodigo, editClienteRazonSocial,
		getRecCliente(), "CODIGO", "RAZONSOCIAL", Xtring::null) )
		scatterCliente();
	if( !sender || (sender == editNumeroTPV && editNumeroTPV->isJustEdited() ) ) {
		if( editNumeroTPV->toInt() == 0 ) {
			validresult->addError( _("El número de TPV no puede ser 0."),
										"NUMEROTPV" );
			*isvalid = false;
		}
	}
	if( !ir ) {
		showValidMessages(isvalid, *validresult, sender);
		delete validresult;
	}
}

void FrmEditSesionTPV::slotPushCerrar()
{
	editFin->setText( DateTime::currentDateTime() );
	updateStatus( true );
}

void FrmEditSesionTPV::slotPushReabrir()
{
	editFin->setText( Xtring::null );
	updateStatus( true );
}

void FrmEditSesionTPV::slotPushZ()
{
	if( mMustRead )
		read();
	ModuleInstance->printZ( getRecSesionTPV()->getRecordID() );
}

void FrmEditSesionTPV::updateStatus( bool callbehaviors )
{
	FrmEditRecMaster::updateStatus(callbehaviors);
	pushReabrir->setEnabled( getRecSesionTPV()->getRecordID() != 0
		&& !editFin->toDateTime().isNull() );
	pushCerrar->setEnabled( getRecSesionTPV()->getRecordID() != 0
		&& editFin->toDateTime().isNull() );
	if( getDataTable() )
		pushZ->setEnabled( getDataTable()->numRows() != 0 );
}

void FrmEditSesionTPV::scatterCliente()
{
/*<<<<<FRMEDITSESIONTPV_SCATTER_CLIENTE*/
	editClienteCodigo->setText( getRecCliente()->getValue("CODIGO") );
	editClienteRazonSocial->setText( getRecCliente()->getValue("RAZONSOCIAL") );
/*>>>>>FRMEDITSESIONTPV_SCATTER_CLIENTE*/
}

void FrmEditSesionTPV::pushClienteCodigo_clicked()
{
/*<<<<<FRMEDITSESIONTPV_PUSH_CLIENTE_CODIGO_CLICKED*/
	char action = mControlKeyPressed;
	if( !isEditing() )
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
				editClienteCodigo->setFocus();
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
					editClienteCodigo->setFocus();
				}
			}
			break;
		case 'E':
			{
				editClienteCodigo->setJustEdited( false );
				DBAPP->createClient( DBAPP->createEditForm(this,
					getRecCliente(), 0, DataTable::selecting,
				dbApplication::simpleEdition, this ) );
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
				editClienteCodigo->setFocus();
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITSESIONTPV_PUSH_CLIENTE_CODIGO_CLICKED*/
}

void FrmEditSesionTPV::specialControlKeyPressed(QWidget *sender, char key)
{
/*<<<<<FRMEDITSESIONTPV_SPECIALACTION*/
	mControlKeyPressed = key;
	FrmEditRec::specialControlKeyPressed(sender,key); // calls the behaviors
	if( sender == editClienteCodigo )
		pushClienteCodigo_clicked();
	mControlKeyPressed = '\0';
/*>>>>>FRMEDITSESIONTPV_SPECIALACTION*/
}

/*<<<<<FRMEDITSESIONTPV_FIN*/
} // namespace tpv
} // namespace gong
/*>>>>>FRMEDITSESIONTPV_FIN*/


