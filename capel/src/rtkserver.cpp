#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <gongfileutils.h>
#include "rtkcppclass.h"
#include "capelcppmodule.h"

using namespace gong;

namespace capel {

#define PROPERTY_FIX    (PROPERTY | 0x1000)
#define PROPERTY_STYLE  (PROPERTY | 0x2000)
#define PROPERTY_READONLY  (PROPERTY | 0x4000)
#define PROPERTY_OBJECT_PARENT  (PROPERTY | 0x10000)
#define PROPERTY_OBJECT_STYLE  (PROPERTY | 0x8000)
#define PROPERTY_OBJECT_FULL  (PROPERTY_OBJECT_PARENT | PROPERTY_OBJECT_STYLE)

const char *RTKProject = "RTK The report toolkit. Copyright (C) 2003-2013 Francisco Santiago Capel Torres";

#define DEVELPATH "/home/santilin/devel/"
#define RTKLIBPATH DEVELPATH "gestiong/mod/rtk/"
#define RTKCONVERTPATH DEVELPATH "reports/trunk/rtkconvert/"
#define RTKVIEWPATH DEVELPATH "reports/trunk/rtkview/"
#define RTKTESTSPATH DEVELPATH "reports/trunk/tests/"

#define ISRTKOBJECT (classname=="Object" || classname=="Section" || classname=="Report")

class GenRTKCode : public RTKCppClass {
    public:
        GenRTKCode( const Xtring &path = "" )
    : RTKCppClass( path ) {}
    // Redefinida
        virtual void addGetSet( const Xtring &filename, const Xtring &classname,
                                GetSetArray *array, int nelem, bool constructor );
    // Redefinida
        virtual Xtring addOneTest(const Xtring &classname, GetSetArray *array);

