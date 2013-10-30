#ifndef _GONG_GUINAMESPACE_H
#define _GONG_GUINAMESPACE_H

/** @file gongguinamespace.h
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
#include "gongdbfieldstyle.h"

namespace gong {

typedef Qt::WindowFlags WidgetFlags;

inline Qt::Alignment toGUI( dbFieldStyle::Alignment al )
{
    switch(al) {
    case dbFieldStyle::AlignAuto:
    case dbFieldStyle::AlignLeft:
        return Qt::AlignLeft;
    case dbFieldStyle::AlignRight:
        return Qt::AlignRight;
    case dbFieldStyle::AlignHCenter:
//		case dbFieldStyle::AlignCenter:
        return Qt::AlignCenter;
    case dbFieldStyle::AlignJustify:
        return Qt::AlignJustify;
    case dbFieldStyle::AlignTop:
        return Qt::AlignTop;
    case dbFieldStyle::AlignBottom:
        return Qt::AlignBottom;
    case dbFieldStyle::AlignVCenter:
//		case dbFieldStyle::AlignMiddle:
//		case dbFieldStyle::AlignVCenter:
    default:
        return Qt::AlignVCenter;
    }
}

inline QString toGUI ( const Xtring &str )
{
    return QString::fromUtf8 ( str.c_str() );
}

inline Xtring fromGUI ( const QString &str )
{
    return Xtring ( str.toLocal8Bit() );
}

inline Xtring fromGUI( const QByteArray &ba )
{
    return ba.toHex().data();
}

inline QByteArray toGUIByteArray( const Xtring &str )
{
    return QByteArray::fromHex( str.c_str() );
}

} // namespace

#endif // _GONG_GUINAMESPACE_H
