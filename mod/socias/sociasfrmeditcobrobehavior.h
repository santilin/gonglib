/*<<<<<COPYLEFT*/
/** @file sociasfrmeditcobrobehavior.h Behavior para el formulario de remesas del módulo socias
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
/*<<<<<FRMEDITCOBROBEHAVIOR_PREAMBLE*/
#ifndef _SOCIAS_SOCIASFRMEDITCOBROBEHAVIOR_H
#define _SOCIAS_SOCIASFRMEDITCOBROBEHAVIOR_H
/*>>>>>FRMEDITCOBROBEHAVIOR_PREAMBLE*/

/*<<<<<FRMEDITCOBROBEHAVIOR_INCLUDES*/
#include <dbappfrmeditrec.h>
#include <dbappfrmeditrecbehavior.h>
/*>>>>>FRMEDITCOBROBEHAVIOR_INCLUDES*/
#include <pagosreccobro.h>

namespace gong {
namespace socias {

typedef pagos::RecCobro RecCobro;

/*<<<<<FRMEDITCOBROBEHAVIOR_CLASS*/
class FrmEditCobroBehavior: public FrmEditRecBehavior
{
public:
	virtual void initGUI();
	RecCobro *getRecCobro() const { return static_cast<RecCobro *>(pTheForm->getRecord()); }
	FrmEditCobroBehavior( FrmEditRec *theform )
		: FrmEditRecBehavior( theform )
/*>>>>>FRMEDITCOBROBEHAVIOR_CLASS*/
    {}
    // virtual void updateStatus();
    // virtual bool canClose();
    /*<<<<<FRMEDITCOBROBEHAVIOR_VIRTUALS_FROM_FRMEDITRECBEHAVIOR*/
protected:
	virtual void scatterFields( bool is_pre );
	virtual void gatherFields();
	virtual void validateFields( bool is_pre,
		QWidget *sender, bool *isvalid, ValidResult *ir = 0);
/*>>>>>FRMEDITCOBROBEHAVIOR_VIRTUALS_FROM_FRMEDITRECBEHAVIOR*/
    virtual bool canBeginEdit(DataTable::EditMode newmode);
    /*<<<<<FRMEDITCOBROBEHAVIOR_CONTROLS*/
protected:
/*>>>>>FRMEDITCOBROBEHAVIOR_CONTROLS*/
};

/*<<<<<FRMEDITCOBROBEHAVIOR_POSTAMBLE*/
} // namespace socias
} // namespace gong
#endif // SOCIAS_SOCIASFRMEDITCOBROBEHAVIOR_H
/*>>>>>FRMEDITCOBROBEHAVIOR_POSTAMBLE*/

