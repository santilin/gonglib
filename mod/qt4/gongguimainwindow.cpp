#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QActionGroup>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QSignalMapper>
#include <QtGui>

#include <gonggettext.h>
#include "gongguiapplication.h"
#include "gongguimainwindow.h"

namespace gong {

/**
  * The base class for GongLib application windows. It sets up the main
  * window as well as providing a menubar and statusbar
  * and statusbar. In initView(), your main view is created as the MDI child window manager.
  * Child windows are created in createClient(),
  *
  * http://qt-project.org/doc/qt-4.8/mainwindows-mdi-mainwindow-cpp.html
  */

GuiMainWindow::GuiMainWindow()
    : QMainWindow( 0 ), pWorkspace( 0 ), mImClosing( false )
{
    _GONG_DEBUG_TRACE( 1 );
}

GuiMainWindow::~GuiMainWindow()
{
    _GONG_DEBUG_TRACE( 1 );
}

QWidget *GuiMainWindow::getViewport() const
{
    return pWorkspace->viewport();
}

int GuiMainWindow::getNumClients()
{
    removeUnusedSubWindows();
    return pWorkspace->subWindowList().size();
}

/**
 * @brief Closes the main window if all the subwindows can be closed.
 *
 * The subwindows are closed in reverse creation order, to avoid closing a parent one
 *
 * @param event ...
 * @return void
 **/
void GuiMainWindow::closeEvent( QCloseEvent *event )
{
    mImClosing = true;
    QList<QMdiSubWindow*> list = pWorkspace->subWindowList( QMdiArea::ActivationHistoryOrder );
    while( list.size() ) {
        QMdiSubWindow *lastw = list[list.size()-1];
        if( lastw && lastw->widget() ) {
            if ( !lastw->close() ) {
                event->ignore();
                mImClosing = false;
                return;
            }
        } else if( lastw ) {
            lastw->close();
        } else {
            event->accept();
            return;
        }
        list = pWorkspace->subWindowList();
    }
    event->accept();
}


void GuiMainWindow::initGUI()
{
    pWorkspace = new QMdiArea();
    pWorkspace->setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded );
    pWorkspace->setVerticalScrollBarPolicy( Qt::ScrollBarAsNeeded );
    /* In the close event, we want to close subwindows in reverse creation order */
    pWorkspace->setActivationOrder( QMdiArea::StackingOrder );
    setCentralWidget( pWorkspace );
    connect( pWorkspace, SIGNAL( subWindowActivated( QMdiSubWindow* ) ), this, SLOT( slotUpdateMenus( QMdiSubWindow* ) ) );
    pWindowMapper = new QSignalMapper( this );
    connect( pWindowMapper, SIGNAL( mapped( QWidget* ) ), this, SLOT( slotSetActiveSubWindow( QWidget* ) ) );

    initActions();

    pMenuSystem = new QMenu( this );
    pMenuSystem->setObjectName( "MenuSystem" );
    menuBar()->insertItem( toGUI( _( "&Sistema" ) ), pMenuSystem );
    menuBar()->setNativeMenuBar( false );
}

