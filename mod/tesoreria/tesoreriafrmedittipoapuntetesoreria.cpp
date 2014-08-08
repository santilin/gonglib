/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de tipos de asiento de tesosería
// FIELD Codigo int - codigo
// FIELD Nombre string - codigo
// FIELD PedirCargo comboint - leftcargo
// FIELD Cargo bool - leftcargo
// FIELD PedirFecha comboint - leftFecha
// FIELD Fecha date - leftFecha
// FIELD PedirImporte comboint - leftImporte
// FIELD Importe money - leftImporte
// FIELD PedirReferencia comboint - leftReferencia
// FIELD Referencia string - leftReferencia
// FIELD PedirCuenta comboint - leftcuenta
// FIELD Cuenta string - leftcuenta
// FIELD PedirTablaTerceros comboint - lefttablaterceros
// FIELD TablaTerceros combostring - lefttablaterceros
// FIELD PedirTercero comboint - lefttercero
// FIELD Tercero string - lefttercero
// FIELD PedirTablaConceptos comboint - lefttablaconceptos
// FIELD TablaConceptos combostring - lefttablaconceptos
// FIELD PedirConcepto comboint - leftconcepto
// FIELD Concepto string - leftconcepto
// FIELD PedirTablaDocumentos comboint - lefttabladocumentos
// FIELD TablaDocumentos string - lefttabladocumentos
// FIELD PedirProyecto comboint - leftproyecto if(empresa::ModuleInstance->usaProyectos())
// FIELD Proyecto string - leftproyecto if(empresa::ModuleInstance->usaProyectos())
// FIELD PedirNotas comboint - leftnotas
// FIELD ValorNotas string - leftnotas
// FIELD Notas text
// TYPE FrmEditRecMaster tesoreria::TipoApunteTesoreria validCodeAndDesc IncCode
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITTIPOAPUNTETESORERIA_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "tesoreriafrmedittipoapuntetesoreria.h"
/*>>>>>FRMEDITTIPOAPUNTETESORERIA_INCLUDES*/
#include "empresamodule.h"

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
	QHBoxLayout *leftcargoLayout = new QHBoxLayout(0, 0, 6, "leftcargoLayout");
	QHBoxLayout *leftFechaLayout = new QHBoxLayout(0, 0, 6, "leftFechaLayout");
	QHBoxLayout *leftImporteLayout = new QHBoxLayout(0, 0, 6, "leftImporteLayout");
	QHBoxLayout *leftReferenciaLayout = new QHBoxLayout(0, 0, 6, "leftReferenciaLayout");
	QHBoxLayout *leftcuentaLayout = new QHBoxLayout(0, 0, 6, "leftcuentaLayout");
	QHBoxLayout *lefttablatercerosLayout = new QHBoxLayout(0, 0, 6, "lefttablatercerosLayout");
	QHBoxLayout *leftterceroLayout = new QHBoxLayout(0, 0, 6, "leftterceroLayout");
	QHBoxLayout *lefttablaconceptosLayout = new QHBoxLayout(0, 0, 6, "lefttablaconceptosLayout");
	QHBoxLayout *leftconceptoLayout = new QHBoxLayout(0, 0, 6, "leftconceptoLayout");
	QHBoxLayout *lefttabladocumentosLayout = new QHBoxLayout(0, 0, 6, "lefttabladocumentosLayout");
	QHBoxLayout *leftproyectoLayout = new QHBoxLayout(0, 0, 6, "leftproyectoLayout");
	QHBoxLayout *leftnotasLayout = new QHBoxLayout(0, 0, 6, "leftnotasLayout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
	editCodigo = addEditField( pControlsFrame, "TIPOAPUNTETESORERIA", "CODIGO", codigoLayout );
	editNombre = addEditField( pControlsFrame, "TIPOAPUNTETESORERIA", "NOMBRE", codigoLayout );
	comboPedirCargo = addComboIntField( pControlsFrame, "TIPOAPUNTETESORERIA", "PEDIRCARGO", leftcargoLayout );
	checkCargo = addCheckField( pControlsFrame, "TIPOAPUNTETESORERIA", "CARGO", leftcargoLayout );
	comboPedirFecha = addComboIntField( pControlsFrame, "TIPOAPUNTETESORERIA", "PEDIRFECHA", leftFechaLayout );
	editFecha = addEditField( pControlsFrame, "TIPOAPUNTETESORERIA", "FECHA", leftFechaLayout );
	comboPedirImporte = addComboIntField( pControlsFrame, "TIPOAPUNTETESORERIA", "PEDIRIMPORTE", leftImporteLayout );
	editImporte = addEditField( pControlsFrame, "TIPOAPUNTETESORERIA", "IMPORTE", leftImporteLayout );
	comboPedirReferencia = addComboIntField( pControlsFrame, "TIPOAPUNTETESORERIA", "PEDIRREFERENCIA", leftReferenciaLayout );
	editReferencia = addEditField( pControlsFrame, "TIPOAPUNTETESORERIA", "REFERENCIA", leftReferenciaLayout );
	comboPedirCuenta = addComboIntField( pControlsFrame, "TIPOAPUNTETESORERIA", "PEDIRCUENTA", leftcuentaLayout );
	editCuenta = addEditField( pControlsFrame, "TIPOAPUNTETESORERIA", "CUENTA", leftcuentaLayout );
	comboPedirTablaTerceros = addComboIntField( pControlsFrame, "TIPOAPUNTETESORERIA", "PEDIRTABLATERCEROS", lefttablatercerosLayout );
	comboTablaTerceros = addComboXtringField( pControlsFrame, "TIPOAPUNTETESORERIA", "TABLATERCEROS", lefttablatercerosLayout );
	comboPedirTercero = addComboIntField( pControlsFrame, "TIPOAPUNTETESORERIA", "PEDIRTERCERO", leftterceroLayout );
	editTercero = addEditField( pControlsFrame, "TIPOAPUNTETESORERIA", "TERCERO", leftterceroLayout );
	comboPedirTablaConceptos = addComboIntField( pControlsFrame, "TIPOAPUNTETESORERIA", "PEDIRTABLACONCEPTOS", lefttablaconceptosLayout );
	comboTablaConceptos = addComboXtringField( pControlsFrame, "TIPOAPUNTETESORERIA", "TABLACONCEPTOS", lefttablaconceptosLayout );
	comboPedirConcepto = addComboIntField( pControlsFrame, "TIPOAPUNTETESORERIA", "PEDIRCONCEPTO", leftconceptoLayout );
	editConcepto = addEditField( pControlsFrame, "TIPOAPUNTETESORERIA", "CONCEPTO", leftconceptoLayout );
	comboPedirTablaDocumentos = addComboIntField( pControlsFrame, "TIPOAPUNTETESORERIA", "PEDIRTABLADOCUMENTOS", lefttabladocumentosLayout );
	editTablaDocumentos = addEditField( pControlsFrame, "TIPOAPUNTETESORERIA", "TABLADOCUMENTOS", lefttabladocumentosLayout );
if(empresa::ModuleInstance->usaProyectos()){
	comboPedirProyecto = addComboIntField( pControlsFrame, "TIPOAPUNTETESORERIA", "PEDIRPROYECTO", leftproyectoLayout );
}
if(empresa::ModuleInstance->usaProyectos()){
	editProyecto = addEditField( pControlsFrame, "TIPOAPUNTETESORERIA", "PROYECTO", leftproyectoLayout );
}
	comboPedirNotas = addComboIntField( pControlsFrame, "TIPOAPUNTETESORERIA", "PEDIRNOTAS", leftnotasLayout );
	editValorNotas = addEditField( pControlsFrame, "TIPOAPUNTETESORERIA", "VALORNOTAS", leftnotasLayout );
	editNotas = addTextField( pControlsFrame, "TIPOAPUNTETESORERIA", "NOTAS", notasLayout );
	pControlsLayout->addLayout( codigoLayout );
	pControlsLayout->addLayout( leftcargoLayout );
	alignLayout( leftcargoLayout, true );
	pControlsLayout->addLayout( leftFechaLayout );
	alignLayout( leftFechaLayout, true );
	pControlsLayout->addLayout( leftImporteLayout );
	alignLayout( leftImporteLayout, true );
	pControlsLayout->addLayout( leftReferenciaLayout );
	alignLayout( leftReferenciaLayout, true );
	pControlsLayout->addLayout( leftcuentaLayout );
	alignLayout( leftcuentaLayout, true );
	pControlsLayout->addLayout( lefttablatercerosLayout );
	alignLayout( lefttablatercerosLayout, true );
	pControlsLayout->addLayout( leftterceroLayout );
	alignLayout( leftterceroLayout, true );
	pControlsLayout->addLayout( lefttablaconceptosLayout );
	alignLayout( lefttablaconceptosLayout, true );
	pControlsLayout->addLayout( leftconceptoLayout );
	alignLayout( leftconceptoLayout, true );
	pControlsLayout->addLayout( lefttabladocumentosLayout );
	alignLayout( lefttabladocumentosLayout, true );
	pControlsLayout->addLayout( leftproyectoLayout );
	alignLayout( leftproyectoLayout, true );
	pControlsLayout->addLayout( leftnotasLayout );
	alignLayout( leftnotasLayout, true );
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
	comboPedirCargo->setCurrentItemByValue(getRecTipoApunteTesoreria()->getValue("PEDIRCARGO").toInt());
	checkCargo->setChecked(getRecTipoApunteTesoreria()->getValue("CARGO").toBool());
	comboPedirFecha->setCurrentItemByValue(getRecTipoApunteTesoreria()->getValue("PEDIRFECHA").toInt());
	editFecha->setText(getRecTipoApunteTesoreria()->getValue("FECHA").toDate());
	comboPedirImporte->setCurrentItemByValue(getRecTipoApunteTesoreria()->getValue("PEDIRIMPORTE").toInt());
	editImporte->setText(getRecTipoApunteTesoreria()->getValue("IMPORTE").toMoney());
	comboPedirReferencia->setCurrentItemByValue(getRecTipoApunteTesoreria()->getValue("PEDIRREFERENCIA").toInt());
	editReferencia->setText(getRecTipoApunteTesoreria()->getValue("REFERENCIA").toString());
	comboPedirCuenta->setCurrentItemByValue(getRecTipoApunteTesoreria()->getValue("PEDIRCUENTA").toInt());
	editCuenta->setText(getRecTipoApunteTesoreria()->getValue("CUENTA").toString());
	comboPedirTablaTerceros->setCurrentItemByValue(getRecTipoApunteTesoreria()->getValue("PEDIRTABLATERCEROS").toInt());
	comboTablaTerceros->setText(getRecTipoApunteTesoreria()->getValue("TABLATERCEROS").toString());
	comboPedirTercero->setCurrentItemByValue(getRecTipoApunteTesoreria()->getValue("PEDIRTERCERO").toInt());
	editTercero->setText(getRecTipoApunteTesoreria()->getValue("TERCERO").toString());
	comboPedirTablaConceptos->setCurrentItemByValue(getRecTipoApunteTesoreria()->getValue("PEDIRTABLACONCEPTOS").toInt());
	comboTablaConceptos->setText(getRecTipoApunteTesoreria()->getValue("TABLACONCEPTOS").toString());
	comboPedirConcepto->setCurrentItemByValue(getRecTipoApunteTesoreria()->getValue("PEDIRCONCEPTO").toInt());
	editConcepto->setText(getRecTipoApunteTesoreria()->getValue("CONCEPTO").toString());
	comboPedirTablaDocumentos->setCurrentItemByValue(getRecTipoApunteTesoreria()->getValue("PEDIRTABLADOCUMENTOS").toInt());
	editTablaDocumentos->setText(getRecTipoApunteTesoreria()->getValue("TABLADOCUMENTOS").toString());
if(empresa::ModuleInstance->usaProyectos()){
	comboPedirProyecto->setCurrentItemByValue(getRecTipoApunteTesoreria()->getValue("PEDIRPROYECTO").toInt());
}
if(empresa::ModuleInstance->usaProyectos()){
	editProyecto->setText(getRecTipoApunteTesoreria()->getValue("PROYECTO").toString());
}
	comboPedirNotas->setCurrentItemByValue(getRecTipoApunteTesoreria()->getValue("PEDIRNOTAS").toInt());
	editValorNotas->setText(getRecTipoApunteTesoreria()->getValue("VALORNOTAS").toString());
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
	getRecTipoApunteTesoreria()->setValue( "PEDIRCARGO", comboPedirCargo->getCurrentItemValue());
	getRecTipoApunteTesoreria()->setValue( "CARGO", checkCargo->isChecked());
	getRecTipoApunteTesoreria()->setValue( "PEDIRFECHA", comboPedirFecha->getCurrentItemValue());
	getRecTipoApunteTesoreria()->setValue( "FECHA", editFecha->toDate());
	getRecTipoApunteTesoreria()->setValue( "PEDIRIMPORTE", comboPedirImporte->getCurrentItemValue());
	getRecTipoApunteTesoreria()->setValue( "IMPORTE", editImporte->toMoney());
	getRecTipoApunteTesoreria()->setValue( "PEDIRREFERENCIA", comboPedirReferencia->getCurrentItemValue());
	getRecTipoApunteTesoreria()->setValue( "REFERENCIA", editReferencia->toString());
	getRecTipoApunteTesoreria()->setValue( "PEDIRCUENTA", comboPedirCuenta->getCurrentItemValue());
	getRecTipoApunteTesoreria()->setValue( "CUENTA", editCuenta->toString());
	getRecTipoApunteTesoreria()->setValue( "PEDIRTABLATERCEROS", comboPedirTablaTerceros->getCurrentItemValue());
	getRecTipoApunteTesoreria()->setValue( "TABLATERCEROS", comboTablaTerceros->toString());
	getRecTipoApunteTesoreria()->setValue( "PEDIRTERCERO", comboPedirTercero->getCurrentItemValue());
	getRecTipoApunteTesoreria()->setValue( "TERCERO", editTercero->toString());
	getRecTipoApunteTesoreria()->setValue( "PEDIRTABLACONCEPTOS", comboPedirTablaConceptos->getCurrentItemValue());
	getRecTipoApunteTesoreria()->setValue( "TABLACONCEPTOS", comboTablaConceptos->toString());
	getRecTipoApunteTesoreria()->setValue( "PEDIRCONCEPTO", comboPedirConcepto->getCurrentItemValue());
	getRecTipoApunteTesoreria()->setValue( "CONCEPTO", editConcepto->toString());
	getRecTipoApunteTesoreria()->setValue( "PEDIRTABLADOCUMENTOS", comboPedirTablaDocumentos->getCurrentItemValue());
	getRecTipoApunteTesoreria()->setValue( "TABLADOCUMENTOS", editTablaDocumentos->toString());
if(empresa::ModuleInstance->usaProyectos()){
	getRecTipoApunteTesoreria()->setValue( "PEDIRPROYECTO", comboPedirProyecto->getCurrentItemValue());
}
if(empresa::ModuleInstance->usaProyectos()){
	getRecTipoApunteTesoreria()->setValue( "PROYECTO", editProyecto->toString());
}
	getRecTipoApunteTesoreria()->setValue( "PEDIRNOTAS", comboPedirNotas->getCurrentItemValue());
	getRecTipoApunteTesoreria()->setValue( "VALORNOTAS", editValorNotas->toString());
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

