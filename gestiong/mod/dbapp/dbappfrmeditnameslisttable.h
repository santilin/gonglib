#ifndef _DBAPP_FRMEDITNAMESLIST_H_
#define _DBAPP_FRMEDITNAMESLIST_H_

/** @file dbappfrmeditnameslist.h A form for editing nameslist tables
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include "dbappnameslisttable.h"
#include "dbappfrmeditrecmaster.h"

namespace gong {

class FrmEditNamesListTable: public FrmEditRecMaster
{
public:
    FrmEditNamesListTable(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm=0,
                          EditMode editmode = DataTable::defaulteditmode,
                          dbApplication::EditFlags editflags = dbApplication::editNone,
                          QWidget *parent = 0, const char* name = 0, WidgetFlags fl = 0 );
protected:
    virtual void scatterFields(); // From FrmEditRec
    virtual void gatherFields();
    virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ve = 0);
    gong::EditBox *editCodigo;
    gong::EditBox *editNombre;
};

} // namespace gong

#endif // _DBAPP_FRMEDITNAMESLIST_H_
