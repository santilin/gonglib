#ifndef _GONG_DBAPP_FRMEDITRECMASTER_H
#define _GONG_DBAPP_FRMEDITRECMASTER_H

/** @file dbappfrmeditrecmaster.h A Form to edit and browse a dbRecord
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

//Added by qt3to4:
#include <Q3GridLayout>
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>
#include <Q3Frame>
#include <QFocusEvent>
#include <QAction>

class Q3VBoxLayout;
class Q3HBoxLayout;
class Q3GridLayout;
class Q3Frame;
class QPushButton;

#include "config.h"
#ifdef HAVE_RTKMODULE
#include <rtknamespace.h>
#endif
#include <gongdbrecorddatamodel.h>
#include "dbappfrmeditrec.h"

namespace gong {

class DataTable;


class FrmEditRecMaster: public FrmEditRec
{
    Q_OBJECT

public:
    FrmEditRecMaster(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm=0,
                     EditMode editmode = DataTable::defaulteditmode,
                     dbApplication::EditFlags editflags = dbApplication::editNone,
                     QWidget *parent = 0, const char* name = 0, WidgetFlags fl = 0 );
    virtual ~FrmEditRecMaster();
    virtual void accept(); // from FrmBase
    virtual void justShown(bool fisrttime); // from FrmBase
    virtual bool read(dbRecordID id = 0); // from FrmEditRec
    virtual void updateFromDetails( class FrmEditRecDetail *frmdetail);
    virtual dbRecordID getTableRecordID() const;

    bool setView( int nview, bool force );
    bool isBrowsing() {
        return mBrowsing;
    };
    bool isChoosing() {
        return mChoosing;
    };
    void applyFilter( const Xtring &filter = Xtring::null);
    DataTable *getDataTable() const {
        return pDataTable;
    }
    Xtring getDataModelFilter() const;
    Xtring getWholeFilter() const;

public slots:
    virtual void pushMiddle_clicked();
    virtual void menuRecordAdd_clicked();
    virtual void menuRecordDelete_clicked();
    virtual void menuRecordModify_clicked();
    virtual void menuRecordDup_clicked();
    virtual void menuRecordPrint_clicked();
    virtual void menuRecordView_clicked();
    virtual void menuRecordInfo_clicked();
    virtual void menuRecordReopen_clicked();
    virtual void menuTableReorder_clicked();
    virtual void menuTableFilter_clicked();
    virtual void menuTableRemoveFilter_clicked();
    virtual void menuTableImport_clicked();
    virtual void menuTablePrint_clicked();
    virtual void menuTableUpdate_clicked();
    virtual void menuTableExport_clicked();
    virtual void menuTableSelectAll_clicked();
    virtual void menuViewAdd_clicked();
    virtual void menuViewModify_clicked();
    virtual void menuViewDup_clicked();
    virtual void menuViewDelete_clicked();
    virtual void tableCurrentChanged(int,int);
    virtual void refresh(); // from FrmBase
private slots:
    void slotBeginEdit(DataTable *dt, DataTable::EditMode editmode,
                       const Xtring &fldvalue = Xtring::null);
    void slotViewChanged();

protected:
    virtual void focusInEvent ( QFocusEvent * event ); // from QWidget
    virtual void initMenus(); // from FrmEditRec
    virtual void updateStatus( bool callbehaviors = true ); // from FrmEditRec
    virtual bool canClose(); // from FrmBase
    virtual void beginEdit(DataTable *dt, DataTable::EditMode editmode,
                           dbApplication::EditFlags flags = dbApplication::editNone,
                           const Xtring &fldvalue = Xtring::null);
    virtual dbRecordPermissions getPermissions() const;

    void ensureEditView();
    void ensureBrowseView();
    void editNewView( dbViewDefinition *viewdef );
    void setTableDataModel();
#ifdef HAVE_RTKMODULE
    void printReport(const Xtring &reportname, const Dictionary<Xtring> &properties,
					 const Xtring &filter, PageOrientation po,
                     const Xtring &defines, bool isafilename, bool resortototablename);
#endif
    virtual void restoreBrowseStatus();
    void saveBrowseStatus() const;
    void updateViewMenus( bool refill = true );

    QAction *pMenuRecordAdd, *pMenuRecordDelete, *pMenuRecordModify, *pMenuRecordDup,
            *pMenuRecordView, *pMenuRecordPrint, *pMenuRecordReopen, *pMenuRecordInfo;
    QAction *pMenuTableReorder, *pMenuTableFilter, *pMenuTableRemoveFilter,
            *pMenuTablePrint, *pMenuTableImport, *pMenuTableExport,
            *pMenuTableSelectAll, *pMenuTableUpdate;
    QMenu *pMenuView;
    QAction *pMenuViewAdd, *pMenuViewModify, *pMenuViewDup, *pMenuViewDelete;
    DataTable* pDataTable;
    QPushButton *pushMiddle;

    bool mBrowsing, mChoosing, mWasBrowsing, mMustRead;
    bool mMustRefresh, mRefreshing, mMultipleDeleting;
    dbRecordDataModel *pDataModel;
    friend class FrmEditRecBehavior;
    friend class FrmAdvancedPrint;
};

}; // namespace

#endif // _GONG_DBAPP_FRMEDITRECMASTER_H
