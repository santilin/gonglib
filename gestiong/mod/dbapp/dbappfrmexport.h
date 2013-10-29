#ifndef _GONG_DBAPPFRMEXPORT_H
#define _GONG_DBAPPFRMEXPORT_H

/** @file dbappfrmexport.h A Form to export records
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include <gongq3treeitem.h>
#include "dbappfrmexport_gui.h"

namespace gong {

typedef GuiTreeCheckItem<Xtring> ExportViewItem;

class FrmExport : public GuiFrmExport
{
    Q_OBJECT

public:
    FrmExport( QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );

protected slots:
    void accept();
private:
    void doExport(ExportViewItem *current, std::ofstream &, const Xtring &tablename);
};

} // namespace gong
#endif // _GONG_DBAPPFRMEXPORT_H

