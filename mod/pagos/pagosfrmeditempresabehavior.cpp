/*<<<<<MODULE_INFO*/
// COPYLEFT Behavior para añadir el campo formato número de recibo
// FIELD FormatoNumRecibo string tabPagos
// TYPE FrmEditRecBehavior pagos::Empresa
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITEMPRESABEHAVIOR_INCLUDES*/
#include <dbappmainwindow.h>
#include "pagosfrmeditempresabehavior.h"
/*>>>>>FRMEDITEMPRESABEHAVIOR_INCLUDES*/

namespace gong {
namespace pagos {

void FrmEditEmpresaBehavior::initGUI()
{
/*<<<<<FRMEDITEMPRESABEHAVIOR_INITGUI*/
// 	QWidget *pControlsFrame = getControlsFrame();
// 	QVBoxLayout* pControlsLayout = getControlsLayout();
	QWidget *tabPagos = getOrCreateTab( "tabPagos" );
	QVBoxLayout *tabPagosLayout = static_cast<QVBoxLayout *>( tabPagos->layout() );
	QHBoxLayout *formatonumreciboLayout = new QHBoxLayout(0, 0, 6, "formatonumreciboLayout");
	editFormatoNumRecibo = pTheForm->addEditField( tabPagos, "EMPRESA", "FORMATONUMRECIBO", formatonumreciboLayout );
	tabPagosLayout->addLayout( formatonumreciboLayout );
/*>>>>>FRMEDITEMPRESABEHAVIOR_INITGUI*/
    pTheForm->setTabTitle( tabPagos, _("Pagos") );
}

void FrmEditEmpresaBehavior::scatterFields( bool is_pre )
{
	if( !is_pre) return;
/*<<<<<FRMEDITEMPRESABEHAVIOR_SCATTER*/
	editFormatoNumRecibo->setText( pTheForm->getRecord()->getValue("FORMATONUMRECIBO").toString());
/*>>>>>FRMEDITEMPRESABEHAVIOR_SCATTER*/
}

void FrmEditEmpresaBehavior::gatherFields()
{
/*<<<<<FRMEDITEMPRESABEHAVIOR_GATHER*/
	pTheForm->getRecord()->setValue( "FORMATONUMRECIBO", editFormatoNumRecibo->toString());
/*>>>>>FRMEDITEMPRESABEHAVIOR_GATHER*/
}

void FrmEditEmpresaBehavior::validateFields(bool is_pre,
		QWidget *sender, bool *isvalid, ValidResult *ir)
{
	if( !is_pre) return;
/*<<<<<FRMEDITEMPRESABEHAVIOR_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
/*>>>>>FRMEDITEMPRESABEHAVIOR_VALIDATE*/
	if( !ir ) {
		showValidMessages(isvalid, *validresult, sender);
		delete validresult;
	}
}

/*<<<<<FRMEDITEMPRESABEHAVIOR_FIN*/
} // namespace pagos
} // namespace gong
/*>>>>>FRMEDITEMPRESABEHAVIOR_FIN*/

