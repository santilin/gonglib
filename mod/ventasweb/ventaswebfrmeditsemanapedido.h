/*<<<<<COPYLEFT*/
/** @file ventaswebfrmeditsemanapedido.h Fichero de edición de
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
/*<<<<<FRMEDITSEMANAPEDIDO_PREAMBLE*/
#ifndef _VENTASWEB_VENTASWEBFRMEDITSEMANAPEDIDO_H
#define _VENTASWEB_VENTASWEBFRMEDITSEMANAPEDIDO_H
/*>>>>>FRMEDITSEMANAPEDIDO_PREAMBLE*/
/*<<<<<FRMEDITSEMANAPEDIDO_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
#include "ventaswebrecsemanapedido.h"
/*>>>>>FRMEDITSEMANAPEDIDO_INCLUDES*/

namespace gong {
namespace ventasweb {


/*<<<<<FRMEDITSEMANAPEDIDO_CLASS*/
class FrmEditSemanaPedido: public FrmEditRecMaster
{
	Q_OBJECT

public:
	FrmEditSemanaPedido(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecSemanaPedido *getRecSemanaPedido() const { return (RecSemanaPedido *)getRecord(); }
/*>>>>>FRMEDITSEMANAPEDIDO_CLASS*/
    /*<<<<<FRMEDITSEMANAPEDIDO_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
/*>>>>>FRMEDITSEMANAPEDIDO_VIRTUALS_FROM_FRMEDITREC*/
    /*<<<<<FRMEDITSEMANAPEDIDO_CONTROLS*/
protected:
	gong::EditBox *editNumeroSemana;
	gong::EditBox *editFechaInicioPedido;
	gong::EditBox *editFechaFinPedido;
	gong::EditBox *editFechaRecogida;
	gong::TextBox *editNotas;
/*>>>>>FRMEDITSEMANAPEDIDO_CONTROLS*/
};
/*<<<<<FRMEDITSEMANAPEDIDO_POSTAMBLE*/
} // namespace ventasweb
} // namespace gong
#endif // VENTASWEB_VENTASWEBFRMEDITSEMANAPEDIDO_H
/*>>>>>FRMEDITSEMANAPEDIDO_POSTAMBLE*/

