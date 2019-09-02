/*<<<<<COPYLEFT*/
/** @file tpvfrmeditagentebehavior.h Behavior del formulario de agentes para el módulo TPV
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
/*<<<<<FRMEDITAGENTEBEHAVIOR_PREAMBLE*/
#ifndef _TPV_TPVFRMEDITAGENTEBEHAVIOR_H
#define _TPV_TPVFRMEDITAGENTEBEHAVIOR_H
/*>>>>>FRMEDITAGENTEBEHAVIOR_PREAMBLE*/
/*<<<<<FRMEDITAGENTEBEHAVIOR_INCLUDES*/
#include <dbappfrmeditrec.h>
#include <dbappfrmeditrecbehavior.h>
/*>>>>>FRMEDITAGENTEBEHAVIOR_INCLUDES*/
#include <facturecagente.h>

namespace gong {

namespace tpv {

typedef gong::factu::RecAgente RecAgente;

/*<<<<<FRMEDITAGENTEBEHAVIOR_CLASS*/
class FrmEditAgenteBehavior: public FrmEditRecBehavior
{
public:
	virtual void initGUI();
	RecAgente *getRecAgente() const { return static_cast<RecAgente *>(pTheForm->getRecord()); }
	FrmEditAgenteBehavior( FrmEditRec *theform )
		: FrmEditRecBehavior( theform )
/*>>>>>FRMEDITAGENTEBEHAVIOR_CLASS*/
	{}
	// virtual void updateStatus();
	// virtual bool canClose();
/*<<<<<FRMEDITAGENTEBEHAVIOR_VIRTUALS_FROM_FRMEDITRECBEHAVIOR*/
protected:
	virtual void scatterFields( bool is_pre );
	virtual void gatherFields();
	virtual void validateFields( bool is_pre,
		QWidget *sender, bool *isvalid, ValidResult *ir = 0);
/*>>>>>FRMEDITAGENTEBEHAVIOR_VIRTUALS_FROM_FRMEDITRECBEHAVIOR*/
/*<<<<<FRMEDITAGENTEBEHAVIOR_CONTROLS*/
protected:
	gong::CheckBox *checkParaTPV;
/*>>>>>FRMEDITAGENTEBEHAVIOR_CONTROLS*/
};

/*<<<<<FRMEDITAGENTEBEHAVIOR_POSTAMBLE*/
} // namespace tpv
} // namespace gong
#endif // TPV_TPVFRMEDITAGENTEBEHAVIOR_H
/*>>>>>FRMEDITAGENTEBEHAVIOR_POSTAMBLE*/

