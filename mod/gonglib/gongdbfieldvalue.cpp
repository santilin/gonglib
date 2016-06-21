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

dbFieldValue::dbFieldValue(Variant::Type type, bool isnull, const dbFieldDefinition *flddef)
    : Variant(type), mNull(isnull), mModified(false)
{
    if( type == Variant::tMoney ) {
		if( flddef ) {
			*this = Variant(Money(0.0, flddef->getDecimals() > 7 ? 7 : flddef->getDecimals() ));
		} else {
			*this = Variant(Money(0.0));
		}
	}
	if (flddef) {
		_GONG_DEBUG_PRINT(0, flddef->getName() );
		_GONG_DEBUG_PRINT(0, mType );
	}
}

bool dbFieldValue::isEmpty() const
{
    if( type() == Variant::tBool )
        return isNull();
    else if( isNumeric( type() ) )
        return toDouble() == 0.0;
    else
        return Variant::isEmpty();
}


void dbFieldValue::clear( const Variant &defvalue )
{
    if( defvalue.isValid() ) {
        static_cast<Variant >(*this) = defvalue;
        mNull = false;
    } else {
		Variant::clear();
        mNull = true;
    }
    mModified = false;
}

dbFieldValue &dbFieldValue::operator=(const Variant &other)
{
	return setValue(&other);
}

dbFieldValue &dbFieldValue::setValue(const Variant &other)
{
#ifdef _GONG_DEBUG
    if( type() != other.type() ) {
        _GONG_DEBUG_WARNING( Xtring::printf("'%s' != '%s'",
                                            Variant::typeToName( type() ),
                                            Variant::typeToName( other.type() ) ) );
    }
#endif
    if( static_cast<Variant>(*this) != other) {
        copy(other);
        mModified = true;
    }
    mNull = false;
#ifdef _GONG_DEBUG
    if( static_cast<Variant>(*this) != other) {
        _GONG_DEBUG_WARNING( Xtring::printf("'%s' != '%s'", toString().c_str(), toString().c_str() ) );
//         bool check_again = (*this != value );
    }
#endif
	return *this;
}


bool dbFieldValue::operator==(const Variant &other) const
{
	return static_cast<Variant>(*this) == other;
}

double dbFieldValue::toDouble(bool *ok) const
{
	return toString().toDoubleLocIndep(ok);
}


}
