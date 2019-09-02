/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de cuentas de tesorería
// FIELD Saldo Money tabExtracto saldo
// FIELD Codigo String - cuenta
// FIELD Nombre String - cuenta
// FIELD SaldoInicial Money - inicial
// FIELD FechaSaldoInicial Date - inicial
// FIELD Notas text - notas
// TYPE FrmEditRecMaster ofertas::CuentaOferta
/*>>>>>MODULE_INFO*/


/*<<<<<FRMEDITCUENTAOFERTA_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "ofertasfrmeditcuentaoferta.h"
/*>>>>>FRMEDITCUENTAOFERTA_INCLUDES*/

#include "ofertasmodule.h"
#include "ofertasfrmeditapunteoferta.h"
#include "ofertasrecapunteoferta.h"

namespace gong {
namespace ofertas {

/*<<<<<FRMEDITCUENTAOFERTA_CONSTRUCTOR*/
FrmEditCuentaOferta::FrmEditCuentaOferta(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditCuentaOferta" );
/*>>>>>FRMEDITCUENTAOFERTA_CONSTRUCTOR*/
/*<<<<<FRMEDITCUENTAOFERTA_INIT_CONTROLS*/
	showTabs(true);
	QWidget *tabExtracto = new QWidget( pTabWidget, "tabExtracto" );
	QVBoxLayout *tabExtractoLayout = new QVBoxLayout(tabExtracto, 11, 6, "tabExtractoLayout");
	QHBoxLayout *saldoLayout = new QHBoxLayout(0, 0, 6, "saldoLayout");
	QHBoxLayout *cuentaLayout = new QHBoxLayout(0, 0, 6, "cuentaLayout");
	QHBoxLayout *inicialLayout = new QHBoxLayout(0, 0, 6, "inicialLayout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
	editSaldo = addEditField( tabExtracto, "CUENTAOFERTA", "SALDO", saldoLayout );
	editCodigo = addEditField( pControlsFrame, "CUENTAOFERTA", "CODIGO", cuentaLayout );
	editNombre = addEditField( pControlsFrame, "CUENTAOFERTA", "NOMBRE", cuentaLayout );
	editSaldoInicial = addEditField( pControlsFrame, "CUENTAOFERTA", "SALDOINICIAL", inicialLayout );
	editFechaSaldoInicial = addEditField( pControlsFrame, "CUENTAOFERTA", "FECHASALDOINICIAL", inicialLayout );
	editNotas = addTextField( pControlsFrame, "CUENTAOFERTA", "NOTAS", notasLayout );
	tabExtractoLayout->addLayout( saldoLayout );
	pControlsLayout->addLayout( cuentaLayout );
	pControlsLayout->addLayout( inicialLayout );
	pControlsLayout->addLayout( notasLayout );
/*>>>>>FRMEDITCUENTAOFERTA_INIT_CONTROLS*/
    // No se genera automáticamente porque usa getViewsByName
    RecApunteOferta *apunte = static_cast<RecApunteOferta*>(ModuleInstance->createRecord("APUNTEOFERTA"));
    dbViewDefinitionDict apunteviews;
    ModuleInstance->getDatabase()->getViewsByName( "APUNTEOFERTA._EXTRACTO_", apunteviews );
    dbRecordDataModel *apuntedm = new dbRecordDataModel(apunte, apunteviews, "");
    pFrmApunteOferta = static_cast<FrmEditApunteOferta *>(
                              ModuleInstance->createEditForm(this, apunte, apuntedm, DataTable::browsing,
                                      static_cast<dbApplication::EditFlags>(dbApplication::embedded + dbApplication::tableEditEvent ),
                                      tabExtracto) );
    pFrmApunteOferta->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding);
    tabExtractoLayout->addWidget(pFrmApunteOferta);
    pTabWidget->addTab( tabExtracto, "&Extracto" );
}

void FrmEditCuentaOferta::scatterFields()
{
/*<<<<<FRMEDITCUENTAOFERTA_SCATTER*/
	editSaldo->setText(getRecCuentaOferta()->getValue("SALDO").toMoney());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editSaldo;
	editCodigo->setText(getRecCuentaOferta()->getValue("CODIGO").toString());
	editNombre->setText(getRecCuentaOferta()->getValue("NOMBRE").toString());
	editSaldoInicial->setText(getRecCuentaOferta()->getValue("SALDOINICIAL").toMoney());
	editFechaSaldoInicial->setText(getRecCuentaOferta()->getValue("FECHASALDOINICIAL").toDate());
	editNotas->setText(getRecCuentaOferta()->getValue("NOTAS").toString());
/*>>>>>FRMEDITCUENTAOFERTA_SCATTER*/
    if( !mSaveSaldoInicial.isValid() ) {
        mSaveSaldoInicial = editSaldoInicial->toVariant();
        mSaveSaldo = editSaldo->toVariant();
    }
    if( !isDeleting() ) {
        Xtring id_cond = getRecord()->getConnection()->toSQL(getRecCuentaOferta()->getRecordID());
        pFrmApunteOferta->setFormFilter( "APUNTEOFERTA.CUENTAOFERTA_ID=" + id_cond
                                            + " OR (APUNTEOFERTA.TABLATERCEROS='APUNTEOFERTA' AND APUNTEOFERTA.TERCERO_ID=" + id_cond + ")");
        // Si no, no se llama al showEvent que es el que hace el setdatamodel
        pTabWidget->setCurrentPage( 1 );
    }
    if( !pFrmApunteOferta->getDataTable()->numRows() ) {
        pTabWidget->setCurrentPage( 0 );
        pFocusWidget = editCodigo;
    } else {
        pTabWidget->setCurrentPage( 1 );
        pFocusWidget = pFrmApunteOferta;
    }
}

void FrmEditCuentaOferta::gatherFields()
{
/*<<<<<FRMEDITCUENTAOFERTA_GATHER*/
	getRecCuentaOferta()->setValue( "SALDO", editSaldo->toMoney());
	getRecCuentaOferta()->setValue( "CODIGO", editCodigo->toString());
	getRecCuentaOferta()->setValue( "NOMBRE", editNombre->toString());
	getRecCuentaOferta()->setValue( "SALDOINICIAL", editSaldoInicial->toMoney());
	getRecCuentaOferta()->setValue( "FECHASALDOINICIAL", editFechaSaldoInicial->toDate());
	getRecCuentaOferta()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITCUENTAOFERTA_GATHER*/
	getRecCuentaOferta()->setValue( "SALDO", editSaldo->toMoney());
	getRecCuentaOferta()->setValue( "CODIGO", editCodigo->toString());
	getRecCuentaOferta()->setValue( "NOMBRE", editNombre->toString());
	getRecCuentaOferta()->setValue( "SALDOINICIAL", editSaldoInicial->toMoney());
	getRecCuentaOferta()->setValue( "FECHASALDOINICIAL", editFechaSaldoInicial->toDate());
	getRecCuentaOferta()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITCUENTAOFERTA_GATHER*/
}

void FrmEditCuentaOferta::validateFields(QWidget *sender, bool *isvalid, ValidResult *ir)
{
/*<<<<<FRMEDITCUENTAOFERTA_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	if( !sender && !pRecord->validate( ValidResult::editing) ) {
		validresult->append( pRecord->getErrors() );
		*isvalid = false;
	}
/*>>>>>FRMEDITCUENTAOFERTA_VALIDATE*/
    if( !ir ) {
        showValidMessages(isvalid, *validresult, sender);
        delete validresult;
    }
    if (sender == editSaldoInicial && editSaldoInicial->isJustEdited()) {
        editSaldo->setText( mSaveSaldo - mSaveSaldoInicial + editSaldoInicial->toVariant() );
        editFechaSaldoInicial->setText( Date::currentDate() );
    }
}

/*<<<<<FRMEDITCUENTAOFERTA_FIN*/
} // namespace ofertas
} // namespace gong
/*>>>>>FRMEDITCUENTAOFERTA_FIN*/
