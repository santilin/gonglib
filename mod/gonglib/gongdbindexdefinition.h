/*<<<<<DBINDEXDEFINITION_PREAMBLE*/
#ifndef ___GONGDBINDEXDEFINITION_H
#define ___GONGDBINDEXDEFINITION_H
/*>>>>>DBINDEXDEFINITION_PREAMBLE*/

/*<<<<<COPYLEFT*/
/** @file gongdbindexdefinition.h Index definitions for the data dictionary
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

#include <vector>
#include "gongdictionary.h"
#include "gongdbfielddefinition.h"

#define _GONG_INDEX_PREFIX "_gong_"

namespace gong {

class dbConnection;
class dbTableDefinition;

class dbIndexDefinition
{
public:
    /*<<<<<DBINDEXDEFINITION_FULLCONSTRUCTOR*/
	dbIndexDefinition(const Xtring& name, bool unique, const dbFieldDefinition::Flags& flags = dbFieldDefinition::NONE, const Xtring& options = Xtring::null)
		: mName(name), mUnique(unique), mFlags(flags), mOptions(options)
/*>>>>>DBINDEXDEFINITION_FULLCONSTRUCTOR*/
    {}

    dbFieldDefinition *getFieldDefinition(const Xtring &name)
    {
        return mFieldDefinitions[name];
    }
    const dbFieldDefinition *getFieldDefinition(const Xtring &name) const
    {
        return mFieldDefinitions[name];
    }
    dbFieldDefinition *getFieldDefinition(unsigned int i)
    {
        return mFieldDefinitions[i];
    }
    const dbFieldDefinition *getFieldDefinition(unsigned int i) const
    {
        return mFieldDefinitions[i];
    }
    unsigned int getFieldCount() const
    {
        return mFieldDefinitions.size();
    }

    // Operations
    bool create( dbConnection *conn, const dbTableDefinition *tbldef,
                 bool ignoreerrors = false);
    bool drop( dbConnection *conn, const dbTableDefinition *tbldef,
               bool ignoreerrors = false);

    dbFieldDefinition *addField(const dbFieldDefinition *fielddef);
    dbFieldDefinition *addField(const Xtring &name, unsigned short int width = 0, bool asc = true);

    /*<<<<<DBINDEXDEFINITION_CLASS*/
public:
	const Xtring& getName() const { return mName; }
	bool isUnique() const { return mUnique; }
	const dbFieldDefinition::Flags& getFlags() const { return mFlags; }
	const Xtring& getOptions() const { return mOptions; }
	const dbFieldDefinitionsList& getFieldDefinitions() const { return mFieldDefinitions; }

	dbIndexDefinition* setUnique(bool unique) { mUnique = unique; return this; }
	dbIndexDefinition* setFlags(const dbFieldDefinition::Flags& flags) { mFlags = flags; return this; }
	dbIndexDefinition* setOptions(const Xtring& options) { mOptions = options; return this; }
private:
	Xtring mName;
	bool mUnique;
	dbFieldDefinition::Flags mFlags;
	Xtring mOptions;
	dbFieldDefinitionsList mFieldDefinitions;
/*>>>>>DBINDEXDEFINITION_CLASS*/
};

typedef std::vector<dbIndexDefinition *> dbIndexDefinitionsList;

// Stream functions
#ifdef _GONG_DEBUG
std::ostream &operator<<(std::ostream &, const dbIndexDefinition &);
#endif

/*<<<<<DBINDEXDEFINITION_POSTAMBLE*/
} // namespace gong
#endif // __GONGDBINDEXDEFINITION_H
/*>>>>>DBINDEXDEFINITION_POSTAMBLE*/
