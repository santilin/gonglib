/*<<<<<COPYLEFT*/
/** @file depositfrmeditventadetbehavior.h Behavior para añadir campos a los detalles de las ventas
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
/*<<<<<FRMEDITVENTADETBEHAVIOR_PREAMBLE*/
#ifndef _DEPOSIT_DEPOSITFRMEDITVENTADETBEHAVIOR_H
#define _DEPOSIT_DEPOSITFRMEDITVENTADETBEHAVIOR_H
/*>>>>>FRMEDITVENTADETBEHAVIOR_PREAMBLE*/

/*<<<<<FRMEDITVENTADETBEHAVIOR_INCLUDES*/
#include <dbappfrmeditrec.h>
#include <dbappfrmeditrecbehavior.h>
/*>>>>>FRMEDITVENTADETBEHAVIOR_INCLUDES*/

namespace gong {
namespace deposit {

/*<<<<<FRMEDITVENTADETBEHAVIOR_CLASS*/
class FrmEditVentaDetBehavior: public FrmEditRecBehavior
{
public:
	virtual void initGUI();
	RecVentaDet *getRecVentaDet() const { return static_cast<RecVentaDet *>(pTheForm->getRecord()); }
	FrmEditVentaDetBehavior( FrmEditRec *theform )
		: FrmEditRecBehavior( theform )
/*>>>>>FRMEDITVENTADETBEHAVIOR_CLASS*/
// {capel} Borrar el RecVentaDet
	{}
/*<<<<<FRMEDITVENTADETBEHAVIOR_VIRTUALS_FROM_FRMEDITRECBEHAVIOR*/
protected:
	virtual void scatterFields( bool is_pre );
	virtual void gatherFields();
	virtual void validateFields( bool is_pre,
		QWidget *sender, bool *isvalid, ValidResult *ir = 0);
/*>>>>>FRMEDITVENTADETBEHAVIOR_VIRTUALS_FROM_FRMEDITRECBEHAVIOR*/
/*<<<<<FRMEDITVENTADETBEHAVIOR_CONTROLS*/
protected:
	gong::EditBox *editImporteLiquidacion;
	gong::EditBox *editImporteLiquidacionIVA;
/*>>>>>FRMEDITVENTADETBEHAVIOR_CONTROLS*/
	gong::EditBox *pEditCantidad, *pEditArticuloCodigo;
	gong::CheckBox *pCheckLiquidada;
};

/*<<<<<FRMEDITTIPODOCBEHAVIOR_POSTAMBLE*/
/*<<<<<FRMEDITVENTADETBEHAVIOR_POSTAMBLE*/
} // namespace deposit
} // namespace gong
#endif // DEPOSIT_DEPOSITFRMEDITVENTADETBEHAVIOR_H
/*>>>>>FRMEDITVENTADETBEHAVIOR_POSTAMBLE*/

