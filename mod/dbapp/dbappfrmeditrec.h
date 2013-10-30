#ifndef _GONG_FRMEDITREC_H
#define _GONG_FRMEDITREC_H

/** @file dbappfrmeditrec.h A Form to edit a dbRecord
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */


#include <vector>
#include <gongguicontrols.h>
#include <gongfrmbase.h>
#include <gongdatatable.h>
#include "dbappsearchbox.h"
#include "dbappdbapplication.h"

namespace gong {

class FrmEditRecBehavior;

typedef std::vector<QWidget *> EditControlsList;

class FrmEditRec: public FrmBase {
    Q_OBJECT

public:
    typedef DataTable::EditMode EditMode;

    enum EditStatus { cancelled = 0, saved, not_saved, never_shown };

    FrmEditRec(FrmEditRec *parentfrm, dbRecord *record,
               EditMode editmode = DataTable::defaulteditmode,
               dbApplication::EditFlags editflags = dbApplication::editNone,
               QWidget *parent = 0, const char* name = 0,  WidgetFlags fl = 0 );
    ~FrmEditRec();

    /* operations on the form */
    virtual bool canClose(); // from FrmBase
    virtual void accept(); // from FrmBase
    virtual bool read(dbRecordID id);
    virtual bool save();
    virtual bool remove();
    virtual void scatterFields() {};
    virtual void gatherFields() {};
    virtual void clearRecord(bool duplicating = false);
    virtual bool edited() const;
    virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ve = 0 );
    virtual void justShown( bool firsttime ); // from FrmBase
    virtual bool canBeginEdit( EditMode newmode );

    void scatter();
    void gather();
    dbRecord *getRecord() const {
        return pRecord;
    }
    dbRecordID getRecordID() const {
        return getRecord()->getRecordID();
    }
    void setEditMode(EditMode p_editmode) {
        mEditMode = p_editmode;
    }
    EditStatus getEditStatus() {
        return mEditStatus;
    }
    bool isSaved() const {
        return mEditStatus == saved;
    }
    bool isInserting() const {
        return mEditMode == DataTable::inserting || mEditMode == DataTable::duplicating;
    };
    bool isDuplicating() const {
        return mEditMode == DataTable::duplicating;
    }
    bool isEditing() const  {
        return ( isInserting() || isUpdating() || isDeleting() || isDuplicating() );
    }
    bool isDeleting() const {
        return mEditMode == DataTable::deleting;
    }
    bool isUpdating() const {
        return mEditMode == DataTable::updating;
    };
    bool isSelecting() const {
        return mEditMode == DataTable::selecting || mEditMode == DataTable::choosing;
    };
    bool isReadOnly() const {
        return mEditFlags & dbApplication::readOnly;
    }
    void setReadOnly(bool ro = true);
    void setFormFilter(const Xtring &filter) {
        mFormFilter = filter;
    }
    const Xtring &getFormFilter() const {
        return mFormFilter;
    }
    QWidget *findControl(const Xtring &fieldname) const;
    Variant getControlValue( const Xtring &fieldname ) const;
    bool setControlValue( const Xtring &fieldname, const Variant &value );
    void setEdited(bool edited);
    void setFocusWidgetText( const Xtring &text) {
        mFocusWidgetText = text;
    }
    bool fieldHasChangedAfterSaving(const Xtring &fldname);
    bool validCodeAndDesc( QWidget *sender, ValidResult &validerror, LineEdit *editCode, LineEdit *editDesc,
                           const Xtring &fldcodename, const Xtring &flddescname, const Xtring &cond = Xtring::null, bool codecanbenull = false );
    bool validSeekCode( QWidget *sender, bool *isvalid, ValidResult &validerror,
                        LineEdit *editCode, LineEdit *editDesc, dbRecord *record,
                        const Xtring &fldcodename, const Xtring &flddescname, const Xtring &cond,
                        dbApplication::SeekCodeFlags flags = dbApplication::SeekCodeNone,
                        LineEdit *editAlt = 0, const Xtring &fldaltname = Xtring::null );
    bool validSeekCode( List<dbRecordID>&rids,
                        QWidget *sender, bool *isvalid, ValidResult &validerror,
                        LineEdit *editCode, LineEdit *editDesc, dbRecord *record,
                        const Xtring &fldcodename, const Xtring &flddescname, const Xtring &cond,
                        dbApplication::SeekCodeFlags flags = dbApplication::SeekCodeNone,
                        LineEdit *editAlt = 0, const Xtring &fldaltname = Xtring::null );
    bool showAndSave();
    bool validateControls(bool justedited = false);
    FrmEditRec* getParentForm() const {
        return pParentForm;
    }
    void showTabs(bool show = true );
    QWidget *insertTab(QWidget *tab, const Xtring &label, int index = -1);
    void setTabTitle( QWidget *tab, const Xtring &title );

