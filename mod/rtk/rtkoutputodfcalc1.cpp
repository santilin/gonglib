#include <config.h>

#define HAVE_OPENDOCUMENT 1
#ifdef HAVE_OPENDOCUMENT

#include <cstdio>
#include <fstream>
#include <cerrno>
#include <unistd.h>

#include <gongiconv.h>
#include <gongfileutils.h>
#include <gongformatter.h>
#include <gonglibrary.h>
#include "rtkobject.h"
#include "rtksection.h"
#include "rtkreport.h"
#include "rtkoutputodfcalc1.h"

namespace gong {}
namespace RTK = gong;

namespace gong {


/// \todo {add} Add styles
/// \todo {add} option in configure.in

static const char *xml0 = "<?xml version=\"1.0\" encoding=\"";
static const char *xml1 = "\"?>\n";

static const char *doctype =
    "<office:document-content xmlns:office=\"urn:oasis:names:tc:opendocument:xmlns:office:1.0\" xmlns:style=\"urn:oasis:names:tc:opendocument:xmlns:style:1.0\""
    " xmlns:text=\"urn:oasis:names:tc:opendocument:xmlns:text:1.0\" xmlns:table=\"urn:oasis:names:tc:opendocument:xmlns:table:1.0\" xmlns:draw=\"urn:oasis:names:tc:opendocument:xmlns:drawing:1.0\""
    " xmlns:fo=\"urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xmlns:dc=\"http://purl.org/dc/elements/1.1/\""
    " xmlns:meta=\"urn:oasis:names:tc:opendocument:xmlns:meta:1.0\" xmlns:number=\"urn:oasis:names:tc:opendocument:xmlns:datastyle:1.0\" xmlns:svg=\"urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0\""
    " xmlns:chart=\"urn:oasis:names:tc:opendocument:xmlns:chart:1.0\" xmlns:dr3d=\"urn:oasis:names:tc:opendocument:xmlns:dr3d:1.0\" xmlns:math=\"http://www.w3.org/1998/Math/MathML\""
    " xmlns:form=\"urn:oasis:names:tc:opendocument:xmlns:form:1.0\" xmlns:script=\"urn:oasis:names:tc:opendocument:xmlns:script:1.0\" xmlns:ooo=\"http://openoffice.org/2004/office\""
    " xmlns:ooow=\"http://openoffice.org/2004/writer\" xmlns:oooc=\"http://openoffice.org/2004/calc\" xmlns:dom=\"http://www.w3.org/2001/xml-events\""
    " xmlns:xforms=\"http://www.w3.org/2002/xforms\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\""
    " office:version=\"1.0\"><office:scripts/><office:font-face-decls><style:font-face style:name=\"Liberation Sans\" svg:font-family=\"&apos;Liberation Sans&apos;\""
    " style:font-family-generic=\"swiss\" style:font-pitch=\"variable\"/><style:font-face style:name=\"DejaVu Sans\" svg:font-family=\"&apos;DejaVu Sans&apos;\""
    " style:font-family-generic=\"system\" style:font-pitch=\"variable\"/></office:font-face-decls>";


static const char *automatic_styles =
    "<office:automatic-styles><style:style style:name=\"co1\" style:family=\"table-column\"><style:table-column-properties fo:break-before=\"auto\" style:column-width=\"2.267cm\"/>"
    "</style:style><style:style style:name=\"ro1\" style:family=\"table-row\"><style:table-row-properties style:row-height=\"0.453cm\" fo:break-before=\"auto\" style:use-optimal-row-height=\"true\"/>"
    "</style:style><style:style style:name=\"ta1\" style:family=\"table\" style:master-page-name=\"Default\"><style:table-properties table:display=\"true\" style:writing-mode=\"lr-tb\"/>"
    "</style:style></office:automatic-styles>";

static const char *body =
    "<office:body><office:spreadsheet><table:table table:name=\"Hoja1\" table:style-name=\"ta1\" table:print=\"false\">"
    "<office:forms form:automatic-focus=\"false\" form:apply-design-mode=\"false\"/>"
    "<table:table-column table:style-name=\"co1\" table:default-cell-style-name=\"Default\"/>";

static const char *body_end =
    "</table:table></office:spreadsheet></office:body></office:document-content>";

int OutputODFCalc1::startReport()
{
    mIntSizeX = static_cast<int>(sizeX());
    mIntSizeY = static_cast<int>(sizeY());
    if( strempty(templatePath()) )
        propTemplatePath = strdup( (Xtring(GongLibraryInstance->getGlobalDataDir()) +  "/opendocument/calc/template/").c_str() );
    switch( replaceContent() ) {
    case CREATE:
        if( FileUtils::exists( fileName() ) ) {
            if( unlink( fileName() ) != 0 ) {
                mReport.addWarning( Error::OpenFile, __FUNCTION__, 0, fileName(), strerror(errno) );
            }
        }
        break;
    case REPLACE_CONTENT:
        if( !FileUtils::exists( fileName() ) ) {
            mReport.addError( Error::OpenFile, __FUNCTION__, 0, fileName(), strerror(errno) );
        }
        break;
    case ADD_FIRST_SHEET:
    case ADD_LAST_SHEET:
    case REPLACE_FIRST_SHEET:
    case REPLACE_LAST_SHEET:
        mReport.addError( Error::FeatureNotSupported, "OutputODFCalc1",
                          "Open Office create option '%d' not supported yet", replaceContent());
        return 1;
    default:
        mReport.addError( Error::InvalidArgument, "OutputODFCalc1",
                          0, Xtring::number(replaceContent()).c_str(), __FUNCTION__);
        return 1;
    }
    /// \todo {add} replaceContentToStr

    const char *tmpname = "/tmp/content.xml"; /// \todo {add} tmpname
    pFile = new std::ofstream( tmpname );
    if( !pFile->is_open() || !pFile->good() ) {
        mReport.addError(Error::OpenFile, "OutputODFCalc1",
                         0, fileName(), strerror(errno) );
        delete pFile;
        pFile = 0;
        return 1;
    }
    Xtring transform_encoding = IConv::canonicalCodeName(encoding());
    pFile->write(xml0, strlen(xml0));
    pFile->write(transform_encoding.c_str(), transform_encoding.size());
    pFile->write(xml1, strlen(xml1));
    pFile->write(doctype, strlen(doctype));
    pFile->write(automatic_styles, strlen(automatic_styles));
    pFile->write(body, strlen(body));
    mDetailsFieldCount = 5;
    return 0;
}

int OutputODFCalc1::endReport()
{
    if ( pFile ) {
        pFile->write(body_end, strlen(body_end));
        pFile->close();
        delete pFile;
    }

    Xtring cmd;
    switch( replaceContent() ) {
    case CREATE:
        if( *fileName() == '/' ) {
            cmd = Xtring("TMPPWD=`pwd`;cd \"") + templatePath() + "\" && "
                  "zip -r \"" + fileName() + "\" * && "
                  "cd \"$TMPPWD\" && "
                  "zip -rj \"" + fileName() + "\" /tmp/content.xml";
        } else {
            cmd = Xtring("TMPPWD=`pwd`;cd \"") + templatePath() + "\" && "
                  "zip -r \"$TMPPWD/" + fileName() + "\" * && "
                  "cd \"$TMPPWD\" && "
                  "zip -rj \"$TMPPWD/" + fileName() + "\" /tmp/content.xml";
        }
        break;
    case REPLACE_CONTENT:
        cmd = Xtring("zip -rj ") + fileName() + " /tmp/content.xml";
        break;
    case ADD_FIRST_SHEET:
    case ADD_LAST_SHEET:
    case REPLACE_FIRST_SHEET:
    case REPLACE_LAST_SHEET:
    default:
        return 0;
    }
    Xtring output, errors;
    int ret;
    if( (ret = FileUtils::execProcess( cmd.c_str(), output, errors) != 0) ) {
        mReport.addError( Error::OpenCommand, __FUNCTION__, 0, cmd.c_str(), strerror(errno) );
        if( !output.isEmpty() )
            mReport.addError( Error::OpenCommand, __FUNCTION__, 0, cmd.c_str(), output.c_str() );
        pFile = 0;
        return ret;
    }
    pFile = 0;
    return 0;
}

Measure OutputODFCalc1::startPage()
{
    return sizeY();
}


Measure OutputODFCalc1::endPage()
{
    return 0.0;
}

Measure OutputODFCalc1::startSection( const Section & /*section*/ )
{
    Xtring text = "<table:table-row table:style-name=\"ro1\">";
    pFile->write(text.c_str(), text.length() );
    return sizeY() - mCurrY;
}


Measure OutputODFCalc1::endSection( const Section &section )
{
    Xtring text = "</table:table-row>";
    pFile->write(text.c_str(), text.length() );
    mCurrY += section.sizeY();
    mCurrX = marginLeft();
    return sizeY() - mCurrY;
}



Measure OutputODFCalc1::printObject( const Object &object )
{
    // table:value-type=\"float\" table:value=\"1\"
    Xtring text = "<table:table-cell table:style-name=\"ce1\" " + getOfficeTypeAndValue( object ) + "><text:p>";
    text += object.formattedText().XMLProtect();
    text += "</text:p></table:table-cell>\n";
    pFile->write(text.c_str(), text.length() );
    return sizeY() - mCurrY;
}

Measure OutputODFCalc1::setPosX( Measure newx )
{
    int oldx = mIntSizeX;
    mIntSizeX = static_cast<int>(newx);
    return oldx;
}

Measure OutputODFCalc1::setPosY( Measure newy )
{
    int oldy = mIntSizeY;
    mIntSizeY = static_cast<int>(newy);
    return oldy;
}

Xtring OutputODFCalc1::getOfficeTypeAndValue(const Object &obj) const
{
    Xtring ret = "office:value-type=\"";
    switch( obj.realValue().type() ) {
    case Variant::tString:
        ret += "string\"";
        break;
    case Variant::tBool:
        ret += "boolean\" office:boolean-value=\"" + obj.realValue().toString() + "\"";
        break;
    case Variant::tDate:
        ret += "date\" office:date-value=\""
               + obj.report()->formatter().format( obj.realValue(), "%Y-%m-%d", 0 ) + "\"";
        break;
    case Variant::tDateTime:
        ret += "date\" office:date-value=\""
               + obj.report()->formatter().format( obj.realValue(), "%Y-%m-%dT%H:%M:%S", 0 ) + "\"";
        break;
    case Variant::tTime:
        ret += "date\" office:time-value=\""
               + obj.report()->formatter().format( obj.realValue(), "PT%HH%MM%SS", 0 ) + "\"";
        break;
    case Variant::tMoney:
        ret += "currency\" office:currency=\"" + obj.report()->formatter().getRegConfig().getCurrencySymbol()
               + "\" office:value=\"" + obj.realValue().toString() + "\"";
        break;
    case Variant::tInt:
    case Variant::tLong:
    case Variant::tDouble:
        ret += "float\" office:value=\"" + obj.realValue().toString() + "\"";
        break;
    default:
        ret += Xtring( Variant::typeToName( obj.realValue().type() ) )
               + "\" office:" + Variant::typeToName( obj.realValue().type() )
               + "-value=\"" + obj.realValue().toString() + "\"";
    }
    return ret;
}

} // namespace RTK