        void genReadRtk( bool create, const Xtring &filename, const Xtring &classname,
                         GetSetArray *array, int nelem);
        void addProperties(const Xtring &filename, const Xtring &classname,
                           GetSetArray *array, int nelem);
        void addPropertiesGet(const Xtring &filename, const Xtring &classname,
                           GetSetArray *array, int nelem);
        void addGetPropValues(const Xtring &filename, const Xtring &classname,
                           GetSetArray *array, int nelem);
};

GenRTKCode *code;


GetSetArray gsReport[] = {
    { PROPERTY_FIX, "const char *", "Title", "", "", "Report title"},
    { PROPERTY_FIX, "UnitsType", "Units", "chars", "", "Report units"},
    { PROPERTY_FIX, "PageOrientation", "PageOrientation", "Portrait", "", "Page Orientation: Portrait or Landscape"},
    { PROPERTY_FIX, "const char *", "StylesPath", "", "", "Path to the xml style files"},
    { PROPERTY_FIX, "const char *", "ImagesPath", "", "", "Path to the image files"},
    { PROPERTY_FIX, "const char *", "IncludePath", "", "", "Path to the included files"},
    { PROPERTY_FIX, "const char *", "ParamDelim", "\"###\"", "", "Delimiter string for the parameters"},
    { PROPERTY_FIX, "const char *", "Order", "", "", "List of order fields"},
    { PROPERTY_FIX, "const char *", "Filter", "", "", "Filter expression"},
	{ PROPERTY_FIX, "int", "Copies", "1", "", "Number of copies to print"},
	{ PROPERTY_FIX, "int", "PagesPerFolio", "1", "", "Number of pages to print on each folio"}
};

GetSetArray gsParameter[] = {
    { PROPERTY_READONLY, "const char *", "Name", "", "", "" },
    { PROPERTY, "const char *", "Value", "", "", "Parameter's value"},
    { PROPERTY, "Variant::Type", "Type", "Variant::tString", "Variant::nameToType", "Parameter's type" },
    { PROPERTY, "const char *", "Caption", "", "", "Parameter's caption"}
};

GetSetArray gsImage[] = {
    { PROPERTY_READONLY, "const char *", "Name", "", "", "" },
    { PROPERTY_FIX, "const char *", "Url", "", "", "Url of the image"},
    { PROPERTY_FIX, "const char *", "MimeType", "", "", "Mime type of the image"},
    { PROPERTY_FIX, "ImageStyle", "ImageStyle", "RTK::ImageNone", "", "Style to apply to the background image" }
                        };

GetSetArray gsOutput[] = {
    { PROPERTY, "Measure", "UnitsPerInchX", "72", "", ""},
    { PROPERTY, "Measure", "UnitsPerInchY", "72", "", ""},
    { PROPERTY, "PaperSize", "PaperSize", "A4", "", ""},
    { PROPERTY, "PageOrientation", "PageOrientation", "Portrait", "", ""},
    { PROPERTY, "Measure", "SizeX", "0", "", ""},
    { PROPERTY, "Measure", "SizeY", "0", "", ""},
    { PROPERTY, "Measure", "MarginLeft", "0", "", ""},
    { PROPERTY, "Measure", "MarginRight", "0", "", ""},
    { PROPERTY, "Measure", "MarginTop", "0", "", ""},
    { PROPERTY, "Measure", "MarginBottom", "0", "", ""},
    // Default properties
    { PROPERTY, "const char *", "DefaultFontFamily", "", "", "Output's default Font family"},
    { PROPERTY, "double", "DefaultFontSize", "10", "", "Output's default Font size"},
    { PROPERTY, "int", "DefaultFontWeight", "0", "", "Output's default Font weight"},
    { PROPERTY, "bool", "DefaultFontItalic", "false", "", "whether the oject's font is italic"},
    { PROPERTY, "double", "DefaultMinFontSize", "4", "", "Output's default minimun font size"},
    { PROPERTY, "Adjustment", "DefaultAdjustment", "AdjustTrim", "", "Output's default type of adjudstment"},
    { PROPERTY, "Color", "DefaultFontColor", "Color::Black", "", "Output's default text color"},
    { PROPERTY, "Color", "DefaultBackColor", "Color::White", "", "Output's default back color"},
    { PROPERTY, "Color", "DefaultBorderColor", "Color::Black", "", "Output's default border color"},
    { PROPERTY, "int", "DefaultBorderWidth", "0", "", "Output's default border width"},
    { PROPERTY, "Color", "DefaultBorderTopColor", "Color::Black", "", "Output's default top border color"},
    { PROPERTY, "int", "DefaultBorderTopWidth", "0", "", "Output's default top border width"},
    { PROPERTY, "Color", "DefaultBorderBottomColor", "Color::Black", "", "Output's default bottom border color"},
    { PROPERTY, "int", "DefaultBorderBottomWidth", "0", "", "Output's default bottom border width"},
    { PROPERTY, "Color", "DefaultBorderLeftColor", "Color::Black", "", "Output's default left border color"},
    { PROPERTY, "int", "DefaultBorderLeftWidth", "0", "", "Output's default left border width"},
    { PROPERTY, "Color", "DefaultBorderRightColor", "Color::Black", "", "Output's default right border color"},
    { PROPERTY, "int", "DefaultBorderRightWidth", "0", "", "Output's default right border width"}
};

GetSetArray gsOutputText[] = {
    { PROPERTY, "const char *", "FileName", "", "", "Output's file name" }
                             };

GetSetArray gsOutputCsv[] = {
    { PROPERTY, "const char *", "FileName", "", "", "Output's file name" },
    { PROPERTY, "const char *", "Delimiters", "\",\"", "", "The delimiter(s) of the columns"},
    { PROPERTY, "const char *", "Quote", "\"\\\"\"", "", "The quotation simbol used for alphanumeric values"},
    { PROPERTY, "bool", "ColumnNames", "false", "", "Whether the first line contains the names of the columns"}
};

GetSetArray gsOutputPS[] =   {
    { PROPERTY, "const char *", "FileName", "", "", "Output's file name" }
                             };
GetSetArray gsOutputMySql[] = {
    { PROPERTY, "bool", "CreateTables", "true", "", ""},
    { PROPERTY, "const char *", "Database", "", "", ""},
    { PROPERTY, "const char *", "Host", "", "", ""},
    { PROPERTY, "const char *", "User", "", "", ""},
    { PROPERTY, "const char *", "Password", "", "", ""},
    { PROPERTY, "int", "Port", "0", "", ""},
                           };
#if 0
GetSetArray gsOutputQPainter[] = {
                                 };
#endif

GetSetArray gsOutputODFCalc1[] = {
    { PROPERTY, "const char *", "FileName", "", "", "Output's file name" },
    { PROPERTY, "const char *", "Encoding", "", "", "Encoding of the file" },
    { PROPERTY, "const char *", "TemplatePath", "", "", "Dir containing the template of an empty OpenDocument" },
    { PROPERTY, "OOCreateOptions", "ReplaceContent", "CREATE", "", "Flag to set what to do if the file exists" },
};
GetSetArray gsOutputXml[] = {
    { PROPERTY, "const char *", "FileName", "", "", "Output's file name" },
    { PROPERTY, "const char *", "Encoding", "", "", "Encoding of the file" },
};

GetSetArray gsOutputHtml[] = {
    { PROPERTY, "const char *", "FileName", "", "", "Output's file name" },
    { PROPERTY, "const char *", "Encoding", "", "", "Encoding of the file" },
};

GetSetArray gsInputField[] = {
    { CONSTRUCTOR_ARG | PROPERTY_READONLY, "const char *", "Name", "", "", "" },
    { CONSTRUCTOR_ARG | PROPERTY_FIX, "const char *", "Source", "", "", "Source position or name in the input" },
    { CONSTRUCTOR_ARG | PROPERTY_FIX, "Variant::Type", "Type", "Variant::tInvalid", "", "Type of this input field" },
    { CONSTRUCTOR_ARG | PROPERTY_FIX, "const char *", "ConstValue", "", "", "Const value of this input field" }
};


GetSetArray gsInput[] = {
    { CONSTRUCTOR_ARG | PROPERTY_READONLY, "const char *", "Name", "", "", "" },
    { CONSTRUCTOR_ARG | PROPERTY_READONLY, "const char *", "Driver", "", "", "Input's driver" },
    { CONSTRUCTOR_ARG | PROPERTY_FIX, "const char *", "Encoding", "", "", "Input's enconding" }
};

GetSetArray gsInputCsv[] = {
    { CONSTRUCTOR_ARG | PROPERTY_FIX, "const char *", "Filename", "", "", "The name of the CSV file"},
    { PROPERTY_FIX, "const char *", "Delimiters", "\",\"", "", "The delimiter(s) of the columns"},
    { PROPERTY_FIX, "const char *", "Quote", "\"\\\"\"", "", "The quotation simbol used for alphanumeric values"},
    { PROPERTY_FIX, "int", "HeaderLines", "0", "", "The number of header lines in the CSV file"}
};

GetSetArray gsInputMySql[] = {
    { PROPERTY_FIX, "const char *", "Host", "", "", ""},
    { PROPERTY_FIX, "int", "Port", "0", "", ""},
    { PROPERTY_FIX, "const char *", "User", "", "", ""},
    { PROPERTY_FIX, "const char *", "Password", "", "", ""},
    { PROPERTY_FIX, "const char *", "Database", "", "", ""},
    { PROPERTY_FIX, "const char *", "From", "", "", ""},
    { PROPERTY_FIX, "const char *", "Where", "", "", ""},
    { PROPERTY_FIX, "const char *", "OrderBy", "", "", ""},
    { PROPERTY_FIX, "const char *", "GroupBy", "", "", ""},
    { PROPERTY_FIX, "const char *", "PreSQL", "", "", ""},
    { PROPERTY_FIX, "const char *", "PostSQL", "", "", ""}
};

GetSetArray gsInputXBase[] = {
    { PROPERTY_FIX, "const char *", "DatabasePath", "", "", ""},
    { PROPERTY_FIX, "bool", "CaseSensitive", "true", "", ""},
    { PROPERTY_FIX, "const char *", "From", "", "", ""},
    { PROPERTY_FIX, "const char *", "Where", "", "", ""},
    { PROPERTY_FIX, "const char *", "OrderBy", "", "", ""},
    { PROPERTY_FIX, "const char *", "GroupBy", "", "", ""},
    { PROPERTY_FIX, "const char *", "PreSQL", "", "", ""},
    { PROPERTY_FIX, "const char *", "PostSQL", "", "", ""}
};

GetSetArray gsInputPgSql[] = {
    { PROPERTY_FIX, "const char *", "Host", "", "", ""},
    { PROPERTY_FIX, "int", "Port", "0", "", ""},
    { PROPERTY_FIX, "const char *", "User", "", "", ""},
    { PROPERTY_FIX, "const char *", "Password", "", "", ""},
    { PROPERTY_FIX, "const char *", "Database", "", "", ""},
    { PROPERTY_FIX, "const char *", "From", "", "", ""},
    { PROPERTY_FIX, "const char *", "Where", "", "", ""},
    { PROPERTY_FIX, "const char *", "OrderBy", "", "", ""},
    { PROPERTY_FIX, "const char *", "GroupBy", "", "", ""},
    { PROPERTY_FIX, "const char *", "PreSQL", "", "", ""},
    { PROPERTY_FIX, "const char *", "PostSQL", "", "", ""}
};



GetSetArray gsObject[] = {
    { PROPERTY, "const char *", "Name", "", "", "Object's name" },
    { PROPERTY_FIX, "const char *", "Value", "", "", "Object's value" },
    { PROPERTY_FIX, "const char *", "FormulaBefore", "", "", "Object's formula executed before printing" },
    { PROPERTY_FIX, "const char *", "FormulaAfter", "", "", "Object's formula executed after printing" },
    { PROPERTY_FIX, "AggregateType", "Aggregate", "AggNone", "", "Aggreate operation on the object"},
    { PROPERTY_FIX, "Measure", "PosX", "0", "", "Object's X position" },
    { PROPERTY_FIX, "Measure", "PosY", "0", "", "Object's Y position" },
    { PROPERTY_FIX, "Measure", "SizeX", "0", "", "Object's X size" },
    { PROPERTY_FIX, "Measure", "SizeY", "0", "", "Object's Y size" },
    { PROPERTY_FIX, "const char *", "Style", "", "", "Object's display style" },
    { PROPERTY_FIX, "const char *", "Url", "", "", "Object's url" },
    { PROPERTY_FIX, "const char *", "Tag", "", "", "Free text/comment for the object, not visible nor computed" },

    // Las mismas que style
    { PROPERTY_FIX, "Measure", "MarginLeft", "0", "", "Object's left margin" },
    { PROPERTY_FIX, "Measure", "MarginRight", "0", "", "Object's right margin" },
    { PROPERTY_FIX, "Measure", "MarginTop", "0", "", "Object's top margin" },
    { PROPERTY_FIX, "Measure", "MarginBottom", "0", "", "Object's bottom margin" },
	{ PROPERTY_FIX, "Measure", "Padding", "0", "", "padding (inner margin) for the object" },
	{ PROPERTY_FIX, "Measure", "PaddingLeft", "0", "", "padding left (inner margin) for the object" },
	{ PROPERTY_FIX, "Measure", "PaddingTop", "0", "", "padding top (inner margin) for the object" },
    { PROPERTY_OBJECT_FULL, "bool", "Visible", "true", "", "visibility of the object (but still computed)" },
    { PROPERTY_OBJECT_FULL, "bool", "Supressed", "false", "", "supression of the object (not computed)" },
    { PROPERTY_OBJECT_STYLE, "bool", "SupressDup", "false", "", "supression of duplicated values" },
    { PROPERTY_OBJECT_FULL, "const char *", "FontFamily", "\"Helvetica\"", "", "Object's Font family"},
    { PROPERTY_OBJECT_FULL, "double", "FontSize", "10", "", "Object's Font size"},
    { PROPERTY_OBJECT_FULL, "int", "FontWeight", "0", "", "Object's Font weight"},
    { PROPERTY_OBJECT_FULL, "bool", "FontItalic", "false", "", "whether the oject's font is italic"},
    { PROPERTY_OBJECT_FULL, "double", "MinFontSize", "8", "", "Object's minimun font size"},
    { PROPERTY_OBJECT_FULL, "Adjustment", "Adjustment", "AdjustTrim", "", "Object's type of adjudstment"},
    { PROPERTY_OBJECT_FULL, "int", "MaxLines", "0", "", "Object's maximum number of lines"},
    { PROPERTY_OBJECT_STYLE, "const char *", "Format", "", "", "Object's format to display the value"},
    { PROPERTY_OBJECT_STYLE, "const char *", "Mask", "", "", "Object's mask to display the value"},
    { PROPERTY_OBJECT_STYLE, "const char *", "Locale", "", "", "Object's locale to display the value"},
    { PROPERTY_OBJECT_FULL, "Alignment", "HAlignment", "AlignAuto", "", "Object's horizontal alignment"},
    { PROPERTY_OBJECT_FULL, "Alignment", "VAlignment", "AlignAuto", "", "Object's vertical alignment"},
    { PROPERTY_OBJECT_FULL, "Color", "FontColor", "Color::Black", "", "Object's text color"},
    { PROPERTY_OBJECT_FULL, "Color", "BackColor", "Color::White", "", "Object's back color"},
    { PROPERTY_OBJECT_FULL, "Color", "BorderColor", "Color::Black", "", "Object's border color"},
    { PROPERTY_OBJECT_FULL, "int", "BorderWidth", "0", "", "Object's border width"},
    { PROPERTY_OBJECT_STYLE, "BorderStyle", "BorderStyle", "BorderNone", "", "Object's border style"},
    { PROPERTY_OBJECT_FULL, "Color", "BorderTopColor", "Color::Black", "", "Object's top border color"},
    { PROPERTY_OBJECT_FULL, "int", "BorderTopWidth", "0", "", "Object's top border width"},
    { PROPERTY_OBJECT_STYLE, "BorderStyle", "BorderTopStyle", "BorderNone", "", "Object's top border style"},
    { PROPERTY_OBJECT_FULL, "Color", "BorderBottomColor", "Color::Black", "", "Object's bottom border color"},
    { PROPERTY_OBJECT_FULL, "int", "BorderBottomWidth", "0", "", "Object's bottom border width"},
    { PROPERTY_OBJECT_STYLE, "BorderStyle", "BorderBottomStyle", "BorderNone", "", "Object's bottom border style"},
    { PROPERTY_OBJECT_FULL, "Color", "BorderLeftColor", "Color::Black", "", "Object's left border color"},
    { PROPERTY_OBJECT_FULL, "int", "BorderLeftWidth", "0", "", "Object's left border width"},
    { PROPERTY_OBJECT_STYLE, "BorderStyle", "BorderLeftStyle", "BorderNone", "", "Object's left border style"},
    { PROPERTY_OBJECT_FULL, "Color", "BorderRightColor", "Color::Black", "", "Object's right border color"},
    { PROPERTY_OBJECT_FULL, "int", "BorderRightWidth", "0", "", "Object's right border width"},
    { PROPERTY_OBJECT_STYLE, "BorderStyle", "BorderRightStyle", "BorderNone", "", "Object's right border style"},
    { PROPERTY_OBJECT_STYLE, "const char *", "BackgroundImage", "", "", "Name of the background image" },
    { PROPERTY_OBJECT_STYLE, "ImageStyle", "ImageStyle", "RTK::ImageNone", "", "Style of the object's or background image" }
};

GetSetArray gsSection[] = {
                              { PROPERTY_READONLY, "SectionType", "Type", "SectionDetails", "", "Section's type"},
                              { PROPERTY, "uint", "GroupLevel", "0", "", "Section's group level"},
                              { PROPERTY_FIX, "int", "Columns", "1", "", "Section's columns number"},
                              { PROPERTY_OBJECT_STYLE, "bool", "PageBefore", "false", "", "whether an end of page is printed before this section"},
                              { PROPERTY_OBJECT_STYLE, "bool", "PageAfter", "false", "", "whether an end of page is printed after this section"},
                              { PROPERTY_OBJECT_STYLE, "bool", "ResetNPage", "false", "", "whether this section reset de page counter to 1"},
                              { PROPERTY_OBJECT_STYLE, "bool", "SupIfBlank", "false", "", "whether this section is supressed if contains no data"},
                              { PROPERTY_OBJECT_STYLE, "bool", "RepeatHeader", "false", "", "whether the section's header is repeated on every page" },
                              { PROPERTY_OBJECT_STYLE, "bool", "PrintInFirstPage", "true", "", "whether the section's header is repeated on every page" },
                          };

GetSetArray gsStyle[] = {
    { PROPERTY_READONLY, "const char *", "Name", "", "", "" },
// Las mismas que object
    { PROPERTY_STYLE, "Measure", "MarginLeft", "0", "", "Object's left margin" },
    { PROPERTY_STYLE, "Measure", "MarginRight", "0", "", "Object's right margin" },
    { PROPERTY_STYLE, "Measure", "MarginTop", "0", "", "Object's top margin" },
    { PROPERTY_STYLE, "Measure", "MarginBottom", "0", "", "Object's bottom margin" },
	{ PROPERTY_STYLE, "Measure", "Padding", "0", "", "padding (inner margin) for the section" },
	{ PROPERTY_STYLE, "Measure", "PaddingLeft", "0", "", "padding left (inner margin) for the section" },
	{ PROPERTY_STYLE, "Measure", "PaddingTop", "0", "", "padding top (inner margin) for the section" },
    { PROPERTY_STYLE, "bool", "Visible", "true", "", "visibility of the object (but still computed)" },
    { PROPERTY_STYLE, "bool", "Supressed", "false", "", "supression of the object (not computed)" },
    { PROPERTY_STYLE, "bool", "SupressDup", "false", "", "supression of duplicated values" },
    { PROPERTY_STYLE, "const char *", "FontFamily", "\"Helvetica\"", "", "Object's Font family"},
    { PROPERTY_STYLE, "double", "FontSize", "10", "", "Object's Font size"},
    { PROPERTY_STYLE, "int", "FontWeight", "0", "", "Object's Font weight"},
    { PROPERTY_STYLE, "bool", "FontItalic", "false", "", "whether the oject's font is italic"},
    { PROPERTY_STYLE, "int", "MaxLines", "0", "", "Object's maximum number of lines"},
    { PROPERTY_STYLE, "double", "MinFontSize", "8", "", "Object's minimun font size"},
    { PROPERTY_STYLE, "const char *", "Format", "", "", "Object's format to display the value"},
    { PROPERTY_STYLE, "const char *", "Mask", "", "", "Object's mask to display the value"},
    { PROPERTY_STYLE, "const char *", "Locale", "", "", "Object's locale to display the value"},
    { PROPERTY_STYLE, "Alignment", "HAlignment", "AlignAuto", "", "Object's horizontal alignment"},
    { PROPERTY_STYLE, "Alignment", "VAlignment", "AlignAuto", "", "Object's vertical alignment"},
    { PROPERTY_STYLE, "Adjustment", "Adjustment", "AdjustTrim", "", "Object's type of adjudstment"},
    { PROPERTY_STYLE, "Color", "FontColor", "Color::Black", "", "Object's text color"},
    { PROPERTY_STYLE, "Color", "BackColor", "Color::White", "", "Object's back color"},
    { PROPERTY_STYLE, "Color", "BorderColor", "Color::Black", "", "Object's border color"},
    { PROPERTY_STYLE, "int", "BorderWidth", "0", "", "Object's border width"},
    { PROPERTY_STYLE, "BorderStyle", "BorderStyle", "BorderNone", "", "Object's border style"},
    { PROPERTY_STYLE, "Color", "BorderTopColor", "Color::Black", "", "Object's top border color"},
    { PROPERTY_STYLE, "int", "BorderTopWidth", "0", "", "Object's top border width"},
    { PROPERTY_STYLE, "BorderStyle", "BorderTopStyle", "BorderNone", "", "Object's top border style"},
    { PROPERTY_STYLE, "Color", "BorderBottomColor", "Color::Black", "", "Object's bottom border color"},
    { PROPERTY_STYLE, "int", "BorderBottomWidth", "0", "", "Object's bottom border width"},
    { PROPERTY_STYLE, "BorderStyle", "BorderBottomStyle", "BorderNone", "", "Object's bottom border style"},
    { PROPERTY_STYLE, "Color", "BorderLeftColor", "Color::Black", "", "Object's left border color"},
    { PROPERTY_STYLE, "int", "BorderLeftWidth", "0", "", "Object's left border width"},
    { PROPERTY_STYLE, "BorderStyle", "BorderLeftStyle", "BorderNone", "", "Object's left border style"},
    { PROPERTY_STYLE, "Color", "BorderRightColor", "Color::Black", "", "Object's right border color"},
    { PROPERTY_STYLE, "int", "BorderRightWidth", "0", "", "Object's right border width"},
    { PROPERTY_STYLE, "BorderStyle", "BorderRightStyle", "BorderNone", "", "Object's right border style"},
    { PROPERTY_STYLE, "const char *", "BackgroundImage", "", "", "Name of the background image" },
    { PROPERTY_STYLE, "ImageStyle", "ImageStyle", "RTK::ImageNone", "", "Style of the object's or background image" },
    // Las mismas que section
    { PROPERTY_STYLE, "bool", "PageBefore", "false", "", "whether an end of page is printed before this section"},
    { PROPERTY_STYLE, "bool", "PageAfter", "false", "", "whether an end of page is printed after this section"},
    { PROPERTY_STYLE, "bool", "ResetNPage", "false", "", "whether this section reset de page counter to 1"},
    { PROPERTY_STYLE, "bool", "SupIfBlank", "false", "", "whether this section is supressed if contains no data"},
    { PROPERTY_STYLE, "bool", "RepeatHeader", "false", "", "whether the section's header is repeated on every page" },
    { PROPERTY_STYLE, "bool", "PrintInFirstPage", "true", "", "whether the section's header is repeated on every page" },
};

extern const char *shortlgpllicense;
void genRTKServerCopyRight(const Xtring &path, const Xtring &filename, const Xtring &desc)
{
    if( FileUtils::exists( (path + filename + ".h").c_str() ) ) {
        CppModule cppcm(path + filename + ".h");
        cppcm.addCopyLeft(RTKProject, desc, shortlgpllicense);
        cppcm.writeIfModified();
    }
    if( FileUtils::exists( (path + filename + ".cpp").c_str() ) ) {
        CppModule cppcm(path + filename + ".cpp");
        cppcm.writeIfModified();
    }
}

void gencopyright()
{
    genRTKServerCopyRight(RTKLIBPATH, "rtk", "Includes all the RTK functionality");
    genRTKServerCopyRight(RTKLIBPATH, "rtknamespace", "RTK namespace");
    genRTKServerCopyRight(RTKLIBPATH, "rtkconfig", "Config include");
    genRTKServerCopyRight(RTKLIBPATH, "rtkvariant", "RTK Variant type");
    genRTKServerCopyRight(RTKLIBPATH, "rtkformatter", "Xtring and number formatting with locale");
    genRTKServerCopyRight(RTKLIBPATH, "rtkbaseimage", "Low level image handling");
    genRTKServerCopyRight(RTKLIBPATH, "rtkcolor", "Color class");
    genRTKServerCopyRight(RTKLIBPATH, "rtkdate", "Date and time classes");
    genRTKServerCopyRight(RTKLIBPATH, "rtkxtring", "Xtring object, derived from std::string");
    genRTKServerCopyRight(RTKLIBPATH, "rtkproperty", "Property templates");
    genRTKServerCopyRight(RTKLIBPATH, "rtkfileutils", "File and file name utilities");
    genRTKServerCopyRight(RTKLIBPATH, "rtkregconfig", "Regional configuration");
    genRTKServerCopyRight(RTKLIBPATH, "rtkimage", "Upper level Image object");
    genRTKServerCopyRight(RTKLIBPATH, "rtkmoney", "Class to handle currency amounts");
    genRTKServerCopyRight(RTKLIBPATH, "rtkerror", "Class to store errors");

    genRTKServerCopyRight(RTKLIBPATH, "rtkinput", "Input object");
    genRTKServerCopyRight(RTKLIBPATH, "rtkinputfield", "Input field object");
    genRTKServerCopyRight(RTKLIBPATH, "rtkinputmysql", "Standard MySQL input object");
    genRTKServerCopyRight(RTKLIBPATH, "rtkinputpgsql", "Standard PostGreSQL input object");
    genRTKServerCopyRight(RTKLIBPATH, "rtkinputcsv", "Standard csv input object");
    genRTKServerCopyRight(RTKLIBPATH, "rtkinputxml", "Standard xml input object");


    genRTKServerCopyRight(RTKLIBPATH, "rtkoutput", "RTK output object");
    genRTKServerCopyRight(RTKLIBPATH, "rtkoutputtext", "RTK standard text output object");
    genRTKServerCopyRight(RTKLIBPATH, "rtkoutputcsv", "RTK standard csv output object");
    genRTKServerCopyRight(RTKLIBPATH, "rtkoutputhtml", "RTK HTML output object");
    genRTKServerCopyRight(RTKLIBPATH, "rtkoutputxml", "RTK XML output object");
//    genRTKServerCopyRight(RTKLIBPATH, "rtkoutputopendocument", "RTK opendocument output object");
    genRTKServerCopyRight(RTKLIBPATH, "rtkoutputqpainter", "RTK standard qpainter output object");
    genRTKServerCopyRight(RTKLIBPATH, "rtkoutputps", "RTK standard PostScript output object");
    genRTKServerCopyRight(RTKLIBPATH, "rtkoutputmysql", "RTK standard MySql output object");

    genRTKServerCopyRight(RTKLIBPATH, "rtkobject", "Base RTK object");
    genRTKServerCopyRight(RTKLIBPATH, "rtkreport", "RTK report object");
    genRTKServerCopyRight(RTKLIBPATH, "rtksection", "RTK section object");
    genRTKServerCopyRight(RTKLIBPATH, "rtkstyle", "RTK style object");
    genRTKServerCopyRight(RTKLIBPATH, "rtkparsertk", "RTK Report definition reader");
    genRTKServerCopyRight(RTKLIBPATH, "rtkparameter", "Report parameters");

    genRTKServerCopyRight(RTKVIEWPATH, "main", "RTK Viewer/printer main");
    genRTKServerCopyRight(RTKVIEWPATH, "rtkview", "RTK Viewer/printer");
    genRTKServerCopyRight(RTKVIEWPATH, "rtkqtpagedisplay", "RTK page display widget");
    genRTKServerCopyRight(RTKVIEWPATH, "rtkqtreportviewer", "RTK standard Qt report viewer");
}




// Redefinida de genCppClass para anadir properties
void GenRTKCode::addGetSet( const Xtring &filename, const Xtring &classname,
                            GetSetArray *array, int nelem, bool implem )
{
    RTKCppClass::addGetSet(filename, classname, array, nelem, implem);

    CapelModule *cmcpp = new CapelModule( mPath + filename + ".cpp" );
    Xtring s= "public:\n", scpp;

    // Funciones get/set publicas
    for ( int i = 0; i < nelem; i++ ) {
        scpp = "";
        if ( ((array[i].flags & PROPERTY_OBJECT_PARENT) == PROPERTY_OBJECT_PARENT)
                || ((array[i].flags & PROPERTY_STYLE) == PROPERTY_STYLE) ) {
            scpp = array[i].getReturnType() + " " + classname + "::";
            scpp += array[i].sName.unproper() + "() const\n"
                    "{\n"
                    "\tif( mNull" + array[i].sName + ") {\n";
            if ( (array[i].flags & PROPERTY_STYLE) == PROPERTY_STYLE) {
                scpp += "\t\tStyle *styledef = report()->findStyle(style());\n"
                        "\t\tif( styledef && !styledef->prop" + cmcpp->capitalize(array[i].sName) + ".isNull()) {\n"
                        "\t\t\treturn styledef->prop" + cmcpp->capitalize(array[i].sName) + ".get();\n"
                        "\t\t}\n";
                if ( !((array[i].flags & PROPERTY_OBJECT_PARENT) == PROPERTY_OBJECT_PARENT) )
                    scpp += "\t\treturn " + array[i].sDefValue + ";\n";
            }
            if ( (array[i].flags & PROPERTY_OBJECT_PARENT) == PROPERTY_OBJECT_PARENT) {
                scpp += "\t\tif( parent() ) {\n"
                        "\t\t\tObject *obj = static_cast<Object *>(parent());\n"
                        "\t\t\treturn obj->" + array[i].sName.unproper() + "();\n"
                        "\t\t}\n";
            }
            //                scpp += "\t\treturn " + array[i].sDefValue + ";\n";
            scpp += "\t}\n"
                    "\treturn m" + array[i].sName + ";\n"
                    "}\n";
            cmcpp->insert_extrusion( classname.upper() + "_" + array[i].sName + "_GETORIG", scpp );
        }
        // Set
        if ( array[i].flags & PROPERTY ) {
            scpp = "";
            if( classname == "Style" ) { // Style has not fixOrig
                s += "\tvoid setOrig" + array[i].sName + "( const char *" + array[i].sName.lower() + " );\n";
                scpp += "void " + classname + "::setOrig" + array[i].sName + "( const char *" + array[i].sName.lower() + " )\n"
                        "{\n"
                        "\tmOrig" + array[i].sName + " = strdup(" + array[i].sName.lower() + ");\n";
                // This function follows below
            } else  {
                s += "\tvoid setOrig" + array[i].sName + "( const char *" + array[i].sName.lower() + " ) "
                     "{ mOrig" + array[i].sName + " = strdup(" + array[i].sName.lower() + "); }\n";
                scpp += "void " + classname + "::fixOrig" + array[i].sName + "()" + "\n"
                        "{\n";
            }
            if( array[i].isNullable() ) {
                scpp += "\t" + array[i].getReturnType() + " tmp" + cmcpp->capitalize(array[i].sName)
                        + " = " + array[i].getStringToFunc() + "( ";
                if( classname != "Style" )
                    scpp += "report()->replaceParams( ";
                scpp += "mOrig" + array[i].sName;
                if( classname != "Style" )
                    scpp += " ).c_str()";
                scpp += ", &mNull" + array[i].sName + " );\n"
                        "\tif( !isNull" + array[i].sName + "() )\n"
                        "\t\tset" + array[i].sName + "( tmp" + cmcpp->capitalize(array[i].sName) + " );\n";
            } else {
                scpp += "\tset" + array[i].sName + "( ";
                if( array[i].sType != "const char *" )
                    scpp += array[i].getStringToFunc() + "( ";
                if( classname != "Style" )
                    scpp += "report()->replaceParams(";
                scpp += "mOrig" + array[i].sName;
                if( classname != "Style" )
                    scpp += ")";
                if( array[i].sType == "const char *" )
                    scpp += ".c_str() );\n";
                else
                    scpp += ") );\n";
            }
            if( array[i].sType != "Measure" && ( ( ISRTKOBJECT ) || classname == "Style" ) ) {
                scpp += "}\n";
                cmcpp->insert_extrusion( classname.upper() + "_" + array[i].sName + "_FIXORIG", scpp, "\n" );
            }
        }
    }
    s += "private:\n";
    for ( int i = 0; i < nelem; i++ ) {
        if ( array[i].flags & PROPERTY ) {
            if( array[i].sType != "Measure" && ( ISRTKOBJECT ) )
                s += "\tvoid fixOrig" + array[i].sName + "();\n";
            s += "\tchar *mOrig" + array[i].sName + ";\n";
        }
    }
    if( s!="public:\nprivate:\n" ) {
        // Protected
        CapelModule * cm = new CapelModule( mPath + filename + ".h" );
        cm->insert_extrusion( classname.upper() + "_ORIG_VALUES", s );
        cm->writeIfModified();
        delete cm;
    }
    cmcpp->writeIfModified();
    delete cmcpp;

}





#define NOT_READ_XML(classname, name ) \
    ( name == "Type" || name == "Name" || name == "GroupLevel" )

#if 0
void GenRTKCode::genReadXml( bool create, const Xtring &filename, const Xtring &classname,
                             GetSetArray *array, int nelem) {
    Xtring simplename = "p" + classname;
    CapelModule * cm = new CapelModule( mPath + filename );
    Xtring extrusion = classname.upper() + "_READXML";
    Xtring s;
    if ( create )
        s = "\t\t" + simplename + "= new " + classname + "(";
    for ( int i = 0; i < nelem; i++ ) {
        if ( create ) {
            if ( i > 0 )
                s += ",\n";
            else
                s += "\n";
        } else {
            if( NOT_READ_XML(classname, array[i].sName) )
                continue;
        }
        if( create ) {
            if( array[i].getReturnType() != "const char *" )
                s += "\t\t\t\tstringTo<" + array[i].getReturnType() + ">(";
            else
                s += "\t\t\t\t";
        } else {
            if ( array[i].flags & PROPERTY ) {
                s += "\t\t" + simplename + "->setOrig" + array[i].sName + "(";
            } else {
                s += "\t\t" + simplename + "->set" + array[i].sName + "(";
            }
        }
        s += "(const char *)getAttrValue(\"" + array[i].sName.lower() + "\", attributes)";
        if( !create )
            s += ");\n";

    }
    cm->insert_extrusion( extrusion, s );
    //  printf("\n%s\n", (const char *)cm->get_extrusion_text(extrusion));
    cm->writeIfModified();
}
#endif

void GenRTKCode::genReadRtk( bool create, const Xtring &filename,
	const Xtring &classname, GetSetArray *array, int nelem) {
    bool first = true;
    Xtring simplename = "p" + classname;
    CapelModule * cm = new CapelModule( mPath + filename );
    Xtring extrusion = classname.upper() + "_READRTK";
    Xtring s;
    for ( int i = 0; i < nelem; i++ ) {
        if( NOT_READ_XML(classname, array[i].sName) )
            continue;
        if ( array[i].flags & PROPERTY ) {
            if( !first )
                s += "\t\t\t\telse if";
            else {
                first = false;
                s += "\t\t\t\tif";
            }
            s += "( strcaseequal(token, \"" + array[i].sName + "\") )\n"
                    "\t\t\t\t\tp" + classname + "->setOrig" + cm->capitalize(array[i].sName) + "( value.c_str() );\n";
//          } else {
//                s += "\t\t" + simplename + "->set" + array[i].sName + "(";
        }
    }
    cm->insert_extrusion( extrusion, s );
    cm->writeIfModified();
}


// Define los miembros y las funciones de acceso
void genGetSet() {

    code->addGetSet( "rtkobject", "Object",
                     gsObject, sizeof( gsObject ) / sizeof( gsObject[ 0 ] ), true );
    code->addGetSet( "rtksection",
                     "Section", gsSection, sizeof( gsSection ) / sizeof( gsSection[ 0 ] ),
                     true);
    code->addGetSet( "rtkreport",
                     "Report", gsReport, sizeof( gsReport ) / sizeof( gsReport[ 0 ] ),
                     true );
    code->addConstructor( "rtkoutput",
                          "Output", gsOutput, sizeof( gsOutput ) / sizeof( gsOutput[ 0 ] ),
                          true );
    static_cast<RTKCppClass *>(code)->addGetSet( "rtkoutput",
            "Output", gsOutput, sizeof( gsOutput ) / sizeof( gsOutput[ 0 ] ),
            true );
    code->addGetSet( "rtkoutputtext",
                     "OutputText", gsOutputText, sizeof( gsOutputText ) / sizeof( gsOutputText[ 0 ] ),
                     true );
    code->addGetSet( "rtkoutputcsv",
                     "OutputCsv", gsOutputCsv, sizeof( gsOutputCsv ) / sizeof( gsOutputCsv[ 0 ] ),
                     true );
    code->addGetSet( "rtkoutputps",
                     "OutputPS", gsOutputPS, sizeof( gsOutputPS ) / sizeof( gsOutputPS[ 0 ] ),
                     true );
    code->addGetSet( "rtkoutputmysql",
                     "OutputMySQL", gsOutputMySql, sizeof( gsOutputMySql ) / sizeof( gsOutputMySql[ 0 ] ),
                     true );
/*
    code->addDerivedConstructor( "rtkoutputqpainter",
                                 "Output", gsOutput, sizeof( gsOutput ) / sizeof( gsOutput[ 0 ] ),
                                 "OutputQPainter", gsOutputQPainter, sizeof( gsOutputQPainter ) / sizeof( gsOutputQPainter[ 0 ] ),
                                 true );
    code->addGetSet( "rtkoutputqpainter",
                     "OutputQPainter", gsOutputQPainter, sizeof( gsOutputQPainter ) / sizeof( gsOutputQPainter[ 0 ] ),
                     true );
    code->addGetSet( "rtkoutputopendocument",
                     "OutputODFCalc1", gsOutputODFCalc1, sizeof( gsOutputODFCalc1 ) / sizeof( gsOutputODFCalc1[ 0 ] ),
                     true );
*/
    code->addGetSet( "rtkoutputxml",
                     "OutputXml", gsOutputXml, sizeof( gsOutputXml ) / sizeof( gsOutputXml[ 0 ] ),
                     true );
    code->addGetSet( "rtkoutputhtml",
                     "OutputHtml", gsOutputHtml, sizeof( gsOutputHtml ) / sizeof( gsOutputHtml[ 0 ] ),
                     true );

    code->addConstructor( "rtkparameter",
                          "Parameter", gsParameter, sizeof( gsParameter ) / sizeof( gsParameter[ 0 ] ),
                          true );
    code->addGetSet( "rtkparameter",
                     "Parameter", gsParameter, sizeof( gsParameter ) / sizeof( gsParameter[ 0 ] ),
                     true );
    code->addConstructor( "rtkimage",
                          "Image", gsImage, sizeof( gsImage ) / sizeof( gsImage[ 0 ] ),
                          true );
    code->addGetSet( "rtkimage",
                     "Image", gsImage, sizeof( gsImage ) / sizeof( gsImage[ 0 ] ),
                     true );
}


// Genera codigo para el parser de rtkreport.cpp
void genReadRtk() {
    /* No es correcto
       code->genReadRtk("rtkreport.cpp",
       gsReport, sizeof(gsReport)/sizeof(gsReport[0]),
       "Report", "Report", "Report");
    */
    uint i, j, l;
    // Unir las propiedades de Object y Report
    GetSetArray *gsReportObject = new GetSetArray[
                                      l = ( sizeof( gsReport ) / sizeof( gsReport[ 0 ] ) +
                                            sizeof( gsObject ) / sizeof( gsObject[ 0 ] ) ) ];
    for ( i = j = 0; i < sizeof( gsReport ) / sizeof( gsReport[ 0 ] ); j++, i++ )
        gsReportObject[ j ] = gsReport[i];
    for ( i = 0; i < sizeof( gsObject ) / sizeof( gsObject[ 0 ] ); j++, i++ )
        gsReportObject[ j ] = gsObject[i];
    code->genReadRtk( false, "rtkparsertk.cpp", "Report", gsReportObject, l );
    delete [] gsReportObject;

    // Unir las propiedades de Object y Section
    GetSetArray *gsSectionObject = new GetSetArray[
                                       l = ( sizeof( gsSection ) / sizeof( gsSection[ 0 ] ) +
                                             sizeof( gsObject ) / sizeof( gsObject[ 0 ] ) ) ];
    for ( i = j = 0; i < sizeof( gsSection ) / sizeof( gsSection[ 0 ] ); j++, i++ )
        gsSectionObject[ j ] = gsSection[i];
    for ( i = 0; i < sizeof( gsObject ) / sizeof( gsObject[ 0 ] ); j++, i++ )
        gsSectionObject[ j ] = gsObject[i];
    code->genReadRtk( false, "rtkparsertk.cpp", "Section", gsSectionObject, l);
    delete [] gsSectionObject;

    code->genReadRtk( false, "rtkparsertk.cpp", "Style",
                      gsStyle, sizeof( gsStyle ) / sizeof( gsStyle[ 0 ] ) );
    code->genReadRtk( false, "rtkparsertk.cpp", "Object",
                      gsObject, sizeof( gsObject ) / sizeof( gsObject[ 0 ] ));
    /*
        code->genReadRtk( true, "rtkparsertk.cpp", "Input",
                          gsInput, sizeof( gsInput ) / sizeof( gsInput[ 0 ] ),
                          "Report", "Input" );
    */
    code->genReadRtk( false, "rtkparsertk.cpp", "InputField",
                      gsInputField, sizeof( gsInputField ) / sizeof( gsInputField[ 0 ] ) );
    code->genReadRtk( false, "rtkparsertk.cpp", "InputCsv",
                      gsInputCsv, sizeof( gsInputCsv ) / sizeof( gsInputCsv[ 0 ] ) );
    code->genReadRtk( false, "rtkparsertk.cpp", "InputMySql",
                      gsInputMySql, sizeof( gsInputMySql ) / sizeof( gsInputMySql[ 0 ] ) );
    code->genReadRtk( false, "rtkparsertk.cpp", "InputXBase",
                      gsInputXBase, sizeof( gsInputXBase ) / sizeof( gsInputXBase[ 0 ] ) );
    code->genReadRtk( false, "rtkparsertk.cpp", "InputPgSql",
                      gsInputPgSql, sizeof( gsInputPgSql ) / sizeof( gsInputPgSql[ 0 ] ) );
    code->genReadRtk( false, "rtkparsertk.cpp", "Image",
                      gsImage, sizeof( gsImage ) / sizeof( gsImage[ 0 ] ) );

    code->genReadRtk( true, "rtkparsertk.cpp", "Parameter",
                      gsParameter, sizeof( gsParameter ) / sizeof( gsParameter[ 0 ] ) - 1 );
}



Xtring GenRTKCode::addOneTest(const Xtring &classname, GetSetArray *getset)
{
    Xtring origvalue = getset->sDefValue;
    if( origvalue[0]!='\"' )
        origvalue = "\"" + origvalue + "\"";
    Xtring defvalue = getset->sDefValue;
    if( getset->sType == "const char *" && !origvalue.size() ) {
        defvalue = "\"\"";
    } else if( getset->sType == "const char *" ) {
        defvalue = origvalue;
/*      if( defvalue.size() )
            origvalue = "\"" + origvalue.substr(1, origvalue.length()-2) + "\"";
        else
            origvalue = "\"\"";*/
    }
    Xtring s;
    if( (getset->flags & PROPERTY_READONLY) != PROPERTY_READONLY ) {
        Xtring sdef = getset->sDefValue;
        if( sdef.size() )
            sdef = ", " + sdef;
        s += "\tTEST_ASSERT( p" + classname + "->prop" + getset->sName + ".isNull() );\n";
        s += "\tp" + classname + "->prop" + getset->sName + ".set(value[0]);\n";
        s += "\tTEST_ASSERT( !p" + classname + "->prop" + getset->sName + ".isNull() );\n";
        s += "\tTEST_ASSERT( p" + classname + "->prop" + getset->sName + " == value[0] );\n";
        s += "\tp" + classname + "->prop" + getset->sName + ".setOrig(valueorig[1]);\n";
        s += "\tp" + classname + "->prop" + getset->sName + ".fix(ParametersList(), \"\"" + sdef + ");\n";
        s += "\tTEST_ASSERT( !p" + classname + "->prop" + getset->sName + ".isNull() );\n";
        s += "\tTEST_ASSERT( p" + classname + "->prop" + getset->sName + " == value[1]);\n";
        s += "\tp" + classname + "->prop" + getset->sName + ".set(value[1]);\n";
        s += "\tTEST_ASSERT( !p" + classname + "->prop" + getset->sName + ".isNull() );\n";
        s += "\tTEST_ASSERT( p" + classname + "->prop" + getset->sName + " == value[1] );\n";
        s += "\tp" + classname + "->prop" + getset->sName + ".setOrig(\"\");\n";
        s += "\tp" + classname + "->prop" + getset->sName + ".fix(ParametersList(), \"\"" + sdef + ");\n";
        s += "\tTEST_ASSERT( p" + classname + "->prop" + getset->sName + ".isNull() );\n";
        s += "\tTEST_ASSERT( p" + classname + "->prop" + getset->sName + " == " + defvalue + ");\n";
        s += "\tp" + classname + "->prop" + getset->sName + ".set(" + defvalue + ");\n";
        s += "\tTEST_ASSERT( !p" + classname + "->prop" + getset->sName + ".isNull() );\n";
        s += "\tTEST_ASSERT( p" + classname + "->prop" + getset->sName + " == " + defvalue + ");\n";
        s += "\tp" + classname + "->prop" + getset->sName + ".setOrig(valueorig[2]);\n";
        s += "\tp" + classname + "->prop" + getset->sName + ".fix(ParametersList(), \"\"" + sdef + ");\n";
        s += "\tTEST_ASSERT( !p" + classname + "->prop" + getset->sName + ".isNull() );\n";
        s += "\tTEST_ASSERT( p" + classname + "->prop" + getset->sName + " == value[2] );\n";
        s += "\tp" + classname + "->prop" + getset->sName + ".setOrig(0);\n";
        s += "\tp" + classname + "->prop" + getset->sName + ".fix(ParametersList(), \"\"" + sdef + ");\n";
        s += "\tTEST_ASSERT( p" + classname + "->prop" + getset->sName + ".isNull() );\n";
        s += "\tTEST_ASSERT( p" + classname + "->prop" + getset->sName + " == " + defvalue + ");\n";
    }
    return s;
}

void gentests() {
    /*
        code->addTests( RTKTESTSPATH "testinput", "Input", gsInput, sizeof( gsInput ) / sizeof( gsInput[ 0 ] ));
        code->addTests( RTKTESTSPATH "testinputfield", "InputField", gsInputField, sizeof( gsInputField ) / sizeof( gsInputField[ 0 ] ));
        code->addTests( RTKTESTSPATH "testinputcsv", "InputCsv", gsInputCsv, sizeof( gsInputCsv ) / sizeof( gsInputCsv[ 0 ] ));
        code->addTests( RTKTESTSPATH "testinputsql", "InputSql", gsInputSql, sizeof( gsInputSql ) / sizeof( gsInputSql[ 0 ] ));
        code->addTests( RTKTESTSPATH "testoutput", "Output", gsOutput, sizeof( gsOutput ) / sizeof( gsOutput[ 0 ] ));
        code->addTests( RTKTESTSPATH "testoutputtext", "OutputText", gsOutputText, sizeof( gsOutputText ) / sizeof( gsOutputText[ 0 ] ));
        code->addTests( RTKTESTSPATH "testoutputqpainter", "OutputQPainter", gsOutputQPainter, sizeof( gsOutputQPainter ) / sizeof( gsOutputQPainter[ 0 ] ));
        code->addTests( RTKTESTSPATH "testoutputopenoffice", "OutputODFCalc1", gsOutputODFCalc1, sizeof( gsOutputODFCalc1 ) / sizeof( gsOutputODFCalc1[ 0 ] ));
        code->addTests( RTKTESTSPATH "testoutputxml", "OutputXml", gsOutputXml, sizeof( gsOutputXml ) / sizeof( gsOutputXml[ 0 ] ));
        code->addTests( RTKTESTSPATH "testimage", "Image", gsImage, sizeof( gsImage ) / sizeof( gsImage[ 0 ] ));
        */
    code->addTests( RTKTESTSPATH "teststyle", "Style", gsStyle, sizeof( gsStyle ) / sizeof( gsStyle[ 0 ] ));
    code->addTests( RTKTESTSPATH "testobject", "Object", gsObject, sizeof( gsObject ) / sizeof( gsObject[ 0 ] ));
    code->addTests( RTKTESTSPATH "testsection", "Section", gsSection, sizeof( gsSection ) / sizeof( gsSection[ 0 ] ) );
//    code->addTests( RTKTESTSPATH "testreport", "Report", gsReport, sizeof( gsReport ) / sizeof( gsReport[ 0 ] ) );
}


void GenRTKCode::addProperties(const Xtring &filename, const Xtring &classname,
                               GetSetArray *array, int nelem)
{
    CapelModule *cm = new CapelModule(mPath + filename + ".h");
    bool hayfixables = false;
    Xtring s = "public:\n";
    Xtring spubsetorig, spubget, spublic = "public:\n";
    for( int i=0; i<nelem; i++) {
        // Property
        if( array[i].isProperty() ) {
            spublic += "\t" + array[i].getReturnType() + " " + array[i].sName.unproper() + "() const";
            spubget =" { return prop" + cm->capitalize(array[i].sName) + ".get(); }\n";
            spubsetorig = "\tvoid setOrig" + cm->capitalize(array[i].sName) + "(const char * " + array[i].sName.lower() + " )"
                    " { prop" + cm->capitalize(array[i].sName) + ".setOrig(" + array[i].sName.lower() + "); }\n";
            if( (array[i].flags & PROPERTY_STYLE) == PROPERTY_STYLE ) {
                s+="\tFixableProperty<" + array[i].getReturnType() + "> prop" + cm->capitalize(array[i].sName) + ";\n";
                spublic += spubget + spubsetorig;
                hayfixables = true;
            } else if( (array[i].flags & PROPERTY_FIX) == PROPERTY_FIX ) {
                s+="\tFixableProperty<" + array[i].getReturnType() + "> prop" + cm->capitalize(array[i].sName) + ";\n";
                spublic += spubget + spubsetorig;
                hayfixables = true;
            } else if( (array[i].flags & PROPERTY_OBJECT_PARENT) == PROPERTY_OBJECT_PARENT ) {
                s+="\tFixableProperty<" + array[i].getReturnType() + "> prop" + cm->capitalize(array[i].sName) + ";\n";
                spublic += ";\n" + spubsetorig;
                hayfixables = true;
            } else if( (array[i].flags & PROPERTY_OBJECT_STYLE) == PROPERTY_OBJECT_STYLE ) {
                s+="\tFixableProperty<" + array[i].getReturnType() + "> prop" + cm->capitalize(array[i].sName) + ";\n";
                spublic += ";\n" + spubsetorig;
                hayfixables = true;
            } else if( (array[i].flags & PROPERTY_READONLY) == PROPERTY_READONLY ) {
                s+="\tReadOnlyProperty<" + array[i].getReturnType() + "> prop" + cm->capitalize(array[i].sName) + ";\n";
                spublic += spubget;
            } else {
                s+="\tProperty<" + array[i].getReturnType() + "> prop" + cm->capitalize(array[i].sName) + ";\n";
                spublic += spubget;
            }

        }
    }
    if( hayfixables ) {
		if( classname == "Input" )
			;
		else {
	        spublic += "\tvirtual void fix";
			if( classname.find( "Input" ) != Xtring::npos && classname != "InputField" )
				spublic += "Input";
			spublic += "Parameters(const ParametersList &parameters, const char *delim); // from Input\n";
		}
    }
    cm->insert_extrusion(classname.upper() + "_PROPERTIES", spublic + "\n" + s);
    cm->writeIfModified();
    delete cm;
    if( hayfixables && classname != "Input" ) {
        cm = new CapelModule(mPath + filename + ".cpp");
        s = "";
        for( int i=0; i<nelem; i++) {
            // Property
            if( (array[i].flags & PROPERTY_FIX) == PROPERTY_FIX ||
                (array[i].flags & PROPERTY_OBJECT_PARENT) == PROPERTY_OBJECT_PARENT ||
                (array[i].flags & PROPERTY_STYLE) == PROPERTY_STYLE  ||
                (array[i].flags & PROPERTY_OBJECT_STYLE) == PROPERTY_OBJECT_STYLE ) {
				if( classname == "Object" || classname == "Section" ) {
					if( array[i].sDefValue.size() )
						s+="\tif( !prop" + cm->capitalize(array[i].sName) + ".fix( parameters, delim, "
						+ array[i].sDefValue + " ) )\n";
					else
						s+="\tif( !prop" + cm->capitalize(array[i].sName) + ".fix( parameters, delim ) )\n";
					s+="\t\treport()->addWarning( Error::InvalidArgument, 0, 0, prop" 
						+ cm->capitalize(array[i].sName) + ".getOrig(), \"" + cm->capitalize(array[i].sName) +"\");\n";
				} else {
					if( array[i].sDefValue.size() )
						s+="\tprop" + cm->capitalize(array[i].sName) + ".fix( parameters, delim, "
						+ array[i].sDefValue + " );\n";
					else
						s+="\tprop" + cm->capitalize(array[i].sName) + ".fix( parameters, delim );\n";
				}
            }
        }
        cm->insert_extrusion(classname.upper() + "_FIXPARAMETERS", s, "\n\n}\n",
                    Xtring("void ") + classname
                        + "::fixParameters(const ParametersList &parameters, const char *delim)\n{\n");
        cm->writeIfModified();
        delete cm;
    }
}

void GenRTKCode::addPropertiesGet( const Xtring &filename, const Xtring &classname,
                                   GetSetArray *array, int nelem)
{
    CapelModule *cmcpp = new CapelModule( mPath + filename + ".cpp" );
    Xtring scpp;

    // Funciones get/set publicas
    for ( int i = 0; i < nelem; i++ ) {
        scpp = "";
        if ( ((array[i].flags & PROPERTY_OBJECT_PARENT) == PROPERTY_OBJECT_PARENT)
                     || ((array[i].flags & PROPERTY_OBJECT_STYLE) == PROPERTY_OBJECT_STYLE) ) {
            scpp = array[i].getReturnType() + " " + classname + "::";
            scpp += array[i].sName.unproper() + "() const\n"
                    "{\n"
                    "\tif( prop" + array[i].sName + ".isNull() ) {\n";
            if ( (array[i].flags & PROPERTY_OBJECT_STYLE) == PROPERTY_OBJECT_STYLE) {
                scpp += "\t\tStyle *styledef = report()->findStyle(style());\n"
                        "\t\tif( styledef && !styledef->prop" + cmcpp->capitalize(array[i].sName) + ".isNull()) {\n"
                        "\t\t\treturn styledef->prop" + cmcpp->capitalize(array[i].sName) + ".get();\n"
                        "\t\t}\n";
/*
                if ( !((array[i].flags & PROPERTY_OBJECT_PARENT) == PROPERTY_OBJECT_PARENT) )
                    scpp += "\t\treturn " + array[i].sDefValue + ";\n";
*/
            }
            if ( (array[i].flags & PROPERTY_OBJECT_PARENT) == PROPERTY_OBJECT_PARENT) {
                scpp += "\t\tif( parent() ) {\n"
                        "\t\t\tObject *obj = static_cast<Object *>(parent());\n"
                        "\t\t\treturn obj->" + array[i].sName.unproper() + "();\n"
                        "\t\t}\n";
            }
            //                scpp += "\t\treturn " + array[i].sDefValue + ";\n";
            scpp += "\t}\n"
                    "\treturn prop" + array[i].sName + ".get();\n"
                    "}\n";
            cmcpp->insert_extrusion( classname.upper() + "_" + array[i].sName + "_GET", scpp );
                     }
    }
    cmcpp->writeIfModified();
    delete cmcpp;

}

void GenRTKCode::addGetPropValues( const Xtring &filename, const Xtring &classname,
                                   GetSetArray *array, int nelem)
{
    CapelModule *cmcpp = new CapelModule( mPath + filename + ".cpp" );
    Xtring scppget, scppset, scppsetorig;

    // Funciones get/set publicas
    for ( int i = 0; i < nelem; i++ ) {
        if( ((array[i].flags & PROPERTY_FIX) == PROPERTY_FIX)
			|| ( (array[i].flags & PROPERTY_OBJECT_STYLE) == PROPERTY_OBJECT_STYLE ) ) {
            scppget += "\telse if( strcasecmp(name, \"" + array[i].sName + "\") == 0 )\n"
				"\t\treturn prop" + array[i].sName + ".get()";
            scppset += "\telse if( strcasecmp(name, \"" + array[i].sName + "\") == 0 )\n"
				"\t\tprop" + array[i].sName + ".set( " + array[i].getVariantTo( "value")  + ")";
            scppsetorig += "\telse if( strcasecmp(name, \"" + array[i].sName + "\") == 0 )\n"
				"\t\tprop" + array[i].sName + ".setOrig( value.c_str() )";
			if( array[i].sType == "Color" )
				scppget += ".toUInt()";
			scppget += ";\n";
			scppset += ";\n";
			scppsetorig += ";\n";
        }
    }
    cmcpp->insert_extrusion( classname.upper() + "_GETPROPVALUE", scppget );
    cmcpp->insert_extrusion( classname.upper() + "_SETPROPVALUE", scppset );
    cmcpp->insert_extrusion( classname.upper() + "_SETORIGPROPVALUE", scppsetorig );
    cmcpp->writeIfModified();
    delete cmcpp;
}


#define ADD_PROPERTIES(filename, litclass, classname) \
    code->addProperties( filename, litclass, gs##classname, sizeof( gs##classname ) / sizeof( gs##classname[ 0 ] ) )

void genProperties()
{
    ADD_PROPERTIES( "rtkobject", "Object", Object );
    code->addPropertiesGet( "rtkobject", "Object", gsObject, sizeof( gsObject ) / sizeof( gsObject[ 0 ] ) );
    code->addGetPropValues( "rtkobject", "Object", gsObject, sizeof( gsObject ) / sizeof( gsObject[ 0 ] ) );
    ADD_PROPERTIES( "rtksection", "Section", Section );
    code->addPropertiesGet( "rtksection", "Section", gsSection, sizeof( gsSection ) / sizeof( gsSection[ 0 ] ) );
    code->addGetPropValues( "rtksection", "Section", gsSection, sizeof( gsSection ) / sizeof( gsSection[ 0 ] ) );
    ADD_PROPERTIES( "rtkreport", "Report", Report );
    code->addGetPropValues( "rtkreport", "Report", gsReport, sizeof( gsReport ) / sizeof( gsReport[ 0 ] ) );

    ADD_PROPERTIES( "rtkinput", "Input", Input );
    ADD_PROPERTIES( "rtkinputfield", "InputField", InputField );
    ADD_PROPERTIES( "rtkinputcsv", "InputCsv", InputCsv );
    ADD_PROPERTIES( "rtkinputmysql", "InputMySql", InputMySql );
    ADD_PROPERTIES( "rtkinputxbase", "InputXBase", InputXBase );
    ADD_PROPERTIES( "rtkinputpgsql", "InputPgSql", InputPgSql );

    ADD_PROPERTIES( "rtkoutput", "Output", Output );
    ADD_PROPERTIES( "rtkoutputtext", "OutputText", OutputText );
    ADD_PROPERTIES( "rtkoutputcsv", "OutputCsv", OutputCsv );
    ADD_PROPERTIES( "rtkoutputxml", "OutputXml", OutputXml );
    ADD_PROPERTIES( "rtkoutputhtml", "OutputHtml", OutputHtml );
//    ADD_PROPERTIES( "rtkoutputopenoffice", "OutputODFCalc1", OutputODFCalc1 );
    ADD_PROPERTIES( "rtkoutputps", "OutputPS", OutputPS );
    ADD_PROPERTIES( "rtkoutputmysql", "OutputMySql", OutputMySql );


    ADD_PROPERTIES( "rtkstyle", "Style", Style );
    ADD_PROPERTIES( "rtkimage", "Image", Image );
    ADD_PROPERTIES( "rtkparameter", "Parameter", Parameter );

}



void genrtk() {
    fprintf( stderr, "Iniciado genrtk\n" );
    code = new GenRTKCode( RTKLIBPATH );
	gencopyright();
    genReadRtk();
    genProperties();
#if 0
    gentests();
#endif
    fprintf( stderr, "Acabado genrtk\n" );
}


}
