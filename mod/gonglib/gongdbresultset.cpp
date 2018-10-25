#include "config.h"
#include <mysql/mysql.h>
#ifdef HAVE_SQLITE3
# include <sqlite3.h>
#endif
#include "gongdebug.h"
#include "gongdbresultset.h"
#include "gongdberror.h"
#include "gongvariant.h"

namespace gong {

/**
    @class gong::dbResultSet gongdbresultset.cpp <gongdbresultset.h>
 	@brief A wrapper for MySQL and SQLITE3 resultsets

	Usage example:
	\code
	dbConnection conn;
	if( conn.connect(DRIVER_MYSQL, "user", "passwd", "database" ) ) {
		dbResultSet *rs = conn.select("SHOW TABLES");
		for( my_ulonglong row = 0; row < rs->size(); row ++)
			std::cout << rs->at(row).toString(0) << std::endl;
		delete rs;
	}
	\endcode

	@see dbConnection

*/

dbResultSet::dbResultSet(dbConnection *dbconn, MYSQL_RES *result)
    : pConnection( dbconn ), mRowNumber( -1 ), mRowCount( 0 ),  mPosition( atBeforeStart )
{
    _data.mysql.pResult = result;
    _data.mysql.pFieldDefs = ::mysql_fetch_fields( result );
    mRowCount = mysql_num_rows( _data.mysql.pResult );
    mColumnCount = ::mysql_num_fields( _data.mysql.pResult );
}

#ifdef HAVE_SQLITE3

dbResultSet::dbResultSet(dbConnection* dbconn, sqlite3_stmt *result)
    : pConnection( dbconn ), mRowNumber( -1 ), mRowCount( 0 ), mPosition( atBeforeStart )
{
    _data.sqlite3.pResult = result;
    _data.sqlite3.pRows = new std::vector<Variant>();
    mColumnCount = sqlite3_column_count( _data.sqlite3.pResult );
}

#endif

dbResultSet::~dbResultSet()
{
    switch( pConnection->getSqlDriver() ) {
    case dbConnection::DRIVER_MYSQL:
        ::mysql_free_result( _data.mysql.pResult );
        break;
    case dbConnection::DRIVER_SQLITE3:
#ifdef HAVE_SQLITE3
        sqlite3_finalize( _data.sqlite3.pResult );
        delete _data.sqlite3.pRows;
#endif
    case dbConnection::DRIVER_POSTGRESQL:
        break;
    };
}

dbResultSet::size_type dbResultSet::getRowCount()
{
    switch( pConnection->getSqlDriver() ) {
    case dbConnection::DRIVER_MYSQL:
        /// TODO The same than sqlite3, do not store resultsets
        return mRowCount;
    case dbConnection::DRIVER_SQLITE3:
#ifdef HAVE_SQLITE3
        while( mPosition != dbResultSet::atEnd ) {
            next();
        }
        return mRowCount;
#endif
    case dbConnection::DRIVER_POSTGRESQL:
    default:
        return 0; /// TODO
    }
}

Variant dbResultSet::getValue(unsigned int colnum) const
{
    Variant::Type t;
    switch( pConnection->getSqlDriver() ) {
    case dbConnection::DRIVER_MYSQL:
        t = dbConnection::MySQLTypetoVariantType( _data.mysql.pFieldDefs[colnum].type );
        break;
#ifdef HAVE_SQLITE3
    case dbConnection::DRIVER_SQLITE3:
        t = dbConnection::SQLiteTypetoVariantType( sqlite3_column_decltype( _data.sqlite3.pResult, colnum ) );
        break;
#endif
    default:
        t = Variant::tInvalid;
    }
    switch( t ) {
    case Variant::tBool:
        return toBool( colnum );
    case Variant::tDate:
        return toDate( colnum );
    case Variant::tDateTime:
        return toDateTime( colnum );
    case Variant::tDouble:
        return toDouble( colnum );
    case Variant::tInt:
    case Variant::tLong:
        return toInt( colnum );
    case Variant::tMoney:
        return toMoney( colnum );
    case Variant::tString:
        return toString(colnum);
    case Variant::tTime:
        return toTime( colnum );
    case Variant::tBinary:
        return toBinary( colnum );
    case Variant::tInvalid:
    default:
        _GONG_DEBUG_WARNING( Xtring::printf("Invalid Variant::type %d,%s in dbResultSet::getValue", t, Variant::typeToName( t )) );
        return Variant();
    }
}

Xtring dbResultSet::toString(unsigned colnum) const
{
    _GONG_DEBUG_ASSERT( colnum < mColumnCount );
    Xtring ret;
    switch( pConnection->getSqlDriver() ) {
    case dbConnection::DRIVER_MYSQL:
        ret = Xtring( _data.mysql.mRow[colnum], _data.mysql.pLengths[colnum] );
        break;
    case dbConnection::DRIVER_SQLITE3:
#ifdef HAVE_SQLITE3
        ret = (*_data.sqlite3.pRows)[mRowNumber * mColumnCount + colnum].toString();
#endif
        break;
    case dbConnection::DRIVER_POSTGRESQL:
        break;
    }
    if( pConnection->getIConvSelectPtr() )
        ret = pConnection->getIConvSelectPtr()->convert(ret);
    return ret;
}

Variant dbResultSet::toBinary(unsigned colnum) const
{
    _GONG_DEBUG_ASSERT( colnum < mColumnCount  );
    Xtring ret;
    switch( pConnection->getSqlDriver() ) {
    case dbConnection::DRIVER_MYSQL:
        return Variant( (void *)_data.mysql.mRow[colnum], _data.mysql.pLengths[colnum] );
    case dbConnection::DRIVER_SQLITE3:
#ifdef HAVE_SQLITE3
        return (*_data.sqlite3.pRows)[mRowNumber * mColumnCount + colnum];
#endif
        break;
    case dbConnection::DRIVER_POSTGRESQL:
        break;
    }
    throw new std::runtime_error( "Feature unsupported" );
}

int dbResultSet::toInt(unsigned colnum) const
{
    _GONG_DEBUG_ASSERT( colnum < mColumnCount  );
    return toString(colnum).toInt();
}

uint dbResultSet::toUInt(unsigned colnum) const
{
    _GONG_DEBUG_ASSERT( colnum < mColumnCount  );
    return toString(colnum).toUInt();
}


double dbResultSet::toDouble(unsigned colnum) const
{
    // Don't call strtod (Xtring::toDouble) because it is locale-dependent
    _GONG_DEBUG_ASSERT( colnum < mColumnCount  );
    Xtring ret;
    switch( pConnection->getSqlDriver() ) {
    case dbConnection::DRIVER_MYSQL:
        return Xtring( _data.mysql.mRow[colnum], _data.mysql.pLengths[colnum] ).toDoubleLocIndep();
    case dbConnection::DRIVER_SQLITE3:
#ifdef HAVE_SQLITE3
        return (*_data.sqlite3.pRows)[mRowNumber * mColumnCount + colnum].toDouble();
#endif
        break;
    case dbConnection::DRIVER_POSTGRESQL:
        break;
    }
    throw new std::runtime_error( "Feature unsupported" );
}

long dbResultSet::toLong(unsigned colnum) const
{
    _GONG_DEBUG_ASSERT( colnum < mColumnCount  );
    return toString(colnum).toLong();
}

Money dbResultSet::toMoney(unsigned colnum) const
{
    _GONG_DEBUG_ASSERT( colnum < mColumnCount  );
    switch( pConnection->getSqlDriver() ) {
    case dbConnection::DRIVER_MYSQL:
    {
        unsigned short int ndecs = _data.mysql.pFieldDefs[colnum].decimals;
        return Money( toDouble(colnum), ndecs > 7?7:ndecs );
    }
    case dbConnection::DRIVER_SQLITE3:
#ifdef HAVE_SQLITE3
        return (*_data.sqlite3.pRows)[mRowNumber * mColumnCount + colnum].toMoney();
#endif
        break;
    case dbConnection::DRIVER_POSTGRESQL:
        break;
    }
    throw new std::runtime_error( "Feature unsupported" );
}

Date dbResultSet::toDate(unsigned colnum) const
{
    _GONG_DEBUG_ASSERT( colnum < mColumnCount  );
    return Date( toString(colnum), MYSQL_DATE_FORMAT );
}

DateTime dbResultSet::toDateTime(unsigned colnum) const
{
    _GONG_DEBUG_ASSERT( colnum < mColumnCount  );
    return DateTime( toString(colnum), MYSQL_DATETIME_FORMAT );
}

Time dbResultSet::toTime(unsigned colnum) const
{
    _GONG_DEBUG_ASSERT( colnum < mColumnCount  );
    return Time( toString(colnum), MYSQL_TIME_FORMAT );
}

bool dbResultSet::toBool(unsigned colnum) const
{
    _GONG_DEBUG_ASSERT( colnum < mColumnCount  );
    return toString(colnum).toBool();
}

bool dbResultSet::isNull(unsigned colnum) const
{
    _GONG_DEBUG_ASSERT( colnum < mColumnCount  );
    switch( pConnection->getSqlDriver() ) {
    case dbConnection::DRIVER_MYSQL:
        return _data.mysql.mRow[colnum] == 0;
    case dbConnection::DRIVER_SQLITE3:
#ifdef HAVE_SQLITE3
        return (*_data.sqlite3.pRows)[mRowNumber * mColumnCount + colnum].type() == Variant::tInvalid;
#endif
        break;
    case dbConnection::DRIVER_POSTGRESQL:
        break;
    }
    throw new std::runtime_error( "Feature unsupported" );
}

Variant dbResultSet::getValue(dbResultSet::size_type tup_num, unsigned int colnum)
{
    switch( pConnection->getSqlDriver() ) {
    case dbConnection::DRIVER_MYSQL:
    {
        ::mysql_data_seek( _data.mysql.pResult, tup_num );
        _data.mysql.mRow = ::mysql_fetch_row( _data.mysql.pResult );
        if( !_data.mysql.mRow )
            return Variant();
        _data.mysql.pLengths = ::mysql_fetch_lengths( _data.mysql.pResult );
//         _data.mysql.pFieldDefs = ::mysql_fetch_fields( _data.mysql.pResult );
        return getValue( colnum );
    }
#ifdef HAVE_SQLITE3
    case dbConnection::DRIVER_SQLITE3:
        while( mRowNumber < tup_num ) {
            if( !next() )
                break;
        }
        return getValue( colnum );
#endif
    default:
        return Variant();
    }
}


bool dbResultSet::next()
{
    if( mPosition == dbResultSet::atEnd )
        return false;
    switch( pConnection->getSqlDriver() ) {
    case dbConnection::DRIVER_MYSQL:
    {
        mPosition = dbResultSet::atMiddle;
        _data.mysql.mRow = ::mysql_fetch_row( _data.mysql.pResult );
        if( !_data.mysql.mRow ) {
            mPosition = dbResultSet::atEnd;
            return false;
        }
        mRowNumber++;
        _data.mysql.pLengths = ::mysql_fetch_lengths( _data.mysql.pResult );
//         _data.mysql.pFieldDefs = ::mysql_fetch_fields( _data.mysql.pResult );
        return true;
    }
#ifdef HAVE_SQLITE3
    case dbConnection::DRIVER_SQLITE3:
    {
        mPosition = dbResultSet::atMiddle;
        int res = sqlite3_step( _data.sqlite3.pResult );
        switch(res) {
        case SQLITE_ROW:
        {
            mRowCount++;
            mRowNumber++;
            for (uint i = 0; i < mColumnCount; ++i) {
                switch (sqlite3_column_type(_data.sqlite3.pResult, i)) {
                case SQLITE_BLOB:
                    _data.sqlite3.pRows->push_back(
                        Variant( sqlite3_column_blob(_data.sqlite3.pResult, i),
                                 sqlite3_column_bytes(_data.sqlite3.pResult, i)) );
                    break;
                case SQLITE_INTEGER:
                    _data.sqlite3.pRows->push_back( sqlite3_column_int64(_data.sqlite3.pResult, i) );
                    break;
                case SQLITE_FLOAT:
                    _data.sqlite3.pRows->push_back( sqlite3_column_double(_data.sqlite3.pResult, i) );
                    break;
                case SQLITE_NULL:
                    _data.sqlite3.pRows->push_back( Variant( Xtring::null ) );
                    break;
                default:
                    Xtring s((const char *)sqlite3_column_text(_data.sqlite3.pResult, i),
                             sqlite3_column_bytes(_data.sqlite3.pResult, i) );
                    _data.sqlite3.pRows->push_back( s );
                    break;
                }
            }
            return true;
        }
        case SQLITE_DONE:
            mPosition = dbResultSet::atEnd;
            sqlite3_reset(_data.sqlite3.pResult);
            return false;
        case SQLITE_CONSTRAINT:
        case SQLITE_ERROR:
            // SQLITE_ERROR is a generic error code and we must call sqlite3_reset()
            // to get the specific error message.
            res = sqlite3_reset(_data.sqlite3.pResult);
            return false;
        case SQLITE_MISUSE:
        case SQLITE_BUSY:
        default:
            // something wrong, don't get col info, but still return false
            sqlite3_reset(_data.sqlite3.pResult);
            mPosition = dbResultSet::atEnd;
            return false;
        }
    }
#endif
    case dbConnection::DRIVER_POSTGRESQL:
    default:
        return false; /// TODO
    }
}


Xtring dbResultSet::getColumnName(unsigned int colnum) const
{
    if ( colnum >= mColumnCount )
        return "";
    switch( pConnection->getSqlDriver() ) {
    case dbConnection::DRIVER_MYSQL:
	{
		Xtring ret = _data.mysql.pFieldDefs[colnum].table;
		if( !ret.isEmpty() ) {
			ret += ".";
		}
        return ret + _data.mysql.pFieldDefs[colnum].name;
	}
		break;
#ifdef HAVE_SQLITE3
    case dbConnection::DRIVER_SQLITE3:
        throw;
#endif
    default:
        return Xtring::null; /// TODO
    }
}

SqlColumnType dbResultSet::getColumnType( unsigned int colnum ) const
{
    if ( colnum >= mColumnCount )
        return SQLSTRING;
    switch( pConnection->getSqlDriver() ) {
    case dbConnection::DRIVER_MYSQL:
    {
        Variant::Type vt = dbConnection::MySQLTypetoVariantType( _data.mysql.pFieldDefs[colnum].type );
        return dbConnection::toSqlColumnType( vt );
    }
#ifdef HAVE_SQLITE3
    case dbConnection::DRIVER_SQLITE3:
    {
        Variant::Type vt = dbConnection::SQLiteTypetoVariantType( sqlite3_column_decltype( _data.sqlite3.pResult, colnum ) );
        return dbConnection::toSqlColumnType( vt );
    }
#endif
    default:
        return SQLSTRING; /// TODO
    }
}

unsigned int dbResultSet::getColumnWidth( unsigned int colnum ) const
{
    if ( colnum >= mColumnCount )
        return 0;
    switch( pConnection->getSqlDriver() ) {
    case dbConnection::DRIVER_MYSQL:
        return _data.mysql.pFieldDefs[colnum].length;
#ifdef HAVE_SQLITE3
    case dbConnection::DRIVER_SQLITE3:
        throw;
        break;
#endif
    default:
        return 0;
    }
}

unsigned int dbResultSet::getColumnDecimals( unsigned int colnum ) const
{
    if ( colnum >= mColumnCount )
        return 0;
    switch( pConnection->getSqlDriver() ) {
    case dbConnection::DRIVER_MYSQL:
        return _data.mysql.pFieldDefs[colnum].decimals;
#ifdef HAVE_SQLITE3
    case dbConnection::DRIVER_SQLITE3:
        throw; /// TODO
        break;
#endif
    default:
        return 2; /// TODO
    }
}


unsigned int dbResultSet::getColumnPos(const char *fldname) const
{
	if (!fldname || *fldname == '\0')
		throw std::invalid_argument(__FILE__);
// 	_GONG_DEBUG_PRINT(0, Xtring::printf("Looking for %s", fldname));
    switch( pConnection->getSqlDriver() ) {
    case dbConnection::DRIVER_MYSQL:
	{
		for (uint i = 0; i < mColumnCount; ++i ) {
			const char *name = _data.mysql.pFieldDefs[i].name;
			if( strcasecmp( name, fldname ) == 0 ) {
// 				_GONG_DEBUG_PRINT(0, Xtring::printf("Found at %d", i));
				return i;
			}
		}
		throw std::invalid_argument(Xtring::printf("%s, %s:%s", fldname, __FILE__,__FUNCTION__));
	}
#ifdef HAVE_SQLITE3
    case dbConnection::DRIVER_SQLITE3:
        throw;
#endif
    default:
        throw; /// TODO
    }

}


}
