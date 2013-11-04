/*<<<<<REFERENCEID_PREAMBLE*/
#ifndef ___GONGDBFIELDREFERENCEID_H
#define ___GONGDBFIELDREFERENCEID_H
/*>>>>>REFERENCEID_PREAMBLE*/

/*<<<<<COPYLEFT*/
/** @file gongdbfieldreferenceid.h A dbFieldDefinition for RecordIDs
 * Proyecto GestiONG. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */
/*>>>>>COPYLEFT*/

/*<<<<<DBFIELD_REFERENCEID_INCLUDES*/
#include <gongdbfielddefinition.h>
/*>>>>>DBFIELD_REFERENCEID_INCLUDES*/

namespace gong {

class dbFieldReferenceID: public dbFieldDefinition
{
public:
    dbFieldReferenceID(const Xtring &tablename, const Xtring &name,
                       const Xtring &referredfield, dbFieldDefinition::Flags flags = NONE)
        : dbFieldDefinition(tablename, name, SQLINTEGER, 11, 0,
                            flags | REFERENCE, "0")
    {
        setReference(referredfield);
    }

    /*<<<<<DBFIELD_REFERENCEID_MEMBERS*/
	virtual bool isValid( dbRecord *r, dbFieldValue *value,
		ValidResult::Context context, ValidResult *integres) const; // From dbFieldDefinition
/*>>>>>DBFIELD_REFERENCEID_MEMBERS*/

}; // class

/*<<<<<REFERENCEID_POSTAMBLE*/
} // namespace gong
#endif // __GONGDBFIELDREFERENCEID_H
/*>>>>>REFERENCEID_POSTAMBLE*/



