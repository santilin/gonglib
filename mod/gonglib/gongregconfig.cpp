#include <limits>
#include <clocale>
#include <langinfo.h>

#include "gongdebug.h"
#include "gongregconfig.h"
#include "gongiconv.h"

namespace gong {

#define MAX_FORMAT_BUF 255

RegConfig::RegConfig(const char * language)
    : mDecimalPoint('\0'), mThousandsSep('\0'), mCurrencyDecimalPoint('\0'),
      mCurrencyThousandsSep('\0'), mCurrencyFracDigits(0), mPSignPosn(0),
      mPCSPrecedes(0), mPSepBySpace(0), mNSignPosn(0), mNCSPrecedes(0),
      mNSepBySpace(0)
{
    setLanguage( language );
}

/**
 * Sets the language for the application messages.
   Supports GNU extension: LANGUAGE can be a colon separated list of locales, like "es_ES@UTF-8:es"


	Para las fechas, la configuración regional solo tiene un formato, que para el locale es_ES es
	DATE: %d/%m/%y
	TIME: %T
	DATETIME: %a %d %b %Y %T %Z

	A partir de esto, esta clase define formatos, que son los anteriores, y mascaras, que se construyen a partir de ellos:
	DATE: dd/mm/yy
	TIME: HH:MM:SS
	DATETIME: dd/mm/yy HH:MM:SS

 * @param language
 * @return
 */
bool RegConfig::setLanguage(const char *language)
{
    /// \todo {check} In some distributions, LANGUAGE is empty and LANG is not
    _GONG_DEBUG_PRINT(2, Xtring("LANGUAGE=") + (getenv( "LANGUAGE" ) ? getenv( "LANGUAGE" ): "null"));
    _GONG_DEBUG_PRINT(2, Xtring("LC_ALL=") + (getenv( "LC_ALL" ) ? getenv( "LC_ALL" ): "null"));
    _GONG_DEBUG_PRINT(2, Xtring("LANG=") + (getenv( "LANG" ) ? getenv( "LANG" ): "null"));
    // The order in which the locale is set when none specified is (as for man locale(7)):
    // 1) LC_ALL, 2) LC_XXXXXXX, 3) LANG
    // We override it so that LANGUAGE takes precedende over LC_XXXXXXXX
    Xtring lang(language);
    if( !lang.isEmpty() ) {
        XtringList locales;
        lang.tokenize( locales, ":" );
        XtringList::const_iterator it;
        for( it = locales.begin();
                it != locales.end();
                ++ it ) {
            if( setLocale( (*it).c_str() ) ) {
                _GONG_DEBUG_PRINT(1, Xtring("Locale set to: ") + *it );
                return true;
            }
        }
    }
    if( !setLocale( "" ) ) { // Environment variables locale
        _GONG_DEBUG_WARNING( "No se ha podido encontrar el locale especificado en las variables del entorno" );
    }
    return false;
}

/**
 * Sets the locales for LC_MESSAGES, LC_TIME & LC_MONETARY for the application and sets some
		formats in this RegConfig
 * @param locale
			If empty, sets the locale according to the environment variables
            Note that the GNU extension for the env. variable LANGUAGE is not supported here
 * @return
 */
bool RegConfig::setLocale( const char *locale )
{
    _GONG_DEBUG_PRINT(3, Xtring("Trying locale ") + locale );
    bool ret = true;
    if( !setlocale(LC_ALL, locale ) ) {
        _GONG_DEBUG_WARNING( Xtring::printf("LC_ALL '%s' not available", locale) );
        ret = false;
    }
    setLocaleNumberFormat( locale );
    // This must be done after setLocaleNumberFormat, to have the frac_digits set
    setLocaleCurrencyFormat( locale );
    setLocaleDateFormats( locale );
    _GONG_DEBUG_PRINT(3, Xtring("LC_MESSAGES set to ") + setlocale(LC_MESSAGES, NULL) );
    mLanguage = Xtring(setlocale(LC_MESSAGES, NULL)).left(2);
    return ret;
}

bool RegConfig::setLocaleDateFormats( const char *locale )
{
    _GONG_DEBUG_PRINT(10, nl_langinfo( D_FMT ) );
    _GONG_DEBUG_PRINT(10, nl_langinfo( D_T_FMT ) );
    bool ret = true;
    if( ! (ret = setlocale(LC_TIME, locale ) ) ) {
        _GONG_DEBUG_WARNING( Xtring::printf("LC_TIME '%s' not available", locale) );
        ret = false;
    }
    _GONG_DEBUG_PRINT(3, Xtring("LC_TIME set to ") + setlocale(LC_TIME, NULL) );
    setDateFormat( nl_langinfo( D_FMT ) );
    setTimeFormat( nl_langinfo( T_FMT ) );
    setDateTimeFormat( nl_langinfo( D_T_FMT ) );
    setDateMask( nl_langinfo( D_FMT ) );
    setTimeMask( nl_langinfo( T_FMT ) );
    setDateTimeMask( nl_langinfo( D_T_FMT ) );
    return ret;
}

/**
 * Sets the current LC_MONETARY locale.and sets the monetary number format for RegConfig
 * Warning: The glib "es_ES" family (and probably others too) of locales defines incorrectly
            the value of p_cs_precedes and n_cs_precedes to 1, when it shoud be 0

	http://sources.redhat.com/bugzilla/post_bug.cgi

 *
 * @param locale
 * @return
 */
bool RegConfig::setLocaleCurrencyFormat( const char *locale )
{
    struct lconv * lc;
    char *ret = 0;
    Xtring codeset = IConv::canonicalCodeName(nl_langinfo( CODESET ));
    if( ! (ret = setlocale(LC_MONETARY, locale ) ) ) {
        _GONG_DEBUG_WARNING( Xtring::printf("LC_MONETARY '%s' not available", locale) );
        return false;
    }
    _GONG_DEBUG_PRINT(10, Xtring("LC_MONETARY set to ") + setlocale( LC_MONETARY, NULL) );
    lc = localeconv();
    _GONG_DEBUG_PRINT(10, Xtring::printf("codeset='%s' decpoint='%c' currency='%s' thousep='%c'",
                                         codeset.c_str(), *lc->mon_decimal_point, lc->currency_symbol, *lc->mon_thousands_sep ) );
    // glibc bug: es_ES localedata is not correct for p_cs_precedes and n_cs_precedes
    bool is_es_ES = (strncasecmp( ret, "es_ES", 4 ) == 0);
    if( !codeset.isEmpty() && codeset != "UTF-8" ) {
        IConv iconverter( "UTF-8", codeset );
        setCurrencyFormat(
            iconverter.convert(lc->currency_symbol),
            static_cast<int>(lc->frac_digits),
            *lc->mon_decimal_point,
            *lc->mon_thousands_sep,
            is_es_ES ? 0 : lc->p_cs_precedes,
            lc->p_sep_by_space,
            lc->p_sign_posn,
            is_es_ES ? 0 : lc->n_cs_precedes,
            lc->n_sep_by_space,
            lc->n_sign_posn,
            lc->grouping );
    } else {
        setCurrencyFormat(
            *lc->currency_symbol == '\xa4'? "\xe2\x82\xa0" : lc->currency_symbol,
            static_cast<int>(lc->frac_digits),
            *lc->mon_decimal_point,
            *lc->mon_thousands_sep,
            is_es_ES ? 0 : lc->p_cs_precedes,
            lc->p_sep_by_space,
            lc->p_sign_posn,
            is_es_ES ? 0 : lc->n_cs_precedes,
            lc->n_sep_by_space,
            lc->n_sign_posn,
            lc->grouping );
    }
    return ret;
}

bool RegConfig::setLocaleNumberFormat( const char *locale )
{
    struct lconv * lc;
    bool ret = true;
    if( ! (ret = setlocale(LC_NUMERIC, locale ) ) ) {
        _GONG_DEBUG_WARNING( Xtring::printf("LC_NUMERIC '%s' not available", locale) );
        ret = false;
    }
    lc = localeconv();
    setNumberFormat(
        2,
        *lc->decimal_point,
        *lc->thousands_sep,
        lc->positive_sign,
        lc->negative_sign,
        lc->grouping );
    return ret;
}


void RegConfig::setCurrencyFormat(
    const Xtring &currencysymbol,
    int currencyfracdigits,
    char currencydecimalpoint,
    char currencythousandssep,
    int p_cs_precedes,
    int p_sep_by_space,
    int p_sign_posn,
    int n_cs_precedes,
    int n_sep_by_space,
    int n_sign_posn,
    const char *grouping )
{
    setCurrencySymbol( currencysymbol );
    if ( currencydecimalpoint != '\0' ) {
        setCurrencyDecimalPoint( currencydecimalpoint );
    } else if ( getDecimalPoint() != '\0' ) {
        setCurrencyDecimalPoint( getDecimalPoint() );
    } else {
        setCurrencyDecimalPoint( '.' );
    }
    if ( currencythousandssep != '\0' ) {
        setCurrencyThousandsSep( currencythousandssep );
    } else if ( getThousandsSep() != '\0' ) {
        setCurrencyThousandsSep( getThousandsSep() );
    } else {
        setCurrencyThousandsSep( ',' );
    }
    mCurrencyMask = buildNumberMask( grouping, (currencyfracdigits==127)?2:currencyfracdigits );
    mPSignPosn = p_sign_posn;
    mPCSPrecedes = p_cs_precedes;
    mPSepBySpace = p_sep_by_space;
    mNSignPosn = n_sign_posn;
    mNCSPrecedes = n_cs_precedes;
    mNSepBySpace = n_sep_by_space;
}

void RegConfig::setNumberFormat(
    int frac_digits,
    char decimalpoint,
    char thousandssep,
    const Xtring& positivesign,
    const Xtring& negativesign,
    const char *grouping )
{
    if ( ( positivesign != "," ) && ( positivesign != "." ) )
        setPositiveSign( positivesign );
    if ( negativesign.size() && negativesign != "," && negativesign != "." )
        setNegativeSign( negativesign );
    else
        setNegativeSign( "-" );
    if ( decimalpoint != '\0' ) {
        setDecimalPoint( decimalpoint );
    } else {
        setDecimalPoint( ',' );
    }
    if ( thousandssep != '\0' ) {
        setThousandsSep( thousandssep );
    } else {
        setThousandsSep( '.' );
    }
    mNumberMask = buildNumberMask( grouping, frac_digits );
}


/* Create default monetary/number mask. See <locale.h> for description
   Esta implementación no es exactamente la misma que sugiere man 7 locale.
   Allí se dice que el último elemento de grouping, si es cero, indica que se
   repite solo el elemento anterior. Sin embargo, por la forma de tratar las
   mascaras en gong::Formatter con el *, aqui se repite toda la agrupación de
   dígitos.
*/
Xtring RegConfig::buildNumberMask( const char *grouping, int digits )
{
    Xtring mask;
    mask = "#,###,###,###,###,##0"; // Maximum number
    // TODO Interpret correctly the array grouping as of locale(7)
#if 0
    if ( !grouping || !*grouping ) {
        mask = "###,###,###,###,###,###";
    } else {
        mask = grouping;
        for( uint ngroups = 1; ngroups < (18 / (strlen(grouping) + 1)); ngroups ++ ) {
            mask += ",";
            mask += grouping;
        }
    }
#endif
    if ( digits > 0 && digits != std::numeric_limits<char>::max() ) {
        mask += '.';
        while ( digits-- )
            mask += '0';
    }
    return mask;
}


void RegConfig::setDateMask( const char *dateformat )
{
    mDateMask = buildDateMask( D_FMT, dateformat );
}

void RegConfig::setTimeMask( const char *timeformat )
{
    mTimeMask = buildDateMask( T_FMT, timeformat );
}

void RegConfig::setDateTimeMask( const char *datetimeformat )
{
    mDateTimeMask = buildDateMask( D_T_FMT, datetimeformat );
}

/* Create default date/time mask. See <locale.h> for description */
Xtring RegConfig::buildDateMask( int wich_format, const char *nl_format )
{
    Xtring mask;
    char format[ MAX_FORMAT_BUF ], *pformat;
    int inpercent, embedded;
    int hasday, hasmonth, hasyear, hashour, hasminute, hassecond;


    if ( !nl_format || !*nl_format ) {
        switch ( wich_format ) {
        case D_FMT:
            mask = "dd/mm/yy";
            break;
        case T_FMT:
            mask = "HH/MM/SS";
            break;
        case D_T_FMT:
            mask = "dd/mm/yy HH/MM/SS";
            break;
        default:
            mask.clear();
        }
    } else {
        /* expand embedded date or time formats %c, %x, %X and %T */
        pformat = format;
        do {
            inpercent = 0;
            embedded = 0;
            while ( *nl_format ) {
                if ( *nl_format == '%' ) {
                    if ( inpercent ) {
                        *pformat++ = '%';
                        *pformat++ = *nl_format;
                        inpercent = 0;
                    } else {
                        inpercent = 1;
                    }
                } else if ( !inpercent ) {
                    *pformat++ = *nl_format;
                } else { /* within percent */
                    inpercent = 0;
                    switch ( *nl_format ) {
                    case 'c':  /* embedded datetime as %x %X */
                        strcpy( pformat, nl_langinfo( D_FMT ) );
                        strcat( pformat, " " );
                        strcat( pformat, nl_langinfo( T_FMT ) );
                        pformat = pformat + strlen( pformat );
                        embedded = 1;
                        break;
                    case 'C':  /* embedded long format locale datetime */
                        strcpy( pformat, nl_langinfo( D_T_FMT ) );
                        pformat = pformat + strlen( pformat );
                        embedded = 1;
                        break;
                    case 'D':  /* embedded date as %m/%d/%y */
                        strcpy( pformat, "%m/%d/%y" );
                        pformat = format + strlen( format );
                        embedded = 1;
                        break;
                    case 'x':  /* embedded locale date */
                        strcpy( pformat, nl_langinfo( D_FMT ) );
                        pformat = pformat + strlen( pformat );
                        embedded = 1;
                        break;
                    case 'X':  /* embedded locale time */
                        strcpy( pformat, nl_langinfo( T_FMT ) );
                        pformat = pformat + strlen( pformat );
                        embedded = 1;
                        break;
                    case 'T':  /* embedded time as %H:%M:%S */
                        strcpy( pformat, "%H:%M:%S" );
                        pformat = format + strlen( format );
                        embedded = 1;
                        break;
                    case 'R': 		/* GNU extension.  */
                        strcpy( pformat, "%H:%M" );
                        pformat = format + strlen( format );
                        embedded = 1;
                        break;
                    case 'r': 		/* POSIX.2 extension.  */
                        strcpy( pformat, "%H:%M:%S" ); /*"%I:%M:%S %p" originally */
                        pformat = format + strlen( format );
                        embedded = 1;
                        break;
                    default:
                        *pformat++ = '%';
                        *pformat++ = *nl_format;
                    }
                }
                nl_format++;
            }
        } while ( embedded != 0 );
        *pformat = '\0';
        // Translate format to mask
        pformat = format;
        hasday = hasmonth = hasyear = hashour = hasminute = hassecond = 0;
        inpercent = 0;
        while ( *pformat ) {
            if ( *pformat == '%' ) {
                if ( inpercent ) {
                    mask += *pformat;
                    inpercent = 0;
                } else {
                    inpercent = 1;
                }
            } else if ( !inpercent ) {
                mask += *pformat;
            } else { /* within percent */
                inpercent = 0;
                switch ( *pformat ) {
                case 'y':
                    if ( !hasyear ) {
                        mask += 'y';
                        mask += 'y';
                        hasyear = 1;
                    }
                    break;
                case 'Y':
                    if ( !hasyear ) {
                        mask += 'y';
                        mask += 'y';
                        mask += 'y';
                        mask += 'y';
                        hasyear = 1;
                    }
                    break;
                case 'm':
                case 'b':
                    if ( !hasmonth ) {
                        mask += 'm';
                        mask += 'm';
                        hasmonth = 1;
                    }
                    break;
                case 'd':
                case 'e':
                    if ( !hasday ) {
                        mask += 'd';
                        mask += 'd';
                        hasday = 1;
                    }
                    break;
                case 'H':
                case 'I':
                    if ( !hashour ) {
                        mask += 'H';
                        mask += 'H';
                        hashour = 1;
                    }
                    break;
                case 'M':
                    if ( !hasminute ) {
                        mask += 'M';
                        mask += 'M';
                        hashour = 1;
                    }
                    break;
                case 'S':
                    if ( !hassecond ) {
                        mask += 'S';
                        mask += 'S';
                        hassecond = 1;
                    }
                    break;
                case 'U':
                case 'W':
                case 'w':
                case 'p':
                case 'a':
                case 'A':
                case 'j':
                case 'B':
                case 'Z':
                    /* not used */
                    break;
                }
            }
            pformat++;
        }
    }
    return Xtring( mask );
}

}



