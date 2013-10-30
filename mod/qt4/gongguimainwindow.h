#ifndef _GONG_GUIMAINWINDOW_H
#define _GONG_GUIMAINWINDOW_H

/** @file gongguimainwindow.h Qt based MainWindow
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include "gongfrmbase.h"
#include <QMainWindow>

class QMdiArea;
class QMdiSubWindow;
class QActionGroup;
class QSignalMapper;

namespace gong {

class GuiMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    GuiMainWindow();
    virtual ~GuiMainWindow();
    virtual void initGUI();
    virtual void finishGUI();

    virtual QMdiSubWindow *createClient(QWidget *w, bool force = false );
    QMenu *findMenu(const Xtring &name) const;
    QAction *findAction( const Xtring &name) const;
    QWidget *findWindow(const Xtring &name) const;
    QMdiSubWindow *findClient(const QWidget *w) const;
    bool deleteClient(QWidget *frm);
    int getNumClients();
    QWidget *getViewport() const;
    bool amIClosing() const {
        return mImClosing;
    }

    int compressKey(const QKeyEvent *e) const;
    int refreshByName(const Xtring &sendingName, const Xtring &name);
    QMenu *getMenuSystem() const {
        return pMenuSystem;
    }

protected:
    virtual void closeEvent(QCloseEvent *e);
    void removeUnusedSubWindows();

private slots:
    void slotEditUndo();
    void slotEditCut();
    void slotEditCopy();
    void slotEditPaste();
    void slotViewStatusBar(bool);
    void slotMenuWindowRefresh();
    void slotUpdateMenus(QMdiSubWindow*);
    void slotUpdateMenuWindow();
    void slotSetActiveSubWindow(QWidget *window);
    void slotMenuSystemQuit();
    virtual void slotMenuHelpAbout();
    void slotWindowStateChanged(Qt::WindowStates oldstate,Qt::WindowStates newstate);

protected:
    void initActions();
    void initStatusBar();
    QWidget *activeClient() const;

    QMenu *pMenuSystem;
    QAction *pMenuSystemQuit;
    QMenu* pMenuEdit;
    QAction *pMenuEditUndo;
    QAction *pMenuEditCut;
    QAction *pMenuEditCopy;
    QAction *pMenuEditPaste;
    QMenu* pMenuWindow;
    QAction *pMenuWindowStatusBar;
    QActionGroup *windowAction;
    QAction *pMenuWindowNext;
    QAction *pMenuWindowPrev;
    QAction *pMenuWindowClose;
    QAction *pMenuWindowCloseAll;
    QAction *pMenuWindowRefresh;
    QAction *pMenuWindowTile;
    QAction *pMenuWindowCascade;
    QAction *pMenuWindowSeparator;

    QMenu* pMenuHelp;
    QAction *pMenuHelpAbout;

    QMdiArea *pWorkspace;
    QSignalMapper *pWindowMapper;
    bool mImClosing;
};

};

#endif // GONGGUIMAINWINDOW_H
