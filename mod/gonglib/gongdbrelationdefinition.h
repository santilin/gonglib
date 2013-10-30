/*<<<<<DBRELATIONDEFINITION_PREAMBLE*/
#ifndef ___GONGDBRELATIONDEFINITION_H
#define ___GONGDBRELATIONDEFINITION_H
/*>>>>>DBRELATIONDEFINITION_PREAMBLE*/

/*<<<<<COPYLEFT*/
/** @file gongdbrelationdefinition.h Table relations definitions
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */
/*>>>>>COPYLEFT*/

#include "gongxtring.h"
#include "gongdictionary.h"

namespace gong {

class dbRelationDefinition
{
public:
    enum Type { one2one, one2many, many2many, aggregate };

    /*<<<<<DBRELATIONDEFINITION_FULLCONSTRUCTOR*/
    dbRelationDefinition(const Type& type, const Xtring& lefttable, const Xtring& leftfield, const Xtring& righttable, const Xtring& rightfield)
        : mType(type), mLeftTable(lefttable), mLeftField(leftfield), mRightTable(righttable), mRightField(rightfield)
/*>>>>>DBRELATIONDEFINITION_FULLCONSTRUCTOR*/
    {}

    Xtring getName() const;
    Xtring getFullName() const;

    /*<<<<<DBRELATIONDEFINITION_CLASS*/
public:
    const Type& getType() const {
        return mType;
    }
    const Xtring& getLeftTable() const {
        return mLeftTable;
    }
    const Xtring& getLeftField() const {
        return mLeftField;
    }
    const Xtring& getRightTable() const {
        return mRightTable;
    }
    const Xtring& getRightField() const {
        return mRightField;
    }

    void setType(const Type& type) {
        mType = type;
    }
    void setLeftTable(const Xtring& lefttable) {
        mLeftTable = lefttable;
    }
    void setLeftField(const Xtring& leftfield) {
        mLeftField = leftfield;
    }
    void setRightTable(const Xtring& righttable) {
        mRightTable = righttable;
    }
    void setRightField(const Xtring& rightfield) {
        mRightField = rightfield;
    }
private:
    Type mType;
    Xtring mLeftTable;
    Xtring mLeftField;
    Xtring mRightTable;
    Xtring mRightField;
    /*>>>>>DBRELATIONDEFINITION_CLASS*/
};

typedef Dictionary<dbRelationDefinition *> dbRelationDefinitionsList;

#ifdef _GONG_DEBUG
std::ostream &operator<<(std::ostream &, const dbRelationDefinition &);
#endif

/*<<<<<DBRELATIONDEFINITION_POSTAMBLE*/
} // namespace gong
#endif // __GONGDBRELATIONDEFINITION_H
/*>>>>>DBRELATIONDEFINITION_POSTAMBLE*/
