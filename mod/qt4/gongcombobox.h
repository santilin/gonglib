#ifndef _GONG_COMBOBOX_H
#define _GONG_COMBOBOX_H

/** @file gongcombobox.h Qt based ComboBox
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include <QComboBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <gongguiapplication.h>
#include <gongdbfielddefinition.h>

namespace gong {

class ComboBoxInt: public QComboBox
{
public:
    ComboBoxInt( const XtringList &captions, const IntList &values,
                 QWidget *parent, const Xtring &name, const Xtring &caption,
                 bool horizontal = true );
    ComboBoxInt( XtringList &captions, IntList &values,
                 QWidget *parent, const Xtring &name, const Xtring &caption,
                 bool horizontal = true );
    void setVisible( bool visible );
    void setText(const Xtring &caption);
    void setCurrentItemByValue(int value);
    int getCurrentItemValue() const {
        return getItemValue( currentItem() );
    }
    int getItemValue(int i) const;
    bool isEdited() const {
        return mEdited;
    }
    void setEdited(bool edited) {
        mEdited = edited;
    }
    bool isJustEdited() const {
        return mJustEdited;
    }
    void setJustEdited( bool justedited = true ) {
        mJustEdited = justedited;
    }
    bool insertItems();
    void insertItem(const Xtring &caption, int value, int index = -1);
    QBoxLayout *getLayout() const {
        return pLayout;
    }
    QLabel *getLabel() const {
        return pLabel;
    }
    bool mustBeReadOnly() const {
        return mMustBeReadOnly;
    }
    void setMustBeReadOnly(bool must) {
        setEnabled(!(mMustBeReadOnly = must));
    }
    bool isSettingProgrammatically() const {
        return mSettingProgrammatically;
    }
    Xtring toString() const {
        return fromGUI(currentText());
    }
    bool isNewItem() const;
    // Overloaded
    Xtring currentString() const {
        return fromGUI( currentText() );
    }

protected:
    virtual void keyPressEvent( QKeyEvent *e ); // from QWidget

private:
    XtringList mCaptions;
    IntList mValues;
    const XtringList &mRefCaptions;
    const IntList &mRefValues;
    QLabel *pLabel;
    QBoxLayout *pLayout;
    bool mHorizontal, mMustBeReadOnly, mEdited;
    bool mJustEdited, mSettingProgrammatically, mIsRef;
};


class ComboBoxXtring: public QComboBox
{
public:
    ComboBoxXtring( const XtringList &captions, const XtringList &values,
                    QWidget *parent, const Xtring &name, const Xtring &caption,
                    bool horizontal = true );
    ComboBoxXtring( XtringList &captions, XtringList &values,
                    QWidget *parent, const Xtring &name, const Xtring &caption,
                    bool horizontal = true );
    void setVisible( bool visible );
    void setText(const Xtring &caption);
    void setCurrentItemByValue(const Xtring &value);
    const Xtring &getCurrentItemValue() const {
        return getItemValue( currentItem() );
    }
    const Xtring &getItemValue(int i) const;
    bool isEdited() const {
        return mEdited;
    }
    void setEdited(bool edited) {
        mEdited = edited;
    }
    bool isJustEdited() const {
        return mJustEdited;
    }
    void setJustEdited( bool justedited = true ) {
        mJustEdited = justedited;
    }
    bool insertItems();
    void insertItem(const Xtring &caption, const Xtring &value, int index = -1);
    QBoxLayout *getLayout() const {
        return pLayout;
    }
    QLabel *getLabel() const {
        return pLabel;
    }
    bool mustBeReadOnly() const {
        return mMustBeReadOnly;
    }
    void setMustBeReadOnly(bool must) {
        setEnabled(!(mMustBeReadOnly = must));
    }
    bool isSettingProgrammatically() const {
        return mSettingProgrammatically;
    }
    Xtring toString() const {
        return fromGUI(currentText());
    }
    bool isNewItem() const;
    // Overloaded
    Xtring currentString() const {
        return fromGUI( currentText() );
    }

protected:
    virtual void keyPressEvent( QKeyEvent *e ); // from QWidget

private:
    XtringList mCaptions, mValues;
    const XtringList &mRefCaptions, &mRefValues;
    QLabel *pLabel;
    QBoxLayout *pLayout;
    bool mHorizontal, mMustBeReadOnly, mEdited;
    bool mJustEdited, mSettingProgrammatically, mIsRef;
};

} // namespace gong

#endif // _GONG_COMBOBOX_H
