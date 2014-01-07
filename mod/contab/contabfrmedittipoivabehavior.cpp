/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de tipos de IVA
// FIELD SubcuentaSoportado string
// FIELD SubcuentaRepercutido string
// TYPE FrmEditRecBehavior contab::TipoIVA
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITTIPOIVABEHAVIOR_INCLUDES*/
#include <dbappmainwindow.h>
#include "contabfrmedittipoivabehavior.h"
/*>>>>>FRMEDITTIPOIVABEHAVIOR_INCLUDES*/

namespace gong {
namespace contab {

void FrmEditTipoIVABehavior::initGUI()
{
    /*<<<<<FRMEDITTIPOIVABEHAVIOR_INITGUI*/
	QWidget *pControlsFrame = getControlsFrame();
	QVBoxLayout* pControlsLayout = getControlsLayout();
	QHBoxLayout *subcuentasoportadoLayout = new QHBoxLayout(0, 0, 6, "subcuentasoportadoLayout");
	QHBoxLayout *subcuentarepercutidoLayout = new QHBoxLayout(0, 0, 6, "subcuentarepercutidoLayout");
	editSubcuentaSoportado = pTheForm->addEditField( pControlsFrame, "TIPOIVA", "SUBCUENTASOPORTADO", subcuentasoportadoLayout );
	editSubcuentaRepercutido = pTheForm->addEditField( pControlsFrame, "TIPOIVA", "SUBCUENTAREPERCUTIDO", subcuentarepercutidoLayout );
	pControlsLayout->addLayout( subcuentasoportadoLayout );
	pControlsLayout->addLayout( subcuentarepercutidoLayout );
/*>>>>>FRMEDITTIPOIVABEHAVIOR_INITGUI*/
}

void FrmEditTipoIVABehavior::scatterFields( bool is_pre )
{
    if( !is_pre) return;
    /*<<<<<FRMEDITTIPOIVABEHAVIOR_SCATTER*/
	editSubcuentaSoportado->setText( pTheForm->getRecord()->getValue("SUBCUENTASOPORTADO").toString());
	editSubcuentaRepercutido->setText( pTheForm->getRecord()->getValue("SUBCUENTAREPERCUTIDO").toString());
/*>>>>>FRMEDITTIPOIVABEHAVIOR_SCATTER*/
}

void FrmEditTipoIVABehavior::gatherFields()
{
    /*<<<<<FRMEDITTIPOIVABEHAVIOR_GATHER*/
	pTheForm->getRecord()->setValue( "SUBCUENTASOPORTADO", editSubcuentaSoportado->toString());
	pTheForm->getRecord()->setValue( "SUBCUENTAREPERCUTIDO", editSubcuentaRepercutido->toString());
/*>>>>>FRMEDITTIPOIVABEHAVIOR_GATHER*/
}

void FrmEditTipoIVABehavior::validateFields(bool is_pre,
        QWidget *sender, bool *isvalid, ValidResult *ir)
{
    /*<<<<<FRMEDITTIPOIVABEHAVIOR_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
/*>>>>>FRMEDITTIPOIVABEHAVIOR_VALIDATE*/
    if( !ir ) {
        showValidMessages( isvalid, *validresult, sender);
        delete validresult;
    }
}

/*<<<<<FRMEDITTIPOIVABEHAVIOR_FIN*/
} // namespace contab
} // namespace gong
/*>>>>>FRMEDITTIPOIVABEHAVIOR_FIN*/

