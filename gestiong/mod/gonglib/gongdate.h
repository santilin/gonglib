#ifndef _GONG_DATE_H
#define _GONG_DATE_H

/** @file gongdate.h Date and time classes
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include <stdint.h>
#include "gongxtring.h"

namespace gong {

class Date
{
public:
    typedef unsigned char dateitem;
    // constructors
    Date(): mJulianDay(NullDate) {}
    Date(const Date& other) : mJulianDay(other.mJulianDay) {}
    Date(long julianday): mJulianDay(julianday) {}
    Date(int year, dateitem month, dateitem day)
    {
        setDMY(day, month, year);
    }
    Date(const Xtring &datestr, const char *format = 0);
    Date &setCurrentDate();
    static Date currentDate();
    Xtring toString(const char *format = 0) const;
    Date firstDayOfYear(int year = 0) const;
    Date lastDayOfYear(int year = 0) const;
    Date firstDayOfMonth(dateitem month = 0) const;
    // return number of days in month.
    dateitem daysInMonth() const;
    Date lastDayOfMonth(dateitem month = 0) const;
    Date firstDayOfWeek(dateitem week = 0) const;
    Date lastDayOfWeek(dateitem week = 0) const;

    static bool checkForValidDate(dateitem day, dateitem month, int year);
    static long DMYtoJulian(dateitem Day, dateitem Month, int Year);
    static void JulianToDMY(unsigned long Jul, dateitem *Day, dateitem *Month, int *Year);
    static dateitem daysInMonth(const dateitem Month, const int year);
    static bool isLeap(int year);
    static Xtring guessFormat(const Xtring &datestr);

    bool isValid() const {
        return (mJulianDay != BadDate);
    }
    bool isNull() const {
        return (mJulianDay == NullDate);
    }
    void getDMY(dateitem *day, dateitem *month, int *year) const
    {
        JulianToDMY(mJulianDay, day, month, year);
    }
    dateitem getDay() const {
        dateitem day, month;
        int year;
        getDMY(&day, &month, &year);
        return day;
    }
    dateitem getMonth() const {
        dateitem day, month;
        int year;
        getDMY(&day, &month, &year);
        return month;
    } ;
    int getYear() const {
        dateitem day, month;
        int year;
        getDMY(&day, &month, &year);
        return year;
    } ;
    int getQuarter() const;
    // returns Day of week (sunday=0 ... saturday=6)
    int getDayOfWeek() const;
    long getJulianDay() const {
        return mJulianDay;
    }
    // if new date invalid returns false and makes no changes, if valid returns true
    bool setDMY(dateitem day, dateitem month, int year)
    {
        return ((mJulianDay = DMYtoJulian(day, month, year)) != BadDate );
    }

    // check for leap year 1 if it is, 0 if not
    bool isLeap() const;


    // logical operators
    friend bool operator == (const Date &d1, const Date &d2)
    {
        return (d1.mJulianDay == d2.mJulianDay);
    }
    friend bool operator != (const Date &d1, const Date &d2)
    {
        return (d1.mJulianDay != d2.mJulianDay);
    }
    friend bool operator > (const Date &d1, const Date &d2)
    {
        return (d1.mJulianDay > d2.mJulianDay);
    }
    friend bool operator < (const Date &d1, const Date &d2)
    {
        return (d1.mJulianDay < d2.mJulianDay);
    }
    friend bool operator >= (const Date &d1, const Date &d2)
    {
        return (d1.mJulianDay >= d2.mJulianDay);
    }
    friend bool operator <= (const Date &d1, const Date &d2)
    {
        return (d1.mJulianDay <= d2.mJulianDay);
    }

    // Arithmetic operators
    friend const Date operator + (const Date &d, long days)
    {
        return Date( d.mJulianDay + days );
    }
    friend const Date operator + (long days, const Date &d)
    {
        return Date( d.mJulianDay + days );
    }
    friend int operator - (const Date &d1, const Date &d2)
    {
        return d1.mJulianDay - d2.mJulianDay;
    }
    friend const Date operator - (const Date &d, long days)
    {
        return Date( d.mJulianDay - days );
    }
    friend const Date operator - (long days, const Date &d)
    {
        return Date( d.mJulianDay - days );
    }

    static const uint32_t BadDate = static_cast<uint32_t>(-1L);	/* returned when date can't be determined */
    static const uint32_t NullDate = 0L;
protected:
    uint32_t mJulianDay;
    static const dateitem MinMonth =	1;		/* January */
    static const dateitem MaxMonth =	12;		/* December */
    static const dateitem MinDay =		1;		/* minimum day of a month */
    static const dateitem MaxDay =		31;		/* maximum days in month */
    static const dateitem MinWeekday =	0;		/* Sunday */
    static const dateitem MaxWeekday =	6;		/* Saturday */
    static const int32_t MinYear =	-4700L;	/* earliest year handled is 4700 BC */
    static const int32_t MaxYear =	25000L;	/* Latest year handled is 25000 AD */
    static const uint32_t MinDate =	4749L;		/* 1 January 4700 BC */
    static const uint32_t MaxDate =	10852487L;	/* 31 December 25,000 */
};


class Time
{
public:
    typedef unsigned char timeitem;
    static const uint32_t BadTime = static_cast<uint32_t>(-1L);	/* returned when a time can't be determined */
    static const uint32_t NullTime = 0L;

    Time() : mTimeSecs(0) {}
    Time(const Time& other) : mTimeSecs(other.mTimeSecs) {}
    Time(int totalsecs) : mTimeSecs(totalsecs) {}
    Time(timeitem hours, timeitem mins, timeitem secs = 0)
    {
        setTime(hours, mins, secs);
    }
    Time(const Xtring &datestr, const char *format = 0);
    Time &setCurrentTime();
    static Time currentTime();
    Xtring toString(const char *format = 0) const;

