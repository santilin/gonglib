/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de 
// FIELD Numero int - numero
// FIELD Automatico bool - numero
// FIELD Fecha date - numero
// FIELD TipoApunteTesoreria comboint - numero
// FIELD CuentaTesoreria_ID Reference(CuentaTesoreria,Codigo,Nombre)
// FIELD Debe Money - money
// FIELD Haber Money - money
// FIELD Concepto string
// FIELD Proyecto_ID Reference(empresa::Proyecto,Codigo,Nombre)
// FIELD Notas text
// TYPE FrmEditRecMaster tesoreria::ApunteTesoreria
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITAPUNTETESORERIA_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "tesoreriafrmeditapuntetesoreria.h"
/*>>>>>FRMEDITAPUNTETESORERIA_INCLUDES*/

namespace gong {
namespace tesoreria {

/*<<<<<FRMEDITAPUNTETESORERIA_CONSTRUCTOR*/
FrmEditApunteTesoreria::FrmEditApunteTesoreria(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditApunteTesoreria" );
/*>>>>>FRMEDITAPUNTETESORERIA_CONSTRUCTOR*/
/*<<<<<FRMEDITAPUNTETESORERIA_INIT_CONTROLS*/
	QHBoxLayout *numeroLayout = new QHBoxLayout(0, 0, 6, "numeroLayout");
	QHBoxLayout *cuentatesoreria_idLayout = new QHBoxLayout(0, 0, 6, "cuentatesoreria_idLayout");
	QHBoxLayout *moneyLayout = new QHBoxLayout(0, 0, 6, "moneyLayout");
	QHBoxLayout *conceptoLayout = new QHBoxLayout(0, 0, 6, "conceptoLayout");
	QHBoxLayout *proyecto_idLayout = new QHBoxLayout(0, 0, 6, "proyecto_idLayout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
	editNumero = addEditField( pControlsFrame, "APUNTETESORERIA", "NUMERO", numeroLayout );
	checkAutomatico = addCheckField( pControlsFrame, "APUNTETESORERIA", "AUTOMATICO", numeroLayout );
	editFecha = addEditField( pControlsFrame, "APUNTETESORERIA", "FECHA", numeroLayout );
	comboTipoApunteTesoreria = addComboIntField( pControlsFrame, "APUNTETESORERIA", "TIPOAPUNTETESORERIA", numeroLayout );

	searchCuentaTesoreriaCodigo = addSearchField( pControlsFrame, "CUENTATESORERIA_ID", "CUENTATESORERIA", "CODIGO", "NOMBRE", cuentatesoreria_idLayout );
	pushCuentaTesoreriaCodigo = searchCuentaTesoreriaCodigo->getButton();
	connect( pushCuentaTesoreriaCodigo, SIGNAL( clicked() ), this, SLOT( pushCuentaTesoreriaCodigo_clicked() ) );
	editCuentaTesoreriaCodigo = searchCuentaTesoreriaCodigo->getEditCode();
	editCuentaTesoreriaNombre = searchCuentaTesoreriaCodigo->getEditDesc();
	editDebe = addEditField( pControlsFrame, "APUNTETESORERIA", "DEBE", moneyLayout );
	editHaber = addEditField( pControlsFrame, "APUNTETESORERIA", "HABER", moneyLayout );
	editConcepto = addEditField( pControlsFrame, "APUNTETESORERIA", "CONCEPTO", conceptoLayout );

	searchProyectoCodigo = addSearchField( pControlsFrame, "PROYECTO_ID", "PROYECTO", "CODIGO", "NOMBRE", proyecto_idLayout );
	pushProyectoCodigo = searchProyectoCodigo->getButton();
	connect( pushProyectoCodigo, SIGNAL( clicked() ), this, SLOT( pushProyectoCodigo_clicked() ) );
	editProyectoCodigo = searchProyectoCodigo->getEditCode();
	editProyectoNombre = searchProyectoCodigo->getEditDesc();
	editNotas = addTextField( pControlsFrame, "APUNTETESORERIA", "NOTAS", notasLayout );
	pControlsLayout->addLayout( numeroLayout );
	pControlsLayout->addLayout( cuentatesoreria_idLayout );
	pControlsLayout->addLayout( moneyLayout );
	pControlsLayout->addLayout( conceptoLayout );
	pControlsLayout->addLayout( proyecto_idLayout );
	pControlsLayout->addLayout( notasLayout );
/*>>>>>FRMEDITAPUNTETESORERIA_INIT_CONTROLS*/
}

void FrmEditApunteTesoreria::scatterFields()
{
/*<<<<<FRMEDITAPUNTETESORERIA_SCATTER*/
	editNumero->setText(getRecApunteTesoreria()->getValue("NUMERO").toInt());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editNumero;
	checkAutomatico->setChecked(getRecApunteTesoreria()->getValue("AUTOMATICO").toBool());
	editFecha->setText(getRecApunteTesoreria()->getValue("FECHA").toDate());
	comboTipoApunteTesoreria->setCurrentItemByValue(getRecApunteTesoreria()->getValue("TIPOAPUNTETESORERIA").toInt());
	editDebe->setText(getRecApunteTesoreria()->getValue("DEBE").toMoney());
	editHaber->setText(getRecApunteTesoreria()->getValue("HABER").toMoney());
	editConcepto->setText(getRecApunteTesoreria()->getValue("CONCEPTO").toString());
	editNotas->setText(getRecApunteTesoreria()->getValue("NOTAS").toString());
	scatterCuentaTesoreria();
	scatterProyecto();
/*>>>>>FRMEDITAPUNTETESORERIA_SCATTER*/
}

void FrmEditApunteTesoreria::gatherFields()
{
/*<<<<<FRMEDITAPUNTETESORERIA_GATHER*/
	getRecApunteTesoreria()->setValue( "NUMERO", editNumero->toInt());
	getRecApunteTesoreria()->setValue( "AUTOMATICO", checkAutomatico->isChecked());
	getRecApunteTesoreria()->setValue( "FECHA", editFecha->toDate());
	getRecApunteTesoreria()->setValue( "TIPOAPUNTETESORERIA", comboTipoApunteTesoreria->getCurrentItemValue());
	getRecApunteTesoreria()->setValue( "CUENTATESORERIA_ID", getRecCuentaTesoreria()->getRecordID() );
	getRecApunteTesoreria()->setValue( "DEBE", editDebe->toMoney());
	getRecApunteTesoreria()->setValue( "HABER", editHaber->toMoney());
	getRecApunteTesoreria()->setValue( "CONCEPTO", editConcepto->toString());
	getRecApunteTesoreria()->setValue( "PROYECTO_ID", getRecProyecto()->getRecordID() );
	getRecApunteTesoreria()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITAPUNTETESORERIA_GATHER*/
}

void FrmEditApunteTesoreria::validateFields(QWidget *sender, bool *isvalid, ValidResult *ir)
{
/*<<<<<FRMEDITAPUNTETESORERIA_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	if( !sender && !pRecord->isValid( ValidResult::editing, validresult ) )
			*isvalid = false;
	if( focusWidget() != pushCuentaTesoreriaCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editCuentaTesoreriaCodigo, editCuentaTesoreriaNombre,
		getRecCuentaTesoreria(), "CODIGO", "NOMBRE", Xtring::null) )
		scatterCuentaTesoreria();
	if( focusWidget() != pushProyectoCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editProyectoCodigo, editProyectoNombre,
		getRecProyecto(), "CODIGO", "NOMBRE", Xtring::null) )
		scatterProyecto();
/*>>>>>FRMEDITAPUNTETESORERIA_VALIDATE*/
	if( !ir ) {
		showValidMessages(isvalid, *validresult, sender);
		delete validresult;
	}
}

void FrmEditApunteTesoreria::specialControlKeyPressed(QWidget *sender, char key)
{
/*<<<<<FRMEDITAPUNTETESORERIA_SPECIALACTION*/
	mControlKeyPressed = key;
	FrmEditRecMaster::specialControlKeyPressed(sender,key); // calls the behaviors
	if( sender == editCuentaTesoreriaCodigo )
		pushCuentaTesoreriaCodigo_clicked();
	if( sender == editProyectoCodigo )
		pushProyectoCodigo_clicked();
	mControlKeyPressed = '\0';
/*>>>>>FRMEDITAPUNTETESORERIA_SPECIALACTION*/
}

void FrmEditApunteTesoreria::scatterProyecto()
{
/*<<<<<FRMEDITAPUNTETESORERIA_SCATTER_PROYECTO*/
	editProyectoCodigo->setText( getRecProyecto()->getValue("CODIGO") );
	editProyectoNombre->setText( getRecProyecto()->getValue("NOMBRE") );
/*>>>>>FRMEDITAPUNTETESORERIA_SCATTER_PROYECTO*/
}
void FrmEditApunteTesoreria::pushProyectoCodigo_clicked()
{
/*<<<<<FRMEDITAPUNTETESORERIA_PUSH_PROYECTO_CODIGO_CLICKED*/
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
/*>>>>>FRMEDITAPUNTETESORERIA_PUSH_PROYECTO_CODIGO_CLICKED*/
}

void FrmEditApunteTesoreria::pushCuentaTesoreriaCodigo_clicked()
{
/*<<<<<FRMEDITAPUNTETESORERIA_PUSH_CUENTATESORERIA_CODIGO_CLICKED*/
	char action = mControlKeyPressed;
	if( !isEditing() || searchCuentaTesoreriaCodigo->mustBeReadOnly() )
		action = 'E';
	switch( action ) {
		case 'F':
		case '\0':
			editCuentaTesoreriaCodigo->setJustEdited( false );
			editCuentaTesoreriaCodigo->setCancelling();
			if( DBAPP->choose(this, getRecCuentaTesoreria(), 0, dbApplication::editNone, this ) ) {
				setEdited(true);
				scatterCuentaTesoreria();
				editCuentaTesoreriaCodigo->setJustEdited( true );
				setWiseFocus(editCuentaTesoreriaCodigo);
			}
			break;
		case 'M':
			{
				if( getRecCuentaTesoreria()->getRecordID() ) {
					editCuentaTesoreriaCodigo->setJustEdited( false );
					if( DBAPP->editRecord(this,
							getRecCuentaTesoreria(), 0, DataTable::updating,
							dbApplication::simpleEdition, this ) ) {
						editCuentaTesoreriaCodigo->setJustEdited( true );
						scatterCuentaTesoreria();
					}
				setWiseFocus(editCuentaTesoreriaCodigo);
				}
			}
			break;
		case 'E':
			{
				if( getRecCuentaTesoreria()->getRecordID() != 0 ) {
					editCuentaTesoreriaCodigo->setJustEdited( false );
					DBAPP->getMainWindow()->createClient( DBAPP->createEditForm(this, getRecCuentaTesoreria(),
						0, DataTable::selecting, dbApplication::simpleEdition, this ) );
				}
			}
			break;
		case 'A':
			{
				RecCuentaTesoreria *tmprec = static_cast<RecCuentaTesoreria *>(DBAPP->createRecord( "CuentaTesoreria" ));
				editCuentaTesoreriaCodigo->setJustEdited( false );
				tmprec->clear( true ); // set default values
				DBAPP->setCodeNotFound( editCuentaTesoreriaCodigo->toString() );
				if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
					dbApplication::simpleEdition, this ) ) {
					editCuentaTesoreriaCodigo->setJustEdited( true );
					getRecCuentaTesoreria()->copyRecord( tmprec );
					scatterCuentaTesoreria();
				}
				setWiseFocus(editCuentaTesoreriaCodigo);
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITAPUNTETESORERIA_PUSH_CUENTATESORERIA_CODIGO_CLICKED*/
}


void FrmEditApunteTesoreria::scatterCuentaTesoreria()
{
/*<<<<<FRMEDITAPUNTETESORERIA_SCATTER_CUENTATESORERIA*/
	editCuentaTesoreriaCodigo->setText( getRecCuentaTesoreria()->getValue("CODIGO") );
	editCuentaTesoreriaNombre->setText( getRecCuentaTesoreria()->getValue("NOMBRE") );
/*>>>>>FRMEDITAPUNTETESORERIA_SCATTER_CUENTATESORERIA*/
}

/*<<<<<FRMEDITAPUNTETESORERIA_FIN*/
} // namespace tesoreria
} // namespace gong
/*>>>>>FRMEDITAPUNTETESORERIA_FIN*/