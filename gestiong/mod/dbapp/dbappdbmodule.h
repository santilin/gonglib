#ifndef _DBAPP_DBMODULE_H
#define _DBAPP_DBMODULE_H

/** @file dbappdbmodule.h A module for the application
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include <gongguiapplication.h>
#include "dbappfrmeditrec.h"
#include "dbappfrmeditrecdetail.h"

class QAction;

namespace gong {

class dbApplication;
class MainWindow;

struct dbModuleSetting {
    enum Type { None, Bool, Int, Long, Double, String, Text, Date, Time, DateTime, Money, Image,
                StringList, FileName, DirName, Password
              };
    Type type;
    const char *key;
    const char *description;
    const char *defaultvalue;
    const char *values;
};

class dbModule
{
public:
    dbModule(const char *unixname);
    virtual ~dbModule() {};

    // module's public data
    const Xtring &getUnixName() const {
        return mUnixName;
    }
    const Xtring &getDescription() const {
        return mDescription;
    }
    uint getVersion() const {
        return mVersion;
    }
    const XtringList &getMasterTables() const {
        return mMasterTables;
    }
    const XtringList &getDetailTables() const {
        return mDetailTables;
    }
    const XtringList &getExportTables() const {
        return mExportTables;
    }
    const XtringList &getImportTables() const {
        return mImportTables;
    }
    const XtringList &getModuleRequires() const {
        return mModuleRequires;
    }
    bool isEnabled() const {
        return mEnabled;
    }
    void setEnabled( bool enabled ) {
        mEnabled = enabled;
    }
    // module's interface
    virtual void readSettings();
    virtual void afterLoad() {};
    virtual bool initMainWindow(MainWindow *win) = 0;
    virtual bool login(FrmLogin *frmlogin, const Xtring &version,
                       Xtring &addTitle, bool startingapp = true) {
        return true;
    }
    virtual bool initDatabase(dbDefinition *dbdef) {
        pMainDatabase = dbdef;
        return true;
    }
    virtual dbRecord *createRecord(
        const Xtring &tablename, dbRecordID recid=0, dbUser *user=0) {
        return 0;
    }
    virtual void afterCreateRecord( dbRecord *rec ) {}
    virtual FrmEditRec *createEditForm(FrmEditRec *parentfrm, dbRecord *rec, dbRecordDataModel *dm = 0,
                                       DataTable::EditMode editmode = DataTable::defaulteditmode,
                                       dbApplication::EditFlags editflags = dbApplication::editNone,
                                       QWidget *parent = 0, const char* name = 0,
                                       WidgetFlags fl = WidgetFlags(0) ) {
        return 0;    // From GuiApplication
    }
    virtual FrmEditRecDetail *createEditDetailForm(
        FrmEditRecMaster *frmmaster, int ndetalle,
        dbRecord *rec, const Xtring &dettablename, dbRecordDataModel *dm = 0,
        DataTable::EditMode editmode = DataTable::defaulteditmode,
        dbApplication::EditFlags editflags = dbApplication::editNone,
        QWidget *parent = 0, const char* name = 0,
        WidgetFlags fl = WidgetFlags(0) ) {
        return 0;
    }
    virtual void afterCreateEditForm( FrmEditRec *frm, dbRecord *rec ) {};
    virtual Variant getGlobalProperty(const Xtring &propertyname) const;

    // helpers
    dbDefinition *getDatabase() const {
        return pMainDatabase;
    }
    MainWindow *getMainWindow() const {
        return pMainWindow;
    }
    dbApplication *getApp() const {
        return pApplication;
    }
    dbConnection *getConnection() const;
    QAction *createActionForTable(const Xtring &tablename, const Xtring &shortcut = Xtring::null);
    const Xtring &getModuleGlobalDataDir() const {
        return mModuleGlobalDataDir;
    }
    const Xtring &getModuleLocalDataDir() const {
        return mModuleLocalDataDir;
    }
    virtual Xtring getMigrationSQL( uint old_version ) const {
        return Xtring::null;
    }
    const dbModuleSetting *getModuleSettings() const {
        return pModuleSettings;
    }
    Variant getModuleSetting( const Xtring &settingname ) const;

protected:
    dbApplication *pApplication;
    MainWindow *pMainWindow;
    dbDefinition *pMainDatabase;
    dbConnection *pConnection;
    Xtring mModuleGlobalDataDir, mModuleLocalDataDir;
    uint mVersion;
    Xtring mUnixName, mDescription;
    XtringList mMasterTables, mDetailTables, mExportTables, mImportTables;
    XtringList mModuleRequires;
    dbModuleSetting *pModuleSettings;
    bool mEnabled;
};

} // Namespace

#endif
