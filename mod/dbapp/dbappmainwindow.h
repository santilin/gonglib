#ifndef _GONG_DBAPP_MAINWINDOW_H
#define _GONG_DBAPP_MAINWINDOW_H

/** @file dbappmainwindow.h The application main window
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include <gongfrmbase.h>
#include <gongguimainwindow.h>

namespace gong {

class MainWindow : public GuiMainWindow
{
    Q_OBJECT
public:
    MainWindow() : GuiMainWindow() {}
    virtual ~MainWindow() {}

    virtual QMdiSubWindow *createClient(QWidget *w, bool force = false );
    virtual void initGUI(); // from GuiMainWindow
    virtual void finishGUI(); // from GuiMainWindow
    bool editRecMaestroModal(const Xtring &tablename, dbRecord *record = 0,
                             bool deleterecord = false, FrmBase *parent = 0 );

public slots:
    void slotMenuEditRecMaestro(const Xtring &tablename, dbRecord *record = 0, bool deleterecord = false );
    void slotMenuSystemLogin();
    void slotMenuSystemReports();
    void slotMenuSystemDatabaseTools();
    void slotMenuSystemIntegrity();
    void slotMenuSystemConfigEditor();
    void slotMenuSystemModuleLocalUserSettingsEditor();
    void slotMenuSystemModuleGlobalSettingsEditor();
    void slotMenuSystemImport();
    void slotMenuSystemExport();
    void slotMenuWindowSaveGeometry();
    void slotMenuWindowRemoveGeometry();
    void slotMenuOtherTables();
    virtual void slotMenuHelpAbout(); // from GuiMainWindow

protected:
    QAction *pMenuSystemLogin, *pMenuSystemReports, *pMenuSystemDatabaseTools,
            *pMenuSystemIntegrity, *pMenuSystemConfigEditor,
            *pMenuSystemImport, *pMenuSystemExport, *pMenuOtherTables,
            *pMenuWindowSaveGeometry, *pMenuWindowRemoveGeometry;
    QMenu *pMenuSystemConfig;
    QAction *pMenuSystemModuleLocalUserSettingsEditor,
            *pMenuSystemModuleGlobalSettingsEditor;
};

};

#endif // _GONG_DBAPP_MAINWINDOW_H
