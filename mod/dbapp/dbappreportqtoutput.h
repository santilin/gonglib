#ifndef _DBAPP_REPORTQTOUTPUT_H
#define _DBAPP_REPORTQTOUTPUT_H

/** @file dbappreportqtoutput.h A rtk output to show a record in a QPainter
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include <QPicture>
#include <QPainter>
#include "rtkoutput.h"

namespace gong {

typedef std::vector<QPicture *> QtPageCollection;

class ReportQtOutput : public Output
{
public:
    ReportQtOutput(class Report &r, Measure unitsperinchx, Measure unitsperinchy,
                   PaperSize papersize = A4, PageOrientation pageorientation = Portrait, Measure sizex = 0,
                   Measure sizey = 0, Measure marginleft = 0, Measure marginright = 0,
                   Measure margintop = 0, Measure marginbottom = 0)
        :Output(r, unitsperinchx, unitsperinchy,
                papersize, pageorientation, sizex,
                sizey, marginleft, marginright,
                margintop, marginbottom)
    {
        initReportQtOutput();
    };
    ReportQtOutput(class Report &r, QPaintDevice *d, PaperSize papertype = A4,
                   PageOrientation pageorientation = Portrait,
                   Measure sizex = 0, Measure sizey = 0, Measure marginleft = 0,
                   Measure marginright = 0, Measure margintop = 0, Measure marginbottom = 0)
        :Output(r, 0, 0, papertype, pageorientation,
                sizex, sizey, marginleft,
                marginright, margintop, marginbottom)
        , mPainter(d)
    {
        initReportQtOutput();
    }

    ~ReportQtOutput() {};
    virtual int startReport(); // from Output
    virtual int endReport(); // from Output
    virtual Measure startPage();  // from Output
    virtual Measure endPage();  // from Output
    virtual Measure startSection(const Section &section);  // from Output
    virtual Measure endSection(const Section &section);  // from Output
    virtual Measure printObject(const Object &object);  // from Output
    virtual const char *encoding() const {
        return "ISO-8859-1";    // from Output
    }

    QPicture *getFirstPage() const;
    const QtPageCollection *getPageCollection() const
    {
        return &mPageCollection;
    }

protected:
    QtPageCollection mPageCollection;
    QPainter mPainter;
    QBrush mSavedBrush;
private:
    void initReportQtOutput();
    void drawGraphics( const Object &object, int x, int y, int width, int height, bool beforetext );
};

} // namespace

#endif // _DBAPP_REPORTQTOUTPUT_H

