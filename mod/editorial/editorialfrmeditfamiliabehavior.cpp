/*<<<<<MODULE_INFO*/
// COPYLEFT Behavior del formulario de familias para el módulo TPV
// FIELD libros bool - libro
// TYPE FrmEditRecBehavior editorial::Familia
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITFAMILIABEHAVIOR_INCLUDES*/
#include <dbappmainwindow.h>
#include "editorialfrmeditfamiliabehavior.h"
/*>>>>>FRMEDITFAMILIABEHAVIOR_INCLUDES*/

namespace gong {
namespace editorial {

void FrmEditFamiliaBehavior::initGUI()
{
/*<<<<<FRMEDITFAMILIABEHAVIOR_INITGUI*/
	QWidget *pControlsFrame = getControlsFrame();
	QVBoxLayout* pControlsLayout = getControlsLayout();
	QHBoxLayout *libro = new QHBoxLayout(0, 0, 6, "libro");
	checklibros = pTheForm->addCheckField( pControlsFrame, "FAMILIA", "LIBROS", libro );
	pControlsLayout->addLayout( libro );
/*>>>>>FRMEDITFAMILIABEHAVIOR_INITGUI*/
}

void FrmEditFamiliaBehavior::scatterFields( bool is_pre )
{
	if( !is_pre ) return;
/*<<<<<FRMEDITFAMILIABEHAVIOR_SCATTER*/
	checklibros->setChecked( pTheForm->getRecord()->getValue("LIBROS").toBool());
/*>>>>>FRMEDITFAMILIABEHAVIOR_SCATTER*/
}

void FrmEditFamiliaBehavior::gatherFields()
{
/*<<<<<FRMEDITFAMILIABEHAVIOR_GATHER*/
	pTheForm->getRecord()->setValue( "LIBROS", checklibros->isChecked());
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
} // namespace editorial
} // namespace gong
/*>>>>>FRMEDITFAMILIABEHAVIOR_FIN*/

