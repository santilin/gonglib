/**
	@file gongdbfieldvalue.cpp Field value templates
	@class gong::dbFieldValue
	@brief Field value templates

	This class holds the values of dbFieldDefinitions.
	They are created by dbFieldDefinition::createFieldValue()

	@see dbRecord
*/

#include "gongdebug.h"
#include "gongdbconnection.h"
#include "gongdbfieldvalue.h"
#include "gongdbfielddefinition.h"

namespace gong {

dbFieldValue::dbFieldValue(const dbFieldDefinition *flddef, Variant::Type type, bool isnull)
    : mValue(type), mNull(isnull), mModified(false)
{
    if( type == Variant::tMoney ) {
        mValue = Money(0.0, flddef->getDecimals() > 7 ? 7 : flddef->getDecimals() );
    }
}

void dbFieldValue::setValue(const Variant &value)
{
#ifdef _GONG_DEBUG
    if( mValue.type() != value.type() ) {
        _GONG_DEBUG_WARNING( Xtring::printf("'%s' != '%s'",
                                            Variant::typeToName( mValue.type() ),
                                            Variant::typeToName( value.type() ) ) );
    }
#endif
    if( mValue != value ) {
        mValue.copy(value);
        mModified = true;
    }
    mNull = false;
#ifdef _GONG_DEBUG
    if( mValue != value ) {
        _GONG_DEBUG_WARNING( Xtring::printf("'%s' != '%s'", mValue.toString().c_str(), value.toString().c_str() ) );
//         bool check_again = (mValue != value );
    }
#endif
}


bool dbFieldValue::isEmpty() const
{
    if( mValue.type() == Variant::tBool )
        return isNull();
    else if( mValue.isNumeric( mValue.type() ) )
        return mValue.toDouble() == 0.0;
    else
        return mValue.isEmpty();
}


void dbFieldValue::clear( const Variant &defvalue )
{
    if( defvalue.isValid() ) {
        mValue = defvalue;
        mNull = false;
    } else {
        if( mValue.type() == Variant::tMoney )
            mValue = Money( 0.0, mValue.toMoney().getDecimals() );
        else
            mValue = Variant(mValue.type());
        mNull = true;
    }
    mModified = false;
}

}
