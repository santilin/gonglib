#ifndef _GONG_DBCONNECTION_H
#define _GONG_DBCONNECTION_H

/** @file gongdbconnection.h Uncomplicated connection to a SQL server
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include "config.h"
#include "gongvariant.h"
#include "gongiconv.h"
#include "gongdberror.h"

namespace gong {

class dbResultSet;

typedef unsigned int dbRecordID;
enum SqlColumnType { SQLSTRING, SQLTEXT, SQLINTEGER, SQLDECIMAL, SQLDATE, SQLDATETIME, SQLTIME, SQLTIMESTAMP, SQLBOOL, SQLFLOAT, SQLBLOB };

class dbConnection
{
public:
    enum SqlDriver { DRIVER_MYSQL, DRIVER_POSTGRESQL, DRIVER_SQLITE3 }; // XBASE
    dbConnection();
    virtual ~dbConnection();
    bool isMySQL() const {
        return mSqlDriver == DRIVER_MYSQL;
    }
    bool isPGSQL() const {
        return mSqlDriver == DRIVER_POSTGRESQL;
    }
    bool isSQLite() const {
        return mSqlDriver == DRIVER_SQLITE3;
    }
    enum SqlDriver getSqlDriver() const {
        return mSqlDriver;
    }

    static XtringList &getDriversList() {
        return sDrivers;
    }

    bool connect(enum SqlDriver driver, const Xtring &user, const Xtring &password,
                 const Xtring &defaultdatabase, const Xtring &host=Xtring(), unsigned int port = 0,
                 const Xtring &options = Xtring::null );
    bool createDatabase( const Xtring &databasename );
    virtual dbResultSet *select(const Xtring &query, bool ignorerrors = false);
    virtual bool exec(const Xtring &query, bool ignorerrors = false);
    bool exec(const XtringList &querys, bool ignorerrors = false);
    const dbError &getLastError() const {
        return mLastError;
    }
    unsigned long getAffectedRows() const {
        return mRowsAffected;
    }
    const Xtring &getEncoding() const {
        return mEncoding;
    }
    void setEncoding(const Xtring &encoding = "UTF-8" );
    IConv *getIConvSelectPtr() const {
        return pIConvSelect;
    }
    IConv *getIConvUpdatePtr() const {
        return pIConvUpdate;
    }

    bool beginTransaction();
    bool commitTransaction();
    bool rollbackTransaction();

    Xtring toSQL( int p_val ) const;
    Xtring toSQL( long int p_val ) const;
    Xtring toSQL( dbRecordID p_val ) const {
        return toSQL( (int)p_val );
    }
    Xtring nameToSQL( const Xtring &name ) const;
    Xtring toSQL( double p_val ) const;
    Xtring toSQL( bool p_val ) const;
    Xtring toSQL( const char *p_val ) const;
    Xtring toSQL( const Xtring &p_val ) const;
    Xtring toSQL( const Time &date ) const;
    Xtring toSQL( const Date &date ) const;
    Xtring toSQL( const DateTime &date ) const;
    Xtring toSQL( Money p_val ) const;
    Xtring toSQL( const Variant &p_val ) const;
    Xtring toSQLStartLike( const Xtring &p_field, const Xtring &p_val, bool negated = false ) const;
    Xtring toSQLLike( const Xtring &p_field, const Xtring &p_val, bool negated = false ) const;
    Xtring toSQLLikeLiteral( const Xtring &p_field, const Xtring &p_val, bool negated = false ) const;
    Xtring timestrToSQL( const Xtring &p_val ) const;

    /* Helper functions to execute simple queries and get the first value */
    dbRecordID getLastInsertID( const Xtring &table, const Xtring &fld );
    int selectInt( const Xtring &select, int nfield = 0 );
    double selectDouble( const Xtring &select, int nfield = 0 );
    Money selectMoney( const Xtring &select, int nfield = 0 );
    Date selectDate( const Xtring &select, int nfield = 0 );
    Xtring selectString( const Xtring &select, int nfield = 0 );
    int selectValues(const Xtring &select, ... ); // the arguments must be variants
    int selectNextInt(const Xtring &tablename, const Xtring &fldname,
                      const Xtring &conds = Xtring::null, bool usegaps = true);
    XtringList selectStringList(int nfields, const Xtring &select);
    bool selectDatabase( const Xtring &dbname );
    bool existsDatabase( const Xtring &databasename );
    bool dropDatabase( const Xtring &dbname, bool ignoreerrors = false );
    bool existsTable( const Xtring &tablename, const Xtring &dbname = Xtring::null );
    static Variant::Type MySQLTypetoVariantType(unsigned mysqltype);
    static Variant::Type SQLiteTypetoVariantType(const char *sqlitetype);
    static Variant::Type toVariantType(SqlColumnType coltype);
    static SqlColumnType toSqlColumnType(Variant::Type coltype);
    static SqlColumnType extractSqlColumnType(const char *sqldef,
            unsigned short int *size, unsigned short int *decimals);
	static const char *sqlColumnTypeToString(SqlColumnType sqltype);
    static XtringList extractSqlTables(const Xtring &select);
    bool createUser( const Xtring &databasename, const Xtring &username,
                     const Xtring &password, const Xtring &host, const Xtring &privileges,
                     bool ignoreerrors = false );
    bool dropUser( const Xtring &databasename, const Xtring &username,
                   bool ignoreerrors = false );
    bool isRootUser() const {
        return mbRootUser;
    }
    void clearError(void);
	static SqlDriver stringToSqlDriver(const char *driver);

private:
    void setError( const Xtring &query = Xtring::null);
    void clearIConv();
    union {
        struct st_mysql *pMySql;
#ifdef HAVE_SQLITE3
        struct sqlite3 *pSqLite;
#endif
    };
    unsigned long mRowsAffected;
    dbError mLastError;
    Xtring mEncoding;
    enum SqlDriver mSqlDriver;
    IConv *pIConvSelect, *pIConvUpdate;
    bool mbRootUser;
    static XtringList sDrivers;
    Xtring mDBName, mHost, mUser, mPassword, mOptions;
    int mPort;
};


extern const char *MYSQL_DATETIME_FORMAT;
extern const char *MYSQL_DATE_FORMAT;
extern const char *MYSQL_TIME_FORMAT;

}; // namespace gong

#endif // _GONG_DBCONNECTION_H
