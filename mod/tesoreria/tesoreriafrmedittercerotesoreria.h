/*<<<<<FRMEDITTERCEROTESORERIA_PREAMBLE*/
#ifndef _TESORERIA_TESORERIAFRMEDITTERCEROTESORERIA_H
#define _TESORERIA_TESORERIAFRMEDITTERCEROTESORERIA_H
/*>>>>>FRMEDITTERCEROTESORERIA_PREAMBLE*/
/*<<<<<COPYLEFT*/
/** @file tesoreriafrmedittercerotesoreria.h Fichero de edición de terceros de tesoreria
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

/*<<<<<FRMEDITTERCEROTESORERIA_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
#include "tesoreriarectercerotesoreria.h"
/*>>>>>FRMEDITTERCEROTESORERIA_INCLUDES*/

namespace gong {
namespace tesoreria {


/*<<<<<FRMEDITTERCEROTESORERIA_CLASS*/
class FrmEditTerceroTesoreria: public FrmEditRecMaster
{
	Q_OBJECT

public:
	FrmEditTerceroTesoreria(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecTerceroTesoreria *getRecTerceroTesoreria() const { return (RecTerceroTesoreria *)getRecord(); }
/*>>>>>FRMEDITTERCEROTESORERIA_CLASS*/
/*<<<<<FRMEDITTERCEROTESORERIA_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
/*>>>>>FRMEDITTERCEROTESORERIA_VIRTUALS_FROM_FRMEDITREC*/
/*<<<<<FRMEDITTERCEROTESORERIA_CONTROLS*/
protected:
	gong::EditBox *editCodigo;
	gong::EditBox *editNombre;
	gong::TextBox *editNotas;
/*>>>>>FRMEDITTERCEROTESORERIA_CONTROLS*/
};
/*<<<<<FRMEDITTERCEROTESORERIA_POSTAMBLE*/
} // namespace tesoreria
} // namespace gong
#endif // TESORERIA_TESORERIAFRMEDITTERCEROTESORERIA_H
/*>>>>>FRMEDITTERCEROTESORERIA_POSTAMBLE*/

