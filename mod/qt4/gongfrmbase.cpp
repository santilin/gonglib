#include <unistd.h>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QPlainTextEdit>
#include <q3listbox.h>
#include <QDialogButtonBox>
#include <QPointer>

#include <gongfileutils.h>
#include <gonggettext.h>
#include "gongfrmbase.h"
#include "gongguiapplication.h"
#include "gongguimainwindow.h"
#include "gonglineedit.h"
#include "gongpushbutton.h"

namespace gong {

FrmBase::FrmBase ( QWidget *parent, const Xtring &name, WidgetFlags f )
    : QWidget ( theGuiApp->getMainWindow() ? parent : 0, name.c_str(), f ),
      mWasCancelled ( true ), mShownYet( false ), mClosingExternally( true ),
      pShowModalFor ( 0 ), pRealParent ( parent ), pFocusWidget ( 0 ),
      pSavedFocusWidget( 0 ), pEventLoop( 0 )
{
    _GONG_DEBUG_PRINT(4, Xtring("Creating ") + name );
    // Para que no tome el foco el formulario
    setFocusPolicy ( Qt::ClickFocus);
//    setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Minimum );
    // Las clases derivadas se encargarán de situar estos botones en layouts
    pushAccept = new QPushButton ( toGUI ( _ ( "&Aceptar" ) ), this, "pushAccept" );
    pushAccept->setIcon( pushAccept->style()->standardIcon( QStyle::SP_DialogYesButton ) );
    pushAccept->setDefault ( true );
    pushCancel = new QPushButton ( toGUI ( _ ( "&Cancelar" ) ), this, "pushCancel" );
    pushCancel->setIcon( pushAccept->style()->standardIcon( QStyle::SP_DialogCancelButton ) );
    // signals and slots connections
    connect ( pushAccept, SIGNAL ( clicked() ), this, SLOT ( pushAccept_clicked() ) );
    connect ( pushCancel, SIGNAL ( clicked() ), this, SLOT ( cancel() ) );
}


FrmBase::~FrmBase()
{
    _GONG_DEBUG_PRINT(1, Xtring("Destroying ") + name() );
}

void FrmBase::focusInEvent(QFocusEvent* e)
{
    QWidget::focusInEvent(e);
// 	restoreFocusWidget();
}

void FrmBase::focusOutEvent(QFocusEvent* e)
{
    QWidget::focusOutEvent(e);
// 	saveFocusWidget();
}

void FrmBase::changeEvent(QEvent* event)
{
    QWidget::changeEvent( event );
}


#if 0
QWidget* FrmBase::restoreFocusWidget()
{
    if( theGuiApp->focusWidget() ) _GONG_DEBUG_PRINT(0, Xtring("FW de la aplicacion:") + theGuiApp->focusWidget()->name() );
    if( focusWidget() ) _GONG_DEBUG_PRINT(0, Xtring("FW del widget:") + focusWidget()->name() );
    if( pSavedFocusWidget && pSavedFocusWidget != focusWidget()
            && pSavedFocusWidget != theGuiApp->focusWidget() ) {
        _GONG_DEBUG_PRINT(0, pSavedFocusWidget->name() );
        pSavedFocusWidget->setFocus();
    }
    pSavedFocusWidget = 0;
}


QWidget* FrmBase::saveFocusWidget()
{
    if( theGuiApp->focusWidget() ) _GONG_DEBUG_PRINT(0, Xtring("FW de la aplicacion:") + theGuiApp->focusWidget()->name() );
    if( focusWidget() ) _GONG_DEBUG_PRINT(0, Xtring("FW del widget:") + focusWidget()->name() );
    pSavedFocusWidget = focusWidget();
    if( pSavedFocusWidget )
        _GONG_DEBUG_PRINT(0, pSavedFocusWidget->name() );
}


void FrmBase::changeEvent(QEvent* event)
{
    QWidget::changeEvent( event );
    return;
    switch( event->type() ) {
    case QEvent::ActivationChange:
        if( focusWidget() ) _GONG_DEBUG_PRINT(0, Xtring("FW del widget '") + name() + "':" + focusWidget()->name() );
        if( theGuiApp->focusWidget() ) _GONG_DEBUG_PRINT(0, Xtring("FW de la aplicacion:") + theGuiApp->focusWidget()->name() );
        break;
    case QEvent::WindowStateChange:
        QWindowStateChangeEvent *wse = static_cast<QWindowStateChangeEvent *>(event);
        _GONG_DEBUG_PRINT(0, name() );
        _GONG_DEBUG_PRINT(0, wse->oldState() );
        _GONG_DEBUG_PRINT(0, windowState() );
        if( wse->oldState() == Qt::WindowNoState ) {
            if( focusWidget() ) _GONG_DEBUG_PRINT(0, Xtring("FW del widget '") + name() + "':" + focusWidget()->name() );
            if( theGuiApp->focusWidget() ) _GONG_DEBUG_PRINT(0, Xtring("FW de la aplicacion:") + theGuiApp->focusWidget()->name() );
        }
        break;
    }
}
#endif


/*
  reimplementada de QWidget
  Maneja la tecla Escape para que cancele y la configurada para que
  acepte el formulario.
*/
void FrmBase::keyPressEvent ( QKeyEvent *e )
{
    if ( e->state() == 0 && e->key() == Qt::Key_Escape )
    {
        cancel();
        e->accept();
        return;
    }
    else if ( e->key() == Qt::Key_F12 ) // TODO: read from config
    {
        _GONG_DEBUG_PRINT ( 6, Xtring::printf ( "Setting focus to pushAccept" ) );
        pushAccept->setFocus();
        // Obligamos al control que tiene el foco a que se valide.
        // La función setFocus es síncrona, por lo tanto, si hay un error de validación
        // que provoca que el foco no recaiga finalmente en pushAccept, simplemente no grabamos
        if ( focusWidget() == pushAccept )
            accept();
        return;
    }
    else if ( e->key() == Qt::Key_F5 )
    {
        refresh();
        return;
    }
#ifdef _GONG_DEBUG
    if( e->key() == Qt::Key_F10 ) {
        _GONG_DEBUG_WARNING( Xtring("ClassName: ") + metaObject()->className());
        _GONG_DEBUG_WARNING( Xtring("ObjectName: " + objectName()) );
        _GONG_DEBUG_WARNING( Xtring::printf("FocusWidget: %s", theGuiApp->focusWidget()->name() ) );
        QWidget *widget = qApp->widgetAt(QCursor::pos());
        _GONG_DEBUG_WARNING( Xtring::printf("Widget under mouse: (%s) %s",
                                            widget->metaObject()->className(), widget->name() ) );
    }
#endif
    QWidget::keyPressEvent ( e );
}

void FrmBase::pushAccept_clicked()
{
    LineEdit *edit = dynamic_cast<LineEdit *> ( focusWidget() );
    if ( edit )
    {
        pushAccept->setFocus();
        // It is very curious that when you have just choosen a record and closed the
        // window, if you just click on pushAccept, the focus goes to pushCancel
        if ( focusWidget() == pushAccept || focusWidget() == pushCancel )
            accept();
    }
    else
    {
        accept();
    }
}

void FrmBase::showEvent(QShowEvent *event)
{
    justShown( !mShownYet );
    mShownYet = true;
}

void FrmBase::accept()
{
    mWasCancelled = false;
    mClosingExternally = false;
    close();
    mClosingExternally = true;
    if( pEventLoop )
        pEventLoop->exit( 0 ); // mWasCancelled = false
}

void FrmBase::cancel()
{
    if( LineEdit *le = dynamic_cast<LineEdit *>(focusWidget()) )
        le->setCancelling();
    mWasCancelled = true;
    mClosingExternally = false;
    close();
    mClosingExternally = true;
}

/**
 * @brief Tries to close this FrmBase.
 * If the closing reason is external, the qmdisubwindow will be closed automatically.
 * It not, we must close the qmidsubwindow here.
 *
 * @param e ...
 * @return void
 **/
void FrmBase::closeEvent ( QCloseEvent *e )
{
// 	_GONG_DEBUG_PRINT( 0, "Close event for " + Xtring( name() ) );
    theGuiApp->waitCursor( true );
    if ( canClose() ) {
        if( !mClosingExternally && parentWidget() && theGuiApp->getMainWindow() ) {
            QWidget *pw = theGuiApp->getMainWindow()->findClient( parentWidget() );
            if( pw ) {
                pw->deleteLater();
                pw->close();
            }
        }
        if( pEventLoop )
            pEventLoop->exit( mWasCancelled );
        if ( pShowModalFor ) {
//            _GONG_DEBUG_PRINT ( 0, Xtring::printf ( "Activating %s", pShowModalFor->name() ) );
            pShowModalFor->setEnabled( true );
            theGuiApp->processEvents();
/*			
            pShowModalFor->hide();
            pShowModalFor->showNormal();
/*			
            pShowModalFor->activateWindow();
            pShowModalFor->showNormal();
            pShowModalFor->raise();
            */
            pShowModalFor = 0;
            if( pSavedFocusWidget ) {
//                _GONG_DEBUG_PRINT(0, "Setting focus to " + Xtring( pSavedFocusWidget->name() ) );
                pSavedFocusWidget->setFocus();
                pSavedFocusWidget = 0;
            }
        }
        e->accept();
    } else {
        e->ignore();
    }
    theGuiApp->resetCursor();
}

bool FrmBase::canClose()
{
    return true;
}

void FrmBase::refresh()
{
    _GONG_DEBUG_PRINT(1, Xtring("Refreshing ") + name() );
}

void FrmBase::setInitialFocus()
{
    if( pFocusWidget && pFocusWidget != focusWidget() ) {
        setWiseFocus( pFocusWidget );
    }
}

/**
 * @brief Sets focus to widget w.
 * If the widget is in a hiden tab wihtin a QTabWidget, activates the tab
 *
 * @param w ...
 * @return void
 **/
void FrmBase::setWiseFocus(QWidget *w)
{
    if( !w )
        return;
    QWidget *grandpa = static_cast<QWidget *>(w->parentWidget());
    bool exit = false;
    while( grandpa && !exit ) {
        if( QTabWidget *tabwidget = dynamic_cast<QTabWidget *>(grandpa) ) {
            for( int i = 0; i < tabwidget->count() && !exit; ++i ) {
                QWidget *tab = tabwidget->widget(i);
                if( tab == tabwidget->currentWidget() )
                    break;
                /// TODO: Look up a function QWidget->child( QWidget *)
                QList<QWidget *>allwidgets = tab->findChildren<QWidget *>();
                for( QList<QWidget *>::const_iterator wit = allwidgets.begin();
                        wit != allwidgets.end(); ++wit ) {
                    if( *wit == w ) {
                        tabwidget->setCurrentWidget( tab );
                        exit = true;
                        break;
                    }
                }
            }
        }
        grandpa = static_cast<QWidget *>(grandpa->parentWidget());
    }
    w->setFocus( Qt::OtherFocusReason );
    if( LineEdit *le = dynamic_cast<LineEdit *>(w) ) {
        if( le->getValueType() == Variant::tDouble
                || le->getValueType() == Variant::tMoney )
            le->setCursorAtDecPoint();
    }
}


/*
 * When in MDI, if the parent is maximized, here parentWidget() is also maximized,
 * therefore its size and position is not that of the window we want to show.
 * We must show the parent normal first
*/
void FrmBase::showModalFor( QWidget *parent, bool centered, bool createclient )
{
    pShowModalFor = parent;
    QWidget *wasmaximized = 0;
    if( parent ) {
        wasmaximized = parent->isMaximized() ? 
			parent : (parent->parentWidget() && parent->parentWidget()->isMaximized()) ? 
				parent->parentWidget() : 0;
        parent->setEnabled ( false );
    }
    if( !isVisible() ) {
        if( createclient && theGuiApp->getMainWindow() ) {
            theGuiApp->getMainWindow()->createClient(this)->deleteLater();
        } else {
            show();
        }
    }
    QWidget *parentwin = parentWidget();
    if ( !parentwin || parentwin == parent )
        parentwin = this;
    int x=parentwin->x(), y=parentwin->y();
#if 0    
 	_GONG_DEBUG_PRINT ( 0, Xtring::printf ( "Centrando ventana: Padre (x=%d,y=%d,w=%d,h=%d), this (x=%d,y=%d,w=%d,h=%d), parentwin (x=%d,y=%d,w=%d,h=%d)",
 											parent?parent->x() :-1, parent?parent->y() :-1, parent?parent->width() :-1, parent?parent->height() :-1,
 											this->x(), this->y(), width(), height(),
 											parentwin->x(), parentwin->y(), parentwin->width(), parentwin->height() ) );
#endif	
    if ( centered || !theGuiApp->getMainWindow() )
    {
        if ( parent /*&& !wasmaximized*/ )  // Center to the parent
        {
            if ( parentwin->width() < parent->width() )
                x = parent->x() + ( parent->width() - parentwin->width() ) / 2;
            if ( parentwin->height() < parent->height() )
                y = parent->y() + ( parent->height() - parentwin->height() ) / 2;
        }
        else if ( theGuiApp->getMainWindow() )  // Center to the main window
        {
            x = ( theGuiApp->getMainWindow()->getViewport()->width()
                  - parentwin->width() ) / 2;
            y = ( theGuiApp->getMainWindow()->getViewport()->height()
                  - parentwin->height() ) / 2;
        }
    }
    // Check if window lays out of the main window
    if ( theGuiApp->getMainWindow() )
    {
        if ( parentwin->width() > theGuiApp->getMainWindow()->getViewport()->width() )
            x = 0;
        else if ( parentwin->x() + parentwin->width() > theGuiApp->getMainWindow()->getViewport()->width() )
            x = ( theGuiApp->getMainWindow()->getViewport()->width() - parentwin->width() ) / 2;
        if ( parentwin->height() > theGuiApp->getMainWindow()->getViewport()->height() )
            y = 0;
        else if ( parentwin->y() + parentwin->height() > theGuiApp->getMainWindow()->getViewport()->height() )
            y = ( theGuiApp->getMainWindow()->getViewport()->height() - parentwin->height() ) / 2;
    }
    if ( x != parentwin->x() || y != parentwin->y() )
        parentwin->move ( x, y );
//    raise();
//    activateWindow();
    theGuiApp->changeOverrideCursor( QCursor( Qt::ArrowCursor ) );
//     while ( !isHidden() ) {
//         theGuiApp->processEvents();
//         usleep ( 1000 );
//     }
    pEventLoop = new QEventLoop();
    QPointer<FrmBase> guard = this;
    mWasCancelled = pEventLoop->exec(QEventLoop::DialogExec);
    if( guard.isNull() )
        mWasCancelled = true;
    delete pEventLoop;
    pEventLoop = 0;
//     theGuiApp->resetCursor();
    if( wasmaximized )
        wasmaximized->showMaximized();
    // parent is reenabled in the close event of this form
}

bool FrmBase::isModalFor( QWidget *parent) const
{
    return pShowModalFor == parent;
}

void FrmBase::alignLayout( QBoxLayout *layout, bool totheleft )
{
    _GONG_DEBUG_ASSERT( layout );
    QSpacerItem * spacer = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    if( totheleft )
        layout->insertSpacerItem( -1, spacer );
    else
        layout->insertSpacerItem(0, spacer );
}


/**
 * @brief This function gets called every time the form receives a ShowEvent
 * 	Sets the initial focus of the form
 * 
 * You can override this function to do something the first time the form is shown
 * 
 * @param firsttime whether this is the first time that this form is shown
 * @return void
 */
void FrmBase::justShown(bool firsttime)
{
    if( firsttime ) {
        if( pShowModalFor )
            pSavedFocusWidget = pShowModalFor->focusWidget();
        else
            pSavedFocusWidget = theGuiApp->focusWidget();
        setInitialFocus();
    }
}

/**
 * @brief sets the form geometry from a string.
 *
 * @param geometry Geometries are separated by |. The first one is the Qt one
 * @return void
 **/
void FrmBase::setGeometry(const Xtring& geometry)
{
    Xtring first, second;
    geometry.splitIn2( first, second, "|" );
    if( parentWidget() )
        parentWidget()->restoreGeometry( toGUIByteArray( first ) );
    else
        restoreGeometry( toGUIByteArray( first ) );
}

Xtring FrmBase::getGeometry() const
{
    if( parentWidget() )
        return fromGUI( parentWidget()->saveGeometry() );
    else
        return fromGUI( saveGeometry() );
}

// Static functions

bool FrmBase::msgYesNo ( const Xtring &caption, const Xtring &text, bool defbutton, QWidget *parent )
{
    QApplication::setOverrideCursor ( QCursor ( Qt::ArrowCursor ) );
    switch ( QMessageBox::question ( parent ? parent : theGuiApp->getMainWindow(),
                                     toGUI( caption ), toGUI( text ),
                                     defbutton?FrmBase::Yes:FrmBase::No,
                                     defbutton?FrmBase::No:FrmBase::Yes ) )
    {
    case FrmBase::Yes:
        QApplication::restoreOverrideCursor();
        return true;
    case FrmBase::No:
    default: // escape
        QApplication::restoreOverrideCursor();
        return false;
    }
}

bool FrmBase::msgYesNo ( QWidget *parent, const Xtring &text, bool defbutton )
{
    QApplication::setOverrideCursor ( QCursor ( Qt::ArrowCursor ) );
    switch ( QMessageBox::question ( parent, parent? parent->windowTitle() : "",
                                     toGUI(text),
                                     defbutton?FrmBase::Yes:FrmBase::No,
                                     defbutton?FrmBase::No:FrmBase::Yes ) )
    {
    case FrmBase::Yes:
        QApplication::restoreOverrideCursor();
        return true;
    case FrmBase::No:
    default: // escape
        QApplication::restoreOverrideCursor();
        return false;
    }
}

void FrmBase::msgOk ( QWidget *parent, const Xtring &text, const MsgBoxType type )
{
    QApplication::setOverrideCursor ( QCursor ( Qt::ArrowCursor ) );
    QMessageBox msgBox( parent );
    msgBox.setText( toGUI( text ) );
    if( parent )
        msgBox.setWindowTitle( parent->windowTitle() );
    else
        msgBox.setWindowTitle( toGUI( theGuiApp->getPackageString() ) );
    msgBox.setStandardButtons( QMessageBox::Ok );
    switch( type ) {
    case information:
        msgBox.setIcon( QMessageBox::Information );
        break;
    case question:
        msgBox.setIcon( QMessageBox::Question );
        break;
    case warning:
        msgBox.setIcon( QMessageBox::Warning );
        break;
    case error:
        msgBox.setIcon( QMessageBox::Critical );
        break;
    }
    msgBox.exec();
    QApplication::restoreOverrideCursor();
}

void FrmBase::msgOk(const Xtring& caption, const Xtring& text, FrmBase::MsgBoxType type, QWidget* parent)
{
    QApplication::setOverrideCursor ( QCursor ( Qt::ArrowCursor ) );
    QMessageBox msgBox( parent ? parent : (theGuiApp->getMainWindow() ? theGuiApp->getMainWindow() : 0) );
    msgBox.setText( toGUI( text ) );
    msgBox.setWindowTitle( toGUI( caption ) );
    msgBox.setStandardButtons( QMessageBox::Ok );
    switch( type ) {
    case information:
        msgBox.setIcon( QMessageBox::Information );
        break;
    case question:
        msgBox.setIcon( QMessageBox::Question );
        break;
    case warning:
        msgBox.setIcon( QMessageBox::Warning );
        break;
    case error:
        msgBox.setIcon( QMessageBox::Critical );
        break;
    }
    msgBox.exec();
    QApplication::restoreOverrideCursor();
}

void FrmBase::msgOkLarge ( QWidget *parent, const Xtring &text, const Xtring &largetext )
{
    if( parent )
        msgOkLarge ( fromGUI(parent->windowTitle()), text, largetext, parent );
    else
        msgOkLarge ( theGuiApp->getPackageString(), text, largetext, parent);
}

void FrmBase::msgOkLarge ( const Xtring &caption, const Xtring &text,
                           const Xtring &largeText, QWidget * parent )
{
    QDialog *largebox = new QDialogWithSaveAs ( largeText, parent, "largebox", true );
    largebox->setWindowTitle( toGUI(caption) );
    largebox->setFocusPolicy ( Qt::ClickFocus );
    QVBoxLayout *pFormLayout = new QVBoxLayout ( largebox, 2, 0, "pFormLayout" );
    QVBoxLayout *pMainLayout = new QVBoxLayout ( 0, 0, 0, "pMainLayout" );
    QVBoxLayout *pControlsLayout = new QVBoxLayout ( 0, 10, 6, "pControlsLayout" );

    QPlainTextEdit *te = new QPlainTextEdit ( toGUI(largeText), largebox );
    te->setReadOnly( true );
    pControlsLayout->addWidget ( te );

    QLabel *question = new QLabel ( toGUI(text), largebox );
    pControlsLayout->addWidget ( question );

    // Botones
    QHBoxLayout *pButtonsLayout = new QHBoxLayout ( 0, 5, 6, "pButtonsLayout" );
    QSpacerItem *spacer1 = new QSpacerItem ( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    pButtonsLayout->addItem ( spacer1 );
    PushButton *pushSaveAs = new PushButton ( largebox, _( "&Guardar" ) );
    pushSaveAs->setIcon( pushSaveAs->style()->standardIcon( QStyle::SP_DialogSaveButton ) );
    connect (pushSaveAs, SIGNAL ( clicked() ), largebox, SLOT ( slotSaveAs() ) );
    pButtonsLayout->addWidget (pushSaveAs );
    PushButton *pushAccept = new PushButton ( largebox, _( "&Vale" ) );
    pushAccept->setIcon( pushAccept->style()->standardIcon( QStyle::SP_DialogOkButton ) );
    pushAccept->setDefault ( true );
    connect (pushAccept, SIGNAL ( clicked() ), largebox, SLOT ( accept() ) );
    pButtonsLayout->addWidget (pushAccept );

    pMainLayout->addLayout ( pControlsLayout );
    pMainLayout->addLayout ( pButtonsLayout );

    pFormLayout->addLayout ( pMainLayout );
    largebox->setMinimumSize ( QSize ( 600, 500 ) );

    largebox->exec();
    QApplication::restoreOverrideCursor();
}

void FrmBase::msgError(QWidget* parent, const Xtring& text, const Xtring& detailedError)
{
    QApplication::setOverrideCursor ( QCursor ( Qt::ArrowCursor ) );
    QMessageBox msgBox( parent );
    msgBox.setText( toGUI( text ) );
    if( parent )
        msgBox.setWindowTitle( parent->windowTitle() );
    else
        msgBox.setWindowTitle( toGUI( theGuiApp->getPackageString() ) );
    msgBox.setDetailedText( toGUI( detailedError ) );
    msgBox.setStandardButtons( QMessageBox::Ok );
    msgBox.setIcon( QMessageBox::Critical );
    msgBox.exec();
    QApplication::restoreOverrideCursor();
}

void FrmBase::msgError(const Xtring& caption, const Xtring& text,
                       const Xtring& detailedError, QWidget* parent )
{
    QApplication::setOverrideCursor ( QCursor ( Qt::ArrowCursor ) );
    QMessageBox msgBox( parent ? parent : (theGuiApp->getMainWindow() ? theGuiApp->getMainWindow() : 0) );
    msgBox.setText( toGUI( text ) );
    msgBox.setWindowTitle( toGUI( caption ) );
    msgBox.setDetailedText( toGUI( detailedError ) );
    msgBox.setStandardButtons( QMessageBox::Ok );
    msgBox.setIcon( QMessageBox::Critical );
    msgBox.exec();
    QApplication::restoreOverrideCursor();
}


int FrmBase::msgYesNoCancel ( QWidget *parent, const Xtring &text, bool defbutton )
{
    return msgYesNoCancel ( fromGUI ( parent->windowTitle() ), text, defbutton, parent );
}

int FrmBase::msgYesNoCancel ( const Xtring &caption, const Xtring &text, bool defbutton,
                              QWidget *parent )
{
    QApplication::setOverrideCursor ( QCursor ( Qt::ArrowCursor ) );
    int ret = QMessageBox::critical ( parent, toGUI( caption ), toGUI( text ),
                                      defbutton?FrmBase::Yes:FrmBase::No,
                                      defbutton?FrmBase::No:FrmBase::Yes,
                                      FrmBase::Cancel );
    QApplication::restoreOverrideCursor();
    return ret;
}

int FrmBase::msgYesNoAllCancel( QWidget *parent, const Xtring &text, bool defbutton )
{
    if( parent )
        return FrmBase::msgYesNoAllCancel( fromGUI(parent->windowTitle()), text, defbutton, parent );
    else
        return FrmBase::msgYesNoAllCancel(theGuiApp->getPackageString(), text, defbutton, parent );
}

int FrmBase::msgYesNoAllCancel(const Xtring& caption, const Xtring& text, bool defbutton, QWidget* parent)
{
    QApplication::setOverrideCursor ( QCursor ( Qt::ArrowCursor ) );
    QMessageBox msgBox( parent );
    msgBox.setWindowTitle( toGUI(caption) );
    msgBox.setInformativeText( toGUI( text ) );
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::YesToAll | QMessageBox::NoToAll | QMessageBox::Cancel);
    msgBox.setDefaultButton(defbutton?QMessageBox::Yes:QMessageBox::No);
    int ret = msgBox.exec();
    QApplication::restoreOverrideCursor();
    return ret;
}

bool FrmBase::msgYesNoLarge ( QWidget *parent, const Xtring &text,
                              const Xtring &largeText, bool defbutton )
{
    return msgYesNoLarge ( fromGUI( parent->windowTitle() ), text, largeText, parent );
}

bool FrmBase::msgYesNoLarge ( const Xtring &caption, const Xtring &text,
                              const Xtring &largeText, bool defbutton, QWidget *parent )
{
    QApplication::setOverrideCursor ( QCursor ( Qt::ArrowCursor ) );
#if 0
    if ( largeText.length() < 80 )
        largeText = largeText.rightJustify ( 80 ) + ".";
    else
#endif

        QDialog *largebox = new QDialog ( parent, "largebox", true );
    largebox->setWindowTitle( toGUI( caption ) );
    largebox->setFocusPolicy ( Qt::ClickFocus );
    QVBoxLayout *pFormLayout = new QVBoxLayout ( largebox, 2, 0, "pFormLayout" );
    QVBoxLayout *pMainLayout = new QVBoxLayout ( 0, 0, 0, "pMainLayout" );
    QVBoxLayout *pControlsLayout = new QVBoxLayout ( 0, 10, 6, "pControlsLayout" );

    QPlainTextEdit *te = new QPlainTextEdit ( toGUI(largeText), largebox );
    te->setReadOnly( true );
    pControlsLayout->addWidget ( te );

    QLabel *question = new QLabel ( toGUI(text), largebox );
    pControlsLayout->addWidget ( question );

    // Botones
// QHBoxLayout *pButtonsLayout = new QHBoxLayout ( 0, 5, 6, "pButtonsLayout" );
// 	QSpacerItem *spacer1 = new QSpacerItem ( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
// 	pButtonsLayout->addItem ( spacer1 );
// 	// Las clases derivadas se encargarán de situar estos botones en layouts
// 	PushButton *pushAccept = new PushButton( largebox, _( "&Sí" ) );
// 	pushAccept->setDefault ( true );
// 	pushAccept->setIcon( pushAccept->style()->standardIcon( QStyle::SP_DialogYesButton ) );
// 	PushButton *pushCancel = new PushButton( largebox, _( "&No" ) );
// 	pushCancel->setIcon( pushAccept->style()->standardIcon( QStyle::SP_DialogCancelButton ) );
// 	// signals and slots connections
// 	connect( pushAccept, SIGNAL ( clicked() ), largebox, SLOT ( accept() ) );
// 	connect( pushCancel, SIGNAL ( clicked() ), largebox, SLOT ( reject() ) );
// 	pButtonsLayout->addWidget( pushAccept );
// 	pButtonsLayout->addWidget( pushCancel );

    // Botones
    QHBoxLayout *pButtonsLayout = new QHBoxLayout();
    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Yes | QDialogButtonBox::No, Qt::Horizontal, largebox );
    QPushButton *yesButton = buttonBox->button( QDialogButtonBox::Yes );
    yesButton->setDefault( true );
    connect(buttonBox, SIGNAL(accepted()), largebox, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), largebox, SLOT(reject()));
    pButtonsLayout->addWidget( buttonBox );

    pMainLayout->addLayout ( pControlsLayout );
    pMainLayout->addLayout ( pButtonsLayout );

    pFormLayout->addLayout ( pMainLayout );
    largebox->setMinimumSize ( QSize ( 600, 500 ) );

    largebox->exec();
    QApplication::restoreOverrideCursor();
    return largebox->result() == QDialog::Accepted;
}

