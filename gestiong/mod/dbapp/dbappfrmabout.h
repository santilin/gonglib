#ifndef _DBAPP_FRMABOUT_H
#define _DBAPP_FRMABOUT_H

/** @file dbappfrmabout.h The about form
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include <dbappfrmcustom.h>

class QPushButton;

namespace gong {

class FrmAbout: public FrmCustom
{
    Q_OBJECT

public:
    FrmAbout( QWidget* parent = 0, WidgetFlags fl = 0 );
    int exec();
protected slots:
    void button_clicked();
private:
    QPushButton *pushDirGlobalSettings;
    QPushButton *pushDirLocalSettings;
};

}

#endif // _DBAPP_FRMABOUT_H

