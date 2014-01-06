#ifndef RTKOUTPUTTEXT_H
#define RTKOUTPUTTEXT_H

/*<<<<<COPYLEFT*/
/** @file rtkoutputtext.h RTK standard text output object
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
#include <fstream>

#include "rtkoutput.h"

namespace gong {}
namespace RTK = gong;

namespace gong {

class OutputText : public Output
{
public:
    OutputText(class Report &r, const char *filename, Measure unitsperinchx = 72, Measure unitsperinchy = 72,
               PaperSize papersize = A4, PageOrientation pageorientation = Portrait, Measure sizex = 0,
               Measure sizey = 0, Measure marginleft = 0, Measure marginright = 0,
               Measure margintop = 0, Measure marginbottom = 0)
        :Output(r, unitsperinchx, unitsperinchy,
                papersize, pageorientation, sizex,
                sizey, marginleft, marginright,
                margintop, marginbottom)
        , propFileName(filename)
    {};

    ~OutputText() {};
    int startReport();
    int endReport();
    Measure startPage();
    Measure endPage();
    Measure startSection(const Section &section);
    Measure endSection(const Section &section);
    Measure printObject(const Object &object);

    /*<<<<<OUTPUTTEXT_PROPERTIES*/
public:
	const char * fileName() const { return propFileName.get(); }

public:
	Property<const char *> propFileName;
/*>>>>>OUTPUTTEXT_PROPERTIES*/

private:
    std::ofstream *pFile;
    char *mPageBuf;
    int mIntSizeX, mIntSizeY;
    void internal_print(const char *text, int x, int y, int len);
};

} // namespace

#endif // RTKOUTPUTTEXT_H
