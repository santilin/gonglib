/*<<<<<COPYLEFT*/
/** @file produccionfrmeditlotedet.h Fichero de edición de detalles de dotes
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
/*<<<<<FRMEDITLOTEDET_PREAMBLE*/
#ifndef _PRODUCCION_PRODUCCIONFRMEDITLOTEDET_H
#define _PRODUCCION_PRODUCCIONFRMEDITLOTEDET_H
/*>>>>>FRMEDITLOTEDET_PREAMBLE*/
/*<<<<<FRMEDITLOTEDET_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecdetail.h>
#include "produccionreclotedet.h"
/*>>>>>FRMEDITLOTEDET_INCLUDES*/
#include <facturecarticulo.h>

namespace gong {
namespace produccion {


/*<<<<<FRMEDITLOTEDET_CLASS*/
class FrmEditLoteDet: public FrmEditRecDetail
{
	Q_OBJECT

public:
	FrmEditLoteDet(FrmEditRecMaster *frmmaster, int ndetail,
		dbRecord *detail, const Xtring &dettablename, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecLoteDet *getRecLoteDet() const { return (RecLoteDet *)getRecord(); }
/*>>>>>FRMEDITLOTEDET_CLASS*/
/*<<<<<FRMEDITLOTEDET_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
	void specialControlKeyPressed(QWidget *sender, char key);
/*>>>>>FRMEDITLOTEDET_VIRTUALS_FROM_FRMEDITREC*/

/*<<<<<FRMEDITLOTEDET_SCATTERS_AND_SLOTS*/
protected:
	void scatterArticulo();

private slots:
	void pushArticuloCodigo_clicked();

public:
	factu::RecArticulo* getRecArticulo() const
		{ return static_cast<RecLoteDet*>(getRecord())->getRecArticulo(); }
/*>>>>>FRMEDITLOTEDET_SCATTERS_AND_SLOTS*/

/*<<<<<FRMEDITLOTEDET_CONTROLS*/
protected:
	gong::EditBox *editCantidad;
	gong::SearchBox *searchArticuloCodigo;
	QPushButton *pushArticuloCodigo;
	gong::LineEdit *editArticuloCodigo;
	gong::LineEdit *editArticuloNombre;
/*>>>>>FRMEDITLOTEDET_CONTROLS*/
};
/*<<<<<FRMEDITLOTEDET_POSTAMBLE*/
} // namespace produccion
} // namespace gong
#endif // PRODUCCION_PRODUCCIONFRMEDITLOTEDET_H
/*>>>>>FRMEDITLOTEDET_POSTAMBLE*/

