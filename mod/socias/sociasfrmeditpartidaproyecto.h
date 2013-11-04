/*<<<<<COPYLEFT*/
/** @file sociasfrmeditpartidaproyecto.h Fichero de edición de partidas de proyectos
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
/*<<<<<FRMEDITPARTIDAPROYECTO_PREAMBLE*/
#ifndef _SOCIAS_SOCIASFRMEDITPARTIDAPROYECTO_H
#define _SOCIAS_SOCIASFRMEDITPARTIDAPROYECTO_H
/*>>>>>FRMEDITPARTIDAPROYECTO_PREAMBLE*/

/*<<<<<FRMEDITPARTIDAPROYECTO_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
#include "sociasrecpartidaproyecto.h"
/*>>>>>FRMEDITPARTIDAPROYECTO_INCLUDES*/
#include "sociasrecproyecto.h"

namespace gong {
namespace socias {

/*<<<<<FRMEDITPARTIDAPROYECTO_CLASS*/
class FrmEditPartidaProyecto: public FrmEditRecMaster
{
	Q_OBJECT

public:
	FrmEditPartidaProyecto(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecPartidaProyecto *getRecPartidaProyecto() const { return (RecPartidaProyecto *)getRecord(); }
/*>>>>>FRMEDITPARTIDAPROYECTO_CLASS*/
    /*<<<<<FRMEDITPARTIDAPROYECTO_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
	void specialControlKeyPressed(QWidget *sender, char key);
/*>>>>>FRMEDITPARTIDAPROYECTO_VIRTUALS_FROM_FRMEDITREC*/

    /*<<<<<FRMEDITPARTIDAPROYECTO_SCATTERS_AND_SLOTS*/
protected:
	void scatterProyecto();

private slots:
	void pushProyectoCodigo_clicked();

public:
	RecProyecto* getRecProyecto() const
		{ return static_cast<RecPartidaProyecto*>(getRecord())->getRecProyecto(); }
/*>>>>>FRMEDITPARTIDAPROYECTO_SCATTERS_AND_SLOTS*/

    /*<<<<<FRMEDITPARTIDAPROYECTO_CONTROLS*/
protected:
	gong::EditBox *editCodigo;
	gong::EditBox *editDescripcion;
	gong::SearchBox *searchProyectoCodigo;
	QPushButton *pushProyectoCodigo;
	gong::LineEdit *editProyectoCodigo;
	gong::LineEdit *editProyectoDescripcion;
	gong::EditBox *editMadre;
	gong::EditBox *editTipo;
	gong::EditBox *editOrden;
	gong::EditBox *editPresupuesto;
	gong::EditBox *editImporte;
	gong::TextBox *editNotas;
/*>>>>>FRMEDITPARTIDAPROYECTO_CONTROLS*/
};
/*<<<<<FRMEDITPARTIDAPROYECTO_POSTAMBLE*/
} // namespace socias
} // namespace gong
#endif // SOCIAS_SOCIASFRMEDITPARTIDAPROYECTO_H
/*>>>>>FRMEDITPARTIDAPROYECTO_POSTAMBLE*/
