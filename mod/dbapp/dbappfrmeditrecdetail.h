#ifndef _GONG_FRMEDITRECDETAIL_H
#define _GONG_FRMEDITRECDETAIL_H

/** @file dbappfrmeditrecdetail.h A Form to edit a dbRecord of type detail
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include <gongdbrecordlist.h>
#include <gongdbrecordlistdatamodel.h>
#include "dbappfrmeditrecmaster.h"

namespace gong {

class FrmEditRecDetail : public FrmEditRecMaster
{
    Q_OBJECT

public:
    FrmEditRecDetail(FrmEditRecMaster *frmmaster, int ndetalle,
                     dbRecord *detalle, const Xtring &detailstablename, dbRecordDataModel *dm=0,
                     EditMode editmode = DataTable::defaulteditmode,
                     dbApplication::EditFlags editflags = dbApplication::editNone,
                     QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
    ~FrmEditRecDetail();
    dbRecordList *getDetalles() const {
        return pDetalles;
    }
    FrmEditRecMaster *getFrmMaster() const {
        return static_cast<FrmEditRecMaster *>(pParentForm);
    }
    unsigned int getNumDetalle() const {
        return mNumDetalle;
    }
    void addDetailIfNeeded( bool forcerefresh = true );
    virtual void restoreBrowseStatus(); // from FrmEditRecMaster
    const Xtring &getDetallesTableName() const {
        return mDetallesTableName;
    }

protected:
    virtual void clearRecord(bool duplicating = false); // From FrmEditRec
    virtual bool read(dbRecordID id = 0); // From FrmEditRec
    virtual bool save(); // From FrmEditRec
    virtual bool remove(); // From FrmEditRec
    virtual dbRecordID getTableRecID() const; // From FrmEditRecMaster
    virtual void beginEdit(DataTable *dt, EditMode newmode,
                           dbApplication::EditFlags editflags,
                           const Xtring &fldtext); // from FrmEditRecMaster
    virtual void initMenus(); // from FrmEditRec
    virtual void keyPressEvent( QKeyEvent *e ); // from FrmBase
    virtual void updateStatus( bool callbehaviors = true ); // from FrmEditRec

protected slots:
    void slotFocusChanged( DataTable *, QFocusEvent *);
    void slotDetailUpClicked();
    void slotDetailDownClicked();

protected:
    dbRecordList *pDetalles;
    Xtring mDetallesTableName;
    unsigned int mNumDetalle;
    QPushButton *pButtonDetailUp, *pButtonDetailDown;
};

}

#endif // _GONG_FRMEDITRECDETAIL_H
