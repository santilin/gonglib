#ifndef GONG_DBAPP_FRMDATABASETOOLS_H
#define GONG_DBAPP_FRMDATABASETOOLS_H

/** @file dbappfrmdatabasetools.h A form for database utilities
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include "dbappfrmcustom.h"

namespace gong {

class FrmDatabaseTools: public FrmCustom
{
public:
    FrmDatabaseTools( QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
    void upgradeDatabase();
    void checkDatabase();
    void backupDatabase( bool automatic );
    void restoreDatabase();
    void checkDDD();
    void lockDatabase();
    void unlockDatabase();
    static bool dump(const Xtring &dumpfname, const Xtring &database,
                     const Xtring &user, const Xtring &host,
                     const Xtring &password, const XtringList &tables,
                     Xtring &errors );
    static bool load(const Xtring &dumpfname, const Xtring &database,
                     const Xtring &user, const Xtring &host,
                     const Xtring &password, Xtring &errors );
protected:
    virtual void validate_input( QWidget *sender, bool *isvalid ); // from FrmCustom
    Xtring checkDDDKey( const Xtring &key, const Xtring &missingvalue = Xtring::null );
    Xtring createDirAndSelectFile( const Xtring &dir, bool automatic );
    QVBoxLayout *pFrmDatabaseToolsLayout;
    QHBoxLayout *pMainLayout;
    PushButton *pButtonUpgrade, *pButtonCheck, *pButtonBackup, *pButtonRestore,
               *pButtonCheckDDD, *pButtonLockDatabase, *pButtonUnlockDatabase;
};

} // namespace gong

#endif // FRMDATABASETOOLS_H
