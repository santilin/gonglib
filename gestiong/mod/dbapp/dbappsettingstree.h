#ifndef DBAPPSETTINGSTREE_H
#define DBAPPSETTINGSTREE_H

/** @file dbappsettingstree.h A widget to show a tree of settings
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include <QIcon>
#include <QTimer>
#include <QTreeWidget>
#include <gongsettings.h>

namespace gong {

class SettingsTree: public QTreeWidget
{
    Q_OBJECT

public:
    SettingsTree(QWidget *parent = 0);

    QSize sizeHint() const;

public slots:
    void setAutoRefresh(bool autoRefresh);
    void setFallbacksEnabled(bool enabled);
    void maybeRefresh();
    void refresh();
    void updateSetting(QTreeWidgetItem *item);

protected:
    bool event(QEvent *event);

private:
    void updateChildItems(QTreeWidgetItem *parent);
    QTreeWidgetItem *createItem(const Xtring &text, QTreeWidgetItem *parent,
                                int index);
    QTreeWidgetItem *childAt(QTreeWidgetItem *parent, int index);
    int childCount(QTreeWidgetItem *parent);
    int findChild(QTreeWidgetItem *parent, const Xtring &text, int startIndex);
    void moveItemForward(QTreeWidgetItem *parent, int oldIndex, int newIndex);

    QTimer mRefreshTimer;
    bool autoRefresh;
    QIcon mGroupIcon;
    QIcon mKeyIcon;
};

} // namespace gong

#endif // DBAPPSETTINGSTREE_H