void GuiMainWindow::initActions()
{


    QString text, desc;
    text = toGUI( _( "Cortar" ) ) ;
    desc = toGUI( _( "Corta la sección seleccionada y la coloca en el portapapeles" ) ) ;
    pMenuEditCut = new QAction( QIcon(":/images/cut.png"), text, this );
    pMenuEditCut->setShortcuts(QKeySequence::Cut);
    pMenuEditCut->setStatusTip( desc );
    connect( pMenuEditCut, SIGNAL( triggered() ), this, SLOT( slotEditCut() ) );

    text = toGUI( _( "Copiar" ) ) ;
    desc = toGUI( _( "Copia la sección seleccionada al portapapeles" ) ) ;
    pMenuEditCopy = new QAction( QIcon(":/images/copy.png"), text, this );
    pMenuEditCopy->setShortcuts(QKeySequence::Copy);
    pMenuEditCopy->setStatusTip( desc );
    connect( pMenuEditCopy, SIGNAL( triggered() ), this, SLOT( slotEditCopy() ) );

    text = toGUI( _( "Pegar" ) ) ;
    desc = toGUI( _( "Pega el contenido del portapapeles a la posición actual" ) ) ;
    pMenuEditPaste = new QAction( QIcon(":/images/paste.png"), text, this );
    pMenuEditPaste->setShortcuts(QKeySequence::Paste);
    pMenuEditPaste->setStatusTip( desc );
    connect( pMenuEditPaste, SIGNAL( triggered() ), this, SLOT( slotEditPaste() ) );

    text = toGUI( _( "Deshacer" ) ) ;
    desc = toGUI( _( "Deshace la última acción de edición" ) ) ;
    pMenuEditUndo = new QAction( text, QKeySequence::Undo, this, 0 );
    pMenuEditUndo->setStatusTip( desc );
    connect( pMenuEditUndo, SIGNAL( triggered() ), this, SLOT( slotEditUndo() ) );

    text = toGUI( _( "Ver línea de estado" ) ) ;
    desc = toGUI( _( "Muestra/Oculta la línea de estado" ) ) ;
    pMenuWindowStatusBar = new QAction( text, this );
    pMenuWindowStatusBar->setCheckable( true );
    pMenuWindowStatusBar->setStatusTip( desc );
    connect( pMenuWindowStatusBar, SIGNAL( toggled( bool ) ), this, SLOT( slotViewStatusBar( bool ) ) );

    text = toGUI( _( "Siguiente" ) ) ;
    desc = toGUI( _( "Activa la siguiente ventana" ) ) ;
    pMenuWindowNext = new QAction( text, QKeySequence::NextChild, this, 0 );
    pMenuWindowNext->setStatusTip( desc );
    connect( pMenuWindowNext, SIGNAL( triggered() ), pWorkspace, SLOT( activateNextSubWindow() ) );

    text = toGUI( _( "Anterior" ) ) ;
    desc = toGUI( _( "Activa la ventana anterior" ) ) ;
    pMenuWindowPrev = new QAction( text, QKeySequence::PreviousChild, this, 0 );
    pMenuWindowPrev->setStatusTip( desc );
    connect( pMenuWindowPrev, SIGNAL( triggered() ), pWorkspace, SLOT( activatePreviousSubWindow() ) );

    text = toGUI( _( "Cascada" ) ) ;
    desc = toGUI( _( "Muestra todas las ventanas en cascada" ) ) ;
    pMenuWindowCascade = new QAction( text, this );
    pMenuWindowCascade->setStatusTip( desc );
    connect( pMenuWindowCascade, SIGNAL( triggered() ), pWorkspace, SLOT( cascadeSubWindows() ) );

    text = toGUI( _( "Alicatar" ) ) ;
    desc = toGUI( _( "Alicatar la aplicación con las ventanas" ) ) ;
    pMenuWindowTile = new QAction( text, this );
    pMenuWindowTile->setStatusTip( desc );
    connect( pMenuWindowTile, SIGNAL( triggered() ), pWorkspace, SLOT( tileSubWindows() ) );

    text = toGUI( _( "Cerrar" ) ) ;
    desc = toGUI( _( "Cerrar la ventana activa" ) ) ;
    pMenuWindowClose = new QAction( text, QKeySequence::Close, this, 0 );
    pMenuWindowClose->setShortcutContext(Qt::WidgetShortcut);
    pMenuWindowClose->setStatusTip( desc );
    connect( pMenuWindowClose, SIGNAL( triggered() ), pWorkspace, SLOT( closeActiveSubWindow() ) );

    text = toGUI( _( "Cerrar todo" ) ) ;
    desc = toGUI( _( "Cerrar todas las ventanas" ) ) ;
    pMenuWindowCloseAll = new QAction( text, this );
    pMenuWindowCloseAll->setStatusTip( desc );
    connect( pMenuWindowCloseAll, SIGNAL( triggered() ), pWorkspace, SLOT( closeAllSubWindows() ) );

    text = toGUI( _( "Refrescar" ) ) ;
    desc = toGUI( _( "Refresca la ventana activa" ) ) ;
    pMenuWindowRefresh = new QAction( text, Qt::Key_F5, this, 0 );
    pMenuWindowRefresh->setStatusTip( desc );
    connect( pMenuWindowRefresh, SIGNAL( triggered() ), this, SLOT( slotMenuWindowRefresh() ) );

    pMenuWindowSeparator = new QAction( this );
    pMenuWindowSeparator->setSeparator( true );
}

