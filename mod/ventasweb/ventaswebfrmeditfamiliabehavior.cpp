/*<<<<<MODULE_INFO*/
// COPYLEFT Behavior del formulario de familias
// FIELD PublicarWeb bool tabExtra pubweb
// FIELD DescripcionWeb text tabExtra descweb
// TYPE FrmEditRecBehavior ventasweb::Familia
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITFAMILIABEHAVIOR_INCLUDES*/
#include <dbappmainwindow.h>
#include "ventaswebfrmeditfamiliabehavior.h"
/*>>>>>FRMEDITFAMILIABEHAVIOR_INCLUDES*/

namespace gong {
namespace ventasweb {

void FrmEditFamiliaBehavior::initGUI()
{
    /*<<<<<FRMEDITFAMILIABEHAVIOR_INITGUI*/
	QWidget *pControlsFrame = getControlsFrame();
	QVBoxLayout* pControlsLayout = getControlsLayout();
	QWidget *tabExtra = getOrCreateTab( "tabExtra" );
	QVBoxLayout *tabExtraLayout = static_cast<QVBoxLayout *>( tabExtra->layout() );
	QHBoxLayout *pubweb = new QHBoxLayout(0, 0, 6, "pubweb");
	QHBoxLayout *descweb = new QHBoxLayout(0, 0, 6, "descweb");
	checkPublicarWeb = pTheForm->addCheckField( tabExtra, "FAMILIA", "PUBLICARWEB", pubweb );
	editDescripcionWeb = pTheForm->addTextField( tabExtra, "FAMILIA", "DESCRIPCIONWEB", descweb );
	tabExtraLayout->addLayout( pubweb );
	tabExtraLayout->addLayout( descweb );
/*>>>>>FRMEDITFAMILIABEHAVIOR_INITGUI*/
    (void)pControlsFrame;
    (void)pControlsLayout;
    pTheForm->setTabTitle( tabExtra, _("Web") );
}

void FrmEditFamiliaBehavior::scatterFields( bool is_pre )
{
    if( !is_pre ) return;
    /*<<<<<FRMEDITFAMILIABEHAVIOR_SCATTER*/
	checkPublicarWeb->setChecked( pTheForm->getRecord()->getValue("PUBLICARWEB").toBool());
	editDescripcionWeb->setText( pTheForm->getRecord()->getValue("DESCRIPCIONWEB").toString());
/*>>>>>FRMEDITFAMILIABEHAVIOR_SCATTER*/
}

void FrmEditFamiliaBehavior::gatherFields()
{
    /*<<<<<FRMEDITFAMILIABEHAVIOR_GATHER*/
	pTheForm->getRecord()->setValue( "PUBLICARWEB", checkPublicarWeb->isChecked());
	pTheForm->getRecord()->setValue( "DESCRIPCIONWEB", editDescripcionWeb->toString());
/*>>>>>FRMEDITFAMILIABEHAVIOR_GATHER*/
}

void FrmEditFamiliaBehavior::validateFields(bool is_pre,
        QWidget *sender, bool *isvalid, ValidResult *ir)
{
    if( !is_pre ) return;
    /*<<<<<FRMEDITFAMILIABEHAVIOR_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
/*>>>>>FRMEDITFAMILIABEHAVIOR_VALIDATE*/
    (void)validresult;
}

/*<<<<<FRMEDITFAMILIABEHAVIOR_FIN*/
} // namespace ventasweb
} // namespace gong
/*>>>>>FRMEDITFAMILIABEHAVIOR_FIN*/
