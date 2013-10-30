#ifndef RTKINPUTXML_H
#define RTKINPUTXML_H

/*<<<<<COPYLEFT*/
/** @file rtkinputxml.h Standard xml input object
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
#include "rtkinput.h"

namespace gong {}
namespace RTK = gong;

namespace gong {

class InputXml : public Input {
public:
    InputXml(class Report &r, const char *name) : Input(r, name, "xml") {};
    ~InputXml();

    virtual bool init(); // from Input
    virtual bool next(); // from Input
    virtual Variant getValue(uint i, Variant::Type type) const; // from Input

};

} // namespace

#endif


