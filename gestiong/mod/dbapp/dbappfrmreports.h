#ifndef _DBAPP_DBAPPFRMREPORTS_H
#define _DBAPP_DBAPPFRMREPORTS_H

/** @file dbappfrmreports.h A form to show the reports of the application
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
#include "dbappfrmreports_gui.h"
#include "dbappreport.h"

namespace gong {

typedef GuiQ3TreeItem<stReportInfo> ReportsTreeItem;

class FrmReports : public GuiFrmReports
{
    Q_OBJECT

public:
    FrmReports( QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
    void show();
    void show(const dbRecord *fic, const Xtring &filter);

public slots:
    virtual void slotCurrentChanged(Q3ListViewItem *);
    virtual void accept();
    virtual void refresh(); // from FrmBase
    void pushFilter_clicked();
    void pushOrder_clicked();

private:
    void readAllReports();
    ReportsTreeItem *mSelectedItem;
    static bool mReportsRead;
};

}
#endif // _DBAPP_DBAPPFRMREPORTS_H

