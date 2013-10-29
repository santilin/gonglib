#ifndef _GONG_PUSHBUTTON_H
#define _GONG_PUSHBUTTON_H

/** @file gongpushbutton.h Qt based Push button
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */


#include <QPushButton>
#include <gongxtring.h>

namespace gong {

class PushButton: public QPushButton
{
public:
    PushButton( QWidget *parent, const Xtring &text );
};

}

#endif // _GONG_PUSHBUTTON
