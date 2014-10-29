/*<<<<<COPYRIGHT*/
/*
 * RTK The report toolkit
 * Copyright (C) 2003 Francisco Santiago Capel Torres

 * rtkqtpagedisplay.cpp RTK page display widget
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
/***************************************************************************
             mpagedisplay.cpp  -  Kugar page display widget
             -------------------
   begin     : Sun Aug 15 1999
   copyright : (C) 1999 by Mutiny Bay Software
   email     : info@mutinybaysoftware.com
***************************************************************************/

#include <QPaintEvent>
#include <QPainter>
#include <QPicture>
#include <gongdebug.h>
#include "dbappreportqtpagedisplay.h"

namespace gong {

/** Constructor */
ReportQtPageDisplay::ReportQtPageDisplay( QWidget *parent, const char *name )
    : QWidget( parent, name )
{
//    buffer.resize( 1, 1 );
}

/** Destructor */
ReportQtPageDisplay::~ReportQtPageDisplay()
{}

/** Sets the report page image */
void ReportQtPageDisplay::setPage( QPicture* image )
{
    buffer.fill( Qt::white );
    QPainter p( &buffer );
    image->play( &p );
}

/** Display object's paint event */
void ReportQtPageDisplay::paintEvent( QPaintEvent* /*event*/ )
{
    bitBlt( this, 0, 0, &buffer );
}

/** Sets the page display dimensions */
void ReportQtPageDisplay::setPageDimensions( QSize size )
{
    buffer.resize( size );
    resize( size );
}

QSize ReportQtPageDisplay::getPageDimensions()
{
    return buffer.size();
}


// Return the preferred size of the display.

QSize ReportQtPageDisplay::sizeHint() const
{
    return buffer.size() + QSize(100,100);
}


// Return the size policy.

QSizePolicy ReportQtPageDisplay::sizePolicy() const
{
    return QSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum);
}



void ReportQtPageDisplay::keyPressEvent(QKeyEvent* e)
{
    _GONG_DEBUG_TRACE(0);
    if ( e->state() == 0 && e->key() == Qt::Key_Escape )
    {
        close();
        e->accept();
        return;
    }
}
} // namespace gong
