/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de proyectos
// FIELD Codigo int - codigo
// FIELD Nombre string - codigo
// FIELD Notas text
// TYPE FrmEditRecMaster empresa::Proyecto validCodeAndDesc IncCode
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITPROYECTO_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "empresafrmeditproyecto.h"
/*>>>>>FRMEDITPROYECTO_INCLUDES*/

namespace gong {
namespace empresa {

/*<<<<<FRMEDITPROYECTO_CONSTRUCTOR*/
FrmEditProyecto::FrmEditProyecto(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditProyecto" );
/*>>>>>FRMEDITPROYECTO_CONSTRUCTOR*/
    /*<<<<<FRMEDITPROYECTO_INIT_CONTROLS*/
	QHBoxLayout *codigoLayout = new QHBoxLayout(0, 0, 6, "codigoLayout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
	editCodigo = addEditField( pControlsFrame, "PROYECTO", "CODIGO", codigoLayout );
	editNombre = addEditField( pControlsFrame, "PROYECTO", "NOMBRE", codigoLayout );
	editNotas = addTextField( pControlsFrame, "PROYECTO", "NOTAS", notasLayout );
	pControlsLayout->addLayout( codigoLayout );
	pControlsLayout->addLayout( notasLayout );
/*>>>>>FRMEDITPROYECTO_INIT_CONTROLS*/
}

void FrmEditProyecto::scatterFields()
{
    /*<<<<<FRMEDITPROYECTO_SCATTER*/
	editCodigo->setText(getRecProyecto()->getValue("CODIGO").toInt());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editCodigo;
	editNombre->setText(getRecProyecto()->getValue("NOMBRE").toString());
	editNotas->setText(getRecProyecto()->getValue("NOTAS").toString());
	if( isInserting() && editCodigo->toInt() == 0 ) {
		editCodigo->setText( getRecord()->selectNextInt( "CODIGO" ) );
	}
/*>>>>>FRMEDITPROYECTO_SCATTER*/
}

void FrmEditProyecto::gatherFields()
{
    /*<<<<<FRMEDITPROYECTO_GATHER*/
	getRecProyecto()->setValue( "CODIGO", editCodigo->toInt());
	getRecProyecto()->setValue( "NOMBRE", editNombre->toString());
	getRecProyecto()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITPROYECTO_GATHER*/
}

void FrmEditProyecto::validateFields(QWidget *sender, bool *isvalid, ValidResult *ir)
{
    /*<<<<<FRMEDITPROYECTO_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	if( !sender && !pRecord->validate( ValidResult::editing) ) {
		validresult->append( pRecord->getErrors() );
		*isvalid = false;
	}
	if( !validCodeAndDesc( sender, *validresult, editCodigo, editNombre, "codigo", "nombre" ) )
		if( !sender )
			*isvalid = false;
/*>>>>>FRMEDITPROYECTO_VALIDATE*/
    if( !ir ) {
        showValidMessages(isvalid, *validresult, sender);
        delete validresult;
    }
}

/*<<<<<FRMEDITPROYECTO_FIN*/
} // namespace empresa
} // namespace gong
/*>>>>>FRMEDITPROYECTO_FIN*/

