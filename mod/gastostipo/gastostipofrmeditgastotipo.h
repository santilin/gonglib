/*<<<<<COPYLEFT*/
/** @file gastostipofrmeditgastotipo.h Fichero de edición de definiciones de gastos e ingresos
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
/*<<<<<FRMEDITGASTOTIPO_PREAMBLE*/
#ifndef _GASTOSTIPO_GASTOSTIPOFRMEDITGASTOTIPO_H
#define _GASTOSTIPO_GASTOSTIPOFRMEDITGASTOTIPO_H
/*>>>>>FRMEDITGASTOTIPO_PREAMBLE*/
/*<<<<<FRMEDITGASTOTIPO_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
#include "gastostiporecgastotipo.h"
/*>>>>>FRMEDITGASTOTIPO_INCLUDES*/

namespace gong {
namespace gastostipo {

/*<<<<<FRMEDITGASTOTIPO_CLASS*/
class FrmEditGastoTipo: public FrmEditRecMaster
{
	Q_OBJECT

public:
	FrmEditGastoTipo(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecGastoTipo *getRecGastoTipo() const { return (RecGastoTipo *)getRecord(); }
/*>>>>>FRMEDITGASTOTIPO_CLASS*/
/*<<<<<FRMEDITGASTOTIPO_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
/*>>>>>FRMEDITGASTOTIPO_VIRTUALS_FROM_FRMEDITREC*/
/*<<<<<FRMEDITGASTOTIPO_CONTROLS*/
protected:
	gong::ComboBoxInt *comboCategoriaGasto;
	gong::ComboBoxInt *comboPedirImporte;
	gong::EditBox *editImporte;
	gong::ComboBoxInt *comboPedirDescripcion;
	gong::EditBox *editDescripcion;
	gong::ComboBoxInt *comboPedirNotas;
	gong::TextBox *editNotas;
/*>>>>>FRMEDITGASTOTIPO_CONTROLS*/
};
/*<<<<<FRMEDITGASTOTIPO_POSTAMBLE*/
} // namespace gastostipo
} // namespace gong
#endif // GASTOSTIPO_GASTOSTIPOFRMEDITGASTOTIPO_H
/*>>>>>FRMEDITGASTOTIPO_POSTAMBLE*/

