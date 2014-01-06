#include "rtkconfig.h"
#ifdef HAVE_PGSQL

#include <gongdebug.h>
#include "rtkreport.h"
#include "rtkinputpgsql.h"

namespace gong {}
namespace RTK = gong;

namespace gong {

/*
    The InputPgSql class provides the data from a SQL Database query.
    The conexion to the database is done with the values defined in the
        INPUT section of the RTK file and the query is built based upon the INPUTFIELDS.
    You can also let RTK read the conexion info from the RTK file and then call
        setDatabase, setDriver, setUser and setPassword to set the conexion
        before printing the report.
*/

bool InputPgSql::init()
{
    _GONG_DEBUG_ASSERT( mInputFields.size() );
    pConn = PQsetdbLogin(propHost.get(), Xtring::number(propPort.get()).c_str(), "", "", propDatabase.get(),
                         propUser.get(), propPassword.get());
    if( PQstatus(pConn) != CONNECTION_OK ) {
        mReport.addError(Error::SqlConnect, "InputPgSql", 0, PQerrorMessage(pConn));
        return false;
    }
    if( strlen(preSQL()) ) {
        _GONG_DEBUG_PRINT(2, Xtring::printf("Executing prequery\n%s", preSQL().c_str() ) );
        pResultSet = PQexec( pConn, preSQL() );
        if( PQresultStatus(pResultSet) != PGRES_TUPLES_OK ) {
            mReport.addError(Error::SqlQuery, "InputPgSql:preSQL:", 0, PQerrorMessage(pConn), preSQL());
            PQclear(pResultSet);
            return false;
        }
        PQclear(pResultSet);
    }
    Xtring select = "SELECT ";
    for ( uint i = 0; i < mInputFields.size(); i++ ) {
        if( strempty(mInputFields[i]->constValue() ) ) {
            if ( i > 0 )
                select += ",";
            select += mInputFields[i]->source();
            if( strcmp(mInputFields[i]->source(), mInputFields[i]->name() ) != 0 )
                select += " AS " + Xtring( mInputFields[i]->name() );
            // Set the source pos of the inputfields
            mInputFields[i]->setSourcePos(i);
        }
    }
    select += Xtring(" FROM ") + propFrom.get();
    if ( !propWhere.isEmpty() )
        select += Xtring(" WHERE ") + propWhere.get();
    if ( !propGroupBy.isEmpty() )
        select += Xtring(" GROUP BY ") + propGroupBy.get();
    if ( !propOrderBy.isEmpty() )
        select += Xtring(" ORDER BY ") + propOrderBy.get();
    _GONG_DEBUG_PRINT(1, Xtring::printf("Executing query\n%s", select.c_str() ) );
    pResultSet = PQexec( pConn, select.c_str() );
    if( PQresultStatus(pResultSet) != PGRES_TUPLES_OK ) {
        mReport.addError(Error::SqlQuery, "InputPgSql:", 0, PQerrorMessage(pConn), select.c_str());
        PQclear(pResultSet);
        return false;
    }
    _GONG_DEBUG_PRINT(1, Xtring::printf("Returned %d rows", PQntuples(pResultSet) ) );
    mNRow = -1;
    return true;
}


bool InputPgSql::next()
{
    if( !pResultSet )
        return false;
    if( mNRow == PQntuples(pResultSet) || PQntuples(pResultSet) == 0 ) {
        if( strlen(postSQL()) ) {
            PQclear(pResultSet);
            pResultSet = PQexec( pConn, postSQL() );
            if( PQresultStatus(pResultSet) != PGRES_TUPLES_OK )
                mReport.addWarning(Error::SqlQuery, "InputMySql:postSQL:", 0, PQerrorMessage(pConn), postSQL());
        }
        PQclear(pResultSet);
        return false;
    } else {
        mNRow++;
        return true;
    }
}

Variant InputPgSql::getValue(uint i, Variant::Type type ) const
{
    if ( i >= mInputFields.size() || i >= (uint)PQnfields(pResultSet) )
        return Variant();
    else {
        char* value = PQgetvalue(pResultSet, mNRow, i);
        unsigned long int len = PQgetlength(pResultSet, mNRow, i);
        return Variant(Xtring(value, len).c_str(), type, len, getRegConfig() );
    }
}


bool InputPgSql::parseAttribute(const char *attribute, const char *value)
{
    /*<<<<<INPUTPGSQL_READRTK*/
    if( strcaseequal(attribute, "Host") )
        setOrigHost( value );
    else if( strcaseequal(attribute, "Port") )
        setOrigPort( value );
    else if( strcaseequal(attribute, "User") )
        setOrigUser( value );
    else if( strcaseequal(attribute, "Password") )
        setOrigPassword( value );
    else if( strcaseequal(attribute, "Database") )
        setOrigDatabase( value );
    else if( strcaseequal(attribute, "From") )
        setOrigFrom( value );
    else if( strcaseequal(attribute, "Where") )
        setOrigWhere( value );
    else if( strcaseequal(attribute, "OrderBy") )
        setOrigOrderBy( value );
    else if( strcaseequal(attribute, "GroupBy") )
        setOrigGroupBy( value );
    else if( strcaseequal(attribute, "PreSQL") )
        setOrigPreSQL( value );
    else if( strcaseequal(attribute, "PostSQL") )
        setOrigPostSQL( value );
    /*>>>>>INPUTPGSQL_READRTK*/
    else
        return false;
    return true;
}

void InputPgSql::fixInputParameters(const ParametersList &parameters, const char *delim)
{
    Input::fixInputParameters(parameters, delim);
    /*<<<<<INPUTPGSQL_FIXPARAMETERS*/
	propHost.fix( parameters, delim );
	propPort.fix( parameters, delim, 0 );
	propUser.fix( parameters, delim );
	propPassword.fix( parameters, delim );
	propDatabase.fix( parameters, delim );
	propFrom.fix( parameters, delim );
	propWhere.fix( parameters, delim );
	propOrderBy.fix( parameters, delim );
	propGroupBy.fix( parameters, delim );
	propPreSQL.fix( parameters, delim );
	propPostSQL.fix( parameters, delim );
/*>>>>>INPUTPGSQL_FIXPARAMETERS*/
}

} // namespace




#endif // ifdef HAVE_PGSQL

