#include <cstdio>
#include <cstdlib>
#include <ctype.h>
#include <clocale>

#include "gongdebug.h"
#include "gonggettext.h"
#include "gongformatter.h"

namespace gong {

/**
       @class Formatter
       @brief Localized Xtring and number formatting

       This module handles conversions of numbers, dates, currencies, etc based on locale information.
       The locale setting is not done automatically by glibc, so a call to setlocale must be done by the application. If not, localeconv will return an empty struct, with the only setting of decimal_point to ','. Not even the negative sign will be set
       @todo {bug9} multibytes in LC_****
*/

#define MAX_FORMAT_BUF 256
#define LC_DECIMAL_POINT ( (valuetype==Variant::tMoney)?mRegConfig.getCurrencyDecimalPoint():mRegConfig.getDecimalPoint() )
#define LC_THOUSANDS_SEP ( (valuetype==Variant::tMoney)?mRegConfig.getCurrencyThousandsSep():mRegConfig.getThousandsSep() )
#define LC_NEGATIVE_SIGN ( *mRegConfig.getNegativeSign().c_str() )
#define LC_POSITIVE_SIGN ( *mRegConfig.getPositiveSign().c_str() )

#define LITERAL_MASK_CHAR '\\'


// Default constructor
Formatter::Formatter( const RegConfig &regcfg, char blank_char )
    : mRegConfig(regcfg), mBlankChar( blank_char )
{}

// set blank char
void Formatter::setBlankChar( char blank_char )
{
    mBlankChar = blank_char;
}

// get blank char
char Formatter::getBlankChar() const
{
    return mBlankChar;
}

/* Formats a text
%v -> El valor tal cual
%$ -> El valor como moneda, con su símbolo correspondiente
%! -> El valor sin el formato habitual
%< -> A minúsculas
%> -> A mayúsculas
%p -> A proper
%% -> %
%b -> Blank if zero
*/

Xtring Formatter::format( const Xtring &text, const char *aformat, const char *amask ) const
{
    if( !aformat || !*aformat )
        return text;
    if( !strcmp(aformat, "%b" ) )
        aformat = "%b%v";
    Xtring formatted;
    bool low = false, up = false, blankifzero = false;
    for( const char *pformat = aformat; *pformat; ++pformat ) {
        switch( *pformat ) {
        case '%':
            ++pformat;
            if( *pformat ) {
                switch( *pformat ) {
                case '<':
                    low = true;
                    up = false;
					formatted += text.upper();
                    break;
                case '>':
                    up = true;
                    low = false;
					formatted += text.lower();
                    break;
                case 'p':
                    low = false;
                    up = false;
                    break;
                case 'b':
                    blankifzero = true;
                    break;
                case 'v':
                case '$':
                case '!':
                    if( up )
                        formatted += text.upper();
                    else if( low )
                        formatted += text.lower();
                    else
                        formatted += text;
                    break;
                case '%':
                    formatted += *pformat;
                    break;
                }
            } else {
                formatted += *pformat;
            }
            break;
        default:
            formatted += *pformat;
        }
    }
    if( blankifzero ) {
        for( const char *pformatted = formatted.c_str(); *pformatted; ++pformatted ) {
            if( *pformatted >= '1' && *pformatted <= '9' )
                return formatted;
        }
        return "";
    }
    return formatted;
}

Xtring Formatter::format( bool val, const char *aformat, const char *amask ) const
{
    if( !strempty(aformat) ) {
        const char *pbarra = strchr(aformat, '|' );
        if( pbarra ) {
            // Format is falsestr|truestr
            if( val ) { // true
                return strchr(aformat, '|') + 1;
            } else {
                Xtring strfalse = aformat;
                return strfalse.mid(0, strfalse.find('|'));
            }
        }
    }
    return ( val ? _("true") : _("false") );
}


Xtring Formatter::format( long long val, const char *aformat, const char *amask ) const
{
    if( aformat && strstr(aformat, "%b") && val == 0 )
        return "";
    Xtring masked_value = mask( val, amask );
    return format( masked_value, aformat, amask );
}

Xtring Formatter::format( double val, const char *aformat, const char *amask ) const
{
    if( aformat && strstr(aformat, "%b") && val == 0.0 )
        return "";
    Xtring masked_value;
    if ( aformat && strstr(aformat, "%$" ) != 0 ) {
        short int ndecimals = masknum_ndecimals( (amask && *amask)?amask:mRegConfig.getCurrencyMask().c_str() );
        if( ndecimals == -1 )
            ndecimals = 2;
        Money m(val, ndecimals);
        return format( m, aformat, amask );
    } else {
        if( amask && strlen(amask) ) {
            short int ndecimals = masknum_ndecimals( amask );
            if( ndecimals != -1 )
                val = round( val, ndecimals );
        }
        masked_value = mask( val, amask );
    }
    return format( masked_value, aformat, amask );
}

Xtring Formatter::format( const Money &val, const char *aformat, const char *amask ) const
{
    if( aformat && strstr(aformat, "%b") && val.toDouble() == 0.0 ) {
        return "";
    }
    Xtring masked_value;
    Money local_val = val;
    if( aformat && strstr(aformat, "%!" ) != 0 ) {
        short int ndecimals = masknum_ndecimals( (amask && *amask)?amask:mRegConfig.getNumberMask().c_str() );
        if( ndecimals != -1 )
            local_val = Money(round( val.toDouble(), ndecimals ), val.getDecimals() );
        masked_value = mask( local_val, (amask && *amask)?amask:mRegConfig.getNumberMask().c_str() );
    } else {
        short int ndecimals = masknum_ndecimals( (amask && *amask)?amask:mRegConfig.getCurrencyMask().c_str() );
        if( ndecimals != -1 )
            local_val = Money(round( val.toDouble(), ndecimals ), val.getDecimals() );
        masked_value = formatLocaleCurrency(
                           mask( local_val, (amask && *amask)?amask:mRegConfig.getCurrencyMask().c_str(), Variant::tMoney), local_val.toDouble() );
    }
//     _GONG_DEBUG_PRINT(0, Xtring::printf("Will format '%s' with format '%s' and mask '%s' and return '%s'", masked_value.c_str(), aformat, amask, format(masked_value, aformat, amask).c_str() ) );
    return format( masked_value, aformat, amask );
}


Xtring Formatter::format( const Variant &avalue, const char *aformat,
                          const char *amask, Variant::Type valuetype ) const
{
    if( valuetype == Variant::tInvalid )
        valuetype = avalue.type();
    if( valuetype == Variant::tString && aformat && strstr(aformat,"%$")!=0)
        valuetype = Variant::tMoney;
    switch ( valuetype ) {
    case Variant::tDate:
        return format( avalue.toDate(), aformat);
    case Variant::tTime:
        return format( avalue.toTime(), aformat );
    case Variant::tDateTime:
        return format( avalue.toDateTime(), aformat );
    case Variant::tBool:
        return format( avalue.toBool(), aformat, amask );
    case Variant::tInt:
    case Variant::tLong:
        return format( static_cast<long long>(avalue.toInt()), aformat, amask );
    case Variant::tDouble:
        return format( avalue.toDouble(), aformat, amask );
    case Variant::tMoney:
        return format( avalue.toMoney(), aformat, amask );
    default:  /*  Formatter::string */
        return format( avalue.toString(), aformat, amask );
    }
}

/*
  Formatea un Date a una cadena segn una máscara o un formato.
  Si es un formato, es del tipo estandard de c, %d%M%Y etc.
*/
Xtring Formatter::format( const Date &adate, const char *aformat ) const
{
    return adate.toString((aformat&&*aformat)?aformat:mRegConfig.getDateFormat().c_str());
}

Xtring Formatter::format( const Time &atime, const char *aformat) const
{
    return atime.toString((aformat&&*aformat)?aformat:mRegConfig.getTimeFormat().c_str());
}


Xtring Formatter::format( const DateTime &adatetime, const char *aformat ) const
{
    return adatetime.toString((aformat&&*aformat)?aformat:mRegConfig.getDateTimeFormat().c_str());
}


Xtring Formatter::mask(const Variant &avalue, const char *amask, Variant::Type valuetype) const
{
    if( valuetype == Variant::tInvalid )
        valuetype = avalue.type();
    switch( valuetype ) {
    case Variant::tDate:
    case Variant::tTime:
    case Variant::tDateTime:
        return mask(avalue.toDateTime(), amask, valuetype);
    case Variant::tInt:
        return mask(avalue.toInt(), amask, valuetype);
    case Variant::tDouble:
        return mask(avalue.toDouble(), amask, valuetype);
    case Variant::tMoney:
        // Si llamo a toMoney se llama a si misma
        // Pero necesito poner la mascara regional de moneda
        return mask(avalue.toDouble(),
                    (amask==0||*amask=='\0')?mRegConfig.getCurrencyMask().c_str():amask, valuetype );
    default: /*  Formatter::string */
        return mask(avalue.toString(), amask, valuetype);
    }
}


Xtring Formatter::mask( const char *text, const char *amask, Variant::Type valuetype ) const
{
    return maskalfa_mask( text, valuetype, amask );
}

Xtring Formatter::mask( long long val, const char *amask, Variant::Type valuetype ) const
{
    char text[ 32 ];

    sprintf( text, "%lld", val );
    return masknum_mask( text, valuetype, amask );
}

Xtring Formatter::mask( Money val, const char *amask, Variant::Type valuetype ) const
{
    return mask( val.toDouble(), amask, valuetype );
}

/**
 * Formats a double value into the locale information of the RegConfig member
   If the libc locale information does not match the the RegConfig information, sprintf will
   aformat the value with the locale information, while the Formatter will do it with the RegConfig
   information. I.E., if val == 1234.34 and the locale is set to "C", sprintf will return
	1234.340000, but then, if the RegConfig information is set to es_ES, this function will return
    1234,34.
 * @param val
 * @param amask
 * @param valuetype
 * @return
 */
Xtring Formatter::mask( double val, const char *amask, Variant::Type valuetype ) const
{
    char buffer[ 128 ], *pbuffer = buffer;
    char *lastnonzero = 0;
    sprintf( buffer, "%f", val );
    struct lconv *lc = localeconv();
    while ( *pbuffer ) {
        if( *pbuffer != '0' && lastnonzero )
            lastnonzero = pbuffer;
        if ( *pbuffer == *lc->decimal_point ) {
            lastnonzero = pbuffer+1;
            if( LC_DECIMAL_POINT != '\0' )
                *pbuffer = LC_DECIMAL_POINT;
        }
        if ( *pbuffer == *lc->negative_sign ) {
            if( LC_NEGATIVE_SIGN != '\0' )
                *pbuffer = LC_NEGATIVE_SIGN;
        }
        pbuffer++;
    }
    // Remove unnecessary zeros after decimal point
    if( lastnonzero )
        *(lastnonzero + 1) = '\0';
    return masknum_mask( buffer, valuetype, amask );
}



Xtring Formatter::mask( const DateTime &adate, const char *amask, Variant::Type valuetype ) const
{
    const char *pmask;
    char buffer[ MAX_FORMAT_BUF ] = "", itemformat[] = "%0_d";
    int l, days, months, years, hours, minutes, seconds;
    struct tm lt;

    if ( !adate.isValid() )
        return "00/00/0000";
    else {
        lt.tm_sec = adate.getSeconds();
        lt.tm_min = adate.getMinutes();
        lt.tm_hour = adate.getHours();
        lt.tm_mday = adate.getDay();
        lt.tm_mon = adate.getMonth() - 1;
        lt.tm_year = adate.getYear() - 1900;
    }

    int module = 1;

    days = months = years = hours = minutes = seconds = 0;
    if ( amask == NULL || !( *amask ) ) {
        switch ( valuetype ) {
        case Variant::tDate:
            pmask = mRegConfig.getDateMask().c_str();
            break;
        case Variant::tTime:
            pmask = mRegConfig.getTimeMask().c_str();
            break;
        default:  /* Variant::tDateTime */
            pmask = mRegConfig.getDateTimeMask().c_str();
        }
    } else
        pmask = amask;
    pmask--;
    do {
        pmask++;
        switch ( *pmask ) {
        case 'd':
        case 'D':
            days++;
            break;
        case 'm':
            months++;
            break;
        case 'y':
        case 'Y':
            years++;
            module *= 10;
            break;
        case 'h':
        case 'H':
            hours++;
            break;
        case 'M':
            minutes++;
            break;
        case 's':
        case 'S':
            seconds++;
            break;
        default:
            if ( days ) {
                itemformat[2] = static_cast<char>(days + '0');
                sprintf( buffer + strlen( buffer ), itemformat, lt.tm_mday );
                days = 0;
            }
            if ( months ) {
                itemformat[2] = static_cast<char>(months + '0');
                sprintf( buffer + strlen( buffer ), itemformat, lt.tm_mon + 1 );
                months = 0;
            }
            if ( years ) {
                itemformat[2] = static_cast<char>(years + '0');
                sprintf( buffer + strlen( buffer ), itemformat, ( lt.tm_year + 1900 ) % module );
                years = 0;
            }
            if ( hours ) {
                itemformat[2] = static_cast<char>(hours + '0');
                sprintf( buffer + strlen( buffer ), itemformat, lt.tm_hour );
                hours = 0;
            }
            if ( minutes ) {
                itemformat[2] = static_cast<char>(minutes + '0');
                sprintf( buffer + strlen( buffer ), itemformat, lt.tm_min );
                minutes = 0;
            }
            if ( seconds ) {
                itemformat[2] = static_cast<char>(seconds + '0');
                sprintf( buffer + strlen( buffer ), itemformat, lt.tm_sec );
                seconds = 0;
            }
            l = strlen( buffer );
            buffer[ l ] = *pmask;
            buffer[ l + 1 ] = '\0';
        }
    } while ( *pmask );
    return Xtring( buffer );
}


int Formatter::unMaskInt( const Xtring &text, const char *amask ) const
{
    return text.toInt();
}

int Formatter::unFormatInt( const Xtring &text, const char *aformat ) const
{
    return text.toInt();
}


double Formatter::unMaskDouble( const Xtring &text, const char * /*amask*/ ) const
{
    char buffer[ 128 ], *pbuffer = buffer;
    const char *ptext = text.c_str();

    Variant::Type valuetype = Variant::tDouble; // Para las macros LC_
    struct lconv *lc = localeconv();
    /* Remove invalid characters */
    while ( *ptext ) {
        if ( isdigit( *ptext ) )
            *pbuffer++ = *ptext;
        else if ( *ptext == LC_NEGATIVE_SIGN )
            *pbuffer++ = *lc->negative_sign ? *lc->negative_sign : '-';
        else if ( *ptext == LC_DECIMAL_POINT )
            *pbuffer++ = *lc->decimal_point;
        ptext++;
    }
    *pbuffer = '\0';
    return atof( buffer );
}

double Formatter::unFormatDouble( const Xtring &text, const char *aformat ) const
{
    return unMaskDouble( text, aformat );
}

Money Formatter::unMaskMoney( const Xtring &text, const char * /*amask*/ ) const
{
    char buffer[ 128 ], *pbuffer = buffer;
    const char *ptext = text.c_str();
    int ndecs = -1;

    Variant::Type valuetype = Variant::tMoney; // Para las macros LC_
    struct lconv *lc = localeconv();
    /* Remove invalid characters */
    while ( *ptext ) {
        if ( isdigit( *ptext ) ) {
            *pbuffer++ = *ptext;
            if( ndecs >= 0 )
                ndecs++;
        }
        else if ( *ptext == LC_NEGATIVE_SIGN )
            *pbuffer++ = *lc->negative_sign ? *lc->negative_sign : '-';
        else if ( *ptext == LC_DECIMAL_POINT ) {
            *pbuffer++ = *lc->decimal_point;
            ndecs = 0;
        }
        ptext++;
    }
    *pbuffer = '\0';
    return Money(atof( buffer ), ndecs < 2 ? 2: ndecs );
}

Money Formatter::unFormatMoney( const Xtring &text, const char *aformat ) const
{
    return unMaskMoney( text, aformat );
}



DateTime Formatter::unMaskDateTime( const Xtring &text, const char *amask, Variant::Type valuetype ) const
{
    const char * pmask, *ptext;
    struct tm lt = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    pmask = static_cast<const char *>(amask);
    if ( amask == NULL || !( *amask ) ) {
        switch ( valuetype ) {
        case Variant::tDate:
            pmask = mRegConfig.getDateMask().c_str();
            break;
        case Variant::tTime:
            pmask = mRegConfig.getTimeMask().c_str();
            break;
        default:  /* Variant::tDateTime */
            pmask = mRegConfig.getDateTimeMask().c_str();
        }
    }
    ptext = text.c_str();
    while ( *pmask && *ptext ) {
        switch ( *pmask ) {
        case 'd':
        case 'D':
            lt.tm_mday = lt.tm_mday * 10 + static_cast<int>( ( *ptext ) - '0' );
            break;
        case 'm':
            lt.tm_mon = lt.tm_mon * 10 + static_cast<int>( *ptext - '0' );
            break;
        case 'y':
        case 'Y':
            lt.tm_year = lt.tm_year * 10 + static_cast<int>( *ptext - '0' );
            break;
        case 'h':
        case 'H':
            lt.tm_hour = lt.tm_hour * 10 + static_cast<int>( ( *ptext ) - '0' );
            break;
        case 'M':
            lt.tm_min = lt.tm_min * 10 + static_cast<int>( ( *ptext ) - '0' );
            break;
        case 's':
        case 'S':
            lt.tm_sec = lt.tm_sec * 10 + static_cast<int>( ( *ptext ) - '0' );
            break;
        }
        pmask++;
        ptext++;
    }
    // If there is something left, it must be the year
    while( *ptext ) {
        lt.tm_year = lt.tm_year * 10 + static_cast<int>( *ptext - '0' );
        ptext++;
    }
    if( lt.tm_mday == 0 && lt.tm_mon == 0 && lt.tm_year == 0 )
        return DateTime( Date(Date::NullDate), Time( lt.tm_hour, lt.tm_min, lt.tm_sec ) );
    if ( lt.tm_mon > 0 )
        lt.tm_mon--;
    if ( lt.tm_year == 0 )  /* Just time, nor date */
        lt.tm_year = 70;
    else if ( lt.tm_year < 30 )
        lt.tm_year += 100;
    else if ( lt.tm_year > 100 )
        lt.tm_year -= 1900;
    return DateTime( Date( lt.tm_year + 1900, lt.tm_mon + 1, lt.tm_mday ),
                     Time( lt.tm_hour, lt.tm_min, lt.tm_sec ) );
}

DateTime Formatter::unFormatDateTime( const Xtring &text, const char *aformat, Variant::Type valuetype ) const
{
    struct tm tm = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    const char *pformat = aformat;
    if ( aformat == NULL || !( *aformat) ) {
        switch ( valuetype ) {
        case Variant::tDate:
            pformat = mRegConfig.getDateFormat().c_str();
            break;
        case Variant::tTime:
            pformat = mRegConfig.getTimeFormat().c_str();
            break;
        default:  /* Variant::tDateTime */
            pformat = mRegConfig.getDateTimeFormat().c_str();
        }
    }
    if ( strptime( text.c_str(), pformat, &tm ) ) {
        Date d( tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday );
        Time t( tm.tm_hour, tm.tm_min, tm.tm_sec );
        return DateTime( d, t );
    } else // La fecha no era válida
        return DateTime();
}




/* Formats the text of a currency or numeric type
   La implementación difiere de lo que dice man 7 locale cuando la cantidad
   se rodea con parentesis. Aqui mantengo el signo, si no, al convertir el
   formato a la cantidad, se pierde el signo
*/

Xtring Formatter::formatLocaleCurrency( Xtring text, double value ) const
{
    Xtring result;

    int lc_p_cs_precedes = mRegConfig.getPCSPrecedes();
    int lc_p_sep_by_space = mRegConfig.getPSepBySpace();
    int lc_p_sign_posn = mRegConfig.getPSignPosn();
    int lc_n_cs_precedes = mRegConfig.getNCSPrecedes();
    int lc_n_sep_by_space = mRegConfig.getNSepBySpace();
    int lc_n_sign_posn = mRegConfig.getNSignPosn();

    text.replace(mRegConfig.getNegativeSign(), "" );
    text.replace(mRegConfig.getPositiveSign(), "" );
    text.removeWhiteSpace();
    if ( value < 0.0 ) {
        switch ( lc_n_sign_posn ) {
            /* Positive and negative sign positions:
               0 Parentheses surround the quantity and currency_symbol.
               1 The sign string precedes the quantity and currency_symbol.
               2 The sign string follows the quantity and currency_symbol.
               3 The sign string immediately precedes the currency_symbol.
               4 The sign string immediately follows the currency_symbol.  */
        case 0:
            result = "(" + mRegConfig.getNegativeSign();
            if ( lc_n_cs_precedes ) {
                result += mRegConfig.getCurrencySymbol();
                if ( lc_n_sep_by_space )
                    result += " ";
            }
            result += text;
            if ( !lc_n_cs_precedes ) {
                if ( lc_n_sep_by_space )
                    result += " ";
                result += mRegConfig.getCurrencySymbol();
            }
            result += ")";
            break;
        case 1:
            result = mRegConfig.getNegativeSign();
            if ( lc_n_cs_precedes ) {
                result += mRegConfig.getCurrencySymbol();
                if ( lc_n_sep_by_space )
                    result += " ";
            }
            result += text;
            if ( !lc_n_cs_precedes ) {
                if ( lc_n_sep_by_space )
                    result += " ";
                result += mRegConfig.getCurrencySymbol();
            }
            break;
        case 2:
            if ( lc_n_cs_precedes ) {
                result = mRegConfig.getCurrencySymbol();
                if ( lc_n_sep_by_space )
                    result += " ";
                result += text;
            } else {
                result = text;
                if ( lc_n_sep_by_space )
                    result += " ";
                result += mRegConfig.getCurrencySymbol();
            }
            result += mRegConfig.getNegativeSign();
            break;
        case 3:
            if ( lc_n_cs_precedes ) {
                result = mRegConfig.getNegativeSign();
                result += mRegConfig.getCurrencySymbol();
                if ( lc_n_sep_by_space )
                    result += " ";
                result += text;
            } else {
                result = text;
                if ( lc_n_sep_by_space )
                    result += " ";
                result += mRegConfig.getNegativeSign();
                result += mRegConfig.getCurrencySymbol();
            }
            break;
        case 4:
            if ( lc_n_cs_precedes ) {
                result = mRegConfig.getCurrencySymbol();
                result += mRegConfig.getNegativeSign();
                if ( lc_n_sep_by_space )
                    result += " ";
                result += text;
            } else {
                result = text;
                if ( lc_n_sep_by_space )
                    result += " ";
                result += mRegConfig.getCurrencySymbol();
                result += mRegConfig.getNegativeSign();
            }
            break;
        default:
            result += mRegConfig.getNegativeSign() + text;
        }
    } else { /* positive values */
        switch ( lc_p_sign_posn ) {
            /* Positive and negative sign positions:
               0 Parentheses surround the quantity and currency_symbol.
               1 The sign string precedes the quantity and currency_symbol.
               2 The sign string follows the quantity and currency_symbol.
               3 The sign string immediately precedes the currency_symbol.
               4 The sign string immediately follows the currency_symbol.  */
        case 0:
            result = "(";
            if ( lc_p_cs_precedes ) {
                result += mRegConfig.getCurrencySymbol();
                if ( lc_p_sep_by_space )
                    result += " ";
            }
            result += text;
            if ( !lc_p_cs_precedes ) {
                if ( lc_p_sep_by_space )
                    result += " ";
                result += mRegConfig.getCurrencySymbol();
            }
            result += ")";
            break;
        case 1:
            result = mRegConfig.getPositiveSign();
            if ( lc_p_cs_precedes ) {
                result += mRegConfig.getCurrencySymbol();
                if ( lc_p_sep_by_space )
                    result += " ";
            }
            result += text;
            if ( !lc_p_cs_precedes ) {
                if ( lc_p_sep_by_space )
                    result += " ";
                result += mRegConfig.getCurrencySymbol();
            }
            break;
        case 2:
            if ( lc_p_cs_precedes ) {
                result = mRegConfig.getCurrencySymbol();
                if ( lc_p_sep_by_space )
                    result += " ";
                result += text;
            } else {
                result = text;
                if ( lc_p_sep_by_space )
                    result += " ";
                result += mRegConfig.getCurrencySymbol();
            }
            result += mRegConfig.getPositiveSign();
            break;
        case 3:
            if ( lc_p_cs_precedes ) {
                result = mRegConfig.getPositiveSign();
                result += mRegConfig.getCurrencySymbol();
                if ( lc_p_sep_by_space )
                    result += " ";
                result += text;
            } else {
                result = text;
                if ( lc_p_sep_by_space )
                    result += " ";
                result += mRegConfig.getPositiveSign();
                result += mRegConfig.getCurrencySymbol();
            }
            break;
        case 4:
            if ( lc_p_cs_precedes ) {
                result = mRegConfig.getCurrencySymbol();
                result += mRegConfig.getPositiveSign();
                if ( lc_p_sep_by_space )
                    result += " ";
                result += text;
            } else {
                result = text;
                if ( lc_p_sep_by_space )
                    result += " ";
                result += mRegConfig.getCurrencySymbol();
                result += mRegConfig.getPositiveSign();
            }
            break;
        default:
            result += text;
        }

    }
    if( result.startsWith( mRegConfig.getCurrencyDecimalPoint() ) )
        result = "0" + result;
    return result;
}



/*****************************************************************************
 * Processing of numeric masks
 *
 * #,9 -> Digit. If the first character is zero, it is replaced with a blank
 * 0 -> Digit. If the first character is zero, the zero is remained
 * . -> Decimal point position
 * , -> Thousands separator
 * + -> The number doesn't admit the negative sign
 * * -> Repeat the amask pattern
 *
 ****************************************************************************/


/* Check whether a character is valid for a amask */
/* Valuetype can be Formatter::numeric and Variant::tMoney */
int Formatter::masknum_match( Variant::Type valuetype, char text, char amask ) const
{
    switch ( amask ) {
    case '0':
        return isdigit( text );
    case '#':
    case '9':
        return isdigit( text ) || text == ' ';
    case ',':
        return text == LC_THOUSANDS_SEP;
    case '.':
        return text == LC_DECIMAL_POINT;
    default:
        return ( text == amask );
    }
}


/* Returns the default character for a amask character */
char Formatter::masknum_defaultchar( Variant::Type valuetype, char amask ) const
{
    switch ( amask ) {
    case '#':
    case '9':
        return '0';
    case ',':
        return LC_THOUSANDS_SEP;
    case '.':
        return LC_DECIMAL_POINT;
    default:
        return amask;
    }
}


/* Repeats the amask pattern up to a given length */
char *Formatter::masknum_dupmask( char *start, char *end, int maxlen ) const
{
    char * start_save, *pend;

    start_save = start;
    if ( end < start ) {
        start = end;
        end = start_save;
        start_save = start;
    } else if ( end == start ) {
        *start = '#';
        end++;
        maxlen--;
    }
    maxlen -= (end - start); // Avoid buffer overflow
    pend = end;
    while ( maxlen > 0 ) {
        start = start_save;
        while ( start < end ) {
            *pend++ = *start++;
            maxlen--;
        }
    }
    return pend;
}


/* Formats a numeric text according to a amask.
 * Tracks the position of the cursor.
*/
Xtring Formatter::masknum_mask( const char *origtext, Variant::Type valuetype, const char *origmask,
                                int startpos, int *position ) const
{
    const char *porig, *puntotext;
    char amask[ MAX_FORMAT_BUF ], *pmask, *puntomask;
    char text[ MAX_FORMAT_BUF ], *ptext;
    char entera[ MAX_FORMAT_BUF ], *pentera;
    char decimal[ MAX_FORMAT_BUF ], *pdecimal;
    char result[ MAX_FORMAT_BUF ];
    int longinsert, aborrarlen;
    bool bneg = false, on_point = false;
    int ltext, lmask, pos, ndec, borrando, autoposition;

    if ( position == NULL ) {  // No nos interesa la posición
        autoposition = static_cast<int>(strlen(origtext));
        startpos = autoposition - 1;
        position = &autoposition;
        longinsert = strlen(origtext);
    } else {
        longinsert = *position - startpos;
    }
    if ( origmask == NULL )
        origmask = "";

    borrando = ( startpos >= *position );
    ndec = 0;
    bool sign = true;
    puntomask = NULL;
    pmask = amask;
    lmask = 0;
    bool puntodeleted = false;
    char decpoint = LC_DECIMAL_POINT == '\0' ? '.' : LC_DECIMAL_POINT;

    // Create a dup of the origmask in amask
    while ( *origmask ) {
        if ( *origmask == '+' )
            sign = false;
        else if ( *origmask == '.' ) {
            puntomask = pmask;
            *pmask++ = *origmask;
            lmask++;
        } else {
            if ( puntomask != NULL )
                ndec++;
            *pmask++ = *origmask;
            lmask++;
        }
        origmask++;
    }
    *pmask = '\0';

    // ver si hemos borrado el punto decimal y añadirlo
    if( !strchr( origtext, decpoint ) && puntomask) {
        if( borrando
                && (valuetype == Variant::tDouble || valuetype == Variant::tMoney )
                && (lmask == 0 || puntomask ) ) {
            puntodeleted = true;
        }
    }

    int point_pos;
    puntotext = strchr( origtext, decpoint );
    if ( puntotext == NULL )
        point_pos = strlen( origtext );
    else
        point_pos = puntotext - origtext;
    porig = origtext;
    // If not on point and not a whole setText, overwrite
    on_point = (*position == point_pos);
    if ( (!on_point) && (position != &autoposition)
            && !( longinsert == static_cast<int>(strlen( origtext )) && startpos == 0 ) ) {
        porig = origtext;
        ptext = text;
        pos = 0;
        int maskpos = 0;
        aborrarlen = longinsert;
        if ( *porig == LC_NEGATIVE_SIGN ) {
            *ptext++ = *porig++; // No forma parte de la máscara
            pos++;
        }
        while ( *porig ) {
            // Keeps track of the number of negative and positive sign that are deleted
            if ( *porig == LC_NEGATIVE_SIGN ) {
                bneg = !bneg;
                *position -= 1;
                aborrarlen--;
            }
            else if ( pos >= startpos && pos < *position ) {
                if ( ( *porig == LC_THOUSANDS_SEP )
                        /* No sobreescribir si el caracter no va a ser valido */
                        || ( *porig == decpoint && *( porig + 1 ) != decpoint ) ) {
                    *position -= 1;
                    aborrarlen--;
                } else if ( *porig != decpoint
                            && *porig != LC_THOUSANDS_SEP
                            && amask[ maskpos ] != ','
                            && !masknum_match( valuetype, *porig, amask[ maskpos ] ) ) {
                    aborrarlen--;
                    *position -= 1;
                } else {
                    *ptext++ = *porig;
                }
            } else if ( ( pos >= *position && pos < *position + aborrarlen ) ) {
                if ( *porig == LC_THOUSANDS_SEP ) {
                    aborrarlen++;
                }
            } else {
                *ptext++ = *porig;
            }
            pos++;
            maskpos++;
            porig++;
        }
        *ptext = '\0';
        porig = text;
    }

    if( puntodeleted && strlen(text) && puntomask ) {
        int numdecsinmask = (amask + strlen(amask)) - puntomask -1;
        int postmptextpoint = strlen(text) - numdecsinmask;
        if( postmptextpoint > 0 ) {
            char *tmptext = new char[strlen(text) + numdecsinmask + 1];
            strncpy( tmptext, text, postmptextpoint );
            tmptext[postmptextpoint] = decpoint;
            tmptext[postmptextpoint+1] = '\0';
            strcat(tmptext, text + strlen(text)-numdecsinmask );
            strcpy( text, tmptext);
            delete [] tmptext;
        }
    }

    // Remove not numeric characters from text
    bool pointfound = false;
    bool removingzeros = true;
    ptext = text;
    pos = 0;
    while ( *porig ) {
        if ( isdigit( *porig ) && !( *porig == '0' && removingzeros ) ) {
            *ptext++ = *porig;
            removingzeros = false;
        } else {
            /* Se ha pulsado . en vez del punto decimal local */
            if ( *porig == '.'
                    && ( *( porig + 1 ) == decpoint /*|| *(porig+1)=='\0'*/ ) ) {
                ; //*position+=1;
            }   else if ( *porig == decpoint ) {
                if ( !pointfound ) {
                    pointfound = true;
                    *ptext++ = decpoint;
                    removingzeros = false;
                }
            } else if ( *porig == LC_NEGATIVE_SIGN ) {
                bneg = !bneg;
                *position -= 1;
            } else if ( *porig == LC_POSITIVE_SIGN ) {
                bneg = 0;
                *position -= 1;
            } else if( !pointfound ) {
                *position -= 1;
            }
        }
        porig++;
        pos++;
    }
    *ptext = '\0';

    // If there is no amask, return the number as it is
    if ( !*amask ) {
        char *p_strcpy_orig = text, *p_strcpy_dest = result;
        int maxlen = MAX_FORMAT_BUF - 1, len = 0;
        if ( bneg && sign ) {
            *p_strcpy_dest++ = char( LC_NEGATIVE_SIGN );
            *position += 1;
            maxlen--;
        }
        if( *p_strcpy_orig == decpoint ) {
            *p_strcpy_dest++ = '0';
        }
        while( (*p_strcpy_dest = *p_strcpy_orig) && (len++ < maxlen) ) {
            p_strcpy_dest++;
            p_strcpy_orig++;
        }
        *p_strcpy_dest = '\0';
        return result;
    }

    ltext = strlen( text );
    puntotext = strchr( text, decpoint );
    puntomask = strchr( amask, '.' );
    pdecimal = decimal;
    // Format decimal part
    if ( puntomask ) {
        if ( !puntotext )
            if ( borrando ) {
                ptext = text + *position;
            } else {
                ptext = text + ltext - ( ( ltext == longinsert ) ? 0 : ndec );
            }
        else
            ptext = (char *)puntotext + 1;
        pmask = puntomask + 1;
        *pdecimal++ = decpoint;
        while ( *ptext && *pmask ) {
            if ( masknum_match( valuetype, *ptext, *pmask ) ) {
                *pdecimal++ = *ptext++;
                if ( *pmask )
                    pmask++;
            } else if ( *pmask == ',' ) {
                // Raro que haya comas en parte decimal
                *pdecimal++ = LC_THOUSANDS_SEP;
                pmask++;
            } else {
                ptext++;
            }
            // creando parte decimal
        }
        while ( *pmask ) {
            if ( *pmask == '#' )
                pmask++;
            else
                *pdecimal++ = masknum_defaultchar( valuetype, *pmask++ );
        }
    }
    *pdecimal = '\0';
    // Formatear parte entera, desde el punto hacia la izquierda
    // o desde el final si no hay punto
    pentera = entera + MAX_FORMAT_BUF - 1;
    *pentera-- = '\0';
    if ( !puntomask ) {
        pmask = amask + lmask - 1;
    } else {
        pmask = puntomask - 1;
    }
    if ( !puntotext ) {
        if( borrando ) {
            ptext = text + ltext - 1;
        } else {
            ptext = text + ltext - ( ( ltext == longinsert ) ? 0 : ndec ) - 1;
        }
    } else
        ptext = (char *)puntotext - 1;
    // Save the text and then format the integer part.
    char save_text[MAX_FORMAT_BUF + 1];
    char *save_ptext = ptext;
    char *save_pmask = pmask;
    char *save_pentera = pentera;
    int save_position = *position;
    strcpy( save_text, text );
    while( true ) {
        while ( ptext >= text && pmask >= amask && pentera > entera ) {
            if ( masknum_match( valuetype, *ptext, *pmask ) ) {
                *pentera-- = *ptext--;
                pmask--;
            } else if ( *pmask == ',' ) {
                if( LC_THOUSANDS_SEP != '\0' ) {
                    *pentera-- = LC_THOUSANDS_SEP;
                    *position += 1;
                }
                pmask--;
            } else {
                ptext--;
            }
        }
        // Anadir lo que queda de mascara
        while ( pmask >= amask && pentera > entera ) {
            if ( *pmask == '0' ) {
                *pentera-- = '0';
            } else if ( *pmask == ',' && *( pmask - 1 ) == '0' ) {
                if( LC_THOUSANDS_SEP != '\0' )
                    *pentera-- = LC_THOUSANDS_SEP;
            } else {
                *pentera-- = ' ';
            }
            pmask--;
            *position += 1;
        }
        if( ptext >= text ) {
            strcpy( text, save_text);
            ptext = --save_ptext;
            pmask = save_pmask;
            pentera = save_pentera;
            *position = --save_position;
        } else {
            break;
        }
    }
    pentera++;

    if ( *pentera == LC_THOUSANDS_SEP && LC_THOUSANDS_SEP ) {
        *position -= 1;
        pentera++;
        // quitando coma inicial: position
    }
    if ( bneg && sign && LC_NEGATIVE_SIGN != '\0' ) {
        char *pfinalsignpos = --pentera;
        *pfinalsignpos = ' ';
        while( *pfinalsignpos == ' ' )
            ++pfinalsignpos;
        *( --pfinalsignpos ) = char( LC_NEGATIVE_SIGN );
        *position += 1;
    }
    strncpy( result, pentera, MAX_FORMAT_BUF );
    result[ MAX_FORMAT_BUF - 1 ] = '\0';
    strncat( result, decimal, MAX_FORMAT_BUF-1 );
    result[ MAX_FORMAT_BUF - 1 ] = '\0';
    if ( *position < 0 )
        * position = 0;
    if ( result[ *position ] == ' ' ) {
        while ( result[ *position ] == ' ' )
            * position += 1;
        *position += 1;
    }
    if ( *position > static_cast<int>(strlen( result )))
        * position = static_cast<int>(strlen( result ));

    return Xtring( result );
}


/*****************************************************************************
 * Processing of date and string masks
 *
 * General:
 * # -> Digit, space, + or -
 * 9 -> Digit or space
 * 0 -> Digit or zero
 * A -> Alphanumeric character
 *
 * Dates and times:
 *
 * 0   -> Digit or zero
 * d,D -> Day of the month
 * m   -> Month
 * y,Y -> Year
 * h,H -> Hour
 * M   -> Minutes
 * s,S -> Seconds
 *
 * \   -> The next character is treated as literal
 *
 * Formats:
 * Numeric:
 *     $   Currency symbol
 *     B   Blank if zero
 * Dates:
 *     same as strftime
 *
 ****************************************************************************/


/* Returns the length of a amask */
int Formatter::maskalfa_len( const char *pmask ) const
{
    int len = 0;
    while ( *pmask ) {
        if ( *pmask++ == LITERAL_MASK_CHAR )
            pmask++;
        len++;
    }
    return len;
}

/* Returns the default character for a amask character */
char Formatter::maskalfa_defaultchar( Variant::Type valuetype, const char *pmask, int pos ) const
{
    int inbackslash;

    pmask--;
    do {
        pmask++;
        if ( *pmask == LITERAL_MASK_CHAR ) {
            inbackslash = 1;
            pmask++;
        } else
            inbackslash = 0;
        pos--;
    } while ( *pmask && pos >= 0 );
    if ( inbackslash )
        return * pmask;
    else
        switch ( *pmask ) {
        case 'd':
        case 'D':
        case 'y':
        case 'Y':
        case 'h':
        case 'H':
        case 'm':
        case 'M':
        case 's':
        case 'S':
            if ( valuetype == Variant::tDate
                    || valuetype == Variant::tTime
                    || valuetype == Variant::tDateTime )
                return '0';
            else
                return *pmask;
        case '0':
            return mBlankChar;
        case '9':
        case '#':
        case 'A':
        case '>':
        case '<':
            return mBlankChar;
        default:
            return *pmask;
        }
}

/* Check whether a character is valid for a amask */
int Formatter::maskalfa_match( Variant::Type valuetype, char text, const char *pmask, int pos ) const
{
    int inbackslash;

    pmask--;
    do {
        pmask++;
        if ( *pmask == LITERAL_MASK_CHAR ) {
            inbackslash = 1;
            pmask++;
        } else
            inbackslash = 0;
        pos--;
    } while ( *pmask && pos >= 0 );
    if ( inbackslash )
        return ( text == *pmask );
    else {
        if ( text == maskalfa_defaultchar( valuetype, pmask, 0 ) )
            return 1;
        else
            switch ( *pmask ) {
            case 'd':
            case 'D':
            case 'm':
            case 'M':
            case 'y':
            case 'Y':
            case 'h':
            case 'H':
            case 's':
            case 'S':
                if ( valuetype == Variant::tDate
                        || valuetype == Variant::tTime
                        || valuetype == Variant::tDateTime )
                    return isdigit( text );
                else
                    return ( text == *pmask );
            case '9':
                return isdigit( text ) || text == ' ';
            case '#':
                return isdigit( text ) || text == ' '
                       || text == LC_POSITIVE_SIGN || text == LC_NEGATIVE_SIGN;
            case '0':
                return isdigit( text );
            case '>':
            case '<':
            case 'A':
                return isalpha( text );
            default:
                return ( text == *pmask );
            }
    }
}

/* gets the nth character in a amask, if it is not literal */
char Formatter::maskalfa_nth( Variant::Type /* valuetype */, const char *pmask, int pos ) const
{
    int inbackslash;

    pmask--;
    do {
        pmask++;
        if ( *pmask == LITERAL_MASK_CHAR ) {
            inbackslash = 1;
            pmask++;
        } else
            inbackslash = 0;
        pos--;
    } while ( *pmask && pos >= 0 );
    if ( inbackslash )
        return '\0';
    else
        return *pmask;
}


/* returns whether a character in a amask is editable */
bool Formatter::maskalfa_iseditable( Variant::Type valuetype, const char *pmask, int pos ) const
{
    bool inbackslash;

    pmask--;
    do {
        pmask++;
        if ( *pmask == LITERAL_MASK_CHAR ) {
            inbackslash = true;
            pmask++;
        } else
            inbackslash = false;
        pos--;
    } while ( *pmask && pos >= 0 );
    if ( inbackslash )
        return 0;
    else
        switch ( *pmask ) {
        case 'd':
        case 'D':
        case 'm':
        case 'M':
        case 'y':
        case 'Y':
        case 'h':
        case 'H':
        case 's':
        case 'S':
            if ( valuetype == Variant::tDate
                    || valuetype == Variant::tTime
                    || valuetype == Variant::tDateTime )
                return true;
            else
                return false;
        case '9':
        case '#':
        case '0':
        case 'A':
        case '<':
        case '>':
            return true;
        default:
            return false;
        }
}

/* Returns the first editable character in a amask */
int Formatter::maskalfa_firsteditable( Variant::Type valuetype, const char *pmask ) const
{
    int pos = 0;
    while ( !maskalfa_iseditable( valuetype, pmask, pos ) && pos < static_cast<int>(strlen( pmask ) ))
        pos++;
    return pos;
}

/* Returns the previous editable character in a amask */
int Formatter::maskalfa_previouseditable( Variant::Type valuetype, const char *pmask, int pos ) const
{
    while ( !maskalfa_iseditable( valuetype, pmask, pos ) && pos )
        pos--;
    return pos;
}

/* Returns the next editable character in a amask */
int Formatter::maskalfa_nexteditable( Variant::Type valuetype, const char *pmask, int pos ) const
{
    while ( !maskalfa_iseditable( valuetype, pmask, pos ) && pos < static_cast<int>(strlen( pmask ) ))
        pos++;
    return pos;
}

/* Returns the last editable character in a amask */
int Formatter::maskalfa_lasteditable( Variant::Type valuetype, const char *pmask ) const
{
    int pos = strlen( pmask );
    while ( !maskalfa_iseditable( valuetype, pmask, pos ) && pos )
        pos--;
    return pos;
}

/* Formats a text according to its type and mask(string, date and time) */
Xtring Formatter::maskalfa_mask( const char *origtext, Variant::Type valuetype, const char *amask,
                                 int startpos, int *position ) const
{
    const char * porig;
    char *text, *ptext;
    int borrandomask, borrando, endpos;
    char result[ MAX_FORMAT_BUF ], *presult;
    int autoposition = 0;

    if ( amask == 0 || *amask == 0 ) {
        return Xtring( origtext );
    }
    if ( position == 0 )
        position = &autoposition;

    text = new char[ MAX_FORMAT_BUF ];
    presult = result;
    borrandomask = ( startpos >= *position && !maskalfa_iseditable( valuetype, amask, *position ) );
    borrando = ( startpos >= *position );
    porig = origtext;
    ptext = text;
    int postext = 0;
    int posmask = 0;
    if ( borrando ) { // Insertar el/los caracteres por defecto
        while ( *porig && postext < *position ) {
            *ptext++ = *porig++;
            postext++;
            posmask++;
        }
        while ( *porig && postext < startpos ) {
            *ptext++ = maskalfa_defaultchar( valuetype, amask, posmask );
            postext++;
        }
        while ( *porig ) {
            *ptext++ = *porig++;
        }
        *ptext = '\0';
        ptext = text;
    } else { /* sobreescribir */
        endpos = *position + ( *position - startpos );
        while ( *porig ) {
            /* No sobreescribir si el caracter no va a ser valido */
            if ( postext >= startpos && postext < *position ) {
                if ( !maskalfa_match( valuetype, *porig, amask, postext ) ) {
                    *position -= 1;
                    endpos--;
                } else {
                    *ptext++ = *porig;
                }
            } else if ( postext >= *position && postext < endpos ) {}
            else {
                *ptext++ = *porig;
            }
            porig++;
            postext++;
        }
        *ptext = '\0';
        ptext = text;
    }

    postext = posmask = 0;
    while ( *ptext && maskalfa_defaultchar( valuetype, amask, posmask ) ) {
        if ( maskalfa_match( valuetype, *ptext, amask, posmask ) ) {
            if ( maskalfa_nth( valuetype, amask, posmask ) == '>' ) {
                *presult++ = toupper( *ptext );
                ptext++;
            } else if ( maskalfa_nth( valuetype, amask, posmask ) == '<' ) {
                *presult++ = tolower( *ptext );
                ptext++;
            } else
                *presult++ = *ptext++;
            postext++;
            posmask++;
        } else if ( !maskalfa_iseditable( valuetype, amask, posmask ) ) {
            *presult++ = maskalfa_defaultchar( valuetype, amask, posmask );
            posmask++;
            if ( postext <= startpos && !borrandomask ) {
                *position += 1;
            }
        } else {
            ptext++;
            postext++;
            if ( postext <= startpos && !borrandomask ) {
                *position -= 1;
            }
        }
        *presult = '\0';
    }
    while ( maskalfa_defaultchar( valuetype, amask, posmask ) ) {
        *presult++ = maskalfa_defaultchar( valuetype, amask, posmask );
        posmask++;
    }
    *presult = '\0';
    if ( *position < 0 )
        * position = 0;
    if ( borrandomask ) {
        if ( *position > 0 && !maskalfa_iseditable( valuetype, amask, *position - 1 ) )
            * position = maskalfa_previouseditable( valuetype, amask, *position ) + 1;
    } else if ( !maskalfa_iseditable( valuetype, amask, *position ) ) {
        *position = maskalfa_nexteditable( valuetype, amask, *position );
    }
    if ( *position < maskalfa_firsteditable( valuetype, amask ) ) {
        *position = maskalfa_firsteditable( valuetype, amask );
    } else if ( *position > maskalfa_lasteditable( valuetype, amask ) ) {
        *position = maskalfa_lasteditable( valuetype, amask ) + 1;
    }
    delete [] text;
    return Xtring( result );
}



/* Generic function to format a text according to its type and amask */
Xtring Formatter::mask_inedit( const char *origtext, Variant::Type valuetype,
                               const char *amask, int start_pos, int *position ) const
{
    switch ( valuetype ) {
    case Variant::tDate:
        if ( !amask || !*amask )
            amask = mRegConfig.getDateMask().c_str();
        return maskalfa_mask( origtext, valuetype, amask, start_pos, position );
    case Variant::tTime:
        if ( !amask || !*amask )
            amask = mRegConfig.getTimeMask().c_str();
        return maskalfa_mask( origtext, valuetype, amask, start_pos, position );
    case Variant::tDateTime:
        if ( !amask || !*amask )
            amask = mRegConfig.getDateTimeMask().c_str();
        return maskalfa_mask( origtext, valuetype, amask, start_pos, position );
    case Variant::tInt:
        if ( !amask || !*amask )
            amask = "999999";
        return masknum_mask( origtext, valuetype, amask, start_pos, position );
    case Variant::tDouble:
        if ( !amask || !*amask )
            amask = mRegConfig.getNumberMask().c_str();
        return masknum_mask( origtext, valuetype, amask, start_pos, position );
    case Variant::tMoney:
        if ( !amask || !*amask )
            amask = mRegConfig.getCurrencyMask().c_str();
        return masknum_mask( origtext, valuetype, amask, start_pos, position );
    default:  /*  Formatter::string */
        return maskalfa_mask( origtext, valuetype, amask, start_pos, position );
    }
}

short int Formatter::masknum_ndecimals(const char *mask)
{
    short int ret = -1;
    const char *ppoint = strchr( mask, '.' );
    if( strempty( mask ) )
        return ret;
    ret ++;
    if( ppoint ) {
        while( * ++ppoint )
            if( *ppoint == '9' || *ppoint == '0' || *ppoint == '#' )
                ret++;
    }
    return ret;
}

} // nampespace GONG
