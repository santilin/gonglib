#ifndef _GONG_MONEY_H
#define _GONG_MONEY_H

/** @file gongmoney.h Class to handle currency amounts
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

namespace gong {

class Money
{
public:
    Money(long long amount, unsigned short int ndecimals = stDefaultDecimals);
    Money(int amount=0, unsigned short int ndecimals = stDefaultDecimals);
    Money(double amount, unsigned short int ndecimals = stDefaultDecimals);
    static void setDefaultDecimals(unsigned short int dec) {
        stDefaultDecimals = (dec>7?7:dec);
    }
    static unsigned short int defaultDecimals() {
        return stDefaultDecimals;
    }
    unsigned short int getDecimals() const {
        return u.s.mDecimals;
    }

    Xtring toString() const;
    Xtring toString( const class RegConfig *regconfig,
                     const char *aformat=0, const char *amask=0 ) const;
    long long toInt() const;
    double toDouble() const;

    Money &operator=(const Money &other);
    Money &operator=(int amount);
    Money &operator=(long long amount);
    Money &operator=(double amount);

    bool operator==( const Money& other) const;
    bool operator!=( const Money& other) const {
        return !(*this==other);
    }
    bool operator<( const Money& other) const;
    bool operator<=( const Money& other) const;
    bool operator>( const Money& other) const {
        return !(*this<=other);
    }
    bool operator>=( const Money& other) const {
        return !(*this<other);
    }
    Money &operator+=( const Money& other);
    Money &operator-=( const Money& other);
    Money operator -() const {
        return Money( -toDouble() );
    }

    Money operator+( const Money& other) const;
    Money operator-( const Money& other) const;
    Money operator*( double factor ) const;
    Money operator/( double factor ) const;

    long long packed() const {
        return u.mPacked;
    }
    static Money unpack(long long packed) {
        return *reinterpret_cast<Money *>(&packed);
    }

private:
    Money(long long amount, unsigned short int ndecimals, unsigned short int sign, bool internalconst);
    union {
        struct {
            unsigned long long mSign:1;
            unsigned long long mDecimals:3;
            unsigned long long mAmount:60;
        } s;
        unsigned long long mPacked;
    } u;
    static unsigned short int stDefaultDecimals;
};

} // namespace gong

#endif // _GONG_MONEY_H
