/*<<<<<MODULE_INFO*/
// COPYLEFT Behavior para añadir campos a TIPODOC
// FIELD EsDeposito bool - eslayout
// FIELD EsLiquidacion bool - eslayout
// TYPE FrmEditRecBehavior deposit::TipoDoc
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITTIPODOCBEHAVIOR_INCLUDES*/
#include <dbappmainwindow.h>
#include "depositfrmedittipodocbehavior.h"
/*>>>>>FRMEDITTIPODOCBEHAVIOR_INCLUDES*/

namespace gong {
namespace deposit {

void FrmEditTipoDocBehavior::initGUI()
{
/*<<<<<FRMEDITTIPODOCBEHAVIOR_INITGUI*/
	QWidget *pControlsFrame = getControlsFrame();
	QVBoxLayout* pControlsLayout = getControlsLayout();
	QHBoxLayout *eslayout = new QHBoxLayout(0, 0, 6, "eslayout");
	checkEsDeposito = pTheForm->addCheckField( pControlsFrame, "TIPODOC", "ESDEPOSITO", eslayout );
	checkEsLiquidacion = pTheForm->addCheckField( pControlsFrame, "TIPODOC", "ESLIQUIDACION", eslayout );
	pControlsLayout->addLayout( eslayout );
/*>>>>>FRMEDITTIPODOCBEHAVIOR_INITGUI*/
}

void FrmEditTipoDocBehavior::scatterFields( bool is_pre )
{
	if( !is_pre) return;
/*<<<<<FRMEDITTIPODOCBEHAVIOR_SCATTER*/
	checkEsDeposito->setChecked( pTheForm->getRecord()->getValue("ESDEPOSITO").toBool());
	checkEsLiquidacion->setChecked( pTheForm->getRecord()->getValue("ESLIQUIDACION").toBool());
/*>>>>>FRMEDITTIPODOCBEHAVIOR_SCATTER*/
}

void FrmEditTipoDocBehavior::gatherFields()
{
/*<<<<<FRMEDITTIPODOCBEHAVIOR_GATHER*/
	pTheForm->getRecord()->setValue( "ESDEPOSITO", checkEsDeposito->isChecked());
	pTheForm->getRecord()->setValue( "ESLIQUIDACION", checkEsLiquidacion->isChecked());
/*>>>>>FRMEDITTIPODOCBEHAVIOR_GATHER*/
	// Las liquidaciones no actualizan el stock
	pTheForm->getRecord()->setValue( "ACTUALIZASTOCKS", !checkEsLiquidacion->isChecked());
}

void FrmEditTipoDocBehavior::validateFields(bool is_pre,
		QWidget *sender, bool *isvalid, ValidResult *ir)
{
/*<<<<<FRMEDITTIPODOCBEHAVIOR_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
/*>>>>>FRMEDITTIPODOCBEHAVIOR_VALIDATE*/
	if( !ir ) {
		showValidMessages(isvalid, *validresult, sender);
		delete validresult;
	}
}

/*<<<<<FRMEDITTIPODOCBEHAVIOR_FIN*/
} // namespace deposit
} // namespace gong
/*>>>>>FRMEDITTIPODOCBEHAVIOR_FIN*/

