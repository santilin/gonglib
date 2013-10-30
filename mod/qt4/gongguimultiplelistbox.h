#ifndef _GONG_GUIMULTIPLELISTBOX_H
#define _GONG_GUIMULTIPLELISTBOX_H

/** @file gongguimultiplelistbox.h Qt based Multiple list box
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include <QListWidget>
#include <gongxtring.h>

namespace gong {

class MultipleListBoxItem: public QListWidgetItem
{
public:
    MultipleListBoxItem( const Xtring &id, const Xtring &text,
                         QListWidget *parent = 0, bool checked = false );
    MultipleListBoxItem( long id, const Xtring &text,
                         QListWidget *parent = 0, bool checked = false );
    Xtring &getId() {
        return mId;
    }
    bool isChecked() const;

private:
    Xtring mId;
};

class MultipleListBox: public QListWidget
{
public:
    MultipleListBox(QWidget *parent);
    void addItem( long id, const Xtring &text, bool checked = false);
    void addItem( const Xtring &id, const Xtring &text, bool checked = false);
    int getCheckedIds(XtringList &idslist) const;
    int getCheckedIds(List<long> &idslist) const;

};

} // namespace gongxtring

#endif //_GONG_GUIMULTIPLELISTBOX_H
