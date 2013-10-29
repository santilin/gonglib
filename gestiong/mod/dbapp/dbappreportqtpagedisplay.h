#ifndef _DBAPP_REPORTQTPAGEDISPLAY_H
#define _DBAPP_REPORTQTPAGEDISPLAY_H

/** @file dbappreportqtpagedisplay.h A widget to show a page of a record in a QPainter
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
              mpagedisplay.h  -  Kugar page display widget
              -------------------
    begin     : Sun Aug 15 1999
    copyright : (C) 1999 by Mutiny Bay Software
    email     : info@mutinybaysoftware.com
 ***************************************************************************/


#include <QPaintEvent>
#include <QWidget>

namespace gong {

class ReportQtPageDisplay : public QWidget
{
public:
    /** Constructor */
    ReportQtPageDisplay(QWidget *parent=0, const char *name=0);
    /** Destructor */
    virtual ~ReportQtPageDisplay();

private:
    /** Page Widget's image buffer */
    QPixmap buffer;

public:
    /** Sets the page image */
    void setPage(QPicture* image);
    /** Sets the page display dimensions */
    void setPageDimensions(QSize size);
    QSize getPageDimensions();

    QSize sizeHint() const;
    QSizePolicy sizePolicy() const;

protected:
    /** Page widget's's paint event */
    virtual void paintEvent(QPaintEvent* event);
    virtual void keyPressEvent ( QKeyEvent *e ); // from QWidget
};

} // namespace gong

#endif  // _DBAPP_REPORTQTPAGEDISPLAY_H
