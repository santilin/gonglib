#ifndef _GONG_VARIANT_H
#define _GONG_VARIANT_H

/** @file gongvariant.h A simple Variant type
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include "gongxtring.h"
#include "gongdate.h"
#include "gongmoney.h"

namespace gong
{

class RegConfig;
class Formatter;

/**
    @class gong::Variant gongvariant.cpp <gongvariant.h>
    @brief A simple value object that can act as different types like integers, dates, strings, etc.

    Some of the contained types are integral types: tBool, tInt, tLong and tDouble.

    Others, tMoney, tDate, tTime and tDateTime correspond to the GONG::Money, GONG::Date,
    GONG::Time and GONG::DateTime classes. The storage of all of them is a long integer as they
    are packed into the variant.

    The rest, tString and tImage, are pointers that are allocated at construction
    and deallocated at destruction.

    The normal operations are construction and destruction, copy, conversions and
    arithmetic and logical operations. Some operations are forbidden, and in case
    of being used, the program aborts. When a Variant can not be converted to a different type,
    an appropiated default value for that type is returned.

 */

class Variant
{
public:
    /** The types of the supported Variants */
    enum Type {
        tInvalid,
        tBool,
        tInt,
        tLong,
        tDouble,
        tString,
        tDate,
        tTime,
        tDateTime,
        tMoney,
        tBinary /// A byte array
    };

    /** @brief Constructs an invalid, without type Variant */
    Variant() : mType( tInvalid ) {}
    /** @brief Copy constructor */
    Variant( const Variant &val );
    /** @brief Constructs a boolean Variant */
    Variant( bool val );
    /** @brief Constructs a char Variant (promoted to a Xtring) */
    Variant( char ch );
    /** @brief Constructs an integer Variant */
    Variant( int val );
    /** @brief Constructs an integer Variant */
    Variant( uint val );
    /** @brief Constructs a long long Variant */
    Variant( long long val );
    /** @brief Constructs a double Variant */
    Variant( double val );
    /** @brief Constructs a string Variant, storing a copy of the passed string */
    Variant( const Xtring &val );
    /** @brief Constructs a string Variant, storing a copy of the passed string */
    Variant( const char *val );
    /** @brief Constructs a Variant converting the string into the specified type */
    Variant( const char *svalue, Type t, unsigned long len = 0, const RegConfig *rc = 0);
    /** @brief Constructs a Date Variant */
    Variant( const Date &val );
    /** @brief Constructs a Time Variant */
    Variant( const Time &val );
    /** @brief Constructs a DateTime Variant */
    Variant( const DateTime &val );
    /** @brief Constructs a Money Variant */
    Variant( const Money &val );
    /** @brief Constructs a Binary Variant */
    Variant( const void *data, long length );
    /** @brief Constructs a Variant of type \a type */
    Variant( const Type &type );

    Variant& operator= ( const Variant &other );
    Variant& operator+=( const Variant &other );

    /** Copy this variant from another without changing its type */
    Variant &copy( const Variant &other );

    /** @brief Destructs the Variant, freeing the allocated space if neccesary */
    virtual ~Variant();

    /** @brief Returns the bool value of the variant */
    bool toBool() const;
    /** @brief Returns the variant as an int if can be cast to tInt or 0 if not */
    int toInt ( bool * ok = 0 ) const;
    /** @brief Returns the variant as an unsigned int if can be cast to tInt or 0 if not */
    uint toUInt ( bool * ok = 0 ) const;
    /** @brief Returns the variant as a long if can be cast to tLong or 0LL if not */
    long long toLong ( bool * ok = 0 ) const;
    /** @brief Returns the variant as a double if can be cast to tDouble or 0.0 if not */
    double toDouble( bool * ok = 0 ) const;
    /** @brief Returns the variant as a string */
    const Xtring toString() const;
    /** @brief Returns the variant as a Date if can be cast to tDate or an empty date if not */
    const Date toDate() const;
    /** @brief Returns the variant as a Time if can be cast to tTime or an empty time if not */
    const Time toTime() const;
    /** @brief Returns the variant as a DateTime if can be cast to tDateTime or an empty datetime if not */
    const DateTime toDateTime() const;
    /** @brief Returns the variant as a Money if can be cast to tMoney or an empty Money if not */
    const Money toMoney( short int ndecimals = Money::defaultDecimals(), bool *ok = 0 ) const;

    bool operator==( const Variant& other) const;
    bool operator!=( const Variant& other) const {
        return !( *this == other );
    }
    bool operator<(const Variant& other) const;
    bool operator<=(const Variant& other) const;
    bool operator>(const Variant& other ) const;
    bool operator>=(const Variant& other ) const;



    bool isValid() const {
        return ( mType != tInvalid );
    }
    bool isEmpty() const;
    bool isZero() const;
    Type type() const {
        return mType;
    }
    static bool isNumeric(Type type);
    static Type nameToType( const char* name );
    static const char *typeToName(Type type);

    // Serialize and deserialize of variants
    int serialize(char **bufferptr, int *pos, int *bufferlen) const;
    int serializeAscendent(bool asc, char **bufferptr, int *pos, int *bufferlen) const;
    static Variant deserialize( const char **serializedvariant );

    friend const Variant operator+( const Variant &v1, const Variant &v2 );
    friend const Variant operator-( const Variant &v1, const Variant &v2 );
    friend const Variant operator*( const Variant &v1, const Variant &v2 );
    friend const Variant operator/( const Variant &v1, const Variant &v2 );
    void clear();

    // This allows you to use variants in template functions
    template <class T> T to() const;

protected:
    Type mType;
    union {
        bool b;
        int i;
        long long l;
        double d;
        Xtring *stringptr;
        long date;
        long time;
        long long datetime;
        long long money;
    } mValue;

}; // class Variant

template <> inline bool Variant::to() const {
    return toBool();
}
template <> inline int Variant::to() const {
    return toInt();
}
template <> inline uint Variant::to() const {
    return toUInt();
}
template <> inline long long Variant::to() const {
    return toLong();
}
template <> inline double Variant::to() const {
    return toDouble();
}
template <> inline Xtring Variant::to() const {
    return toString();
}
template <> inline Date Variant::to() const {
    return toDate();
}
template <> inline DateTime Variant::to() const {
    return toDateTime();
}
template <> inline Time Variant::to() const {
    return toTime();
}

double round(double val, short int ndecimals);
long long int round(double val);
double round(Money val);
bool areSame(double, double);
int EpsilonCompare( double a, double b );

} // namespace gong
#endif // GONGVARIANT_H

