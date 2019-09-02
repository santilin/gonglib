/*<<<<<COPYLEFT*/
/** @file editorialfrmeditlibro.h Fichero de edición de editorial
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
/*<<<<<FRMEDITLIBRO_PREAMBLE*/
#ifndef _EDITORIAL_EDITORIALFRMEDITLIBRO_H
#define _EDITORIAL_EDITORIALFRMEDITLIBRO_H
/*>>>>>FRMEDITLIBRO_PREAMBLE*/

/*<<<<<FRMEDITLIBRO_INCLUDES*/
#include <gongfrmbase.h>
#include "editorialreclibro.h"
/*>>>>>FRMEDITLIBRO_INCLUDES*/
#include <factufrmeditarticulo.h>

#include "editorialreceditorial.h"

namespace gong {
namespace editorial {

/*<<<<<FRMEDITLIBRO_CLASS*/
class FrmEditLibro: public factu::FrmEditArticulo
{
	Q_OBJECT

public:
	FrmEditLibro(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecLibro *getRecLibro() const { return (RecLibro *)getRecord(); }
/*>>>>>FRMEDITLIBRO_CLASS*/

/*<<<<<FRMEDITLIBRO_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
	void specialControlKeyPressed(QWidget *sender, char key);
/*>>>>>FRMEDITLIBRO_VIRTUALS_FROM_FRMEDITREC*/
/*<<<<<FRMEDITLIBRO_CONTROLS*/
protected:
	gong::EditBox *editISBN13;
	gong::EditBox *editISBN10;
	gong::EditBox *editDepositoLegal;
	gong::EditBox *editTitulo;
	gong::TextBox *editSubtitulo;
	gong::TextBox *editAutoras;
	gong::SearchBox *searchEditorialCodigo;
	QPushButton *pushEditorialCodigo;
	gong::LineEdit *editEditorialCodigo;
	gong::LineEdit *editEditorialRazonSocial;
	gong::EditBox *editAno;
	gong::EditBox *editEdicion;
	gong::EditBox *editFechaEdicion;
	gong::EditBox *editPaginas;
	gong::EditBox *editLengua;
	gong::EditBox *editColeccion;
	gong::EditBox *editCDU;
	gong::EditBox *editLenguasTraduccion;
	gong::EditBox *editTituloOriginal;
	gong::EditBox *editImpresor;
	gong::EditBox *editFechaImpresion;
	gong::EditBox *editLugarImpresion;
	gong::EditBox *editEncuadernacion;
	gong::TextBox *editCreditos;
/*>>>>>FRMEDITLIBRO_CONTROLS*/
	QPushButton *pushSearchISBN;

/*<<<<<FRMEDITLIBRO_SCATTERS_AND_SLOTS*/
protected:
	void scatterEditorial();

private slots:
	void pushEditorialCodigo_clicked();

public:
	RecEditorial* getRecEditorial() const
		{ return static_cast<RecLibro*>(getRecord())->getRecEditorial(); }
/*>>>>>FRMEDITLIBRO_SCATTERS_AND_SLOTS*/
protected slots:
	void pushSearchISBN_clicked();

protected:
	bool esLibro() const { return mEsLibro; }
	void updateControls();
	bool mEsLibro;
private:
	void guiInit();
	int mTabLibroPos;
};
/*<<<<<FRMEDITLIBRO_POSTAMBLE*/
} // namespace editorial
} // namespace gong
#endif // EDITORIAL_EDITORIALFRMEDITLIBRO_H
/*>>>>>FRMEDITLIBRO_POSTAMBLE*/
