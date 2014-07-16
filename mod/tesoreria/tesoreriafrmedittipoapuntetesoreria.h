/*<<<<<FRMEDITTIPOAPUNTETESORERIA_PREAMBLE*/
#ifndef _TESORERIA_TESORERIAFRMEDITTIPOAPUNTETESORERIA_H
#define _TESORERIA_TESORERIAFRMEDITTIPOAPUNTETESORERIA_H
/*>>>>>FRMEDITTIPOAPUNTETESORERIA_PREAMBLE*/
/*<<<<<FRMEDITTIPOAPUNTETESORERIA_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
#include "tesoreriarectipoapuntetesoreria.h"
/*>>>>>FRMEDITTIPOAPUNTETESORERIA_INCLUDES*/

namespace gong {
namespace tesoreria {


/*<<<<<COPYLEFT*/
/** @file tesoreriafrmedittipoapuntetesoreria.h Fichero de edición de tipos de asiento de tesosería
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
/*<<<<<FRMEDITTIPOAPUNTETESORERIA_CLASS*/
class FrmEditTipoApunteTesoreria: public FrmEditRecMaster
{
	Q_OBJECT

public:
	FrmEditTipoApunteTesoreria(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecTipoApunteTesoreria *getRecTipoApunteTesoreria() const { return (RecTipoApunteTesoreria *)getRecord(); }
/*>>>>>FRMEDITTIPOAPUNTETESORERIA_CLASS*/
/*<<<<<FRMEDITTIPOAPUNTETESORERIA_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
/*>>>>>FRMEDITTIPOAPUNTETESORERIA_VIRTUALS_FROM_FRMEDITREC*/
/*<<<<<FRMEDITTIPOAPUNTETESORERIA_CONTROLS*/
protected:
	gong::EditBox *editCodigo;
	gong::EditBox *editNombre;
	gong::ComboBoxInt *comboPedirFecha;
	gong::EditBox *editFecha;
	gong::ComboBoxInt *comboPedirImporte;
	gong::EditBox *editImporte;
	gong::ComboBoxInt *comboPedirReferencia;
	gong::EditBox *editReferencia;
	gong::TextBox *editNotas;
/*>>>>>FRMEDITTIPOAPUNTETESORERIA_CONTROLS*/
};
/*<<<<<FRMEDITTIPOAPUNTETESORERIA_POSTAMBLE*/
} // namespace tesoreria
} // namespace gong
#endif // TESORERIA_TESORERIAFRMEDITTIPOAPUNTETESORERIA_H
/*>>>>>FRMEDITTIPOAPUNTETESORERIA_POSTAMBLE*/

