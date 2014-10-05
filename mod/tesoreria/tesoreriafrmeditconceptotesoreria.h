/*<<<<<FRMEDITCONCEPTOTESORERIA_PREAMBLE*/
#ifndef _TESORERIA_TESORERIAFRMEDITCONCEPTOTESORERIA_H
#define _TESORERIA_TESORERIAFRMEDITCONCEPTOTESORERIA_H
/*>>>>>FRMEDITCONCEPTOTESORERIA_PREAMBLE*/
/*<<<<<FRMEDITCONCEPTOTESORERIA_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
#include "tesoreriarecconceptotesoreria.h"
/*>>>>>FRMEDITCONCEPTOTESORERIA_INCLUDES*/

namespace gong {
namespace tesoreria {


/*<<<<<COPYLEFT*/
/** @file tesoreriafrmeditconceptotesoreria.h Fichero de edición de conceptos de tesoreria
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
/*<<<<<FRMEDITCONCEPTOTESORERIA_CLASS*/
class FrmEditConceptoTesoreria: public FrmEditRecMaster
{
	Q_OBJECT

public:
	FrmEditConceptoTesoreria(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecConceptoTesoreria *getRecConceptoTesoreria() const { return (RecConceptoTesoreria *)getRecord(); }
/*>>>>>FRMEDITCONCEPTOTESORERIA_CLASS*/
    /*<<<<<FRMEDITCONCEPTOTESORERIA_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
/*>>>>>FRMEDITCONCEPTOTESORERIA_VIRTUALS_FROM_FRMEDITREC*/
    /*<<<<<FRMEDITCONCEPTOTESORERIA_CONTROLS*/
protected:
	gong::EditBox *editCodigo;
	gong::EditBox *editNombre;
	gong::TextBox *editNotas;
/*>>>>>FRMEDITCONCEPTOTESORERIA_CONTROLS*/
};
/*<<<<<FRMEDITCONCEPTOTESORERIA_POSTAMBLE*/
} // namespace tesoreria
} // namespace gong
#endif // TESORERIA_TESORERIAFRMEDITCONCEPTOTESORERIA_H
/*>>>>>FRMEDITCONCEPTOTESORERIA_POSTAMBLE*/

