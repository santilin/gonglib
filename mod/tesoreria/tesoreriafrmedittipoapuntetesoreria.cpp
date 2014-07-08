/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de tipos de asiento de tesosería
// FIELD Codigo int
// FIELD Nombre string
// FIELD PedirFecha comboint - leftFecha
// FIELD Fecha date - leftFecha
// FIELD PedirImporte comboint - leftImporte
// FIELD Importe money - leftImporte
// FIELD PedirReferencia comboint - leftReferencia
// FIELD Referencia string - leftReferencia
// FIELD Notas text
// TYPE FrmEditRecMaster tesoreria::TipoApunteTesoreria validCodeAndDesc IncCode
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITTIPOAPUNTETESORERIA_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "tesoreriafrmedittipoapuntetesoreria.h"
/*>>>>>FRMEDITTIPOAPUNTETESORERIA_INCLUDES*/

namespace gong {
namespace tesoreria {

/*<<<<<FRMEDITTIPOAPUNTETESORERIA_CONSTRUCTOR*/
FrmEditTipoApunteTesoreria::FrmEditTipoApunteTesoreria(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditTipoApunteTesoreria" );
/*>>>>>FRMEDITTIPOAPUNTETESORERIA_CONSTRUCTOR*/
/*<<<<<FRMEDITTIPOAPUNTETESORERIA_INIT_CONTROLS*/
	QHBoxLayout *codigoLayout = new QHBoxLayout(0, 0, 6, "codigoLayout");
	QHBoxLayout *nombreLayout = new QHBoxLayout(0, 0, 6, "nombreLayout");
	QHBoxLayout *fechaLayout = new QHBoxLayout(0, 0, 6, "fechaLayout");
	QHBoxLayout *leftImporteLayout = new QHBoxLayout(0, 0, 6, "leftImporteLayout");
	QHBoxLayout *referenciaLayout = new QHBoxLayout(0, 0, 6, "referenciaLayout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
	editCodigo = addEditField( pControlsFrame, "TIPOAPUNTETESORERIA", "CODIGO", codigoLayout );
	editNombre = addEditField( pControlsFrame, "TIPOAPUNTETESORERIA", "NOMBRE", nombreLayout );
	comboPedirFecha = addComboIntField( pControlsFrame, "TIPOAPUNTETESORERIA", "PEDIRFECHA", fechaLayout );
	editFecha = addEditField( pControlsFrame, "TIPOAPUNTETESORERIA", "FECHA", fechaLayout );
	comboPedirImporte = addComboIntField( pControlsFrame, "TIPOAPUNTETESORERIA", "PEDIRIMPORTE", leftImporteLayout );
	editImporte = addEditField( pControlsFrame, "TIPOAPUNTETESORERIA", "IMPORTE", leftImporteLayout );
	comboPedirReferencia = addComboIntField( pControlsFrame, "TIPOAPUNTETESORERIA", "PEDIRREFERENCIA", referenciaLayout );
	editReferencia = addEditField( pControlsFrame, "TIPOAPUNTETESORERIA", "REFERENCIA", referenciaLayout );
	editNotas = addTextField( pControlsFrame, "TIPOAPUNTETESORERIA", "NOTAS", notasLayout );
	pControlsLayout->addLayout( codigoLayout );
	pControlsLayout->addLayout( nombreLayout );
	pControlsLayout->addLayout( fechaLayout );
	pControlsLayout->addLayout( leftImporteLayout );
	alignLayout( leftImporteLayout, true );
	pControlsLayout->addLayout( referenciaLayout );
	pControlsLayout->addLayout( notasLayout );
/*>>>>>FRMEDITTIPOAPUNTETESORERIA_INIT_CONTROLS*/
}

void FrmEditTipoApunteTesoreria::scatterFields()
{
/*<<<<<FRMEDITTIPOAPUNTETESORERIA_SCATTER*/
	editCodigo->setText(getRecTipoApunteTesoreria()->getValue("CODIGO").toInt());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editCodigo;
	editNombre->setText(getRecTipoApunteTesoreria()->getValue("NOMBRE").toString());
	comboPedirFecha->setCurrentItemByValue(getRecTipoApunteTesoreria()->getValue("PEDIRFECHA").toInt());
	editFecha->setText(getRecTipoApunteTesoreria()->getValue("FECHA").toDate());
	comboPedirImporte->setCurrentItemByValue(getRecTipoApunteTesoreria()->getValue("PEDIRIMPORTE").toInt());
	editImporte->setText(getRecTipoApunteTesoreria()->getValue("IMPORTE").toMoney());
	comboPedirReferencia->setCurrentItemByValue(getRecTipoApunteTesoreria()->getValue("PEDIRREFERENCIA").toInt());
	editReferencia->setText(getRecTipoApunteTesoreria()->getValue("REFERENCIA").toString());
	editNotas->setText(getRecTipoApunteTesoreria()->getValue("NOTAS").toString());
	if( isInserting() && editCodigo->toInt() == 0 ) {
		editCodigo->setText( getRecord()->selectNextInt( "CODIGO" ) );
	}
/*>>>>>FRMEDITTIPOAPUNTETESORERIA_SCATTER*/
}

void FrmEditTipoApunteTesoreria::gatherFields()
{
/*<<<<<FRMEDITTIPOAPUNTETESORERIA_GATHER*/
	getRecTipoApunteTesoreria()->setValue( "CODIGO", editCodigo->toInt());
	getRecTipoApunteTesoreria()->setValue( "NOMBRE", editNombre->toString());
	getRecTipoApunteTesoreria()->setValue( "PEDIRFECHA", comboPedirFecha->getCurrentItemValue());
	getRecTipoApunteTesoreria()->setValue( "FECHA", editFecha->toDate());
	getRecTipoApunteTesoreria()->setValue( "PEDIRIMPORTE", comboPedirImporte->getCurrentItemValue());
	getRecTipoApunteTesoreria()->setValue( "IMPORTE", editImporte->toMoney());
	getRecTipoApunteTesoreria()->setValue( "PEDIRREFERENCIA", comboPedirReferencia->getCurrentItemValue());
	getRecTipoApunteTesoreria()->setValue( "REFERENCIA", editReferencia->toString());
	getRecTipoApunteTesoreria()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITTIPOAPUNTETESORERIA_GATHER*/
}

void FrmEditTipoApunteTesoreria::validateFields(QWidget *sender, bool *isvalid, ValidResult *ir)
{
/*<<<<<FRMEDITTIPOAPUNTETESORERIA_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	if( !sender && !pRecord->isValid( ValidResult::editing, validresult ) )
			*isvalid = false;
	if( !validCodeAndDesc( sender, *validresult, editCodigo, editNombre, "codigo", "nombre" ) )
		if( !sender )
			*isvalid = false;
/*>>>>>FRMEDITTIPOAPUNTETESORERIA_VALIDATE*/
	if( !ir ) {
		showValidMessages(isvalid, *validresult, sender);
		delete validresult;
	}
}

/*<<<<<FRMEDITTIPOAPUNTETESORERIA_FIN*/
} // namespace tesoreria
} // namespace gong
/*>>>>>FRMEDITTIPOAPUNTETESORERIA_FIN*/

