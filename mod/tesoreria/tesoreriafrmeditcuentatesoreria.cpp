/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de cuentas de tesorería
// FIELD Codigo String - cuentaLayout
// FIELD Nombre String - cuentaLayout
// FIELD Debe Money - saldoLayout
// FIELD Haber Money - saldoLayout
// FIELD Saldo Money - saldoLayout
// FIELD Notas text - notasLayout
// TYPE FrmEditRecMaster tesoreria::CuentaTesoreria
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITCUENTATESORERIA_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "tesoreriafrmeditcuentatesoreria.h"
/*>>>>>FRMEDITCUENTATESORERIA_INCLUDES*/

namespace gong {
namespace tesoreria {

/*<<<<<FRMEDITCUENTATESORERIA_CONSTRUCTOR*/
FrmEditCuentaTesoreria::FrmEditCuentaTesoreria(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditCuentaTesoreria" );
/*>>>>>FRMEDITCUENTATESORERIA_CONSTRUCTOR*/
/*<<<<<FRMEDITCUENTATESORERIA_INIT_CONTROLS*/
	QHBoxLayout *cuentaLayout = new QHBoxLayout(0, 0, 6, "cuentaLayout");
	QHBoxLayout *saldoLayout = new QHBoxLayout(0, 0, 6, "saldoLayout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
	editCodigo = addEditField( pControlsFrame, "CUENTATESORERIA", "CODIGO", cuentaLayout );
	editNombre = addEditField( pControlsFrame, "CUENTATESORERIA", "NOMBRE", cuentaLayout );
	editDebe = addEditField( pControlsFrame, "CUENTATESORERIA", "DEBE", saldoLayout );
	editHaber = addEditField( pControlsFrame, "CUENTATESORERIA", "HABER", saldoLayout );
	editSaldo = addEditField( pControlsFrame, "CUENTATESORERIA", "SALDO", saldoLayout );
	editNotas = addTextField( pControlsFrame, "CUENTATESORERIA", "NOTAS", notasLayout );
	pControlsLayout->addLayout( cuentaLayout );
	pControlsLayout->addLayout( saldoLayout );
	pControlsLayout->addLayout( notasLayout );
/*>>>>>FRMEDITCUENTATESORERIA_INIT_CONTROLS*/
}

void FrmEditCuentaTesoreria::scatterFields()
{
/*<<<<<FRMEDITCUENTATESORERIA_SCATTER*/
	editCodigo->setText(getRecCuentaTesoreria()->getValue("CODIGO").toString());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editCodigo;
	editNombre->setText(getRecCuentaTesoreria()->getValue("NOMBRE").toString());
	editDebe->setText(getRecCuentaTesoreria()->getValue("DEBE").toMoney());
	editHaber->setText(getRecCuentaTesoreria()->getValue("HABER").toMoney());
	editSaldo->setText(getRecCuentaTesoreria()->getValue("SALDO").toMoney());
	editNotas->setText(getRecCuentaTesoreria()->getValue("NOTAS").toString());
/*>>>>>FRMEDITCUENTATESORERIA_SCATTER*/
}

void FrmEditCuentaTesoreria::gatherFields()
{
/*<<<<<FRMEDITCUENTATESORERIA_GATHER*/
	getRecCuentaTesoreria()->setValue( "CODIGO", editCodigo->toString());
	getRecCuentaTesoreria()->setValue( "NOMBRE", editNombre->toString());
	getRecCuentaTesoreria()->setValue( "DEBE", editDebe->toMoney());
	getRecCuentaTesoreria()->setValue( "HABER", editHaber->toMoney());
	getRecCuentaTesoreria()->setValue( "SALDO", editSaldo->toMoney());
	getRecCuentaTesoreria()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITCUENTATESORERIA_GATHER*/
}

void FrmEditCuentaTesoreria::validateFields(QWidget *sender, bool *isvalid, ValidResult *ir)
{
/*<<<<<FRMEDITCUENTATESORERIA_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	if( !sender && !pRecord->isValid( ValidResult::editing, validresult ) )
			*isvalid = false;
/*>>>>>FRMEDITCUENTATESORERIA_VALIDATE*/
	if( !ir ) {
		showValidMessages(isvalid, *validresult, sender);
		delete validresult;
	}
}

/*<<<<<FRMEDITCUENTATESORERIA_FIN*/
} // namespace tesoreria
} // namespace gong
/*>>>>>FRMEDITCUENTATESORERIA_FIN*/

