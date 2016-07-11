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

dbFieldValue::dbFieldValue(const Variant &v)
	: Variant(v), mNull(true), mModified(false)
{
}

dbFieldValue::dbFieldValue(Variant::Type type, bool isnull, const dbFieldDefinition *flddef)
    : Variant(type), mNull(isnull), mModified(false)
{
    if( type == Variant::tMoney ) {
		if( flddef ) {
			*this = Money(0.0, flddef->getDecimals() > 7 ? 7 : flddef->getDecimals());
		} else {
			*this = Variant(Money(0.0));
		}
		mNull = isnull;
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


/**
 * @brief Borra o pone un valor por defecto para el campo
 * Si se pasa un valor por defecto, el campo no se considera null.
 * La diferencia con setValue es que setValue hace comprobaciones de si se ha modificado,
 * mientras que clear impone que el campo no está modificado
 *
 * @param defvalue ...
 * @return void
 */
void dbFieldValue::clear( const Variant &defvalue )
{
    if( defvalue.isValid() ) {
        *this = defvalue;
        mNull = false;
    } else {
		Variant::clear();
        mNull = true;
    }
    mModified = false;
}

dbFieldValue &dbFieldValue::operator=(const Variant &other)
{
	return setValue(other);
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
    if( *this != other || (mType == tMoney && toMoney().getDecimals() != other.toMoney().getDecimals()) ) {
        copy(other); // Do not use = because we want to preserve the type
        mModified = true;
    }
    mNull = false;
#ifdef _GONG_DEBUG
    if( *this != other) {
        _GONG_DEBUG_WARNING( Xtring::printf("'%s' != '%s'", toString().c_str(), toString().c_str() ) );
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
