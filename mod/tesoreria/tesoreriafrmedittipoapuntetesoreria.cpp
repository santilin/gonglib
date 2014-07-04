/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de tipos de asiento de tesosería
// FIELD Codigo int
// FIELD Nombre string
// FIELD Instrucciones text
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
	QHBoxLayout *instruccionesLayout = new QHBoxLayout(0, 0, 6, "instruccionesLayout");
	editCodigo = addEditField( pControlsFrame, "TIPOAPUNTETESORERIA", "CODIGO", codigoLayout );
	editNombre = addEditField( pControlsFrame, "TIPOAPUNTETESORERIA", "NOMBRE", nombreLayout );
	editInstrucciones = addTextField( pControlsFrame, "TIPOAPUNTETESORERIA", "INSTRUCCIONES", instruccionesLayout );
	pControlsLayout->addLayout( codigoLayout );
	pControlsLayout->addLayout( nombreLayout );
	pControlsLayout->addLayout( instruccionesLayout );
/*>>>>>FRMEDITTIPOAPUNTETESORERIA_INIT_CONTROLS*/
}

void FrmEditTipoApunteTesoreria::scatterFields()
{
/*<<<<<FRMEDITTIPOAPUNTETESORERIA_SCATTER*/
	editCodigo->setText(getRecTipoApunteTesoreria()->getValue("CODIGO").toInt());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editCodigo;
	editNombre->setText(getRecTipoApunteTesoreria()->getValue("NOMBRE").toString());
	editInstrucciones->setText(getRecTipoApunteTesoreria()->getValue("INSTRUCCIONES").toString());
	if( isInserting() && editCodigo->toInt() == 0 ) {
		editCodigo->setText( getRecord()->selectNextInt( "CODIGO" ) );
	}
/*>>>>>FRMEDITTIPOAPUNTETESORERIA_SCATTER*/
	if( isInserting() && editInstrucciones->toString().isEmpty() ) {
		editInstrucciones->setText(Xtring(
			"FECHA=\n"
			"IMPORTE=\n"
			"REFERENCIA=\n"
			"TABLATERCEROS=\n"
			"TERCERO=\n"
			"TABLACONCEPTOS=\n"
			"CONCEPTO=\n"));
	}
}

void FrmEditTipoApunteTesoreria::gatherFields()
{
/*<<<<<FRMEDITTIPOAPUNTETESORERIA_GATHER*/
	getRecTipoApunteTesoreria()->setValue( "CODIGO", editCodigo->toInt());
	getRecTipoApunteTesoreria()->setValue( "NOMBRE", editNombre->toString());
	getRecTipoApunteTesoreria()->setValue( "INSTRUCCIONES", editInstrucciones->toString());
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

