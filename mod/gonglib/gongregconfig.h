#ifndef _GONG_REGCONFIG_H
#define _GONG_REGCONFIG_H

/** @file gongregconfig.h Regional configuration
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

#define UNICODE_EURO_SYMBOL "\u20A0"

class RegConfig
{
public:
    bool setLanguage(const char *language="");
    const Xtring &getLanguage() const {
        return mLanguage;
    }
    bool setLocale(const char *locale="");
    bool setLocaleCurrencyFormat(const char *locale="");
    bool setLocaleNumberFormat(const char *locale="");
    bool setLocaleDateFormats(const char *locale="");
    void setCurrencyFormat(const Xtring& currencysymbol,
                           int currencyfracdigits,
                           char currencydecimalpoint,
                           char currencythousandssep = '.',
                           int p_cs_precedes = 0,
                           int p_sep_by_space = 1,
                           int p_sign_posn = 1,
                           int n_cs_precedes = 0,
                           int n_sep_by_space = 1,
                           int n_sign_posn = 1,
                           const char *grouping="\x03");

    void setNumberFormat( int frac_digits,
                          char decimalpoint,
                          char thousandssep = '.',
                          const Xtring& positivesign = Xtring::null,
                          const Xtring& negativesign = "-",
                          const char *grouping="\x03");

    const Xtring &getCurrencyMask() const {
        return mCurrencyMask;
    }
    const Xtring &getNumberMask() const {
        return mNumberMask;
    }

public:
    RegConfig(const char *language);
    RegConfig(char decimalpoint = ',', char thousandssep = '.',
              const Xtring& positivesign = Xtring::null,
              const Xtring& negativesign = "-", const Xtring& currencysymbol = Xtring::null,
              int currencyfracdigits = 2, char currencydecimalpoint = ',',
              char currencythousandssep = '.', const Xtring& datemask = Xtring::null,
              const Xtring& timemask = Xtring::null, const Xtring& datetimemask = Xtring::null,
              int psignposn = 1, int pcsprecedes = 0, int psepbyspace = 0,
              int nsignposn = 1, int ncsprecedes = 0, int nsepbyspace = 0, const Xtring& grouping = "\x03")
        : mDecimalPoint(decimalpoint), mThousandsSep(thousandssep),
          mCurrencyDecimalPoint(currencydecimalpoint),
          mCurrencyThousandsSep(currencythousandssep),
          mPositiveSign(positivesign), mNegativeSign(negativesign),
          mCurrencySymbol(currencysymbol),
          mCurrencyFracDigits(currencyfracdigits),
          mDateMask(datemask), mTimeMask(timemask), mDateTimeMask(datetimemask),
          mPSignPosn(psignposn), mPCSPrecedes(pcsprecedes), mPSepBySpace(psepbyspace),
          mNSignPosn(nsignposn), mNCSPrecedes(ncsprecedes), mNSepBySpace(nsepbyspace),
          mGrouping(grouping), mDateFormat(Xtring::null), mTimeFormat(Xtring::null),
          mDateTimeFormat(Xtring::null), mCurrencyMask(Xtring::null),
          mNumberMask(Xtring::null), mLanguage(Xtring::null)
    {};
    char getDecimalPoint() const {
        return mDecimalPoint;
    };
    void setDecimalPoint(char decimalpoint) {
        mDecimalPoint = decimalpoint;
    };
    char getThousandsSep() const {
        return mThousandsSep;
    };
    void setThousandsSep(char thousandssep) {
        mThousandsSep = thousandssep;
    };
    const Xtring& getPositiveSign() const {
        return mPositiveSign;
    };
    void setPositiveSign(const Xtring& positivesign) {
        mPositiveSign = positivesign;
    };
    const Xtring& getNegativeSign() const {
        return mNegativeSign;
    };
    void setNegativeSign(const Xtring& negativesign) {
        mNegativeSign = negativesign;
    };
    const Xtring& getCurrencySymbol() const {
        return mCurrencySymbol;
    };
    void setCurrencySymbol(const Xtring& currencysymbol) {
        mCurrencySymbol = currencysymbol;
    };
    int getCurrencyFracDigits() const {
        return mCurrencyFracDigits;
    };
    void setCurrencyFracDigits(int currencyfracdigits) {
        mCurrencyFracDigits = currencyfracdigits;
    };
    char getCurrencyDecimalPoint() const {
        return mCurrencyDecimalPoint;
    };
    void setCurrencyDecimalPoint(char currencydecimalpoint) {
        mCurrencyDecimalPoint = currencydecimalpoint;
    };
    char getCurrencyThousandsSep() const {
        return mCurrencyThousandsSep;
    };
    void setCurrencyThousandsSep(char currencythousandssep) {
        mCurrencyThousandsSep = currencythousandssep;
    };

    const Xtring& getDateMask() const {
        return mDateMask;
    };
    void setDateMask(const char *datemask);
    const Xtring& getTimeMask() const {
        return mTimeMask;
    };
    void setTimeMask(const char *timemask);
    const Xtring& getDateTimeMask() const {
        return mDateTimeMask;
    };
    void setDateTimeMask(const char *datetimemask);
    const Xtring& getDateFormat() const {
        return mDateFormat;
    };
    void setDateFormat(const char *dateformat) {
        mDateFormat = dateformat;
    };
    const Xtring& getTimeFormat() const {
        return mTimeFormat;
    };
    void setTimeFormat(const char *timeformat) {
        mTimeFormat = timeformat;
    };
    const Xtring& getDateTimeFormat() const {
        return mDateTimeFormat;
    };
    void setDateTimeFormat(const char *datetimeformat) {
        mDateTimeFormat = datetimeformat;
    };

    int getPSignPosn() const {
        return mPSignPosn;
    };
    void setPSignPosn(int psignposn) {
        mPSignPosn = psignposn;
    };
    int getPCSPrecedes() const {
        return mPCSPrecedes;
    };
    void setPCSPrecedes(int pcsprecedes) {
        mPCSPrecedes = pcsprecedes;
    };
    int getPSepBySpace() const {
        return mPSepBySpace;
    };
    void setPSepBySpace(int psepbyspace) {
        mPSepBySpace = psepbyspace;
    };
    int getNSignPosn() const {
        return mNSignPosn;
    };
    void setNSignPosn(int nsignposn) {
        mNSignPosn = nsignposn;
    };
    int getNCSPrecedes() const {
        return mNCSPrecedes;
    };
    void setNCSPrecedes(int ncsprecedes) {
        mNCSPrecedes = ncsprecedes;
    };
    int getNSepBySpace() const {
        return mNSepBySpace;
    };
    void setNSepBySpace(int nsepbyspace) {
        mNSepBySpace = nsepbyspace;
    };
    const Xtring& getGrouping() const {
        return mGrouping;
    };
    void setGrouping(const Xtring& grouping) {
        mGrouping = grouping;
    };
	Xtring getCurrencyName() const;

private:
    char mDecimalPoint;
    char mThousandsSep;
    char mCurrencyDecimalPoint;
    char mCurrencyThousandsSep;
    Xtring mPositiveSign;
    Xtring mNegativeSign;
    Xtring mCurrencySymbol;
    int mCurrencyFracDigits;
    Xtring mDateMask;
    Xtring mTimeMask;
    Xtring mDateTimeMask;
    int mPSignPosn;
    int mPCSPrecedes;
    int mPSepBySpace;
    int mNSignPosn;
    int mNCSPrecedes;
    int mNSepBySpace;
    Xtring mGrouping;
    Xtring mDateFormat;
    Xtring mTimeFormat;
    Xtring mDateTimeFormat;
    Xtring mCurrencyMask, mNumberMask;
    Xtring mLanguage;
    Xtring buildNumberMask(const char *grouping, int digits);
    Xtring buildDateMask(int wich_format, const char *nl_format);

};

} // namespace gong

#endif // _GONG_REGCONFIG_H
