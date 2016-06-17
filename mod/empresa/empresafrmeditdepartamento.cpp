/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de departamentos
// FIELD Codigo int - codigo
// FIELD Nombre String - codigo
// FIELD Notas text
// TYPE FrmEditRecMaster empresa::Departamento validCodeAndDesc IncCode CodeNotFound
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITDEPARTAMENTO_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "empresafrmeditdepartamento.h"
/*>>>>>FRMEDITDEPARTAMENTO_INCLUDES*/

namespace gong
{
namespace empresa {

/*<<<<<FRMEDITDEPARTAMENTO_CONSTRUCTOR*/
FrmEditDepartamento::FrmEditDepartamento(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditDepartamento" );
/*>>>>>FRMEDITDEPARTAMENTO_CONSTRUCTOR*/
    /*<<<<<FRMEDITDEPARTAMENTO_INIT_CONTROLS*/
	QHBoxLayout *codigoLayout = new QHBoxLayout(0, 0, 6, "codigoLayout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
	editCodigo = addEditField( pControlsFrame, "DEPARTAMENTO", "CODIGO", codigoLayout );
	editNombre = addEditField( pControlsFrame, "DEPARTAMENTO", "NOMBRE", codigoLayout );
	editNotas = addTextField( pControlsFrame, "DEPARTAMENTO", "NOTAS", notasLayout );
	pControlsLayout->addLayout( codigoLayout );
	pControlsLayout->addLayout( notasLayout );
/*>>>>>FRMEDITDEPARTAMENTO_INIT_CONTROLS*/
}
void FrmEditDepartamento::scatterFields()
{
    /*<<<<<FRMEDITDEPARTAMENTO_SCATTER*/
	editCodigo->setText(getRecDepartamento()->getValue("CODIGO").toInt());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editCodigo;
	editNombre->setText(getRecDepartamento()->getValue("NOMBRE").toString());
	editNotas->setText(getRecDepartamento()->getValue("NOTAS").toString());
	if( isInserting() && editCodigo->toInt() == 0 ) {
		editCodigo->setText( getRecord()->selectNextInt( "CODIGO" ) );
	}
	if( isInserting() && !isDuplicating() && !DBAPP->codeNotFound().isEmpty() ) {
		if( DBAPP->codeNotFound().toInt() != 0 ) {
			editCodigo->setText( DBAPP->codeNotFound() );
			editCodigo->setJustEdited( true );
		} else {
		editNombre->setText( DBAPP->codeNotFound() );
		editNombre->setJustEdited( true );
		}
	}
/*>>>>>FRMEDITDEPARTAMENTO_SCATTER*/
}

void FrmEditDepartamento::gatherFields()
{
    /*<<<<<FRMEDITDEPARTAMENTO_GATHER*/
	getRecDepartamento()->setValue( "CODIGO", editCodigo->toInt());
	getRecDepartamento()->setValue( "NOMBRE", editNombre->toString());
	getRecDepartamento()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITDEPARTAMENTO_GATHER*/
}

void FrmEditDepartamento::validateFields( QWidget *sender, bool *isvalid, ValidResult *ir )
{
    /*<<<<<FRMEDITDEPARTAMENTO_VALIDATE*/
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
/*>>>>>FRMEDITDEPARTAMENTO_VALIDATE*/
    if ( !ir ) {
        showValidMessages( isvalid, *validresult, sender );
        delete validresult;
    }
}
/*<<<<<FRMEDITDEPARTAMENTO_FIN*/
} // namespace empresa
} // namespace gong
/*>>>>>FRMEDITDEPARTAMENTO_FIN*/

