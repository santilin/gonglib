/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de cuentas de tesorería
// FIELD Saldo Money tabExtracto saldo
// FIELD Codigo String - cuenta
// FIELD Nombre String - cuenta
// FIELD SaldoInicial Money - inicial
// FIELD FechaSaldoInicial Date - inicial
// FIELD Notas text - notas
// TYPE FrmEditRecMaster tesoreria::CuentaTesoreria
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITCUENTATESORERIA_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "tesoreriafrmeditcuentatesoreria.h"
/*>>>>>FRMEDITCUENTATESORERIA_INCLUDES*/
#include "tesoreriamodule.h"
#include "tesoreriafrmeditapuntetesoreria.h"
#include "tesoreriarecapuntetesoreria.h"

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
	showTabs(true);
	QWidget *tabExtracto = new QWidget( pTabWidget, "tabExtracto" );
	QVBoxLayout *tabExtractoLayout = new QVBoxLayout(tabExtracto, 11, 6, "tabExtractoLayout");
	QHBoxLayout *saldoLayout = new QHBoxLayout(0, 0, 6, "saldoLayout");
	QHBoxLayout *cuentaLayout = new QHBoxLayout(0, 0, 6, "cuentaLayout");
	QHBoxLayout *inicialLayout = new QHBoxLayout(0, 0, 6, "inicialLayout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
	editSaldo = addEditField( tabExtracto, "CUENTATESORERIA", "SALDO", saldoLayout );
	editCodigo = addEditField( pControlsFrame, "CUENTATESORERIA", "CODIGO", cuentaLayout );
	editNombre = addEditField( pControlsFrame, "CUENTATESORERIA", "NOMBRE", cuentaLayout );
	editSaldoInicial = addEditField( pControlsFrame, "CUENTATESORERIA", "SALDOINICIAL", inicialLayout );
	editFechaSaldoInicial = addEditField( pControlsFrame, "CUENTATESORERIA", "FECHASALDOINICIAL", inicialLayout );
	editNotas = addTextField( pControlsFrame, "CUENTATESORERIA", "NOTAS", notasLayout );
	tabExtractoLayout->addLayout( saldoLayout );
	pControlsLayout->addLayout( cuentaLayout );
	pControlsLayout->addLayout( inicialLayout );
	pControlsLayout->addLayout( notasLayout );
/*>>>>>FRMEDITCUENTATESORERIA_INIT_CONTROLS*/
    // No se genera automáticamente porque usa getViewsByName
    RecApunteTesoreria *apunte = static_cast<RecApunteTesoreria*>(ModuleInstance->createRecord("APUNTETESORERIA"));
    dbViewDefinitionDict apunteviews;
    ModuleInstance->getDatabase()->getViewsByName( "APUNTETESORERIA._EXTRACTO_", apunteviews );
    dbRecordDataModel *apuntedm = new dbRecordDataModel(apunte, apunteviews, "");
    pFrmApunteTesoreria = static_cast<FrmEditApunteTesoreria *>(
                              ModuleInstance->createEditForm(this, apunte, apuntedm, DataTable::browsing,
                                      static_cast<dbApplication::EditFlags>(dbApplication::embedded + dbApplication::tableEditEvent ),
                                      tabExtracto) );
    pFrmApunteTesoreria->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding);
    tabExtractoLayout->addWidget(pFrmApunteTesoreria);
    pTabWidget->addTab( tabExtracto, "&Extracto" );
}

void FrmEditCuentaTesoreria::scatterFields()
{
    /*<<<<<FRMEDITCUENTATESORERIA_SCATTER*/
	editSaldo->setText(getRecCuentaTesoreria()->getValue("SALDO").toMoney());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editSaldo;
	editCodigo->setText(getRecCuentaTesoreria()->getValue("CODIGO").toString());
	editNombre->setText(getRecCuentaTesoreria()->getValue("NOMBRE").toString());
	editSaldoInicial->setText(getRecCuentaTesoreria()->getValue("SALDOINICIAL").toMoney());
	editFechaSaldoInicial->setText(getRecCuentaTesoreria()->getValue("FECHASALDOINICIAL").toDate());
	editNotas->setText(getRecCuentaTesoreria()->getValue("NOTAS").toString());
/*>>>>>FRMEDITCUENTATESORERIA_SCATTER*/
    if( isInserting() && !isDuplicating() && !DBAPP->codeNotFound().isEmpty() ) {
        if( DBAPP->codeNotFound().toInt() != 0 ) {
            editCodigo->setText( DBAPP->codeNotFound() );
            editCodigo->setJustEdited( true );
        } else {
            editNombre->setText( DBAPP->codeNotFound() );
            editNombre->setJustEdited( true );
        }
    }
    if( !mSaveSaldoInicial.isValid() ) {
        mSaveSaldoInicial = editSaldoInicial->toVariant();
        mSaveSaldo = editSaldo->toVariant();
    }
    if( !isDeleting() ) {
        Xtring id_cond = getRecord()->getConnection()->toSQL(getRecCuentaTesoreria()->getRecordID());
        pFrmApunteTesoreria->setFormFilter( "APUNTETESORERIA.CUENTATESORERIA_ID=" + id_cond
                                            + " OR (APUNTETESORERIA.TABLATERCEROS='APUNTETESORERIA' AND APUNTETESORERIA.TERCERO_ID=" + id_cond + ")");
        // Si no, no se llama al showEvent que es el que hace el setdatamodel
        pTabWidget->setCurrentPage( 1 );
    }
    if( !pFrmApunteTesoreria->getDataTable()->numRows() ) {
        pTabWidget->setCurrentPage( 0 );
        pFocusWidget = editCodigo;
    } else {
        pTabWidget->setCurrentPage( 1 );
        pFocusWidget = pFrmApunteTesoreria;
    }
}

void FrmEditCuentaTesoreria::gatherFields()
{
    /*<<<<<FRMEDITCUENTATESORERIA_GATHER*/
	getRecCuentaTesoreria()->setValue( "SALDO", editSaldo->toMoney());
	getRecCuentaTesoreria()->setValue( "CODIGO", editCodigo->toString());
	getRecCuentaTesoreria()->setValue( "NOMBRE", editNombre->toString());
	getRecCuentaTesoreria()->setValue( "SALDOINICIAL", editSaldoInicial->toMoney());
	getRecCuentaTesoreria()->setValue( "FECHASALDOINICIAL", editFechaSaldoInicial->toDate());
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
    if (sender == editSaldoInicial && editSaldoInicial->isJustEdited()) {
        editSaldo->setText( mSaveSaldo - mSaveSaldoInicial + editSaldoInicial->toVariant() );
        editFechaSaldoInicial->setText( Date::currentDate() );
    }
}

/*<<<<<FRMEDITCUENTATESORERIA_FIN*/
} // namespace tesoreria
} // namespace gong
/*>>>>>FRMEDITCUENTATESORERIA_FIN*/

