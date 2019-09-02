/*<<<<<COPYLEFT*/
/** @file produccionfrmeditarticulocomponente.h Fichero de edición de productos de artículos
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
/*<<<<<FRMEDITARTICULOCOMPONENTE_PREAMBLE*/
#ifndef _PRODUCCION_PRODUCCIONFRMEDITARTICULOCOMPONENTE_H
#define _PRODUCCION_PRODUCCIONFRMEDITARTICULOCOMPONENTE_H
/*>>>>>FRMEDITARTICULOCOMPONENTE_PREAMBLE*/

/*<<<<<FRMEDITARTICULOCOMPONENTE_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecdetail.h>
#include "produccionrecarticulocomponente.h"
/*>>>>>FRMEDITARTICULOCOMPONENTE_INCLUDES*/
#include <facturecarticulo.h>

namespace gong {
namespace produccion {

/*<<<<<FRMEDITARTICULOCOMPONENTE_CLASS*/
class FrmEditArticuloComponente: public FrmEditRecDetail
{
	Q_OBJECT

public:
	FrmEditArticuloComponente(FrmEditRecMaster *frmmaster, int ndetail,
		dbRecord *detail, const Xtring &dettablename, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecArticuloComponente *getRecArticuloComponente() const { return (RecArticuloComponente *)getRecord(); }
/*>>>>>FRMEDITARTICULOCOMPONENTE_CLASS*/
/*<<<<<FRMEDITARTICULOCOMPONENTE_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
	void specialControlKeyPressed(QWidget *sender, char key);
/*>>>>>FRMEDITARTICULOCOMPONENTE_VIRTUALS_FROM_FRMEDITREC*/
/*<<<<<FRMEDITARTICULOCOMPONENTE_SCATTERS_AND_SLOTS*/
protected:
	void scatterArticulo();

private slots:
	void pushArticuloCodigo_clicked();

public:
	RecArticulo* getRecArticulo() const
		{ return static_cast<RecArticuloComponente*>(getRecord())->getRecArticulo(); }
/*>>>>>FRMEDITARTICULOCOMPONENTE_SCATTERS_AND_SLOTS*/
/*<<<<<FRMEDITARTICULOCOMPONENTE_CONTROLS*/
protected:
	gong::EditBox *editOrden;
	gong::EditBox *editCantidad;
	gong::SearchBox *searchArticuloCodigo;
	QPushButton *pushArticuloCodigo;
	gong::LineEdit *editArticuloCodigo;
	gong::LineEdit *editArticuloNombre;
	gong::EditBox *editCoste;
	gong::EditBox *editImporte;
	gong::TextBox *editNotas;
/*>>>>>FRMEDITARTICULOCOMPONENTE_CONTROLS*/
};
/*<<<<<FRMEDITARTICULOCOMPONENTE_POSTAMBLE*/
} // namespace produccion
} // namespace gong
#endif // PRODUCCION_PRODUCCIONFRMEDITARTICULOCOMPONENTE_H
/*>>>>>FRMEDITARTICULOCOMPONENTE_POSTAMBLE*/