    static bool checkForValidTime(timeitem hours, timeitem mins, timeitem secs);
    bool isValid() const {
        return mTimeSecs < SecondsPerDay;
    }
    timeitem getHours() const {
        timeitem h, m, s;
        getHMS(&h,&m,&s);
        return h;
    }
    timeitem getMinutes() const {
        timeitem h, m, s;
        getHMS(&h,&m,&s);
        return m;
    }
    timeitem getSeconds() const {
        timeitem h, m, s;
        getHMS(&h,&m,&s);
        return s;
    }
    int getTotalSeconds() const {
        return mTimeSecs;
    }

    // if new time invalid returns false and makes no changes, if valid returns true
    bool setTime(timeitem hours, timeitem mins, timeitem secs);
    void getHMS(timeitem *hour, timeitem *min, timeitem *secs) const;


    // logical operators
    friend bool operator == (const Time &d1, const Time &d2)
    {
        return (d1.mTimeSecs == d2.mTimeSecs);
    }
    friend bool operator != (const Time &d1, const Time &d2)
    {
        return (d1.mTimeSecs != d2.mTimeSecs);
    }
    friend bool operator > (const Time &d1, const Time &d2)
    {
        return (d1.mTimeSecs > d2.mTimeSecs);
    }
    friend bool operator < (const Time &d1, const Time &d2)
    {
        return (d1.mTimeSecs < d2.mTimeSecs);
    }
    friend bool operator >= (const Time &d1, const Time &d2)
    {
        return (d1.mTimeSecs >= d2.mTimeSecs);
    }
    friend bool operator <= (const Time &d1, const Time &d2)
    {
        return (d1.mTimeSecs <= d2.mTimeSecs);
    }

    // Arithmetic operators
    friend const Time operator + (const Time &d1, const Time &d2)
    {
        return Time( d1.mTimeSecs + d2.mTimeSecs );
    }
    friend const Time operator + (const Time &d, long secs)
    {
        return Time( d.mTimeSecs + secs );
    }
    friend const Time operator + (long secs, const Time &d)
    {
        return Time( d.mTimeSecs + secs );
    }
    friend int operator - (const Time &d1, const Time &d2)
    {   if ((d1.mTimeSecs - d2.mTimeSecs) > SecondsPerDay ) return 0;
        else return (d1.mTimeSecs - d2.mTimeSecs);
    }
    friend const Time operator - (const Time &d, long secs)
    {   if ((d.mTimeSecs - secs) > SecondsPerDay ) return 0;
        else return (d.mTimeSecs - secs);
    }

protected:
    uint32_t mTimeSecs;
    static const uint SecondsPerDay = 24 * 60 * 60;
    static const long MaxTime = SecondsPerDay - 1; // Maximum seconds in one time
};


class DateTime: public Date, public Time
{
public:
    DateTime(): Date(), Time() {}
#if 0
    DateTime(unsigned long long seconds):
        Date( (seconds - (seconds % SecondsPerDay)) / SecondsPerDay ),
        Time(seconds % SecondsPerDay) {}
#endif
    DateTime(const Date &date): Date(date), Time() {}
    DateTime(const Time &time): Date(), Time(time) {}
    DateTime(const Date &date, const Time &time): Date(date), Time(time) {}
    DateTime(const DateTime& other): Date(other), Time(other) {}
    DateTime(int year, dateitem month, dateitem day, timeitem hours, timeitem mins, timeitem secs) {
        setDMY(day, month, year);
        setTime(hours, mins, secs);
    }
    DateTime(const Xtring &datestr, const char *format = 0);
    DateTime &setCurrentDateTime();
    static DateTime currentDateTime();

    Xtring toString(const char *format = 0) const;

    long long packed() const {
        return (*(reinterpret_cast<const long long *>(this)));
    }
    static DateTime unpack(long long packed);
    bool isValid() const {
        return Date::isValid() && Time::isValid();
    }
    unsigned long long getTotalSeconds() const {
        return mTimeSecs + mJulianDay * SecondsPerDay;
    }

    // logical operators
    friend bool operator == (const DateTime &d1, const DateTime &d2)
    {
        return (d1.mJulianDay == d2.mJulianDay) && d1.mTimeSecs == d2.mTimeSecs;
    }
    friend bool operator != (const DateTime &d1, const DateTime &d2)
    {
        return (d1.mJulianDay != d2.mJulianDay) || d1.mTimeSecs == d2.mTimeSecs;
    }
    friend bool operator > (const DateTime &d1, const DateTime &d2)
    {   return (d1.mJulianDay > d2.mJulianDay)
               || ( d1.mJulianDay == d2.mJulianDay && d1.mTimeSecs > d2.mTimeSecs );
    }
    friend bool operator < (const DateTime &d1, const DateTime &d2)
    {   return (d1.mJulianDay < d2.mJulianDay)
               || ( d1.mJulianDay == d2.mJulianDay && d1.mTimeSecs < d2.mTimeSecs );
    }
    friend bool operator >= (const DateTime &d1, const DateTime &d2)
    {   return (d1.mJulianDay > d2.mJulianDay)
               || ( d1.mJulianDay == d2.mJulianDay && d1.mTimeSecs >= d2.mTimeSecs );
    }
    friend bool operator <= (const DateTime &d1, const DateTime &d2)
    {   return (d1.mJulianDay < d2.mJulianDay)
               || ( d1.mJulianDay == d2.mJulianDay && d1.mTimeSecs <= d2.mTimeSecs );
    }

    friend int operator - (const DateTime &d1, const DateTime &d2)
    {
        return d1.getTotalSeconds() - d2.getTotalSeconds();
    }

};

} // namespace gong

#endif // _GONG_DATE_H
