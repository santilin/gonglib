#ifndef RTKRTKINPUTXBASE_H
#define RTKRTKINPUTXBASE_H

/*<<<<<COPYLEFT*/
/** @file rtkinputxbase.h Standard xbase input object
 * RTK The report toolkit. Copyright (C) 2003-2013 Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */
/*>>>>>COPYLEFT*/

#include "rtkconfig.h"
#ifdef RTK_HAVE_XBASE

#include <xbsql.h>
#include "rtkinput.h"


namespace gong {}
namespace RTK = gong;

namespace gong {

class InputXBase : public Input
{
public:
    InputXBase(class Report &r, const char * name);
    virtual ~InputXBase();

    virtual bool parseAttribute(const char *attribute, const char *value); // from Input
    virtual bool init(); // from Input
    virtual bool next(); // from Input
    virtual Variant getValue( uint nfield, Variant::Type type ) const; // from Input
    /* Override to order the input on this fields */
    virtual bool setOrder(const Xtring &orderby); // from Input
    static Variant::Type XBaseTypeToVariantType(enum XBSQL::VType vtype);
    static const char *XBASE_DATETIME_FORMAT;
    static const char *XBASE_TIME_FORMAT;
    static const char *XBASE_DATE_FORMAT;


    /*<<<<<INPUTXBASE_PROPERTIES*/
public:
    const char * databasePath() const {
        return propDatabasePath.get();
    }
    void setOrigDatabasePath(const char * databasepath )		{
        propDatabasePath.setOrig(databasepath);
    }
    bool caseSensitive() const {
        return propCaseSensitive.get();
    }
    void setOrigCaseSensitive(const char * casesensitive )		{
        propCaseSensitive.setOrig(casesensitive);
    }
    const char * from() const {
        return propFrom.get();
    }
    void setOrigFrom(const char * from )		{
        propFrom.setOrig(from);
    }
    const char * where() const {
        return propWhere.get();
    }
    void setOrigWhere(const char * where )		{
        propWhere.setOrig(where);
    }
    const char * orderBy() const {
        return propOrderBy.get();
    }
    void setOrigOrderBy(const char * orderby )		{
        propOrderBy.setOrig(orderby);
    }
    const char * groupBy() const {
        return propGroupBy.get();
    }
    void setOrigGroupBy(const char * groupby )		{
        propGroupBy.setOrig(groupby);
    }
    const char * preSQL() const {
        return propPreSQL.get();
    }
    void setOrigPreSQL(const char * presql )		{
        propPreSQL.setOrig(presql);
    }
    const char * postSQL() const {
        return propPostSQL.get();
    }
    void setOrigPostSQL(const char * postsql )		{
        propPostSQL.setOrig(postsql);
    }
    virtual void fixInputParameters(const ParametersList &parameters, const char *delim); // from Input

public:
    FixableProperty<const char *> propDatabasePath;
    FixableProperty<bool> propCaseSensitive;
    FixableProperty<const char *> propFrom;
    FixableProperty<const char *> propWhere;
    FixableProperty<const char *> propOrderBy;
    FixableProperty<const char *> propGroupBy;
    FixableProperty<const char *> propPreSQL;
    FixableProperty<const char *> propPostSQL;
    /*>>>>>INPUTXBASE_PROPERTIES*/

private:
    XBaseSQL *pXBaseSQL;
    XBSQLSelect *pQuery;
    uint mRecordNumber;
};

} // namespace

#endif // ifdef HAVE_XBASE

#endif