/**
 * Shows a dialog to choose a string from a list of strings
 * @param parent
 * @param text
 * @param strlist The list of strings to choose from
 * @return The index of the chosen string, or -1 if none
 */
int FrmBase::msgXtringList ( QWidget *parent, const Xtring &text, const XtringList &strlist )
{
    QApplication::setOverrideCursor ( QCursor ( Qt::ArrowCursor ) );
    QDialog *msgxtringlist = new QDialog ( parent, "msgxtringlist", true, Qt::WindowTitleHint );
    if( parent )
        msgxtringlist->setWindowTitle( parent->windowTitle() );
    else
        msgxtringlist->setWindowTitle( toGUI( theGuiApp->getPackageString() ) );
    msgxtringlist->setFocusPolicy ( Qt::ClickFocus );
    QVBoxLayout *pFormLayout = new QVBoxLayout ( msgxtringlist, 2, 0, "pFormLayout" );
    QVBoxLayout *pMainLayout = new QVBoxLayout ( 0, 0, 0, "pMainLayout" );
    QVBoxLayout *pControlsLayout = new QVBoxLayout ( 0, 10, 6, "pControlsLayout" );

    Q3ListBox *strings = new Q3ListBox( msgxtringlist );
    connect ( strings, SIGNAL ( doubleClicked ( Q3ListBoxItem * ) ), msgxtringlist, SLOT ( accept() ) );
    for( XtringList::const_iterator it = strlist.begin(); it != strlist.end(); ++ it )
        strings->insertItem ( QString::fromLocal8Bit ( (*it).c_str() ) );
    strings->setSelected( 0, true );
    pControlsLayout->addWidget ( strings );

    QLabel *question = new QLabel ( toGUI(text), msgxtringlist );
    pControlsLayout->addWidget ( question );

    // Botones
    QHBoxLayout *pButtonsLayout = new QHBoxLayout();
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
            | QDialogButtonBox::Cancel);
    connect(buttonBox, SIGNAL(accepted()), msgxtringlist, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), msgxtringlist, SLOT(reject()));
    QPushButton *acceptButton = buttonBox->button( QDialogButtonBox::Ok );
    acceptButton->setDefault( true );
    acceptButton->setAutoDefault( true );
    pButtonsLayout->addWidget( buttonBox );
    pMainLayout->addLayout ( pControlsLayout );
    pMainLayout->addLayout ( pButtonsLayout );

    pFormLayout->addLayout ( pMainLayout );
    msgxtringlist->setMinimumSize ( QSize ( 600, 500 ) );

    msgxtringlist->exec();
    QApplication::restoreOverrideCursor();
    if( msgxtringlist->result() == QDialog::Accepted ) {
        return strings->currentItem();
    } else
        return -1;
}

