/*<<<<<COPYLEFT*/
/** @file gastosfrmeditgasto.h Fichero de edición de
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */
/*>>>>>COPYLEFT*/
/*<<<<<FRMEDITGASTO_PREAMBLE*/
#ifndef _GASTOS_GASTOSFRMEDITGASTO_H
#define _GASTOS_GASTOSFRMEDITGASTO_H
/*>>>>>FRMEDITGASTO_PREAMBLE*/
/*<<<<<FRMEDITGASTO_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
#include "gastosrecgasto.h"
/*>>>>>FRMEDITGASTO_INCLUDES*/

namespace gong {
namespace gastos {

/*<<<<<FRMEDITGASTO_CLASS*/
class FrmEditGasto: public FrmEditRecMaster
{
	Q_OBJECT

public:
	FrmEditGasto(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecGasto *getRecGasto() const { return (RecGasto *)getRecord(); }
/*>>>>>FRMEDITGASTO_CLASS*/
/*<<<<<FRMEDITGASTO_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
/*>>>>>FRMEDITGASTO_VIRTUALS_FROM_FRMEDITREC*/
/*<<<<<FRMEDITGASTO_CONTROLS*/
protected:
	gong::ComboBox<int> *comboTipo;
/*>>>>>FRMEDITGASTO_CONTROLS*/
};
/*<<<<<FRMEDITGASTO_POSTAMBLE*/
} // namespace gastos
} // namespace gong
#endif // GASTOS_GASTOSFRMEDITGASTO_H
/*>>>>>FRMEDITGASTO_POSTAMBLE*/

