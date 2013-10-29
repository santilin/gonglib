#ifndef _GONG_TREEITEM_H
#define _GONG_TREEITEM_H

/** @file gongtreeitem.h Qt based tree item
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include <QTreeWidget>

template<class T>
class GuiTreeItem : public QTreeWidgetItem
{
public:
    GuiTreeItem(QTreeWidget *parent, const T &value, const QString &text=QString::null)
        : QTreeWidgetItem(parent)
    {
        setText(0, text);
        mLevel = 0;
        mValue = value;
    }
    GuiTreeItem(GuiTreeItem<T> *parent, const T &value, const QString &text=QString::null)
        : QTreeWidgetItem(parent)
    {
        setText(0, text);
        mLevel = parent->getLevel() + 1;
        mValue = value;
    }
    GuiTreeItem(QTreeWidget *parent, GuiTreeItem<T> *after, const T &value, const QString &text=QString::null)
        : QTreeWidgetItem(parent, after)
    {
        setText(0, text);
        mLevel = 0;
        mValue = value;
    }
    GuiTreeItem(GuiTreeItem<T> *parent, GuiTreeItem<T> *after, const T &value, const QString &text=QString::null)
        : QTreeWidgetItem(parent, after)
    {
        setText(0, text);
        mLevel = parent->getLevel() + 1;
        mValue = value;
    }
    const T &getValue() const
    {
        return mValue;
    }
    const int getLevel() const
    {
        return mLevel;
    }
    GuiTreeItem<T> *findItem(QTreeWidget *root, const T &value)
    {
        GuiTreeItem<T>* current = static_cast<GuiTreeItem<T> *>(root->topLevelItem(0));
        while( current && current->getValue() != value ) {
            current = static_cast<GuiTreeItem<T> *>(current->nextSibling());
        }
        return current;
    }

private:
    int mLevel;
    T mValue;
};


#endif // GONGTREEITEM_H
