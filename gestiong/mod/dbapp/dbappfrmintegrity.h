#ifndef _GONG_FRMINTEGRITY_H
#define _GONG_FRMINTEGRITY_H

/** @file dbappfrmintegrity.h A Form to check the integrity of the database
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

namespace gong {
#if 0
struct IntegrityListItemInfo
{
    dbTableDefinition *tabledef;
    dbFieldDefinition *fielddef;
    bool inView;
};

class IntegrityListItem: public QListWidgetItem
{
public:
    IntegrityListItem ( IntegrityListItemInfo &it, const QString & text, QListWidget * parent = 0 )
        : QListWidgetItem( text, parent ), mItemInfo(it) {}
    IntegrityListItemInfo &getItemInfo() {
        return mItemInfo;
    }
private:
    IntegrityListItemInfo mItemInfo;
};
#endif

class dbDefinition;

class FrmIntegrity : public FrmCustom
{
    Q_OBJECT

public:
    FrmIntegrity ( dbDefinition *db,
                   QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
    int exec();

    dbDefinition *pDbDefinition;
};

}

#endif // _GONG_FRMINTEGRITY_H

