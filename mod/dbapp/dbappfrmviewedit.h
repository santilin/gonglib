#ifndef _GONG_FRMVIEWEDIT_H
#define _GONG_FRMVIEWEDIT_H

/** @file dbappfrmviewedit.h A form to edit a view for a FrmEditRecMaster
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
#include <QListWidgetItem>
//Added by qt3to4:
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QFrame>
#include <gongdbviewdefinition.h>
#include <gongdatatable.h>
#include "gongfrmbase.h"
#include "gongguiapplication.h"

class QListWidget;
class QComboBox;
namespace gong {

class FrmEditRecMaster;

struct ViewEditListItemInfo
{
    dbTableDefinition *tabledef;
    dbFieldDefinition *fielddef;
    bool inView;
};

class ViewEditListItem: public QListWidgetItem
{
public:
    ViewEditListItem ( ViewEditListItemInfo &it, const QString & text, QListWidget * parent = 0 )
        : QListWidgetItem( text, parent ), mItemInfo(it) {}
    ViewEditListItemInfo &getItemInfo() {
        return mItemInfo;
    }
private:
    ViewEditListItemInfo mItemInfo;
};


class FrmViewEdit : public FrmBase
{
    Q_OBJECT

public:
    FrmViewEdit ( DataTable::EditMode editmode, dbRecord *rec, dbViewDefinition *viewdef,
                  QWidget* parent = 0, WidgetFlags fl = 0 );
    int exec();

protected slots:
    void lstFields_currentItemChanged(QListWidgetItem *current,QListWidgetItem *previous);
    void viewModified();
    void pushApply_clicked();
    void validate( QWidget *, bool * );
    void itemChanged ( QListWidgetItem * item );
    void pushUp_clicked();
    void pushDown_clicked();
    void pushAddField_clicked();

protected:
    virtual bool canClose(); // from FrmBase
    Xtring getFldExpression ( const ViewEditListItemInfo &ii ) const;
    int addFieldToList ( const Xtring &text, ViewEditListItemInfo &iteminfo );
    Xtring chooseField(const dbDefinition *db, const Xtring &tablename,
                       QWidget *parent = 0, const char *name = 0,
                       WidgetFlags fl = 0);
    void addRelatedFields(XtringList &fieldnames, const dbDefinition *db,
                          const Xtring &tablename, size_t levels, const Xtring &othertables);
    bool saveView();
    void updateStatus();
    void updateView();

private:
    QVBoxLayout *verticalLayout;
    QVBoxLayout *mainLayout;
    QVBoxLayout *mainLayout_2;
    QVBoxLayout *layViewName;
    QLabel *lblViewName;
    LineEdit *txtViewName;
    QVBoxLayout *layViewOrigin;
    QLabel *lblViewOrigin;
    LineEdit *txtViewOrigin;
    QHBoxLayout *fieldsLayout;
    QVBoxLayout *layFields;
    QLabel *lblFields;
    QListWidget *lstFields;
    QFrame *frmFieldProps;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *layProps;
    QVBoxLayout *layCaption;
    QLabel *lblCaption;
    LineEdit *txtCaption;
    QVBoxLayout *layWidth;
    QLabel *lblWidth;
    LineEdit *txtWidth;
    QVBoxLayout *layStyle;
    QLabel *lblStyle;
    LineEdit *txtStyle;
    QSpacerItem *spProps;
    QHBoxLayout *horizontalLayout;
    QCheckBox *chkIncludeFilter;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *buttonsLayout;
    QSpacerItem *sbButtons;
    QPushButton *pushApply;
    QPushButton *pushUp, *pushDown, *pushAddField;
    QHBoxLayout *layFieldButtons;
    QLabel *lblSetting;
    QComboBox *comboSetting;
    QHBoxLayout *settingsLayout;
    QLabel *lblOrderBy;
    LineEdit *txtOrderBy;

    DataTable::EditMode mEditMode;
    FrmEditRecMaster *pFrmEditRecMaster;
    dbViewDefinition *pViewDefinition;
    dbRecord *pRecord;
	int mSaveSettingIndex;
};

}

#endif // _GONG_FRMVIEWEDIT_H

