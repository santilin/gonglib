#include <cstdarg>
#include <memory>  // auto_ptr<>

#include "config.h"
#include <mysql/mysql.h>
#ifdef HAVE_SQLITE3
# include <sqlite3.h>
#endif

#include "gongfileutils.h"
#include "gongdebug.h"
#include "gongdbresultset.h"
#include "gonglist.h"
#include "gongdbconnection.h"

/**
    @file gongdbconnection.cpp Uncomplicated connection to a SQL server
	@class gong::dbConnection

	@brief This is a very simple class to create a connection to a SQL server (currently only MYSQL is supported).

	It makes it easy to exec selects and get results.
	It handles charset conversions between the server (encoding) and the application (locale) transparently.
	It does not use plugins, everything is hard coded.

	Use \a connect to connect to a SQL server and/or database.
	Use \a select to issue a SELECT statement or \a exec for other types of SQL statements.
	Use \a getLastError to get the error of the last sentence
	Use \a createUser to create users in your database
	Use \a dropDatabase to permanently drop a database

	There are many helper functions to make SELECT and UPDATE statements easy.
*/

namespace gong
{

const char *MYSQL_DATETIME_FORMAT = "%Y-%m-%d %H:%M:%S";
const char *MYSQL_TIME_FORMAT = "%H:%M:%S";
const char *MYSQL_DATE_FORMAT = "%Y-%m-%d";

XtringList dbConnection::sDrivers;

dbConnection::dbConnection()
    : mRowsAffected( 0 ), mLastError( Xtring() ), mSqlDriver( DRIVER_MYSQL ),
      pIConvSelect(0), pIConvUpdate(0), mbRootUser(false)
{
    if( sDrivers.size() == 0 )
        sDrivers << "MYSQL" << "POSTGRESQL" << "SQLITE3";
    pMySql = 0;
#ifdef HAVE_SQLITE3
    pSqLite = 0;
#endif
}

void dbConnection::clearIConv()
{
    if( pIConvSelect ) {
        delete pIConvSelect;
        pIConvSelect = 0;
    }
    if( pIConvUpdate ) {
        delete pIConvUpdate;
        pIConvUpdate = 0;
    }
}

dbConnection::~dbConnection()
{
    if( isMySQL() )
        ::mysql_close( pMySql ); // it is also gnu-deleted here
#ifdef HAVE_SQLITE3
    if( isSQLite() ) {
        int error = sqlite3_close( pSqLite );
        if( error != SQLITE_OK )
            setError( "deleting connection" );
    }
#endif
    clearIConv();
}

/**
 * @brief ...
 *
 * @param driver ...
 * @param user ...
 * @param password ...
 * @param dbname ...
 * @param host ...
 * @param port ...
 * @param options Semicolon separated list of database options
 * @return bool
 **/
bool dbConnection::connect( enum SqlDriver driver, const Xtring &user, const Xtring &password,
                            const Xtring &dbname, const Xtring &host, unsigned int port,
                            const Xtring &options )
{
    Xtring constr;
    clearIConv();
    clearError();
    _GONG_DEBUG_PRINT( 2, Xtring::printf( "db=%s, user=%s, host=%s, port=%d",
                                          dbname.c_str(), user.c_str(), host.c_str(), port ) );
    mHost = host;
    mPassword = password;
    mUser = user;
    mPort = port;
    mOptions = options.upper();
    mDBName = dbname;
    switch ( mSqlDriver = driver ) {
    case DRIVER_MYSQL:
        if( pMySql != 0 ) {
            ::mysql_close( pMySql );
            pMySql = 0;
        }
        if( (pMySql = ::mysql_init( pMySql )) ) {
            pMySql->reconnect = 1;
            ::mysql_options( pMySql, MYSQL_OPT_COMPRESS, NULL );
        } else
            setError( mDBName );
        if ( !::mysql_real_connect( pMySql, host.c_str(), user.c_str(), password.c_str(),
                                    dbname.c_str(), port, 0, 0 ) ) {
            setError( mDBName );
            return false;
        } else {
            if( !dbname.isEmpty() )
                setEncoding( selectString("show variables like 'character_set_database'", 1) );
// 	#define MYSQL_VERSION_ID		50534
#if MYSQL_VERSION_ID >= 50007
            // This is critical in order to have all the strings correctly saved on the server
			mysql_set_character_set( pMySql, "utf8");
#else
            exec( "SET character_set_connection = 'utf8'", true );
            exec( "SET character_set_results = 'utf8'", true );
            exec( "SET NAMES 'utf8'", true );
#endif
        }
        mbRootUser = ( user == "root" );
        break;
    case DRIVER_POSTGRESQL:
        _GONG_DEBUG_ASSERT( 1 == 0 ); // No está compilado
        break;
#ifdef HAVE_SQLITE3
    case DRIVER_SQLITE3:
        if( pSqLite != 0 ) {
            if( sqlite3_close( pSqLite ) != SQLITE_OK )
                setError( "closing");
            pSqLite = 0;
        }
        clearError();
        if( mDBName.isEmpty() )
            return true;
        Xtring fname = mHost;
        if( !fname.isEmpty() )
            FileUtils::addSeparator( fname );
        fname += dbname;
		if( !fname.endsWith( ".sql3" ) )
			fname += ".sql3";
        int error;
        XtringList options;
        mOptions.tokenize( options, ";" );
        if( options.contains( "CREATEDATABASE") )
            error = sqlite3_open_v2( fname.c_str(), &pSqLite, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, 0 );
		else
            error = sqlite3_open_v2( fname.c_str(), &pSqLite, SQLITE_OPEN_READWRITE, 0 );
        if( error != SQLITE_OK ) {
            setError( fname );
            return false;
        }
        setEncoding( "UTF8" );
#endif
    }
    return true;
}

void dbConnection::setEncoding(const Xtring &encoding)
{
    Xtring can_encoding = IConv::canonicalCodeName(encoding.c_str());
    if( can_encoding == mEncoding && pIConvSelect && pIConvUpdate )
        return;
    mEncoding = can_encoding;
    clearIConv();
    if( !mEncoding.isEmpty() && mEncoding != "UTF-8" ) {
        pIConvSelect = new IConv("UTF-8", mEncoding );
        pIConvUpdate = new IConv(mEncoding, "UTF-8" );
    }
}

dbResultSet *dbConnection::select( const Xtring &query, bool ignoreerrors )
{
    _GONG_DEBUG_ASSERT( this );
    clearError();
    switch( mSqlDriver ) {
    case DRIVER_MYSQL:
        if ( !::mysql_real_query( pMySql, query.c_str(), query.length() ) ) {
            ::MYSQL_RES * res = ::mysql_store_result( pMySql );
            _GONG_DEBUG_PRINT(2, Xtring::printf( "My:(%lu rows):%s",
                                                 ( unsigned long ) ::mysql_num_rows( res ), query.c_str() ) );
            return new dbResultSet( this, res );
        } else {
            setError( query );
			_GONG_DEBUG_WARNING( mLastError.getWholeError());
            if ( !ignoreerrors )
                throw mLastError;
        }
        break;
#ifdef HAVE_SQLITE3
    case DRIVER_SQLITE3: {
        if( query.upper().trim() ==  "SHOW TABLES" )
            return select( "SELECT name FROM sqlite_master WHERE type='table'" );
        else if( query.upper().trim() ==  "SHOW DATABASES" )
            return select( "PRAGMA database_list" );
        else if( query.upper().trim().startsWith("SHOW INDEXES FROM" ) )
			return select( "SELECT name FROM sqlite_master WHERE type='index' and name='" + query.trim().mid(18) + "'" );
        sqlite3_stmt *ppSmt = 0;
		_GONG_DEBUG_PRINT(2, Xtring::printf( "S3:%s", query.c_str() ) );
        int ret = sqlite3_prepare_v2( pSqLite, query.c_str(), query.size() + 1, &ppSmt, 0 );
        if( ret == SQLITE_OK && ppSmt ) {
            return new dbResultSet( this, ppSmt );
        } else {
            setError( query );
			_GONG_DEBUG_WARNING( mLastError.getWholeError());
            if( ppSmt )
                sqlite3_finalize( ppSmt );
            if ( !ignoreerrors )
                throw mLastError;
        }
        break;
    }
#endif
	default:
		break;
    }
    return 0;
}

// Las líneas que comienzan con # son comentarios
bool dbConnection::exec( const XtringList &querys, bool ignoreerrors )
{
    bool ret = false;
    for ( XtringList::const_iterator it = querys.begin(); it != querys.end(); ++it ) {
        if ( !it->startsWith( "#" ) && !Xtring(*it).trim().isEmpty() )  {
            if( it->find( "DROP INDEX" ) ) {
                if( !exec( *it, true ) )
                    ret = false;
            } else {
                if( !exec( *it, ignoreerrors ) )
                    ret = false;
            }
        }
    }
    return ret;
}

bool dbConnection::exec( const Xtring &query, bool ignoreerrors )
{
    bool ret = true;
    clearError();
    switch( mSqlDriver ) {
    case DRIVER_MYSQL:
        if ( !::mysql_real_query( pMySql, query.c_str(), query.length() ) ) {
            _GONG_DEBUG_PRINT(1, Xtring::printf( "My:(%lu rows):%s ", ( unsigned long ) ::mysql_affected_rows( pMySql ), query.c_str() ) );
            mRowsAffected = ::mysql_affected_rows( pMySql );
        } else {
            setError( query );
			_GONG_DEBUG_WARNING( mLastError.getWholeError() );
            ret = false;
            if ( !ignoreerrors )
                throw mLastError;
        }
        break;
#ifdef HAVE_SQLITE3
    case DRIVER_SQLITE3: {
        if( query.upper().startsWith( "CREATE DATABASE" ) ) {
            Xtring dbname = query.mid( 16 ).trim();
            ret = connect( mSqlDriver, mUser, mPassword, dbname,
                           mHost, mPort, mOptions + ";CREATEDATABASE" );
        } else {
            sqlite3_stmt *ppSmt = 0;
            int ret = sqlite3_prepare_v2( pSqLite, query.c_str(), query.size() + 1, &ppSmt, 0 );
            if( ret == SQLITE_OK && ppSmt ) {
                switch( sqlite3_step( ppSmt ) ) {
                case SQLITE_BUSY:
                    break;
                case SQLITE_DONE:
                    _GONG_DEBUG_PRINT(1, Xtring::printf( "S3:(%d rows):%s ", sqlite3_changes( pSqLite ), query.c_str() ) );
                    ret = true;
                    break;
                case SQLITE_ROW:
                    ret = true;
                    break;
                case SQLITE_ERROR:
                    setError( query );
					_GONG_DEBUG_WARNING( mLastError.getWholeError() );
                    ret = false;
                    break;
                }
            } else {
				_GONG_DEBUG_WARNING( mLastError.getWholeError() );
                setError( query );
				ret = false;
            }
            if( ppSmt )
                sqlite3_finalize( ppSmt );
        }
        break;
    }
#endif
	default:
		break;
    }
    return ret;
}

void dbConnection::clearError( void )
{
    mLastError = dbError( Xtring() );
    mRowsAffected = 0L;
}

void dbConnection::setError( const Xtring &query )
{
    switch ( mSqlDriver ) {
    case DRIVER_MYSQL:
        mLastError = dbError( ::mysql_error( pMySql ), ::mysql_errno( pMySql ), query );
        break;
#ifdef HAVE_SQLITE3
    case DRIVER_SQLITE3:
    {
#if 0
#define SQLITE_ERROR        1   /* SQL error or missing database */
#define SQLITE_INTERNAL     2   /* Internal logic error in SQLite */
#define SQLITE_PERM         3   /* Access permission denied */
#define SQLITE_ABORT        4   /* Callback routine requested an abort */
#define SQLITE_BUSY         5   /* The database file is locked */
#define SQLITE_LOCKED       6   /* A table in the database is locked */
#define SQLITE_NOMEM        7   /* A malloc() failed */
#define SQLITE_READONLY     8   /* Attempt to write a readonly database */
#define SQLITE_INTERRUPT    9   /* Operation terminated by sqlite3_interrupt()*/
#define SQLITE_IOERR       10   /* Some kind of disk I/O error occurred */
#define SQLITE_CORRUPT     11   /* The database disk image is malformed */
#define SQLITE_NOTFOUND    12   /* Unknown opcode in sqlite3_file_control() */
#define SQLITE_FULL        13   /* Insertion failed because database is full */
#define SQLITE_CANTOPEN    14   /* Unable to open the database file */
#define SQLITE_PROTOCOL    15   /* Database lock protocol error */
#define SQLITE_EMPTY       16   /* Database is empty */
#define SQLITE_SCHEMA      17   /* The database schema changed */
#define SQLITE_TOOBIG      18   /* String or BLOB exceeds size limit */
#define SQLITE_CONSTRAINT  19   /* Abort due to constraint violation */
#define SQLITE_MISMATCH    20   /* Data type mismatch */
#define SQLITE_MISUSE      21   /* Library used incorrectly */
#define SQLITE_NOLFS       22   /* Uses OS features not supported on host */
#define SQLITE_AUTH        23   /* Authorization denied */
#define SQLITE_FORMAT      24   /* Auxiliary database format error */
#define SQLITE_RANGE       25   /* 2nd parameter to sqlite3_bind out of range */
#define SQLITE_NOTADB      26   /* File opened that is not a database file */
#define SQLITE_NOTICE      27   /* Notifications from sqlite3_log() */
#define SQLITE_WARNING     28   /* Warnings from sqlite3_log() */
#define SQLITE_ROW         100  /* sqlite3_step() has another row ready */
#define SQLITE_DONE        101  /* sqlite3_step() has finished executing */
#endif
        int errno = sqlite3_errcode( pSqLite );
        switch( errno ) {
        case SQLITE_CANTOPEN: /* 14 */
            errno = 1049;
            break;
        }
        mLastError = dbError( sqlite3_errmsg( pSqLite ), errno, query );
        if( mLastError.getNumber() != 1091 ) {
            _GONG_DEBUG_WARNING( Xtring::printf( "(%s):%d:%s",
                                                 query.c_str(), mLastError.getNumber(), mLastError.what() ) );
        }
        break;
    }
#endif
    case DRIVER_POSTGRESQL:
        break;
    }
}

bool dbConnection::selectDatabase( const Xtring &dbname )
{
    switch ( mSqlDriver ) {
    case DRIVER_MYSQL:
        exec( "USE " + dbname, true );
        if( mLastError.getNumber() == 0 )
            setEncoding( selectString("show variables like 'character_set_database'", 1) );
        return ( mLastError.getNumber() == 0 );
    case DRIVER_POSTGRESQL:
        break;
#ifdef HAVE_SQLITE3
    case DRIVER_SQLITE3:
        if( dbname == mDBName )
            return true;
        else
            return connect( mSqlDriver, mUser, mPassword, dbname, mHost, mPort, mOptions );
#endif
    }
    return false;
}

bool dbConnection::existsDatabase( const Xtring &databasename )
{
    switch ( mSqlDriver ) {
    case DRIVER_MYSQL: {
        /// TODO: usar mysql_list_databases
        std::auto_ptr<dbResultSet> rs( select( "SHOW DATABASES" ) );
        if( rs->next() ) {
            if ( rs->toString(0).upper() == databasename.upper() ) {
                return true;
            }
        }
    }
    break;
    case DRIVER_POSTGRESQL:
        return ( selectInt( "SELECT COUNT(*) FROM pg_database "
                            "WHERE datname=" + toSQL( databasename ) ) != 0 );
#ifdef HAVE_SQLITE3
    case DRIVER_SQLITE3:
        Xtring fname = mHost;
        if( !fname.isEmpty() )
            FileUtils::addSeparator(fname);
        fname += databasename + ".sql3";
        return FileUtils::exists( fname.c_str() );
#endif
    }
    return false;
}

bool dbConnection::existsTable(const Xtring& tablename, const Xtring& dbname)
{
    switch ( mSqlDriver ) {
    case DRIVER_MYSQL: {
        /// TODO: usar mysql_list_tables
        Xtring sql = "SHOW FIELDS FROM ";
        if( !dbname.isEmpty() )
            sql += dbname + ".";
        sql += tablename;
        try {
            select( sql );
            return true;
        } catch( dbError &e ) {
            return false;
        }
    }
    break;
    case DRIVER_POSTGRESQL:
        return false;
#ifdef HAVE_SQLITE3
    case DRIVER_SQLITE3:
        Xtring where = "WHERE type='table' AND name='" + tablename + "'";
        Xtring sql = "SELECT name FROM sqlite_master " + where
                     + "UNION ALL SELECT name FROM sqlite_temp_master " + where;
        return !selectString( sql ).isEmpty();
#endif
    }
    return false;
}

/**
	Converts a sql backend name to sql syntax. This should be used more throroughly
 */
Xtring dbConnection::nameToSQL( const Xtring &p_val ) const
{
    Xtring ret = p_val;
    switch ( mSqlDriver ) {
    case DRIVER_MYSQL:
        return "`" + ret.replace(".", "`.`") + "`";
#ifdef HAVE_SQLITE3
    case DRIVER_SQLITE3:
        ret = p_val;
        if( !ret.isEmpty() && ret.left(1) != "\"" && ret.right(1) != "\"" ) {
            ret.replace("\"", "\"\"");
            ret = "\"" + ret + "\"";
            ret.replace(".", "\".\"" );
        }
        break;
#endif
    default:
        ret = toSQL( p_val );
    }
    return ret;
}

/* Convert values to SQL strings */
Xtring dbConnection::toSQL( const Xtring &p_val ) const
{
    Xtring ret;
    if( getIConvUpdatePtr() )
        ret = getIConvUpdatePtr()->convert(p_val);
    else
        ret = p_val;
    switch ( mSqlDriver ) {
    case DRIVER_MYSQL:
    {
        ret.trimRight();
        char *buffer = new char[ret.length() * 2 + 3 ];
        *buffer = '\'';
        ::mysql_real_escape_string( pMySql, buffer + 1,	ret.c_str(), ret.length() );
        strcat( buffer, "\'");
        ret = Xtring( buffer );
        delete [] buffer;
    }
    break;
    case DRIVER_POSTGRESQL:
        ret = p_val;
        ret.replace( "\\", "\\\\" );
        ret.replace( "'", "''" );
        ret = "'" + ret + "'";
        break;
#ifdef HAVE_SQLITE3
    case DRIVER_SQLITE3:
        char *pret = sqlite3_mprintf("%Q", ret.c_str() );
        ret = pret;
        sqlite3_free( pret );
        break;
#endif
    }
    return ret;
}

Xtring dbConnection::toSQL( const char *p_val ) const
{
    return toSQL( Xtring( p_val ) );
}

Xtring dbConnection::toSQLStartLike( const Xtring &p_field, const Xtring &p_val, bool negated ) const
{
    Xtring ret;
    if( getIConvUpdatePtr() )
        ret = getIConvUpdatePtr()->convert(p_val);
    else
        ret = p_val;
    switch ( mSqlDriver ) {
    case DRIVER_MYSQL:
    {
        char buffer[ ret.length() * 2 + 1 ];
        ::mysql_real_escape_string( pMySql, buffer, ret.c_str(), ret.length() );
        ret = nameToSQL(p_field) + (negated ? " NOT " : "" ) + " LIKE '" + buffer + "%'"; // MySql is case insensitive
        _GONG_DEBUG_PRINT(4, ret.c_str() );
    }
    break;
    case DRIVER_POSTGRESQL:
        ret.replace( "\\", "\\\\" );
        ret.replace( "'", "''" );
        ret = p_field + (negated ? " NOT " : "" ) + " ILIKE '" + ret + "%'";
        break;
#ifdef HAVE_SQLITE3
    case DRIVER_SQLITE3:
        char *pret = sqlite3_mprintf("%q", ret.c_str() );
        ret = pret;
        sqlite3_free( pret );
        ret = nameToSQL( p_field ) + (negated ? " NOT " : "" ) + " LIKE \'" + ret + "%\'"; // case insensitive
        break;
#endif
    }
    return ret;
}

/**
 * @brief Creates a sql condition in the form FIELD LIKE '%p_val_word1%p_val_word2%...%'.
 *
 *
 * @param p_field The field name
 * @param p_val The value
 * @param negated whether the condition is negated
 * @return Xtring
 **/
Xtring dbConnection::toSQLLike( const Xtring &p_field, const Xtring &p_val, bool negated ) const
{
    Xtring ret;
    if( getIConvUpdatePtr() )
        ret = getIConvUpdatePtr()->convert(p_val);
    else
        ret = p_val;
    switch ( mSqlDriver ) {
    case DRIVER_MYSQL:
    {
        char buffer[ ret.length() * 2 + 1 ];
        ::mysql_real_escape_string( pMySql, buffer, ret.c_str(), ret.length() );
        ret = nameToSQL(p_field) + (negated ? " NOT ":"" ) + " LIKE '%" + Xtring(buffer).replace(" ", "%") + "%'"; // MySql is case insensitive
    }
    break;
    case DRIVER_POSTGRESQL:
        ret.replace( "\\", "\\\\" );
        ret.replace( "'", "''" );
        ret = nameToSQL(p_field) + (negated ? " NOT ":"" ) + " ILIKE '%" + ret + "%'";
        break;
#ifdef HAVE_SQLITE3
    case DRIVER_SQLITE3:
        char *pret = sqlite3_mprintf("%q", ret.c_str() );
        ret = pret;
        sqlite3_free( pret );
        ret = nameToSQL(p_field) + (negated ? " NOT ":"" ) + " LIKE '%" + Xtring(ret).replace(" ", "%") + "%'"; // case insensitive
        break;
#endif
    }
    return ret;
}

/**
 * @brief Creates a sql condition in the form FIELD LIKE 'p_val'
 *
 * @param p_field The field name
 * @param p_val The value
 * @param negated whether the condition is negated
 * @return Xtring
 **/
Xtring dbConnection::toSQLLikeLiteral(const gong::Xtring& p_field, const gong::Xtring& p_val, bool negated) const
{
    Xtring ret;
    if( getIConvUpdatePtr() )
        ret = getIConvUpdatePtr()->convert(p_val);
    else
        ret = p_val;
    switch ( mSqlDriver ) {
    case DRIVER_MYSQL:
    {
        char buffer[ ret.length() * 2 + 1 ];
        ::mysql_real_escape_string( pMySql, buffer, ret.c_str(), ret.length() );
        ret = nameToSQL(p_field)  + (negated ? " NOT ":"" ) + " LIKE '" + buffer + "'"; // MySql is case insensitive
    }
    break;
    case DRIVER_POSTGRESQL:
        ret.replace( "\\", "\\\\" );
        ret.replace( "'", "''" );
        ret = p_field + (negated ? " NOT ":"" ) + " ILIKE '" + ret + "'";
        break;
#ifdef HAVE_SQLITE3
    case DRIVER_SQLITE3:
        ret += "%";
        char *pret = sqlite3_mprintf("%q", ret.c_str() );
        ret = pret;
        sqlite3_free( pret );
        ret = nameToSQL(p_field) + (negated ? " NOT ":"" ) + " LIKE '" + ret + "'"; // case insensitive
        break;
#endif
    }
    return ret;

}


Xtring dbConnection::toSQL( int p_val ) const
{
    return Xtring::number( p_val );
}

Xtring dbConnection::toSQL( double p_val ) const
{
    // sprintf uses the locale to format numbers, but the server should use the POSIX locale
    char *locale_save = setlocale( LC_NUMERIC, "C" );
    Xtring ret = Xtring::number( p_val );
    setlocale( LC_NUMERIC, locale_save );
    return ret;
}

Xtring dbConnection::toSQL( Money p_val ) const
{
    return toSQL( p_val.toDouble() );
}


Xtring dbConnection::toSQL( bool p_val ) const
{
    return p_val ? "1" : "0";
}


Xtring dbConnection::timestrToSQL( const Xtring &p_val ) const
{
    DateTime date = DateTime( p_val );
    return toSQL( date );
}

Xtring dbConnection::toSQL( const Date &date ) const
{
    return "'" + date.toString( MYSQL_DATE_FORMAT ) + "'";
}

Xtring dbConnection::toSQL( const Time &time ) const
{
    return "'" + time.toString( MYSQL_TIME_FORMAT ) + "'";
}

Xtring dbConnection::toSQL( const DateTime &date ) const
{
    return "'" + date.toString( MYSQL_DATETIME_FORMAT ) + "'";
    /*		else if ( isPSQL() )
    		{
    			QDate dt = date.date();
    			QTime tm = date.time();
    			return "'" + Xtring::number( dt.year() ) + "-" +
    			       Xtring::number( dt.month() ) + "-" +
    			       Xtring::number( dt.day() ) + " " +
    			       tm.toString() + "." +
    			       Xtring::number( tm.msec() ).rightJustify( 3, '0' ) + "'";
    		}
    		else
    		{
    			assert( "Unknown driver" );
    			return Xtring();
    		}*/
}

Xtring dbConnection::toSQL( const Variant &p_val ) const
{
    Variant v = Variant();
    switch ( p_val.type() ) {
    case Variant::tInt:
        return toSQL( p_val.toInt() );
    case Variant::tDouble:
        return toSQL( p_val.toDouble() );
    case Variant::tDate:
    case Variant::tTime:
    case Variant::tDateTime:
        return toSQL( p_val.toDateTime() );
    case Variant::tMoney:
    default:
        return toSQL( p_val.toString() );
    }
}


dbRecordID dbConnection::getLastInsertID( const Xtring &table, const Xtring &fld )
{
    switch ( mSqlDriver ) {
    case DRIVER_MYSQL:
        return selectInt( "SELECT LAST_INSERT_ID()" );
    case DRIVER_POSTGRESQL:
        return selectInt( "SELECT last_value FROM " + table.lower() + "_" + fld.lower() + "_seq" );
#ifdef HAVE_SQLITE3
    case DRIVER_SQLITE3:
        return sqlite3_last_insert_rowid( pSqLite );
        break;
#endif
    default:
        return 0;
    }
}

int dbConnection::selectInt( const Xtring &sql, int nfield )
{
    std::auto_ptr<dbResultSet> rs( select( sql ) );
    if( rs->next() )
        return rs->toInt( nfield );
    else
        return 0;
}

double dbConnection::selectDouble( const Xtring &sql, int nfield )
{
    std::auto_ptr<dbResultSet> rs( select( sql ) );
    if ( rs->next() )
        return rs->toDouble( nfield );
    else
        return 0.0;
}

Money dbConnection::selectMoney(const Xtring& sql, int nfield)
{
    std::auto_ptr<dbResultSet> rs( select( sql ) );
    if ( rs->next() )
        return rs->toMoney( nfield );
    else
        return Money();
}


Date dbConnection::selectDate(const Xtring & sql, int nfield)
{
    std::auto_ptr<dbResultSet> rs( select( sql ) );
    if ( rs->next() )
        return rs->toDate( nfield );
    else
        return Date();
}

XtringList dbConnection::selectStringList( int nfields, const Xtring &sql )
{
    XtringList ret;
    std::auto_ptr<dbResultSet> rs( select( sql ) );
    while( rs->next() ) {
        Xtring astring;
        for ( int i = 0; i < nfields; i++ ) {
            if( i!=0 )
                astring +=", ";
            astring += rs->toString( i );
        }
        ret << astring;
    }
    return ret;
}

Xtring dbConnection::selectString( const Xtring & sql, int nfield )
{
    std::auto_ptr<dbResultSet> rs( select( sql ) );
	_GONG_DEBUG_ASSERT( nfield < rs->getColumnCount() );
    if ( rs->next() && nfield < rs->getColumnCount() )
        return rs->toString( nfield );
    else
        return Xtring();
}


bool dbConnection::dropDatabase( const Xtring &dbname, bool ignoreerrors )
{
    try {
        switch ( mSqlDriver ) {
        case DRIVER_MYSQL:
        {
            dbResultSet *rs = select( "SELECT user FROM mysql.db WHERE Db=" + toSQL( dbname ) );
            while( rs->next() )
                dropUser( dbname, rs->toString(0), true );
            delete rs;
            return exec( "DROP DATABASE " + dbname, ignoreerrors );
        }
#ifdef HAVE_SQLITE3
        case DRIVER_SQLITE3:
            /// TODO
            // unlink( sqlite3_db_filename( pSqLite, "main" ) );
            break;
#endif
		default:
			break;
        }
    } catch ( dbError e ) {
        if ( !ignoreerrors )
            throw;
    }
    return false;
}

/** Execute a select query and places the values of the fields
	of the first row (if any ) in the parameters

	Example:

	<pre>
	Variant depname(Xtring()); depcode( 0 );
	selectValues("SELECT depname, depcode FROM department", depname, depcode);
	<pre>

	@return Number of rows in the query

*/
int dbConnection::selectValues( const Xtring & statement, ... )
{
    va_list fieldtypes;
    va_start( fieldtypes, statement );
    std::auto_ptr<dbResultSet> rs( select( statement ) );
    if( rs->next() ) {
        for ( unsigned int i = 0; i < rs->getColumnCount(); i++ ) {
            Variant *pvariant = va_arg( fieldtypes, Variant * );
            if ( pvariant == 0 )
                break;
            Variant::Type t = pvariant->type();
            if( t == Variant::tInvalid )
                t = toVariantType( rs->getColumnType(i));
            switch ( t ) {
            case Variant::tBool:
                *pvariant = rs->toBool( i );
                break;
            case Variant::tDate:
                *pvariant = Date( rs->toString( i ) );
                break;
            case Variant::tTime:
                *pvariant = Time( rs->toString( i ) );
                break;
            case Variant::tDateTime:
                *pvariant = DateTime( rs->toString( i ) );
                break;
            case Variant::tDouble:
                *pvariant = rs->toDouble( i );
                break;
            case Variant::tInt:
            case Variant::tLong:
                *pvariant = rs->toInt( i );
                break;
            case Variant::tMoney:
                *pvariant = Money( rs->toDouble( i ) );
                break;
            case Variant::tString:
                *pvariant = rs->toString( i ).c_str();
                break;
            case Variant::tBinary:
                *pvariant = rs->toBinary( i );
                break;
            case Variant::tInvalid:
                break;
            }
        }
    }
    va_end( fieldtypes );
    return rs->getRowCount();
}

int dbConnection::selectNextInt( const Xtring & tablename, const Xtring & fldname,
                                 const Xtring &conds, bool usegaps )
{
    Xtring sql;
    if( usegaps )
        sql = "SELECT " + nameToSQL( fldname );
    else
        sql = "SELECT MAX(" + nameToSQL( fldname ) + ")";
    sql+= " FROM " + nameToSQL( tablename );
    if( usegaps || !conds.isEmpty() )
        sql+=" WHERE ";
    if( !conds.isEmpty() )
        sql+="(" + conds + ")";
    if( usegaps ) {
        if( !conds.isEmpty() )
            sql+=" AND ";
        sql+= "(" + nameToSQL( fldname ) + "+1 NOT IN "
              "(SELECT " + nameToSQL( fldname ) + " FROM " + nameToSQL( tablename );
        if( !conds.isEmpty() )
            sql+= "WHERE (" + conds + ")";
        sql+= ") )";
    }
    try {
		return selectInt( sql ) + 1;
	} catch( dbError &e ) {
		if( e.getNumber() == 1137 ) { // ERROR 1137 (HY000): Can't reopen table:
			sql = "SELECT MAX(" + nameToSQL( fldname ) + ")";
			sql+= " FROM " + nameToSQL( tablename );
			if( !conds.isEmpty() )
				sql+=" WHERE (" + conds + ")";
			return selectInt( sql ) + 1;
		} else throw;
	}
}

/// \todo {0.4} Nested transactions
bool dbConnection::beginTransaction()
{
    switch ( mSqlDriver ) {
    case DRIVER_MYSQL:
        exec( "BEGIN WORK" );
        break;
#ifdef HAVE_SQLITE3
    case DRIVER_SQLITE3:
        exec( "BEGIN" );
        break;
#endif
	default:
		break;
    }
    return (mLastError.getNumber() == 0);
}

bool dbConnection::commitTransaction()
{
    exec( "COMMIT" );
    return (mLastError.getNumber() == 0);
}

bool dbConnection::rollbackTransaction()
{
    exec( "ROLLBACK" );
    return (mLastError.getNumber() == 0);
}

#ifdef HAVE_SQLITE3
Variant::Type dbConnection::SQLiteTypetoVariantType(const char *sqlitetype)
{
    if( strcasecmp(sqlitetype, "integer" ) == 0
            || strcasecmp(sqlitetype, "int" ) == 0 )
        return Variant::tInt;
    if( strcasecmp(sqlitetype, "double" ) == 0
            || strcasecmp(sqlitetype, "float" ) == 0
            || strcasecmp(sqlitetype, "real" ) == 0 )
        return Variant::tDouble;
    if( strncasecmp( sqlitetype, "numeric", 7) == 0 )
        return Variant::tMoney;
    if( strcasecmp(sqlitetype, "blob" ) == 0 )
        return Variant::tBinary;
    return Variant::tString;
}
#endif

Variant::Type dbConnection::MySQLTypetoVariantType( unsigned mysqltype )
{
    // refman-5.1-en.a4.pdf, page 1239
    switch ( mysqltype ) {
    case MYSQL_TYPE_TINY:
    case MYSQL_TYPE_BIT:
        return Variant::tBool;
    case MYSQL_TYPE_SHORT:
    case MYSQL_TYPE_LONG:
    case MYSQL_TYPE_INT24:
    case MYSQL_TYPE_YEAR:
        return Variant::tInt;
    case MYSQL_TYPE_LONGLONG:
        return Variant::tLong;
    case MYSQL_TYPE_DECIMAL:
    case MYSQL_TYPE_NEWDECIMAL:
        return Variant::tMoney;
    case MYSQL_TYPE_FLOAT:
    case MYSQL_TYPE_DOUBLE:
        return Variant::tDouble;
    case MYSQL_TYPE_DATE:
    case MYSQL_TYPE_NEWDATE:
        return Variant::tDate;
    case MYSQL_TYPE_TIME:
        return Variant::tTime;
    case MYSQL_TYPE_DATETIME:
    case MYSQL_TYPE_TIMESTAMP:
        return Variant::tDateTime;
    case MYSQL_TYPE_STRING:
    case MYSQL_TYPE_VAR_STRING:
    case MYSQL_TYPE_VARCHAR:
    case MYSQL_TYPE_SET:
    case MYSQL_TYPE_ENUM:
        return Variant::tString;
    case MYSQL_TYPE_BLOB:
    case MYSQL_TYPE_TINY_BLOB:
    case MYSQL_TYPE_MEDIUM_BLOB:
    case MYSQL_TYPE_LONG_BLOB:
        return Variant::tBinary;
    case MYSQL_TYPE_GEOMETRY:
    case MYSQL_TYPE_NULL:
        return Variant::tInvalid;
    default:
        _GONG_DEBUG_WARNING( Xtring::printf( "Type %d not recognized", mysqltype ) );
        return Variant::tInvalid;
    }
}

Variant::Type dbConnection::toVariantType(SqlColumnType t)
{
    switch( t ) {
    case SQLTEXT:
    case SQLSTRING:
        return Variant::tString;
    case SQLINTEGER:
        return Variant::tInt;
    case SQLDECIMAL:
        return Variant::tMoney;
    case SQLDATE:
        return Variant::tDate;
    case SQLDATETIME:
    case SQLTIMESTAMP:
        return Variant::tDateTime;
    case SQLTIME:
        return Variant::tTime;
    case SQLBOOL:
        return Variant::tBool;
    case SQLFLOAT:
        return Variant::tDouble;
    case SQLBLOB:
        return Variant::tBinary;
    }
    return Variant::tInvalid;
}

SqlColumnType dbConnection::toSqlColumnType(Variant::Type coltype)
{
    switch( coltype ) {
    case Variant::tString:
        return SQLSTRING;
    case Variant::tInt:
    case Variant::tLong:
        return SQLINTEGER;
    case Variant::tMoney:
        return SQLDECIMAL;
    case Variant::tDate:
        return SQLDATE;
    case Variant::tTime:
        return SQLTIME;
    case Variant::tDateTime:
        return SQLDATETIME;
    case Variant::tBool:
        return SQLBOOL;
    case Variant::tDouble:
        return SQLFLOAT;
    case Variant::tBinary:
        return SQLBLOB;
    case Variant::tInvalid:
    default:
        throw std::runtime_error( Xtring::printf("Unrecognized columntype %d", coltype ) );
    }
}

/*
	To be able to connect to a remote sever in MySQL and create all the tables, you have to do:
	GRANT ALL ON *.* TO user@remotehost
*/
bool dbConnection::createUser( const Xtring &databasename, const Xtring &name,
                               const Xtring &password, const Xtring &host, const Xtring &privileges,
                               bool ignoreerrors )
{
    Xtring sql;
    if (  isMySQL() ) {
        sql = "GRANT " + ( privileges.isEmpty() ? "ALL" : privileges ) + " ON " + databasename + ".*"
              " TO '" + name + "'@'";
        if( host.isEmpty() )
            sql += "%";
        else
            sql += host;
        sql += "%' IDENTIFIED BY '" + password + "'";
        exec( sql, ignoreerrors );
        sql = "GRANT " + ( privileges.isEmpty() ? "ALL" : privileges ) + " ON " + databasename + ".*"
              " TO '" + name + "'@'localhost' IDENTIFIED BY '" + password + "'";
        exec( sql, ignoreerrors );
#if 0
    } else if ( isPSQL() ) {
        sql = "CREATE USER " + name;
        if ( !password.isEmpty() )
            sql += " WITH PASSWORD '" + password + "';";
        exec( sql, ignoreerrors );
        XtringList tables = getDatabase() ->tables();
        for ( XtringList::Iterator it = tables.begin();
                it != tables.end();
                ++it ) {
            sql = "GRANT " + ( privileges.isEmpty() ? "ALL" : privileges ) + " ON TABLE " + *it
                  + " TO " + name;
            exec( sql, ignoreerrors );
        }
        if ( exec( "SELECT * FROM pg_class WHERE relkind='S'" ) ) {
            GSqlQuery * newquery = new GSqlQuery( *this );
            while ( next() ) {
                sql = "GRANT " + ( privileges.isEmpty() ? "ALL" : privileges ) + " ON " + getString( 0 )
                      + " TO " + name;
                newquery->exec( sql, ignoreerrors );
            }
            delete newquery;
        }
#endif
    }
    return true;
}

/// \todo if database is empty, check if the user uses other databases, and if not, drop the user
bool dbConnection::dropUser(const Xtring &databasename, const Xtring &username, bool ignoreerrors)
{
    if (  isMySQL() ) {
        return exec( "DROP USER " + username + ", '" + username + "'@'localhost'", ignoreerrors );
//			return exec( "REVOKE ALL ON " + databasename + ".* FROM " + username, ignoreerrors );
    }
    return false;
}


// Warning: compare types form smaller to greater, because strtypelen makes DATE and DATETIME the same
SqlColumnType
dbConnection::extractSqlColumnType(const char *sqldef, unsigned short int *size, unsigned short int *decimals)
{
    char typebuf[100];
    const char *parpos = strchr(sqldef, '(');
    unsigned int strtypelen = 0;
    *size = 0;
    *decimals = 0;
    if( parpos ) {
        strtypelen = parpos - sqldef;
        parpos++;
        while( isdigit(*parpos) ) {
            *size = (*size*10) + (*parpos - '0');
            parpos ++;
        }
        if ( *parpos == ',' ) {
            parpos++;
            while( isdigit(*parpos) ) {
                *decimals = (*decimals*10) + (*parpos - '0');
                parpos ++;
            }
        }
    } else {
        strtypelen = strlen(sqldef);
    }
    strncpy( typebuf, sqldef, strtypelen );
    typebuf[strtypelen] = '\0';
    if( (!strcasecmp(typebuf, "TINYINT") && *size==1)
            || !strcasecmp(typebuf, "BOOL") )
        return SQLBOOL;
    else if( !strcasecmp(typebuf, "INTEGER") || !strcasecmp(typebuf, "BIT")
             || !strcasecmp(typebuf, "TINYINT") || !strcasecmp(typebuf, "SMALLINT")
             || !strcasecmp(typebuf, "INT") || !strcasecmp(typebuf, "BIGINT") )
        return SQLINTEGER;
    else if( !strcasecmp(typebuf, "FLOAT") || !strcasecmp(typebuf, "DOUBLE") )
        return SQLFLOAT;
    else if( !strcasecmp(typebuf, "DATE") )
        return SQLDATE;
    else if( !strcasecmp(typebuf, "TIME") )
        return SQLTIME;
    else if( !strcasecmp(typebuf, "TIMESTAMP") )
        return SQLTIMESTAMP;
    else if( !strcasecmp(typebuf, "DATETIME") )
        return SQLDATETIME;
    else if( !strcasecmp(typebuf, "TEXT") || !strcasecmp(typebuf, "TINYTEXT")
             || !strcasecmp(typebuf, "MEDIUMTEXT") || !strcasecmp(typebuf, "LONGTEXT") )
        return SQLTEXT;
    else if( !strcasecmp(typebuf, "BLOB") || !strcasecmp(typebuf, "TINYBLOB")
             || !strcasecmp(typebuf, "MEDIUMBLOB") || !strcasecmp(typebuf, "LONGBLOB") )
        return SQLBLOB;
    else if( !strcasecmp(typebuf, "DECIMAL") )
        return SQLDECIMAL;
    else
        return SQLSTRING;
}


/**
 * Extracts tablenames from a sql statement, looking into the FROM clause
 * \todo {lint} check for FROM and JOIN inside quotes or other expressions
 * @param select
 * @return A list of table names
 */
XtringList dbConnection::extractSqlTables(const Xtring & select)
{
    XtringList tables;
    XtringList tokens;
    select.tokenize( tokens, " ,()" );
    XtringList::const_iterator it = tokens.begin();
    while( it != tokens.end() ) {
        if( (*it).upper() == "FROM" )
            break;
        ++it;
    }
    if( it != tokens.end() ) {
        ++it;
        if( it != tokens.end() )
            tables << *it++;
        while( it != tokens.end() && (*it).upper() != "WHERE" && (*it).upper() != "ORDER" ) {
            if( (*it++).upper() == "JOIN" ) {
                if( it != tokens.end() && !tables.contains( *it ) )
                    tables << *it;
            }
        }
    }
    return tables;
}

} // Namespace
