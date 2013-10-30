#ifndef _GONG_ICONV_H
#define _GONG_ICONV_H

/** @file gongiconv.h Charset conversions for strings
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include <iconv.h>
#include "gongxtring.h"

namespace gong {

class IConv
{
    Xtring mToCode;
    Xtring mFromCode;
    iconv_t cd;
    IConv &operator =(const IConv &);
    IConv(const IConv &);
public:
    IConv(const Xtring& tocode, const Xtring& fromcode);
    ~IConv();
    const Xtring& getToCode() const            {
        return mToCode;
    }
    const Xtring& getFromCode() const          {
        return mFromCode;
    }
    Xtring convert(const Xtring &str);
    static const char *canonicalCodeName( const char *alias);
};

} // namespace gong
#endif
