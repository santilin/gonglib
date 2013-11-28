/*<<<<<FRMEDITPROYECTO_PREAMBLE*/
#ifndef _SOCIAS_SOCIASFRMEDITPROYECTO_H
#define _SOCIAS_SOCIASFRMEDITPROYECTO_H
/*>>>>>FRMEDITPROYECTO_PREAMBLE*/
/*<<<<<COPYLEFT*/
/** @file sociasfrmeditproyecto.h Fichero de edición de proyectos
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

/*<<<<<FRMEDITPROYECTO_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
#include "sociasrecproyecto.h"
#include "sociasfrmeditmiembro.h"
#include "sociasfrmeditpartidaproyecto.h"
/*>>>>>FRMEDITPROYECTO_INCLUDES*/

#include <pagosfrmeditremesacobro.h>

namespace gong {
namespace socias {

typedef gong::pagos::RecRemesaCobro RecRemesaCobro;
typedef gong::pagos::FrmEditRemesaCobro FrmEditRemesaCobro;

/*<<<<<FRMEDITPROYECTO_CLASS*/
class FrmEditProyecto: public FrmEditRecMaster
{
	Q_OBJECT

public:
	FrmEditProyecto(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecProyecto *getRecProyecto() const { return (RecProyecto *)getRecord(); }
/*>>>>>FRMEDITPROYECTO_CLASS*/
    /*<<<<<FRMEDITPROYECTO_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
/*>>>>>FRMEDITPROYECTO_VIRTUALS_FROM_FRMEDITREC*/

    /*<<<<<FRMEDITPROYECTO_CONTROLS*/
protected:
	gong::EditBox *editCodigo;
	gong::EditBox *editNombre;
	gong::TextBox *editDescripcion;
	gong::EditBox *editFechaInicio;
	gong::EditBox *editFechaFin;
	gong::ComboBoxInt *comboSociasEstado;
	gong::EditBox *editNumCuotas;
	gong::EditBox *editImporte;
	gong::ComboBoxInt *comboPeriodicidad;
	gong::EditBox *editFormatoNumRecibo;
	gong::TextBox *editNotas;
	FrmEditMiembro *pFrmMiembro;
	FrmEditRemesaCobro *pFrmRemesaCobro;
	FrmEditPartidaProyecto *pFrmPartidaProyecto;
/*>>>>>FRMEDITPROYECTO_CONTROLS*/
    QWidget *pTabRemesas, *pTabMiembros, *pTabPartidas;

};

/*<<<<<FRMEDITPROYECTO_POSTAMBLE*/
} // namespace socias
} // namespace gong
#endif // SOCIAS_SOCIASFRMEDITPROYECTO_H
/*>>>>>FRMEDITPROYECTO_POSTAMBLE*/
