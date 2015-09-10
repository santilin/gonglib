/*<<<<<COPYRIGHT*/
/*
 * RTK The report toolkit
 * Copyright (C) 2003 Francisco Santiago Capel Torres
 *
 * rtkqtreportviewer.cpp RTK standard Qt report viewer
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
 *
 */
/*>>>>>COPYRIGHT*/

// Some changes made by Tomeu Borrás Riera, Marzo 2005
#include "dbappdbapplication.h"
#include "dbappreportqtviewer.h"
#include "dbappfrmmailing.h"

#include <QPrinter>
#include <Q3ToolBar>
#include <QStatusBar>
#include <QMenu>
#include <QMenuBar>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QPrintDialog>
#include <QProgressDialog>
#include <Q3Frame>
#include <QPixmap>
#include <QMessageBox>
#include <gongguimainwindow.h>
#include "dbappreportqtpagedisplay.h"

namespace gong {

/** Constructor */
ReportQtViewer::ReportQtViewer( Output *output, bool autodelete, QWidget *parent,
                                const char *name,  WidgetFlags f )
    : Q3MainWindow(parent,name,f)
{
    Q_ASSERT( output );
    mOutput = output;
    mAutodelete = autodelete;
    init();
}


// Create the scrollview
void ReportQtViewer::init()
{
    view_back = new Q3VBox( this);
    view_back->setFrameStyle( Q3Frame::StyledPanel | Q3Frame::Sunken );
    setCentralWidget(view_back);
    scroller = new Q3ScrollView( view_back );

    // Get the current color palette
    QPalette p = palette();
    QColorGroup g = p.active();

    display = new ReportQtPageDisplay( scroller->viewport() );
    scroller->addChild( display );

    QIcon forwardIcon, backIcon, finishIcon, startIcon, printIcon, pdfIcon, emailIcon;

    printIcon =  QIcon::fromTheme("printer", QIcon(":/printer.png"));
    forwardIcon = QIcon::fromTheme("go-next-view-page", QIcon(":/go-next-view-page.png"));
    backIcon = QIcon::fromTheme("go-previous-view-page", QIcon(":/go-previous-view-page.png"));
    startIcon = QIcon::fromTheme("go-first-view-page", QIcon(":/go-first-view-page.png"));
    finishIcon = QIcon::fromTheme("go-last-view-page", QIcon(":/go-last-view-page.png"));
    pdfIcon = QIcon::fromTheme("application-pdf", QIcon(":/application-pdf.png"));
	emailIcon = QIcon::fromTheme("mail-send", QIcon(":/mail-send.png"));

    QString s_next = _("Next");
    forwardaction = new QAction(forwardIcon, s_next, this);
    forwardaction->setStatusTip(s_next);
    forwardaction->setWhatsThis(s_next);
    connect(forwardaction, SIGNAL(activated()), this, SLOT(slotNextPage()));

    QString s_previous = _("Previous");
    backaction = new QAction(backIcon, s_previous, this);
    backaction->setStatusTip(s_previous);
    backaction->setWhatsThis(s_previous);
    connect(backaction, SIGNAL(activated()), this, SLOT(slotPrevPage()));

    QString s_last = _("Last");
    finishaction = new QAction(finishIcon, s_last, this);
    finishaction->setStatusTip(s_last);
    finishaction->setWhatsThis(s_last);
    connect(finishaction, SIGNAL(activated()), this, SLOT(slotLastPage()));

    QString s_first = _("First");
    startaction = new QAction(startIcon, s_first, this);
    startaction->setStatusTip(s_first);
    startaction->setWhatsThis(s_first);
    connect(startaction, SIGNAL(activated()), this, SLOT(slotFirstPage()));

    QString s_print= _("Print");
    printaction = new QAction(printIcon, s_print, this);
    printaction->setStatusTip(s_print);
    printaction->setWhatsThis(s_print);
    connect(printaction, SIGNAL(activated()), this, SLOT(slotPrint()));

    QString s_pdf= _("PDF");
    pdfaction = new QAction(pdfIcon, s_pdf, this);
    pdfaction->setStatusTip(s_pdf);
    pdfaction->setWhatsThis(s_pdf);
    connect(pdfaction, SIGNAL(activated()), this, SLOT(slotPdf()));

    QString s_email= _("correo-e");
    emailaction = new QAction(emailIcon, s_email, this);
    emailaction->setStatusTip(s_email);
    emailaction->setWhatsThis(s_email);
    connect(emailaction, SIGNAL(activated()), this, SLOT(slotEmail()));
	
	
    /// Creamos un toolbox
    m_fileToolbar = new Q3ToolBar(this, "Navigation");
    m_fileToolbar->addSeparator();
    startaction->addTo(m_fileToolbar);
    finishaction->addTo(m_fileToolbar);
    backaction->addTo(m_fileToolbar);
    forwardaction->addTo(m_fileToolbar);
    printaction->addTo(m_fileToolbar);
    pdfaction->addTo(m_fileToolbar);
    emailaction->addTo(m_fileToolbar);

    m_fileToolbar->setLabel("Navigation");
    m_fileToolbar->show();


    statusBar()->show();
    statusBar()->message(_("Ready."));


    QMenu *pHelpMenu=new QMenu();
    pHelpMenu->insertSeparator();

//  menuBar()->insertItem(_("&Ayuda"), pHelpMenu);

}

/** Destructor */
ReportQtViewer::~ReportQtViewer()
{
    clearReport();
    if ( mAutodelete )
        delete mOutput;
}

/** Report viewer's paint event */
void ReportQtViewer::paintEvent( QPaintEvent* ) {
    m_fileToolbar->show();
}

/** Report viewer's resize event */
void ReportQtViewer::resizeEvent( QResizeEvent* ) {
}

/** Clears the report's page collection */
void ReportQtViewer::clearReport() {
    // Hide the display
    display->hide();
}


/** Shows the first page in the report */
void ReportQtViewer::slotFirstPage()
{
    QPicture * page;

    if ( mPageCollection == 0 )
        return ;

    if ( ( page = mPageCollection->at( mCurrentPage = 0 ) ) != 0 ) {
        display->setPage( page );
        display->repaint();
    }
}

/** Shows the next page in the report */
void ReportQtViewer::slotNextPage()
{
    QPicture *page;

    if ( mPageCollection == 0
            || mCurrentPage == (int)(mPageCollection->size()) - 1 )
        return ;

    if ( ( page = mPageCollection->at( ++mCurrentPage ) ) != 0 ) {
        display->setPage( page );
        display->repaint();
    } else
        mCurrentPage--;
}

/** Shows the prevoius page in the report */
void ReportQtViewer::slotPrevPage()
{
    QPicture * page;

    if ( mPageCollection == 0 || mCurrentPage == 0 )
        return ;

    if ( ( page = mPageCollection->at( --mCurrentPage ) ) != 0 ) {
        display->setPage( page );
        display->repaint();
    } else
        mCurrentPage++;
}

/** Shows the last page in the report */
void ReportQtViewer::slotLastPage()
{
    QPicture * page;

    if ( mPageCollection == 0 )
        return ;

    if ( ( page = mPageCollection->at( mPageCollection->size() - 1 ) ) != 0 ) {
        mCurrentPage = mPageCollection->size() - 1;
        display->setPage( page );
        display->repaint();
    }
}


// Return the preferred size.

// QSize ReportQtViewer::sizeHint() const
// {
//     return QSize(800,600); // scroller -> sizeHint();
// }

void ReportQtViewer::setPageCollection( const QtPageCollection *pc )
{
    mPageCollection = pc;
    display->show();
}


void ReportQtViewer::setPageDimensions( int sizex, int sizey )
{
    display->setPageDimensions( QSize( sizex, sizey ) );
}

void ReportQtViewer::slotPrint()
{
    _print( true );
}


void ReportQtViewer::_print(bool with_dialog)
{
    if ( !mPageCollection )
        return ;
    // Get the page count
    int cnt = mPageCollection->size();

    // Check if there is a report or any pages to print
    if ( cnt == 0 ) {
        QMessageBox::critical( this, PACKAGE, "There are no pages in the report to print.",
                               QMessageBox::Ok, Qt::NoButton, Qt::NoButton );
        return ;
    }

    // Set the printer dialog
    QPrinter *printer = new QPrinter();
    printer->setPageSize( QPrinter::A4 );
    //  printer->setOrientation((QPrinter::Orientation)report->pageOrientation());
    printer->setMinMax( 1, cnt );
    printer->setFromTo( 1, cnt );
    printer->setFullPage( true );

    QPrintDialog dlg( printer );
    if( mOutput->pageOrientation() == Landscape )
        printer->setOrientation(QPrinter::Landscape);
    if( with_dialog ) {
        if( dlg.exec() != QDialog::Accepted )
            return;
    } else {
        dlg.show();
        dlg.close();
    }
    QPicture *page;
    QPainter painter;
    bool printRev;

    DBAPP->waitCursor();
    // Check the order we are printing the pages
    if ( printer->pageOrder() == QPrinter::FirstPageFirst )
        printRev = false;
    else
        printRev = true;

    // Get the count of pages and copies to print
    int printFrom, printTo;
    switch( dlg.printRange() ) {
    case QAbstractPrintDialog::AllPages:
        printFrom = 0;
        printTo = mPageCollection->size();
        break;
    case QAbstractPrintDialog::Selection:
#if QT_VERSION >= 0x040700
    case QAbstractPrintDialog::CurrentPage:
#endif
        printFrom = printTo = mCurrentPage;
        break;
    case QAbstractPrintDialog::PageRange:
        printFrom = dlg.fromPage() - 1;
        printTo = dlg.toPage();
    }
    int printCnt = ( printTo - printFrom );
    int printCopies = printer->numCopies();
    int totalSteps = printCnt * printCopies;
    int currentStep = 1;

    // Set copies to 1, QPrinter copies does not appear to work ...
    printer->setNumCopies( 1 );

    // Setup the progress dialog
    QProgressDialog progress( _("Imprimiendo informe..."), _("Cancelar"), 1, totalSteps, this );
    progress.show();
    progress.setMinimumDuration( M_PROGRESS_DELAY );
    //		QObject::connect(&progress, SIGNAL(cancelled()), this, SLOT(slotCancelPrinting()));
    progress.setValue( 0 );

    // Start the printer
    painter.begin( printer );

    // Print each copy
    for ( int j = 0; j < printCopies; j++ ) {
        // Print each page in the collection
        for ( int i = printFrom ; i < printTo; i++, currentStep++ ) {
            if ( !printer->aborted() ) {
                progress.setValue( currentStep );

                if ( printRev )
                    page = mPageCollection->at( ( printCnt == 1 ) ? i : ( printCnt - 1 ) - i );
                else
                    page = mPageCollection->at( i );

                page->play( &painter );
                if ( i < printTo -1 )
                    printer->newPage();
            } else {
                j = printCopies;
                break;
            }
        }
        if ( j < printCopies - 1 )
            printer->newPage();
    }

    // Cleanup printing
    painter.end();
    delete printer;
    DBAPP->resetCursor();
}

void ReportQtViewer::slotPdf()
{
    if ( !mPageCollection )
        return ;
    // Get the page count
    int cnt = mPageCollection->size();

    // Check if there is a report or any pages to print
    if ( cnt == 0 ) {
        FrmBase::msgError( this, _("El informe no contiene ninguna página") );
        return;
    }

    mDestPdf = GuiApplication::getSaveFileName( _("Guardar informe como PDF"),
                       fromGUI(caption()),
                       "Ficheros PDF (*.pdf);;Todos los ficheros (*)", this );
    if( !mDestPdf.isEmpty() ) {
        DBAPP->waitCursor( true );
        QPrinter *printer = new QPrinter();
        printer->setPageSize( QPrinter::A4 );
        //  printer->setOrientation((QPrinter::Orientation)report->pageOrientation());
        printer->setMinMax( 1, cnt );
        printer->setFromTo( 1, cnt );
        printer->setOutputFormat( QPrinter::PdfFormat );
        printer->setOutputFileName( mDestPdf.c_str() );
        printer->setFullPage( true );
        printer->setNumCopies( 1 );
        int printCopies = 1;
        int totalSteps = cnt * printCopies;
        QPicture *page;
        QPainter painter;

        // Setup the progress dialog
        QProgressDialog progress( _("Imprimiendo a PDF..."),
                                  _("Cancelar"), 1, totalSteps, this );
        progress.setWindowModality(Qt::WindowModal);
        progress.setMinimumDuration( M_PROGRESS_DELAY );
        //		QObject::connect(&progress, SIGNAL(cancelled()), this, SLOT(slotCancelPrinting()));
        progress.setVisible( true );
        progress.setValue( 0 );

        // Start the printer
        painter.begin( printer );

        int currentStep = 1;
        // Print each copy
        for ( int j = 0; j < printCopies; j++ ) {
            // Print each page in the collection
            for ( int i = 0; i < cnt; i++, currentStep++ ) {
                if ( !printer->aborted() ) {
                    progress.setValue( currentStep );
                    page = mPageCollection->at( i );
                    page->play( &painter );
                    if ( i < cnt - 1 )
                        printer->newPage();
                } else {
                    j = printCopies;
                    break;
                }
            }
            if ( j < printCopies - 1 )
                printer->newPage();
        }
		painter.end();
        delete printer;
        DBAPP->resetCursor();
    }
}


void ReportQtViewer::slotEmail()
{
	slotPdf(); // crea el pdf en el fichero mDestPdf
	if( !mDestPdf.isEmpty() ) {
		gong::FrmMailing *frmmailing = new gong::FrmMailing("CONTACTO", "CONTACTO_ID");
		frmmailing->setAttachement(mDestPdf, "application/pdf");
		DBAPP->getMainWindow()->createClient( frmmailing );
	}

}


} // namespace gong