/**
 * Finaliza la inicialización de la ventana principal, después de las inicializaciones de los plugins
 */
void GuiMainWindow::finishGUI()
{
    pMenuSystem->insertSeparator();
    QString desc = toGUI( _( "Termina el programa" ) );
    QString text = toGUI( _( "&Terminar" ) );
    pMenuSystemQuit = new QAction( text, QKeySequence::Quit, this, 0 );
    pMenuSystemQuit->setStatusTip( text );
    pMenuSystemQuit->setWhatsThis( text + "\n\n" + desc );
    connect( pMenuSystemQuit, SIGNAL( triggered() ), this, SLOT( slotMenuSystemQuit() ) );
    pMenuSystemQuit->addTo( pMenuSystem );

    // menuBar entry editMenu
    pMenuEdit = new QMenu( this );
    pMenuEditUndo->addTo( pMenuEdit );
    pMenuEdit->insertSeparator();
    pMenuEditCut->addTo( pMenuEdit );
    pMenuEditCopy->addTo( pMenuEdit );
    pMenuEditPaste->addTo( pMenuEdit );
    pMenuEdit->insertSeparator();

    ///////////////////////////////////////////////////////////////////
    // menuBar entry pMenuWindow
    pMenuWindow = new QMenu( this );
    pMenuWindow->addAction( pMenuWindowStatusBar );
    pMenuWindow->insertSeparator();
    pMenuWindow->addAction( pMenuWindowCascade );
    pMenuWindow->addAction( pMenuWindowTile );
    pMenuWindow->insertSeparator();
    pMenuWindow->addAction( pMenuWindowClose );
    pMenuWindow->addAction( pMenuWindowCloseAll );
    pMenuWindow->addAction( pMenuWindowRefresh );
    pMenuWindow->addAction( pMenuWindowNext );
    pMenuWindow->addAction( pMenuWindowPrev );
    pMenuWindow->addAction( pMenuWindowSeparator );
    slotUpdateMenuWindow();
    connect( pMenuWindow, SIGNAL( aboutToShow() ), this, SLOT( slotUpdateMenuWindow() ) );

    ///////////////////////////////////////////////////////////////////
    // menuBar entry helpMenu
    pMenuHelp = new QMenu( this );
    desc = "Muestra información sobre el programa " + toGUI(GongLibraryInstance->getPackageString());
    text = "Acerca de " + toGUI(GongLibraryInstance->getPackageString());
    pMenuHelpAbout = new QAction( text, this );
    pMenuHelpAbout->setStatusTip( text );
    pMenuHelpAbout->setWhatsThis( text + "\n\n" + desc );
    connect( pMenuHelpAbout, SIGNAL( triggered() ), this, SLOT( slotMenuHelpAbout() ) );
    pMenuHelp->addAction( pMenuHelpAbout );

    menuBar()->insertItem( toGUI( _( "&Edición" ) ), pMenuEdit );
    menuBar()->insertItem( toGUI( _( "Ve&ntanas" ) ), pMenuWindow );
    menuBar()->addSeparator();
    menuBar()->insertItem( toGUI( _( "A&yuda" ) ), pMenuHelp );

    slotUpdateMenus( 0 );
    showMaximized();
}

void GuiMainWindow::slotEditCut()
{
    /*
    if (activeClient())
        activeClient()->cut();
    	*/
}

void GuiMainWindow::slotEditCopy()
{
    /*
    if (activeClient())
    	activeClient()->copy();
    	*/
}

void GuiMainWindow::slotEditPaste()
{
    /*
    if (activeClient())
        activeClient()->paste();
    	*/
}

void GuiMainWindow::slotEditUndo()
{
    /*
    if (activeClient())
        activeClient()->undo();
    	*/
}

void GuiMainWindow::slotMenuHelpAbout()
{
    QMessageBox::about( this, toGUI( _( "Acerca de " + GongLibraryInstance->getPackageString() ) ),
                        toGUI( _( "Una bonita aplicación desarrollada por Santilín" ) ) );
}

