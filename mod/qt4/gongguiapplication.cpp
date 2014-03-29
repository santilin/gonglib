/** @file gongguiapplication.cpp El objeto global aplicación
 * GongLib project. (L) Santiago Capel Torres, 2003-2011
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <qtextcodec.h>
#include <QFileDialog>
#include <QEvent>
#include <QKeyEvent>
#include <QDesktopWidget>

#include <cerrno>
#include <gongdebug.h>
#include <gonggettext.h>
#include "gongfileutils.h"
#include "gongguiapplication.h"
#include "gongdatatable.h"
#include "gongguimainwindow.h"

namespace gong {

GuiApplication *theGuiApp = 0;
GuiApplication::GuiApplication ( const char *datadir, const char *packagename,
                                 const char *packageversion, int &argc, char **argv )
    : QApplication ( argc, argv, true ),
      GongLibrary ( datadir, packagename, packageversion, argc, argv ), pMainWindow ( 0 ), pOSD( 0 )
{
    theGuiApp = this;
    _GONG_DEBUG_TRACE ( 1 );
}

void GuiApplication::init()
{
    GongLibrary::init();
    char *language = getenv ( "LANG" );
    bool langok = language && *language && getRegConfig()->setLanguage ( language );
    if( !langok ) {
        language = getenv ( "LANGUAGE" );
        langok = getRegConfig()->setLanguage ( language );
    }
    if( mTranslator.load ( toGUI( "qt_" + getRegConfig()->getLanguage() ), QT_TRANSLATIONS ) ) {
        installTranslator ( &mTranslator );
    } else {
        _GONG_DEBUG_WARNING( "Error loading qt transtalor: qt_" + getRegConfig()->getLanguage() );
    }
}

GuiApplication::~GuiApplication()
{
    //	writeSettings(); Don't save settings, let the application do it
    if( pOSD )
        delete pOSD;
    if( pMainWindow )
        delete pMainWindow;

}

bool GuiApplication::initMainWindow()
{
    if( !pMainWindow )
        pMainWindow = new GuiMainWindow();
    pMainWindow->initGUI();
    // Si no, la aplicación no termina
    setMainWidget ( pMainWindow );
    pMainWindow->finishGUI();
    _GONG_DEBUG_PRINT ( 1, "Ventana principal creada" );
    return true;
}

void GuiApplication::sendKeyEvent ( QWidget *to, int key, int ascii, int state )
{
    QKeyEvent *keyevent = new QKeyEvent ( QEvent::KeyPress, key, ascii, state );
    QApplication::sendEvent ( to, keyevent );
}

void GuiApplication::waitCursor ( bool on )
{
    if ( on )
        setOverrideCursor( QCursor ( Qt::WaitCursor ) );
    else
        setOverrideCursor( QCursor( Qt::ArrowCursor ) );
}

void GuiApplication::resetCursor()
{
    restoreOverrideCursor();
}

void GuiApplication::setTitle(const Xtring & newtitle)
{
    if( pMainWindow ) {
        if( newtitle.isEmpty() )
            pMainWindow->setWindowTitle( toGUI( mTitle ) );
        else
            pMainWindow->setWindowTitle( toGUI( newtitle ) );
    }
}

void GuiApplication::showOSD(const Xtring &caption, const Xtring &text, int duration)
{
    if( !pOSD )
        pOSD = new OSDWidget( pMainWindow );
    if( duration >= 0 )
        pOSD->setDuration( duration );
    if( this->getMainWindow() )
        pOSD->setScreen( desktop()->screenNumber( this->getMainWindow() ) );
    pOSD->showOSD( caption, text, false );
}

void GuiApplication::showStickyOSD(const Xtring &caption, const Xtring &text, int duration)
{
    if( !pOSD )
        pOSD = new OSDWidget( pMainWindow );
    if( duration >= 0 )
        pOSD->setDuration( duration );
    if( this->getMainWindow() )
        pOSD->setScreen( desktop()->screenNumber( this->getMainWindow() ) );
    pOSD->showOSD( caption, text, true );
}

void GuiApplication::hideOSD()
{
    if( pOSD )
        pOSD->hideOSD();
}

static Xtring sLastOpenPath, sLastSavePath;

/**
 * @brief ...
 *
 * @param caption ...
 * @param initialname ...
 * @param filter  "Ficheros CSV (*.csv);;Todos los ficheros (*)"
 * @param parent ...
 * @return Xtring
 **/
Xtring GuiApplication::getOpenFileName(const Xtring &caption,
                                       const Xtring &initialname,
                                       const Xtring &filter,
                                       QWidget *parent)
{
    Xtring initial;
    if( initialname.isEmpty() )
        initial = sLastOpenPath ;
    else
        initial = initialname;
    QString fn = QFileDialog::getOpenFileName(parent, toGUI( caption ),
                 toGUI ( initial ),
                 toGUI( filter ) );
    if ( fn.isEmpty() )
        return Xtring::null;
    sLastOpenPath = FileUtils::path( fromGUI(fn) );
    return fromGUI(fn);
}

/**
 * @brief ...
 *
 * @param caption ...
 * @param initialname ...
 * @param filter  "Ficheros CSV (*.csv);;Todos los ficheros (*)"
 * @param parent ...
 * @return Xtring
 **/
Xtring GuiApplication::getSaveFileName(const Xtring &caption,
                                       const Xtring &initialname,
                                       const Xtring &filter,
                                       QWidget *parent)
{
    Xtring initial;
    if( initialname.isEmpty() )
        initial = sLastOpenPath ;
    else
        initial = initialname;
    QString fn = QFileDialog::getSaveFileName(parent, toGUI( caption ),
                 toGUI ( initial ), toGUI( filter ) );
    if ( fn.isEmpty() )
        return Xtring::null;
    sLastSavePath = FileUtils::path( fromGUI(fn) );
    return fromGUI(fn);
}


Xtring GuiApplication::getExistingDirName(const Xtring& caption,
        const Xtring& initialname, QWidget* parent)
{
    _GONG_DEBUG_TRACE(0);
    QString dirname = QFileDialog::getExistingDirectory(
                          parent, toGUI(caption), toGUI( initialname ),
                          QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    return fromGUI( dirname );
}

bool GuiApplication::writeFile(const Xtring &title, const Xtring& filename, const Xtring& content)
{
	const char *fname = filename.c_str();
	if( FileUtils::exists( fname ) ) {
		if( FrmBase::msgYesNoCancel( title, Xtring::printf( _("El fichero %s existe,\n¿quieres sobreescribirlo?"),
			fname ) ) != FrmBase::Yes )
			return false;
	}
	if( FileUtils::writeFile( fname, content ) ) {
		theGuiApp->showOSD( title, Xtring::printf( _("%s guardado correctamente"), fname ) );
		return true;
	} else {
		FrmBase::msgError(title, Xtring::printf( _("Error guardando %s\n%s"), fname, strerror( errno ) ) );
		return false;
	}
}

} // namespace gong

