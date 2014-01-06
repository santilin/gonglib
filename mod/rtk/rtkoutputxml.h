#ifndef RTKOUTPUTXML_H
#define RTKOUTPUTXML_H

/*<<<<<COPYLEFT*/
/** @file rtkoutputxml.h RTK XML output object
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

#include "rtkoutput.h"

namespace gong {}
namespace RTK = gong;

namespace gong {

class OutputXml : public Output
{
public:
    OutputXml(class Report &r, const char *filename, const char *encoding = 0,
              Measure unitsperinchx = 72, Measure unitsperinchy = 72,
              PaperSize papersize = A4, PageOrientation pageorientation = Portrait, Measure sizex = 0,
              Measure sizey = 0, Measure marginleft = 0, Measure marginright = 0,
              Measure margintop = 0, Measure marginbottom = 0)
        :Output(r, unitsperinchx, unitsperinchy,
                papersize, pageorientation, sizex,
                sizey, marginleft, marginright,
                margintop, marginbottom)
        , propFileName(filename), propEncoding(encoding)
    {};

    ~OutputXml() {};
    int startReport();
    int endReport();
    Measure startPage() {
        return 1;
    }
    Measure endPage() {
        return 1;
    }
    Measure startSection(const Section &section);
    Measure endSection(const Section &section);
    Measure printObject(const Object &object);

    /*<<<<<OUTPUTXML_PROPERTIES*/
public:
	const char * fileName() const { return propFileName.get(); }
	const char * encoding() const { return propEncoding.get(); }

public:
	Property<const char *> propFileName;
	Property<const char *> propEncoding;
/*>>>>>OUTPUTXML_PROPERTIES*/

private:
    std::ofstream *pFile;
};

} // namespace

#endif

