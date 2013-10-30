#ifndef _GONG_LINEEDIT_H
#define _GONG_LINEEDIT_H

/** @file gonglineedit.h Qt based Line edit
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include <qwidget.h>
#include <qlineedit.h>
#include <qlabel.h>
//Added by qt3to4:
#include <QKeyEvent>
#include <QFocusEvent>

#include <gongxtring.h>
#include <gongvariant.h>

namespace gong {

class LineEdit : public QLineEdit
{
    Q_OBJECT

public:
    LineEdit( QWidget *parent, const Xtring &name,
              Variant::Type valuetype = Variant::tString,
              Xtring format=Xtring(), Xtring mask=Xtring(), char blankchar=' ' );
    ~LineEdit();

    void setBlankChar(char blankchar);
    char getBlankChar() const;

    void setValueType(Variant::Type valuetype);
    Variant::Type getValueType() const;

    void setMask(const Xtring &mask);
    Xtring getMask() const;

    void setFormat(const Xtring &format);
    Xtring getFormat() const;

    void setWidthInChars(int chars);
    void setCursorAtDecPoint();
    void setSelectedOnEntry( bool selected = true ) {
        mSelectedOnEntry = selected;
    }
    bool selectedOnEntry() const {
        return mSelectedOnEntry;
    }

    Xtring toString() const;
    Variant toVariant() const;
    int toInt() const;
    bool toBool() const;
    double toDouble() const;
    Money toMoney() const;
    DateTime toDateTime() const;
    Date toDate() const;
    Time toTime() const;
    Variant getPreviousValue() {
        return mPreviousValue;
    }

    void setText(bool val);
    void setText(int val);
    void setText(double val);
    void setText(const Money &val);
    void setText(const DateTime &val);
    void setText(const Date &val);
    void setText(const Time &val);
    void setText(const Variant &);
    void setText(const Xtring &);
    void setText(const char *);

    void setEdited( bool edited = true);
    bool isEdited() const;
    bool isJustEdited() const;
    void setJustEdited( bool justedited = true );
    void setCancelling() {
        mCancelling = true;
    }
    void emitValidate( bool *isvalid );

    static void setSpecialControlKeys(const Xtring &keys) {
        mSpecialControlKeys = keys;
    }
    static Xtring getSpecialControlsKey() {
        return mSpecialControlKeys;
    }

signals:
    void validate(QWidget *sender, bool *isvalid);
    void specialControlKeyPressed(QWidget *sender, char key);

public slots:
    virtual void setReadOnly(bool);
    void cursorPositionChanged ( int old, int _new );

protected:
    virtual void keyPressEvent( QKeyEvent *e ); // from QLineEdit
    virtual void focusOutEvent( QFocusEvent *e); // from QWidget
    virtual void focusInEvent( QFocusEvent *e); // from QWidget
    bool calcValue();
    void setTextNoValidate(const Xtring &text);

    static int mGlobalValidating, mLastGlobalValidating;
    bool mValidating, mGettingFocus;

private:
    Formatter *mMaskFormatter;
    Variant::Type mValueType;
    Xtring mFormat, mMask;
    Variant mValue, mPreviousValue;
    bool mValueValid, mFormatted, mJustEdited;
    bool mWasFocusIn, mCancelling, mDoNotValidate, mSelectedOnEntry;
    static Xtring mSpecialControlKeys;
    int mOldCursorPosition;
    Xtring mOldText;
    friend class LineEditValidator;
};

}; // namespace

#endif  // GONGGUILINEEDIT_H
