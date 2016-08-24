/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de usuarias
// FIELD Name string
// FIELD Password string
// TYPE FrmEditRecMaster user::User
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITUSER_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "userfrmedituser.h"
/*>>>>>FRMEDITUSER_INCLUDES*/

namespace gong {
namespace user {


/*<<<<<FRMEDITUSER_CONSTRUCTOR*/
FrmEditUser::FrmEditUser(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditUser" );
/*>>>>>FRMEDITUSER_CONSTRUCTOR*/
/*<<<<<FRMEDITUSER_INIT_CONTROLS*/
	QHBoxLayout *nameLayout = new QHBoxLayout(0, 0, 6, "nameLayout");
	QHBoxLayout *passwordLayout = new QHBoxLayout(0, 0, 6, "passwordLayout");
	editName = addEditField( pControlsFrame, "USER", "NAME", nameLayout );
	editPassword = addEditField( pControlsFrame, "USER", "PASSWORD", passwordLayout );
	pControlsLayout->addLayout( nameLayout );
	pControlsLayout->addLayout( passwordLayout );
/*>>>>>FRMEDITUSER_INIT_CONTROLS*/
}

void FrmEditUser::scatterFields()
{
/*<<<<<FRMEDITUSER_SCATTER*/
	editName->setText(getRecUser()->getValue("NAME").toString());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editName;
	editPassword->setText(getRecUser()->getValue("PASSWORD").toString());
/*>>>>>FRMEDITUSER_SCATTER*/
}

void FrmEditUser::gatherFields()
{
/*<<<<<FRMEDITUSER_GATHER*/
	getRecUser()->setValue( "NAME", editName->toString());
	getRecUser()->setValue( "PASSWORD", editPassword->toString());
/*>>>>>FRMEDITUSER_GATHER*/
}

void FrmEditUser::validateFields(QWidget *sender, bool *isvalid, ValidResult *ir)
{
/*<<<<<FRMEDITUSER_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	if( !sender && !pRecord->validate( ValidResult::editing) ) {
		validresult->append( pRecord->getErrors() );
		*isvalid = false;
	}
/*>>>>>FRMEDITUSER_VALIDATE*/
	if( !ir ) {
		showValidMessages(isvalid, *validresult, sender);
		delete validresult;
	}
}

/*<<<<<FRMEDITUSER_FIN*/
} // namespace user
} // namespace gong
/*>>>>>FRMEDITUSER_FIN*/

