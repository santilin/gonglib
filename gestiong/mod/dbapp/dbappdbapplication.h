#ifndef _DBAPP_DBAPPLICATION_H
#define _DBAPP_DBAPPLICATION_H

/** @file dbappdbapplication.h El objeto global aplicación
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include "config.h"
#include <gonggettext.h>
#include <gongdbdefinition.h>
#include <gongsettings.h>
#include <gongguiapplication.h>
#include <gongdatatable.h>
#include <gongdbrecordtimestampbehavior.h> // It is used frecuently


namespace gong {

class dbModule;
class FrmEditRec;
class FrmEditRecMaster;
class FrmEditRecDetail;
class dbRecordPermissions;
class dbRecordDataModel;
class FrmLogin;
class dbMasterTable;
class RecMetaDBData;
class LineEdit;

class dbApplication: public GuiApplication
{
public:
    dbApplication( const char *dbversion, const char *datadir,
                   const char *packagename, const char *packageversion,
                   int &argc, char **argv );
    virtual ~dbApplication();
    virtual void readSettings();
    virtual bool initMainWindow(); // from GuiApplication

    bool login(const Xtring &version, bool startingapp = true, bool autologin = false);
    bool initDatabases();
    /// Creates a record from a table name
    dbRecord *createRecord(const Xtring &tablename, dbRecordID recid=0, dbUser *user=0);
    /// Creates a record from another record
    dbRecord *createRecord(const dbRecord *record);

    bool isReadOnly() const {
        return mReadOnly;
    }
    void setReadOnly(bool ro);
    bool preventOperationIf( bool cond, const Xtring &operation, const Xtring &reason );
    dbRecordPermissions getRecordPermissions( dbRecord *rec, dbUser *user );

    // The main application database
    dbConnection *getConnection() const {
        return pConnection;
    }
    dbDefinition *getDatabase() const {
        return pDatabase;
    }
    Xtring getdbVersion() const {
        return mdbVersion;
    }
    Xtring upgradeDatabase( dbDefinition *programdb, const Xtring &sqldbname,
                            bool purging = false );
    void setDatabaseChanged(bool changed = true) {
        mDatabaseChanged = changed;
    }
    bool isDatabaseChanged() const {
        return mDatabaseChanged;
    }
    bool manageDbError( const Xtring &context, const dbError &e );
    const Xtring &getDbUser() const {
        return mDbUser;
    }
    const Xtring &getDbUserPassword() const {
        return mDbUserPassword;
    }
    const Xtring &getDbHost() const {
        return mDbHost;
    }
    bool isRealSQLUser() const {
        return mRealSQLUser;
    }
    void setRealSQLUser(bool realsqluser) {
        mRealSQLUser = realsqluser;
    }
    dbMasterTable *pFicMetaDBData, *pFicGlobalConfig;
    RecMetaDBData *getRecMetaDBData() const;

    // Modules
    bool addModule(dbModule *module);
    bool removeModule(const Xtring &name) {
        return mModules.erase(name);
    }
    dbModule *findModule(const Xtring &name) const;
    const Dictionary<dbModule *> &getModules() const {
        return mModules;
    }

    // Settings
    Settings *getMachineSettings() const {
        return pMachineSettings;
    }
    Settings *getUserLocalSettings() const {
        return pUserLocalSettings;
    }
    Settings *getGlobalSettings() const {
        return pGlobalSettings;
    }
    Settings *getUserGlobalSettings() const {
        return pUserGlobalSettings;
    }
    Variant getAppSetting(const Xtring &settingname, const Variant &defaultvalue = Variant()) const;
    Xtring getAppSettingConcat(const Xtring &settingname, const Xtring &concat,
                               bool fromglobaltolocal = true, const Xtring &defaultvalue = Xtring::null) const;
    Variant getMachineSetting(const Xtring &settingname, const Variant &defaultvalue = Variant()) const;
    Variant getUserLocalSetting(const Xtring &settingname, const Variant &defaultvalue = Variant()) const;
    Variant getUserGlobalSetting(const Xtring &settingname, const Variant &defaultvalue = Variant()) const;
    Variant getGlobalSetting(const Xtring &settingname, const Variant &defaultvalue = Variant()) const;
    bool concatMachineSetting(const Xtring &settingname, const Xtring &val, const Xtring &sep);
    bool concatUserLocalSetting(const Xtring &settingname, const Xtring &val, const Xtring &sep);
    void setMachineSetting(const Xtring &settingname, const Variant &val) {
        pMachineSettings->setValue( settingname, val);
    }
    void setUserLocalSetting(const Xtring &settingname, const Variant &val) {
        pUserLocalSettings->setValue( settingname, val);
    }
    void setGlobalSetting(const Xtring &settingname, const Variant &val) {
        pGlobalSettings->setValue( settingname, val);
    }
    void setUserGlobalSetting(const Xtring &settingname, const Variant &val) {
        pUserGlobalSettings->setValue( settingname, val);
    }
    int getSettingsValues(const Xtring &key_pattern, Dictionary<Variant> &result);
    bool updateSetting(const Xtring &settingname, const Variant &val);
    bool removeSetting(const Xtring &settingname, bool global = false);
    void writeSettings();
    void setDDDFromConfig(dbDefinition *pdb);
    void setStylesFromConfig(dbDefinition *pdb);
    int setViewsFromConfig(dbDefinition *pdb = 0);
    bool readMachineSettings( const Xtring &filename);
    bool readUserLocalSettings( const Xtring &filename);
    bool readDatabaseSettings(const Xtring &tablename, const Xtring &filter);

public:	// Edit forms
    enum EditFlags {
        editNone       = 0,
        simpleEdition  = 1<<0,
        cantMoveRecord = 1<<1,
        cantInsert     = 1<<2,
        cantUpdate     = 1<<3,
        cantDelete     = 1<<4,
        cantSelect     = 1<<5,
        cantPrint      = 1<<6,
        cantReorder    = 1<<7,
        cantFilter     = 1<<8,
        deleteRecord   = 1<<9,
        deleteDataModel = 1<<10,
        embedded	   = 1<<11,
        recordListEdit = 1<<12,
        tableEditEvent	= 1<<13,
        editContinuous = 1<<14,
        readOnly = 1 << 15,
        onlyBrowse = 1 << 16
    };
    /** Creates a dialog to edit the contents of the record */
    FrmEditRec *createEditForm(FrmEditRec *parentfrm, dbRecord *rec, dbRecordDataModel *dm = 0,
                               DataTable::EditMode editmode = DataTable::defaulteditmode,
                               dbApplication::EditFlags editflags = dbApplication::editNone,
                               QWidget *parentWidget = 0, const char* name = 0, WidgetFlags fl = 0 );
    FrmEditRecDetail *createEditDetailForm(
        FrmEditRecMaster *frmmaster, int ndetalle,
        dbRecord *rec, const Xtring &dettablename, dbRecordDataModel *dm = 0,
        DataTable::EditMode editmode = DataTable::defaulteditmode,
        dbApplication::EditFlags editflags = dbApplication::editNone,
        QWidget *parentWidget = 0, const char* name = 0, WidgetFlags fl = 0 );
    /** Shows a dialog to fully edit the contents of the record */
    bool editRecord(FrmEditRec *parentfrm, dbRecord *rec, dbRecordDataModel *dm = 0,
                    DataTable::EditMode editmode = DataTable::defaulteditmode,
                    dbApplication::EditFlags editflags = dbApplication::editNone,
                    QWidget *parentWidget = 0, const char* name = 0, WidgetFlags fl = 0 );
    /** Shows a dialog to browse the contents of the record */
    FrmEditRec *browse(FrmEditRec *parentfrm, dbRecord *rec, dbRecordDataModel *dm = 0,
                       DataTable::EditMode editmode = DataTable::defaulteditmode,
                       dbApplication::EditFlags editflags = dbApplication::editNone,
                       QWidget *parentWidget = 0, const char* name = 0, WidgetFlags fl = 0 );
    /** Shows a modal dialog to choose from a list of records */
    dbRecordID choose(FrmEditRec *parentfrm, dbRecord *rec, dbRecordDataModel *dm = 0,
                      dbApplication::EditFlags editflags = dbApplication::editNone,
                      QWidget *parentWidget = 0, const char* name = 0, WidgetFlags fl = 0 );
    /** Shows a modal dialog to choose a list of records from a list of records */
    bool chooseMulti(List<dbRecordID>& v,
                     FrmEditRec *parentfrm, dbRecord *rec, dbRecordDataModel *dm = 0,
                     dbApplication::EditFlags editflags = dbApplication::editNone,
                     QWidget *parentWidget = 0, const char* name = 0, WidgetFlags fl = 0 );

    enum SeekCodeFlags { SeekCodeNone = 0x00,  InsertIfNotFound = 0x01,
                         FindCodeInDesc = 0x02, SeekCodeMultiple = 0x04, DontShowBrowse = 0x08,
                         AllowNotFound = 0x10, SeekCodeReadonly = 0x20, AskIfFoundOne = 0x40
                       };
    dbRecordID seekCode(dbRecord *rec, QWidget *owner,
                        const Xtring &fldcod, const Xtring &foundcod,
                        const Xtring &flddesc = Xtring::null, const Xtring &founddesc = Xtring::null,
                        const Xtring &cond = Xtring::null, SeekCodeFlags flags = SeekCodeNone,
                        LineEdit *editAlt = 0, const Xtring &fldaltname = Xtring::null );
    dbRecordID seekCode(dbRecord *rec, QWidget *owner,
                        const Xtring &fldcod, int foundcod,
                        const Xtring &flddesc = Xtring::null, const Xtring &founddesc = Xtring::null,
                        const Xtring &cond = Xtring::null, SeekCodeFlags flags = SeekCodeNone,
                        LineEdit *editAlt = 0, const Xtring &fldaltname = Xtring::null );
    Xtring getReportsPath( bool reversed );
    void addReportsPath( bool local, const Xtring &path );
    static Xtring codeNotFound() {
        return sCodeNotFound;
    }
    static void setCodeNotFound( const Xtring &code ) {
        sCodeNotFound = code;
    }
    /* The last description typed by the user that could not be found */
    static Xtring descNotFound() {
        return sDescNotFound;
    }
    static void setDescNotFound( const Xtring &desc ) {
        sDescNotFound = desc;
    }
    static List<dbRecordID> &getSeekCodeRecordIDs() {
        return sSeekCodeRecordIDs;
    }
    static dbRecordTimestampBehavior *getRecordTimestampBehavior() {
        return sTimestampBehavior;
    }
    static const Xtring getTableDescSingular(const Xtring &tablename, const Xtring &demostrativo = Xtring::null );
    static const Xtring getTableDescPlural(const Xtring &tablename, const Xtring &demostrativo = Xtring::null );
    static Xtring getClipBoardContent() {
        return sClipBoardContent;
    }
    static void setClipBoardContent(const Xtring &content) {
        sClipBoardContent = content;
    }
    static dbRecordID getAnotherRecordID() {
        return sAnotherRecordID;
    }
    static void setAnotherRecordID(const dbRecordID &another_id) {
        sAnotherRecordID = another_id;
    }
    static XtringList &getMasterTablesList() {
        return mMasterTables;
    }
    static XtringList &getDetailTablesList() {
        return mDetailTables;
    }
    static long version2Long( const Xtring &version );

protected:
    dbConnection *pConnection;
    dbDefinition *pDatabase;
    FrmLogin *pFrmLogin;
    const char *mdbVersion;
    Dictionary<dbModule *> mModules;
    Settings *pMachineSettings, *pUserLocalSettings, *pGlobalSettings, *pUserGlobalSettings;
    bool mDatabaseChanged;
    bool mReadOnly;
    Xtring mDbUser, mDbHost, mDbUserPassword, mReportsLocalPath, mReportsGlobalPath;
    static XtringList mMasterTables, mDetailTables;
    static Xtring sCodeNotFound, sDescNotFound, sClipBoardContent;
    static List<dbRecordID> sSeekCodeRecordIDs;
    static dbRecordID sAnotherRecordID;
    static Date sWorkingDate;
    static dbRecordTimestampBehavior *sTimestampBehavior;
private:
    bool mRealSQLUser;
    RecMetaDBData *pRecMetaDBData;
};

extern dbApplication *DBAPP;

} // namespace gong
#endif // _DBAPP_DBAPPLICATION_H
