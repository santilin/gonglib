#include <cmath>
#include <gongdebug.h>
#include "rtkoutput.h"
#include "rtksection.h"
#include "rtkreport.h"

namespace gong {

#define MM(n) int((n * 720 + 127) / 254)
#define IN(n) int(n * 72)

static struct {
    int width, height;
} paperSizes[NPageSize] = {
    {  MM(210), MM(297) },      // A4
    {  MM(176), MM(250) },      // B5
    {  IN(8.5), IN(11) },       // Letter
    {  IN(8.5), IN(14) },       // Legal
    {  IN(7.5), IN(10) },       // Executive
    {  MM(841), MM(1189) },     // A0
    {  MM(594), MM(841) },      // A1
    {  MM(420), MM(594) },      // A2
    {  MM(297), MM(420) },      // A3
    {  MM(148), MM(210) },      // A5
    {  MM(105), MM(148) },      // A6
    {  MM(74), MM(105)},        // A7
    {  MM(52), MM(74) },        // A8
    {  MM(37), MM(52) },        // A9
    {  MM(1000), MM(1414) },    // B0
    {  MM(707), MM(1000) },     // B1
    {  MM(31), MM(44) },        // B10
    {  MM(500), MM(707) },      // B2
    {  MM(353), MM(500) },      // B3
    {  MM(250), MM(353) },      // B4
    {  MM(125), MM(176) },      // B6
    {  MM(88), MM(125) },       // B7
    {  MM(62), MM(88) },        // B8
    {  MM(44), MM(62) },        // B9
    {  MM(162),    MM(229) },   // C5E
    {  IN(4.125),  IN(9.5) },   // Comm10E
    {  MM(110),    MM(220) },   // DLE
    {  IN(8.5),    IN(13) },    // Folio
    {  IN(17),     IN(11) },    // Ledger
    {  IN(11),     IN(17) }     // Tabloid
};

const char *Output::encoding() const
{
    return "";
}

void Output::initOutput()
{
    propDefaultFontSize = 10;
    propDefaultFontWeight = 50;
    propDefaultFontItalic = false;
    propDefaultMinFontSize = 8;
    propDefaultAdjustment = AdjustTrim;
    propDefaultFontColor = Color::Black;
    propDefaultBackColor = Color::White;
    propDefaultBorderColor = Color::Black;
    propDefaultBorderWidth = 0;
    propDefaultBorderTopColor = Color::Black;
    propDefaultBorderTopWidth = 0;
    propDefaultBorderBottomColor = Color::Black;
    propDefaultBorderBottomWidth = 0;
    propDefaultBorderLeftColor = Color::Black;
    propDefaultBorderLeftWidth = 0;
    propDefaultBorderRightColor = Color::Black;
    propDefaultBorderRightWidth = 0;
}

void Output::initPaperSizes()
{
    if( propPaperSize == Custom && (propSizeX <= 0 || propSizeY <= 0) )
        propPaperSize = A4;
    if( propPaperSize == Custom ) {
        propSizeX = (sizeX() / mmperinch) * unitsPerInchX();
        propSizeY = (sizeY() / mmperinch) * unitsPerInchY();
        paperSizes[paperSize()].width = (int)sizeX();
        paperSizes[paperSize()].height = (int)sizeY();
    } else {
        // If the screen resolution is not at 72 inches, we have to adjust the paper size
        propSizeX = paperSizes[PaperSize()].width / 72 * unitsPerInchX();
        propSizeY = paperSizes[PaperSize()].height / 72 * unitsPerInchY();
    }
    if( propPageOrientation == Landscape ) {
        Measure swap = sizeX();
        propSizeX = sizeY();
        propSizeY = swap;
    }
    _GONG_DEBUG_PRINT(3, Xtring::printf("Paper Size: %f, %f", sizeX(), sizeY()) );
}

Measure Output::setPosX(Measure newx) {
    Measure oldx = mCurrX;
    mCurrX = newx;
    return oldx;
}

Measure Output::setPosY(Measure newy) {
    Measure oldy = mCurrY;
    mCurrY = newy;
    return oldy;
}

int Output::startReport()
{
    mTotalPages = mCurrentPage = mCurrentColumn = 0;
    mCurrentPageInFolio = 1;
    mFolioSizeY = propSizeY.get();
    if( mReport.pagesPerFolio() > 1 )
        propSizeY.set( propSizeY.get() / mReport.pagesPerFolio() );
    mCurrY = mColumnCurrY = 0;
    return 0;
}

int Output::endReport()
{
    return 0;
}

Measure Output::startPage()
{
    mTotalPages++;
    mCurrentPage++;
    mCurrentColumn = 1;
    mGrowthY = 0;
    mCurrX = marginLeft();
    mCurrY = mColumnCurrY = ( mCurrY + marginTop() );
    return 0;
}

Measure Output::endPage()
{
    if( mReport.pagesPerFolio() > 1 ) {
        if(	mCurrentPageInFolio >= mReport.pagesPerFolio() ) {
            mCurrentPageInFolio = 1;
            mCurrY = mColumnCurrY = 0;
        } else {
            mCurrY = sizeY() * mCurrentPageInFolio;
            mCurrentPageInFolio ++;
        }
    } else {
        mCurrentPageInFolio = 1;
        mCurrY = mColumnCurrY = 0;
    }
    return 0;
}

Measure Output::startSection(const Section &section)
{
    mCurrY += section.posY() + section.marginTop();
    mCurrX += section.posX() + section.marginLeft();
    mColumnCurrY = mCurrY;
    _GONG_DEBUG_PRINT(3, Xtring::printf("Printing section '%s' at x=%f, y=%f, w=%f, h=%f",
                                        section.name(), mCurrX, mCurrY, section.sizeX(), section.sizeY() ) );
    return 0;
}

Measure Output::endSection(const Section &section)
{
    if( section.columns() > 1 ) {
        mCurrentColumn++;
        if( mCurrentColumn == (int)section.columns() ) {
            mCurrentColumn = 0;
            mCurrX = marginLeft();
            mCurrY += section.sizeY()  + section.marginBottom() + mGrowthY;
        } else {
            mCurrY = mColumnCurrY;
            mCurrX += section.sizeX() + section.marginLeft() + section.marginRight();
        }
    } else {
        mCurrY += section.sizeY()  + section.marginBottom() + mGrowthY;
        // For the next subsection to start in the same x-pos
        mCurrX -= ( section.posX() + section.marginLeft() );
    }
    return 0;
}

Measure Output::printObject(const Object &)
{
    return 0;
}

bool Output::sectionFits( const Section *section,
                          Measure nextsectionheight ) const
{
    if( sizeY() == 0 ) // OutputCached
        return true;
    bool fits = mCurrY + section->sizeY() + nextsectionheight <= (mCurrentPageInFolio * sizeY()) - marginBottom();
    if( !fits )
        _GONG_DEBUG_PRINT(3, Xtring::printf("Section %s doesn't fit:curry=%f, section.fixedsizey=%f, propSizeY=%f, mMarginBottom=%f",
                                            section->name(), mCurrY, section->sizeY(), sizeY(), marginBottom()) );
    return fits;
}

int Output::clipMeasures(const Object &object, int *x, int *y, int *w, int *h) const
{
    Measure realPosX = mCurrX + object.posX();
    Measure realPosY = mCurrY + object.posY();
    if( !object.isSection() ) {
        // If this was a section, the margins would have been added to mCurrY and mCurrX in startSection
        realPosX += object.marginLeft();
        realPosY += object.marginTop();
    }
    Measure realSizeX = object.sizeX();
    // Problem: If the section has grown
    Measure realSizeY = object.sizeY();

    if ( realPosX > sizeX() - marginRight()
            || realPosY > mFolioSizeY - marginBottom() ) {
        _GONG_DEBUG_PRINT(4, Xtring::printf("Object %s starts beyond the margin", object.name()) );
        return 0; // Out of the page
    }
    if ( realPosX + realSizeX > sizeX() - object.marginRight() - marginRight() ) {
        _GONG_DEBUG_PRINT(4, Xtring::printf("%s's width cropped from %f to %f",
                                            object.name(), realSizeX, sizeX() - marginRight() - object.marginRight() - realPosX ) );
        realSizeX = sizeX() - marginRight() - object.marginRight() - realPosX;
    }
    if ( realPosY + realSizeY > mFolioSizeY - object.marginBottom() - marginBottom()) {
        if( mReport.recordNumber() == 1 )
            _GONG_DEBUG_WARNING(Xtring::printf("%s's height cropped from %f to %f",
                                               object.name(), realSizeY,
                                               mFolioSizeY - marginBottom() - object.marginBottom() - realPosY ) );
        realSizeY = mFolioSizeY - marginBottom() - object.marginBottom() - realPosY ;
    }
    *x = int(floor(realPosX)); // + object.marginLeft()));
    *w = int(ceil(realSizeX)); //  - object.marginLeft() - object.marginRight()));
    *y = int(floor(realPosY)); // + object.marginTop()));
    *h = int(ceil(realSizeY)); // - object.marginTop() - object.marginBottom()));
    return 0;
}

#ifdef _GONG_DEBUG
Xtring Output::debugPrint(const Xtring &file, const Xtring &function, int line) const
{
    return file + ":" + Xtring::number(line) + ':' + function + ": ";
}
#endif

}
