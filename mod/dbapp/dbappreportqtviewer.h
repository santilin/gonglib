#ifndef _DBAPP_REPORTQTVIEWER_H
#define _DBAPP_REPORTQTVIEWER_H

/** @file dbappreportqtviewer.h A widget to show a record in collection of QPainters
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

/***************************************************************************
              mreportviewer.h  -  Kugar QT report viewer widget
              -------------------
    begin     : Fri Aug 13 1999
    copyright : (C) 1999 by Mutiny Bay Software
    email     : info@mutinybaysoftware.com
    copyright : (C) 2002 Alexander Dymo
    email     : cloudtemple@mksat.net
 ***************************************************************************/

#include "config.h"

#include <QResizeEvent>
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>
#include <QPaintEvent>
#include <Q3MainWindow>
#include <qwidget.h>
#include <qlayout.h>
#include <q3scrollview.h>
#include <qpushbutton.h>
#include <qpixmap.h>
#include <qpainter.h>
#include <q3paintdevicemetrics.h>
#include <q3progressdialog.h>
#include <q3mainwindow.h>

#include <Q3ToolBar>
#include <qaction.h>
#include <q3vbox.h>
#include <qworkspace.h>

#include "dbappreportqtoutput.h"
#include "dbappreportqtpagedisplay.h"

#define M_PROGRESS_DELAY 500            // Number of ms to delay progress dialog display

namespace gong {

class ReportQtViewer: public Q3MainWindow
{
    Q_OBJECT

public:
    ReportQtViewer( Output *output, bool autodelete = false,
                    QWidget *parent = 0, const char *name = 0, Qt::WFlags f = 0 );
    virtual ~ReportQtViewer();
    void setPageCollection( const QtPageCollection *pc );
    void clearReport();
    void setPageDimensions( int sizex, int sizey );
    void printWithDialog() {
        _print( true );
    }
    void printWithoutDialog() {
        _print( false );
    }
//    QSize sizeHint() const;

public slots:
    void slotPrint();
    void slotFirstPage();
    void slotNextPage();
    void slotPrevPage();
    void slotLastPage();
    void slotPdf();
	void slotEmail();

protected:
    Q3VBoxLayout* controlsLayout;
    Q3HBoxLayout* mainLayout;
    QPushButton *pushPrint, *pushFirst, *pushNext, *pushPrev, *pushLast, *pushPdf;
	Xtring mDestPdf;

protected:
    virtual void paintEvent( QPaintEvent *event );
    virtual void resizeEvent( QResizeEvent *event );
    void _print( bool with_dialog );
    Q3ScrollView *scroller;
    class ReportQtPageDisplay *display;

private:
    Q3ToolBar *m_fileToolbar;
    QAction *forwardaction, *backaction, *finishaction, *startaction, *printaction,
		*pdfaction, *emailaction;
    Q3VBox *view_back;
    QWorkspace *pWorkspace;

    Q3ProgressDialog* progress;
    const QtPageCollection *mPageCollection;
    int totalSteps;
    int mCurrentPage;
    void init();
    Output *mOutput;
    bool mAutodelete;
};

} // namespace

#endif // _DBAPP_REPORTQTVIEWER_H

