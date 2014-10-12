/*<<<<<MODULE_INFO*/
// COPYLEFT Behavior para añadir campos al formulario de empresas
// FIELD CuentaCaja string
// TYPE FrmEditRecBehavior tesoreria::Empresa 
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITEMPRESABEHAVIOR_INCLUDES*/
#include <dbappmainwindow.h>
#include "tesoreriafrmeditempresabehavior.h"
/*>>>>>FRMEDITEMPRESABEHAVIOR_INCLUDES*/

namespace gong {
namespace tesoreria {

void FrmEditEmpresaBehavior::initGUI()
{
/*<<<<<FRMEDITEMPRESABEHAVIOR_INITGUI*/
	QWidget *pControlsFrame = getControlsFrame();
	QVBoxLayout* pControlsLayout = getControlsLayout();
	QHBoxLayout *cuentacajaLayout = new QHBoxLayout(0, 0, 6, "cuentacajaLayout");
	editCuentaCaja = pTheForm->addEditField( pControlsFrame, "EMPRESA", "CUENTACAJA", cuentacajaLayout );
	pControlsLayout->addLayout( cuentacajaLayout );
/*>>>>>FRMEDITEMPRESABEHAVIOR_INITGUI*/
}

void FrmEditEmpresaBehavior::scatterFields( bool is_pre )
{
	if( !is_pre) return;
/*<<<<<FRMEDITEMPRESABEHAVIOR_SCATTER*/
	editCuentaCaja->setText( pTheForm->getRecord()->getValue("CUENTACAJA").toString());
/*>>>>>FRMEDITEMPRESABEHAVIOR_SCATTER*/
}

void FrmEditEmpresaBehavior::gatherFields()
{
/*<<<<<FRMEDITEMPRESABEHAVIOR_GATHER*/
	pTheForm->getRecord()->setValue( "CUENTACAJA", editCuentaCaja->toString());
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
} // namespace tesoreria
} // namespace gong
/*>>>>>FRMEDITEMPRESABEHAVIOR_FIN*/

