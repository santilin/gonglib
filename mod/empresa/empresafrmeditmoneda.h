/*<<<<<COPYLEFT*/
/** @file empresafrmeditmoneda.h Fichero de edición de monedas
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
/*<<<<<FRMEDITMONEDA_PREAMBLE*/
#ifndef _EMPRESA_EMPRESAFRMEDITMONEDA_H
#define _EMPRESA_EMPRESAFRMEDITMONEDA_H
/*>>>>>FRMEDITMONEDA_PREAMBLE*/
/*<<<<<FRMEDITMONEDA_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
#include "empresarecmoneda.h"
/*>>>>>FRMEDITMONEDA_INCLUDES*/

namespace gong
{
namespace empresa {


/*<<<<<FRMEDITMONEDA_CLASS*/
class FrmEditMoneda: public FrmEditRecMaster
{
	Q_OBJECT

public:
	FrmEditMoneda(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecMoneda *getRecMoneda() const { return (RecMoneda *)getRecord(); }
/*>>>>>FRMEDITMONEDA_CLASS*/
    /*<<<<<FRMEDITMONEDA_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
/*>>>>>FRMEDITMONEDA_VIRTUALS_FROM_FRMEDITREC*/
    /*<<<<<FRMEDITMONEDA_CONTROLS*/
protected:
	gong::EditBox *editCodigo;
	gong::EditBox *editNombre;
	gong::EditBox *editSimbolo;
	gong::EditBox *editDecimales;
	gong::EditBox *editPuntoDecimal;
	gong::EditBox *editSeparadorMiles;
	gong::EditBox *editActualizacion;
	gong::EditBox *editTasaCambio;
/*>>>>>FRMEDITMONEDA_CONTROLS*/
private:
    QLabel *lblEjemplosMoneda;
};
/*<<<<<FRMEDITMONEDA_POSTAMBLE*/
} // namespace empresa
} // namespace gong
#endif // EMPRESA_EMPRESAFRMEDITMONEDA_H
/*>>>>>FRMEDITMONEDA_POSTAMBLE*/

