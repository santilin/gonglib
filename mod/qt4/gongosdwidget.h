#ifndef _GONG_OSDWIDGET_H
#define _GONG_OSDWIDGET_H

/** @file gongosdwidget.h Qt based osd widget
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

/*
  Provides an interface to a plain QWidget, which is independent of KDE (bypassed to X11)
  begin:     Fre Sep 26 2003
  copyright: (C) 2003 by Christian Muehlhaeuser
  copyright: (C) 2004 by Michael Goettsche
*/

#include <QPixmap> //stack allocated
#include <QTimer>  //stack allocated
#include <QWidget> //baseclass
#include <gongxtring.h>

class QFont;
class QTimer;

namespace gong {

class OSDWidget : public QWidget
{
    Q_OBJECT
public:
    enum Alignment { Left, Middle, Center, Right };

    OSDWidget(QWidget *parent = 0, const char *name = "osd");
    void setDuration(int ms);
    void setFont(QFont newfont);
    void setShadow(bool shadow);
    void setTextColor(const QColor &newcolor);
    void setBackgroundColor(const QColor &newColor);
    void setOffset( int x, int y );
    void setAlignment(Alignment);
    void setScreen(uint screen);
    void setText(const Xtring &caption, const Xtring &text )
    {
        m_currentText = text;
        m_currentCaption = caption;
        refresh();
    }

    void unsetColors();

    int screen()    {
        return m_screen;
    }
    int alignment() {
        return m_alignment;
    }
    int y()         {
        return m_y;
    }

public slots:
    void showOSD(const Xtring &caption, const Xtring &text, bool preemptive=false );
    void hideOSD();

protected slots:
    void minReached();

protected:
    /* render text into osdBuffer */
    void renderOSDText(const Xtring &caption, const Xtring &text);
    void mousePressEvent( QMouseEvent* );
    bool event(QEvent*);

    void show();

    /* call to reposition a new OSD text or when position attributes change */
    void reposition( QSize newSize = QSize() );

    /* called after most set*() calls to update the OSD */
    void refresh();

    static const int MARGIN = 15;

    int         m_duration;
    QTimer      timer;
    QTimer      timerMin;
    QPixmap     osdBuffer;
    XtringList  m_captions, m_texts;
    Xtring      m_currentCaption, m_currentText;
    bool        m_shadow;

    Alignment   m_alignment;
    int         m_screen;
    uint        m_y;

    bool m_dirty; //if dirty we will be re-rendered before we are shown
};


} // namespace

#endif
