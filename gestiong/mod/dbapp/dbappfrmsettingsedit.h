#ifndef _GONG_DBAPPFRMSETTINGSEDIT_H
#define _GONG_DBAPPFRMSETTINGSEDIT_H

/** @file dbappfrmsettingsedit.h A form to edit all the application settings
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include "gongfrmbase.h"
#include "dbappsettingstree.h"

namespace gong {

class FrmSettingsEdit: public FrmBase
{
    Q_OBJECT

public:
    FrmSettingsEdit( QWidget* parent = 0, WidgetFlags fl = 0 );
    int exec();

protected:
    virtual bool canClose(); // from FrmBase

private:
    QVBoxLayout *mainLayout, *settingsLayout;
};

}

#endif // _GONG_FRMVIEWEDIT_H

