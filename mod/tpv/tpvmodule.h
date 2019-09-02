/*<<<<<COPYLEFT*/
/** @file tpvmodule.h Module Tpv
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
/*<<<<<TPVMODULE_PREAMBLE*/
#ifndef _TPV_TPVMODULE_H
#define _TPV_TPVMODULE_H
/*>>>>>TPVMODULE_PREAMBLE*/

/*<<<<<TPVMODULE_INCLUDES*/
#include "config.h"
#include <qobject.h>
#include <dbappdbmodule.h>
#include "tpvmastertable.h"

class QAction;
class QMenu;
#ifdef HAVE_FACTUMODULE
#include <factumodule.h>
#else
#error El módulo 'tpv::TpvModule' requiere el módulo 'Factu'
#endif
/*>>>>>TPVMODULE_INCLUDES*/

namespace gong {
namespace tpv {

const int TPV_GALLERY_PADDING_RIGHT = 5;
const int TPV_GALLERY_PADDING_BOTTOM = 15;

class FrmEditFamiliaBehavior;
class FrmEditArticuloBehavior;

/*<<<<<TPVMODULE_CLASS_DEFINITION*/
class TpvModule: public QObject, public dbModule
{
	Q_OBJECT // Si da error de funciones virtuales no encontradas, elimina esto
public:
	TpvModule(int argc, char *argv[]);
	virtual ~TpvModule();
	virtual bool initMainWindow(class MainWindow *win);
	virtual bool initDatabase(dbDefinition *dbdef);
	virtual bool login(const Xtring &version, Xtring &addTitle, bool startingapp = true);
	virtual dbRecord *createRecord(const Xtring &tablename, dbRecordID recid=0, dbUser *user=0);
	virtual FrmEditRec *createEditForm(FrmEditRec *parentfrm, dbRecord *rec, dbRecordDataModel *dm = 0,
		FrmEditRec::EditMode editmode = DataTable::defaulteditmode,
		dbApplication::EditFlags editflags = dbApplication::editNone,
		QWidget *parent = 0, const char* name = 0,
		WidgetFlags fl = WidgetFlags(0) );
	virtual FrmEditRecDetail *createEditDetailForm(
		FrmEditRecMaster *frmmaster, int ndetail,
		dbRecord *rec, const Xtring &dettablename, dbRecordDataModel *dm = 0,
		FrmEditRec::EditMode editmode = DataTable::defaulteditmode,
		dbApplication::EditFlags editflags = dbApplication::editNone,
		QWidget *parent = 0, const char* name = 0,
		WidgetFlags fl = WidgetFlags(0) );
/*>>>>>TPVMODULE_CLASS_DEFINITION*/
	void readSettings(); // from dbModule
	void afterCreateEditForm( FrmEditRec *frm, dbRecord *rec ); // from dbModule
	void printZ( dbRecordID sesiontpv_id );
	uint getNumeroTPV() { return mNumeroTPV; }
	void setNumeroTPV( uint ntpv ) { mNumeroTPV = ntpv; }
    class FrmTpvScreen *initTPV();

/*<<<<<TPVMODULE_RECORD_DEFINITIONS*/
public:
	tpv::MasterTable *getFicSesionTPV() const { return pFicSesionTPV; }

private:
	tpv::MasterTable *pFicSesionTPV;
/*>>>>>TPVMODULE_RECORD_DEFINITIONS*/
/*<<<<<TPVMODULE_MENU_DEFINITIONS*/
private slots:
	void slotMenuTPVSesionTPV();
protected:
	QAction *pMenuTPVSesionTPV;
/*>>>>>TPVMODULE_MENU_DEFINITIONS*/
	QMenu *pMenuTPV;
	QAction *pMenuTPVIniciaTpv, *pMenuTPVArqueoCaja, *pMenuTPVCerrarSesion,
		*pMenuTPVZActual;
/*<<<<<TPVMODULE_REQUIRES*/
public:
	factu::FactuModule *getFactuModule() const { return pFactuModule; }
private:
	factu::FactuModule *pFactuModule;
/*>>>>>TPVMODULE_REQUIRES*/

protected slots:
	void slotMenuTPVIniciaTpv();
	void slotMenuTPVArqueoCaja();
	void slotMenuTPVCerrarSesion();
	void slotMenuTPVZActual();
private:
	uint mNumeroTPV;
};

extern TpvModule *ModuleInstance;


/*<<<<<TPVMODULE_POSTAMBLE*/
} // namespace tpv
} // namespace gong
#endif // TPV_TPVMODULE_H
/*>>>>>TPVMODULE_POSTAMBLE*/

