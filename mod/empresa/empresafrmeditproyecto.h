/*<<<<<COPYLEFT*/
/** @file empresafrmeditproyecto.h Fichero de edición de proyectos
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
/*<<<<<FRMEDITPROYECTO_PREAMBLE*/
#ifndef _EMPRESA_EMPRESAFRMEDITPROYECTO_H
#define _EMPRESA_EMPRESAFRMEDITPROYECTO_H
/*>>>>>FRMEDITPROYECTO_PREAMBLE*/

/*<<<<<FRMEDITPROYECTO_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
#include "empresarecproyecto.h"
/*>>>>>FRMEDITPROYECTO_INCLUDES*/

namespace gong {
namespace empresa {

/*<<<<<FRMEDITPROYECTO_CLASS*/
class FrmEditProyecto: public FrmEditRecMaster
{
	Q_OBJECT

public:
	FrmEditProyecto(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecProyecto *getRecProyecto() const { return (RecProyecto *)getRecord(); }
/*>>>>>FRMEDITPROYECTO_CLASS*/
    /*<<<<<FRMEDITPROYECTO_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
/*>>>>>FRMEDITPROYECTO_VIRTUALS_FROM_FRMEDITREC*/
    /*<<<<<FRMEDITPROYECTO_CONTROLS*/
protected:
	gong::EditBox *editCodigo;
	gong::EditBox *editNombre;
	gong::TextBox *editNotas;
/*>>>>>FRMEDITPROYECTO_CONTROLS*/
};

/*<<<<<FRMEDITPROYECTO_POSTAMBLE*/
} // namespace empresa
} // namespace gong
#endif // EMPRESA_EMPRESAFRMEDITPROYECTO_H
/*>>>>>FRMEDITPROYECTO_POSTAMBLE*/

