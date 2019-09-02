/*<<<<<COPYLEFT*/
/** @file ventaswebfrmeditfamiliabehavior.h Behavior del formulario de familias
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
/*<<<<<FRMEDITFAMILIABEHAVIOR_PREAMBLE*/
#ifndef _VENTASWEB_VENTASWEBFRMEDITFAMILIABEHAVIOR_H
#define _VENTASWEB_VENTASWEBFRMEDITFAMILIABEHAVIOR_H
/*>>>>>FRMEDITFAMILIABEHAVIOR_PREAMBLE*/

/*<<<<<FRMEDITFAMILIABEHAVIOR_INCLUDES*/
#include <dbappfrmeditrec.h>
#include <dbappfrmeditrecbehavior.h>
/*>>>>>FRMEDITFAMILIABEHAVIOR_INCLUDES*/
#include <facturecfamilia.h>

namespace gong {
namespace ventasweb {
typedef factu::RecFamilia RecFamilia;

/*<<<<<FRMEDITFAMILIABEHAVIOR_CLASS*/
class FrmEditFamiliaBehavior: public FrmEditRecBehavior
{
public:
	virtual void initGUI();
	RecFamilia *getRecFamilia() const { return static_cast<RecFamilia *>(pTheForm->getRecord()); }
	FrmEditFamiliaBehavior( FrmEditRec *theform )
		: FrmEditRecBehavior( theform )
/*>>>>>FRMEDITFAMILIABEHAVIOR_CLASS*/
    {}
    /*<<<<<FRMEDITFAMILIABEHAVIOR_VIRTUALS_FROM_FRMEDITRECBEHAVIOR*/
protected:
	virtual void scatterFields( bool is_pre );
	virtual void gatherFields();
	virtual void validateFields( bool is_pre,
		QWidget *sender, bool *isvalid, ValidResult *ir = 0);
/*>>>>>FRMEDITFAMILIABEHAVIOR_VIRTUALS_FROM_FRMEDITRECBEHAVIOR*/
    /*<<<<<FRMEDITFAMILIABEHAVIOR_CONTROLS*/
protected:
	gong::CheckBox *checkPublicarWeb;
	gong::TextBox *editDescripcionWeb;
/*>>>>>FRMEDITFAMILIABEHAVIOR_CONTROLS*/
};

/*<<<<<FRMEDITFAMILIABEHAVIOR_POSTAMBLE*/
} // namespace ventasweb
} // namespace gong
#endif // VENTASWEB_VENTASWEBFRMEDITFAMILIABEHAVIOR_H
/*>>>>>FRMEDITFAMILIABEHAVIOR_POSTAMBLE*/

