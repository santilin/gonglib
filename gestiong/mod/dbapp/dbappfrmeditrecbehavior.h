#ifndef _GONG_DBAPP_FRMEDITRECBEHAVIOR_H
#define _GONG_DBAPP_FRMEDITRECBEHAVIOR_H

/** @file dbappfrmeditrecbehavior.h A behavior to add to FrmEditRecs
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include <QObject>
#include <gongdbrecord.h>
#include <gonglist.h>
#include <gongdatatable.h>

#ifndef BEHAVIOR_PRE
#define BEHAVIOR_PRE true
#define BEHAVIOR_POST false
#endif

class QWidget;
class QTabWidget;
class QVBoxLayout;
class QHBoxLayout;

namespace gong {

class FrmEditRec;
class ValidResult;

typedef std::vector<QWidget *> EditControlsList;

class FrmEditRecBehavior: public QObject
{
public:
    FrmEditRecBehavior(FrmEditRec *theForm): pTheForm(theForm) {};
    virtual void initGUI() {}
    virtual void scatterFields( bool is_pre ) {}
    virtual void gatherFields() {}
    virtual void validateFields( bool is_pre,
                                 QWidget *sender, bool *isvalid, ValidResult *ve = 0 ) {}
    virtual bool read(dbRecordID id) {
        return true;
    }
    virtual bool save() {
        return true;
    }
    virtual bool remove() {
        return true;
    }
    virtual void updateStatus() {}
    virtual bool canBeginEdit(DataTable::EditMode editmode ) {
        return true;
    }
    virtual bool canClose() {
        return true;
    }
    virtual void specialControlKeyPressed( QWidget *sender, char key ) {}
    virtual void updateFromDetails( class FrmEditRecDetail *frmdetail ) {}

protected:
    FrmEditRec *pTheForm;
    char getControlKeyPressed();
    QWidget *getOrCreateTab(const char *tabname );
    QWidget *getControlsFrame();
    QVBoxLayout *getControlsLayout();
    QHBoxLayout *getButtonsLayout();
    EditControlsList &getEditControlsList() const;
    DataTable *changeDataTable( DataTable *new_dt );
    void showValidMessages( bool *isvalid,
                            const ValidResult &validresult, QWidget *sender);
};

typedef List<FrmEditRecBehavior *> FrmEditRecBehaviorsList;

} // namespace gong

#endif // _GONG_DBAPP_FRMEDITRECBEHAVIOR_H
