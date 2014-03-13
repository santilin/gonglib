#ifndef _GONG_GONGGUIAPPLICATION_H
#define _GONG_GONGGUIAPPLICATION_H

/** @file gongguiapplication.h Qt based Application
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include <QApplication>
#include <QTranslator>
#include <gonglibrary.h>
#include <gongosdwidget.h>
#include "gongguinamespace.h"

namespace gong {

class GuiMainWindow;

class GuiApplication: public QApplication, public GongLibrary
{
public:
    GuiApplication( const char *datadir, const char *packagename,
                    const char *packcageversion, int &argc, char **argv );
    virtual ~GuiApplication();
    virtual void init(); // from GongLibrary

    /* GUI related stuff */
    GuiMainWindow *getMainWindow() const {
        return pMainWindow;
    }
    virtual bool initMainWindow();
    static void sendKeyEvent( QWidget *to, int key, int ascii, int state );
    void waitCursor(bool on=true);
    void resetCursor();
    void setTitle( const Xtring &newtitle = Xtring::null);
    Xtring getTitle() const {
        return mTitle;
    }
    void showOSD(const Xtring &caption, const Xtring &text, int duration = -1);
    void showStickyOSD( const Xtring &caption, const Xtring &text, int duration = -1);
    void hideOSD();

    static Xtring getOpenFileName(const Xtring &caption, const Xtring &initialname = Xtring::null,
                                  const Xtring &filter = Xtring::null, QWidget *parent = 0);
    static Xtring getSaveFileName(const Xtring &caption, const Xtring &initialname = Xtring::null,
                                  const Xtring &filter = Xtring::null, QWidget *parent = 0);
    static Xtring getExistingDirName(const Xtring &caption, const Xtring &initialname = Xtring::null,
                                     QWidget *parent = 0);
	static bool writeFile( const Xtring &title, const Xtring &fname, const Xtring &content );

protected:
    Xtring mTitle;
    QTranslator mTranslator;
    GuiMainWindow *pMainWindow;
    OSDWidget *pOSD;
};

extern GuiApplication *theGuiApp;

} // namespace gong

#endif
