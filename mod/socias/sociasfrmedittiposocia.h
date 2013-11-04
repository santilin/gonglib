/*<<<<<COPYLEFT*/
/** @file sociasfrmedittiposocia.h Formulario de edición de tipos de socia
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
/*<<<<<FRMEDITTIPOSOCIA_PREAMBLE*/
#ifndef _SOCIAS_SOCIASFRMEDITTIPOSOCIA_H
#define _SOCIAS_SOCIASFRMEDITTIPOSOCIA_H
/*>>>>>FRMEDITTIPOSOCIA_PREAMBLE*/

/*<<<<<FRMEDITTIPOSOCIA_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
#include "sociasrectiposocia.h"
/*>>>>>FRMEDITTIPOSOCIA_INCLUDES*/

namespace gong {
namespace socias {

/*<<<<<FRMEDITTIPOSOCIA_CLASS*/
class FrmEditTipoSocia: public FrmEditRecMaster
{
	Q_OBJECT

public:
	FrmEditTipoSocia(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecTipoSocia *getRecTipoSocia() const { return (RecTipoSocia *)getRecord(); }
/*>>>>>FRMEDITTIPOSOCIA_CLASS*/
    /*<<<<<FRMEDITTIPOSOCIA_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
/*>>>>>FRMEDITTIPOSOCIA_VIRTUALS_FROM_FRMEDITREC*/

    /*<<<<<FRMEDITTIPOSOCIA_CONTROLS*/
protected:
	gong::EditBox *editCodigo;
	gong::EditBox *editNombre;
	gong::EditBox *editPorcentajeCuota;
	gong::TextBox *editNotas;
/*>>>>>FRMEDITTIPOSOCIA_CONTROLS*/
};

/*<<<<<FRMEDITTIPOSOCIA_POSTAMBLE*/
} // namespace socias
} // namespace gong
#endif // SOCIAS_SOCIASFRMEDITTIPOSOCIA_H
/*>>>>>FRMEDITTIPOSOCIA_POSTAMBLE*/
