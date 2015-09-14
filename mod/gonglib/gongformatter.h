#ifndef _GONG_FORMATTER_H
#define _GONG_FORMATTER_H

/** @file gongformatter.h Localized Xtring and number formatting
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
#include "gongmoney.h"
#include "gongregconfig.h"
#include "gongvariant.h"

namespace gong {

class Formatter
{
public:
    Formatter(const RegConfig &regcfg, char blank_char = ' ');

    Xtring format(const Xtring &text, const char *format, const char *mask) const;
    Xtring format(int val, const char *aformat, const char *amask) const {
        return this->format( static_cast<long long>(val), aformat, amask);
    }
    Xtring format(bool val, const char *aformat, const char *amask ) const;
    Xtring format(long long val, const char *aformat, const char *amask) const;
    Xtring format(double val, const char *aformat, const char *amask) const;
    Xtring format(const Money &val, const char *aformat, const char *amask) const;
    Xtring format(const Date &adate, const char *format) const;
    Xtring format(const Time &atime, const char *format) const;
    Xtring format(const DateTime &adatetime, const char *format) const;
    Xtring format(const Variant &avalue, const char *format,
                  const char *mask, Variant::Type valuetype = Variant::tInvalid) const;

    int unMaskInt(const Xtring &text, const char *mask) const;
    int unFormatInt(const Xtring &text, const char *format) const;
    double unMaskDouble(const Xtring &text, const char *mask) const;
    double unFormatDouble(const Xtring &text, const char *format) const;
    Money unMaskMoney(const Xtring &text, const char *mask) const;
    Money unFormatMoney(const Xtring &text, const char *format) const;
    DateTime unMaskDateTime(const Xtring &text, const char *mask, Variant::Type valuetype = Variant::tDateTime ) const;
    DateTime unFormatDateTime(const Xtring &text, const char *format, Variant::Type valuetype = Variant::tDateTime ) const;

    Xtring mask(const char *text, const char *mask, Variant::Type valuetype=Variant::tString ) const;
    Xtring mask(const Xtring &text, const char *amask, Variant::Type valuetype=Variant::tString ) const
    {
        return this->mask(text.c_str(), amask, valuetype);
    }
    Xtring mask(long long val, const char *mask, Variant::Type valuetype=Variant::tInt ) const;
    Xtring mask(int val, const char *_mask, Variant::Type valuetype=Variant::tInt ) const
    {
        return mask(static_cast<long long>(val), _mask, valuetype);
    }
    Xtring mask(double valValue, const char *mask, Variant::Type valuetype=Variant::tDouble ) const;
    Xtring mask(Money monValue, const char *mask, Variant::Type valuetype=Variant::tMoney ) const;
    Xtring mask(const Variant &avalue, const char *amask, Variant::Type valuetype=Variant::tInvalid) const;
    Xtring mask(const DateTime &adatetime, const char *mask, Variant::Type valuetype = Variant::tDateTime) const;
    Xtring mask(const Date &adate, const char *amask, Variant::Type valuetype = Variant::tDateTime) const
    {
        return mask(DateTime(adate), amask, valuetype);
    }
    Xtring mask(const Time &atime, const char *amask, Variant::Type valuetype = Variant::tDateTime) const
    {
        return mask(DateTime(atime), amask, valuetype);
    }


// formatea un texto y mantiene la posicion del cursor
    Xtring mask_inedit(const char *origtext, Variant::Type valuetype, const char *amask,
                       int start_pos, int *position) const;
    const RegConfig &getRegConfig() const {
        return mRegConfig;
    }
    void setBlankChar(char blank_char);
    char getBlankChar() const;

private:
    const RegConfig &mRegConfig;
    char mBlankChar;

    Xtring formatLocaleCurrency(Xtring text, double value) const;
    char maskalfa_nth(Variant::Type valuetype, const char *pmask, int pos) const;
    int maskalfa_len(const char *pmask) const;
    bool maskalfa_iseditable(Variant::Type valuetype, const char *pmask, int pos) const;
    int maskalfa_firsteditable(Variant::Type valuetype, const char *pmask) const;
    int maskalfa_previouseditable(Variant::Type valuetype, const char *pmask, int pos) const;
    int maskalfa_nexteditable(Variant::Type valuetype, const char *pmask, int pos) const;
    int maskalfa_lasteditable(Variant::Type valuetype, const char *pmask) const;
    char maskalfa_defaultchar(Variant::Type valuetype, const char *pmask, int pos) const;
    int maskalfa_match(Variant::Type valuetype, char text, const char *pmask, int pos) const;
    Xtring maskalfa_mask(const char *text, Variant::Type valuetype, const char *mask,
                         int startpos=-1, int *position=NULL) const;
    Xtring formatDate(const char *p_text, Variant::Type valuetype,
                      const char *p_format, const char *p_mask) const;
    int masknum_match(Variant::Type valuetype, char text, char mask) const;
    static short int masknum_ndecimals(const char *mask);
    char masknum_defaultchar(Variant::Type valuetype, char mask) const;
    char *masknum_dupmask(char *start, char *end, int maxlen) const;
    Xtring masknum_mask(const char *text, Variant::Type valuetype, const char *mask,
                        int startpos=-1, int *position=NULL) const;
};

} // namespace

#endif // GONGFORMATTER_H

