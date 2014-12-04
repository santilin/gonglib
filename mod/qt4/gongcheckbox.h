#ifndef _GONG_CHECKBOX_H
#define _GONG_CHECKBOX_H

/** @file gongcheckbox.h Qt based CheckBox
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include <QCheckBox>
#include <gongxtring.h>

namespace gong {

class CheckBox: public QCheckBox
{
public:
    CheckBox( QWidget *parent, const Xtring &name, const Xtring &caption );
    bool isEdited() const {
        return mEdited;
    }
    void setEdited(bool edited) {
        mEdited = edited;
    }
    bool isJustEdited() const {
        return mJustEdited;
    }
    void setJustEdited(bool justedited) {
        mJustEdited = justedited;
    }
    bool mustBeReadOnly() const {
        return mMustBeReadOnly;
    }
    void setMustBeReadOnly(bool must) {
        setEnabled(!(mMustBeReadOnly = must));
    }
    void setChecked( bool checked );
    bool isCheckingProgrammatically() const {
        return mCheckingProgrammatically;
    }
protected:
    virtual void keyPressEvent( QKeyEvent *e ); // from QWidget
    virtual void nextCheckState(); // from QAbstractButton
private:
    bool mMustBeReadOnly;
    bool mEdited, mJustEdited, mCheckingProgrammatically;
};

} // namespace gong

#endif // _GONG_CHECKBOX_H
