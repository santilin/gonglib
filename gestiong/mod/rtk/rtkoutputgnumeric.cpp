/*<<<<<COPYLEFT*/
/** @file rtkoutputgnumeric.cpp RTK gnumeric output object */
/*
 * RTK The report toolkit
 * Copyright (C) 2003 Francisco Santiago Capel Torres
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
/*>>>>>COPYLEFT*/


#include <config.h>

#define HAVE_GNUMERIC 1
#ifdef HAVE_GNUMERIC

#include <cstdio>
#include <fstream>
#include <cerrno>
#include <unistd.h>

#include <gongiconv.h>
#include <gongfileutils.h>
#include <gongformatter.h>
#include "rtkobject.h"
#include "rtksection.h"
#include "rtkreport.h"
#include "rtkoutputgnumeric.h"

namespace gong {}
namespace RTK = gong;

namespace gong {


/**
	http://www.jfree.org/jworkbook/download/gnumeric-xml.pdf
 */

static const char *xml0 = "<?xml version=\"1.0\" encoding=\"";
static const char *xml1 = "\"?>\n";

static const char *preamble =
    "<gnm:Workbook xmlns:gnm=\"http://www.gnumeric.org/v10.dtd\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:schemaLocation=\"http://www.gnumeric.org/v9.xsd\">\n"
    "  <gnm:Version Epoch=\"1\" Major=\"8\" Minor=\"3\" Full=\"1.8.3\"/>\n"
    "  <gnm:Attributes>\n"
    "    <gnm:Attribute>\n"
    "      <gnm:type>4</gnm:type>\n"
    "      <gnm:name>WorkbookView::show_horizontal_scrollbar</gnm:name>\n"
    "      <gnm:value>TRUE</gnm:value>\n"
    "    </gnm:Attribute>\n"
    "    <gnm:Attribute>\n"
    "      <gnm:type>4</gnm:type>\n"
    "      <gnm:name>WorkbookView::show_vertical_scrollbar</gnm:name>\n"
    "      <gnm:value>TRUE</gnm:value>\n"
    "    </gnm:Attribute>\n"
    "    <gnm:Attribute>\n"
    "      <gnm:type>4</gnm:type>\n"
    "      <gnm:name>WorkbookView::show_notebook_tabs</gnm:name>\n"
    "      <gnm:value>TRUE</gnm:value>\n"
    "    </gnm:Attribute>\n"
    "    <gnm:Attribute>\n"
    "      <gnm:type>4</gnm:type>\n"
    "      <gnm:name>WorkbookView::do_auto_completion</gnm:name>\n"
    "      <gnm:value>TRUE</gnm:value>\n"
    "    </gnm:Attribute>\n"
    "    <gnm:Attribute>\n"
    "      <gnm:type>4</gnm:type>\n"
    "      <gnm:name>WorkbookView::is_protected</gnm:name>\n"
    "      <gnm:value>FALSE</gnm:value>\n"
    "    </gnm:Attribute>\n"
    "  </gnm:Attributes>\n"
    "  <office:document-meta xmlns:office=\"urn:oasis:names:tc:opendocument:xmlns:office:1.0\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xmlns:dc=\"http://purl.org/dc/elements/1.1/\" xmlns:meta=\"urn:oasis:names:tc:opendocument:xmlns:meta:1.0\" xmlns:ooo=\"http://openoffice.org/2004/office\" office:version=\"1.0\">\n"
    "    <office:meta/>\n"
    "  </office:document-meta>\n"
    "  <gnm:SheetNameIndex>\n"
    "    <gnm:SheetName gnm:Cols=\"256\" gnm:Rows=\"65536\">Hoja1</gnm:SheetName>\n"
    "    <gnm:SheetName gnm:Cols=\"256\" gnm:Rows=\"65536\">Hoja2</gnm:SheetName>\n"
    "    <gnm:SheetName gnm:Cols=\"256\" gnm:Rows=\"65536\">Hoja3</gnm:SheetName>\n"
    "  </gnm:SheetNameIndex>\n"
    "  <gnm:Geometry Width=\"768\" Height=\"419\"/>\n"
    "  <gnm:Sheets>\n"
    "    <gnm:Sheet DisplayFormulas=\"0\" HideZero=\"0\" HideGrid=\"0\" HideColHeader=\"0\" HideRowHeader=\"0\" DisplayOutlines=\"1\" OutlineSymbolsBelow=\"1\" OutlineSymbolsRight=\"1\" Visibility=\"GNM_SHEET_VISIBILITY_VISIBLE\" GridColor=\"0:0:0\">\n"
    "      <gnm:Name>Hoja1</gnm:Name>\n"
    "      <gnm:MaxCol>4</gnm:MaxCol>\n"
    "      <gnm:MaxRow>0</gnm:MaxRow>\n"
    "      <gnm:Zoom>1</gnm:Zoom>\n"
    "      <gnm:Names>\n"
    "        <gnm:Name>\n"
    "          <gnm:name>Print_Area</gnm:name>\n"
    "          <gnm:value>Hoja1!$A$1:$IV$65536</gnm:value>\n"
    "          <gnm:position>A1</gnm:position>\n"
    "        </gnm:Name>\n"
    "        <gnm:Name>\n"
    "          <gnm:name>Sheet_Title</gnm:name>\n"
    "          <gnm:value>&quot;Hoja1&quot;</gnm:value>\n"
    "          <gnm:position>A1</gnm:position>\n"
    "        </gnm:Name>\n"
    "      </gnm:Names>\n"
    "      <gnm:PrintInformation>\n"
    "        <gnm:Margins>\n"
    "          <gnm:top Points=\"120\" PrefUnit=\"mm\"/>\n"
    "          <gnm:bottom Points=\"120\" PrefUnit=\"mm\"/>\n"
    "          <gnm:left Points=\"72\" PrefUnit=\"mm\"/>\n"
    "          <gnm:right Points=\"72\" PrefUnit=\"mm\"/>\n"
    "          <gnm:header Points=\"72\" PrefUnit=\"mm\"/>\n"
    "          <gnm:footer Points=\"72\" PrefUnit=\"mm\"/>\n"
    "        </gnm:Margins>\n"
    "        <gnm:Scale type=\"percentage\" percentage=\"100\"/>\n"
    "        <gnm:vcenter value=\"0\"/>\n"
    "        <gnm:hcenter value=\"0\"/>\n"
    "        <gnm:grid value=\"0\"/>\n"
    "        <gnm:even_if_only_styles value=\"0\"/>\n"
    "        <gnm:monochrome value=\"0\"/>\n"
    "        <gnm:draft value=\"0\"/>\n"
    "        <gnm:titles value=\"0\"/>\n"
    "        <gnm:do_not_print value=\"0\"/>\n"
    "        <gnm:order>d_then_r</gnm:order>\n"
    "        <gnm:orientation>portrait</gnm:orientation>\n"
    "        <gnm:Header Left=\"\" Middle=\"&amp;[TAB]\" Right=\"\"/>\n"
    "        <gnm:Footer Left=\"\" Middle=\"Page &amp;[PAGE]\" Right=\"\"/>\n"
    "        <gnm:paper>iso_a4</gnm:paper>\n"
    "      </gnm:PrintInformation>\n"
    "      <gnm:Styles>\n"
    "        <gnm:StyleRegion startCol=\"0\" startRow=\"0\" endCol=\"255\" endRow=\"65535\">\n"
    "          <gnm:Style HAlign=\"1\" VAlign=\"2\" WrapText=\"0\" ShrinkToFit=\"0\" Rotation=\"0\" Shade=\"0\" Indent=\"0\" Locked=\"1\" Hidden=\"0\" Fore=\"0:0:0\" Back=\"FFFF:FFFF:FFFF\" PatternColor=\"0:0:0\" Format=\"General\">\n"
    "            <gnm:Font Unit=\"10\" Bold=\"0\" Italic=\"0\" Underline=\"0\" StrikeThrough=\"0\" Script=\"0\">Sans</gnm:Font>\n"
    "          </gnm:Style>\n"
    "        </gnm:StyleRegion>\n"
    "      </gnm:Styles>\n"
    "      <gnm:Cols DefaultSizePts=\"48\">\n"
    "        <gnm:ColInfo No=\"0\" Unit=\"48\" Count=\"5\"/>\n"
    "      </gnm:Cols>\n"
    "      <gnm:Rows DefaultSizePts=\"12.75\">\n"
    "        <gnm:RowInfo No=\"0\" Unit=\"12.75\"/>\n"
    "      </gnm:Rows>\n"
    "      <gnm:Selections CursorCol=\"5\" CursorRow=\"0\">\n"
    "        <gnm:Selection startCol=\"5\" startRow=\"0\" endCol=\"5\" endRow=\"0\"/>\n"
    "      </gnm:Selections>\n"
    "      <gnm:Cells>\n";

static const char *postamble =
    "      </gnm:Cells>\n"
    "      <gnm:SheetLayout TopLeft=\"A1\"/>\n"
    "      <gnm:Solver ProblemType=\"1\" Inputs=\"\" MaxTime=\"0\" MaxIter=\"0\" NonNeg=\"1\" Discr=\"0\" AutoScale=\"0\" ShowIter=\"0\" AnswerR=\"0\" SensitivityR=\"0\" LimitsR=\"0\" PerformR=\"0\" ProgramR=\"0\"/>\n"
    "    </gnm:Sheet>\n"
    "    <gnm:Sheet DisplayFormulas=\"0\" HideZero=\"0\" HideGrid=\"0\" HideColHeader=\"0\" HideRowHeader=\"0\" DisplayOutlines=\"1\" OutlineSymbolsBelow=\"1\" OutlineSymbolsRight=\"1\" Visibility=\"GNM_SHEET_VISIBILITY_VISIBLE\" GridColor=\"0:0:0\">\n"
    "      <gnm:Name>Hoja2</gnm:Name>\n"
    "      <gnm:MaxCol>-1</gnm:MaxCol>\n"
    "      <gnm:MaxRow>-1</gnm:MaxRow>\n"
    "      <gnm:Zoom>1</gnm:Zoom>\n"
    "      <gnm:Names>\n"
    "        <gnm:Name>\n"
    "          <gnm:name>Print_Area</gnm:name>\n"
    "          <gnm:value>Hoja2!$A$1:$IV$65536</gnm:value>\n"
    "          <gnm:position>A1</gnm:position>\n"
    "        </gnm:Name>\n"
    "        <gnm:Name>\n"
    "          <gnm:name>Sheet_Title</gnm:name>\n"
    "          <gnm:value>&quot;Hoja2&quot;</gnm:value>\n"
    "          <gnm:position>A1</gnm:position>\n"
    "        </gnm:Name>\n"
    "      </gnm:Names>\n"
    "      <gnm:PrintInformation>\n"
    "        <gnm:Margins>\n"
    "          <gnm:top Points=\"120\" PrefUnit=\"mm\"/>\n"
    "          <gnm:bottom Points=\"120\" PrefUnit=\"mm\"/>\n"
    "          <gnm:left Points=\"72\" PrefUnit=\"mm\"/>\n"
    "          <gnm:right Points=\"72\" PrefUnit=\"mm\"/>\n"
    "          <gnm:header Points=\"72\" PrefUnit=\"mm\"/>\n"
    "          <gnm:footer Points=\"72\" PrefUnit=\"mm\"/>\n"
    "        </gnm:Margins>\n"
    "        <gnm:Scale type=\"percentage\" percentage=\"100\"/>\n"
    "        <gnm:vcenter value=\"0\"/>\n"
    "        <gnm:hcenter value=\"0\"/>\n"
    "        <gnm:grid value=\"0\"/>\n"
    "        <gnm:even_if_only_styles value=\"0\"/>\n"
    "        <gnm:monochrome value=\"0\"/>\n"
    "        <gnm:draft value=\"0\"/>\n"
    "        <gnm:titles value=\"0\"/>\n"
    "        <gnm:do_not_print value=\"0\"/>\n"
    "        <gnm:order>d_then_r</gnm:order>\n"
    "        <gnm:orientation>portrait</gnm:orientation>\n"
    "        <gnm:Header Left=\"\" Middle=\"&amp;[TAB]\" Right=\"\"/>\n"
    "        <gnm:Footer Left=\"\" Middle=\"Page &amp;[PAGE]\" Right=\"\"/>\n"
    "        <gnm:paper>iso_a4</gnm:paper>\n"
    "      </gnm:PrintInformation>\n"
    "      <gnm:Styles>\n"
    "        <gnm:StyleRegion startCol=\"0\" startRow=\"0\" endCol=\"255\" endRow=\"65535\">\n"
    "          <gnm:Style HAlign=\"1\" VAlign=\"2\" WrapText=\"0\" ShrinkToFit=\"0\" Rotation=\"0\" Shade=\"0\" Indent=\"0\" Locked=\"1\" Hidden=\"0\" Fore=\"0:0:0\" Back=\"FFFF:FFFF:FFFF\" PatternColor=\"0:0:0\" Format=\"General\">\n"
    "            <gnm:Font Unit=\"10\" Bold=\"0\" Italic=\"0\" Underline=\"0\" StrikeThrough=\"0\" Script=\"0\">Sans</gnm:Font>\n"
    "          </gnm:Style>\n"
    "        </gnm:StyleRegion>\n"
    "      </gnm:Styles>\n"
    "      <gnm:Cols DefaultSizePts=\"48\"/>\n"
    "      <gnm:Rows DefaultSizePts=\"12.75\"/>\n"
    "      <gnm:Selections CursorCol=\"0\" CursorRow=\"0\">\n"
    "        <gnm:Selection startCol=\"0\" startRow=\"0\" endCol=\"0\" endRow=\"0\"/>\n"
    "      </gnm:Selections>\n"
    "      <gnm:Cells/>\n"
    "      <gnm:SheetLayout TopLeft=\"A1\"/>\n"
    "      <gnm:Solver ProblemType=\"1\" Inputs=\"\" MaxTime=\"0\" MaxIter=\"0\" NonNeg=\"1\" Discr=\"0\" AutoScale=\"0\" ShowIter=\"0\" AnswerR=\"0\" SensitivityR=\"0\" LimitsR=\"0\" PerformR=\"0\" ProgramR=\"0\"/>\n"
    "    </gnm:Sheet>\n"
    "    <gnm:Sheet DisplayFormulas=\"0\" HideZero=\"0\" HideGrid=\"0\" HideColHeader=\"0\" HideRowHeader=\"0\" DisplayOutlines=\"1\" OutlineSymbolsBelow=\"1\" OutlineSymbolsRight=\"1\" Visibility=\"GNM_SHEET_VISIBILITY_VISIBLE\" GridColor=\"0:0:0\">\n"
    "      <gnm:Name>Hoja3</gnm:Name>\n"
    "      <gnm:MaxCol>-1</gnm:MaxCol>\n"
    "      <gnm:MaxRow>-1</gnm:MaxRow>\n"
    "      <gnm:Zoom>1</gnm:Zoom>\n"
    "      <gnm:Names>\n"
    "        <gnm:Name>\n"
    "          <gnm:name>Print_Area</gnm:name>\n"
    "          <gnm:value>Hoja3!$A$1:$IV$65536</gnm:value>\n"
    "          <gnm:position>A1</gnm:position>\n"
    "        </gnm:Name>\n"
    "        <gnm:Name>\n"
    "          <gnm:name>Sheet_Title</gnm:name>\n"
    "          <gnm:value>&quot;Hoja3&quot;</gnm:value>\n"
    "          <gnm:position>A1</gnm:position>\n"
    "        </gnm:Name>\n"
    "      </gnm:Names>\n"
    "      <gnm:PrintInformation>\n"
    "        <gnm:Margins>\n"
    "          <gnm:top Points=\"120\" PrefUnit=\"mm\"/>\n"
    "          <gnm:bottom Points=\"120\" PrefUnit=\"mm\"/>\n"
    "          <gnm:left Points=\"72\" PrefUnit=\"mm\"/>\n"
    "          <gnm:right Points=\"72\" PrefUnit=\"mm\"/>\n"
    "          <gnm:header Points=\"72\" PrefUnit=\"mm\"/>\n"
    "          <gnm:footer Points=\"72\" PrefUnit=\"mm\"/>\n"
    "        </gnm:Margins>\n"
    "        <gnm:Scale type=\"percentage\" percentage=\"100\"/>\n"
    "        <gnm:vcenter value=\"0\"/>\n"
    "        <gnm:hcenter value=\"0\"/>\n"
    "        <gnm:grid value=\"0\"/>\n"
    "        <gnm:even_if_only_styles value=\"0\"/>\n"
    "        <gnm:monochrome value=\"0\"/>\n"
    "        <gnm:draft value=\"0\"/>\n"
    "        <gnm:titles value=\"0\"/>\n"
    "        <gnm:do_not_print value=\"0\"/>\n"
    "        <gnm:order>d_then_r</gnm:order>\n"
    "        <gnm:orientation>portrait</gnm:orientation>\n"
    "        <gnm:Header Left=\"\" Middle=\"&amp;[TAB]\" Right=\"\"/>\n"
    "        <gnm:Footer Left=\"\" Middle=\"Page &amp;[PAGE]\" Right=\"\"/>\n"
    "        <gnm:paper>iso_a4</gnm:paper>\n"
    "      </gnm:PrintInformation>\n"
    "      <gnm:Styles>\n"
    "        <gnm:StyleRegion startCol=\"0\" startRow=\"0\" endCol=\"255\" endRow=\"65535\">\n"
    "          <gnm:Style HAlign=\"1\" VAlign=\"2\" WrapText=\"0\" ShrinkToFit=\"0\" Rotation=\"0\" Shade=\"0\" Indent=\"0\" Locked=\"1\" Hidden=\"0\" Fore=\"0:0:0\" Back=\"FFFF:FFFF:FFFF\" PatternColor=\"0:0:0\" Format=\"General\">\n"
    "            <gnm:Font Unit=\"10\" Bold=\"0\" Italic=\"0\" Underline=\"0\" StrikeThrough=\"0\" Script=\"0\">Sans</gnm:Font>\n"
    "          </gnm:Style>\n"
    "        </gnm:StyleRegion>\n"
    "      </gnm:Styles>\n"
    "      <gnm:Cols DefaultSizePts=\"48\"/>\n"
    "      <gnm:Rows DefaultSizePts=\"12.75\"/>\n"
    "      <gnm:Selections CursorCol=\"0\" CursorRow=\"0\">\n"
    "        <gnm:Selection startCol=\"0\" startRow=\"0\" endCol=\"0\" endRow=\"0\"/>\n"
    "      </gnm:Selections>\n"
    "      <gnm:Cells/>\n"
    "      <gnm:SheetLayout TopLeft=\"A1\"/>\n"
    "      <gnm:Solver ProblemType=\"1\" Inputs=\"\" MaxTime=\"0\" MaxIter=\"0\" NonNeg=\"1\" Discr=\"0\" AutoScale=\"0\" ShowIter=\"0\" AnswerR=\"0\" SensitivityR=\"0\" LimitsR=\"0\" PerformR=\"0\" ProgramR=\"0\"/>\n"
    "    </gnm:Sheet>\n"
    "  </gnm:Sheets>\n"
    "  <gnm:UIData SelectedTab=\"0\"/>\n"
    "  <gnm:Calculation ManualRecalc=\"0\" EnableIteration=\"1\" MaxIterations=\"100\" IterationTolerance=\"0.001\"/>\n"
    "</gnm:Workbook>";


int OutputGNumeric::startReport()
{
    mIntSizeX = static_cast<int>(sizeX());
    mIntSizeY = static_cast<int>(sizeY());
    nrow = ncol = 0;
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
        mReport.addError( Error::FeatureNotSupported, "OutputGNumeric",
                          "GNumeric create option '%d' not supported yet", replaceContent());
        return 1;
    default:
        mReport.addError( Error::InvalidArgument, "OutputGNumeric",
                          0, Xtring::number(replaceContent()).c_str(), __FUNCTION__);
        return 1;
    }
    /// \todo {add} replaceContentToStr

