/*<<<<<COPYLEFT*/
/** @file rtkcolor.h Color class
 * RTK The report toolkit. Copyright (C) 2003-2013 Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */
/*>>>>>COPYLEFT*/

#ifndef RTKCOLOR_H
#define RTKCOLOR_H

#include <cstdlib>
#include <cstdio>

namespace gong {}
namespace RTK = gong;

namespace gong {

class Color
{
public:
    Color(int hexcode = 0)
        : mCode(hexcode) {}
    Color(unsigned char r, unsigned char g, unsigned char b)
        : mCode ((r << 16) | (g << 8 ) | b) {}
    Color(const Color &other)
        : mCode(other.mCode) {}
    Color(const char *name);

    inline bool isValid() const {
        return mCode != Color::Invalid;
    }
    inline unsigned char red() const {
        return (mCode & 0x00ff0000) >> 16;
    }
    inline unsigned char green() const {
        return (mCode & 0x0000ff00) >> 8;
    }
    inline unsigned char blue() const {
        return (mCode & 0x000000ff);
    }
    inline void setRed(unsigned char r) {
        mCode &= 0x0000ffff;
        mCode |= (r<<16);
    }
    inline void setGreen(unsigned char g) {
        mCode &= 0x00ff00ff;
        mCode |= (g<<8);
    }
    inline void setBlue(unsigned char b) {
        mCode &= 0x00ffff00;
        mCode |= b;
    }

    bool operator==(uint other) const {
        return mCode == other;
    }
    bool operator!=(uint other) const {
        return mCode != other;
    }
    bool operator==(const Color &other) const
    {
        return mCode == other.mCode;
    }
    bool operator!=(const Color &other) const
    {
        return mCode != other.mCode;
    }

    static const uint Black;
    static const uint White;
    static const uint Red;
    static const uint Green;
    static const uint Blue;
    static const uint Cyan;
    static const uint Magenta;
    static const uint Yellow;
    static const uint Gray;
    static const uint Invalid;

    const char *name() const;
    uint toUInt() const {
        return mCode;
    }

private:
    uint mCode;
};


} // namespace RTK

#endif // RTKCOLOR
