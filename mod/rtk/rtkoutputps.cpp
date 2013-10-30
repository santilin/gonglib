#include <cmath>
#include "rtkobject.h"
#include "rtksection.h"
#include "rtkreport.h"
#include "rtkoutputps.h"

using namespace std;

namespace gong {}
namespace RTK = gong;

namespace gong {

/// \todo {bug5} landscape does not work
/// \todo {bug9} El margen derecho sale el doble que el izquierdo
/// \todo Implementar el padding

static const char *const DSC_Comments =
    "%%!PS-Adobe-3.0\n"
    "%%%%Title: %s\n"
    "%%%%Creator: RTK " RTK_VERSION "\n"
    "%%%%CreationDate: %s\n"
    "%%%%BoundingBox: %d %d %d %d\n"
    "%%%%DocumentData: Clean7Bit\n"
    "%%%%Orientation: %s\n"
    "%%%%Pages: (atend)\n"
    "%%%%PageOrder: Ascend\n"
    "%%%%DocumentMedia: %s %d %d white () ()\n"
    "%%%%EndComments\n";

static const char *const Prolog =
    "%%BeginProlog\n"
    "/s {show} bind def\n"
    "/cr (\\012) def\n"
    "/rtkendpage {showpage} bind def\n"
    "/rtkbox{newpath 4 copy pop pop moveto\n"
    "2 copy 0 exch neg rlineto 0 rlineto 0 exch\n"
    "rlineto neg 0 rlineto pop pop\n"
    "closepath}bind def\n"
    "/dsp {dup stringwidth pop } bind def\n"
    "/cpp {currentpoint pop } bind def\n"
    // Move relative to the beginning of new line. Assumes /x0 /y0 /wi /he /lh (line height)
    "/newline {currentpoint pop x0 sub neg lh neg rmoveto } bind def\n"
    // Counts spaces in string
    "/spacecount { 0 exch { ( ) search { pop pop exch 1 add exch} {pop exit} ifelse } loop } bind def\n"
    // Counts lines in string
    "/linecount { 0 exch { cr search { pop pop exch 1 add exch} {pop exit} ifelse } loop } bind def\n"
    // Searchs space and returns first word
    "/ssp { ( ) search { exch pop } {} ifelse } bind def\n"
    // Searchs newlines and returns first line
    "/scr { cr search { exch pop } {} ifelse } bind def\n"
    // Shows a line, with word-wrap
    "/L { dup spacecount 1 add { ssp dsp cpp add x0 wi add gt { newline s ( ) s } { s ( ) s } ifelse } repeat } bind def\n"
    // Shows a centered line
    "/LC {dup stringwidth pop 2 div neg x0 wi add 2 div add 0 rmoveto show} bind def\n"
    // Shows a right aligned line
    "/LR { dup stringwidth pop neg wi add 0 rmoveto show} bind def\n"
    // Shows a paragrah, divided into lines
    "/P { dup linecount 1 add { scr L newline } repeat } bind def\n"
    // Calc Ascent, Descent and Line Height of current font
    "/fontmeasures { currentfont /FontBBox get aload pop currentfont /FontMatrix get transform\n"
    "/Ascent exch def pop\n"
    "currentfont /FontMatrix get transform /Descent exch def pop\n"
    "/lh Ascent Descent add 1.8 mul def } bind def\n"
    // http://blackshell.usebox.net/archivo/569.php
    "/ReEncode { exch findfont dup length dict begin { 1 index /FID eq { pop pop } { def } ifelse } forall /Encoding ISOLatin1Encoding def currentdict end definefont pop } bind def\n"
    "%%EndProlog\n";

static const char *const Setup =
    "%%BeginSetup\n"
    "%%EndSetup\n";


static Xtring escapeText(const Xtring &text)
{
    Xtring ret;
    char buff[10];
    for( uint i=0; i<text.length(); i++) {
        char ch = text.at(i);
        if( ch == '\\' )
            ret += "\\\\";
        else if( ch == '(' )
            ret += "\\(";
        else if( ch == ')' )
            ret += "\\)";
        else if( (ch >= '\x00' && ch <= '\x1f' ) or (ch >= '\x7f' && ch <= '\xff' ) ) {
            sprintf(buff, "\\%03o", (uint)ch);
            ret += buff;
        } else {
            ret += ch;
        }
    }
    return ret;
}

OutputPS::OutputPS(class Report & r, const char * filename,
                   Measure unitsperinchx, Measure unitsperinchy, PaperSize papersize,
                   PageOrientation pageorientation, Measure sizex, Measure sizey,
                   Measure marginleft, Measure marginright, Measure margintop, Measure marginbottom)
    : Output(r, unitsperinchx, unitsperinchy,
             papersize, pageorientation, sizex,
             sizey, marginleft, marginright,
             margintop, marginbottom), propFileName(filename)
{
    pIConverter = new IConv("ISO_8859-1","UTF-8");
}

OutputPS::~OutputPS()
{
    delete pIConverter;
}

int OutputPS::startReport()
{
    Output::startReport();
    pFile = new std::ofstream(fileName());
    if( !pFile->is_open() ) {
        perror(fileName());
        delete pFile;
        pFile = 0;
        return 1;
    }
    uint mtop = (uint)mReport.marginTop();
    uint mleft = (uint)mReport.marginLeft();
    uint mbottom = (uint)mReport.marginBottom();
    uint mright = (uint)mReport.marginRight();

    int width = (int)sizeX();
    int height = (int)sizeY();

    char *b = new char[strlen(DSC_Comments) + 1000];
    sprintf( b, DSC_Comments,
             mReport.title(), Date().toString().c_str(),
             mleft, mtop, width - mleft - mright, height - mtop - mbottom,
             pageOrientation() == Portrait ? "Portrait" : "Landscape",
             "Custom", width, height);
    (*pFile) << b;
    delete b;
    (*pFile) << Prolog;
    (*pFile) << Setup;
    return 0;
}

int OutputPS::endReport()
{
    (*pFile) << "%%Trailer" << endl;
    (*pFile) << "%%Pages: " << getTotalPages() << endl;
    (*pFile) << "%%DocumentFonts: Helvetica"  << endl;
    (*pFile) << "%%EOF" << endl;
    if ( pFile ) {
        pFile->close();
        delete pFile;
    }
    pFile = 0;
    return 0;
}


Measure OutputPS::startPage()
{
    Output::startPage();
    (*pFile) << "%%Page: " << getCurrentPage() << ' ' << getCurrentPage() << endl;
    (*pFile) << "%%BeginPageSetup" << endl;
    (*pFile) << "%%EndPageSetup" << endl;
//	if( pageOrientation() == Landscape )
//   	(*pFile) << "90 rotate" << endl;
    return 0;
}

Measure OutputPS::endPage()
{
    Output::endPage();
    (*pFile) << "rtkendpage" << endl;
    return 0;
}

Measure OutputPS::startSection(const Section & section)
{
    mSectionStarted = true;
    return Output::startSection( section );
}

Measure OutputPS::endSection(const Section & section)
{
    mSectionStarted = false;
    printObject( section );
    return Output::endSection( section );
}



Measure OutputPS::printObject( const Object &object )
{
    char strcolor[13];
    Report *r = object.report();
    int x0, y0, width, height, ret;
    clipMeasures( object, &x0, &y0, &width, &height);

    // Transform the y to PS user space
    y0 = (int)round(sizeY() - y0, 0);

    // Borders and background
    // Draw the border and the background color
    Color bc = object.backColor();
    if( !object.isSection() || (object.isSection() && mSectionStarted ) ) {
        sprintf(strcolor, "%d %d %d", bc.red(), bc.green(), bc.blue() );
        (*pFile) << strcolor << " setrgbcolor" << endl;
        (*pFile) << x0 << ' ' << y0 << ' ' << width << ' '
                 << height << " rtkbox fill\n";
        // Background image
        if( !strempty(object.backgroundImage()) ) {
            const BaseImage *bi;
            RTK::Image *rtkimage = r->findImage(object.backgroundImage());
            if( rtkimage ) {
                if( (bi = rtkimage->findBaseImage(r->imagesPath())) )  {
                    int imw = bi->width();
                    int imh = bi->height();
                    switch( rtkimage->imageStyle() ) {
                    case ImageNone:
                        drawPixmap(bi, x0, y0, width, height);
                        break;
                    case ImageCenter:
                    {
                        if( imw > width )
                            imw = width;
                        if( imh > height )
                            imh = height;
                        int imx0 = ( x0 + (width - imw) / 2 );
                        int imy0 = ( y0 + (height - imh) / 2 );
                        drawPixmap( bi, imx0, imy0, imw, imh);
                    }
                    break;
                    case ImageTile: /// \todo {add} tile image
                        drawPixmap(bi, x0, y0, width, height);
                        break;
                    case ImageExpand: /// \todo {add} Expand image
                        drawPixmap(bi, x0, y0, width, height);
                        break;
                    }
                } else {
                    mReport.addError(Error::ImageFileNotFound, __FUNCTION__, rtkimage->url());
                }
            } else {
                mReport.addWarning(Error::ImageNotDefined, __FUNCTION__, object.backgroundImage());
            }
        }
    }

    ret = y0;
    if( !object.isSection() ) {
        Xtring objtext = object.formattedText();
        if( objtext.length() ) {
            objtext = escapeText(objtext);
            const char *text = objtext.c_str();
            if( mLastFontFamily != object.fontFamily()
                    || mLastFontSize != object.fontSize()
                    || mLastFontWeight != object.fontWeight()
                    || mLastFontItalic != object.fontItalic() ) {
                mLastFontFamily = object.fontFamily();
                mLastFontSize = object.fontSize();
                mLastFontWeight = object.fontWeight();
                mLastFontItalic = object.fontItalic();
                Xtring usedfont;
                if( mLastFontFamily.size() ) {
                    usedfont = mLastFontFamily;
                } else {
                    usedfont = "Helvetica";
                }
                (*pFile) << "/" + usedfont + " /_" + usedfont + " ReEncode /_" + usedfont + "  findfont" << endl;
                (*pFile) << mLastFontSize << " scalefont setfont fontmeasures" << endl;
            }
            Color fc = object.fontColor();
            sprintf(strcolor, "%d %d %d", fc.red(), fc.green(), fc.blue() );
            (*pFile) << strcolor << " setrgbcolor" << endl;
            (*pFile) << "/x0 " << x0 << " def /y0 " << y0 << " def /wi "
                     << width << " def /he " << height << " def" << std::endl;
            (*pFile) << "x0 y0 moveto 0 Ascent neg rmoveto" << endl;
/// \todo {bug} Crop text if not wide enough
            (*pFile) << '(' << pIConverter->convert(text) << ')';
            if( object.hAlignment() == AlignHCenter )
                (*pFile) << "LC";
            else if( object.hAlignment() == AlignRight )
                (*pFile) << "LR";
            else
                (*pFile) << "P";
            (*pFile) << std::endl;
        } else {
            ret = 0;
        }
    }

    // Draws the border after the object and after the section
    if( !object.isSection() || (object.isSection() && !mSectionStarted ) ) {
        // If the general border is defined, use it
        if ( object.borderStyle() != BorderNone ) {
            bc = object.borderColor();
            sprintf(strcolor, "%d %d %d", bc.red(), bc.green(), bc.blue() );
            (*pFile) << strcolor << " setrgbcolor" << endl;
            (*pFile) << object.borderWidth() << " setlinewidth ";
            (*pFile) << x0 << ' ' << y0 << ' ' << width << ' ' << height - 2*object.borderWidth() << " rtkbox stroke\n";
        } else { // Use the other borders instead
            if ( object.borderTopStyle() != BorderNone ) {
                bc = object.borderTopColor();
                sprintf(strcolor, "%d %d %d", bc.red(), bc.green(), bc.blue() );
                (*pFile) << strcolor << " setrgbcolor" << endl;
                (*pFile) << object.borderTopWidth() << " setlinewidth ";
                (*pFile) << x0 << ' ' << y0 << ' ' << width << ' '
                         << 0 << " rtkbox stroke\n";
            }
            if ( object.borderBottomStyle() != BorderNone ) {
                bc = object.borderBottomColor();
                sprintf(strcolor, "%d %d %d", bc.red(), bc.green(), bc.blue() );
                (*pFile) << strcolor << " setrgbcolor" << endl;
                (*pFile) << object.borderBottomWidth() << " setlinewidth ";
                (*pFile) << x0 << ' ' << y0 - height - 2 * object.borderTopWidth() << ' ' << width << ' '
                         << 0 << " rtkbox stroke\n";
            }
            if ( object.borderLeftStyle() != BorderNone ) {
                bc = object.borderLeftColor();
                sprintf(strcolor, "%d %d %d", bc.red(), bc.green(), bc.blue() );
                (*pFile) << strcolor << " setrgbcolor" << endl;
                (*pFile) << object.borderLeftWidth() << " setlinewidth ";
                (*pFile) << x0 << ' ' << y0 << ' ' << 0 << ' '
                         << height << " rtkbox stroke\n";
            }
            if ( object.borderRightStyle() != BorderNone ) {
                bc = object.borderRightColor();
                sprintf(strcolor, "%d %d %d", bc.red(), bc.green(), bc.blue() );
                (*pFile) << strcolor << " setrgbcolor" << endl;
                (*pFile) << object.borderRightWidth() << " setlinewidth ";
                (*pFile) << x0 + width - 2 * object.borderRightWidth() << ' ' << y0 << ' ' << 0 << ' '
                         << height << " rtkbox stroke\n";
            }
        }
    }
    return ret;
}


void OutputPS::drawPixmap(const BaseImage *bi, int x0, int y0, int width, int height)
{
    int imw = bi->width();
    int imh = bi->height();
    (*pFile) << "gsave" << endl;
    (*pFile) << x0 << ' ' << y0 - height << " translate" << endl;
    (*pFile) << imw << ' ' << imh << ' ' << " scale" << endl;
    (*pFile) << imw << ' ' << imh << ' ' << bi->depth()
             << "[ " << imw << " 0 0 -" << imh << " 0 " << imh << ']' << endl;
    char hexbuf[3];
    (*pFile) << "{<";
    for( int i=0; i<bi->size(); i++ ) {
        if( (i % 26) == 0 )
            (*pFile) << endl;
        sprintf(hexbuf, "%02x", (unsigned short int)(*(bi->pixels()+i)));
        (*pFile) << hexbuf;
    }
    (*pFile) << ">} image" << endl;
    (*pFile) << "grestore" << endl;
}

} // namespace RTK
