#ifndef _GONG_FRMBASE_H
#define _GONG_FRMBASE_H

/** @file gongfrmbase.h Qt based Form
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include <QWidget>
#include <QMessageBox>
#include <gongxtring.h>
#include "gongguiapplication.h"
#include "gongguicontrols.h"

class QKeyEvent;
class QCloseEvent;
class QPushButton;

namespace gong {

class FrmBase: public QWidget
{
    Q_OBJECT
public:
    enum { NoButton = QMessageBox::NoButton, Yes=QMessageBox::Yes, No=QMessageBox::No, Cancel = QMessageBox::Cancel, YesToAll = QMessageBox::YesToAll, NoToAll = QMessageBox::NoToAll };

    FrmBase ( QWidget *parent=0, const Xtring &name = Xtring(), WidgetFlags f=0 );
    virtual ~FrmBase();
    virtual void closeEvent ( QCloseEvent *e ); // from QWidget
    virtual void keyPressEvent ( QKeyEvent *e ); // from QWidget
    virtual void showEvent(QShowEvent *event); // from QWidget
    virtual bool canClose();
    virtual void refresh();
    virtual void accept();
    virtual void justShown( bool firsttime );
    virtual Xtring getGeometry() const;
    virtual void setGeometry( const Xtring &geometry );
    virtual void focusInEvent( QFocusEvent *e); // from QWidget
    virtual void focusOutEvent( QFocusEvent *e); // from QWidget
    virtual void changeEvent ( QEvent * event ); // from QWidget

    Xtring getTitle() const {
        return ( const char * ) caption().local8Bit();
    }
    void setTitle( const Xtring &title ) {
        setCaption ( QString::fromUtf8 ( title.c_str() ) );
    }
    bool wasCancelled() const {
        return mWasCancelled;
    }
    int exec(bool centered = true);
    void showModalFor( QWidget *parent, bool centered, bool createclient );
    bool isModalFor( QWidget *parent ) const;
    QWidget *getRealParent() const {
        return pRealParent;
    }
    void setInitialFocus();
    void setWiseFocus(QWidget *w);
    QWidget *saveFocusWidget();
    QWidget *restoreFocusWidget();
    void alignLayout( QBoxLayout *layout, bool totheleft = true );
    QPushButton *getAcceptButton() const {
        return pushAccept;
    }
    QPushButton *getCancelButton() const {
        return pushCancel;
    }
    Xtring dumpChildren() const;

    enum MsgBoxType { information, question, warning, error };

    /** Shows a messagebox with ok button */
    static void msgOk ( QWidget *parent, const Xtring &text, MsgBoxType type = information );
    static void msgOk ( const Xtring &caption, const Xtring &text, MsgBoxType type, QWidget *parent = 0 );

    /** Shows a warning */
    static void msgWarning ( QWidget *parent, const Xtring &text )
    {
        msgOk( parent, text, warning );
    }
    static void msgWarning ( const Xtring &caption, const Xtring &text, QWidget *parent = 0  )
    {
        msgOk( caption, text, warning, parent );
    }

    /** Shows an error */
    static void msgError ( QWidget *parent, const Xtring &text, const Xtring &detailedError = Xtring::null );
    static void msgError ( const Xtring &caption, const Xtring &text,
                           const Xtring &detailedError = Xtring::null, QWidget *parent = 0 );

    /** Shows a large messagebox with ok button */
    static void msgOkLarge( QWidget *parent, const Xtring &text, const Xtring &largetext );
    static void msgOkLarge( const Xtring &caption, const Xtring &text,
                            const Xtring &largetext, QWidget *parent = 0 );

    /** Shows a messagebox with yes/no buttons */
    static bool msgYesNo ( const Xtring &caption, const Xtring &text,
                           bool defbutton=true, QWidget *parent = 0 );
    static bool msgYesNo ( QWidget *parent, const Xtring &text, bool defbutton=true );

    /** Shows a messagebox with yes/no/cancel buttons */
    static int msgYesNoCancel ( QWidget *parent, const Xtring &text, bool defbutton=true );
    static int msgYesNoCancel ( const Xtring &caption, const Xtring &text,
                                bool defbutton=true, QWidget *parent = 0 );

    static int msgYesNoAllCancel ( QWidget *parent, const Xtring &text, bool defbutton=true );
    static int msgYesNoAllCancel ( const Xtring &caption, const Xtring &text,
                                   bool defbutton=true, QWidget *parent = 0 );

    /** Shows a messagebox with yes/no buttons */
    static bool msgYesNoLarge ( QWidget *parent, const Xtring &text,
                                const Xtring &largeText, bool defbutton=true );
    static bool msgYesNoLarge ( const Xtring &caption, const Xtring &text,
                                const Xtring &largeText, bool defbutton=true, QWidget *parent = 0 );

    /** Shows a list of strings to choose from */
    static int msgXtringList ( QWidget *parent, const Xtring &text, const XtringList &strlist );
    static bool calcLayoutPositionOfWidget( QLayout *layout, const char *name, int &pos, int level );

    static bool inputBoxInt( int &value, const Xtring &prompt, const Xtring &title = Xtring::null, QWidget *parent = 0);
    static bool inputBoxString( Xtring &value, const Xtring &prompt, const Xtring &title = Xtring::null, QWidget *parent = 0);
    static bool inputBoxDouble( double &value, const Xtring &prompt, const Xtring &title = Xtring::null, QWidget *parent = 0);
    static bool inputBoxDate( Date &value, const Xtring &prompt, const Xtring &title = Xtring::null, QWidget *parent = 0);
    static bool inputBoxTime( Time &value, const Xtring &prompt, const Xtring &title = Xtring::null, QWidget *parent = 0);
    static bool inputBoxDateTime( DateTime &value, const Xtring &prompt, const Xtring &title = Xtring::null, QWidget *parent = 0);
    static bool inputBoxMoney( Money &value, const Xtring &prompt, const Xtring &title = Xtring::null, QWidget *parent = 0);
    static bool inputBoxText( Xtring &value, const Xtring &prompt, const Xtring &title = Xtring::null, QWidget *parent = 0);

protected slots:
    void pushAccept_clicked();
    virtual void cancel(); // from QWidget

protected:
    bool mWasCancelled, mShownYet, mClosingExternally;
    QWidget *pShowModalFor, *pRealParent, *pFocusWidget, *pSavedFocusWidget;
    QPushButton *pushCancel, *pushAccept;
    QEventLoop *pEventLoop;
};

class QDialogWithSaveAs: public QDialog
{
    Q_OBJECT
public:
    QDialogWithSaveAs( const Xtring &text, QWidget *parent, const char *name, bool modal )
        : QDialog( parent, name, modal ), mText(text) {}
public slots:
    void slotSaveAs();
protected:
    const Xtring &mText;
};

} // Namespace

#endif // _GONG_FRMBASE_H
