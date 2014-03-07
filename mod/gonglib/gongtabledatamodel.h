#ifndef _GONG_TABLEDATAMODEL_H
#define _GONG_TABLEDATAMODEL_H

/** @file gongtabledatamodel.h Gui table data model base class
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include "gongdbviewdefinition.h"

namespace gong {

class dbRecord;

class TableDataModel
{
public:
    TableDataModel(const dbViewDefinitionDict &views, const Xtring &filter = Xtring());
    virtual ~TableDataModel() {};

    // Pure virtual functions
    virtual Variant getValue(unsigned int row, unsigned int col) const =0;
    virtual bool setValue(unsigned int row, unsigned int col, const Variant &value) =0;
    virtual unsigned int getRowCount() const =0;
    virtual bool setViewIndex(int nview) =0;
    virtual int getGroupLevel(int /*row*/) const {
        return 0;
    }
    int getGroupCount() const {
        return mGroupCount;
    }

    Variant getValue(unsigned int row, const Xtring &fldname) const {
        return getValue(row, findFieldPos(fldname));
    }
    bool setValue(unsigned int row, const Xtring &fldname, const Variant &value) {
        return setValue(row, findFieldPos(fldname), value );
    }
    dbRecordID getRowID(int row) const {
        return getValue(row, 0).toInt();
    }
    int findFieldPos(const Xtring &fldname) const;
    const Xtring &getColName(unsigned int col) const {
        return mFieldPos.getKey(col);
    }
    unsigned int getColCount() const {
        return mColCount;
    }
    bool isReadOnly() const {
        return mReadOnly;
    }
    bool hasFetchedAllRows() const {
        return mHasFetchedAllRows;
    }
    const dbViewDefinitionDict &getViews() const {
        return mViewDefinitionDict;
    }
    dbViewDefinition *getView(int index) const;
    unsigned int getViewCount() const {
        return mViewDefinitionDict.size();
    }
    dbViewDefinition *getCurrentView() const {
        return getView(mCurrentViewIndex);
    }
    int getCurrentViewIndex() const {
        return mCurrentViewIndex;
    }
    void setViews(const dbViewDefinitionDict &v) {
        mViewDefinitionDict = v;
        mCurrentViewIndex=-1;
    }
    void addView(const dbViewDefinition *view);
    void deleteView(const dbViewDefinition *view);

    void addFilter(const Xtring &aFilter);
    void setFilter(const Xtring &aFilter);
    void removeFilters();
    const Xtring getFilter(const Xtring &where, const Xtring &addTo = Xtring::null,
                           const Xtring &condsSep= "AND") const;
protected:
    dbViewDefinitionDict mViewDefinitionDict;
    Xtring mFilter;
    unsigned int mColCount;
    bool mReadOnly, mHasFetchedAllRows, mGroupCount;
    int mCurrentViewIndex;
    Dictionary<int> mFieldPos;
};

}; // namespace gong

#endif // _GONG_DBTABLEDATAMODEL_H