bool FrmBase::inputBoxInt(int& value, const Xtring& prompt, const Xtring& title, QWidget* parent)
{
    FrmBase *frm = new FrmBase( parent );
    QVBoxLayout *frmlayout = new QVBoxLayout( frm );
    LineEdit *edit = new LineEdit ( frm, Xtring::null, Variant::tInt );
    edit->setText ( value );
    QLabel *label = new QLabel( toGUI(prompt), frm );
    QHBoxLayout *hlay = new QHBoxLayout();
    hlay->addWidget ( label );
    hlay->addWidget ( edit );
    frmlayout->addLayout( hlay );
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget( frm->getCancelButton() );
    buttonsLayout->addWidget( frm->getAcceptButton() );
    frmlayout->addLayout( buttonsLayout );
    frm->pFocusWidget = edit;
    frm->showModalFor(parent, true, false );
    if( !frm->wasCancelled() ) {
        value = edit->toInt();
        return true;
    } else
        return false;
}

bool FrmBase::inputBoxString(Xtring& value, const Xtring& prompt, const Xtring& title, QWidget* parent)
{
    FrmBase *frm = new FrmBase( parent );
    QVBoxLayout *frmlayout = new QVBoxLayout( frm );
    LineEdit *edit = new LineEdit ( frm, Xtring::null, Variant::tString );
    edit->setText ( value );
    QLabel *label = new QLabel( toGUI(prompt), frm );
    QHBoxLayout *hlay = new QHBoxLayout();
    hlay->addWidget ( label );
    hlay->addWidget ( edit );
    frmlayout->addLayout( hlay );
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget( frm->getCancelButton() );
    buttonsLayout->addWidget( frm->getAcceptButton() );
    frmlayout->addLayout( buttonsLayout );
    frm->pFocusWidget = edit;
    frm->showModalFor(parent, true, true );
    if( !frm->wasCancelled() ) {
        value = edit->toString();
        return true;
    } else
        return false;
}


