#ifndef _GONG_GUICONTROLS_H
#define _GONG_GUICONTROLS_H

/** @file gongguicontrols.h Qt based gui controls
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include <QFrame>
#include <QPushButton>
#include <QGroupBox>
#include <gongdbfieldstyle.h>
#include <gongdbfielddefinition.h>
#include "gongcombobox.h"
#include "gongcheckbox.h"
#include "gonglineedit.h"
#include "gongtextedit.h"

namespace gong {

// \ojo Si se añade un nuevo tipo de control,
// revisar FrmEditRec::edited(), isEdited(), fixFocusWidgetText(), validateControls y enableEditControls()


#if 0
class IControl
{
public:
    IControl( QWidget *control );
    void setVisible( bool visible );
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
        mMustBeReadOnly = must;
    }
    bool horizontal() const {
        return mHorizontal;
    }
    void setHorizontal(bool must) {
        mHorizontal = must;
    }
    virtual void setVisible( bool visible );
    bool isReadOnly() const {
        return mIsReadOnly;
    }
    virtual void setReadOnly(bool readonly);
    bool isEdited() const {
        return mIsEdited;
    }
    virtual void setEdited(bool edited) {
        mIsEdited = edited;
    }
    Variant getValue() const;
    void setValue( const Variant &value);
private:
    QLabel *pLabel;
    QBoxLayout *pLayout;
    QWidget *pControl;
    bool mHorizontal, mMustBeReadOnly, mIsReadOnly, mIsEdited;
};
#endif

class EditBox: public LineEdit
{
private:
    QLabel *pLabel;
    QBoxLayout *pLayout;
    bool mHorizontal, mMustBeReadOnly;
public:
    EditBox(QWidget *parent, const Xtring &name, const Xtring &caption,
            bool horizontal = true );
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
        setReadOnly(mMustBeReadOnly = must);
    }
    void setVisible( bool visible );
};


class TextBox : public TextEdit
{
private:
    QLabel *pLabel;
    QBoxLayout *pLayout;
    bool mHorizontal, mMustBeReadOnly;
public:
    TextBox(QWidget *parent, const Xtring &name,
            const Xtring &caption, bool horizontal = false );
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
        setReadOnly(mMustBeReadOnly = must);
    }
    void setHeightInLines(int lines);
};

class RichTextBox : public QLabel
{
    Q_OBJECT
private:
    QLabel *pLabel;
    QBoxLayout *pLayout;
    bool mHorizontal, mMustBeReadOnly, mIsReadOnly, mIsEdited;
    QPushButton *pEditRich, *pEditRichHTML;
    XtringList mCustomStyles;

protected slots:
    virtual void slotBeginEdit();
    virtual void slotBeginEditHTML();
    virtual QSize sizeHint() const; // from QWidget

public:
    RichTextBox(QWidget *parent, const Xtring &name,
                const Xtring &caption, bool horizontal = false );
    Xtring getText() const;
    Xtring toString() const {
        return getText();
    }
    void setText( const Xtring &text);
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
        setReadOnly(mMustBeReadOnly = must);
    }
    bool isReadOnly() const {
        return mIsReadOnly;
    }
    void setReadOnly(bool readonly);
    bool isEdited() const {
        return mIsEdited;
    }
    void setEdited(bool edited) {
        mIsEdited = edited;
    }
    const XtringList &getCustomStyles() const {
        return mCustomStyles;
    }
    void setCustomStyles( const XtringList &customstyles ) {
        mCustomStyles = customstyles;
    }
};

class FileNameBox: public QPushButton
{
    Q_OBJECT
public:
    FileNameBox( QWidget *parent, const Xtring &name, const Xtring &caption,
                 bool choosedir, bool horizontal);

    QBoxLayout *getLayout() const {
        return pLayout;
    }
    QPushButton *getButton() {
        return this;
    }
    Xtring getFileName() const;
    void setFileName(const Xtring &filename);
    LineEdit *getEditFileName() const {
        return pEditFileName;
    }
    bool mustBeReadOnly() const {
        return mMustBeReadOnly;
    }
    void setMustBeReadOnly(bool must) {
        setEnabled(!(mMustBeReadOnly = must));
    }
    void setFocus() {
        pEditFileName->setFocus();
    }
    void setEnabled( bool enabled ); // from QWidget

private:
    LineEdit *pEditFileName;
    QBoxLayout *pLayout;
    bool mHorizontal, mMustBeReadOnly, mChooseDir;
protected slots:
    void slot_button_clicked();
};


class GroupBox: public QGroupBox
{
    Q_OBJECT
public:
    GroupBox( XtringList &options, const Xtring &caption,
              QWidget *parent, bool horizontal = true);
    int getSelectedPos() const;
    void setSelectedPos( int sel );
    Xtring getSelectedString() const;
    void setSelectedString( const Xtring &value );
    bool mustBeReadOnly() const {
        return mMustBeReadOnly;
    }
    void setMustBeReadOnly(bool must) {
        setReadOnly(mMustBeReadOnly = must);
    }
    bool isReadOnly() const {
        return isEnabled();
    }
    void setReadOnly(bool readonly) {
        setEnabled( !readonly );
    }
    bool isEdited() const {
        return mIsEdited;
    }
    void setEdited(bool edited) {
        mIsEdited = edited;
    }
protected slots:
    void radio_clicked(bool checked);
signals:
    void radio_clicked(int pos);
private:
    XtringList mOptions;
    bool mMustBeReadOnly, mIsReadOnly, mIsEdited;
};


}; // namespace gong

#endif // _GONG_GUICONTROLS_H
