#ifndef _GONG_Q3TREEITEM_H
#define _GONG_Q3TREEITEM_H

/** @file gongq3treeitem.h Qt3 based legacy tree item
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */


#include <q3listview.h>

/* Originalmente, este template era:
template<class T, item=QListViewItem>
class GuiQ3TreeItem : public item
para poder definir QListViewItem y QCheckListItem, pero puesto
que los constructores son diferentes, habria que especializar
todos los constructores, y puesto que la clase basicamente solo tiene
constructores, es lo mismo que crear un nuevo template
*/

template<class T>
class GuiQ3TreeItem : public Q3ListViewItem
{
public:
    GuiQ3TreeItem(Q3ListView *parent, const T &value, const QString &text=QString::null)
        : Q3ListViewItem(parent)
    {
        setText(0, text);
        mLevel = 0;
        mValue = value;
    }
    GuiQ3TreeItem(GuiQ3TreeItem<T> *parent, const T &value, const QString &text=QString::null)
        : Q3ListViewItem(parent)
    {
        setText(0, text);
        mLevel = parent->getLevel() + 1;
        mValue = value;
    }
    GuiQ3TreeItem(Q3ListView *parent, GuiQ3TreeItem<T> *after, const T &value, const QString &text=QString::null)
        : Q3ListViewItem(parent, after)
    {
        setText(0, text);
        mLevel = 0;
        mValue = value;
    }
    GuiQ3TreeItem(GuiQ3TreeItem<T> *parent, GuiQ3TreeItem<T> *after, const T &value, const QString &text=QString::null)
        : Q3ListViewItem(parent, after)
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
    GuiQ3TreeItem<T> *findItem(Q3ListView *root, const T &value)
    {
        GuiQ3TreeItem<T>* current = static_cast<GuiQ3TreeItem<T> *>(root->firstChild());
        while( current && current->getValue() != value ) {
            current = static_cast<GuiQ3TreeItem<T> *>(current->nextSibling());
        }
        return current;
    }

private:
    int mLevel;
    T mValue;
};


template<class T>
class GuiTreeCheckItem : public Q3CheckListItem
{
public:
    GuiTreeCheckItem(Q3ListView *parent, const T &value, const QString &text=QString::null,
                     Q3CheckListItem::Type tt = Q3CheckListItem::CheckBox)
        : Q3CheckListItem(parent, text, tt)
    {
        mLevel = 0;
        mValue = value;
    }
    GuiTreeCheckItem(GuiTreeCheckItem<T> *parent, const T &value, const QString &text=QString::null,
                     Q3CheckListItem::Type tt = Q3CheckListItem::CheckBox)
        : Q3CheckListItem(parent, text, tt)
    {
        mLevel = parent->getLevel() + 1;
        mValue = value;
    }
    GuiTreeCheckItem(Q3ListView *parent, GuiTreeCheckItem<T> *after, const T &value, const QString &text=QString::null,
                     Q3CheckListItem::Type tt = Q3CheckListItem::CheckBox)
        : Q3CheckListItem(parent, after, text, tt)
    {
        mLevel = 0;
        mValue = value;
    }
    GuiTreeCheckItem(GuiTreeCheckItem<T> *parent, GuiTreeCheckItem<T> *after, const T &value, const QString &text=QString::null,
                     Q3CheckListItem::Type tt = Q3CheckListItem::CheckBox)
        : Q3CheckListItem(parent, after, text, tt)
    {
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
    GuiTreeCheckItem<T> *findItem(Q3ListView *root, const T &value)
    {
        GuiTreeCheckItem<T>* current = static_cast<GuiTreeCheckItem<T> *>(root->firstChild());
        while( current && current->getValue() != value ) {
            current = static_cast<GuiTreeCheckItem<T> *>(current->nextSibling());
        }
        return current;
    }

private:
    int mLevel;
    T mValue;
};

#endif // GONGTREEITEM_H
