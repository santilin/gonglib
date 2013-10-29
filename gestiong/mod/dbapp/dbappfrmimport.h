#ifndef _GONG_DBAPPFRMIMPORT_H
#define _GONG_DBAPPFRMIMPORT_H

/** @file dbappfrmimport.h A Form to import a records
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
#include "dbappfrmimport_gui.h"
#include "dbappfugitimporter.h"

namespace gong {

typedef GuiTreeCheckItem<Xtring> ImportViewItem;

class FrmImport: public GuiFrmImport
{
    Q_OBJECT
public:
    FrmImport( QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
protected slots:
    virtual void next();  // from GuiWizardImport
    virtual void accept(); // from GuiWizardImport
    virtual void reject(); // from QDialog
private:
    XtringList getImportTablesList() const;
    FugitImporter::ImportOptions getImportOptions();
    void addMessage(const Xtring &message);
    bool mWorking;
};

} // namespace gong
#endif
