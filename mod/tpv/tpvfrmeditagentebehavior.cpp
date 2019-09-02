/*<<<<<MODULE_INFO*/
// COPYLEFT Behavior del formulario de agentes para el módulo TPV
// FIELD ParaTPV bool tabTPV tpvlayout
// TYPE FrmEditRecBehavior tpv::Agente
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITAGENTEBEHAVIOR_INCLUDES*/
#include <dbappmainwindow.h>
#include "tpvfrmeditagentebehavior.h"
/*>>>>>FRMEDITAGENTEBEHAVIOR_INCLUDES*/

namespace gong {
namespace tpv {

void FrmEditAgenteBehavior::initGUI()
{
/*<<<<<FRMEDITAGENTEBEHAVIOR_INITGUI*/
	QWidget *pControlsFrame = getControlsFrame();
	QVBoxLayout* pControlsLayout = getControlsLayout();
	QWidget *tabTPV = getOrCreateTab( "tabTPV" );
	QVBoxLayout *tabTPVLayout = static_cast<QVBoxLayout *>( tabTPV->layout() );
	QHBoxLayout *tpvlayout = new QHBoxLayout(0, 0, 6, "tpvlayout");
	checkParaTPV = pTheForm->addCheckField( tabTPV, "AGENTE", "PARATPV", tpvlayout );
	tabTPVLayout->addLayout( tpvlayout );
/*>>>>>FRMEDITAGENTEBEHAVIOR_INITGUI*/
	(void)pControlsFrame; (void)pControlsLayout;
}

void FrmEditAgenteBehavior::scatterFields( bool is_pre )
{
	if( !is_pre) return;
/*<<<<<FRMEDITAGENTEBEHAVIOR_SCATTER*/
	checkParaTPV->setChecked( pTheForm->getRecord()->getValue("PARATPV").toBool());
/*>>>>>FRMEDITAGENTEBEHAVIOR_SCATTER*/
}

void FrmEditAgenteBehavior::gatherFields()
{
/*<<<<<FRMEDITAGENTEBEHAVIOR_GATHER*/
	pTheForm->getRecord()->setValue( "PARATPV", checkParaTPV->isChecked());
/*>>>>>FRMEDITAGENTEBEHAVIOR_GATHER*/
}

void FrmEditAgenteBehavior::validateFields(bool is_pre,
		QWidget *sender, bool *isvalid, ValidResult *ir)
{
	if( !is_pre ) return;
/*<<<<<FRMEDITAGENTEBEHAVIOR_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
/*>>>>>FRMEDITAGENTEBEHAVIOR_VALIDATE*/
	(void)validresult;
}

/*<<<<<FRMEDITAGENTEBEHAVIOR_FIN*/
} // namespace tpv
} // namespace gong
/*>>>>>FRMEDITAGENTEBEHAVIOR_FIN*/

