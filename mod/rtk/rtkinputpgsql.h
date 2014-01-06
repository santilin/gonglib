#ifndef RTKINPUTPGSQL_H
#define RTKINPUTPGSQL_H

/*<<<<<COPYLEFT*/
/** @file rtkinputpgsql.h Standard PostGreSQL input object
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
#ifdef HAVE_PGSQL

#include <libpq-fe.h>
#include "rtkinput.h"


namespace gong {}
namespace RTK = gong;

namespace gong {

class InputPgSql : public Input
{
public:
    InputPgSql(class Report &r, const char * name)
        :Input(r, name, "pgsql") , pResultSet( 0 )   {};

    virtual bool parseAttribute(const char *attribute, const char *value); // from Input
    virtual bool init(); // from Input
    virtual bool next(); // from Input
    virtual Variant getValue( uint nfield, Variant::Type type ) const; // from Input


    /*<<<<<INPUTPGSQL_PROPERTIES*/
public:
	const char * host() const { return propHost.get(); }
	void setOrigHost(const char * host )		{ propHost.setOrig(host); }
	int port() const { return propPort.get(); }
	void setOrigPort(const char * port )		{ propPort.setOrig(port); }
	const char * user() const { return propUser.get(); }
	void setOrigUser(const char * user )		{ propUser.setOrig(user); }
	const char * password() const { return propPassword.get(); }
	void setOrigPassword(const char * password )		{ propPassword.setOrig(password); }
	const char * database() const { return propDatabase.get(); }
	void setOrigDatabase(const char * database )		{ propDatabase.setOrig(database); }
	const char * from() const { return propFrom.get(); }
	void setOrigFrom(const char * from )		{ propFrom.setOrig(from); }
	const char * where() const { return propWhere.get(); }
	void setOrigWhere(const char * where )		{ propWhere.setOrig(where); }
	const char * orderBy() const { return propOrderBy.get(); }
	void setOrigOrderBy(const char * orderby )		{ propOrderBy.setOrig(orderby); }
	const char * groupBy() const { return propGroupBy.get(); }
	void setOrigGroupBy(const char * groupby )		{ propGroupBy.setOrig(groupby); }
	const char * preSQL() const { return propPreSQL.get(); }
	void setOrigPreSQL(const char * presql )		{ propPreSQL.setOrig(presql); }
	const char * postSQL() const { return propPostSQL.get(); }
	void setOrigPostSQL(const char * postsql )		{ propPostSQL.setOrig(postsql); }
	virtual void fixInputParameters(const ParametersList &parameters, const char *delim); // from Input

public:
	FixableProperty<const char *> propHost;
	FixableProperty<int> propPort;
	FixableProperty<const char *> propUser;
	FixableProperty<const char *> propPassword;
	FixableProperty<const char *> propDatabase;
	FixableProperty<const char *> propFrom;
	FixableProperty<const char *> propWhere;
	FixableProperty<const char *> propOrderBy;
	FixableProperty<const char *> propGroupBy;
	FixableProperty<const char *> propPreSQL;
	FixableProperty<const char *> propPostSQL;
/*>>>>>INPUTPGSQL_PROPERTIES*/

private:
    PGconn *pConn;
    PGresult *pResultSet;
    int mNRow;
};

} // namespace


#endif // ifdef HAVE_PGSQL

#endif
