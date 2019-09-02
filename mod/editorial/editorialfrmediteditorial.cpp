/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de editoriales
// FIELD Codigo int - codigo
// FIELD Contacto_ID Reference(contactos::Contacto,CIF,Nombre,dbRecord::InsertIfNotFound) - codigo
// FIELD RazonSocial string - razonsocial
// FIELD Notas text - notas
// TYPE FrmEditRecMaster editorial::Editorial IncCode descNotFound
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITEDITORIAL_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "editorialfrmediteditorial.h"
/*>>>>>FRMEDITEDITORIAL_INCLUDES*/

namespace gong {
namespace editorial {

/*<<<<<FRMEDITEDITORIAL_CONSTRUCTOR*/
FrmEditEditorial::FrmEditEditorial(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditEditorial" );
/*>>>>>FRMEDITEDITORIAL_CONSTRUCTOR*/
/*<<<<<FRMEDITEDITORIAL_INIT_CONTROLS*/
	QHBoxLayout *codigoLayout = new QHBoxLayout(0, 0, 6, "codigoLayout");
	QHBoxLayout *razonsocialLayout = new QHBoxLayout(0, 0, 6, "razonsocialLayout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
	editCodigo = addEditField( pControlsFrame, "EDITORIAL", "CODIGO", codigoLayout );

	searchContactoCIF = addSearchField( pControlsFrame, "CONTACTO_ID", "CONTACTO", "CIF", "NOMBRE", codigoLayout );
	pushContactoCIF = searchContactoCIF->getButton();
	connect( pushContactoCIF, SIGNAL( clicked() ), this, SLOT( pushContactoCIF_clicked() ) );
	editContactoCIF = searchContactoCIF->getEditCode();
	editContactoNombre = searchContactoCIF->getEditDesc();
	editRazonSocial = addEditField( pControlsFrame, "EDITORIAL", "RAZONSOCIAL", razonsocialLayout );
	editNotas = addTextField( pControlsFrame, "EDITORIAL", "NOTAS", notasLayout );
	pControlsLayout->addLayout( codigoLayout );
	pControlsLayout->addLayout( razonsocialLayout );
	pControlsLayout->addLayout( notasLayout );
/*>>>>>FRMEDITEDITORIAL_INIT_CONTROLS*/
}
void FrmEditEditorial::scatterFields()
{
	// Quitar el valor por defecto del cif del contacto
	if( getRecContacto()->isNew() )
		getRecContacto()->setValue("CIF", "");
/*<<<<<FRMEDITEDITORIAL_SCATTER*/
	editCodigo->setText(getRecEditorial()->getValue("CODIGO").toInt());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editCodigo;
	editRazonSocial->setText(getRecEditorial()->getValue("RAZONSOCIAL").toString());
	editNotas->setText(getRecEditorial()->getValue("NOTAS").toString());
	if( isInserting() && editCodigo->toInt() == 0 ) {
		editCodigo->setText( getRecord()->selectNextInt( "CODIGO" ) );
	}
	scatterContacto();
/*>>>>>FRMEDITEDITORIAL_SCATTER*/
	if( isInserting() && !DBAPP->codeNotFound().isEmpty() ) {
		editRazonSocial->setText( DBAPP->codeNotFound() );
		editRazonSocial->setJustEdited( true );
	}

}
void FrmEditEditorial::gatherFields()
{
/*<<<<<FRMEDITEDITORIAL_GATHER*/
	getRecEditorial()->setValue( "CODIGO", editCodigo->toInt());
	getRecEditorial()->setValue( "CONTACTO_ID", getRecContacto()->getRecordID() );
	getRecEditorial()->setValue( "RAZONSOCIAL", editRazonSocial->toString());
	getRecEditorial()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITEDITORIAL_GATHER*/
}
void FrmEditEditorial::validateFields(QWidget *sender, bool *isvalid, ValidResult *ir)
{
/*<<<<<FRMEDITEDITORIAL_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	if( !sender && !pRecord->validate( ValidResult::editing) ) {
		validresult->append( pRecord->getErrors() );
		*isvalid = false;
	}
	if( focusWidget() != pushContactoCIF) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editContactoCIF, editContactoNombre,
		getRecContacto(), "CIF", "NOMBRE", Xtring::null, dbRecord::SeekCodeFlags( dbRecord::InsertIfNotFound )) )
		scatterContacto();
/*>>>>>FRMEDITEDITORIAL_VALIDATE*/
	if( !validCodeAndDesc( sender, *validresult, editCodigo, editRazonSocial, "codigo", "razonsocial" ) )
		if( !sender )
			*isvalid = false;
    if ( sender == editContactoCIF && editContactoCIF->isJustEdited()
            && editRazonSocial->toString().isEmpty() ) {
        editRazonSocial->setText( editContactoNombre->toString() );
        editRazonSocial->setJustEdited( true );
    }
	if (!ir) {
		showValidMessages(isvalid, *validresult, sender);
		delete validresult;
	}
}

void FrmEditEditorial::scatterContacto()
{
/*<<<<<FRMEDITEDITORIAL_SCATTER_CONTACTO*/
	editContactoCIF->setText( getRecContacto()->getValue("CIF") );
	editContactoNombre->setText( getRecContacto()->getValue("NOMBRE") );
/*>>>>>FRMEDITEDITORIAL_SCATTER_CONTACTO*/
}

void FrmEditEditorial::pushContactoCIF_clicked()
{
/*<<<<<FRMEDITEDITORIAL_PUSH_CONTACTO_CIF_CLICKED*/
	char action = mControlKeyPressed;
	if( !isEditing() || searchContactoCIF->mustBeReadOnly() )
		action = 'E';
	switch( action ) {
		case 'F':
		case '\0':
			editContactoCIF->setJustEdited( false );
			editContactoCIF->setCancelling();
			if( DBAPP->choose(this, getRecContacto(), 0, dbApplication::editNone, this ) ) {
				setEdited(true);
				scatterContacto();
				editContactoCIF->setJustEdited( true );
				setWiseFocus(editContactoCIF);
			}
			break;
		case 'M':
			{
				if( getRecContacto()->getRecordID() ) {
					editContactoCIF->setJustEdited( false );
					if( DBAPP->editRecord(this,
							getRecContacto(), 0, DataTable::updating,
							dbApplication::simpleEdition, this ) ) {
						editContactoCIF->setJustEdited( true );
						scatterContacto();
					}
				setWiseFocus(editContactoCIF);
				}
			}
			break;
		case 'E':
			{
				if( getRecContacto()->getRecordID() != 0 ) {
					editContactoCIF->setJustEdited( false );
					DBAPP->createClient( DBAPP->createEditForm(this, getRecContacto(),
						0, DataTable::selecting, dbApplication::simpleEdition, this ) );
				}
			}
			break;
		case 'A':
			{
				contactos::RecContacto *tmprec = static_cast<contactos::RecContacto *>(DBAPP->createRecord( "Contacto" ));
				editContactoCIF->setJustEdited( false );
				tmprec->clear( true ); // set default values
				DBAPP->setCodeNotFound( editContactoCIF->toString() );
				if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
					dbApplication::simpleEdition, this ) ) {
					editContactoCIF->setJustEdited( true );
					getRecContacto()->copyRecord( tmprec );
					scatterContacto();
				}
				setWiseFocus(editContactoCIF);
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITEDITORIAL_PUSH_CONTACTO_CIF_CLICKED*/
}

void FrmEditEditorial::specialControlKeyPressed( QWidget *sender, char key )
{
/*<<<<<FRMEDITEDITORIAL_SPECIALACTION*/
	mControlKeyPressed = key;
	FrmEditRecMaster::specialControlKeyPressed(sender,key); // calls the behaviors
	if( sender == editContactoCIF )
		pushContactoCIF_clicked();
	mControlKeyPressed = '\0';
/*>>>>>FRMEDITEDITORIAL_SPECIALACTION*/
}

/*<<<<<FRMEDITEDITORIAL_FIN*/
} // namespace editorial
} // namespace gong
/*>>>>>FRMEDITEDITORIAL_FIN*/
