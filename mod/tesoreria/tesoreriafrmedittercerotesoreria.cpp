/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de terceros de tesoreria
// FIELD Codigo int
// FIELD Nombre string
// FIELD Notas text
// TYPE FrmEditRecMaster tesoreria::TerceroTesoreria
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITTERCEROTESORERIA_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "tesoreriafrmedittercerotesoreria.h"
/*>>>>>FRMEDITTERCEROTESORERIA_INCLUDES*/

namespace gong {
namespace tesoreria {

/*<<<<<FRMEDITTERCEROTESORERIA_CONSTRUCTOR*/
FrmEditTerceroTesoreria::FrmEditTerceroTesoreria(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditTerceroTesoreria" );
/*>>>>>FRMEDITTERCEROTESORERIA_CONSTRUCTOR*/
    /*<<<<<FRMEDITTERCEROTESORERIA_INIT_CONTROLS*/
	QHBoxLayout *codigoLayout = new QHBoxLayout(0, 0, 6, "codigoLayout");
	QHBoxLayout *nombreLayout = new QHBoxLayout(0, 0, 6, "nombreLayout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
	editCodigo = addEditField( pControlsFrame, "TERCEROTESORERIA", "CODIGO", codigoLayout );
	editNombre = addEditField( pControlsFrame, "TERCEROTESORERIA", "NOMBRE", nombreLayout );
	editNotas = addTextField( pControlsFrame, "TERCEROTESORERIA", "NOTAS", notasLayout );
	pControlsLayout->addLayout( codigoLayout );
	pControlsLayout->addLayout( nombreLayout );
	pControlsLayout->addLayout( notasLayout );
/*>>>>>FRMEDITTERCEROTESORERIA_INIT_CONTROLS*/
}

void FrmEditTerceroTesoreria::scatterFields()
{
    /*<<<<<FRMEDITTERCEROTESORERIA_SCATTER*/
	editCodigo->setText(getRecTerceroTesoreria()->getValue("CODIGO").toInt());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editCodigo;
	editNombre->setText(getRecTerceroTesoreria()->getValue("NOMBRE").toString());
	editNotas->setText(getRecTerceroTesoreria()->getValue("NOTAS").toString());
/*>>>>>FRMEDITTERCEROTESORERIA_SCATTER*/
}

void FrmEditTerceroTesoreria::gatherFields()
{
    /*<<<<<FRMEDITTERCEROTESORERIA_GATHER*/
	getRecTerceroTesoreria()->setValue( "CODIGO", editCodigo->toInt());
	getRecTerceroTesoreria()->setValue( "NOMBRE", editNombre->toString());
	getRecTerceroTesoreria()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITTERCEROTESORERIA_GATHER*/
}

void FrmEditTerceroTesoreria::validateFields(QWidget *sender, bool *isvalid, ValidResult *ir)
{
    /*<<<<<FRMEDITTERCEROTESORERIA_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	if( !sender && !pRecord->validate( ValidResult::editing) ) {
		validresult->append( pRecord->getErrors() );
		*isvalid = false;
	}
/*>>>>>FRMEDITTERCEROTESORERIA_VALIDATE*/
    if( !ir ) {
        showValidMessages(isvalid, *validresult, sender);
        delete validresult;
    }
}

/*<<<<<FRMEDITTERCEROTESORERIA_FIN*/
} // namespace tesoreria
} // namespace gong
/*>>>>>FRMEDITTERCEROTESORERIA_FIN*/

