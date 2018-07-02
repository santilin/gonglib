#include <cfloat>
#include <cmath>
#include <stdexcept>
#include <limits>
#include <netinet/in.h>

#include "gongdebug.h"
#include "gongformatter.h"
#include "gongvariant.h"

namespace gong {

/**
    @class Variant
    @brief A simple value holder that can act as different types like integers, dates, strings, etc.

    Some of the contained types are integral types: tBool, tInt, tLong and tDouble.

    Others, tMoney, tDate, tTime and tDateTime correspond to the gong::Money, gong::Date,
    gong::Time and gong::DateTime classes. The storage of all of them is a long integer as they
    are packed into the variant.

    The rest, tString and tBinary, are pointers that are allocated at construction
    and deallocated at destruction.

    The normal operations are construction and destruction, copy, conversions and
    arithmetic and logical operations. Some operations are forbidden, and in case
    of being used, the program aborts. When a Variant can not be converted to a different type,
    an appropiated default value for that type is returned.

 */

Variant::Variant( const Variant &other )
{
    switch ( mType = other.mType ) {
    case tInvalid:
        break;
    case tBool:
        mValue.b = other.mValue.b;
        break;
    case tInt:
        mValue.i = other.mValue.i;
        break;
    case tLong:
        mValue.l = other.mValue.l;
        break;
    case tDouble:
        mValue.d = other.mValue.d;
        break;
    case tString:
    case tBinary:
        mValue.stringptr = new Xtring(*other.mValue.stringptr);
        break;
    case tDate:
        mValue.date = other.mValue.date;
        break;
    case tTime:
        mValue.time = other.mValue.time;
        break;
    case tDateTime:
        mValue.datetime = other.mValue.datetime;
        break;
    case tMoney:
        mValue.money = other.mValue.money;
        break;
    }
}

Variant::~Variant()
{
    switch ( mType ) {
    case tString:
    case tBinary:
        if( mValue.stringptr )
            delete mValue.stringptr;
        break;
	default:
		break;
	}
}

void Variant::clear()
{
    switch ( mType ) {
    case tString:
    case tBinary:
        if( mValue.stringptr )
            *mValue.stringptr = Xtring("");
        break;
    case tMoney:
        mValue.money = Money( 0.0, toMoney().getDecimals() ).packed();
        break;
    default:
        mValue.stringptr = 0;
        break;
    }
}

/**
	Construye un variant vacío del tipo especificado
 */
Variant::Variant( const Type &atype )
{
    switch ( mType = atype ) {
    case tString:
    case tBinary:
        mValue.stringptr = new Xtring();
        break;
    case tMoney: {
        Money m;
        mValue.money = m.packed();
        break;
    }
    default:
        mValue.l = 0;
    }
}

Variant::Variant( bool val )
    : mType( tBool )
{
    mValue.b = val;
}

Variant::Variant( char val )
    : mType( tString )
{
    mValue.stringptr = new Xtring( 1, val );
}

Variant::Variant( int val )
    : mType( tInt )
{
    mValue.i = val;
}

Variant::Variant( uint val )
    : mType( tInt )
{
    mValue.i = static_cast<int>(val);
}


Variant::Variant( long long val )
    : mType( tLong )
{
    mValue.l = val;
}

Variant::Variant( double val )
    : mType( tDouble )
{
    mValue.d = val;
}

Variant::Variant( const Xtring &val )
    : mType( tString )
{
    mValue.stringptr = new Xtring( val );
}

/// \todo {add} Use RegConfig for every type
Variant::Variant( const char *svalue, Variant::Type t, unsigned long len, const RegConfig *rc )
{
    mType = t;
    switch( t ) {
    case Variant::tString:
    case Variant::tBinary:
        if( len )
            mValue.stringptr = new Xtring(svalue, len);
        else
            mValue.stringptr = new Xtring(svalue);
        break;
    case Variant::tBool:
        mValue.b = Xtring(svalue).toBool();
        break;
    case Variant::tInt:
        mValue.i = Xtring(svalue).toInt();
        break;
    case Variant::tLong:
        mValue.l = Xtring(svalue).toLong();
        break;
    case Variant::tDouble:
        mValue.d = Xtring(svalue).toDouble();
        break;
    case Variant::tMoney:
        mValue.money = Money(Xtring(svalue).toDouble()).packed();
        break;
    case Variant::tDate:
        mValue.date = Date(svalue, rc?rc->getDateFormat().c_str():0).getJulianDay();
        break;
    case Variant::tTime:
        mValue.time = Time(svalue,rc?rc->getTimeFormat().c_str():0).getTotalSeconds();
        break;
    case Variant::tDateTime:
        mValue.datetime = DateTime(svalue,rc?rc->getDateTimeFormat().c_str():0).packed();
        break;
    case Variant::tInvalid:
        break;
    }
}

Variant::Variant( const char *val )
{
    mType = tString;
    mValue.stringptr = new Xtring( val );
}

Variant::Variant( const Date &val )
{
    mType = tDate;
    mValue.date = val.getJulianDay();
}

Variant::Variant( const Time &val )
{
    mType = tTime;
    mValue.time = val.getTotalSeconds();
}

Variant::Variant( const DateTime &val )
{
    mType = tDateTime;
    mValue.datetime = val.packed();
}

Variant::Variant( const Money &val )
{
    mType = tMoney;
    mValue.money = val.packed();
}

// Stores an array of long ints, the first of which is the lenght of the data
Variant::Variant( const void *data, long length )
{
    mType = tBinary;
    mValue.stringptr = new Xtring( (const char *)data, length );
}

/*************************************************************************************/

bool Variant::isNumeric( Type type )
{
    return ( type == tInt || type == tDouble || type == tMoney || type == tBool || type == tLong );
}

bool Variant::isZero() const
{
    switch ( mType ) {
    case tInvalid:
        return true;
    case tBool:
        return mValue.b == false;
    case tInt:
        return mValue.i == 0;
    case tLong:
        return mValue.l == 0L;
    case tDouble:
        return mValue.d == 0.0;
    case tMoney:
        return toDouble() == 0.0;
    case tString:
    case tBinary:
        return mValue.stringptr->isEmpty();
    case tDate:
        return mValue.date == 0;
    case tTime:
        return mValue.time == 0;
    case tDateTime:
        return mValue.datetime == 0;
    }
    return true;
}

/**
 * @brief Comprueba si un variant está vacío. Ojo, los numéricos nunca están vacíos
 * 
 * @return bool
 */
bool Variant::isEmpty() const
{
    switch ( mType ) {
    case tInvalid:
        return true;
    case tBool:
    case tInt:
    case tLong:
    case tDouble:
    case tMoney:
        return false;
    case tString:
    case tBinary:
        return mValue.stringptr->isEmpty();
    case tDate:
        return mValue.date == 0;
    case tTime:
        return mValue.time == 0;
    case tDateTime:
        return mValue.datetime == 0;
    }
    return true;
}


bool Variant::toBool() const
{
    switch ( mType ) {
    case tBool:
        return mValue.b;
    case tInt:
        return mValue.i != 0;
    case tLong:
        return mValue.l != 0LL;
    case tDouble:
        return mValue.d != 0.0;
    case tString:
        return mValue.stringptr->toBool();
    case tBinary:
        return !isEmpty();
    case tMoney:
        return ( Money::unpack(mValue.money).toInt() != 0 );
    case tDate:
        return mValue.date != 0;
    case tTime:
        return mValue.time != 0;
    case tDateTime:
        return mValue.datetime != 0;
    case tInvalid:
    default:
        return false;
    }
}


int Variant::toInt( bool * ok ) const
{
    if( ok )
        *ok = true;
    switch ( mType ) {
    case tInt:
        return mValue.i;
    case tString:
        return mValue.stringptr->toInt( ok );
    case tDouble:
        return static_cast<int>(mValue.d);
    case tBool:
        return static_cast<int>(mValue.b);
    case tMoney:
        return ( Money::unpack(mValue.money).toInt() );
    case tLong:
        return mValue.l;
    case tDate:
        return mValue.date;
    case tTime:
        return mValue.time;
    case tDateTime:
        return mValue.date; // JulianDay
    case tBinary:
    case tInvalid:
        if( ok )
            *ok = false;
        return 0;
    }
    return 0;
}

uint Variant::toUInt( bool * ok ) const
{
    if( ok )
        *ok = true;
    switch ( mType ) {
    case tInt:
        return static_cast<uint>(mValue.i);
    case tString:
        return mValue.stringptr->toUInt( ok );
    case tDouble:
        return static_cast<uint>(mValue.d);
    case tBool:
        return static_cast<uint>(mValue.b);
    case tMoney:
        throw std::runtime_error( "Money can not be converted into uint" );
    case tLong:
        return static_cast<uint>(mValue.l);
    case tDate:
        return mValue.date;
    case tTime:
        return mValue.time;
    case tDateTime:
        return mValue.date; // JulianDay
    case tBinary:
    case tInvalid:
        if( ok )
            *ok = false;
        return 0;
    }
    return 0;
}

/**
    If \a ok is non-null: \a *ok is set to TRUE if the value could be
    converted to a double; otherwise \a *ok is set to FALSE.
 */

long long Variant::toLong( bool * ok ) const
{
    if( ok )
        *ok = true;
    switch ( mType ) {
    case tLong:
        return mValue.l;
    case tInt:
        return mValue.i;
    case tString:
        return mValue.stringptr->toLong( ok );
    case tDouble:
        return static_cast<long long>(mValue.d);
    case tBool:
        return static_cast<long long>(mValue.b);
    case tMoney:
        return ( Money::unpack(mValue.money).toInt() );
    case tDate:
        return static_cast<long long>(mValue.date);
    case tTime:
        return static_cast<long long>(mValue.time);
    case tDateTime:
        return static_cast<long long>(mValue.date); // JulianDay
    case tBinary: // Returns the array size
        if( !isEmpty() )
            return mValue.stringptr->size();
        else
            return 0;
    case tInvalid:
    default:
        if( ok ) *ok = false;
        return 0LL;
    }
}

/**
    If \a ok is non-null: \a *ok is set to TRUE if the value could be
    converted to a double; otherwise \a *ok is set to FALSE.
 */

double Variant::toDouble( bool * ok ) const
{
    if( ok )
        *ok = true;
    switch ( mType ) {
    case tString:
        return mValue.stringptr->toDouble( ok );
    case tDouble:
        return mValue.d;
    case tInt:
        return static_cast<double>(mValue.i);
    case tLong:
        return static_cast<double>(mValue.l);
    case tBool:
        return static_cast<double>(mValue.b);
    case tMoney:
        return ( Money::unpack(mValue.money).toDouble() );
    case tDate:
        return mValue.date;
    case tTime:
        return mValue.time;
    case tDateTime:
        return mValue.date; // JulianDay
    case tBinary:
    case tInvalid:
        if( ok ) *ok = false;
        return 0.0;
    }
    return 0;
}


const Xtring Variant::toString() const
{
    switch ( mType ) {
    case tString:
    case tBinary:
        return *mValue.stringptr;
    case tInt:
        return Xtring::number( mValue.i );
    case tLong:
        return Xtring::number( mValue.l );
    case tDouble:
        return Xtring::number( mValue.d );
    case tBool:
        return mValue.b ? "true" : "false";
    case tDate:
        return Date(mValue.date).toString();
    case tTime:
        return Time(mValue.time).toString();
    case tDateTime:
        return DateTime::unpack(mValue.datetime).toString();
    case tMoney:
        return ( Money::unpack(mValue.money).toString() );
    case tInvalid:
    default:
        return Xtring();
    }
}


const Date Variant::toDate() const
{
    switch ( mType ) {
    case tDate:
        return mValue.date;
    case tDateTime:
        return mValue.date;
    case tString:
        return Date(*mValue.stringptr);
    case tInt:
        return Date(mValue.i);
    case tLong:
        return Date(mValue.l);
    case tDouble:
        return Date(static_cast<int>(mValue.d));
    case tBinary:
    default:
        return Date(Date::BadDate);
    }
}

const Time Variant::toTime() const
{
    switch ( mType ) {
    case tTime:
        return Time(mValue.time);
    case tDateTime:
        return Time(DateTime::unpack(mValue.datetime));
    case tString:
        return Time(*mValue.stringptr);
    case tInt:
        return Time(mValue.i);
    case tLong:
        return Time(mValue.l);
    case tDouble:
        return Time(static_cast<long>(mValue.d));
    case tBinary:
    default:
        return Time(Time::BadTime);
    }
}

const DateTime Variant::toDateTime() const
{
    switch ( mType ) {
    case tDateTime:
        return DateTime::unpack(mValue.datetime);
    case tDate:
        return Date(mValue.date);
    case tTime:
        return Time(mValue.time);
    case tString:
        return DateTime(*mValue.stringptr);
    case tInt:
        return DateTime(Date(mValue.i));
    case tLong:
        return DateTime(Date(mValue.l));
    case tDouble:
        return DateTime(Date(static_cast<int>(mValue.d)));
    case tBinary:
    default:
        return DateTime(Date::BadDate,Time::BadTime);
    }
}


const Money Variant::toMoney( short int ndecimals, bool * ok ) const
{
    if( ok )
        *ok = true;
    switch ( mType ) {
    case tMoney: // The ndecimasl from the money takes precedence over the parameter
        return Money(Money::unpack(mValue.money));
    case tString:
        return Money( toDouble( ok ), ndecimals );
    case tInt:
        return Money( static_cast<long long>(mValue.i), ndecimals );
    case tLong:
        return Money( mValue.l, ndecimals );
    case tDouble:
        return Money( mValue.d, 7 );
    case tBool:
        return Money( mValue.b ? 1LL : 0LL, ndecimals );
    case tBinary:
    default:
        return Money(0LL, ndecimals );
    }
}


/**
 * Assigns other to this variant, including type and value
 * @param other
 * @return
 */
Variant& Variant::operator= ( const Variant& other )
{
    if ( this == &other )
        return * this;
    clear();
    mType = other.mType;
    switch ( mType ) {
    case tInvalid:
        break;
    case tBool:
        mValue.b = other.toBool();
        break;
    case tInt:
        mValue.i = other.toInt();
        break;
    case tLong:
        mValue.l = other.toLong();
        break;
    case tDouble:
        mValue.d = other.toDouble();
        break;
    case tString:
    case tBinary:
        mValue.stringptr = new Xtring( other.toString() );
        break;
    case tDate:
        mValue.date = other.toDate().getJulianDay();
        break;
    case tTime:
        mValue.time = other.toTime().getTotalSeconds();
        break;
    case tDateTime:
        mValue.datetime = other.toDateTime().packed();
        break;
    case tMoney:
        mValue.money = other.toMoney().packed();
        break;
    }
    return *this;
}

/**
 * Assigns this variant to other, but converting the other to this type unless this is invalid, in which case assigns the other to this
 * This is required when we want to copy a variant but don't want to lose its type (example: db::FieldValue)
 * @param other
 * @return
 */
Variant& Variant::copy( const Variant& other )
{
    if ( this == &other )
        return * this;
    clear();
    if( mType == tInvalid )
        mType = other.mType;
    switch ( mType ) {
    case tInvalid:
        break;
    case tBool:
        mValue.b = other.toBool();
        break;
    case tInt:
        mValue.i = other.toInt();
        break;
    case tLong:
        mValue.l = other.toLong();
        break;
    case tDouble:
        mValue.d = other.toDouble();
        break;
    case tString:
    case tBinary:
        mValue.stringptr = new Xtring( other.toString() );
        break;
    case tDate:
        mValue.date = other.toDate().getJulianDay();
        break;
    case tTime:
        mValue.time = other.toTime().getTotalSeconds();
        break;
    case tDateTime:
        mValue.datetime = other.toDateTime().packed();
        break;
    case tMoney:
		if( other.mType == tMoney )
			mValue.money = other.toMoney().packed();
		else
			*this = Money(other.toDouble(), toMoney().getDecimals());
		// Money(other.toDouble(), other.toMoney().getDecimals() ).packed();
        break;
    }
    return *this;
}


Variant& Variant::operator+= ( const Variant& other )
{
    switch ( mType ) {
    case tBool:
        mValue.b |= other.toBool();
        break;
    case tInt:
        mValue.i += other.toInt();
        break;
    case tLong:
        mValue.l += other.toLong();
        break;
    case tDouble:
        mValue.d += other.toDouble();
        break;
    case tString:
        mValue.stringptr->operator+=( other.toString() );
        break;
    case tMoney:
        *this = toMoney() + other.toMoney();
        break;
    case tDate:
        mValue.date = mValue.date + other.toDate().getJulianDay();
        break;
    case tTime:
        mValue.time = mValue.time + other.toTime().getTotalSeconds();
        break;
    case tDateTime:
    case tBinary:
        throw std::runtime_error(Xtring("Invalid operator += on ") + typeToName(mType) + "\n");
        break;
    case tInvalid:
    default:
        *this = other;
        break;
    }
    return *this;
}

static const char *variant_names[] = {
    // Standard names
    "invalid",
    "bool",
    "int",
    "long",
    "double",
    "string",
    "date",
    "time",
    "dateTime",
    "money",
    "binary"
};

const char *Variant::typeToName(Type type)
{
    return variant_names[type];
}


Variant::Type Variant::nameToType( const char* name )
{
    if( strempty(name) )
        return tString;
    for( uint i=0; i<sizeof(variant_names)/sizeof(variant_names[0]); i++ )
        if( strcasecmp( name, variant_names[i]) == 0 )
            return Type(i);
    if( strcasecmp( name, "integer" ) == 0 || strcasecmp( name, "uint" ) == 0
            || strcasecmp( name, "unsigned" ) == 0  || strcasecmp( name, "unsigned int" ) == 0
            || strcasecmp( name, "number" ) == 0 )
        return tInt;
    else if( strcasecmp( name, "long long" ) == 0 )
        return tLong;
    else if( strcasecmp( name, "boolean" ) == 0 )
        return tBool;
    else if( strcasecmp( name, "real" ) == 0  || strcasecmp( name, "float" ) == 0 )
        return tDouble;
    else if( strcasecmp( name, "timestamp" ) == 0 )
        return tDateTime;
    else if( strcasecmp( name, "binary" ) == 0 || strcasecmp( name, "image" ) == 0
             || strcasecmp( name, "picture" ) == 0 )
        return tBinary;
    else if( strcasecmp( name, "currency" ) == 0  || strcasecmp( name, "decimal" ) == 0 )
        return tMoney;
    else if( strcasecmp( name, "text" ) == 0  )
        return tString;
    else {
        return tInvalid;
    }
}


int Variant::serializeAscendent(bool asc, char **bufferptr, int *pos, int *len) const
{
    if( *bufferptr == 0 )
        *len = 0;
    char *newbuffer, numericdata[100];
    int datalength = 0;
    const char *data = 0;
    switch ( type() ) {
    case tString:
    case tBinary:
        data = mValue.stringptr->c_str();
        if( data == 0 )
            datalength = sizeof(char); // Only the \0
        else
            datalength = strlen(data) + sizeof(char);
        break;
    case tInt:
    case tDate:
    case tTime:
        if( mValue.i > 0 )
            sprintf(numericdata, " % *d", 12, mValue.i);
        else { // Negative values need to be sorted
            if( asc ) {
                sprintf(numericdata, "\001% *d", 12,
                        std::numeric_limits<int>::min() + mValue.i );
            } else {
                sprintf(numericdata, "\001% *d", 12,
                        - mValue.i );
            }
        }
        datalength = strlen(numericdata) + sizeof(char);
        break;
    case tLong:
    case tDateTime:
        if( mValue.l > 0 )
            sprintf(numericdata, " % *Ld", 12, mValue.l);
        else { // Negative values need to be sorted
            if( asc ) {
                sprintf(numericdata, "\001% *Ld", 12,
                        std::numeric_limits<int>::min() + mValue.l );
            } else {
                sprintf(numericdata, "\001% *Ld", 12,
                        - mValue.l );
            }
        }
        datalength = strlen(numericdata) + sizeof(char);
        break;
    case tDouble:
        sprintf(numericdata, "% *.*f", 10, 20, mValue.d);
        datalength = strlen(numericdata) + sizeof(char);
        break;
    case tBool:
        datalength = sizeof(bool);
        break;
    case tMoney:
        datalength = sizeof(Money);
        break;
    case Variant::tInvalid:
        datalength = 0;
    }
    datalength += sizeof(char); // Type
    if( *pos + datalength > *len ) { // Resize the buffer
        int newlength = *len + (datalength>1024?datalength:1024);
        newbuffer = new char[newlength];
        memcpy(newbuffer, *bufferptr, *len);
        *len = newlength;
        delete *bufferptr;
        *bufferptr = newbuffer;
    }
    char *pdest = *bufferptr + *pos;
    *pdest++ = static_cast<char>(type());  // Without pointer
    switch ( mType ) {
    case tString:
    case tBinary:
        if( data == 0 )
            *pdest++ = '\0';
        else
            strcpy( pdest, data ); // OPTIM
        break;
    case tInt:
    case tDate: // Stored like an integer
    case tTime: // Stored like an integer
        strcpy( pdest, numericdata ); // OPTIM
        break;
    case tLong:
    case tDateTime: // Stored like a long integer
        strcpy( pdest, numericdata ); // OPTIM
        break;
    case tDouble:
        strcpy( pdest, numericdata ); // OPTIM
        break;
    case tBool:
        *reinterpret_cast<bool *>(pdest) = mValue.b;
        break;
    case tMoney:
        *reinterpret_cast<long long *>(pdest) = *reinterpret_cast<const long long *>(&mValue);
        break;
    case tInvalid:
        break;
    }
    *pos += datalength;
    return datalength;

}

/**
 * @brief Serialize this variant to a char buffer.

    If the buffer is not big enough for the data, reallocates it

 * @param bufferptr
 * @param pos
 * @param len
 * @return The length of the serialized data
 */
int Variant::serialize(char **bufferptr, int *pos, int *len) const
{
    if( *bufferptr == 0 )
        *len = 0;
    char *newbuffer, *numericdata = 0;
    int datalength = 0;
    const char *data = 0;
    switch ( type() ) {
    case tString:
    case tBinary:
        data = mValue.stringptr->c_str();
        if( data == 0 )
            datalength = sizeof(char); // Only the \0
        else
            datalength = strlen(data) + sizeof(char);
        break;
    case tInt:
        datalength = sizeof(int);
        break;
    case tLong:
        datalength = sizeof(long long);
        break;
    case tDouble:
        numericdata = new char[100];
        sprintf(numericdata, "% *.*f", 10, 20, mValue.d);
        datalength = strlen(numericdata) + sizeof(char);
#if 0
        This is a try to serialize the double in a more compressed way, but it didnot work
            due to problems with decimal roundings
        {
            long long double_int_part, double_dec_part;

            if( mValue.d < std::numeric_limits<long long>::min()
            || mValue.d > std::numeric_limits<long long>::max() ) {
                std::cerr << __FILE__ << ":" << __LINE__ << ": Double value too big: " << mValue.d << std::endl;
                abort();
            }
            bool negative = ( mValue.d < 0 );
            double int_part;
            double dec_part = fabs(modf(mValue.d, &int_part));
            double_int_part = static_cast<long long>int_part;
            double_dec_part = 1;
            while( dec_part != 0 ) {
                if( negative && double_dec_part < std::numeric_limits<long long>::min() / 10 )
                    break;
                if ( !negative && double_dec_part > std::numeric_limits<long long>::max() / 10)
                    break;
                dec_part *= 10;
                dec_part = modf(dec_part, &int_part);
                double_dec_part = double_dec_part * 10 + static_cast<long long>int_part;
            }
            if( negative )
                double_dec_part = -double_dec_part;
            /*              if( ndigits > 8 ) {
                                if( lastdigit == 9 )
                                    double_dec_part += 1;
                                else if( lastdigit == -9 )
                                    double_dec_part -= 1;
                                else if( lastdigit == 1 )
                                    double_dec_part -= 1;
                                else if( lastdigit == -1 )
                                    double_dec_part += 1;
                            }*/
            datalength = 2 * sizeofstatic_cast<long long>;
        }
#endif
        break;
    case tDate:
        datalength = sizeof(Date);
        break;
    case tTime:
        datalength = sizeof(Time);
        break;
    case tDateTime:
        datalength = sizeof(DateTime);
        break;
    case tBool:
        datalength = sizeof(bool);
        break;
    case tMoney:
        datalength = sizeof(Money);
        break;
    case tInvalid:
        datalength = 0;
    }
    datalength += sizeof(char); // Type
    if( *pos + datalength > *len ) { // Resize the buffer
        int newlength = *len + (datalength>1024?datalength:1024);
        newbuffer = new char[newlength];
        memcpy(newbuffer, *bufferptr, *len);
        *len = newlength;
        delete *bufferptr;
        *bufferptr = newbuffer;
    }
    char *pdest = *bufferptr + *pos;
    *pdest++ = static_cast<char>(type());  // Without pointer
    switch ( mType ) {
    case tString:
    case tBinary:
        if( data == 0 )
            *pdest++ = '\0';
        else
            strcpy( pdest, data ); // OPTIM
        break;
    case tInt:
    case tDate: // Stored like an integer
    case tTime: // Stored like an integer
        *reinterpret_cast<int *>(pdest) = htonl(mValue.i);
        break;
    case tLong: /// \todo {refactor} htonl is only for 32 bits.
        *reinterpret_cast<long long *>(pdest) = mValue.l;
        break;
    case tDateTime: // Stored like a long integer
        *reinterpret_cast<long long *>(pdest) = mValue.datetime;
        break;
    case tDouble:
        strcpy( pdest, numericdata ); // OPTIM
        delete numericdata;
#if 0
        *static_cast<long long *>(pdest) = double_int_part;
        *static_cast<long long *>(pdest + sizeofstatic_cast<long long>) = double_dec_part;
#endif
        break;
    case tBool:
        *reinterpret_cast<bool *>(pdest) = mValue.b;
        break;
    case tMoney:
        *reinterpret_cast<long long *>(pdest) = *reinterpret_cast<const long long *>(&mValue);
        break;
    case Variant::tInvalid:
        break;
    }
    *pos += datalength;
    return datalength;
}

Variant Variant::deserialize(const char **serialized)
{
    Variant v;
    const char *source = *serialized;
    int len = 0;
    v.mType = Type(*source++);
    switch ( v.mType ) {
    case tString:
    case tBinary:
        v.mValue.stringptr = new Xtring( source );  // null terminated string
        len = v.mValue.stringptr->length() + 1;
        break;
    case tBool:
        v.mValue.b = *reinterpret_cast<const bool *>(source);
        len = sizeof(bool);
        break;
    case tInt:
        v.mValue.i = ntohl(*reinterpret_cast<const int *>(source));
        len = sizeof(int);
        break;
    case tLong:
        v.mValue.l = *reinterpret_cast<const long long *>(source);
        len = sizeof(long long);
        break;
    case tDouble:
    {   // This is valid as long as the locale doesn't change between serialize and deserialize
        const char *numericdata = source;  // null terminated string
        len = strlen(numericdata) + 1;
        v.mValue.d = strtod(numericdata, 0);
    }
    break;
#if 0
    double int_part = *static_cast<long long *>source;
    double dec_part = *static_cast<long long *>(source + sizeofstatic_cast<long long>);
    len = 2 * sizeofstatic_cast<long long>;
    long long divisor= 10;
    while( dec_part >= divisor || dec_part <= -divisor ) {
        divisor *= 10;
    }
    divisor /= 10;
    dec_part /= divisor;
    if( dec_part > 0 )
        dec_part -= 1;
    else
        dec_part += 1;
    mValue.d = int_part + dec_part;
#endif
    case tDate:
        v.mValue.date = ntohl(*reinterpret_cast<const long *>(source));
        len = sizeof(long);
        break;
    case tTime:
        v.mValue.time = ntohl(*reinterpret_cast<const long *>(source));
        len = sizeof(long);
        break;
    case tDateTime:
        v.mValue.datetime = *reinterpret_cast<const long long *>(source);
        len = sizeof(DateTime);
        break;
    case tMoney:
        v.mValue.money = *reinterpret_cast<const long long *>(source);
        len = sizeof(Money);
        break;
    case tInvalid:
        break;
    }
    *serialized += len + 1;
    return v;
}

const Variant operator+( const Variant &v1, const Variant &v2 )
{
    switch ( v1.type() )
    {
    case Variant::tBool:
        // bool op int returns int
        return v1.toBool() + v2.toInt();
    case Variant::tInt:
        return v1.toInt() + v2.toInt();
    case Variant::tLong:
        return v1.toLong() + v2.toLong();
    case Variant::tDouble:
        return v1.toDouble() + v2.toDouble();
    case Variant::tString:
        return Variant(v1.toString() + v2.toString());
    case Variant::tMoney:
        return v1.toMoney() + v2.toMoney();
    case Variant::tDate:
        return v1.toDate() + v2.toInt(); // date + date not defined
    case Variant::tTime:
        return v1.toTime() + v2.toInt(); // time + time not defined
    case Variant::tDateTime:
        return v1.toDateTime();
    case Variant::tBinary:
    case Variant::tInvalid:
        break;
    }
    return v2;
}

const Variant operator-( const Variant &v1, const Variant &v2 )
{
    switch ( v1.type() )
    {
    case Variant::tBool:
        return v1.toBool() - v2.toInt();
    case Variant::tInt:
        return v1.toInt() - v2.toInt();
    case Variant::tLong:
        return v1.toLong() - v2.toLong();
    case Variant::tDouble:
    case Variant::tString: // Convert to number
        return v1.toDouble() - v2.toDouble();
    case Variant::tMoney:
        return v1.toMoney() - v2.toMoney();
    case Variant::tDate:
        return v1.toDate() - v2.toInt();
    case Variant::tTime:
        return v1.toTime() - v2.toInt();
    case Variant::tDateTime:
        return v1.toDateTime();
    case Variant::tBinary:
    case Variant::tInvalid:
        break;
    }
    return v2;
}

const Variant operator*( const Variant &v1, const Variant &v2 )
{
    switch ( v1.type() )
    {
    case Variant::tBool:
        return v1.toBool() * v2.toInt();
    case Variant::tInt:
        return v1.toInt() * v2.toInt();
    case Variant::tLong:
        return v1.toLong() * v2.toLong();
    case Variant::tDouble:
    case Variant::tString: // Convert to number
        return v1.toDouble() * v2.toDouble();
    case Variant::tMoney:
        return v1.toMoney() * v2.toDouble();
    case Variant::tDate:
        return Date(v1.toInt() * v2.toInt());
    case Variant::tTime:
        return Time(v1.toInt() * v2.toInt());
    case Variant::tDateTime:
        return v1.toDateTime();
    case Variant::tBinary:
    case Variant::tInvalid:
        break;
    }
    return v2;
}

const Variant operator/( const Variant &v1, const Variant &v2 )
{
    switch ( v1.type() )
    {
    case Variant::tBool:
        return v1.toBool() / v2.toInt();
    case Variant::tInt:
        return v1.toInt() / v2.toInt();
    case Variant::tLong:
        return v1.toLong() / v2.toLong();
    case Variant::tDouble:
    case Variant::tString: // Convert to number
        return v1.toDouble() / v2.toDouble();
    case Variant::tMoney:
        return v1.toMoney() / v2.toDouble();
    case Variant::tDate:
        return Date(v1.toInt() / v2.toInt());
    case Variant::tTime:
        return Time(v1.toInt() / v2.toInt());
    case Variant::tDateTime:
        return v1.toDateTime();
    case Variant::tBinary:
    case Variant::tInvalid:
        break;
    }
    return v2;
}

bool Variant::operator==( const Variant &other ) const
{
    bool otherok;
    switch ( mType ) {
    case tBool:
        return mValue.b == other.toBool();
    case tInt:
    {
        int otherval = other.toInt(&otherok);
        return ( otherok && mValue.i == otherval );
    }
    case tLong:
    {
        long long otherval = other.toLong(&otherok);
        return ( otherok && mValue.l == otherval );
    }
    case tDouble:
    {
        double otherval = other.toDouble(&otherok);
        return ( otherok && areSame(mValue.d, otherval) );
    }
    case tMoney:
    {
        // Even if the values of the moneys are equal,
        // if the number of decimals are not, comparing the packed value would be wrong
		if( other.mType == tMoney ) {
			return toMoney() == other.toMoney();
		} else {
			int thisdec = toMoney().getDecimals();
			int otherdec = other.toMoney().getDecimals();
			if( otherdec == thisdec ) {
				return toMoney() == other.toMoney();
			} else {
				return toDouble() == other.toDouble();
			}
		}
    }
    case tString:
    case tBinary:
        return other.toString() == *mValue.stringptr;
    case tDate:
        return other.toDate() == toDate();
    case tTime:
        return other.toTime() == toTime();
    case tDateTime:
        return other.toDateTime() == toDateTime();
    case tInvalid:
        return ( other.type() == tInvalid );
    }
    return false;
}



bool Variant::operator<( const Variant &other ) const
{
    if( !isValid() && !other.isValid() )
        return false;
    if( !isValid() || !other.isValid() )
        return true;
    bool otherok;
    switch ( mType ) {
    case tBool:
        return mValue.b < other.toBool();
    case tInt:
    {
        int otherval = other.toInt(&otherok);
        return ( otherok && mValue.i < otherval );
    }
    case tDouble:
    {
        double otherval = other.toDouble(&otherok);
        return ( otherok && EpsilonCompare(mValue.d, otherval) < 0 );
    }
    case tMoney:
    {
        // I don't want to use the double comparison, but the money comparison
        Money otherval = other.toMoney( 7, &otherok);
        return ( otherok && toMoney() < otherval );
    }
    case tLong:
    {
        long long otherval = other.toLong(&otherok);
        return ( otherok && mValue.l < otherval );
    }
    case tString:
    case tBinary:
        return *mValue.stringptr < other.toString();
    case tDate:
        return toDate() < other.toDate();
    case tTime:
        return toTime() < other.toTime();
    case tDateTime:
        return toDateTime() < other.toDateTime();
    case tInvalid:
        break; // For completeness
    }
    return false;
}

bool Variant::operator>( const Variant &other ) const
{
    if( !isValid() && !other.isValid() )
        return false;
    if( !isValid() || !other.isValid() )
        return true;
    return !(*this <= other);
}

bool Variant::operator>=( const Variant &other ) const
{
    if( !isValid() && !other.isValid() )
        return false;
    if( !isValid() || !other.isValid() )
        return true;
    return !(*this < other);
}

bool Variant::operator<=( const Variant &other ) const
{
    if( !isValid() && !other.isValid() )
        return false;
    if( !isValid() || !other.isValid() )
        return true;
    bool otherok;
    switch ( mType ) {
    case tBool:
        return mValue.b <= other.toBool();
    case tInt:
    {
        int otherval = other.toInt(&otherok);
        return ( otherok && mValue.i <= otherval );
    }
    case tDouble:
    {
        double otherval = other.toDouble(&otherok);
        return ( otherok && EpsilonCompare(mValue.d, otherval) <= 0 );
    }
    case tMoney:
    {
        // I don't want to use the double comparison, but the money comparison
        Money otherval = other.toMoney( 7, &otherok);
        return ( otherok && toMoney() <= otherval );
    }
    case tLong:
    {
        long long otherval = other.toLong(&otherok);
        return ( otherok && mValue.l <= otherval );
    }
    case tString:
    case tBinary:
        return *mValue.stringptr <= other.toString();
    case tDate:
        return toDate() <= other.toDate();
    case tTime:
        return toTime() <= other.toTime();
    case tDateTime:
        return toDateTime() <= other.toDateTime();
    case tInvalid:
        break; // For completeness
    }
    return false;
}

double round(Money val)
{
    return round(val.toDouble(), val.getDecimals() );
}

double round(double d, short int ndecimals)
{
    double factor = 1;
    for( ; ndecimals > 0; --ndecimals )
        factor *= 10;
    d *= factor;
    double res;
    if( d >= 0.0 ) {
        if( d > std::numeric_limits<long long int>::max() )
            throw std::runtime_error( Xtring::printf("El número %.8f es demasiado grande para redondearlo", d).c_str() );
        res = floor(d + 0.5);
    } else {
        if( d < std::numeric_limits<long long int>::min() )
            throw std::runtime_error( Xtring::printf("El número %.8f es demasiado grande para redondearlo", d).c_str() );
        res = ::round(d); // (d - (long long int)(d-1) + 0.5) + (long long int)(d-1);
    }
    return res/factor;
}

long long int round(double d)
{
    if( d >= 0.0 ) {
        if( d > std::numeric_limits<long long int>::max() )
            throw std::runtime_error( Xtring::printf("El número %.8f es demasiado grande para redondearlo", d).c_str() );
        return (long long int)(d + 0.5);
    } else {
        if( d < std::numeric_limits<long long int>::min() )
            throw std::runtime_error( Xtring::printf("El número %.8f es demasiado grande para redondearlo", d).c_str() );
        return (long long int)(d - (long long int)(d-1) + 0.5) + (long long int)(d-1);
    }
}

bool areSame(double a, double b) {
    return std::fabs(a - b) < std::numeric_limits<double>::epsilon();
}

int EpsilonCompare( double a, double b )
{
    double _res = fabsf( a - b );
    if( _res < std::numeric_limits<double>::epsilon() )
        return 0;
    else if( a < b )
        return -1;
    else
        return 1;
}

} // namespace gong
