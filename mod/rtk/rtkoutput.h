#ifndef RTKOUTPUT_H
#define RTKOUTPUT_H

/*<<<<<COPYLEFT*/
/** @file rtkoutput.h RTK output object
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

#include <gongxtring.h>
#include "rtkproperty.h"

namespace gong {}
namespace RTK = gong;

namespace gong {

class Object;
class Section;
class Report;

class Output {
public:
    Output(class Report &r, Measure unitsperinchx = 72, Measure unitsperinchy = 72,
           PaperSize papersize = A4, PageOrientation pageorientation = Portrait, Measure sizex = 0,
           Measure sizey = 0, Measure marginleft = 0, Measure marginright = 0,
           Measure margintop = 0, Measure marginbottom = 0)
        : propUnitsPerInchX(unitsperinchx),
          propUnitsPerInchY(unitsperinchy), propPaperSize(papersize),
          propPageOrientation(pageorientation), propSizeX(sizex),
          propSizeY(sizey), propMarginLeft(marginleft),
          propMarginRight(marginright), propMarginTop(margintop),
          propMarginBottom(marginbottom),
          mReport(r)
    {
        initOutput();
    }
public:
    virtual ~Output() {}
    virtual int startReport();
    virtual int endReport();
    virtual Measure startPage();
    virtual Measure endPage();
    virtual Measure startSection(const Section &section);
    virtual Measure endSection(const Section &section);
    virtual Measure printObject(const Object &object);
    virtual Measure setPosX(Measure newx);
    virtual Measure setPosY(Measure newy);
    virtual bool sectionFits(const Section *, Measure /*nextsectheight*/) const;
    virtual const char *encoding() const;

    void initPaperSizes();
    Measure getFolioSizeY() const {
        return mFolioSizeY;
    }
    int getCurrentPage() {
        return mCurrentPage;
    }
    int getTotalPages() {
        return mTotalPages;
    }
    void setUnitsPerInchX(const Measure &ppix) {
        propUnitsPerInchX.set(ppix);
    }
    void setUnitsPerInchY(const Measure &ppiy) {
        propUnitsPerInchY.set(ppiy);
    }

    void setPageOrientation( const PageOrientation po ) {
        propPageOrientation.set( po );
    }
    /*<<<<<OUTPUT_PROPERTIES*/
public:
    Measure unitsPerInchX() const {
        return propUnitsPerInchX.get();
    }
    Measure unitsPerInchY() const {
        return propUnitsPerInchY.get();
    }
    PaperSize paperSize() const {
        return propPaperSize.get();
    }
    PageOrientation pageOrientation() const {
        return propPageOrientation.get();
    }
    Measure sizeX() const {
        return propSizeX.get();
    }
    Measure sizeY() const {
        return propSizeY.get();
    }
    Measure marginLeft() const {
        return propMarginLeft.get();
    }
    Measure marginRight() const {
        return propMarginRight.get();
    }
    Measure marginTop() const {
        return propMarginTop.get();
    }
    Measure marginBottom() const {
        return propMarginBottom.get();
    }
    const char * defaultFontFamily() const {
        return propDefaultFontFamily.get();
    }
    double defaultFontSize() const {
        return propDefaultFontSize.get();
    }
    int defaultFontWeight() const {
        return propDefaultFontWeight.get();
    }
    bool defaultFontItalic() const {
        return propDefaultFontItalic.get();
    }
    double defaultMinFontSize() const {
        return propDefaultMinFontSize.get();
    }
    Adjustment defaultAdjustment() const {
        return propDefaultAdjustment.get();
    }
    Color defaultFontColor() const {
        return propDefaultFontColor.get();
    }
    Color defaultBackColor() const {
        return propDefaultBackColor.get();
    }
    Color defaultBorderColor() const {
        return propDefaultBorderColor.get();
    }
    int defaultBorderWidth() const {
        return propDefaultBorderWidth.get();
    }
    Color defaultBorderTopColor() const {
        return propDefaultBorderTopColor.get();
    }
    int defaultBorderTopWidth() const {
        return propDefaultBorderTopWidth.get();
    }
    Color defaultBorderBottomColor() const {
        return propDefaultBorderBottomColor.get();
    }
    int defaultBorderBottomWidth() const {
        return propDefaultBorderBottomWidth.get();
    }
    Color defaultBorderLeftColor() const {
        return propDefaultBorderLeftColor.get();
    }
    int defaultBorderLeftWidth() const {
        return propDefaultBorderLeftWidth.get();
    }
    Color defaultBorderRightColor() const {
        return propDefaultBorderRightColor.get();
    }
    int defaultBorderRightWidth() const {
        return propDefaultBorderRightWidth.get();
    }

public:
    Property<Measure> propUnitsPerInchX;
    Property<Measure> propUnitsPerInchY;
    Property<PaperSize> propPaperSize;
    Property<PageOrientation> propPageOrientation;
    Property<Measure> propSizeX;
    Property<Measure> propSizeY;
    Property<Measure> propMarginLeft;
    Property<Measure> propMarginRight;
    Property<Measure> propMarginTop;
    Property<Measure> propMarginBottom;
    Property<const char *> propDefaultFontFamily;
    Property<double> propDefaultFontSize;
    Property<int> propDefaultFontWeight;
    Property<bool> propDefaultFontItalic;
    Property<double> propDefaultMinFontSize;
    Property<Adjustment> propDefaultAdjustment;
    Property<Color> propDefaultFontColor;
    Property<Color> propDefaultBackColor;
    Property<Color> propDefaultBorderColor;
    Property<int> propDefaultBorderWidth;
    Property<Color> propDefaultBorderTopColor;
    Property<int> propDefaultBorderTopWidth;
    Property<Color> propDefaultBorderBottomColor;
    Property<int> propDefaultBorderBottomWidth;
    Property<Color> propDefaultBorderLeftColor;
    Property<int> propDefaultBorderLeftWidth;
    Property<Color> propDefaultBorderRightColor;
    Property<int> propDefaultBorderRightWidth;
    /*>>>>>OUTPUT_PROPERTIES*/

protected:
    int clipMeasures(const Object &object, int *x, int *y, int *w, int *h) const;
    class Report &mReport;
    static PaperSize mPaperSizes[];
    int mCurrentPage, mTotalPages, mCurrentColumn, mCurrentPageInFolio;
    Measure mCurrX, mCurrY, mGrowthY, mColumnCurrY, mFolioSizeY;
private:
    void initOutput();

#ifdef _GONG_DEBUG
protected:
    virtual Xtring debugPrint(const Xtring &file, const Xtring &function, int nline) const;
#endif


};

} // namespace

#endif
