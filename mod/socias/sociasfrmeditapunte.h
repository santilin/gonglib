/*<<<<<FRMEDITAPUNTE_PREAMBLE*/
#ifndef _SOCIAS_SOCIASFRMEDITAPUNTE_H
#define _SOCIAS_SOCIASFRMEDITAPUNTE_H
/*>>>>>FRMEDITAPUNTE_PREAMBLE*/
/*<<<<<COPYLEFT*/
/** @file sociasfrmeditapunte.h Fichero de edición de apuntes
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

#include "config.h"
#ifdef HAVE_CONTABMODULE
#include <qobject.h>

/*<<<<<FRMEDITAPUNTE_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecdetail.h>
/*>>>>>FRMEDITAPUNTE_INCLUDES*/
#include <contabfrmeditapunte.h>
#include "sociasrecapunte.h"


namespace gong {
namespace socias {

typedef contab::RecCuenta RecContrapartida;
class RecPartidaProyecto;

/*<<<<<FRMEDITAPUNTE_CLASS*/
class FrmEditApunte: public contab::FrmEditApunte
{
	Q_OBJECT

public:
	FrmEditApunte(FrmEditRecMaster *frmmaster, int ndetail,
		dbRecord *detail, const Xtring &dettablename, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
/*>>>>>FRMEDITAPUNTE_CLASS*/
    /*<<<<<FRMEDITAPUNTE_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
	void specialControlKeyPressed(QWidget *sender, char key);
/*>>>>>FRMEDITAPUNTE_VIRTUALS_FROM_FRMEDITREC*/
    /*<<<<<FRMEDITAPUNTE_SCATTERS_AND_SLOTS*/
protected:
	void scatterPartidaProyecto();

private slots:
	void pushPartidaProyectoCodigo_clicked();

public:
	RecPartidaProyecto* getRecPartidaProyecto() const
		{ return static_cast<RecApunte*>(getRecord())->getRecPartidaProyecto(); }
/*>>>>>FRMEDITAPUNTE_SCATTERS_AND_SLOTS*/

    /*<<<<<FRMEDITAPUNTE_CONTROLS*/
protected:
	gong::SearchBox *searchPartidaProyectoCodigo;
	QPushButton *pushPartidaProyectoCodigo;
	gong::LineEdit *editPartidaProyectoCodigo;
	gong::LineEdit *editPartidaProyectoDescripcion;
/*>>>>>FRMEDITAPUNTE_CONTROLS*/
};

#endif // ifdef HAVE_CONTABMODULE

/*<<<<<FRMEDITAPUNTE_POSTAMBLE*/
} // namespace socias
} // namespace gong
#endif // SOCIAS_SOCIASFRMEDITAPUNTE_H
/*>>>>>FRMEDITAPUNTE_POSTAMBLE*/