bool FrmBase::inputBoxText(Xtring& value, const Xtring& prompt, const Xtring& title, QWidget* parent)
{
    FrmBase *frm = new FrmBase( parent );
    QVBoxLayout *frmlayout = new QVBoxLayout(frm);

    TextEdit *edit = new TextEdit ( frm );
    edit->setText ( value );
    edit->setMinimumHeight( edit->fontMetrics().height() * 15);
    edit->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::MinimumExpanding );
    QLabel *label = new QLabel( toGUI(prompt), frm );
    QHBoxLayout *hlay = new QHBoxLayout();
    hlay->addWidget ( label );
    hlay->addWidget ( edit );

    QHBoxLayout *buttonslayout = new QHBoxLayout();
    buttonslayout->addWidget( frm->getCancelButton() );
    buttonslayout->addWidget( frm->getAcceptButton() );
    frmlayout->addLayout( hlay );
    frmlayout->addLayout( buttonslayout );
    frm->pFocusWidget = edit;
    bool ret = false;
    frm->showModalFor(parent, true, true );
    if( !frm->wasCancelled() ) {
        ret = true;
        value = edit->toString();
    }
    delete frm;
    return ret;
}

void QDialogWithSaveAs::slotSaveAs()
{
    Xtring fnamedest = theGuiApp->getSaveFileName( _("Guardar texto"), Xtring::null,
                       "Ficheros de texto (*.txt);;Ficheros CSV (*.csv);;Todos los ficheros (*)",
                       this );
    if( !fnamedest.isEmpty() ) {
        if( FileUtils::writeFile(fnamedest, mText ) )
            theGuiApp->showOSD( fnamedest, _("Se ha guardado correctamente") );
        else
            theGuiApp->showOSD( fnamedest, _("Ha ocurrido un error al guardarlo.") );
    }
}

bool FrmBase::calcLayoutPositionOfWidget( QLayout *layout, const char *name, int &pos, int level )
{
    for( int l = 0; l < layout->count(); ++ l ) {
        if( level == 0 )
            pos = l;
        QLayoutItem *li = layout->itemAt( l );
        if( li->widget() ) {
//             _GONG_DEBUG_PRINT(10, Xtring::printf("Found widget %s at pos %d at level %d", li->widget()->name(), pos, level ) );
            if( strcmp( li->widget()->name(), name) == 0 ) {
                return true;
            }
        } else if( li->layout() ) {
            if( calcLayoutPositionOfWidget( li->layout(), name, pos, level + 1 ) )
                return true;
        }
    }
    pos = -1;
    return false;
}

} // Namespace gong