void GuiMainWindow::slotUpdateMenus( QMdiSubWindow *activatedwindow )
{
    if ( activatedwindow && activatedwindow->widget() ) {
        if ( !activatedwindow->widget()->isEnabled() )
            activatedwindow->lower();
    }
    bool hasMdiChild = ( activeClient() != 0 );
    pMenuEditPaste->setEnabled( hasMdiChild );
    pMenuWindowClose->setEnabled( hasMdiChild );
    pMenuWindowCloseAll->setEnabled( hasMdiChild );
    pMenuWindowTile->setEnabled( hasMdiChild );
    pMenuWindowCascade->setEnabled( hasMdiChild );
    pMenuWindowNext->setEnabled( hasMdiChild );
    pMenuWindowPrev->setEnabled( hasMdiChild );
    pMenuWindowSeparator->setVisible( hasMdiChild );
    pMenuWindowRefresh->setVisible( hasMdiChild );

#if 0
    bool hasSelection = ( activeClient() &&
                          activeClient()->hasSelection() );
    pMenuEditCut->setEnabled( hasSelection );
    pMenuEditCopy->setEnabled( hasSelection );
#endif
}

void GuiMainWindow::removeUnusedSubWindows()
{
    QList<QMdiSubWindow *> list = pWorkspace->subWindowList();
    for( int i = 0; i < list.size(); ++i ) {
        QMdiSubWindow *aw = list[i];
        if( aw && (!aw->widget() || !aw->isVisible() ) )
            pWorkspace->removeSubWindow( aw );
    }
}

void GuiMainWindow::slotUpdateMenuWindow()
{
    QList<QAction *> allMenus = pMenuWindow->findChildren<QAction *>();
    for ( int i = allMenus.count() -1; i>0; --i )
    {
        QAction *qa = allMenus[i];
        if( qa->isCheckable() )
            pMenuWindow->removeAction(qa);
    }
    removeUnusedSubWindows();
    QList<QMdiSubWindow *> windows = pWorkspace->subWindowList();
    pMenuWindowSeparator->setVisible( !windows.isEmpty() );
    for ( int i = 0; i < windows.size(); ++i ) {
        QWidget *child = windows.at( i )->widget();
        if( !child )
            child = windows.at(i);
        _GONG_DEBUG_ASSERT( child );
        QString text;
        if ( i < 9 ) {
            text = tr( "&%1 %2" ).arg( i + 1 ).arg( child->caption() );
        } else {
            text = tr( "%1 %2" ).arg( i + 1 ).arg( child->caption() );
        }
        QAction *action  = pMenuWindow->addAction( text );
        action->setCheckable( true );
        action->setChecked( child == activeClient() );
        connect( action, SIGNAL( triggered() ), pWindowMapper, SLOT( map() ) );
        pWindowMapper->setMapping( action, windows.at( i ) );
    }
}


void GuiMainWindow::slotSetActiveSubWindow( QWidget *window )
{
    if ( !window )
        return;
    pWorkspace->setActiveSubWindow( qobject_cast<QMdiSubWindow *>( window ) );
}

QWidget *GuiMainWindow::activeClient() const
{
    if ( QMdiSubWindow *activeSubWindow = pWorkspace->activeSubWindow() )
        return activeSubWindow->widget();
    return 0;
}

QMdiSubWindow *GuiMainWindow::createClient( QWidget *w, bool force )
{
    removeUnusedSubWindows();
//     _GONG_DEBUG_PRINT( 0, Xtring::printf( "w: %d, %d, %d, %d", w->pos().x(), w->pos().y(), w->size().width(), w->size().height() ) );
    QMdiSubWindow *mdiw = pWorkspace->addSubWindow( w );
    mdiw->setAttribute( Qt::WA_DeleteOnClose );
    mdiw->setObjectName( "MDI_" + w->objectName() );
    connect( mdiw, SIGNAL( windowStateChanged(Qt::WindowStates,Qt::WindowStates) ),
             this, SLOT(slotWindowStateChanged(Qt::WindowStates,Qt::WindowStates) ) );
// 	connect( mdiw, SIGNAL(copyAvailable(bool)), pMenuEditCut, SLOT(setEnabled(bool)));
// 	connect( mdiw, SIGNAL(copyAvailable(bool)), pMenuEditCopy, SLOT(setEnabled(bool)));
    if ( !w->isVisible() )
        w->setVisible( true );
    // Don't go beyond the mainwindow's viewport
    int x= mdiw->x(), y= mdiw->y();
    if ( x > 0 && mdiw->width() > getViewport()->width() )
        x = 0;
    else if ( mdiw->x() + mdiw->width() > getViewport()->width() )
        x = getViewport()->width() - mdiw->width() - 10;
    if ( y > 0 && mdiw->height() > getViewport()->height() )
        y = 0;
    else if ( mdiw->y() + mdiw->height() > getViewport()->height() )
        y = getViewport()->height() - mdiw->height() - 10 ;
    mdiw->move( x, y );
    return mdiw;
}

