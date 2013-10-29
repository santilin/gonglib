#ifndef DBAPP_REPORTINPUT_H
#define DBAPP_REPORTINPUT_H

/** @file dbappreportinput.h An input for RTK::Report to be used by the application
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include <gongdbconnection.h>
#include <gongdbresultset.h>
#include <gongvariant.h>
#include <rtkproperty.h>
#include <rtkinput.h>

namespace gong {

class ReportInput: public RTK::Input
{
public:
    ReportInput(dbConnection *conn, class RTK::Report &r, const char *name);
    ~ReportInput();
    virtual bool parseAttribute(const char *attribute, const char *value); // from Input
    virtual bool init(); // from Input
    virtual bool next(); // from Input
    virtual Variant getValue( unsigned int nfield, Variant::Type type ) const; // from Input
    /* Override to order the input on this fields */
    virtual bool setOrder(const Xtring &orderby); // from Input
    virtual bool setFilter(const Xtring &filter); // from Input
    // Properties
    const char * host() const {
        return propHost.get();
    }
    void setOrigHost(const char * host )		{
        propHost.setOrig(host);
    }
    int port() const {
        return propPort.get();
    }
    void setOrigPort(const char * port )		{
        propPort.setOrig(port);
    }
    const char * user() const {
        return propUser.get();
    }
    void setOrigUser(const char * user )		{
        propUser.setOrig(user);
    }
    const char * password() const {
        return propPassword.get();
    }
    void setOrigPassword(const char * password )		{
        propPassword.setOrig(password);
    }
    const char * database() const {
        return propDatabase.get();
    }
    void setOrigDatabase(const char * database )		{
        propDatabase.setOrig(database);
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
    virtual void fixInputParameters(const RTK::ParametersList &parameters, const char *delim); // from Input

    // Needed by readRtkFromFile
    const char *getOrigFrom() const {
        return propFrom.getOrig();
    }
    const char *getOrigWhere() const {
        return propWhere.getOrig();
    }
    const char *getOrigOrderBy() const {
        return propOrderBy.getOrig();
    }


protected:
    RTK::FixableProperty<const char *> propHost;
    RTK::FixableProperty<int> propPort;
    RTK::FixableProperty<const char *> propUser;
    RTK::FixableProperty<const char *> propPassword;
    RTK::FixableProperty<const char *> propDatabase;
    RTK::FixableProperty<const char *> propFrom;
    RTK::FixableProperty<const char *> propWhere;
    RTK::FixableProperty<const char *> propOrderBy;
    RTK::FixableProperty<const char *> propGroupBy;
    RTK::FixableProperty<const char *> propPreSQL;
    RTK::FixableProperty<const char *> propPostSQL;

private:
    dbConnection *pConnection;
    dbResultSet *pResultSet;
    unsigned long long mRowNumber;
};

}

#endif // DBAPP_REPORTINPUT_H

