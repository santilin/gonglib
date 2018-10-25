#include <cerrno>
#include <ctime>
#include <cmath>
#include <langinfo.h>
#include <iostream>

#include "gongdebug.h"
#include "gongdate.h"

#define MAX_FORMAT_BUF 200

namespace gong {

/**
       @class Date
       @brief Class for handling dates
*/
/**
       @class Time
       @brief Class for handling times
*/
/**
       @class DateTime
       @brief Class for handling datetimes
*/

static unsigned MonthDays[12] = {
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

Date::Date(const Xtring &datestr, const char *format)
{
    if( datestr.isEmpty() ) {
        mJulianDay = NullDate;
    } else {
        struct tm stm;
        memset(&stm, 0, sizeof(stm) );
        char *res;
        if( strempty(format) )
            format = nl_langinfo(D_FMT);
        Xtring fixedformat;
        Xtring guessedformat = guessFormat(datestr);
        if( !guessedformat.isEmpty() && guessedformat != format ) {
            if( guessedformat.startsWith("%Y") && !(Xtring(format).startsWith("%Y") ) ) {
                // if the date string started with a four-digit year, the guessed format was correct
                fixedformat = guessedformat;
                format = fixedformat.c_str();
            } else if( (guessedformat.find("%Y") != Xtring::npos) && strstr(format, "%y" ) ) {
                // if the date string contains a four-digit year, but the format does not, the conversion would be wrong
                fixedformat = Xtring(format).replace("%y", "%Y");
                format = fixedformat.c_str();
            }
        }
        res = strptime(datestr.c_str(), format, &stm);
        _GONG_DEBUG_PRINT(8, Xtring::printf("%d, %d, %d", stm.tm_mday, stm.tm_mon, stm.tm_year ) );
        if( res )
            setDMY(stm.tm_mday, stm.tm_mon + 1, stm.tm_year + 1900);
        else
            mJulianDay = BadDate;
    }
}

Xtring Date::toString(const char *format) const
{
    if ( !isValid() || *this == NullDate)
        return Xtring();
    else {
        char buffer[ MAX_FORMAT_BUF+1 ];
        struct tm stm;
        memset(&stm, 0, sizeof(stm) );
        Date::dateitem d, mon;
        int y;
        getDMY(&d, &mon, &y);
        stm.tm_mday = d;
        stm.tm_mon = mon;
        stm.tm_year = y;
        stm.tm_mon --;
        stm.tm_year -= 1900;
        mktime( &stm ); // fix day of week, daylight saving, etc.
        strftime( buffer, MAX_FORMAT_BUF, (format && *format)?format:"%x", &stm );
        return Xtring( buffer );
    }
}

Date &Date::setCurrentDate()
{
    time_t systim;
    struct tm *timelocal;

    time(&systim);
    timelocal = localtime(&systim);

    int day = timelocal->tm_mday;
    int month = timelocal->tm_mon + 1; // tm mon is zero based.
    int year = timelocal->tm_year + 1900;
    mJulianDay = DMYtoJulian(day, month, year);
    return *this;
}

Date Date::currentDate()
{
    Date d;
    d.setCurrentDate();
    return d;
}

Xtring Date::guessFormat(const Xtring &datestr)
{
    long d=-1, m=-1, y=-1;
    const char *ds = datestr.c_str();
    char *endp;
    char sep;
    // get three numbers
    errno = 0;
    do {
        d = strtol(ds, &endp, 10);
        ds = ++endp;
    } while( errno != 0 && !strempty(endp) );
    sep = *(endp-1);
    if( !strempty(endp) ) {
        do {
            m = strtol(ds, &endp, 10);
            ds = ++endp;
        } while( errno != 0 && !strempty(endp) );
    }
    if( !strempty(endp) ) {
        do {
            y = strtol(ds, &endp, 10);
            ds = ++endp;
        } while( errno != 0 && !strempty(endp) );
    }
    // see what we got
    if( d > 31 ) return Xtring("%Y") + sep + Xtring("%m") + sep + Xtring("%d");
    if( y > 31 ) {
        if( m > 12 )
            return Xtring("%m") + sep + Xtring("%d") + sep + Xtring("%Y");
        else if( d > 12 )
            return Xtring("%d") + sep + Xtring("%m") + sep + Xtring("%Y");
        else
            return "%x";
    }
    // No item was > 31, so we are in trouble. Assume year at end. Try to guess the month
    if( m > 12 )
        return Xtring("%m") + sep + Xtring("%d") + sep + Xtring("%y");
    else if( d > 12 )
        return Xtring("%d") + sep + Xtring("%m") + sep + Xtring("%y"); // European
    else return "%x";
}



bool Date::isLeap(int year)
{
    return (year % 4 == 0) && (year % 4000 != 0) &&
           ((year % 100 != 0) || (year % 400 == 0));
}


/*
**  DaysInMonth -- return the number of days in month.  year
**  is passed to test for a leap year if the month is February.
*/
Date::dateitem Date::daysInMonth(const Date::dateitem month, const int year)
{
    if ((month == 2) && isLeap(year))
        return (29);
    else
        return (MonthDays[month - 1]);
}

Date::dateitem Date::daysInMonth() const
{
    Date::dateitem day, month;
    int year;
    JulianToDMY(mJulianDay, &day, &month, &year);
    return daysInMonth(month,year);
}

Date Date::lastDayOfMonth(Date::dateitem month) const
{
    return Date( getYear(), month ? month : getMonth(),
                 daysInMonth( month ? month : getMonth(), getYear()) );
}

Date Date::firstDayOfMonth(Date::dateitem month) const
{
    return Date( getYear(), month ? month : getMonth(), 1 );
}

Date Date::firstDayOfYear(int year) const
{
    return Date( year ? year : getYear(), getMonth(), 1 );
}

Date Date::lastDayOfYear(int year) const
{
    if( year == 0 )
        year = getYear();
    return Date( year, 12, 31 );
}

/*
**  CheckForValidDate -- check that day, month and year
**  represent a valid date.  Return non-zero if so, 0 otherwise.
*/

bool Date::checkForValidDate(Date::dateitem day, Date::dateitem month, int year)
{
    if ((year < MinYear) || (year > MaxYear) || (year == 0) ||
            (month < MinMonth) || (month > MaxMonth))
        return (0);
    return (day <= daysInMonth(month, year));
}

/*
**  DMYtoJulian -- after validating the date passed as a day,
**  month and year, convert it to a Julian date.
**
**  The algorithm shown here is swiped directly from "Numerical
**  Recipes in C" by Press, Flannery, Teukolsky and Vettering, p. 10.
*/

#define	IGREG	(15+31L*(10+12L*1582))

long Date::DMYtoJulian(Date::dateitem day, Date::dateitem month, int year)
{
    unsigned long Ja, Jm, Jy, Jul;

    if( day == 0 && month == 0 )
        return NullDate;
    if (!checkForValidDate(day, month, year))
        return BadDate;
    if (year < 0)
        year++;
    if (month > 2)
    {
        Jy = year;
        Jm = month + 1;
    }
    else
    {
        Jy = year - 1;
        Jm = month + 13;
    }
    Jul = static_cast<long>(floor(365.25*Jy) + floor(30.6001*Jm) + day + 1720995);
    if (day + 31L*(month + 12L*year) >= IGREG)
    {
        Ja = static_cast<long>(0.01*Jy);
        Jul += 2 - Ja + static_cast<int>(0.25*Ja);
    }
    return Jul;
}


/*  JulianToDMY -- convert a Julian date to the appropriate
**  day, month and year.
**
**  Also swiped from "Numerical Recipes".
*/

#define	GREGOR	2299161

void Date::JulianToDMY(unsigned long Jul, Date::dateitem *day, Date::dateitem *month, int *year)
{
    long Ja, JAlpha, Jb, Jc, Jd, Je;

    if ((Jul != BadDate) && (Jul != NullDate ) && (Jul >= MinDate) && (Jul <= MaxDate))
    {
        if (Jul >= GREGOR)
        {
            JAlpha = static_cast<long>((static_cast<double>(Jul - 1867216) - 0.25)/36524.25);
            Ja = Jul + 1 + JAlpha - static_cast<long> (0.25*JAlpha);
        }
        else
            Ja = Jul;
        Jb = Ja + 1524;
        Jc = static_cast<long>(6680.0 + (static_cast<double>(Jb - 2439870) - 122.1)/365.25);
        Jd = static_cast<long>(365*Jc + (0.25*Jc));
        Je = static_cast<long>((Jb - Jd)/30.6001);
        *day = Jb - Jd - static_cast<int>(30.6001*Je);
        *month = Je - 1;
        if (*month > 12)
            *month -= 12;
        *year = Jc - 4715;
        if (*month > 2)
            --(*year);
        if (*year <= 0)
            --(*year);
    } else {
        *day = 0;
        *month = 0;
        *year = 0;
    }

}


int Date::getQuarter() const
{
    int month = getMonth() - 1;
    return int(month / 3) + 1;
}


/*
**  Time  -- constructor if no args given.  Just initialize
**  to todays time.
*/
Time &Time::setCurrentTime()
{
    time_t systim;
    struct tm *timelocal;

    time(&systim);
    timelocal = localtime(&systim);

    int hour = timelocal->tm_hour;
    int min = timelocal->tm_min;
    int sec = timelocal->tm_sec;
    mTimeSecs = hour * 3600 + min * 60 + sec;
    return *this;
}

Time Time::currentTime()
{
    Time t;
    t.setCurrentTime();
    return t;
}


Time::Time(const Xtring &timestr, const char *format)
{
    struct tm stm;
    memset(&stm, 0, sizeof(stm) );
    char *res = strptime(timestr.c_str(), (format && *format)?format:"%X", &stm);
    if( !res )
        res = strptime(timestr.c_str(), "%H:%M", &stm);
    if( res )
        setTime(stm.tm_hour, stm.tm_min, stm.tm_sec);
    else
        mTimeSecs = static_cast<uint>(-1);
}


Xtring Time::toString(const char *format) const
{
    if ( !isValid() )
        return Xtring();
    else {
        char buffer[ MAX_FORMAT_BUF+1 ];
        struct tm stm;
        memset(&stm, 0, sizeof(stm) );
        Time::timeitem h, m, s;
        getHMS(&h, &m, &s);
        stm.tm_hour = h;
        stm.tm_min = m;
        stm.tm_sec = s;
        mktime( &stm ); // fix day of week, daylight saving, etc.
        strftime( buffer, MAX_FORMAT_BUF, (format && *format)?format:"%X", &stm );
        return Xtring( buffer );
    }
}


/*
**  CheckForValidTime -- check that day, month and year
**  represent a valid date.  Return non-zero if so, 0 otherwise.
*/

bool Time::checkForValidTime(Time::timeitem hours, Time::timeitem mins, Time::timeitem secs)
{
    return (static_cast<long>(secs + (mins *60) + (hours *3600)) < MaxTime);
}



bool Time::setTime(Time::timeitem hours, Time::timeitem mins, Time::timeitem secs)
{
    long totalsecs = static_cast<long>(secs + (mins *60) + (hours *3600));
    if( totalsecs <= MaxTime ) {
        mTimeSecs = totalsecs;
        return true;
    } else {
        _GONG_DEBUG_WARNING( Xtring::printf("Bad time (%d, %d, %d)", hours, mins, secs ) );
        return false;
    }
}


void Time::getHMS(Time::timeitem *hours, Time::timeitem *mins, Time::timeitem *secs) const
{
    *secs = mTimeSecs % 60;
    *mins = ((mTimeSecs - (*secs)) / 60 ) % 60;
    *hours = (mTimeSecs - (*secs) - (*mins * 60)) / 3600;
}


DateTime::DateTime(const Xtring &datetimestr, const char *format)
{
    if( datetimestr.isEmpty() ) {
        mJulianDay = BadDate;
        mTimeSecs = BadTime;
        return;
    }
    struct tm stm;
    memset(&stm, 0, sizeof(stm) );
    Xtring wholeformat;
    if( strempty( format ) ) {
        wholeformat = guessFormat( datetimestr );
        if( wholeformat.empty() || wholeformat == "%x" ) {
            wholeformat = "%x";
        }
        wholeformat += Xtring(" ") + nl_langinfo( T_FMT );
        format = wholeformat.c_str();
    }
    // if the date string contains a four-digit year, but the format does not, the conversion would be wrong
    Xtring fixedformat;
    if( (wholeformat.find("%Y") != Xtring::npos) && strstr(format, "%y" ) ) {
        fixedformat = Xtring(format).replace("%y", "%Y");
        format = fixedformat.c_str();
    }
    char *res = strptime(datetimestr.c_str(), format, &stm);
    if( res ) {
        setTime(stm.tm_hour, stm.tm_min, stm.tm_sec);
        setDMY(stm.tm_mday, stm.tm_mon + 1, stm.tm_year + 1900);
    } else {
        mJulianDay = BadDate;
        this->mTimeSecs = BadTime;
    }
}


Xtring DateTime::toString(const char *format) const
{
    if ( !isValid() )
        return Xtring();
    else {
        char buffer[ MAX_FORMAT_BUF+1 ];
        struct tm stm;
        memset(&stm, 0, sizeof(stm) );
        Time::timeitem h, m, s;
        Date::dateitem d, mon;
        int y;
        getHMS(&h, &m, &s);
        stm.tm_hour = h;
        stm.tm_min = m;
        stm.tm_sec = s;
        getDMY(&d, &mon, &y);
        stm.tm_mday = d;
        stm.tm_mon = mon;
        stm.tm_year = y;
        stm.tm_mon --;
        stm.tm_year -= 1900;
//		mktime( &stm ); // fix day of week, daylight saving, etc.
//		stm.tm_isdst = 0;
//		stm.tm_gmtoff = 0;
        strftime( buffer, MAX_FORMAT_BUF, (format && *format)?format:"%x %X", &stm );
        return Xtring( buffer );
    }
}


DateTime &DateTime::setCurrentDateTime()
{
    setCurrentDate();
    setCurrentTime();
    return *this;
}

DateTime DateTime::currentDateTime()
{
    DateTime dt;
    dt.setCurrentDate();
    dt.setCurrentTime();
    return dt;
}

DateTime DateTime::unpack(long long packed)
{
    return *reinterpret_cast<DateTime *>(&packed);
}

} // namespace gong



