/*<<<<<COPYLEFT*/
/** @file gestiongmodule.h
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */
/*>>>>>COPYLEFT*/
/*<<<<<GESTIONGMODULE_PREAMBLE*/
#ifndef _GESTIONG_GESTIONGMODULE_H
#define _GESTIONG_GESTIONGMODULE_H
/*>>>>>GESTIONGMODULE_PREAMBLE*/

/*<<<<<GESTIONGMODULE_INCLUDES*/
#include "config.h"
#include <qobject.h>
#include <dbappdbmodule.h>
#include "gestiongmastertable.h"

class QAction;
class QMenu;
/*>>>>>GESTIONGMODULE_INCLUDES*/

namespace gong {
namespace gestiong {

/*<<<<<GESTIONGMODULE_CLASS_DEFINITION*/
class GestiongModule: public QObject, public dbModule
{
    Q_OBJECT
public:
    GestiongModule();
    virtual ~GestiongModule();
    virtual bool initMainWindow(class MainWindow *win);
    virtual bool initDatabase(dbDefinition *dbdef);
    virtual bool login(FrmLogin *frmlogin, const Xtring &version, Xtring &addTitle, bool startingapp = true);
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
    /*>>>>>GESTIONGMODULE_CLASS_DEFINITION*/
};

/*<<<<<GESTIONGMODULE_POSTAMBLE*/
} // namespace gestiong
} // namespace gong
#endif // GESTIONG_GESTIONGMODULE_H
/*>>>>>GESTIONGMODULE_POSTAMBLE*/

