/*<<<<<COPYLEFT*/
/** @file depositfrmedittipodocbehavior.h Behavior para añadir campos a TIPODOC
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
/*<<<<<FRMEDITTIPODOCBEHAVIOR_PREAMBLE*/
#ifndef _DEPOSIT_DEPOSITFRMEDITTIPODOCBEHAVIOR_H
#define _DEPOSIT_DEPOSITFRMEDITTIPODOCBEHAVIOR_H
/*>>>>>FRMEDITTIPODOCBEHAVIOR_PREAMBLE*/

/*<<<<<FRMEDITTIPODOCBEHAVIOR_INCLUDES*/
#include <dbappfrmeditrec.h>
#include <dbappfrmeditrecbehavior.h>
/*>>>>>FRMEDITTIPODOCBEHAVIOR_INCLUDES*/
#include <facturectipodoc.h>

namespace gong {
namespace deposit {

typedef factu::RecTipoDoc RecTipoDoc;

/*<<<<<FRMEDITTIPODOCBEHAVIOR_CLASS*/
class FrmEditTipoDocBehavior: public FrmEditRecBehavior
{
public:
	virtual void initGUI();
	RecTipoDoc *getRecTipoDoc() const { return static_cast<RecTipoDoc *>(pTheForm->getRecord()); }
	FrmEditTipoDocBehavior( FrmEditRec *theform )
		: FrmEditRecBehavior( theform )
/*>>>>>FRMEDITTIPODOCBEHAVIOR_CLASS*/
	{}
/*<<<<<FRMEDITTIPODOCBEHAVIOR_VIRTUALS_FROM_FRMEDITRECBEHAVIOR*/
protected:
	virtual void scatterFields( bool is_pre );
	virtual void gatherFields();
	virtual void validateFields( bool is_pre,
		QWidget *sender, bool *isvalid, ValidResult *ir = 0);
/*>>>>>FRMEDITTIPODOCBEHAVIOR_VIRTUALS_FROM_FRMEDITRECBEHAVIOR*/
/*<<<<<FRMEDITTIPODOCBEHAVIOR_CONTROLS*/
protected:
	gong::CheckBox *checkEsDeposito;
	gong::CheckBox *checkEsLiquidacion;
/*>>>>>FRMEDITTIPODOCBEHAVIOR_CONTROLS*/
};

/*<<<<<FRMEDITTIPODOCBEHAVIOR_POSTAMBLE*/
} // namespace deposit
} // namespace gong
#endif // DEPOSIT_DEPOSITFRMEDITTIPODOCBEHAVIOR_H
/*>>>>>FRMEDITTIPODOCBEHAVIOR_POSTAMBLE*/