QMdiSubWindow *GuiMainWindow::findClient( const QWidget *w ) const
{
    foreach( QMdiSubWindow *window, pWorkspace->subWindowList() ) {
        _GONG_DEBUG_ASSERT( window );
        if ( !window->widget() ) {
            _GONG_DEBUG_WARNING( "No active window found in workspace" );
            return 0;
        }
        if ( window == w || window->widget() == w )
            return window;
    }
    return 0;
}


bool GuiMainWindow::deleteClient( QWidget *w )
{
    foreach( QMdiSubWindow *window, pWorkspace->subWindowList() ) {
        if( window && (window->widget() == w || window == w) ) {
            delete window;
            return true;
        }
    }
    return false;
}

QWidget *GuiMainWindow::findWindow( const Xtring &name ) const
{
    foreach( QMdiSubWindow *window, pWorkspace->subWindowList() ) {
        _GONG_DEBUG_ASSERT( window );
        if ( !window->widget() ) {
            /// TODO: Look up mdiarea example, qt_cast
            _GONG_DEBUG_WARNING( "No active window found in workspace" );
            return 0;
        }
        if( Xtring( window->widget()->objectName() ).upper() == name.upper()
                || Xtring( window->objectName() ).upper() == name.upper() )
            return window;
    }
    return 0;
}

QAction* GuiMainWindow::findAction(const Xtring& name) const
{
    QAction *a = dynamic_cast<QAction *>( child( name.c_str() ) );
    if( !a )
        _GONG_DEBUG_WARNING( "Action '" + name + "' not found" );
    return a;
}


static QMenu *findMenuRecursively(QMenu *menu, const Xtring &name)
{
    QList<QAction *> submenus = menu->actions();
    for( QList<QAction *>::const_iterator it = submenus.begin();
            it != submenus.end(); ++ it) {
        QMenu *menu = (*it)->menu();
        if( menu ) {
            if( !strcmp(name.c_str(), menu->objectName() ) )
                return menu;
            menu = findMenuRecursively( menu, name );
            if( menu )
                return menu;
        }
    }
    return 0;
}

QMenu* GuiMainWindow::findMenu(const Xtring& name) const
{
    QList<QMenu*> menus = findChildren<QMenu*>();
    for( QList<QMenu*>::const_iterator it = menus.begin();
            it != menus.end(); ++ it) {
        QMenu *menu = *it;
        if( !strcmp(name.c_str(), menu->objectName() ) )
            return menu;
        menu = findMenuRecursively( menu, name );
        if( menu )
            return menu;
    }
    _GONG_DEBUG_WARNING( "Menu '" + name + "' not found" );
    return 0;
}


void GuiMainWindow::initStatusBar()
{
    statusBar()->message( toGUI( _( "Ready." ) ) ) ;
}

void GuiMainWindow::slotViewStatusBar( bool status )
{
#if OJO
    GongLibraryInstance->Config.setViewStatusBar( status );
#endif
    if ( status )
        statusBar() ->show();
    else
        statusBar() ->hide();

}

void GuiMainWindow::slotMenuWindowRefresh()
{
    if ( pWorkspace->currentSubWindow() ) {
        FrmBase *frmbase = dynamic_cast<FrmBase *>( pWorkspace->currentSubWindow() );
        if( !frmbase )
            frmbase = dynamic_cast<FrmBase *>( pWorkspace->currentSubWindow()->widget() );
        if( frmbase )
            frmbase->refresh();
        else
            pWorkspace->currentSubWindow()->update();
    }
}

