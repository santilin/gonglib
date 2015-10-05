/*<<<<<FRMEDITEMPRESABEHAVIOR_PREAMBLE*/
#ifndef _PAGOS_PAGOSFRMEDITEMPRESABEHAVIOR_H
#define _PAGOS_PAGOSFRMEDITEMPRESABEHAVIOR_H
/*>>>>>FRMEDITEMPRESABEHAVIOR_PREAMBLE*/
/*<<<<<COPYLEFT*/
/** @file pagosfrmeditempresabehavior.h Behavior para añadir el campo formato número de recibo
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
/*<<<<<FRMEDITEMPRESABEHAVIOR_INCLUDES*/
#include <dbappfrmeditrec.h>
#include <dbappfrmeditrecbehavior.h>
/*>>>>>FRMEDITEMPRESABEHAVIOR_INCLUDES*/
#include <empresamodule.h>

namespace gong {
	
typedef empresa::RecEmpresa RecEmpresa;	
	
namespace pagos {

/*<<<<<FRMEDITEMPRESABEHAVIOR_CLASS*/
class FrmEditEmpresaBehavior: public FrmEditRecBehavior
{
public:
	virtual void initGUI();
	RecEmpresa *getRecEmpresa() const { return static_cast<RecEmpresa *>(pTheForm->getRecord()); }
	FrmEditEmpresaBehavior( FrmEditRec *theform )
		: FrmEditRecBehavior( theform )
/*>>>>>FRMEDITEMPRESABEHAVIOR_CLASS*/
	{}
	// virtual void updateStatus();
	// virtual bool canClose();
/*<<<<<FRMEDITEMPRESABEHAVIOR_VIRTUALS_FROM_FRMEDITRECBEHAVIOR*/
protected:
	virtual void scatterFields( bool is_pre );
	virtual void gatherFields();
	virtual void validateFields( bool is_pre,
		QWidget *sender, bool *isvalid, ValidResult *ir = 0);
/*>>>>>FRMEDITEMPRESABEHAVIOR_VIRTUALS_FROM_FRMEDITRECBEHAVIOR*/
/*<<<<<FRMEDITEMPRESABEHAVIOR_CONTROLS*/
protected:
	gong::EditBox *editFormatoNumRecibo;
/*>>>>>FRMEDITEMPRESABEHAVIOR_CONTROLS*/
};
/*<<<<<FRMEDITEMPRESABEHAVIOR_POSTAMBLE*/
} // namespace pagos
} // namespace gong
#endif // PAGOS_PAGOSFRMEDITEMPRESABEHAVIOR_H
/*>>>>>FRMEDITEMPRESABEHAVIOR_POSTAMBLE*/