    pFile = new std::ofstream( fileName() );
    if( !pFile->is_open() || !pFile->good() ) {
        mReport.addError(Error::OpenFile, "OutputGNumeric",
                         0, fileName(), strerror(errno) );
        delete pFile;
        pFile = 0;
        return 1;
    }
    Xtring transform_encoding = IConv::canonicalCodeName(encoding());
    pFile->write(xml0, strlen(xml0));
    pFile->write(transform_encoding.c_str(), transform_encoding.size());
    pFile->write(xml1, strlen(xml1));
    pFile->write(preamble, strlen(preamble));
    return 0;
}

int OutputGNumeric::endReport()
{
    if ( pFile ) {
        pFile->write(postamble, strlen(postamble));
        pFile->close();
        delete pFile;
    }

    pFile = 0;
    return 0;
}

Measure OutputGNumeric::startPage()
{
    return sizeY();
}


Measure OutputGNumeric::endPage()
{
    return 0.0;
}

Measure OutputGNumeric::startSection( const Section & /*section*/ )
{
    ncol = 0;
    nrow ++;
    return sizeY() - mCurrY;
}


Measure OutputGNumeric::endSection( const Section &section )
{
    mCurrY += section.sizeY();
    mCurrX = marginLeft();
    pFile->write("\n", strlen("\n") );
    return sizeY() - mCurrY;
}



