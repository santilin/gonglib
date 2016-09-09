/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de roles del módulo user
// FIELD Nombre string
// FIELD Notas text
// TYPE FrmEditRecMaster user::Rol
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITROL_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "userfrmeditrol.h"
/*>>>>>FRMEDITROL_INCLUDES*/

namespace gong {
namespace user {

/*<<<<<FRMEDITROL_CONSTRUCTOR*/
FrmEditRol::FrmEditRol(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditRol" );
/*>>>>>FRMEDITROL_CONSTRUCTOR*/
/*<<<<<FRMEDITROL_INIT_CONTROLS*/
	QHBoxLayout *nombreLayout = new QHBoxLayout(0, 0, 6, "nombreLayout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
	editNombre = addEditField( pControlsFrame, "ROL", "NOMBRE", nombreLayout );
	editNotas = addTextField( pControlsFrame, "ROL", "NOTAS", notasLayout );
	pControlsLayout->addLayout( nombreLayout );
	pControlsLayout->addLayout( notasLayout );
/*>>>>>FRMEDITROL_INIT_CONTROLS*/
}

void FrmEditRol::scatterFields()
{
/*<<<<<FRMEDITROL_SCATTER*/
	editNombre->setText(getRecRol()->getValue("NOMBRE").toString());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editNombre;
	editNotas->setText(getRecRol()->getValue("NOTAS").toString());
/*>>>>>FRMEDITROL_SCATTER*/
}

void FrmEditRol::gatherFields()
{
/*<<<<<FRMEDITROL_GATHER*/
	getRecRol()->setValue( "NOMBRE", editNombre->toString());
	getRecRol()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITROL_GATHER*/
}

void FrmEditRol::validateFields(QWidget *sender, bool *isvalid, ValidResult *ir)
{
/*<<<<<FRMEDITROL_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	if( !sender && !pRecord->validate( ValidResult::editing) ) {
		validresult->append( pRecord->getErrors() );
		*isvalid = false;
	}
/*>>>>>FRMEDITROL_VALIDATE*/
	if( !ir ) {
		showValidMessages(isvalid, *validresult, sender);
		delete validresult;
	}
}

/*<<<<<FRMEDITROL_FIN*/
} // namespace user
} // namespace gong
/*>>>>>FRMEDITROL_FIN*/

