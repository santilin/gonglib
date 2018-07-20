#include <gongdebug.h>
#include <rtkreport.h>
#include "dbappreportinput.h"

namespace gong {

ReportInput::ReportInput ( dbConnection *conn, RTK::Report &r, const char * name )
    :Input ( r, name ), pConnection ( conn ), mRowNumber ( 0 )
{
}


ReportInput::~ReportInput()
{
}

bool ReportInput::init()
{
    _GONG_DEBUG_ASSERT ( mInputFields.size() );
    if ( !strempty ( preSQL() ) )
    {
        _GONG_DEBUG_PRINT ( 3, "Executing prequery" );
        XtringList querys;
        Xtring ( preSQL() ).tokenize ( querys, ";\n" );
        if ( !pConnection->exec ( querys ) )
        {
            mReport.addError ( Error::SqlQuery, "ReportInput:preSQL:",
                               0, pConnection->getLastError().what(), preSQL() );
            return false;
        }
    }
    Xtring select = "SELECT ";
    for ( unsigned int i = 0; i < mInputFields.size(); i++ )
    {
        if ( strempty ( mInputFields[i]->constValue() ) )
        {
            if ( i > 0 )
                select += ",";
            select += mInputFields[i]->source();
            if ( mInputFields[i]->source() != mInputFields[i]->name() )
                select += " AS `" + Xtring ( mInputFields[i]->name() ) + "`";
            // Set the source pos of the inputfields
            mInputFields[i]->setSourcePos ( i );
        }
    }
    select += Xtring ( " FROM " ) + from();
    if ( !strempty ( where() ) )
        select += Xtring ( " WHERE " ) + where();
    if ( !strempty ( groupBy() ) )
        select += Xtring ( " GROUP BY " ) + groupBy();
    if ( !strempty ( orderBy() ) )
        select += Xtring ( " ORDER BY " ) + orderBy();
    pResultSet = pConnection->select ( select, true ); // Ignore errors
    if ( !pResultSet )
    {
        mReport.addError ( Error::SqlQuery, "ReportInput", 0, pConnection->getLastError().what(), select.c_str() );
        return false;
    }
    mRowNumber = -1;
    return true;
}


bool ReportInput::next()
{
    if ( !pResultSet )
        return false;
    mRowNumber++;
    if( !pResultSet->next() ) {
        delete pResultSet;
        pResultSet = 0;
        if ( !strempty ( postSQL() ) )
        {
            _GONG_DEBUG_PRINT ( 3, "Executing postquery" );
            XtringList querys;
            Xtring ( postSQL() ).tokenize ( querys, ";\n" );
            if ( !pConnection->exec ( querys ) )
            {
                mReport.addError ( Error::SqlQuery, "ReportInput:postSQL:",
                                   0, pConnection->getLastError().what(), postSQL() );
                return false;
            }
        }
        return false;
    } else {
        return true;
    }
}

/* The value type of the report field can differ from that of the sql query */
Variant ReportInput::getValue( unsigned int ncol, Variant::Type reportfieldtype ) const
{
    if ( ncol < mInputFields.size() && ncol < pResultSet->getColumnCount() ) {
        if( reportfieldtype == Variant::tInvalid )
            reportfieldtype = dbConnection::MySQLTypetoVariantType( pResultSet->getColumnType(ncol) );
        switch( reportfieldtype ) { // takes precedence over the resultset type
        case Variant::tString:
            return pResultSet->toString( ncol );
        case Variant::tInt:
        case Variant::tLong:
            return pResultSet->toInt( ncol );
        case Variant::tBool:
            return pResultSet->toBool( ncol );
        case Variant::tDouble:
            return pResultSet->toDouble( ncol );
        case Variant::tMoney:
            return pResultSet->toMoney( ncol );
        case Variant::tDate:
            return pResultSet->toDate( ncol );
        case Variant::tDateTime:
            return pResultSet->toDateTime( ncol );
        case Variant::tTime:
            return pResultSet->toTime( ncol );
        case Variant::tBinary:
            return pResultSet->toBinary( ncol );
        case Variant::tInvalid:
            throw std::exception();
            return Variant();
        }
    }
    return Variant();
}

bool ReportInput::setOrder ( const Xtring &reporder )
{
    XtringList reporderfields;
    reporder.tokenize ( reporderfields, "," );
    Xtring neworderby;
    for ( XtringList::const_iterator itreporder = reporderfields.begin();
            itreporder != reporderfields.end();
            ++itreporder )
    {
        Xtring orderfld = ( *itreporder ).upper().trim();
        InputField *inputfld;
        if ( orderfld.startsWith ( "=INPUT." ) )
        {
            inputfld = getInputField ( findInputField ( orderfld.mid ( 7 ) ) );
            if ( inputfld )
            {
                if ( neworderby.size() )
                    neworderby += ",";
                neworderby += inputfld->source();
            }
        }
        else   // Pasar el campo directamente a mysql
        {
            if ( neworderby.size() )
                neworderby += ",";
            neworderby += orderfld;
        }
    }
    // Add report definition orderby
    if ( strlen ( orderBy() ) != 0 )
    {
        if ( !neworderby.isEmpty() )
            neworderby += ",";
        neworderby += orderBy();
    }
    propOrderBy = neworderby.c_str();
    return true;
}

bool ReportInput::setFilter ( const Xtring &repfilter )
{
    Xtring finalwhere = "(" + repfilter + ")";
    if ( strlen ( propWhere.get() ) )
        finalwhere += Xtring ( " AND (" ) + propWhere.get() + ")";
    setOrigWhere( finalwhere.c_str() );
    return true;
}

void ReportInput::fixInputParameters ( const ParametersList &parameters, const char *delim )
{
    Input::fixInputParameters ( parameters, delim );
    /*<<<<<INPUTMYSQL_FIXPARAMETERS*/
    propHost.fix ( parameters, delim );
    propPort.fix ( parameters, delim, 0 );
    propUser.fix ( parameters, delim );
    propPassword.fix ( parameters, delim );
    propDatabase.fix ( parameters, delim );
    propFrom.fix ( parameters, delim );
    propWhere.fix ( parameters, delim );
    propOrderBy.fix ( parameters, delim );
    propGroupBy.fix ( parameters, delim );
    propPreSQL.fix ( parameters, delim );
    propPostSQL.fix ( parameters, delim );
    /*>>>>>INPUTMYSQL_FIXPARAMETERS*/
}

bool ReportInput::parseAttribute ( const char *token, const char *value )
{
    if ( strcaseequal ( token, "Host" ) )
        setOrigHost ( value );
    else if ( strcaseequal ( token, "Port" ) )
        setOrigPort ( value );
    else if ( strcaseequal ( token, "User" ) )
        setOrigUser ( value );
    else if ( strcaseequal ( token, "Password" ) )
        setOrigPassword ( value );
    else if ( strcaseequal ( token, "Database" ) )
        setOrigDatabase ( value );
    else if ( strcaseequal ( token, "From" ) )
        setOrigFrom ( value );
    else if ( strcaseequal ( token, "Where" ) ) {
        setOrigWhere ( value );
	}
    else if ( strcaseequal ( token, "OrderBy" ) )
        setOrigOrderBy ( value );
    else if ( strcaseequal ( token, "GroupBy" ) )
        setOrigGroupBy ( value );
    else if ( strcaseequal ( token, "PreSQL" ) )
        setOrigPreSQL ( value );
    else if ( strcaseequal ( token, "PostSQL" ) )
        setOrigPostSQL ( value );
    else {
        return Input::parseAttribute(token, value);
	}
	return true;
}

}