#endif // HAVE_OPENDOCUMENT


#if 0
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define nil ((void*)0)

char*
mkfname(char *tmpdir, char *prefix)
{
    int n;
    char *p, *fname;

    if((p = getenv("TMPDIR")) != nil)
        goto Mktemp;
    if((p = tmpdir) != nil)
        goto Mktemp;
    p = "/tmp";

Mktemp:
    n = strlen(p)+1+strlen(prefix)+1+8+1;
    if((fname = malloc(n)) == nil)
        return nil;
    memset(fname, 0, n);
    strcat(fname, p);
    if((n = strlen(p)) > 0 && p[n-1] != '/')
        strcat(fname, "/");
    strcat(fname, prefix);
    strcat(fname, ".XXXXXXXX");

    return fname;
}

extern  int     mkstemp(char*);

char*
safe_tempnam(char *tmpdir, char *prefix)
{
    int fd;
    char *fname;

    if((fname = mkfname(tmpdir, prefix)) == nil)
        return nil;

    if((fd = mkstemp(fname)) < 0) {         /* XXX: leak fd, fname */
        free(fname);
        return nil;
    }
    return fname;
}

int
safe_tmpnam(char *fname)
{
    char *p;

    if((p = mkfname(nil, "tmpfile")) == nil)
        return -1;
    strcpy(fname, p);
    free(p);
    return mkstemp(fname);
}
#endif
