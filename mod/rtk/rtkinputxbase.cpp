#include "rtkconfig.h"
#ifdef RTK_HAVE_XBASE

#include <gongdebug.h>
#include <gongfileutils.h>
#include "rtkinputxbase.h"
#include "rtkreport.h"

namespace gong {}
namespace RTK = gong;

namespace gong {


const char *InputXBase::XBASE_DATETIME_FORMAT = "%Y-%m-%d %H:%M:%S";
const char *InputXBase::XBASE_TIME_FORMAT = "%H:%M:%S";
const char *InputXBase::XBASE_DATE_FORMAT = "%Y-%m-%d";

/*
*/

InputXBase::InputXBase(Report &r, const char * name)
    :Input(r, name, "xbase"), pXBaseSQL(0), pQuery(0)
{
}


InputXBase::~InputXBase()
{
    if( pQuery )
        delete pQuery;
    if( pXBaseSQL )
        delete pXBaseSQL;
}

/*
   Override to order the input on this fields

	Checks whether the order in the report can be done with this input
	Copied from InputMySQL
*/

bool InputXBase::setOrder(const Xtring &reporder)
{
    XtringList reporderfields;
    reporder.tokenize( reporderfields, "," );
    Xtring neworderby;
    for( XtringList::const_iterator itreporder = reporderfields.begin();
            itreporder != reporderfields.end();
            ++itreporder) {
        Xtring orderfld = (*itreporder).upper().stripWhiteSpace();
        InputField *inputfld;
        if( orderfld.startsWith( "=INPUT." ) ) {
            inputfld = getInputField(findInputField(orderfld.mid(7)));
            if( inputfld ) {
                if( neworderby.size() )
                    neworderby += ",";
                neworderby += inputfld->source();
            }
        } else { // This field in the report does not make reference to input fields
            return false;
        }
    }
    if( !neworderby.isEmpty() )
        propOrderBy = (neworderby + "," + orderBy() ).c_str();
    return true;
}


bool InputXBase::init()
{
    _GONG_DEBUG_ASSERT( mInputFields.size() );
    bool undocumented;

    pXBaseSQL = new XBaseSQL( databasePath() );
    if( !FileUtils::exists( databasePath() ) )  {
        mReport.addError(Error::SqlConnect, "InputXBase", 0,
                         Xtring::printf( "%s does not exist", databasePath() ).c_str() );
        return false;
    }
    if( !FileUtils::is_dir( databasePath() ) )  {
        mReport.addError(Error::SqlConnect, "InputXBase", 0,
                         Xtring::printf( "%s is not a directory", databasePath() ).c_str() );
        return false;
    }
    if( !strempty(pXBaseSQL->lastError() ) ) {
        mReport.addError(Error::SqlConnect, "InputXBase", 0, pXBaseSQL->lastError() );
        return false;
    }
    pXBaseSQL->setCaseSensitive( caseSensitive() );
    if( !strempty(preSQL()) ) {
        _GONG_DEBUG_PRINT(3, Xtring("Executing prequery") + preSQL() );
        XBSQLQuery *q = pXBaseSQL->openQuery( preSQL(), undocumented );
        if( q ) {
            q->execute(0, 0);
            delete q;
        }
        if( !strempty(pXBaseSQL->lastError() ) ) {
            mReport.addError(Error::SqlQuery, "InputXBase:preSQL:", 0, pXBaseSQL->lastError(), preSQL() );
            return false;
        }
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
    _GONG_DEBUG_PRINT(3, "Executing query " + select );
    pQuery = pXBaseSQL->openSelect( select.c_str() );
    if( pQuery ) {
        pQuery->execute(0, 0);
    }
    if( !strempty(pXBaseSQL->lastError() ) ) {
        mReport.addError(Error::SqlQuery, "InputXBase:", 0, pXBaseSQL->lastError(), select.c_str() );
        return false;
    }
    mRecordNumber = -1;
    _GONG_DEBUG_PRINT( 4, Xtring::printf("Returned %d rows", pQuery->getNumRows() ) );
    return true;
}


bool InputXBase::next()
{
    if( !pQuery )
        return false;
    if( ++mRecordNumber == pQuery->getNumRows() ) {
        delete pQuery;
        pQuery = 0;
        if( !strempty(postSQL()) ) {
            _GONG_DEBUG_PRINT(3, Xtring("Executing postquery") + postSQL() );
            bool undocumented;
            XBSQLQuery *q = pXBaseSQL->openQuery( postSQL(), undocumented );
            if( q ) {
                q->execute(0, 0);
                delete q;
            }
            if( !strempty(pXBaseSQL->lastError() ) ) {
                mReport.addError(Error::SqlQuery, "InputXBase:postSQL:", 0, pXBaseSQL->lastError(), postSQL() );
                return false;
            }
        }
        return false;
    } else {
        return true;
    }
}

Variant InputXBase::getValue(uint ncol, Variant::Type type ) const
{
    _GONG_DEBUG_ASSERT( pQuery );
    if ( ncol >= mInputFields.size() || ncol >= pQuery->getNumFields() )
        return Variant();
    else {
        if( type == Variant::tInvalid )
            type = XBaseTypeToVariantType( XBSQL::VType(pQuery->getFieldType( ncol )) );
        Xtring sqlvalue = pQuery->getField( mRecordNumber, ncol).getText();
        switch( type ) {
        case Variant::tBool:
            return sqlvalue.toBool();
        case Variant::tDate:
            return Date( sqlvalue, XBASE_DATE_FORMAT );
        case Variant::tDateTime:
            return DateTime( sqlvalue, XBASE_DATETIME_FORMAT );
        case Variant::tDouble:
            return sqlvalue.toDouble();
        case Variant::tInt:
        case Variant::tLong:
            return sqlvalue.toInt();
        case Variant::tMoney:
            return Money(sqlvalue.toDouble());
        case Variant::tString:
            return sqlvalue;
        case Variant::tTime:
            return Time( sqlvalue, XBASE_TIME_FORMAT );
        case Variant::tBinary:
        case Variant::tInvalid:
        default:
            throw std::runtime_error("XBase type not supported");
        }
    }
}


void InputXBase::fixInputParameters(const ParametersList &parameters, const char *delim)
{
    Input::fixInputParameters(parameters, delim);
    /*<<<<<INPUTXBASE_FIXPARAMETERS*/
	propDatabasePath.fix( parameters, delim );
	propCaseSensitive.fix( parameters, delim, true );
	propFrom.fix( parameters, delim );
	propWhere.fix( parameters, delim );
	propOrderBy.fix( parameters, delim );
	propGroupBy.fix( parameters, delim );
	propPreSQL.fix( parameters, delim );
	propPostSQL.fix( parameters, delim );
/*>>>>>INPUTXBASE_FIXPARAMETERS*/
}

bool InputXBase::parseAttribute(const char *token, const char *value)
{
    /*<<<<<INPUTXBASE_READRTK*/
    if( strcaseequal(token, "DatabasePath") )
        setOrigDatabasePath( value );
    else if( strcaseequal(token, "CaseSensitive") )
        setOrigCaseSensitive( value );
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
    /*>>>>>INPUTXBASE_READRTK*/
    else
        return false;
    return true;
}

Variant::Type InputXBase::XBaseTypeToVariantType( enum XBSQL::VType vtype)
{
    switch ( vtype ) {
    case XBSQL::VText:
    case XBSQL::VMemo:
        return Variant::tString;
    case XBSQL::VNum:
        return Variant::tInt;
    case XBSQL::VDouble:
        return Variant::tDouble;
    case XBSQL::VDate:
        return Variant::tDate;
    case XBSQL::VBool:
        return Variant::tBool;
    default:
        _GONG_DEBUG_WARNING( Xtring::printf( "Type %d not recognized", vtype ) );
        return Variant::tInvalid;
    }
}


} // namespace

#endif // ifdef HAVE_XBASE

