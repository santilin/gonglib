#ifndef RTKOUTPUTPS_H
#define RTKOUTPUTPS_H

/*<<<<<COPYLEFT*/
/** @file rtkoutputps.h RTK standard PostScript output object
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

#include <gongiconv.h>
#include "rtkbaseimage.h"
#include "rtkoutput.h"


namespace gong {}
namespace RTK = gong;

namespace gong {

class OutputPS : public Output
{
public:
    OutputPS(class Report &r, const char *filename, Measure unitsperinchx = 72,
             Measure unitsperinchy = 72,
             PaperSize papersize = A4, PageOrientation pageorientation = Portrait, Measure sizex = 0,
             Measure sizey = 0, Measure marginleft = 0, Measure marginright = 0,
             Measure margintop = 0, Measure marginbottom = 0);
    ~OutputPS();

    virtual int startReport(); // from Output
    virtual int endReport(); // from Output
    virtual Measure startSection(const Section &section); // from Output
    virtual Measure endSection(const Section &section); // from Output
    virtual Measure startPage(); // from Output
    virtual Measure endPage(); // from Output
    virtual Measure printObject(const Object &object); // from Output

    /*<<<<<OUTPUTPS_PROPERTIES*/
public:
	const char * fileName() const { return propFileName.get(); }

public:
	Property<const char *> propFileName;
/*>>>>>OUTPUTPS_PROPERTIES*/


private:
    std::ofstream *pFile;
    void drawPixmap(const BaseImage *bi, int x0, int y0, int width, int height);
    Xtring mLastFontFamily;
    Measure mLastFontSize;
    int mLastFontWeight;
    bool mLastFontItalic;
    Color mLastBackColor;
    Color mLastFontColor;
    IConv *pIConverter;
    bool mSectionStarted;
};

} // namespace

#endif