    // Adition of controls// FIELD Nombre string pFrameContactos nombreLayout
    QHBoxLayout *addHBoxLayout(QBoxLayout *parent);
    QVBoxLayout *addVBoxLayout(QBoxLayout *parent);
    QMenuBar *getMenuBar() const {
        return pMenuBar;
    }
    EditBox *addEditField(QWidget *parent, const Xtring &tablename,
                          const Xtring &fldname, QBoxLayout *layout = 0, bool horizontal = true );
    TextBox *addTextField(QWidget *parent, const Xtring &tablename,
                          const Xtring &fldname, QBoxLayout *layout = 0, bool horizontal = false );
    RichTextBox *addRichTextField(QWidget *parent, const Xtring &tablename,
                                  const Xtring &fldname, QBoxLayout *layout = 0, bool horizontal = false );
    CheckBox *addCheckField(QWidget *parent, const Xtring &tablename,
                            const Xtring &fldname, QBoxLayout *layout = 0);
    template<class ValueT>
    ComboBox<ValueT> *addComboField(QWidget *parent, const Xtring &tablename,
                                    const Xtring &fldname, QBoxLayout *layout = 0, bool horizontal = true);
    SearchBox *addSearchField(QWidget *parent, const Xtring &fullfldname,
                              const Xtring &reftablename, const Xtring &fldnamecodigo,
                              const Xtring &fldnamenombre, QBoxLayout *layout = 0, SearchBox::Flags flags = SearchBox::FlagHorizontal);
    class ImageBox *addImageField(QWidget *parent, const Xtring &tablename, const Xtring &fldnamenombre,
                                          QBoxLayout *layout = 0, bool horizontal = false);
    FileNameBox *addFileNameField(QWidget *parent, const Xtring &tablename, const Xtring &fldnamenombre,
                                  QBoxLayout *layout = 0, bool horizontal = false);
    bool addBehavior(FrmEditRecBehavior *behavior);
    bool removeControl( QWidget *control );

    // static functions to set fields styles and default values
    static void applyFieldStyle(QLabel *label, const dbFieldDefinition *fielddef);
    static void applyFieldStyle(QCheckBox *check,const dbFieldDefinition *fielddef);
    static void applyFieldStyle(QPushButton *button, const dbFieldDefinition *fielddef);
    static void applyBasicStyle(LineEdit *edit, const Xtring &stylename);
    static void applyFieldStyle(LineEdit *edit, const dbFieldDefinition *fielddef);
    static void applyFieldStyle(QComboBox *combo, const dbFieldDefinition *fielddef);
    static void applyFieldStyle(TextEdit *edit, const dbFieldDefinition *fielddef);

public slots:
    void validate(QWidget *sender, bool *isvalid, ValidResult *ve = 0 );
protected slots:
    virtual void specialControlKeyPressed(QWidget *sender, char key);
    void validateComboOrCheck(int selected);

protected:
    template<class ValueT>void validateCombo(int selected);
    virtual void updateStatus( bool callbehaviors = false);
    virtual void enableEditControls(bool enabled);
    virtual void initMenus();
    void enableSearchBoxes(bool enabled);
    void msgInputError(const Xtring &errorText);
    void showValidMessages(bool *isvalid, const ValidResult &validerror, QWidget *sender);
    Xtring getFrom() {
        return getRecord()->getTableName();
    }
    void fixFocusWidgetText();
    void addSpacersToTabs();

protected:
    QVBoxLayout *pFormLayout;
    QVBoxLayout *pMainLayout;
    QTabWidget *pTabWidget;
    QWidget *pControlsFrame;
    QVBoxLayout *pControlsLayout;
    QHBoxLayout *pButtonsLayout;

    QMenuBar *pMenuBar;
    QMenu *pMenuRecord, *pMenuTable;

    /** The form that this form is embedded into, if any. The parent form is refreshed every time this form is saved */
    FrmEditRec *pParentForm;
    dbRecord *pRecord, *pOrigRecord;
    EditMode mEditMode;
    dbApplication::EditFlags mEditFlags;
    EditStatus mEditStatus;
    dbRecordID mLastID;
    EditControlsList mEditControls;
    Xtring mFormFilter, mFocusWidgetText;
    bool mEdited;
    char mControlKeyPressed;
    List<FrmEditRecBehavior *> mBehaviors;
    friend class FrmEditRecBehavior;
};

}; // namespace

#endif // _GONG_FRMEDITREC_H
