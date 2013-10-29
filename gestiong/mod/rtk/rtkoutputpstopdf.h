#ifndef RTKOUTPUTPSTOPDF_H
#define RTKOUTPUTPSTOPDF_H

/*<<<<<COPYRIGHT*/
/** @file rtkoutputpstopdf.h RTK standard PostScript to PDF output object
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
#include "rtkoutputps.h"


namespace gong {}
namespace RTK = gong;

namespace gong {

class OutputPStoPDF : public OutputPS
{
public:
    OutputPStoPDF(const char *pathtopstofpd, class Report &r,
                  const char *filename, Measure unitsperinchx = 72, Measure unitsperinchy = 72,
                  PaperSize papersize = A4, PageOrientation pageorientation = Portrait, Measure sizex = 0,
                  Measure sizey = 0, Measure marginleft = 0, Measure marginright = 0,
                  Measure margintop = 0, Measure marginbottom = 0);

    int endReport();
private:
    const char *pPathPSToPDF;
    Xtring mFileNamePDF;
};

} // namespace

#endif

