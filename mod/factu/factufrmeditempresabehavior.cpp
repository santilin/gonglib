/*<<<<<MODULE_INFO*/
// COPYLEFT Behavior para añadir el campo recargo de equivalencia
// FIELD RecargoEquivalencia bool tabFactu
// TYPE FrmEditRecBehavior factu::Empresa
/*>>>>>MODULE_INFO*/

#include <gonggettext.h>
/*<<<<<FRMEDITEMPRESABEHAVIOR_INCLUDES*/
#include <dbappmainwindow.h>
#include "factufrmeditempresabehavior.h"
/*>>>>>FRMEDITEMPRESABEHAVIOR_INCLUDES*/

namespace gong {
namespace factu {

void FrmEditEmpresaBehavior::initGUI()
{
    /*<<<<<FRMEDITEMPRESABEHAVIOR_INITGUI*/
	QWidget *pControlsFrame = getControlsFrame();
	QVBoxLayout* pControlsLayout = getControlsLayout();
	QWidget *tabFactu = getOrCreateTab( "tabFactu" );
	QVBoxLayout *tabFactuLayout = static_cast<QVBoxLayout *>( tabFactu->layout() );
	QHBoxLayout *recargoequivalenciaLayout = new QHBoxLayout(0, 0, 6, "recargoequivalenciaLayout");
	checkRecargoEquivalencia = pTheForm->addCheckField( tabFactu, "EMPRESA", "RECARGOEQUIVALENCIA", recargoequivalenciaLayout );
	tabFactuLayout->addLayout( recargoequivalenciaLayout );
/*>>>>>FRMEDITEMPRESABEHAVIOR_INITGUI*/
    (void)pControlsFrame;
    (void)pControlsLayout;
    pTheForm->setTabTitle( tabFactu, _("Facturación") );
}

void FrmEditEmpresaBehavior::scatterFields( bool is_pre )
{
    if( !is_pre) return;
    /*<<<<<FRMEDITEMPRESABEHAVIOR_SCATTER*/
	checkRecargoEquivalencia->setChecked( pTheForm->getRecord()->getValue("RECARGOEQUIVALENCIA").toBool());
/*>>>>>FRMEDITEMPRESABEHAVIOR_SCATTER*/
}

void FrmEditEmpresaBehavior::gatherFields()
{
    /*<<<<<FRMEDITEMPRESABEHAVIOR_GATHER*/
	pTheForm->getRecord()->setValue( "RECARGOEQUIVALENCIA", checkRecargoEquivalencia->isChecked());
/*>>>>>FRMEDITEMPRESABEHAVIOR_GATHER*/
}

void FrmEditEmpresaBehavior::validateFields(bool is_pre,
        QWidget *sender, bool *isvalid, ValidResult *ir)
{
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
} // namespace factu
} // namespace gong
/*>>>>>FRMEDITEMPRESABEHAVIOR_FIN*/


