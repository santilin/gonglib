#include <cmath>
#include <stdexcept>
#include <limits>
#include "gongdebug.h"
#include "gongxtring.h"
#include "gongvariant.h"
#include "gongmoney.h"
#include "gongformatter.h"

namespace gong {

const unsigned long long max_money = std::numeric_limits<long long>::max() >> 4;

/**
       @class Money
       @brief Class to handle money amounts
*/

unsigned short int Money::stDefaultDecimals = 2;
static long long _exp10[8] = {
    1LL,
    10LL,
    100LL,
    1000LL,
    10000LL,
    100000LL,
    1000000LL,
    10000000LL
};

Money::Money(long long amount, unsigned short int ndecimals)
{
    _GONG_DEBUG_ASSERT(ndecimals < 8 );
    if( (amount < 0 ? -amount:amount) >= max_money )
        throw( std::overflow_error("Amount too big") );
    u.s.mDecimals = ndecimals;
    if( amount < 0LL ) {
        u.s.mSign = 1;
        u.s.mAmount = -amount * _exp10[getDecimals()];
    } else {
        u.s.mSign = 0;
        u.s.mAmount = amount * _exp10[getDecimals()];
    }
}

Money::Money(long long amount, unsigned short int ndecimals, unsigned short int sign, bool)
{
    _GONG_DEBUG_ASSERT(ndecimals < 8 );
    u.s.mSign = sign;
    u.s.mDecimals = ndecimals;
    u.s.mAmount = amount;
}

Money::Money(int amount, unsigned short int ndecimals)
{
    _GONG_DEBUG_ASSERT(ndecimals < 8 );
    if( (amount < 0 ? -amount:amount) >= max_money )
        throw( std::overflow_error("Amount too big") );
    u.s.mDecimals = ndecimals;
    if( amount < 0 ) {
        u.s.mSign = 1;
        u.s.mAmount = -amount * _exp10[getDecimals()];
    } else {
        u.s.mSign = 0;
        u.s.mAmount = amount * _exp10[getDecimals()];
    }
}


Money::Money(double amount, unsigned short int ndecimals)
{
    _GONG_DEBUG_ASSERT(ndecimals < 8 );
    if( (amount < 0 ? -amount:amount) >= max_money )
        throw( std::overflow_error("Amount too big") );
    u.s.mDecimals = ndecimals;
    if( amount < 0LL ) {
        u.s.mSign = 1;
        u.s.mAmount = static_cast<long long>(rint(-amount * _exp10[getDecimals()]));
    } else {
        u.s.mSign = 0;
        u.s.mAmount = static_cast<long long>(rint(amount * _exp10[getDecimals()]));
    }
    _GONG_DEBUG_PRINT(10, Xtring::printf("sign=%d, amount=%ld, decimals=%d",
                                         (int)u.s.mSign, (long long int)u.s.mAmount, (int)u.s.mDecimals) );
}

long long Money::toInt() const
{
    // TODO Issue a warning if decimals are lost?
    long long res = static_cast<long long>(u.s.mAmount / _exp10[u.s.mDecimals]);
    if( u.s.mSign )
        return -res;
    else
        return res;
}


double Money::toDouble() const
{
    double res = (double)u.s.mAmount / _exp10[u.s.mDecimals];
    if( u.s.mSign )
        return round(-res, getDecimals());
    else
        return round(res, getDecimals());
}

Money &Money::operator+=( const Money& other)
{
    if( u.s.mDecimals == other.u.s.mDecimals)
        u.s.mAmount += other.u.s.mAmount;
    else
        *this = Money(toDouble() + other.toDouble());
    return *this;
}

Money &Money::operator-=( const Money& other)
{
    if( u.s.mDecimals == other.u.s.mDecimals)
        u.s.mAmount -= other.u.s.mAmount;
    else {
        *this = Money(toDouble() - other.toDouble(), u.s.mDecimals);
    }
    return *this;
}


Money Money::operator+( const Money& other) const
{
    return Money(toDouble() + other.toDouble(), u.s.mDecimals);
}

Money Money::operator-( const Money& other) const
{
    return Money(toDouble() - other.toDouble(), u.s.mDecimals);
}

Money Money::operator*( double factor ) const
{
    return Money(toDouble() * factor, u.s.mDecimals );
}


Money Money::operator/( double factor ) const
{
    if( factor == 0 )
        return Money(0, u.s.mDecimals);
    else
        return Money(toDouble() / factor, u.s.mDecimals );
}

bool Money::operator==( const Money& other) const
{
    if( u.s.mDecimals == other.u.s.mDecimals)
        return u.s.mAmount == other.u.s.mAmount && u.s.mSign == other.u.s.mSign;
    else
        return EpsilonCompare(toDouble(), other.toDouble()) == 0;
}


bool Money::operator<( const Money& other) const
{
    return EpsilonCompare(toDouble(), other.toDouble()) < 0;
}

bool Money::operator<=( const Money& other) const
{
    return EpsilonCompare(toDouble(), other.toDouble()) <= 0;
}

Money& Money::operator=( const Money& other )
{
    if( u.s.mDecimals == other.u.s.mDecimals ) {
        u = other.u;
    } else {
        double amount = other.toDouble();
        if( amount < 0LL ) {
            u.s.mSign = 1;
            u.s.mAmount = static_cast<long long>(rint(-amount * _exp10[getDecimals()]));
        } else {
            u.s.mSign = 0;
            u.s.mAmount = static_cast<long long>(rint(amount * _exp10[getDecimals()]));
        }
    }
    return *this;
}


Money& Money::operator=( int amount )
{
    if( amount < 0LL ) {
        u.s.mSign = 1;
        u.s.mAmount = -amount * _exp10[getDecimals()];
    } else {
        u.s.mSign = 0;
        u.s.mAmount = amount * _exp10[getDecimals()];
    }
    return *this;
}

Money& Money::operator=( long long amount )
{
    if( amount < 0LL ) {
        u.s.mSign = 1;
        u.s.mAmount = -amount * _exp10[getDecimals()];
    } else {
        u.s.mSign = 0;
        u.s.mAmount = amount * _exp10[getDecimals()];
    }
    return *this;
}

Money& Money::operator=( double amount )
{
    if( amount < 0LL ) {
        u.s.mSign = 1;
        u.s.mAmount = static_cast<long long>(rint(-amount * _exp10[getDecimals()]));
    } else {
        u.s.mSign = 0;
        u.s.mAmount = static_cast<long long>(rint(amount * _exp10[getDecimals()]));
    }
    return *this;
}


Xtring Money::toString() const
{
    Xtring fmt = "%." + Xtring::number( getDecimals() ) + "f";
    return Xtring::printf( fmt, toDouble() );
}

Xtring Money::toString( const RegConfig *regconfig, const char *aformat, const char *amask ) const
{
    if( !regconfig )
        return toString();
    else {
        Formatter f(*regconfig);
        return f.format( *this, "", "" );
    }
}

} // namespace gong

