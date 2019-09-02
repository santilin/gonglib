/*<<<<<COPYLEFT*/
/** @file produccionfrmeditlote.h Fichero de edición de lotes
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
/*<<<<<FRMEDITLOTE_PREAMBLE*/
#ifndef _PRODUCCION_PRODUCCIONFRMEDITLOTE_H
#define _PRODUCCION_PRODUCCIONFRMEDITLOTE_H
/*>>>>>FRMEDITLOTE_PREAMBLE*/
/*<<<<<FRMEDITLOTE_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
#include "produccionreclote.h"
#include "produccionfrmeditlotedet.h"
/*>>>>>FRMEDITLOTE_INCLUDES*/

namespace gong {
namespace produccion {


/*<<<<<FRMEDITLOTE_CLASS*/
class FrmEditLote: public FrmEditRecMaster
{
	Q_OBJECT

public:
	FrmEditLote(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecLote *getRecLote() const { return (RecLote *)getRecord(); }
/*>>>>>FRMEDITLOTE_CLASS*/
/*<<<<<FRMEDITLOTE_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
/*>>>>>FRMEDITLOTE_VIRTUALS_FROM_FRMEDITREC*/
/*<<<<<FRMEDITLOTE_CONTROLS*/
protected:
	gong::EditBox *editCodigo;
	gong::EditBox *editDescripcion;
	gong::EditBox *editFechaFabricacion;
	gong::EditBox *editFechaCaducidad;
	FrmEditLoteDet *pFrmLoteDet;
	gong::EditBox *editCantidad;
/*>>>>>FRMEDITLOTE_CONTROLS*/
};
/*<<<<<FRMEDITLOTE_POSTAMBLE*/
} // namespace produccion
} // namespace gong
#endif // PRODUCCION_PRODUCCIONFRMEDITLOTE_H
/*>>>>>FRMEDITLOTE_POSTAMBLE*/

