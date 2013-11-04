/*<<<<<COPYLEFT*/
/** @file factufrmeditfamilia.h Fichero de edición de familias de artículos
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
/*<<<<<FRMEDITFAMILIA_PREAMBLE*/
#ifndef _FACTU_FACTUFRMEDITFAMILIA_H
#define _FACTU_FACTUFRMEDITFAMILIA_H
/*>>>>>FRMEDITFAMILIA_PREAMBLE*/

#include "config.h"
/*<<<<<FRMEDITFAMILIA_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
#include "facturecfamilia.h"
/*>>>>>FRMEDITFAMILIA_INCLUDES*/
#include "empresarectipoiva.h"

namespace gong {
namespace factu {

/*<<<<<FRMEDITFAMILIA_CLASS*/
class FrmEditFamilia: public FrmEditRecMaster
{
	Q_OBJECT

public:
	FrmEditFamilia(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecFamilia *getRecFamilia() const { return (RecFamilia *)getRecord(); }
/*>>>>>FRMEDITFAMILIA_CLASS*/

    /*<<<<<FRMEDITFAMILIA_SCATTERS_AND_SLOTS*/
protected:
	void scatterTipoIVA();

private slots:
	void pushTipoIVACodigo_clicked();

public:
	empresa::RecTipoIVA* getRecTipoIVA() const
		{ return static_cast<RecFamilia*>(getRecord())->getRecTipoIVA(); }
/*>>>>>FRMEDITFAMILIA_SCATTERS_AND_SLOTS*/

    /*<<<<<FRMEDITFAMILIA_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
	void specialControlKeyPressed(QWidget *sender, char key);
/*>>>>>FRMEDITFAMILIA_VIRTUALS_FROM_FRMEDITREC*/
    /*<<<<<FRMEDITFAMILIA_CONTROLS*/
protected:
	gong::EditBox *editCodigo;
	gong::EditBox *editNombre;
	gong::SearchBox *searchTipoIVACodigo;
	QPushButton *pushTipoIVACodigo;
	gong::LineEdit *editTipoIVACodigo;
	gong::LineEdit *editTipoIVANombre;
#ifdef HAVE_CONTABMODULE
	gong::EditBox *editSubcuentaCompras;
#endif
#ifdef HAVE_CONTABMODULE
	gong::EditBox *editSubcuentaVentas;
#endif
	gong::TextBox *editNotas;
/*>>>>>FRMEDITFAMILIA_CONTROLS*/
};

/*<<<<<FRMEDITFAMILIA_POSTAMBLE*/
} // namespace factu
} // namespace gong
#endif // FACTU_FACTUFRMEDITFAMILIA_H
/*>>>>>FRMEDITFAMILIA_POSTAMBLE*/


