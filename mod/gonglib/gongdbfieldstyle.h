#ifndef ___GONGDBFIELDSTYLE_H
#define ___GONGDBFIELDSTYLE_H

/** @file gongdbfieldstyle.h Field style definitions
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
#include "gongvariant.h"
#include "gongdictionary.h"

namespace gong {

class dbFieldStyle
{
public:
    enum Alignment {
        AlignAuto, AlignLeft, AlignRight, AlignHCenter,
        AlignCenter = AlignHCenter, AlignJustify,
        AlignTop, AlignBottom, AlignVCenter, AlignMiddle = AlignVCenter
    };
    dbFieldStyle(const Xtring& name, int width = 0, bool visible = true,
                 const Xtring& format = Xtring::null, const Xtring& mask = Xtring::null,
                 const Alignment& halignment = AlignAuto, const Alignment& valignment = AlignAuto)
        : mName(name), mWidth(width), mVisible(visible),
          mFormat(format), mMask(mask), mHAlignment(halignment), mVAlignment(valignment)
    {
    }

public:
    static Alignment fromString(const Xtring &str);

    const Xtring& getName() const {
        return mName;
    }
    unsigned int getWidth() const {
        return mWidth;
    }
    bool isVisible() const {
        return mVisible;
    }
    const Xtring& getFormat() const {
        return mFormat;
    }
    const Xtring& getMask() const {
        return mMask;
    }
    const Alignment& getHAlignment() const {
        return mHAlignment;
    }
    const Alignment& getVAlignment() const {
        return mVAlignment;
    }

    void setWidth(unsigned int width) {
        mWidth = width;
    }
    void setVisible(bool visible) {
        mVisible = visible;
    }
    void setFormat(const Xtring& format) {
        mFormat = format;
    }
    void setMask(const Xtring& mask) {
        mMask = mask;
    }
    void setHAlignment(const Alignment& halignment) {
        mHAlignment = halignment;
    }
    void setVAlignment(const Alignment& valignment) {
        mVAlignment = valignment;
    }
private:
    Xtring mName;
    unsigned int mWidth;
    bool mVisible;
    Xtring mFormat;
    Xtring mMask;
    Alignment mHAlignment;
    Alignment mVAlignment;
};

typedef Dictionary<dbFieldStyle *> dbFieldStyleDict;

} // namespace gong
#endif // __GONGDBFIELDSTYLE_H

