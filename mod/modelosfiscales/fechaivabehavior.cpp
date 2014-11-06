/*<<<<<MODULE_INFO*/
// COPYLEFT Behavior para pedir y validar la fecha de iva de un documento
// FIELD FechaIVA date - desgloseiva
// TYPE FrmEditRecBehavior modulosfiscales::Documento Q_OBJECT
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITDOCUMENTOBEHAVIOR_INCLUDES*/
#include <dbappmainwindow.h>
#include "modulosfiscalesfrmeditdocumentobehavior.h"
/*>>>>>FRMEDITDOCUMENTOBEHAVIOR_INCLUDES*/

namespace gong {
namespace modulosfiscales {

void FrmEditDocumentoBehavior::initGUI()
{
/*<<<<<FRMEDITDOCUMENTOBEHAVIOR_INITGUI*/
	QWidget *pControlsFrame = getControlsFrame();
	QVBoxLayout* pControlsLayout = getControlsLayout();
	QHBoxLayout *desgloseiva = new QHBoxLayout(0, 0, 6, "desgloseiva");
	editFechaIVA = pTheForm->addEditField( pControlsFrame, "DOCUMENTO", "FECHAIVA", desgloseiva );
	pControlsLayout->addLayout( desgloseiva );
/*>>>>>FRMEDITDOCUMENTOBEHAVIOR_INITGUI*/
}

void FrmEditDocumentoBehavior::scatterFields( bool is_pre )
{
	if( !is_pre) return;
/*<<<<<FRMEDITDOCUMENTOBEHAVIOR_SCATTER*/
	editFechaIVA->setText( pTheForm->getRecord()->getValue("FECHAIVA").toDate());
/*>>>>>FRMEDITDOCUMENTOBEHAVIOR_SCATTER*/
    if( editFechaIVA->toDate().isNull() )
        editFechaIVA->setText( editFecha->toDate() );
}

void FrmEditDocumentoBehavior::gatherFields()
{
/*<<<<<FRMEDITDOCUMENTOBEHAVIOR_GATHER*/
	pTheForm->getRecord()->setValue( "FECHAIVA", editFechaIVA->toDate());
/*>>>>>FRMEDITDOCUMENTOBEHAVIOR_GATHER*/
}

void FrmEditDocumentoBehavior::validateFields(bool is_pre,
		QWidget *sender, bool *isvalid, ValidResult *ir)
{
	if( !is_pre) return;
/*<<<<<FRMEDITDOCUMENTOBEHAVIOR_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
/*>>>>>FRMEDITDOCUMENTOBEHAVIOR_VALIDATE*/
    if( sender == editFecha && editFecha->isJustEdited() )
        editFechaIVA->setText( editFecha->toDate() );
    if( sender == editFechaIVA || sender == editFecha || !sender ) {
        if( editFechaIVA->toDate().isNull() ) {
            validresult->addError(_("La fecha del IVA no puede estar vacía."),
                                  "FECHAIVA" );
        } else {
            Date primerodetrimestre = Date( editFecha->toDate().getYear(),
                                            ((editFecha->toDate().getQuarter()-1) * 3) + 1, 1 );
            Date primerodetrimestresiguiente = (primerodetrimestre + 93).firstDayOfMonth();
            if( editFechaIVA->toDate() < primerodetrimestre ) {
                validresult->addError(_("La fecha del IVA no puede ser de un ejercicio o trimestre anterior a la fecha de la factura"),
                                      "FECHAIVA" );
                *isvalid = false;
            } else if( editFechaIVA->toDate() >= primerodetrimestresiguiente ) {
                validresult->addError(_("La fecha del IVA es de un ejercicio o trimestre posterior a la fecha de la factura."),
                                      "FECHAIVA" );
            }
        }
    }
	if( !ir ) {
		showValidMessages(isvalid, *validresult, sender);
		delete validresult;
	}
}

/*<<<<<FRMEDITDOCUMENTOBEHAVIOR_FIN*/
} // namespace modulosfiscales
} // namespace gong
/*>>>>>FRMEDITDOCUMENTOBEHAVIOR_FIN*/

