/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de usuarias
// FIELD login string
// FIELD Password string
// FIELD Activo bool
// FIELD Notas text
// TYPE FrmEditRecMaster user::Usuaria
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITUSUARIA_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "userfrmeditusuaria.h"
/*>>>>>FRMEDITUSUARIA_INCLUDES*/

namespace gong {
namespace user {


/*<<<<<FRMEDITUSUARIA_CONSTRUCTOR*/
FrmEditUsuaria::FrmEditUsuaria(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditUsuaria" );
/*>>>>>FRMEDITUSUARIA_CONSTRUCTOR*/
/*<<<<<FRMEDITUSUARIA_INIT_CONTROLS*/
	QHBoxLayout *loginLayout = new QHBoxLayout(0, 0, 6, "loginLayout");
	QHBoxLayout *passwordLayout = new QHBoxLayout(0, 0, 6, "passwordLayout");
	QHBoxLayout *activoLayout = new QHBoxLayout(0, 0, 6, "activoLayout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
	editlogin = addEditField( pControlsFrame, "USUARIA", "LOGIN", loginLayout );
	editPassword = addEditField( pControlsFrame, "USUARIA", "PASSWORD", passwordLayout );
	checkActivo = addCheckField( pControlsFrame, "USUARIA", "ACTIVO", activoLayout );
	editNotas = addTextField( pControlsFrame, "USUARIA", "NOTAS", notasLayout );
	pControlsLayout->addLayout( loginLayout );
	pControlsLayout->addLayout( passwordLayout );
	pControlsLayout->addLayout( activoLayout );
	pControlsLayout->addLayout( notasLayout );
/*>>>>>FRMEDITUSUARIA_INIT_CONTROLS*/
}

void FrmEditUsuaria::scatterFields()
{
/*<<<<<FRMEDITUSUARIA_SCATTER*/
	editlogin->setText(getRecUsuaria()->getValue("LOGIN").toString());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editlogin;
	editPassword->setText(getRecUsuaria()->getValue("PASSWORD").toString());
	checkActivo->setChecked(getRecUsuaria()->getValue("ACTIVO").toBool());
	editNotas->setText(getRecUsuaria()->getValue("NOTAS").toString());
/*>>>>>FRMEDITUSUARIA_SCATTER*/
}

void FrmEditUsuaria::gatherFields()
{
/*<<<<<FRMEDITUSUARIA_GATHER*/
	getRecUsuaria()->setValue( "LOGIN", editlogin->toString());
	getRecUsuaria()->setValue( "PASSWORD", editPassword->toString());
	getRecUsuaria()->setValue( "ACTIVO", checkActivo->isChecked());
	getRecUsuaria()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITUSUARIA_GATHER*/
}

void FrmEditUsuaria::validateFields(QWidget *sender, bool *isvalid, ValidResult *ir)
{
/*<<<<<FRMEDITUSUARIA_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	if( !sender && !pRecord->validate( ValidResult::editing) ) {
		validresult->append( pRecord->getErrors() );
		*isvalid = false;
	}
/*>>>>>FRMEDITUSUARIA_VALIDATE*/
	if( !ir ) {
		showValidMessages(isvalid, *validresult, sender);
		delete validresult;
	}
}

/*<<<<<FRMEDITUSUARIA_FIN*/
} // namespace user
} // namespace gong
/*>>>>>FRMEDITUSUARIA_FIN*/

