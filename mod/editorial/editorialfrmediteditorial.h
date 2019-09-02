/*<<<<<COPYLEFT*/
/** @file editorialfrmediteditorial.h Fichero de edición de editoriales
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
/*<<<<<FRMEDITEDITORIAL_PREAMBLE*/
#ifndef _EDITORIAL_EDITORIALFRMEDITEDITORIAL_H
#define _EDITORIAL_EDITORIALFRMEDITEDITORIAL_H
/*>>>>>FRMEDITEDITORIAL_PREAMBLE*/
/*<<<<<FRMEDITEDITORIAL_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
#include "editorialreceditorial.h"
/*>>>>>FRMEDITEDITORIAL_INCLUDES*/
#include <contactosreccontacto.h>

namespace gong {
namespace editorial {
	
/*<<<<<FRMEDITEDITORIAL_CLASS*/
class FrmEditEditorial: public FrmEditRecMaster
{
	Q_OBJECT

public:
	FrmEditEditorial(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecEditorial *getRecEditorial() const { return (RecEditorial *)getRecord(); }
/*>>>>>FRMEDITEDITORIAL_CLASS*/

/*<<<<<FRMEDITEDITORIAL_SCATTERS_AND_SLOTS*/
protected:
	void scatterContacto();

private slots:
	void pushContactoCIF_clicked();

public:
	contactos::RecContacto* getRecContacto() const
		{ return static_cast<RecEditorial*>(getRecord())->getRecContacto(); }
/*>>>>>FRMEDITEDITORIAL_SCATTERS_AND_SLOTS*/

/*<<<<<FRMEDITEDITORIAL_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
	void specialControlKeyPressed(QWidget *sender, char key);
/*>>>>>FRMEDITEDITORIAL_VIRTUALS_FROM_FRMEDITREC*/
/*<<<<<FRMEDITEDITORIAL_CONTROLS*/
protected:
	gong::EditBox *editCodigo;
	gong::SearchBox *searchContactoCIF;
	QPushButton *pushContactoCIF;
	gong::LineEdit *editContactoCIF;
	gong::LineEdit *editContactoNombre;
	gong::EditBox *editRazonSocial;
	gong::TextBox *editNotas;
/*>>>>>FRMEDITEDITORIAL_CONTROLS*/
};

/*<<<<<FRMEDITEDITORIAL_POSTAMBLE*/
} // namespace editorial
} // namespace gong
#endif // EDITORIAL_EDITORIALFRMEDITEDITORIAL_H
/*>>>>>FRMEDITEDITORIAL_POSTAMBLE*/

