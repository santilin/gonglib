/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de
// FIELD NumeroSemana int
// FIELD FechaInicioPedido DateTime
// FIELD FechaFinPedido DateTime
// FIELD FechaRecogida Date
// FIELD Notas text
// TYPE FrmEditRecMaster ventasweb::SemanaPedido
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITSEMANAPEDIDO_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "ventaswebfrmeditsemanapedido.h"
/*>>>>>FRMEDITSEMANAPEDIDO_INCLUDES*/

namespace gong {
namespace ventasweb {

/*<<<<<FRMEDITSEMANAPEDIDO_CONSTRUCTOR*/
FrmEditSemanaPedido::FrmEditSemanaPedido(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditSemanaPedido" );
/*>>>>>FRMEDITSEMANAPEDIDO_CONSTRUCTOR*/
    /*<<<<<FRMEDITSEMANAPEDIDO_INIT_CONTROLS*/
	QHBoxLayout *numerosemanaLayout = new QHBoxLayout(0, 0, 6, "numerosemanaLayout");
	QHBoxLayout *fechainiciopedidoLayout = new QHBoxLayout(0, 0, 6, "fechainiciopedidoLayout");
	QHBoxLayout *fechafinpedidoLayout = new QHBoxLayout(0, 0, 6, "fechafinpedidoLayout");
	QHBoxLayout *fecharecogidaLayout = new QHBoxLayout(0, 0, 6, "fecharecogidaLayout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
	editNumeroSemana = addEditField( pControlsFrame, "SEMANAPEDIDO", "NUMEROSEMANA", numerosemanaLayout );
	editFechaInicioPedido = addEditField( pControlsFrame, "SEMANAPEDIDO", "FECHAINICIOPEDIDO", fechainiciopedidoLayout );
	editFechaFinPedido = addEditField( pControlsFrame, "SEMANAPEDIDO", "FECHAFINPEDIDO", fechafinpedidoLayout );
	editFechaRecogida = addEditField( pControlsFrame, "SEMANAPEDIDO", "FECHARECOGIDA", fecharecogidaLayout );
	editNotas = addTextField( pControlsFrame, "SEMANAPEDIDO", "NOTAS", notasLayout );
	pControlsLayout->addLayout( numerosemanaLayout );
	pControlsLayout->addLayout( fechainiciopedidoLayout );
	pControlsLayout->addLayout( fechafinpedidoLayout );
	pControlsLayout->addLayout( fecharecogidaLayout );
	pControlsLayout->addLayout( notasLayout );
/*>>>>>FRMEDITSEMANAPEDIDO_INIT_CONTROLS*/
}

void FrmEditSemanaPedido::scatterFields()
{
    /*<<<<<FRMEDITSEMANAPEDIDO_SCATTER*/
	editNumeroSemana->setText(getRecSemanaPedido()->getValue("NUMEROSEMANA").toInt());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editNumeroSemana;
	editFechaInicioPedido->setText(getRecSemanaPedido()->getValue("FECHAINICIOPEDIDO").toDateTime());
	editFechaFinPedido->setText(getRecSemanaPedido()->getValue("FECHAFINPEDIDO").toDateTime());
	editFechaRecogida->setText(getRecSemanaPedido()->getValue("FECHARECOGIDA").toDate());
	editNotas->setText(getRecSemanaPedido()->getValue("NOTAS").toString());
/*>>>>>FRMEDITSEMANAPEDIDO_SCATTER*/
}

void FrmEditSemanaPedido::gatherFields()
{
    /*<<<<<FRMEDITSEMANAPEDIDO_GATHER*/
	getRecSemanaPedido()->setValue( "NUMEROSEMANA", editNumeroSemana->toInt());
	getRecSemanaPedido()->setValue( "FECHAINICIOPEDIDO", editFechaInicioPedido->toDateTime());
	getRecSemanaPedido()->setValue( "FECHAFINPEDIDO", editFechaFinPedido->toDateTime());
	getRecSemanaPedido()->setValue( "FECHARECOGIDA", editFechaRecogida->toDate());
	getRecSemanaPedido()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITSEMANAPEDIDO_GATHER*/
}

void FrmEditSemanaPedido::validateFields(QWidget *sender, bool *isvalid, ValidResult *ir)
{
    /*<<<<<FRMEDITSEMANAPEDIDO_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	if( !sender && !pRecord->validate( ValidResult::editing) ) {
		validresult->append( pRecord->getErrors() );
		*isvalid = false;
	}
/*>>>>>FRMEDITSEMANAPEDIDO_VALIDATE*/
    if( !ir ) {
        showValidMessages(isvalid, *validresult, sender);
        delete validresult;
    }
}

/*<<<<<FRMEDITSEMANAPEDIDO_FIN*/
} // namespace ventasweb
} // namespace gong
/*>>>>>FRMEDITSEMANAPEDIDO_FIN*/

