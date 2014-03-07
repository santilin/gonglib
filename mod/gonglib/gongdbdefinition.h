/*<<<<<DBDEFINITION_PREAMBLE*/
#ifndef ___GONGDBDEFINITION_H
#define ___GONGDBDEFINITION_H
/*>>>>>DBDEFINITION_PREAMBLE*/
/*<<<<<COPYLEFT*/
/** @file gongdbdefinition.h Root of the data dictionary
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

#ifdef _GONG_DEBUG
#include <iostream>
#endif
#include "gongdbtabledefinition.h"
#include "gongdbviewdefinition.h"
#include "gongdbfieldstyle.h"

namespace gong {

class dbRecord;
class dbConnection;

class dbDefinition
{
public:
    /*<<<<<DBDEFINITION_FULLCONSTRUCTOR*/
    dbDefinition(const Xtring& name, const Xtring& description, const Xtring& encoding = Xtring::null, const Xtring& collation = Xtring::null);
    /*>>>>>DBDEFINITION_FULLCONSTRUCTOR*/

public:
    dbTableDefinitionDict &getTables() {
        return mTables;
    }
    dbTableDefinition *getTableDefinition(unsigned int i);
    const dbTableDefinition *getTableDefinition(unsigned int i) const;
    dbTableDefinition *findTableDefinition(const Xtring &tablename, bool warning = true) const;
//  	const dbTableDefinition *findTableDefinition(const Xtring &tablename) const;
    dbTableDefinitionDict &addTable(const dbTableDefinition *tabledef);

    dbFieldDefinition *findFieldDefinition(const Xtring &tablename, const Xtring &fldname, bool warning = false);
    const dbFieldDefinition *findFieldDefinition(const Xtring &tablename,
            const Xtring &fldname, bool warning = false) const;
    dbFieldDefinition *findFieldDefinition(const Xtring &fullfldname, bool warning = false);
    const dbFieldDefinition *findFieldDefinition(const Xtring &fullfldname, bool warning = false) const;

    int readDescriptionsFromPath( const Xtring &language, const Xtring &path, const Xtring &globalpath);
    int readDescriptionsFromFile( const Xtring &filename );
    dbViewDefinitionDict &getViews() {
        return mViews;
    }
    dbViewDefinition *findView( const Xtring &viewname );
    dbViewDefinitionDict &addView( const dbViewDefinition *view );
    int addViewFromString(const Xtring &caption, const Xtring &viewstr, const Xtring &origin);
    int addViewsFromPath( const Xtring &language, const Xtring &path, const Xtring &globalpath);
    int addViewsFromFile( const Xtring &filename );
    int getViewsForTable(const Xtring &tablename, dbViewDefinitionDict &container);
    int getViewsForTable(const dbRecord *r, dbViewDefinitionDict &container);
    int getViewsByName(const Xtring &viewname, dbViewDefinitionDict &container);

    dbFieldStyleDict& getFieldStyles()  {
        return mFieldStyles;
    }
    dbFieldStyle *findFieldStyle( const Xtring &stylename) const;
    dbFieldStyleDict &addFieldStyle( const dbFieldStyle *_style );
    void addStyleFromString( const Xtring &name, const Xtring &styledef );

    static dbDefinition *fromSQLSchema( dbConnection *conn, const Xtring &dbname,
                                        const XtringList &tables = XtringList() );
    bool create( dbConnection *conn, const Xtring &extraargs = Xtring::null,
                 bool ignoreerrors = true);
    bool createTables( dbConnection *conn, const Xtring &extraargs = Xtring::null,
                       bool ifnotexists = true, bool ignoreerrors = true );
    bool createIndexes( dbConnection *conn,	bool ignoreerrors = false );
    bool dropIndexes( dbConnection *conn,	bool removeall, bool ignoreerrors );

    Xtring sameSQLSchema( const dbDefinition *other, dbConnection *conn, bool purging = false ) const;
    bool isRecordUsed(dbRecord *rec, Xtring *usingtable);

    /*<<<<<DBDEFINITION_CLASS*/
public:
	const Xtring& getName() const { return mName; }
	const Xtring& getDescription() const { return mDescription; }
	const Xtring& getEncoding() const { return mEncoding; }
	const Xtring& getCollation() const { return mCollation; }
	const dbTableDefinitionDict& getTables() const { return mTables; }
	const dbViewDefinitionDict& getViews() const { return mViews; }
	const dbFieldStyleDict& getFieldStyles() const { return mFieldStyles; }

	void setName(const Xtring& name) { mName = name; }
	void setDescription(const Xtring& description) { mDescription = description; }
	void setEncoding(const Xtring& encoding) { mEncoding = encoding; }
	void setCollation(const Xtring& collation) { mCollation = collation; }
	void setTables(const dbTableDefinitionDict& tables) { mTables = tables; }
	void setViews(const dbViewDefinitionDict& views) { mViews = views; }
	void setFieldStyles(const dbFieldStyleDict& fieldstyles) { mFieldStyles = fieldstyles; }
private:
	Xtring mName;
	Xtring mDescription;
	Xtring mEncoding;
	Xtring mCollation;
	dbTableDefinitionDict mTables;
	dbViewDefinitionDict mViews;
	dbFieldStyleDict mFieldStyles;
/*>>>>>DBDEFINITION_CLASS*/

};

// Stream functions
#ifdef _GONG_DEBUG
std::ostream &operator<<(std::ostream &, const dbDefinition &);
#endif

/*<<<<<DBDEFINITION_POSTAMBLE*/
} // namespace gong
#endif // __GONGDBDEFINITION_H
/*>>>>>DBDEFINITION_POSTAMBLE*/

