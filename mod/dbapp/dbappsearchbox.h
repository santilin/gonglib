#ifndef _DBAPP_SEARCHBOX_H
#define _DBAPP_SEARCHBOX_H

/** @file dbappsearchbox.h A widget to show a reference to another record
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include <QPushButton>
#include "gongdbrecord.h"
#include "gonglineedit.h"

class QBoxLayout;

namespace gong {

class RecordCompleter;

class SearchBox: public QPushButton
{
    Q_OBJECT
public:
    enum Flags { FlagHorizontal = 1, FlagMultiple = 2, FlagEditableDesc = 4, FlagShowLabels = 8 };
    SearchBox( const Xtring &caption, const Xtring &tablename,
               const Xtring &fldcode, const Xtring &flddesc,
               QWidget *parent, enum Flags flags = FlagHorizontal );
    ~SearchBox();
    QBoxLayout *getLayout() const {
        return pLayout;
    }
    QPushButton *getButton() {
        return this;
    }
    LineEdit *getEditCode() const {
        return pEditCode;
    }
    LineEdit *getEditDesc() const {
        return pEditDesc;
    }
    QLabel *getLabelCode() const {
        return pLabelCode;
    }
    QLabel *getLabelDesc() const {
        return pLabelDesc;
    }
    bool mustBeReadOnly() const {
        return mMustBeReadOnly;
    }
    void setMustBeReadOnly(bool must) {
		setReadOnly(!(mMustBeReadOnly=must));
	}
	void setReadOnly(bool readonly);
    bool isMultiple() const {
        return mFlags & FlagMultiple;
    }
    void setFocus() {
        pEditCode->setFocus();
    }
    void setValue( const Variant &value );
    Variant getValue() const;
    dbRecord *getRecord() const {
        return pRecord;
    }
    Xtring getTableName() const {
        return mTableName;
    }
    Xtring getFldDesc() const {
        return mFldDesc;
    }
    Xtring getFldCode() const {
        return mFldCode;
    }
    List<dbRecordID> &getRecordIDs() {
        return mRecordIDs;
    }
    Flags getFlags() const {
        return mFlags;
    }
    void setVisible( bool visible );
    void setCompleter( bool wantit );

public slots:
    void slotClicked();
    void slotValidate( QWidget *, bool * );
    void slotSpecialControlKeyPressed ( QWidget *, char );

private:
    LineEdit *pEditCode, *pEditDesc;
    QLabel *pLabelCode, *pLabelDesc;
    QBoxLayout *pLayout;
    Flags mFlags;
    bool mMustBeReadOnly;
    Xtring mTableName, mFldCode, mFldDesc;
    dbRecord *pRecord;
    List<dbRecordID>mRecordIDs;
    RecordCompleter *pRecordCompleter;
};

} // namespace gong

#endif // _DBAPP_SEARCHBOX_H
