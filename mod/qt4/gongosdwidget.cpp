/*
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.
*/

/*
  Provides an interface to a plain QWidget, which is independent of KDE (bypassed to X11)
  begin:     Fre Sep 26 2003
  copyright: (C) 2003 by Christian Muehlhaeuser
  copyright: (C) 2004 by Michael Goettsche
*/

#include "gongosdwidget.h"
#include "gongguiapplication.h"

#include <QDesktopWidget>
#include <QBitmap>
#include <QPainter>
#include <QRegExp>

#include <X11/Xlib.h> //reposition()

namespace gong {

OSDWidget::OSDWidget( QWidget *parent, const char *name )
    : QWidget( parent, name, Qt::WNoAutoErase | Qt::WStyle_Customize | Qt::WX11BypassWM | Qt::WStyle_StaysOnTop | Qt::WStyle_Tool )
    , m_duration( 5000 )
    , m_shadow( false )
    , m_alignment( Right )
    , m_screen( 0 )
    , m_y( MARGIN )
    , m_dirty( false )
{
    setFocusPolicy( Qt::NoFocus );
    setBackgroundMode( Qt::NoBackground );
    unsetColors();

    connect( &timer,     SIGNAL( timeout() ), SLOT( hideOSD() ) );
    connect( &timerMin,  SIGNAL( timeout() ), SLOT( minReached() ) );
}


void OSDWidget::renderOSDText( const Xtring &caption, const Xtring &text )
{
    static QBitmap mask;
    //This is various spacings and margins, based on the font to look "just right"
#if QT_VERSION >= 0x040700
    QFont textFont( "Verdana", 12, QFont::Cursive );
#else
    QFont textFont( "Verdana", 12 );
#endif
    QFontMetrics textFm( textFont );
    const uint METRIC = textFm.width( 'x' );

    // Set a sensible maximum size, don't cover the whole desktop or cross the screen
    QSize max = QApplication::desktop()->screen( m_screen )->size() - QSize( MARGIN*2 + METRIC*2, 100 );
    QFont titleFont( "Verdana", 14, QFont::Bold );
    QFontMetrics titleFm( titleFont );

    // The title cannnot be taller than one line
    // AlignAuto = align Arabic to the right, etc.
    QRect titleRect = titleFm.boundingRect( 0, 0, max.width() - METRIC, titleFm.height(), Qt::AlignLeft, toGUI(caption) );
    // The osd cannot be larger than the screen
    QRect textRect = textFm.boundingRect( 0, 0, max.width(), max.height(), Qt::AlignLeft | Qt::TextWordWrap, toGUI(text) );

    if ( textRect.width() < titleRect.width() )
        textRect.setWidth( titleRect.width() );

    //this should still be within the screen bounds
    textRect.addCoords( 0, 0, METRIC*4, titleRect.height() + METRIC );

    osdBuffer.resize( textRect.size() );
    mask.resize( textRect.size() );

    // Start painting!
    QPainter bufferPainter( &osdBuffer );
    QPainter maskPainter( &mask );

    // Draw backing rectangle
    const uint xround = 0; // (METRIC * 200) / textRect.width();
    const uint yround = 0; // (METRIC * 200) / textRect.height();

    bufferPainter.setPen(  palette().toolTipText() );
    bufferPainter.setBrush( palette().toolTipBase() );
    bufferPainter.drawRoundRect( textRect, xround, yround );
    bufferPainter.setFont( textFont );

    const uint w = textRect.width()  - 1;
    const uint h = textRect.height() - 1;

    // Draw the text shadow
    if ( m_shadow ) {
        bufferPainter.setPen( backgroundColor().dark( 175 ) );
        bufferPainter.drawText( METRIC + 3, (METRIC/2) + titleFm.height() + 1, w, h, Qt::AlignLeft | Qt::TextWordWrap, toGUI(text) );
    }

    // Draw the text
    bufferPainter.setPen( foregroundColor() );
    bufferPainter.drawText( METRIC, (METRIC/2) + titleFm.height() - 1, w, h, Qt::AlignLeft | Qt::TextWordWrap, toGUI(text) );

    // Draw the title text
    bufferPainter.setFont( titleFont );
    bufferPainter.drawText( METRIC * 2, (METRIC/2), w, h, Qt::AlignLeft, toGUI( caption ) );

#if 0
    mask.fill( Qt::black );
    // Masking for transparency
    maskPainter.setBrush( Qt::white );
    maskPainter.drawRoundRect( textRect, xround, yround );
    setMask( mask );
#endif

    //do last to reduce noticeable change when showing multiple OSDs in succession
    reposition( textRect.size() );

    m_currentText = text;
    m_dirty = false;

    update();
}

void OSDWidget::hideOSD()
{
    hide();
    m_currentCaption.clear();
}


void OSDWidget::showOSD( const Xtring &caption, const Xtring &text, bool sticky )
{
    if ( isEnabled() ) {

        QString plaintext = toGUI(text);
        plaintext.replace(QRegExp("</?(?:font|a|b|i)\\b[^>]*>"), QString(""));
        plaintext.replace(QString("&lt;"), QString("<"));
        plaintext.replace(QString("&gt;"), QString(">"));
        plaintext.replace(QString("&amp;"), QString("&"));

        Xtring first_caption =  m_captions.size() ? m_captions.front() : m_currentCaption;
        Xtring first_text = m_texts.size() ? m_texts.front() : m_currentText;
        if ( !timerMin.isActive() ) {
            if( sticky && (caption == first_caption || caption.isEmpty() ) )
                m_currentText += "\n" + fromGUI( plaintext );
            else {
                m_currentCaption = caption;
                m_currentText = fromGUI(plaintext);
            }
            m_dirty = true;
            show();
        } else { // queue
            if( sticky && ( caption == first_caption || caption.isEmpty() )
                    && !first_text.isEmpty() ) {
                if( !m_captions.size() )
                    m_captions << (caption.isEmpty() ? first_caption : caption);
                if( m_texts.size() ) {
                    m_texts.erase( m_texts.begin() );
                    m_texts.insert( m_texts.begin(), first_text + "\n" + fromGUI( plaintext ) );
                } else {
                    m_texts << m_currentText + "\n" + fromGUI( plaintext );
                }
            } else {
                m_captions << caption;
                m_texts << fromGUI( plaintext );
            }
        }
    }
}


void OSDWidget::minReached() //SLOT
{
    if ( m_captions.size() ) {
        renderOSDText( m_captions.front(), m_texts.front() );
        if( m_texts.size() )
            m_texts.erase( m_texts.begin() );
        if( m_captions.size() )
            m_captions.erase( m_captions.begin() );
        if( m_duration ) // timerMin is still running
            timer.start( m_duration, TRUE );
    }
    else timerMin.stop();
}


void OSDWidget::setDuration( int ms )
{
    m_duration = ms;

    if( !m_duration ) timer.stop();
}

void OSDWidget::setFont( QFont newFont )
{
    QWidget::setFont( newFont );
    refresh();
}

void OSDWidget::setShadow( bool shadow )
{
    m_shadow = shadow;
    refresh();
}

void OSDWidget::setTextColor( const QColor &newColor )
{
    setPaletteForegroundColor( newColor );
    refresh();
}

void OSDWidget::setBackgroundColor( const QColor &newColor )
{
    setPaletteBackgroundColor( newColor );
    refresh();
}

void OSDWidget::unsetColors()
{
    setPaletteForegroundColor( palette().toolTipText() );
    setPaletteBackgroundColor( palette().toolTipBase() );
    refresh();
}

void OSDWidget::setOffset( int /*x*/, int y )
{
    //m_offset = QPoint( x, y );
    m_y = y;
    reposition();
}

void OSDWidget::setAlignment( Alignment a )
{
    m_alignment = a;
    reposition();
}

void OSDWidget::setScreen( uint screen )
{
    const uint n = QApplication::desktop()->numScreens();
    m_screen = (screen >= n) ? n-1 : (int)screen;
    reposition();
}


bool OSDWidget::event( QEvent *e )
{
    switch( e->type() )
    {
    case QEvent::Paint:
        bitBlt( this, 0, 0, &osdBuffer );
        return TRUE;

    default:
        return QWidget::event( e );
    }
}

void OSDWidget::mousePressEvent( QMouseEvent* )
{
    hide();
}

void OSDWidget::show()
{
    if ( m_dirty ) renderOSDText( m_currentCaption, m_currentText );

    QWidget::show();

    if ( m_duration ) //duration 0 -> stay forever
    {
        timer.start( m_duration, TRUE ); //calls hide()
        timerMin.start( 150 ); //calls minReached()
    }
}

void OSDWidget::refresh()
{
    if ( isVisible() )
    {
        //we need to update the buffer
        renderOSDText( m_currentCaption, m_currentText );
    }
    else m_dirty = true; //ensure we are re-rendered before we are shown
}

void OSDWidget::reposition( QSize newSize )
{
    if( !newSize.isValid() ) newSize = size();

    QPoint newPos( MARGIN, m_y );
    const QRect screen = QApplication::desktop()->screenGeometry( m_screen );

    //TODO m_y is the middle of the OSD, and don't exceed screen margins

    switch ( m_alignment ) {
    case Left:
        break;

    case Right:
        newPos.rx() = screen.width() - MARGIN - newSize.width();
        break;

    case Center:
        newPos.ry() = (screen.height() - newSize.height()) / 2;

        //FALL THROUGH

    case Middle:
        newPos.rx() = (screen.width() - newSize.width()) / 2;
        break;
    }

    //ensure we don't dip below the screen
    if( newPos.y()+newSize.height() > screen.height()-MARGIN ) newPos.ry() = screen.height()-MARGIN-newSize.height();

    // correct for screen position
    newPos += screen.topLeft();

    //ensure we are painted before we move
    if( isVisible() ) paintEvent( 0 );

    //fancy X11 move+resize, reduces visual artifacts
    XMoveResizeWindow( x11Display(), winId(), newPos.x(), newPos.y(), newSize.width(), newSize.height() );
}

} // namespace gong
