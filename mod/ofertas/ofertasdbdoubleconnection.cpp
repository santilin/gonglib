#include <gongdebug.h>
#include <gongdbresultset.h>
#include "ofertasdbdoubleconnection.h"

namespace gong {
namespace ofertas {

bool dbDoubleConnection::exec(const Xtring& query, bool ignorerrors)
{
    bool ret = dbConnection::exec( query, ignorerrors );
    if( ret && mLinked )
        pOtherConnection->exec(
            translateQuery( query, getSqlDriver(), pOtherConnection->getSqlDriver()), ignorerrors );
    return ret;
}

dbResultSet* dbDoubleConnection::select(const Xtring& query, bool ignorerrors)
{
    if( mLinked )
        return pOtherConnection->select(
                   translateQuery( query, getSqlDriver(), pOtherConnection->getSqlDriver() ), ignorerrors );
    else
        return dbConnection::select( query, ignorerrors );
}

bool dbDoubleConnection::copyTable(bool fromother, const Xtring& tablename, const Xtring &join, const Xtring &condition)
{
    dbConnection *fromConn, *toConn;
    if( fromother ) {
        fromConn = pOtherConnection;
        toConn = this;
    } else {
        fromConn = this;
        toConn = pOtherConnection;
    }
    // Select solo de los campos de la primera tabla en caso de que hay un join
    Xtring sql = "SELECT " + fromConn->nameToSQL(tablename) + ".* FROM " + fromConn->nameToSQL( tablename );
	if( !join.isEmpty() ) {
		sql += join;
	}
	if( !condition.isEmpty() ) {
		sql += " WHERE " + condition;
	}
    dbResultSet *rs = fromConn->select( sql );
    Xtring sqlinsert;
    while( rs->next() ) {
        sqlinsert = "INSERT INTO " + toConn->nameToSQL( tablename ) + " VALUES(";
        for( uint i=0; i < rs->getColumnCount(); ++i ) {
            if( i!=0)
                sqlinsert+=",";
            sqlinsert += toConn->toSQL( rs->getValue(i) );
        }
        sqlinsert += ")";
        toConn->exec( sqlinsert);
    }
    delete rs;
    return true;
}

static const char *translations[][2] = {
//    Sqlite3, MySQL
    { "BEGIN", "BEGIN WORK" }
};

Xtring dbDoubleConnection::translateQuery(const Xtring& query, dbConnection::SqlDriver from, dbConnection::SqlDriver to)
{
    if( from == to )
        return query;
    Xtring trimmed(query);
    trimmed.trim();
    Xtring result;
    bool escaping = false;
    if( from == dbConnection::DRIVER_MYSQL && to == dbConnection::DRIVER_SQLITE3 ) {
        _GONG_DEBUG_PRINT(3, "Translating " + query + " from MYSQL to SQLITE3" );
    } else if( from == dbConnection::DRIVER_SQLITE3 && to == dbConnection::DRIVER_MYSQL ) {
        _GONG_DEBUG_PRINT(3, "Translating " + query + " from SQLITE3 to MYSQL" );
        for ( unsigned int i = 0; i < sizeof( translations ) / sizeof( translations[0] ); i++ )
        {
            if( !strcmp( trimmed.c_str(), translations[i][0] ) )
                return translations[i][1];
        }
        for( Xtring::const_iterator it = query.begin(); it != query.end(); ++it ) {
            if( escaping ) {
                escaping = false;
            } else if( *it == '\\' ) {
                escaping = true;
            } else if( *it == '"' && !escaping ) {
                result += '`';
            } else
                result += *it;
        }
    }
    return result;
}

}
}