Measure OutputGNumeric::printObject( const Object &object )
{
    if( object.isSection() )
        return 0;
    Xtring text = "\t<gnm:Cell Row=\"" + Xtring::number(nrow) + "\" Col=\"" + Xtring::number(ncol) + "\" ";
    text += getGNumericTypeAndValue( object ) + "</gnm:Cell>\n";
    pFile->write(text.c_str(), text.length() );
    ncol++;
    return sizeY() - mCurrY;
}



Measure OutputGNumeric::setPosX( Measure newx )
{
    int oldx = mIntSizeX;
    mIntSizeX = static_cast<int>(newx);
    return oldx;
}

Measure OutputGNumeric::setPosY( Measure newy )
{
    int oldy = mIntSizeY;
    mIntSizeY = static_cast<int>(newy);
    return oldy;
}

Xtring OutputGNumeric::getGNumericTypeAndValue(const Object &obj) const
{
    Xtring ret = "ValueType=\"";
    switch( obj.realValue().type() ) {
    case Variant::tBool:
    case Variant::tString:
        ret += "60\">" + obj.formattedText().XMLProtect();
        break;
    case Variant::tDate:
    case Variant::tDateTime:
    case Variant::tTime:
    {
        Xtring _format = obj.format();
        _format.replace("%d", "dd");
        _format.replace("%m", "mm");
        _format.replace("%Y", "yyyy");
        _format.replace("%y", "yy");
        _format.replace("%H", "hh");
        _format.replace("%M", "MM");
        if( obj.realValue().type() == Variant::tDate ) {
            // Dates in gnumeric are number of days from Jan, 1st, 1900
            ret += Xtring("40\" ValueFormat=\"") + _format + "\">"
                   + Xtring::number(obj.realValue().toDate().getJulianDay() - Date(1899,12,30).getJulianDay());
        } else if( obj.realValue().type() == Variant::tDateTime ) {
            ret += Xtring("40\" ValueFormat=\"") + _format + "\">"
                   + Xtring::number(obj.realValue().toDate().getJulianDay() - Date(1899,12,30).getJulianDay())
                   + Xtring::number(obj.realValue().toTime().getTotalSeconds() / 86400.0).mid(1);
        } else {
            // Times in GNumeric are number of seconds interpolated to 0 and 1
            ret += Xtring("40\" ValueFormat=\"") + _format + "\">"
                   + Xtring::number(obj.realValue().toTime().getTotalSeconds() / 86400.0 );
        }
    }
    break;
    case Variant::tMoney:
        ret += "40\" ValueFormat=\"&quot;€&quot; #,##0.00_);[Red](&quot;€&quot; #,##0.00)\">"
               + obj.realValue().toMoney().toString();
        break;
    case Variant::tInt:
    case Variant::tLong:
    case Variant::tDouble:
        ret += "40\">" + obj.realValue().toString();
        break;
    default:
        ret += "######";
    }
    return ret;
}

} // namespace RTK

#endif // HAVE_GNUMERIC
