#include "rtkconfig.h"
#ifdef HAVE_MYSQL

#include <gongdebug.h>
#include <gonglist.h>
#include "rtkinputmysql.h"
#include "rtkreport.h"

namespace gong {}
namespace RTK = gong;

namespace gong {


const char *InputMySql::MYSQL_DATETIME_FORMAT = "%Y-%m-%d %H:%M:%S";
const char *InputMySql::MYSQL_TIME_FORMAT = "%H:%M:%S";
const char *InputMySql::MYSQL_DATE_FORMAT = "%Y-%m-%d";

/*
    The InputMySql class provides the data from a SQL Database query.
    The conexion to the database is done with the values defined in the
        INPUT section of the RTK file and the query is built based upon the INPUTFIELDS.
    You can also let RTK read the conexion info from the RTK file and then call
        setDatabase, setDriver, setUser and setPassword to set the conexion
        before printing the report.
*/

InputMySql::InputMySql(Report &r, const char * name)
    :Input(r, name, "mysql"), pResultSet( 0 ), pRow( 0 )
{
    if( ::mysql_init(&mMySql) )
        ::mysql_options(&mMySql, MYSQL_OPT_COMPRESS, NULL);
}


InputMySql::~InputMySql()
{
    ::mysql_close( &mMySql );
}

/*
   Override to order the input on this fields

	Checks whether the order in the report can be done with this input
*/

bool InputMySql::setOrder(const Xtring &reporder)
{
    XtringList reporderfields;
    reporder.tokenize( reporderfields, "," );
    Xtring neworderby;
    for( XtringList::const_iterator itreporder = reporderfields.begin();
            itreporder != reporderfields.end();
            ++itreporder) {
        Xtring orderfld = (*itreporder).upper().trim();
        InputField *inputfld;
        if( orderfld.startsWith( "=INPUT." ) ) {
            inputfld = getInputField(findInputField(orderfld.mid(7)));
            if( inputfld ) {
                if( neworderby.size() )
                    neworderby += ",";
                neworderby += inputfld->source();
            }
        } else {
            // This field in the report does not make reference to input fields.
            // Pass it through to MySQL
            if( neworderby.size() )
                neworderby += ",";
            neworderby += orderfld;
        }
    }
    // Add report definition orderby
    if( strlen(orderBy()) != 0 ) {
        if( !neworderby.isEmpty() )
            neworderby += ",";
        neworderby += orderBy();
    }
    propOrderBy = neworderby.c_str();
    return true;
}

bool InputMySql::setFilter(const Xtring &repfilter)
{
    Xtring finalwhere = "(" + repfilter + ")";
    if( strlen(where()) )
        finalwhere += Xtring(" AND (") + where() + ")";
    propWhere = finalwhere.c_str();
    return true;
}


bool InputMySql::init()
{
    _GONG_DEBUG_ASSERT( mInputFields.size() );

    ::mysql_real_connect(&mMySql, host(), user(), password(), database(), port(), 0, 0);
    if( ::mysql_errno(&mMySql) ) {
        mReport.addError(Error::SqlConnect, "InputMySql", 0, ::mysql_error(&mMySql));
        return false;
    }
    if( ::mysql_errno(&mMySql) ) {
        mReport.addError(Error::SqlConnect, "InputMySql", 0, ::mysql_error(&mMySql));
        return false;
    }
    if( strempty(encoding()) ) {
        const char *show_variables_select = "show variables like 'character_set_database'";
        if( ::mysql_real_query(&mMySql, show_variables_select , strlen(show_variables_select)) ) {
            mReport.addError(Error::SqlQuery, "InputMySql:preSQL:", 0, ::mysql_error(&mMySql), preSQL());
            // Ignore errors
        } else {
            MYSQL_RES *rs = ::mysql_use_result(&mMySql);
            MYSQL_ROW row = ::mysql_fetch_row(rs);
            const char *dbencoding = row[1];
            _GONG_DEBUG_PRINT(4, Xtring::printf("db encoding: %s", dbencoding ) );
            propEncoding.set( dbencoding );
            ::mysql_free_result(rs);
        }
    }
    if( !strempty(preSQL()) ) {
        _GONG_DEBUG_PRINT(3, "Executing prequery" );
        if( !execMultiSQL( preSQL() ) )
            return false;
    }
    Xtring select = "SELECT ";
    for ( uint i = 0; i < mInputFields.size(); i++ ) {
        if( strempty(mInputFields[i]->constValue()) ) {
            if ( i > 0 )
                select += ",";
            select += mInputFields[i]->source();
            if( mInputFields[i]->source() != mInputFields[i]->name() )
                select += " AS `" + Xtring( mInputFields[i]->name() ) + "`";
            // Set the source pos of the inputfields
            mInputFields[i]->setSourcePos(i);
        }
    }
    select += Xtring(" FROM ") + from();
    if ( !strempty(where()) )
        select += Xtring(" WHERE ") + where();
    if ( !strempty(groupBy()) )
        select += Xtring(" GROUP BY ") + groupBy();
    if ( !strempty(orderBy()) )
        select += Xtring(" ORDER BY ") + orderBy();
    if ( ::mysql_real_query(&mMySql, select.c_str(), select.size()) ) {
        mReport.addError(Error::SqlQuery, "InputMySql", 0, ::mysql_error(&mMySql), select.c_str());
        return false;
    }
    pResultSet = ::mysql_use_result(&mMySql);
    if( !pResultSet )
        return false;
    _GONG_DEBUG_PRINT( 4, Xtring::printf("Returned %ld rows (use_result)", ::mysql_num_rows(pResultSet) ) );
    return true;
}


bool InputMySql::next()
{
    if( !pResultSet )
        return false;
    pRow = ::mysql_fetch_row(pResultSet);
    if( !pRow ) {
        ::mysql_free_result(pResultSet);
        pResultSet = 0;
        if( !strempty(postSQL()) ) {
            _GONG_DEBUG_PRINT(3, "Executing postquery" );
            execMultiSQL( postSQL() );
        }
        return false;
    }
    return true;
}

Variant InputMySql::getValue(uint ncol, Variant::Type type ) const
{
    if ( ncol >= mInputFields.size() || ncol >= ::mysql_field_count(const_cast<MYSQL *>(&mMySql)) )
        return Variant();
    else {
        if( type == Variant::tInvalid )
            type = SQLTypetoVariantType( ::mysql_fetch_fields(pResultSet)[ncol].type );
        if( type == Variant::tBinary )
            return Variant( (void *)pRow[ncol], ::mysql_fetch_lengths(pResultSet)[ncol]);
        Xtring sqlvalue = Xtring( pRow[ncol], ::mysql_fetch_lengths(pResultSet)[ncol] );
        switch( type ) {
        case Variant::tBool:
            return sqlvalue.toBool();
        case Variant::tDate:
            return Date( sqlvalue, MYSQL_DATE_FORMAT );
        case Variant::tDateTime:
            return DateTime( sqlvalue, MYSQL_DATETIME_FORMAT );
        case Variant::tDouble:
            return sqlvalue.toDouble();
        case Variant::tInt:
        case Variant::tLong:
            return sqlvalue.toInt();
        case Variant::tMoney:
            return Money(sqlvalue.toDoubleLocIndep());
        case Variant::tString:
            return sqlvalue;
        case Variant::tTime:
            return Time( sqlvalue, MYSQL_TIME_FORMAT );
        case Variant::tBinary:
        case Variant::tInvalid:
        default:
            throw std::exception();
        }
    }
}


void InputMySql::fixInputParameters(const ParametersList &parameters, const char *delim)
{
    Input::fixInputParameters(parameters, delim);
    /*<<<<<INPUTMYSQL_FIXPARAMETERS*/
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
    /*>>>>>INPUTMYSQL_FIXPARAMETERS*/
}

bool InputMySql::parseAttribute(const char *token, const char *value)
{
    /*<<<<<INPUTMYSQL_READRTK*/
    if( strcaseequal(token, "Host") )
        setOrigHost( value );
    else if( strcaseequal(token, "Port") )
        setOrigPort( value );
    else if( strcaseequal(token, "User") )
        setOrigUser( value );
    else if( strcaseequal(token, "Password") )
        setOrigPassword( value );
    else if( strcaseequal(token, "Database") )
        setOrigDatabase( value );
    else if( strcaseequal(token, "From") )
        setOrigFrom( value );
    else if( strcaseequal(token, "Where") )
        setOrigWhere( value );
    else if( strcaseequal(token, "OrderBy") )
        setOrigOrderBy( value );
    else if( strcaseequal(token, "GroupBy") )
        setOrigGroupBy( value );
    else if( strcaseequal(token, "PreSQL") )
        setOrigPreSQL( value );
    else if( strcaseequal(token, "PostSQL") )
        setOrigPostSQL( value );
    /*>>>>>INPUTMYSQL_READRTK*/
    else
        return false;
    return true;
}

Variant::Type InputMySql::SQLTypetoVariantType( unsigned mysqltype )
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

bool InputMySql::execMultiSQL( const Xtring &querys )
{
    XtringList querylist;
    querys.tokenize(querylist, ";\n");
    for ( XtringList::const_iterator it = querylist.begin(); it != querylist.end(); ++it ) {
        bool ignoreerrors = false;
        if ( !it->startsWith( "#" ) )  {
            if( it->startsWith( "DROP " ) )
                ignoreerrors = true;
            _GONG_DEBUG_PRINT(3, Xtring("Executing ") + *it );
            if( ::mysql_real_query(&mMySql, (*it).c_str() , strlen((*it).c_str())) ) {
                if( !ignoreerrors ) {
                    mReport.addError(Error::SqlQuery, "InputMySql:preSQL:", 0,
                                     ::mysql_error(&mMySql), preSQL());
                    return false;
                }
            }
        }
    }
    return true;
}

} // namespace

#endif // ifdef HAVE_MYSQL

