#ifndef RTKOUTPUTODFCALC1_H
#define RTKOUTPUTODFCALC1_H

/*<<<<<COPYLEFT*/
/** @file rtkoutputodfcalc1.h RTK OpenDocument 1 Calc  output object
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

class OutputODFCalc1 : public Output
{
public:
    enum OOCreateOptions { CREATE, REPLACE_CONTENT, ADD_FIRST_SHEET, ADD_LAST_SHEET, REPLACE_FIRST_SHEET, REPLACE_LAST_SHEET };
public:
    OutputODFCalc1(class Report &r, const char *filename, const char *encoding = 0,
                   const char *templatepath = 0, const OOCreateOptions& replacecontent = CREATE,
                   Measure unitsperinchx = 72, Measure unitsperinchy = 72,
                   PaperSize papersize = A4, PageOrientation pageorientation = Portrait, Measure sizex = 0,
                   Measure sizey = 0, Measure marginleft = 0, Measure marginright = 0,
                   Measure margintop = 0, Measure marginbottom = 0)
        :Output(r, unitsperinchx, unitsperinchy,
                papersize, pageorientation, sizex,
                sizey, marginleft, marginright,
                margintop, marginbottom),
        propFileName(filename), propEncoding(encoding),
        propTemplatePath(templatepath), propReplaceContent(replacecontent),
        mDetailsFieldCount(0) {}

    ~OutputODFCalc1() {}
    int startReport();
    int endReport();
    Measure startPage();
    Measure endPage();
    Measure startSection( const Section &section );
    Measure endSection( const Section &section );
    Measure printObject( const Object &object );
    Measure setPosX( Measure newx );
    Measure setPosY( Measure newy );
    virtual bool sectionFits(const Section *, Measure /*nextsectheight*/) const // from Output
    {
        return true;    // No division by pages
    }

    /*<<<<<OUTPUTOPENDOCUMENT_PROPERTIES*/
public:
    const char * fileName() const {
        return propFileName.get();
    }
    const char * encoding() const {
        return propEncoding.get();
    }
    const char * templatePath() const {
        return propTemplatePath.get();
    }
    OOCreateOptions replaceContent() const {
        return propReplaceContent.get();
    }

public:
    Property<const char *> propFileName;
    Property<const char *> propEncoding;
    Property<const char *> propTemplatePath;
    Property<OOCreateOptions> propReplaceContent;
    /*>>>>>OUTPUTOPENDOCUMENT_PROPERTIES*/

private:
    int mDetailsFieldCount;
    int mIntSizeX, mIntSizeY;
    Xtring getOfficeTypeAndValue( const Object &obj ) const;
    std::ofstream *pFile;
};

} // namespace

#endif

