#ifndef ___GONGDBRECORDRELATION_H
#define ___GONGDBRECORDRELATION_H

/** @file gongdbrecordrelation.h Relation information associated to a relation definition
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include "gongxtring.h"
#include "gongdictionary.h"
#include "gongdbrelationdefinition.h"

namespace gong {

class dbRecord;
class dbRecordList;

class dbRecordRelation
{
public:
    dbRecordRelation(dbRelationDefinition* reldef, bool enabled, dbRecord* relrecord, dbRecordList* relrecordlist = 0);
    ~dbRecordRelation();
    dbRecord *getRelatedRecord( int index = -1 );
    dbRecord *addRelatedRecord( const dbRecord *newrecord, int pos=-1);
    bool isRelatedRecordCreated( int index = -1 ) const;

public:
    dbRelationDefinition* getRelationDefinition() const {
        return pRelationDefinition;
    }
    bool isEnabled() const {
        return mEnabled;
    }
    dbRecordList* getRelatedRecordList() const {
        return pRelatedRecordList;
    }
    dbRecordList* getRelatedRecordListOrig() const {
        return pRelatedRecordListOrig;
    }

    void setEnabled(bool enabled) {
        mEnabled = enabled;
    }
    void setRelRecord(dbRecord* relrecord) {
        pRelatedRecord = relrecord;
    }
private:
    dbRelationDefinition* pRelationDefinition;
    bool mEnabled;
    dbRecord* pRelatedRecord;
    dbRecordList* pRelatedRecordList, *pRelatedRecordListOrig;

public:
    dbRelationDefinition::Type getType() {
        return pRelationDefinition->getType();
    }
    const Xtring &getLeftTable() const {
        return pRelationDefinition->getLeftTable();
    }
    const Xtring &getLeftField() const {
        return pRelationDefinition->getLeftField();
    }
    const Xtring &getRightTable() const {
        return pRelationDefinition->getRightTable();
    }
    const Xtring &getRightField() const {
        return pRelationDefinition->getRightField();
    }
    const Xtring getName() const {
        return pRelationDefinition->getName();
    }

    friend class dbRecord;
};

typedef Dictionary<dbRecordRelation *> dbRecordRelationsList;

} // namespace gong
#endif // __GONGDBRECORDRELATION_H
