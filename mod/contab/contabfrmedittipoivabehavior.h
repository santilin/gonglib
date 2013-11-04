/*<<<<<COPYLEFT*/
/** @file contabfrmedittipoivabehavior.h Fichero de edición de tipos de IVA
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
/*<<<<<FRMEDITTIPOIVABEHAVIOR_PREAMBLE*/
#ifndef _CONTAB_CONTABFRMEDITTIPOIVABEHAVIOR_H
#define _CONTAB_CONTABFRMEDITTIPOIVABEHAVIOR_H
/*>>>>>FRMEDITTIPOIVABEHAVIOR_PREAMBLE*/
/*<<<<<FRMEDITTIPOIVABEHAVIOR_INCLUDES*/
#include <dbappfrmeditrec.h>
#include <dbappfrmeditrecbehavior.h>
/*>>>>>FRMEDITTIPOIVABEHAVIOR_INCLUDES*/
#include <empresarectipoiva.h>

namespace gong {
namespace contab {

typedef empresa::RecTipoIVA RecTipoIVA;

/*<<<<<FRMEDITTIPOIVABEHAVIOR_CLASS*/
class FrmEditTipoIVABehavior: public FrmEditRecBehavior
{
public:
	virtual void initGUI();
	RecTipoIVA *getRecTipoIVA() const { return static_cast<RecTipoIVA *>(pTheForm->getRecord()); }
	FrmEditTipoIVABehavior( FrmEditRec *theform )
		: FrmEditRecBehavior( theform )
/*>>>>>FRMEDITTIPOIVABEHAVIOR_CLASS*/
    {}
    /*<<<<<FRMEDITTIPOIVABEHAVIOR_VIRTUALS_FROM_FRMEDITRECBEHAVIOR*/
protected:
	virtual void scatterFields( bool is_pre );
	virtual void gatherFields();
	virtual void validateFields( bool is_pre,
		QWidget *sender, bool *isvalid, ValidResult *ir = 0);
/*>>>>>FRMEDITTIPOIVABEHAVIOR_VIRTUALS_FROM_FRMEDITRECBEHAVIOR*/
    /*<<<<<FRMEDITTIPOIVABEHAVIOR_CONTROLS*/
protected:
	gong::EditBox *editSubcuentaSoportado;
	gong::EditBox *editSubcuentaRepercutido;
/*>>>>>FRMEDITTIPOIVABEHAVIOR_CONTROLS*/
};
/*<<<<<FRMEDITTIPOIVABEHAVIOR_POSTAMBLE*/
} // namespace contab
} // namespace gong
#endif // CONTAB_CONTABFRMEDITTIPOIVABEHAVIOR_H
/*>>>>>FRMEDITTIPOIVABEHAVIOR_POSTAMBLE*/

