/*<<<<<FRMEDITCONCEPTOTESORERIA_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "tesoreriafrmeditconceptotesoreria.h"
/*>>>>>FRMEDITCONCEPTOTESORERIA_INCLUDES*/

/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de conceptos de tesoreria
// FIELD Codigo int
// FIELD Nombre string
// FIELD Notas text
// TYPE FrmEditRecMaster tesoreria::ConceptoTesoreria IncCode ValidCodeAndDesc
/*>>>>>MODULE_INFO*/


namespace gong {
namespace tesoreria {

/*<<<<<FRMEDITCONCEPTOTESORERIA_CONSTRUCTOR*/
FrmEditConceptoTesoreria::FrmEditConceptoTesoreria(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditConceptoTesoreria" );
/*>>>>>FRMEDITCONCEPTOTESORERIA_CONSTRUCTOR*/
/*<<<<<FRMEDITCONCEPTOTESORERIA_INIT_CONTROLS*/
	QHBoxLayout *codigoLayout = new QHBoxLayout(0, 0, 6, "codigoLayout");
	QHBoxLayout *nombreLayout = new QHBoxLayout(0, 0, 6, "nombreLayout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
	editCodigo = addEditField( pControlsFrame, "CONCEPTOTESORERIA", "CODIGO", codigoLayout );
	editNombre = addEditField( pControlsFrame, "CONCEPTOTESORERIA", "NOMBRE", nombreLayout );
	editNotas = addTextField( pControlsFrame, "CONCEPTOTESORERIA", "NOTAS", notasLayout );
	pControlsLayout->addLayout( codigoLayout );
	pControlsLayout->addLayout( nombreLayout );
	pControlsLayout->addLayout( notasLayout );
/*>>>>>FRMEDITCONCEPTOTESORERIA_INIT_CONTROLS*/
}

void FrmEditConceptoTesoreria::scatterFields()
{
/*<<<<<FRMEDITCONCEPTOTESORERIA_SCATTER*/
	editCodigo->setText(getRecConceptoTesoreria()->getValue("CODIGO").toInt());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editCodigo;
	editNombre->setText(getRecConceptoTesoreria()->getValue("NOMBRE").toString());
	editNotas->setText(getRecConceptoTesoreria()->getValue("NOTAS").toString());
/*>>>>>FRMEDITCONCEPTOTESORERIA_SCATTER*/
}

void FrmEditConceptoTesoreria::gatherFields()
{
/*<<<<<FRMEDITCONCEPTOTESORERIA_GATHER*/
	getRecConceptoTesoreria()->setValue( "CODIGO", editCodigo->toInt());
	getRecConceptoTesoreria()->setValue( "NOMBRE", editNombre->toString());
	getRecConceptoTesoreria()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITCONCEPTOTESORERIA_GATHER*/
}

void FrmEditConceptoTesoreria::validateFields(QWidget *sender, bool *isvalid, ValidResult *ir)
{
/*<<<<<FRMEDITCONCEPTOTESORERIA_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	if( !sender && !pRecord->isValid( ValidResult::editing, validresult ) )
			*isvalid = false;
/*>>>>>FRMEDITCONCEPTOTESORERIA_VALIDATE*/
	if( !ir ) {
		showValidMessages(isvalid, *validresult, sender);
		delete validresult;
	}
}

/*<<<<<FRMEDITCONCEPTOTESORERIA_FIN*/
} // namespace tesoreria
} // namespace gong
/*>>>>>FRMEDITCONCEPTOTESORERIA_FIN*/

