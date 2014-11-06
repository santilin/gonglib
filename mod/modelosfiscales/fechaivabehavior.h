/*<<<<<FRMEDITDOCUMENTOBEHAVIOR_PREAMBLE*/
#ifndef _MODULOSFISCALES_FECHAIVABEHAVIOR_H
#define _MODULOSFISCALES_FECHAIVABEHAVIOR_H
/*>>>>>FRMEDITDOCUMENTOBEHAVIOR_PREAMBLE*/
/*<<<<<FRMEDITDOCUMENTOBEHAVIOR_INCLUDES*/
#include <dbappfrmeditrec.h>
#include <dbappfrmeditrecbehavior.h>
/*>>>>>FRMEDITDOCUMENTOBEHAVIOR_INCLUDES*/

namespace gong {
namespace modulosfiscales {


/*<<<<<COPYLEFT*/
/** @file fechaivabehavior.h Behavior para pedir y validar la fecha de iva de un documento
 * Proyecto GestiONG. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */
/*>>>>>COPYLEFT*/
/*<<<<<FRMEDITDOCUMENTOBEHAVIOR_CLASS*/
class FrmEditDocumentoBehavior: public FrmEditRecBehavior
{
	Q_OBJECT
public:
	virtual void initGUI();
	RecDocumento *getRecDocumento() const { return static_cast<RecDocumento *>(pTheForm->getRecord()); }
	FrmEditDocumentoBehavior( FrmEditRec *theform )
		: FrmEditRecBehavior( theform )
/*>>>>>FRMEDITDOCUMENTOBEHAVIOR_CLASS*/
	{}
	// virtual void updateStatus();
	// virtual bool canClose();
/*<<<<<FRMEDITDOCUMENTOBEHAVIOR_VIRTUALS_FROM_FRMEDITRECBEHAVIOR*/
protected:
	virtual void scatterFields( bool is_pre );
	virtual void gatherFields();
	virtual void validateFields( bool is_pre,
		QWidget *sender, bool *isvalid, ValidResult *ir = 0);
/*>>>>>FRMEDITDOCUMENTOBEHAVIOR_VIRTUALS_FROM_FRMEDITRECBEHAVIOR*/
/*<<<<<FRMEDITDOCUMENTOBEHAVIOR_CONTROLS*/
protected:
	gong::EditBox *editFechaIVA;
/*>>>>>FRMEDITDOCUMENTOBEHAVIOR_CONTROLS*/
};
/*<<<<<FRMEDITDOCUMENTOBEHAVIOR_POSTAMBLE*/
} // namespace modulosfiscales
} // namespace gong
#endif // MODULOSFISCALES_FECHAIVABEHAVIOR_H
/*>>>>>FRMEDITDOCUMENTOBEHAVIOR_POSTAMBLE*/

