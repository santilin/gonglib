#ifndef _OFERTAS_DBDOUBLECONNECTION_H
#define _OFERTAS_DBDOUBLECONNECTION_H

/** @file ofertasdbdoubleconnection.h Double connection
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include "gongdbconnection.h"

namespace gong {
namespace ofertas {

class dbDoubleConnection: public dbConnection
{
public:
    dbDoubleConnection( dbConnection *conn ) : dbConnection(), pOtherConnection( conn ), mLinked(false) {}
    virtual bool exec(const Xtring &query, bool ignorerrors = false); // from dbConnection
    virtual dbResultSet *select(const Xtring &query, bool ignorerrors = false); // from dbConnection
    void setLinked( bool linked ) {
        mLinked = linked;
    }
    bool isLinked() const {
        return mLinked;
    }
    bool copyTableToOther( const Xtring &tablename, const Xtring &join, const Xtring &condition ) {
        return copyTable( false, tablename, join, condition );
    }
    bool copyTableFromOther( const Xtring &tablename, const Xtring &join, const Xtring &condition ) {
        return copyTable( true, tablename, join, condition );
    }
    Xtring translateQuery( const Xtring &query, dbConnection::SqlDriver from, dbConnection::SqlDriver to);
private:
    bool copyTable( bool fromother, const Xtring &tablename, const Xtring &join, const Xtring &condition );
    dbConnection *pOtherConnection;
    bool mLinked;
};

}; // namespace ofertas
}; // namespace gong

#endif // _OFERTAS_DBDOUBLECONNECTION_H
