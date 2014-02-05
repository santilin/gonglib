/*<<<<<FRMEDITARTICULOBEHAVIOR_PREAMBLE*/
#ifndef _ECOTIENDA_ECOTIENDAFRMEDITARTICULOBEHAVIOR_H
#define _ECOTIENDA_ECOTIENDAFRMEDITARTICULOBEHAVIOR_H
/*>>>>>FRMEDITARTICULOBEHAVIOR_PREAMBLE*/
/*<<<<<COPYLEFT*/
/** @file ecotiendafrmeditarticulobehavior.h Behavior del formulario de artículos para el módulo ecotienda
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
/*<<<<<FRMEDITARTICULOBEHAVIOR_INCLUDES*/
#include <dbappfrmeditrec.h>
#include <dbappfrmeditrecbehavior.h>
/*>>>>>FRMEDITARTICULOBEHAVIOR_INCLUDES*/
#include <facturecarticulo.h>

namespace gong {
namespace ecotienda {

typedef factu::RecArticulo RecArticulo;

/*<<<<<FRMEDITARTICULOBEHAVIOR_CLASS*/
class FrmEditArticuloBehavior: public FrmEditRecBehavior
{
	Q_OBJECT
public:
	virtual void initGUI();
	RecArticulo *getRecArticulo() const { return static_cast<RecArticulo *>(pTheForm->getRecord()); }
	FrmEditArticuloBehavior( FrmEditRec *theform )
		: FrmEditRecBehavior( theform )
/*>>>>>FRMEDITARTICULOBEHAVIOR_CLASS*/
	{}
	// virtual void updateStatus();
	// virtual bool canClose();
/*<<<<<FRMEDITARTICULOBEHAVIOR_VIRTUALS_FROM_FRMEDITRECBEHAVIOR*/
protected:
	virtual void scatterFields( bool is_pre );
	virtual void gatherFields();
	virtual void validateFields( bool is_pre,
		QWidget *sender, bool *isvalid, ValidResult *ir = 0);
/*>>>>>FRMEDITARTICULOBEHAVIOR_VIRTUALS_FROM_FRMEDITRECBEHAVIOR*/
/*<<<<<FRMEDITARTICULOBEHAVIOR_CONTROLS*/
protected:
	gong::EditBox *editAval;
	gong::EditBox *editPesoNeto;
	gong::EditBox *editUnidadesCaja;
/*>>>>>FRMEDITARTICULOBEHAVIOR_CONTROLS*/
};
/*<<<<<FRMEDITARTICULOBEHAVIOR_POSTAMBLE*/
} // namespace ecotienda
} // namespace gong
#endif // ECOTIENDA_ECOTIENDAFRMEDITARTICULOBEHAVIOR_H
/*>>>>>FRMEDITARTICULOBEHAVIOR_POSTAMBLE*/

