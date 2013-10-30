#ifndef _GONG_XTRING_H
#define _GONG_XTRING_H

/** @file gongxtring.h Xtring object, derived from std::string
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "gonglist.h"

namespace gong {

class Xtring: public std::string
{
public:
    Xtring(): std::string() {}
    Xtring(int count, char ch): std::string( count, ch ) {}
    Xtring(const std::string &astr): std::string(astr) {}
    Xtring(const char *astr): std::string(astr?astr:"") {}
    Xtring(const char *astr, size_t asize)
        : std::string(astr?astr:"", asize) {}
    Xtring(const char *astr, size_t pos, size_t asize)
        : std::string(astr?astr:"", pos, asize) {}
    Xtring(const unsigned char *astr)
        : std::string(astr?(const char *)(astr):"") {} // for xmlChar
    static Xtring null;

    Xtring &replace(const Xtring &search, const Xtring &repl);
    bool endsWith(char ch) const;
    bool endsWith(const Xtring &search) const;
    Xtring &remove_white_space();
    Xtring &simplify_white_space(char usethis = ' ');
    Xtring &trimLeft(const Xtring &whitespace = " \n\r\t");
    Xtring trimLeft(const Xtring &whitespace = " \n\r\t") const;
    Xtring &trimRight(const Xtring &whitespace = " \n\r\t");
    Xtring trimRight(const Xtring &whitespace = " \n\r\t") const;
    Xtring &trim(const Xtring &whitespace = " \n\r\t");
    Xtring trim(const Xtring &whitespace = " \n\r\t") const;
    Xtring upper() const;
    Xtring lower() const;
    Xtring proper() const;
    Xtring unproper() const;

    bool toBool() const;
    int toInt(bool *ok = 0, int base = 10) const;
    uint toUInt(bool *ok = 0, int base = 10) const;
    long long toLong(bool *ok = 0, int base = 10) const;
    unsigned long long toULong(bool *ok = 0, int base = 10) const;
    double toDouble(bool *ok = 0) const;
    double toDoubleLocIndep(bool *ok = 0) const;
    bool startsWith(const Xtring &str) const;
    bool startsWith(char ch) const;
    int tokenize( List<Xtring> &tokens, const Xtring& delims ) const;
    int splitIn2( Xtring &first, Xtring &second, const Xtring &sep) const;
    bool isEmpty() const {
        return size()==0;
    }
    Xtring left(size_type n) const {
        return substr(0, n>size()?size():n);
    }
    Xtring right(size_type n) const;
    Xtring last(size_type n = 1) const;
    Xtring mid(size_type pos, size_type n = npos) const;
    Xtring &padL(size_type width, char ch=' ');
    Xtring &pad(size_type width, char ch=' ');
    static Xtring number(int i) {
        return number(static_cast<long long>(i));
    }
    static Xtring number(uint i) {
        return number(static_cast<long long>(i));
    }
    static Xtring number(long l) {
        return number(static_cast<long long>(l));
    }
    static Xtring number(unsigned long l) {
        return number(static_cast<long long>(l));
    }
    static Xtring number(long long l);
    static Xtring number(double d);
    static Xtring printf(const Xtring &format, ... );
    static Xtring stringInc(const Xtring &str, int inc=1);
    static Xtring stringDec(const Xtring &str);
    Xtring toHex() const;
    Xtring XMLProtect() const;
};

typedef List<Xtring> XtringList;

template<> inline
std::string List<Xtring>::join(const std::string &sep) const
{
    std::string result;
    for( const_iterator it = begin(); it != end(); ++it ) {
        if( result.size() )
            result += sep;
        result += *it;
    }
    return result;
}

inline bool strempty(const char *str) {
    return (!str || !*str);
}

} // namespace gong

#endif // _GONG_XTRING_H

