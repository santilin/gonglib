#ifndef RTKOUTPUTMYSQL_H
#define RTKOUTPUTMYSQL_H

/*<<<<<COPYLEFT*/
/** @file rtkoutputmysql.h RTK standard MySql output object
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
#ifdef HAVE_MYSQL

#include <mysql/mysql.h>
#include "rtkoutput.h"

namespace gong {}
namespace RTK = gong;

namespace gong {

class OutputMySql : public Output
{
public:
    OutputMySql(class Report &r, bool createtables = true, const char *database = 0,
                const char *host = 0, const char *user = 0, const char *password = 0,
                const int port = 0, Measure unitsperinchx = 72, Measure unitsperinchy = 72,
                PaperSize papersize = A4, PageOrientation pageorientation = Portrait, Measure sizex = 0,
                Measure sizey = 0, Measure marginleft = 0, Measure marginright = 0,
                Measure margintop = 0, Measure marginbottom = 0)
        :Output(r, unitsperinchx, unitsperinchy,
                papersize, pageorientation, sizex,
                sizey, marginleft, marginright,
                margintop, marginbottom),
        propCreateTables(createtables), propDatabase(database),
        propHost(host), propUser(user), propPassword(password), propPort(port)
    {};

    ~OutputMySql();
    virtual int startReport(); // from Output
    virtual Measure endSection( const Section &section ); // from Output

    /*<<<<<OUTPUTMYSQL_PROPERTIES*/
public:
	bool createTables() const { return propCreateTables.get(); }
	const char * database() const { return propDatabase.get(); }
	const char * host() const { return propHost.get(); }
	const char * user() const { return propUser.get(); }
	const char * password() const { return propPassword.get(); }
	int port() const { return propPort.get(); }

public:
	Property<bool> propCreateTables;
	Property<const char *> propDatabase;
	Property<const char *> propHost;
	Property<const char *> propUser;
	Property<const char *> propPassword;
	Property<int> propPort;
/*>>>>>OUTPUTMYSQL_PROPERTIES*/

private:
    MYSQL mMySql;

};

} // namespace

#endif // ifdef HAVE_MYSQL

#endif