/*
  Refresca los formularios de un determinado nombre.
  Por ejemplo, cuando insertamos un asiento, refresca los formularios
  de las cuentas para que se actualicen los saldos en pantalla
*/
/// TODO check
int GuiMainWindow::refreshByName( const Xtring &sendingName, const Xtring &name )
{
    int count = 0;
    foreach( QMdiSubWindow *window, pWorkspace->subWindowList() ) {
        _GONG_DEBUG_ASSERT( window );
        if ( !window->widget() ) {
            /// TODO: Look up mdiarea example, qt_cast
            _GONG_DEBUG_WARNING( "No active window found in workspace" );
            return 0;
        }
        if ( FrmBase * frm = dynamic_cast<FrmBase *>( window->widget() ) ) {
            if( frm->name() == sendingName ) // Do not refresh the sending one
                continue;
            if ( name.isEmpty() || Xtring( frm->name() ).upper().find( name.upper() ) != Xtring::npos ) {
                _GONG_DEBUG_PRINT( 6, Xtring::printf( "Refreshing %s", frm->name() ) );
                frm->refresh();
                count++;
            }
        }
    }
    return count;
}

/// TODO check
int GuiMainWindow::compressKey( const QKeyEvent *e ) const
{
    int key = e->key();
    key &= ~( Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier | Qt::SHIFT | Qt::CTRL | Qt::ALT );
    if (( e->key() & Qt::ShiftModifier ) || ( e->key() & Qt::SHIFT ) )
        key |= Qt::SHIFT;
    if (( e->key() & Qt::ControlModifier ) || ( e->key() & Qt::CTRL ) )
        key |= Qt::CTRL;
    if (( e->key() & Qt::AltModifier ) || ( e->key() & Qt::ALT ) )
        key |= Qt::ALT;
    return key;
}

void GuiMainWindow::slotMenuSystemQuit()
{
    close();
}

static QMdiSubWindow *pLastWindowDeactivated = 0;
static QMdiSubWindow *pLastWindowActivated = 0;
static bool donothing = false;

void GuiMainWindow::slotWindowStateChanged(Qt::WindowStates oldState, Qt::WindowStates newState)
{
    if( donothing )
        return;
    QMdiSubWindow *w = dynamic_cast<QMdiSubWindow *>(sender());
    if( w ) {
        if(newState == Qt::WindowActive || newState == Qt::WindowMaximized ) {
            pLastWindowActivated = w;
            if( !w->widget() ) {
                _GONG_DEBUG_WARNING( Xtring("Esta ventana ") + w->name() + " no es un widget" );
                return;
            }
            _GONG_DEBUG_PRINT(3, Xtring("Window: ") + w->name() + "(" + w->widget()->name() + ") activada." );
            if( !w->widget()->isEnabled() ) {
                _GONG_DEBUG_PRINT(3, Xtring( w->name() ) + " no está enabled" );
                if( pLastWindowDeactivated ) {
                    if( newState == Qt::WindowMaximized ) {
                        donothing = true;
                        pLastWindowDeactivated->showNormal();
                        donothing = false;
                    }
                    _GONG_DEBUG_PRINT(3, Xtring("Voy a activar ") + pLastWindowDeactivated->name() );
                    QMdiSubWindow *activaresta = pLastWindowDeactivated;
                    pLastWindowDeactivated = 0;
                    QRect r = activaresta->geometry();
                    donothing = true;
                    activaresta->setWindowState( Qt::WindowActive );
                    donothing = false;
                    activaresta->setGeometry( r );
                    activaresta->raise();
// 					if( FrmBase *fb = dynamic_cast<FrmBase *>(w->widget() ) )
// 						fb->restoreFocusWidget();
                }
            } else {
                _GONG_DEBUG_PRINT(3, Xtring("La ventana ") + w->name() + "(" + w->widget()->name() + ") estaba enabled" );
            }
        } else {
            pLastWindowDeactivated = w;
// 			if( FrmBase *fb = dynamic_cast<FrmBase *>(w->widget() ) )
// 				fb->saveFocusWidget();
            _GONG_DEBUG_PRINT(3, Xtring("Window ") + w->name() + " deactivated" );
        }
    } else {
        _GONG_DEBUG_WARNING( Xtring("Window") + w->name() + " is not a mdisubwindow" );
    }
}


}
