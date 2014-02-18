/// \todo {refactor} operator=

#include <cerrno>
#include <cstdlib>
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <cmath> // isnan
#include "gongdebug.h"
#include "gonggettext.h"
#include "gongxtring.h"
#include "gonglist.h"


namespace gong {

/**
  @class Xtring
  @brief Xtring object, derived from std::string

  @todo {refactor} add Xtring::operator= set of functions
*/

static const char *upper_table[][2] =
{
    // Utf8
    { "Á", "á" },
    { "É", "é" },
    { "Í", "í" },
    { "Ó", "ó" },
    { "Ú", "ú" },
    { "Ñ", "ñ" },
    { "Ç", "ç" },
    { "Ä", "ä" },
    { "Ë", "ë" },
    { "Ï", "ï" },
    { "Ö", "ö" },
    { "Ü", "ü" },
    { "À", "à" },
    { "È", "è" },
    { "Ì", "ì" },
    { "Ò", "ò" },
    { "Ù", "ù" },
    { "Â", "â" },
    { "Ê", "ê" },
    { "Î", "î" },
    { "Ô", "ô" },
    { "Û", "û" },
    { "Ŕ", "ŕ" }
};



Xtring Xtring::null;

Xtring Xtring::upper() const
{
    const char *pos = c_str();
    Xtring ret;
    while( *pos ) {
        uint i;
        for ( i = 0; i < sizeof ( upper_table ) /sizeof ( upper_table[0] ); i++ )
        {
            if( !strncmp( upper_table[i][1], pos, strlen( upper_table[i][1]  ) ) )
            {
                ret += upper_table[i][0];
                pos += strlen( upper_table[i][1] );
                break;
            }
        }
        if ( i == sizeof ( upper_table ) /sizeof ( upper_table[0] ) ) {
            ret += ::toupper ( *pos );
            ++pos;
        }
    }
    return ret;
}

Xtring Xtring::lower() const
{
    const char *pos = c_str();
    Xtring ret;
    while( *pos ) {
        uint i;
        for ( i = 0; i < sizeof ( upper_table ) /sizeof ( upper_table[0] ); i++ )
        {
            if( !strncmp( upper_table[i][0], pos, strlen( upper_table[i][0]  ) ) )
            {
                ret += upper_table[i][1];
                pos += strlen( upper_table[i][0] );
                break;
            }
        }
        if ( i == sizeof ( upper_table ) /sizeof ( upper_table[0] ) ) {
            ret += ::tolower ( *pos );
            ++pos;
        }
    }
    return ret;
}

Xtring Xtring::proper() const
{
    const char *pos = c_str();
    Xtring ret;
    // First letter, upper
    if( *pos ) {
        uint i;
        for ( i = 0; i < sizeof ( upper_table ) /sizeof ( upper_table[0] ); i++ )
        {
            if( !strncmp( upper_table[i][1], pos, strlen( upper_table[i][1]  ) ) )
            {
                ret += upper_table[i][0];
                pos += strlen( upper_table[i][1] );
                break;
            }
        }
        if ( i == sizeof ( upper_table ) /sizeof ( upper_table[0] ) ) {
            ret += ::toupper ( *pos );
            ++pos;
        }
    }
    // Next letters, lower
    while( *pos ) {
        uint i;
        for ( i = 0; i < sizeof ( upper_table ) /sizeof ( upper_table[0] ); i++ )
        {
            if( !strncmp( upper_table[i][0], pos, strlen( upper_table[i][0]  ) ) )
            {
                ret += upper_table[i][1];
                pos += strlen( upper_table[i][0] );
                break;
            }
        }
        if ( i == sizeof ( upper_table ) /sizeof ( upper_table[0] ) ) {
            ret += ::tolower ( *pos );
            ++pos;
        }
    }
    return ret;
}

Xtring Xtring::unproper() const
{
    const char *pos = c_str();
    Xtring ret;
    // First letter, lower
    if( *pos ) {
        uint i;
        for ( i = 0; i < sizeof ( upper_table ) /sizeof ( upper_table[0] ); i++ )
        {
            if( !strncmp( upper_table[i][0], pos, strlen( upper_table[i][0]  ) ) )
            {
                ret += upper_table[i][1];
                pos += strlen( upper_table[i][0] );
                break;
            }
        }
        if ( i == sizeof ( upper_table ) /sizeof ( upper_table[0] ) ) {
            ret += ::tolower ( *pos );
            ++pos;
        }
    }
    // Next letters, equal
    while( *pos ) {
        ret += *pos;
        ++pos;
    }
    return ret;
}

Xtring &Xtring::trim(const Xtring &whitespace)
{
    size_type b = find_first_not_of ( whitespace );
    if ( b )
        erase ( 0, b );
    size_type e = find_last_not_of ( whitespace );
    if ( e != npos )
        erase ( ++e, size() );
    return *this;
}

Xtring Xtring::trim(const Xtring &whitespace) const
{
    Xtring ret( *this );
    return ret.trim();
}


Xtring &Xtring::trimLeft(const Xtring &whitespace)
{
    size_type b = find_first_not_of ( whitespace );
    if ( b )
        erase ( 0, b );
    return *this;
}

Xtring Xtring::trimLeft(const Xtring &whitespace) const
{
    Xtring ret( *this );
    return ret.trimLeft();
}

Xtring &Xtring::trimRight(const Xtring &whitespace)
{
    size_type e = find_last_not_of ( whitespace );
    if ( e != npos )
        erase ( ++e, size() );
    return *this;
}

Xtring Xtring::trimRight(const Xtring &whitespace) const
{
    Xtring ret( *this );
    return ret.trimRight();
}




Xtring &Xtring::simplify_white_space(char usethis)
{
    // points one past the end of the unremoved elements after remove()
    bool has_removed = false;
    Xtring::const_iterator st;
    Xtring res;
    for ( st = begin(); st != end(); ++st )
    {
        if ( !isspace ( *st ) ) {
            res.push_back( *st );
            has_removed = false;
        } else if( !has_removed ) {
            has_removed = true;
            res.push_back( usethis );
        }
    }
    return (*this = res);
}

Xtring &Xtring::remove_white_space()
{
    // points one past the end of the unremoved elements after remove()
    Xtring::const_iterator st;
    Xtring res;
    for ( st = begin(); st != end(); ++st )
    {
        if ( !isspace ( *st ) )
            res.push_back ( *st );
    }
    return (*this = res);
}

int Xtring::toInt ( bool *ok, int base ) const
{
    errno = 0;
    char *endptr;
    int i = strtol ( c_str(), &endptr, base );
    if ( errno == 0 && ( *endptr == '\0' || *endptr == '.' ) )
    {
        if ( ok )
            *ok = true;
    }
    else
    {
        if ( ok )
            *ok = false;
        else {
#ifdef _GONG_DEBUG
            if( __gong_debug::_gong_debug_level > 0 ) {
                if( errno ) {
                    _GONG_DEBUG_WARNING( strerror(errno) );
                } else {
                    _GONG_DEBUG_WARNING( Xtring::printf("Error converting '%s' to int, result='%d'",
                                                        c_str(), i ) );
                }
            }
#endif
        }
    }
    return i;
}

uint Xtring::toUInt ( bool *ok, int base ) const
{
    errno = 0;
    char *endptr;
    int i = strtoul ( c_str(), &endptr, base );
    if ( errno == 0 && ( *endptr == '\0' || *endptr == '.' ) )
    {
        if ( ok )
            *ok = true;
    }
    else
    {
        if ( ok )
            *ok = false;
        if( errno ) {
            _GONG_DEBUG_WARNING( strerror(errno) );
        } else {
            _GONG_DEBUG_WARNING( Xtring::printf("Error converting '%s' to int, result='%d'",
                                                c_str(), i ) );
        }
    }
    return i;
}

long long Xtring::toLong ( bool *ok, int base ) const
{
    errno = 0;
    char *endptr;
    long long l = strtoll ( c_str(), &endptr, base );
    if ( errno == 0 && ( *endptr == '\0' || *endptr == '.' ) )
    {
        if ( ok )
            *ok = true;
    }
    else
    {
        if ( ok )
            *ok = false;
        if( errno ) {
            _GONG_DEBUG_WARNING( strerror(errno) );
        } else {
            _GONG_DEBUG_WARNING( Xtring::printf("Error converting '%s' to int, result='%ld'",
                                                c_str(), l ) );
        }
    }
    return l;
}

unsigned long long Xtring::toULong ( bool *ok, int base ) const
{
    errno = 0;
    char *endptr;
    long l = strtol ( c_str(), &endptr, base );
    if ( errno == 0 && ( *endptr == '\0' || *endptr == '.' ) )
    {
        if ( ok )
            *ok = true;
    }
    else
    {
        if ( ok )
            *ok = false;
        if( errno ) {
            _GONG_DEBUG_WARNING( strerror(errno) );
        } else {
            _GONG_DEBUG_WARNING( Xtring::printf("Error converting '%s' to int, result='%ld'",
                                                c_str(), l ) );
        }
    }
    return l;
}


/**
 * Locale dependent conversion from string to double
 * @param ok
 * @return
 */
double Xtring::toDouble ( bool *ok ) const
{
    errno = 0;
    char *endptr;
    double d = strtod ( c_str(), &endptr );
    if ( errno == 0 && *endptr == '\0' ) {
        if( ok )
            *ok = true;
        return d;
    } else {
        d = toDoubleLocIndep( ok );
        if( !ok )
            _GONG_DEBUG_PRINT(2, Xtring::printf("Error converting '%s' to double, result='%f'",
                                                c_str(), d ) );
        return d;
    }
}

bool Xtring::startsWith ( const Xtring &str ) const
{
    Xtring::const_iterator strit = str.begin();
    Xtring::const_iterator thisit = begin();
    for ( ;
            strit != str.end() && thisit!= end();
            ++strit, ++thisit )
    {
        if ( *strit != *thisit )
            return false;
    }
    return strit == str.end();
}

bool Xtring::startsWith ( char ch ) const
{
    if ( size() )
        return ( at ( 0 ) == ch );
    else
        return false;
}

Xtring &Xtring::replace ( const Xtring &search, const Xtring &repl )
{
    int searchlen = search.length();
    if ( searchlen == 0 )
        return *this;
    int repllen = repl.length();
    size_type match = find ( search );
    while ( match != Xtring::npos )
    {
        erase ( match, searchlen );
        insert ( match, repl );
        match = find ( search, match + repllen );
    }
    return *this;
}

int Xtring::tokenize ( List<Xtring> &tokens, const Xtring& delims ) const
{
    int len = length();
    Xtring token;
    for ( int index = 0; index < len; index++ )
    {
        if ( delims.find ( at ( index ) ) != Xtring::npos )
        {
            tokens.push_back ( token );
            token.clear();
        }
        else
        {
            token += at ( index );
        }
    }
    if ( token.length() > 0 )
    {
        tokens.push_back ( token );
    }
    return tokens.size();
}

bool Xtring::toBool() const
{
    const char *origvalue = c_str();
    if ( !size() || strcasecmp ( origvalue, "0" ) == 0
            || strcasecmp ( origvalue, "false" ) == 0
            || strcasecmp ( origvalue, "no" ) == 0
            || strcasecmp ( origvalue, "off" ) == 0
            || strcasecmp ( origvalue, "f" ) == 0
            || strcasecmp ( origvalue, "n" ) == 0
            || strcasecmp ( origvalue, _ ( "false" ) ) == 0
            || strcasecmp ( origvalue, _ ( "no" ) ) == 0 )
        return false;
    return true;
}


Xtring Xtring::number ( long long val )
{
    char buffer[ 64 ];
    sprintf ( buffer, "%Ld", val );
    return Xtring ( buffer );
}

/**
 * Returns a string representation for the double value val, according to the current locale.
   If there are decimals, the result will have at least one decimal position.
	If not, it will not have any decimal at all (sprintf).
 * @param val
 * @return
 */
Xtring Xtring::number ( double val )
{
    char buffer[128];
    sprintf ( buffer, "%f", val );
    struct lconv *lc = localeconv();
    char *p;
    if( strchr(buffer, *lc->decimal_point ) ) {
        for ( p = buffer + strlen ( buffer ) - 1;
                ( p >= buffer ) && ( *p == '0' );
                --p );
        if( *p == *lc->decimal_point )
            *p = '\0';
        else
            *++p = '\0';
    }
    if( !strchr(buffer, *lc->decimal_point ) ) {
        strcat(buffer, lc->decimal_point);
        strcat(buffer, "0");
    }
    return buffer;
}

Xtring Xtring::right ( size_type n ) const
{
    if ( length() > n )
        return mid ( length() - n, n );
    else
        return *this;
}

Xtring &Xtring::padL ( size_type width, char space )
{
    if ( length() >= width )
        *this = right ( width );
    else
    {
        while ( length() < width )
            *this = space + *this;
    }
    return *this;
}

Xtring &Xtring::pad ( size_type width, char space )
{
    if ( length() >= width )
        *this = left ( width );
    else
    {
        while ( length() < width )
            *this += space;
    }
    return *this;
}

Xtring Xtring::printf ( const Xtring &format, ... )
{
    va_list args;
    char *buffer = new char[2040];
    va_start ( args, format );
    vsnprintf ( buffer, 2039, format.c_str(), args );
    va_end ( args );
    Xtring ret ( buffer );
    delete [] buffer;
    return ret;
}

/**
 * Increments or decrements a numeric or alphanumeric string
 * @param strorig
 * @param inc The increment step, can be negative
 * @return If strorig if numeric or ends with numbres, adds \a inc to it
 *         If strorig is not numeric adds \a inc letters as if numbers
 */
Xtring Xtring::stringInc ( const Xtring &strorig, int inc )
{
    uint i, nPosUltimoNumero;

    Xtring str = strorig;
    if ( str.trim().length() == 0 )
        return str;
    nPosUltimoNumero = 0;
    for ( i=0; i < str.length(); i++ )
    {
        if ( isdigit ( str.at ( i ) ) )
        {
            nPosUltimoNumero = i;
        }
    }
    if( i== 0)
        nPosUltimoNumero = str.length() - 1;
    for ( i=nPosUltimoNumero; ( int ) i>=0; i-- )
    {
        char ch = str.at ( i );
        if ( ( ch >= 'A' && ch < 'Z' ) || ( ch >= 'a' && ch < 'z' )
                || ( ch >= '0' && ch < '9' ) )
        {
            str[i] = char ( ( ( int ) ch ) +inc );
            break;
        }
        else
        {
            str[i] = '0';
        }
    }
    return str;
}


Xtring Xtring::stringDec ( const Xtring &str )
{
    return stringInc ( str, -1 );
}


bool Xtring::endsWith ( char ch ) const
{
    size_type pos = rfind ( ch );
    if ( pos == npos )
        return false;
    else
        return pos == size() - 1;
}

bool Xtring::endsWith ( const Xtring &search ) const
{
    size_type pos = rfind ( search );
    if ( pos == npos )
        return false;
    else
        return pos == size() - search.length();
}

Xtring Xtring::mid ( size_type pos, size_type n ) const
{
    if ( pos >= size() )
        return Xtring::null;
    else
        return substr ( pos, n>size() ?size() :n );
}


Xtring Xtring::toHex() const
{
    Xtring ret;
    static char hexdigits[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                                'A', 'B', 'C', 'D', 'E', 'F'
                              };
    for ( Xtring::const_iterator it = begin();
            it != end();
            ++it )
    {
        unsigned char ord = *it;
        ret += hexdigits[ ord / 16 ];
        ret += hexdigits[ ord % 16 ];
    }
    return ret;
}

Xtring Xtring::XMLProtect() const
{
    Xtring s = *this;
    s.replace ( "&", "&amp;" );
    s.replace ( ">", "&gt;" );
    s.replace ( "<", "&lt;" );
    s.replace ( "\"", "&quot;" );
    s.replace ( "\'", "&apos;" );
    return s;
}

double Xtring::toDoubleLocIndep ( bool * ok ) const
{
    double dec = 0.0, integer = 0.0, decexp = 1.0/10.0;
    if ( ok )
        *ok = true;
    bool beforepoint = true, bneg = false;
    for ( uint i = 0; i < size(); i++ )
    {
        char ch = at ( i );
        if ( ch == '-' )
            bneg = !bneg;
        else if ( ch == '.' || ch == ',' )
            beforepoint = false;
        else if ( ch < '0' || ch > '9' )
        {
            if ( ok )
                *ok = false;
        }
        else if ( beforepoint )
            integer = integer * 10 + ( ch - '0' );
        else
        {
            dec = dec + decexp * double ( ch - '0' );
            decexp /= 10;
        }
    }
    if ( bneg )
        return  - ( integer + dec );
    else
        return integer + dec;
}

Xtring Xtring::last(size_type n) const
{
    if( n >= size() )
        return *this;
    else
        return right( n );
}


int Xtring::splitIn2( Xtring &first, Xtring &second, const Xtring &sep) const
{
    size_t pos = find(sep);
    if( pos != Xtring::npos ) {
        first = left( pos );
        second = mid( pos + sep.length() );
        return 2;
    } else {
        first = *this;
        second = Xtring::null;
        return 1;
    }
}

/**
 * @brief appends text to a string, appending a separator if not empty
 * 
 * @param appendwhat 
 * @param separator 
 * @return gong::Xtring&
 */
Xtring &Xtring::appendWithSeparator(const Xtring &appendwhat, const Xtring& separator)
{
	if( !isEmpty() )
		append( separator );
	append( appendwhat );
	return *this;
}


} // namespace gong




