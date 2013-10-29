#ifndef _GONG_TEXTEDIT_H
#define _GONG_TEXTEDIT_H

/** @file gongtextedit.h Qt based text edit box
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */


#include <QPlainTextEdit>
#include <QFocusEvent>
#include <QKeyEvent>
#include <gongxtring.h>

namespace gong {

/**
  @brief reimplentation of the QTextEdit

  Adds some changes in the navigation and focus behavior
  http://lists.trolltech.com/qt-interest/2003-05/thread00460-0.html

*/

class TextEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    TextEdit(QWidget *parent);
    Xtring toString() const;
    void setText(const Xtring &text);
    void setText(const QString &text);
    bool isEdited() const;
    void setEdited( bool edited = true );
    bool isJustEdited() const;
    void setJustEdited( bool justedited = true );

signals:
    void validate(QWidget *sender, bool *isvalid);

protected:
    static int mGlobalValidating, mLastGlobalValidating;
    bool mValidating, mGettingFocus;

public slots:
    virtual void setReadOnly(bool);
    void slotTextChanged();

protected:
    virtual void keyPressEvent( QKeyEvent *e ); // from QTextEdit
    virtual void focusOutEvent( QFocusEvent *e); // from QTextEdit
    virtual void focusInEvent( QFocusEvent *e); // from QTextEdit
    virtual QSize sizeHint() const; // from QWidget
private:
    bool mChanged, mJustEdited;
};

}; // namespace

#endif  // GONGTEXTEDIT_H
