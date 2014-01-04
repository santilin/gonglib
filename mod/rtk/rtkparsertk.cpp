#include <cerrno>
#include <gongdebug.h>
#include <gongfileutils.h>

#include "rtkparameter.h"
#include "rtkerror.h"
#include "rtkinput.h"
#include "rtkinputcsv.h"
#include "rtkinputmysql.h"
#include "rtkinputxbase.h"
#include "rtkinputpgsql.h"
#include "rtkreport.h"

namespace gong {}
namespace RTK = gong;

namespace gong {
// This class is used by any input driver that is not supported.
// This is to avoid having errors while parsing the report.
class NullInput: public Input
{
public:
    NullInput( class Report &r, const char * name, const char *driver = 0 )
        : Input( r, name, driver )
    {}
    ;
    virtual bool init()
    {
        return false;
    }
    virtual bool next()
    {
        return false;
    }
    virtual Variant getValue( uint sourcepos, Variant::Type type ) const
    {
        return Variant();
    }
};

/**
	@class ReportDefParser
	@brief Parses an .rtk file into a Report definition
	This is an internal class used only by Report::readRtkFile
 */
class ReportDefParser
{
public:
    ReportDefParser( Report *report );
    bool parseReport( const char *text, const char *defaultinput,
                      const char *filename = 0, Input *usethisinput = 0, const Xtring &initdefines = Xtring::null );

private:
    Xtring getNextToken( const char **text );
    Xtring getNextValue( const char **text );
    bool insertFile( const char *filename, const char **wholetext, const char **text, bool deleteoldbuffer );
    Section *parseSection( Section::SectionType type, const char *name, const char **text, bool skipping = false );
    Object *parseObject( Section *owner, const char *name, const char **text, bool skipping = false );
    Style *parseStyle( const char *name, const char **text, bool skipping = false );
    Parameter *parseParameter( const char *name, const char **text, bool skipping = false );
    Image *parseImage( const char *name, const char **text, bool skipping = false );
    Input *parseInput( const char *name, Input *pInput, const char **text, bool skipping = false );
    InputField *parseInputField( Input *owner, const char *name, const char **text, bool skipping = false );

    Report *pReport;
    const char *pFileName;
    int mLine, mCol;
    List<Xtring> mDefines;
    List<Xtring> mIfdefs;

    static const bool skipping = true;
};

ReportDefParser::ReportDefParser( Report *report )
{
    pReport = report;
    mLine = 1;
    mCol = 1;
}


/**
 * @brief Inserts a file into the parsed string and updates the text pointers
 *
 * @param filename
 * @param wholetext
 * @param text
 * @param deleteoldbuffer
 * @return
 */
/// \todo {refactor} Make FileUtils::readFile
bool ReportDefParser::insertFile(const char *filename, const char ** wholetext,
                                 const char ** text, bool deleteoldbuffer)
{
    std::ifstream f;
    _GONG_DEBUG_PRINT(5, pReport->includePath() );
    Xtring realfile = FileUtils::findInPath( pReport->includePath(), filename, "informes" );
    f.open( realfile.c_str(), std::ios_base::binary | std::ios_base::in );
    if ( !f.is_open() || !f.good() ) {
        _GONG_DEBUG_WARNING( Xtring("File ") + filename + " not found in " + pReport->includePath() );
        return false;
    }
    f.seekg( 0, std::ios_base::end );
    long end_pos = f.tellg();
    f.seekg( 0, std::ios_base::beg );
    long remaininglen = strlen(*text);
    long newlen = (end_pos + 1) + remaininglen;
    // Allocate a new buffer
    char *buffer = new char[ newlen ];
    f.read( buffer, end_pos );
    strcpy( buffer + end_pos, *text );
    if( deleteoldbuffer )
        delete [] *wholetext;
    *wholetext = buffer;
    *text = buffer;
    return true;
}

#define SKIP_LINE \
	while( *code ) { \
		if ( *code++=='\n' ) { \
			mLine++;mCol=1; \
			break; \
		} \
	}
#define SKIP_SPACES \
			while ( *code && isspace(*code) ) { \
				if ( *code == '\n' ) { \
					mLine++; \
					mCol = 1; \
					code++; \
				} else { \
					mCol++, code++; \
				} \
			}
#define SKIP_NOT_SPACES \
			while ( *code && !isspace(*code) ) { \
				mCol++; \
				code++; \
			}

Xtring ReportDefParser::getNextToken( const char **starttoken )
{
    const char * code = *starttoken;
    Xtring ret;

    while ( *code ) {
        SKIP_SPACES;
        if ( !*code ) {
            _GONG_DEBUG_PRINT(10, "(null)" );
            return Xtring::null;
        }
        *starttoken = code;
        // Preprocessor directive
        while( mCol == 1 && *code == '#' ) {
            code++;
            *starttoken = code;
            SKIP_NOT_SPACES;
            Xtring directive( Xtring(*starttoken, code - *starttoken).lower() );
            SKIP_SPACES;
            *starttoken = code;
            bool skip_code = false;
            if( directive == "define" ) {
                SKIP_NOT_SPACES;
                mDefines.push_back( Xtring(*starttoken, code - *starttoken) );
                SKIP_LINE;
            } else if( directive == "undef" ) {
                SKIP_NOT_SPACES;
                Xtring prep_name( *starttoken, code - *starttoken );
                for( uint del=0; del < mDefines.size(); del ++ )
                    if( mDefines[del] == prep_name )
                        mDefines.erase( mDefines.begin() + del );
                SKIP_LINE;
            } else if( directive == "ifdef" ) {
                uint search;
                SKIP_NOT_SPACES;
                Xtring prep_name( *starttoken, code - *starttoken );
                skip_code = true;
                for( search=0; search < mDefines.size() && skip_code; search ++ )
                    if( mDefines[search] == prep_name )
                        skip_code = false;
                SKIP_LINE;
            } else if( directive == "ifndef" ) {
                uint search;
                SKIP_NOT_SPACES;
                Xtring prep_name( *starttoken, code - *starttoken );
                skip_code = false;
                for( search=0; search < mDefines.size() && !skip_code; search ++ )
                    if( mDefines[search] == prep_name )
                        skip_code = true;
                SKIP_LINE;
            } else if( directive.lower() == "endif" ) {
                // Do nothing
            } else {
                return directive;
            }
            if( skip_code ) {
                int ifdefs = 0;
                while( *code ) {
                    if( strncmp( code, "#ifdef", 6 ) == 0 ) {
                        ifdefs++;
                    } else if( strncmp( code, "#else", 5 ) == 0 ) {
                        if( --ifdefs < 0 )
                            break;
                    } else if( strncmp( code, "#ifndef", 6 ) == 0 ) {
                        ifdefs++;
                    } else if( strncmp( code, "#endif", 6 ) == 0 ) {
                        if( ifdefs-- == 0 ) {
                            break;
                        }
                    }
                    SKIP_LINE
                }
                SKIP_LINE
            }
            SKIP_SPACES
            *starttoken = code;
        }
        // Normal token
        if ( isdigit( *code ) ) { // Number constant
            while ( isdigit( *code ) || *code == 'e' || *code == '.' ) {
                mCol++, code++;
            }
            ret = Xtring( *starttoken, code - *starttoken );
            *starttoken = code;
            _GONG_DEBUG_PRINT(10, ret );
            return ret;
        } else if ( isalpha( *code ) || *code == '_' || ( unsigned char ) ( *code ) >= '\x7f' ) { // a name
            while ( isalpha( *code ) || isdigit( *code ) || *code == '_' || ( unsigned char ) ( *code ) >= '\x7f' ) {
                mCol++, code++;
            }
            ret = Xtring( *starttoken, code - *starttoken );
            *starttoken = code;
            _GONG_DEBUG_PRINT(10, ret );
            return ret;
        } else { // May be an operator, comment or string literal
            char op = '\0';
            switch ( op = *code ) {
            case '\"':
            case '\'': { // Todo "hola""adios" y escape chars
                while ( *code != op && *code ) {
                    if ( *code == '\n' ) {
                        mLine++;
                        mCol = 1;
                        code++;
                    } else {
                        mCol++, code++;
                    }
                }
                if ( *code ) {
                    ret = Xtring( *starttoken, code - *starttoken );
                    *starttoken = code;
                    _GONG_DEBUG_PRINT(10, ret );
                    return ret;
                } else {
                    *starttoken = code;
                    _GONG_DEBUG_PRINT(10, "(error)" );
                    return Xtring::null; // Error
                }
            }
            case '-':  // May be a comment
            case '/':
                if ( *( code + 1 ) == *code ) {
                    SKIP_LINE
                } else {
                    *starttoken = code;
                    _GONG_DEBUG_PRINT(10, *code );
                    return Xtring( 1, *code );
                }
                break;
            default:
                *starttoken = code + 1;
                _GONG_DEBUG_PRINT(10, *code );
                return Xtring( 1, *code );
            }
        }
    }
    _GONG_DEBUG_PRINT(10, ret );
    return ret;
}


// Values can be between quotes, or if not, up to the next un-quoted semicolon
// If the value is not quoted, only the next word is taken

/// \todo {add} Check line numbers
Xtring ReportDefParser::getNextValue( const char **starttoken )
{
    const char * code = *starttoken;
    char quotes = '\0';
    bool escaping = false;
    Xtring ret;
    if ( *code ) {
        while ( isspace( *code ) && *code && *code != ';' && *code != '\n' ) {
            mCol++, code++;
        }
        if ( !*code || *code == ';' || *code == '\n' )
            return Xtring::null;
        if ( *code == '\"' || *code == '\'' ) { // Initial quotes
            quotes = *code++;
        }
        *starttoken = code;
        while ( *code ) {
            if( escaping ) {
                escaping = false;
            } else {
                if ( *code == quotes ) { /// \todo concatenate "abc" "def" ?
                    ret = Xtring( *starttoken, code - *starttoken );
                    *starttoken = code + 1;
                    break;
                } else if ( *code == '\\' ) {
                    escaping = true;
                } else if ( *code == ';' || *code == '\n' || *code == ' ' ) {
                    if ( !quotes ) {
                        ret = Xtring( *starttoken, code - *starttoken ).trimRight();
                        *starttoken = code;
                        break;
                    }
                }
                if ( *code == '\n' ) {
                    mLine++;
                    mCol = 0; // It will be incremented below
                }
            }
            mCol++;
            code++;
        }
    }
    return ret.replace("\\\n", "").replace("\\t", "\t").replace("\\\r", "")
           .replace("\\\"", "\"");
}


bool ReportDefParser::parseReport( const char *wholetext, const char *defaultinput,
                                   const char *filename, Input *usethisinput, const Xtring &initdefines )
{
    const char *initialtext = wholetext;
    const char **text = &initialtext;
    Parameter *pParameter = 0;
    Input *pInput = 0;
    bool bufferExpanded = false;

    _GONG_DEBUG_TRACE(4);
    initdefines.tokenize(mDefines, ",");
    pFileName = filename;
    Xtring token = getNextToken( text );
    if ( strcaseequal( token, "report" ) || strcaseequal( token, "rtkreport" ) ) {
        token = getNextToken( text );
        // Name is optional
        if ( !strequal( token, "{" ) ) {
            pReport->setName( token.c_str() );
            token = getNextToken( text );
            if ( !strequal( token, "{" ) ) {
                pReport->addError( Error::ParseReportOpenBracket, __FUNCTION__, 0, pFileName, mLine, mCol,
                                   pReport->name(), "REPORT", pReport->name() );
                return false;
            }
        }
        // Element or property
        while ( ( token = getNextToken( text ) ) != "}" ) {
            Xtring name = getNextToken( text );
            bool skipsemicolon = false;
            if ( strequal( name, "=" ) ) {
                // Report Properties
                Xtring value = getNextValue( text );
                if ( value.isEmpty() )
                    pReport->addWarning( Error::ParseReportValueMissing, __FUNCTION__, 0, pFileName, mLine, mCol,
                                         token.c_str(), "REPORT", pReport->name() );
                /*<<<<<REPORT_READRTK*/
                if( strcaseequal(token, "Title") )
                    pReport->setOrigTitle( value.c_str() );
                else if( strcaseequal(token, "Units") )
                    pReport->setOrigUnits( value.c_str() );
                else if( strcaseequal(token, "PageOrientation") )
                    pReport->setOrigPageOrientation( value.c_str() );
                else if( strcaseequal(token, "StylesPath") )
                    pReport->setOrigStylesPath( value.c_str() );
                else if( strcaseequal(token, "ImagesPath") )
                    pReport->setOrigImagesPath( value.c_str() );
                else if( strcaseequal(token, "IncludePath") )
                    pReport->setOrigIncludePath( value.c_str() );
                else if( strcaseequal(token, "ParamDelim") )
                    pReport->setOrigParamDelim( value.c_str() );
                else if( strcaseequal(token, "Order") )
                    pReport->setOrigOrder( value.c_str() );
                else if( strcaseequal(token, "Filter") )
                    pReport->setOrigFilter( value.c_str() );
                else if( strcaseequal(token, "Copies") )
                    pReport->setOrigCopies( value.c_str() );
                else if( strcaseequal(token, "PagesPerFolio") )
                    pReport->setOrigPagesPerFolio( value.c_str() );
                else if( strcaseequal(token, "Value") )
                    pReport->setOrigValue( value.c_str() );
                else if( strcaseequal(token, "FormulaBefore") )
                    pReport->setOrigFormulaBefore( value.c_str() );
                else if( strcaseequal(token, "FormulaAfter") )
                    pReport->setOrigFormulaAfter( value.c_str() );
                else if( strcaseequal(token, "Aggregate") )
                    pReport->setOrigAggregate( value.c_str() );
                else if( strcaseequal(token, "PosX") )
                    pReport->setOrigPosX( value.c_str() );
                else if( strcaseequal(token, "PosY") )
                    pReport->setOrigPosY( value.c_str() );
                else if( strcaseequal(token, "SizeX") )
                    pReport->setOrigSizeX( value.c_str() );
                else if( strcaseequal(token, "SizeY") )
                    pReport->setOrigSizeY( value.c_str() );
                else if( strcaseequal(token, "Style") )
                    pReport->setOrigStyle( value.c_str() );
                else if( strcaseequal(token, "Url") )
                    pReport->setOrigUrl( value.c_str() );
                else if( strcaseequal(token, "Tag") )
                    pReport->setOrigTag( value.c_str() );
                else if( strcaseequal(token, "MarginLeft") )
                    pReport->setOrigMarginLeft( value.c_str() );
                else if( strcaseequal(token, "MarginRight") )
                    pReport->setOrigMarginRight( value.c_str() );
                else if( strcaseequal(token, "MarginTop") )
                    pReport->setOrigMarginTop( value.c_str() );
                else if( strcaseequal(token, "MarginBottom") )
                    pReport->setOrigMarginBottom( value.c_str() );
                else if( strcaseequal(token, "Visible") )
                    pReport->setOrigVisible( value.c_str() );
                else if( strcaseequal(token, "Supressed") )
                    pReport->setOrigSupressed( value.c_str() );
                else if( strcaseequal(token, "SupressDup") )
                    pReport->setOrigSupressDup( value.c_str() );
                else if( strcaseequal(token, "FontFamily") )
                    pReport->setOrigFontFamily( value.c_str() );
                else if( strcaseequal(token, "FontSize") )
                    pReport->setOrigFontSize( value.c_str() );
                else if( strcaseequal(token, "FontWeight") )
                    pReport->setOrigFontWeight( value.c_str() );
                else if( strcaseequal(token, "FontItalic") )
                    pReport->setOrigFontItalic( value.c_str() );
                else if( strcaseequal(token, "MinFontSize") )
                    pReport->setOrigMinFontSize( value.c_str() );
                else if( strcaseequal(token, "Adjustment") )
                    pReport->setOrigAdjustment( value.c_str() );
                else if( strcaseequal(token, "MaxLines") )
                    pReport->setOrigMaxLines( value.c_str() );
                else if( strcaseequal(token, "Format") )
                    pReport->setOrigFormat( value.c_str() );
                else if( strcaseequal(token, "Mask") )
                    pReport->setOrigMask( value.c_str() );
                else if( strcaseequal(token, "Locale") )
                    pReport->setOrigLocale( value.c_str() );
                else if( strcaseequal(token, "HAlignment") )
                    pReport->setOrigHAlignment( value.c_str() );
                else if( strcaseequal(token, "VAlignment") )
                    pReport->setOrigVAlignment( value.c_str() );
                else if( strcaseequal(token, "FontColor") )
                    pReport->setOrigFontColor( value.c_str() );
                else if( strcaseequal(token, "BackColor") )
                    pReport->setOrigBackColor( value.c_str() );
                else if( strcaseequal(token, "BorderColor") )
                    pReport->setOrigBorderColor( value.c_str() );
                else if( strcaseequal(token, "BorderWidth") )
                    pReport->setOrigBorderWidth( value.c_str() );
                else if( strcaseequal(token, "BorderStyle") )
                    pReport->setOrigBorderStyle( value.c_str() );
                else if( strcaseequal(token, "BorderTopColor") )
                    pReport->setOrigBorderTopColor( value.c_str() );
                else if( strcaseequal(token, "BorderTopWidth") )
                    pReport->setOrigBorderTopWidth( value.c_str() );
                else if( strcaseequal(token, "BorderTopStyle") )
                    pReport->setOrigBorderTopStyle( value.c_str() );
                else if( strcaseequal(token, "BorderBottomColor") )
                    pReport->setOrigBorderBottomColor( value.c_str() );
                else if( strcaseequal(token, "BorderBottomWidth") )
                    pReport->setOrigBorderBottomWidth( value.c_str() );
                else if( strcaseequal(token, "BorderBottomStyle") )
                    pReport->setOrigBorderBottomStyle( value.c_str() );
                else if( strcaseequal(token, "BorderLeftColor") )
                    pReport->setOrigBorderLeftColor( value.c_str() );
                else if( strcaseequal(token, "BorderLeftWidth") )
                    pReport->setOrigBorderLeftWidth( value.c_str() );
                else if( strcaseequal(token, "BorderLeftStyle") )
                    pReport->setOrigBorderLeftStyle( value.c_str() );
                else if( strcaseequal(token, "BorderRightColor") )
                    pReport->setOrigBorderRightColor( value.c_str() );
                else if( strcaseequal(token, "BorderRightWidth") )
                    pReport->setOrigBorderRightWidth( value.c_str() );
                else if( strcaseequal(token, "BorderRightStyle") )
                    pReport->setOrigBorderRightStyle( value.c_str() );
                else if( strcaseequal(token, "BackgroundImage") )
                    pReport->setOrigBackgroundImage( value.c_str() );
                else if( strcaseequal(token, "ImageStyle") )
                    pReport->setOrigImageStyle( value.c_str() );
                /*>>>>>REPORT_READRTK*/
                else if( strcaseequal(token, "Include") ) {
                    if ( getNextToken( text ) != ";" ) {
                        pReport->addError( Error::ParseReportSemicolon, __FUNCTION__, 0, pFileName, mLine, mCol, value.c_str(), "REPORT", pReport->name() );
                        return false;
                    }
                    if( !insertFile( value.c_str(), &wholetext, text, bufferExpanded ) ) {
                        pReport->addWarning( Error::ParseReportIncludeFileNotFound, __FUNCTION__, 0, pFileName, mLine, mCol, value.c_str(),
                                             pReport->includePath() );
                    } else
                        bufferExpanded = true;
                    skipsemicolon = true;
                }
                else
                    pReport->addError( Error::ParseReportProperty, __FUNCTION__, 0, pFileName, mLine, mCol, token.c_str(), "REPORT", pReport->name() );
                if( !skipsemicolon ) {
                    if ( getNextToken( text ) != ";" ) {
                        pReport->addError( Error::ParseReportSemicolon, __FUNCTION__, 0, pFileName, mLine, mCol, value.c_str(), "REPORT", 	pReport->name() );
                        return false;
                    }
                }
                skipsemicolon = false;
            } else { /* Report object */
                if ( strequal( name, "{" ) ) {
                    name = "(unnamed)";
                    pReport->addWarning( Error::ParseReportName, __FUNCTION__, 0, pFileName, mLine, mCol, token.c_str() );
                } else {
                    if ( !strequal( getNextToken( text ), "{" ) ) {
                        pReport->addError( Error::ParseReportOpenBracket, __FUNCTION__, 0, pFileName, mLine, mCol,
                                           token.c_str(), "REPORT", pReport->name() );
                        return false;
                    }
                }

                if ( strcaseequal( token, "reportheader" ) ) {
                    parseSection( Section::ReportHeader, name.c_str(), text );
                } else if ( strcaseequal( token, "reportfooter" ) ) {
                    parseSection( Section::ReportFooter, name.c_str(), text );
                } else if ( strcaseequal( token, "pageheader" ) ) {
                    parseSection( Section::PageHeader, name.c_str(), text );
                } else if ( strcaseequal( token, "pagefooter" ) ) {
                    parseSection( Section::PageFooter, name.c_str(), text );
                } else if ( strcaseequal( token, "groupheader" ) ) {
                    parseSection( Section::GroupHeader, name.c_str(), text );
                } else if ( strcaseequal( token, "groupfooter" ) ) {
                    parseSection( Section::GroupFooter, name.c_str(), text );
                } else if ( strcaseequal( token, "details" ) ) {
                    parseSection( Section::Details, name.c_str(), text );
                } else if ( strcaseequal( token, "input" ) ) {
                    if ( strcaseequal( name, defaultinput )
                            || ( strcaseequal( defaultinput, "default" ) && name.isEmpty() )
                            || ( strcaseequal( name, "default" ) && strempty( defaultinput ) ) ) {
                        pInput = parseInput( name.c_str(), usethisinput, text );
                    } else {
                        pInput = parseInput( name.c_str(), usethisinput, text, skipping );
                    }
                    if ( pInput )
                        pReport->setDefaultInput( usethisinput = pInput );
                } else if ( strcaseequal( token, "style" ) || strcaseequal( token, "styledef" ) ) {
                    parseStyle( name.c_str(), text );
                } else if ( strcaseequal( token, "image" ) ) {
                    parseImage( name.c_str(), text );
                } else if ( strcaseequal( token, "parameter" ) ) {
                    pParameter = parseParameter( name.c_str(), text );
                    if( pParameter )
                        pReport->insertParameter( pParameter );
                } else {
                    pReport->addError( Error::ParseReportElement, __FUNCTION__, 0, pFileName, mLine, mCol, token.c_str(), "REPORT" );
                    return false;
                }
            }
        }
    } else {
        pReport->addError( Error::ParseReportElement, __FUNCTION__, 0, pFileName, mLine, mCol, token.c_str(), "RTK::Report definition" );
        return false;
    }
    if( bufferExpanded )
        delete wholetext;
    return true;
}


/**
 * Parse a section. If the section exists, update its properties. If not, create a new one and inserts it in the report

 * @param type
 * @param sectname
 * @param text
 * @param skipping
 * @return
 */
Section *ReportDefParser::parseSection( Section::SectionType type, const char *sectname,
                                        const char **text, bool skipping )
{
    _GONG_DEBUG_TRACE(5);
    Section *pSection = pReport->findSection( sectname );
    if( !pSection ) {
        pSection = new Section( type, pReport, sectname );
        pReport->insertSection( pSection );
    }
    Xtring token;
    while ( ( token = getNextToken( text ) ) != "}" ) {
        Xtring name = getNextToken( text );
        if ( strequal( name, "=" ) ) {
            // Section Properties
            Xtring value = getNextValue( text );
            if ( value.isEmpty() )
                pReport->addWarning( Error::ParseReportValueMissing, __FUNCTION__, 0, pFileName, mLine, mCol,
                                     token.c_str(), "SECTION", sectname );
            /*<<<<<SECTION_READRTK*/
            if( strcaseequal(token, "Columns") )
                pSection->setOrigColumns( value.c_str() );
            else if( strcaseequal(token, "PageBefore") )
                pSection->setOrigPageBefore( value.c_str() );
            else if( strcaseequal(token, "PageAfter") )
                pSection->setOrigPageAfter( value.c_str() );
            else if( strcaseequal(token, "ResetNPage") )
                pSection->setOrigResetNPage( value.c_str() );
            else if( strcaseequal(token, "SupIfBlank") )
                pSection->setOrigSupIfBlank( value.c_str() );
            else if( strcaseequal(token, "RepeatHeader") )
                pSection->setOrigRepeatHeader( value.c_str() );
            else if( strcaseequal(token, "PrintInFirstPage") )
                pSection->setOrigPrintInFirstPage( value.c_str() );
            else if( strcaseequal(token, "Padding") )
                pSection->setOrigPadding( value.c_str() );
            else if( strcaseequal(token, "PaddingLeft") )
                pSection->setOrigPaddingLeft( value.c_str() );
            else if( strcaseequal(token, "PaddingTop") )
                pSection->setOrigPaddingTop( value.c_str() );
            else if( strcaseequal(token, "Value") )
                pSection->setOrigValue( value.c_str() );
            else if( strcaseequal(token, "FormulaBefore") )
                pSection->setOrigFormulaBefore( value.c_str() );
            else if( strcaseequal(token, "FormulaAfter") )
                pSection->setOrigFormulaAfter( value.c_str() );
            else if( strcaseequal(token, "Aggregate") )
                pSection->setOrigAggregate( value.c_str() );
            else if( strcaseequal(token, "PosX") )
                pSection->setOrigPosX( value.c_str() );
            else if( strcaseequal(token, "PosY") )
                pSection->setOrigPosY( value.c_str() );
            else if( strcaseequal(token, "SizeX") )
                pSection->setOrigSizeX( value.c_str() );
            else if( strcaseequal(token, "SizeY") )
                pSection->setOrigSizeY( value.c_str() );
            else if( strcaseequal(token, "Style") )
                pSection->setOrigStyle( value.c_str() );
            else if( strcaseequal(token, "Url") )
                pSection->setOrigUrl( value.c_str() );
            else if( strcaseequal(token, "Tag") )
                pSection->setOrigTag( value.c_str() );
            else if( strcaseequal(token, "MarginLeft") )
                pSection->setOrigMarginLeft( value.c_str() );
            else if( strcaseequal(token, "MarginRight") )
                pSection->setOrigMarginRight( value.c_str() );
            else if( strcaseequal(token, "MarginTop") )
                pSection->setOrigMarginTop( value.c_str() );
            else if( strcaseequal(token, "MarginBottom") )
                pSection->setOrigMarginBottom( value.c_str() );
            else if( strcaseequal(token, "Visible") )
                pSection->setOrigVisible( value.c_str() );
            else if( strcaseequal(token, "Supressed") )
                pSection->setOrigSupressed( value.c_str() );
            else if( strcaseequal(token, "SupressDup") )
                pSection->setOrigSupressDup( value.c_str() );
            else if( strcaseequal(token, "FontFamily") )
                pSection->setOrigFontFamily( value.c_str() );
            else if( strcaseequal(token, "FontSize") )
                pSection->setOrigFontSize( value.c_str() );
            else if( strcaseequal(token, "FontWeight") )
                pSection->setOrigFontWeight( value.c_str() );
            else if( strcaseequal(token, "FontItalic") )
                pSection->setOrigFontItalic( value.c_str() );
            else if( strcaseequal(token, "MinFontSize") )
                pSection->setOrigMinFontSize( value.c_str() );
            else if( strcaseequal(token, "Adjustment") )
                pSection->setOrigAdjustment( value.c_str() );
            else if( strcaseequal(token, "MaxLines") )
                pSection->setOrigMaxLines( value.c_str() );
            else if( strcaseequal(token, "Format") )
                pSection->setOrigFormat( value.c_str() );
            else if( strcaseequal(token, "Mask") )
                pSection->setOrigMask( value.c_str() );
            else if( strcaseequal(token, "Locale") )
                pSection->setOrigLocale( value.c_str() );
            else if( strcaseequal(token, "HAlignment") )
                pSection->setOrigHAlignment( value.c_str() );
            else if( strcaseequal(token, "VAlignment") )
                pSection->setOrigVAlignment( value.c_str() );
            else if( strcaseequal(token, "FontColor") )
                pSection->setOrigFontColor( value.c_str() );
            else if( strcaseequal(token, "BackColor") )
                pSection->setOrigBackColor( value.c_str() );
            else if( strcaseequal(token, "BorderColor") )
                pSection->setOrigBorderColor( value.c_str() );
            else if( strcaseequal(token, "BorderWidth") )
                pSection->setOrigBorderWidth( value.c_str() );
            else if( strcaseequal(token, "BorderStyle") )
                pSection->setOrigBorderStyle( value.c_str() );
            else if( strcaseequal(token, "BorderTopColor") )
                pSection->setOrigBorderTopColor( value.c_str() );
            else if( strcaseequal(token, "BorderTopWidth") )
                pSection->setOrigBorderTopWidth( value.c_str() );
            else if( strcaseequal(token, "BorderTopStyle") )
                pSection->setOrigBorderTopStyle( value.c_str() );
            else if( strcaseequal(token, "BorderBottomColor") )
                pSection->setOrigBorderBottomColor( value.c_str() );
            else if( strcaseequal(token, "BorderBottomWidth") )
                pSection->setOrigBorderBottomWidth( value.c_str() );
            else if( strcaseequal(token, "BorderBottomStyle") )
                pSection->setOrigBorderBottomStyle( value.c_str() );
            else if( strcaseequal(token, "BorderLeftColor") )
                pSection->setOrigBorderLeftColor( value.c_str() );
            else if( strcaseequal(token, "BorderLeftWidth") )
                pSection->setOrigBorderLeftWidth( value.c_str() );
            else if( strcaseequal(token, "BorderLeftStyle") )
                pSection->setOrigBorderLeftStyle( value.c_str() );
            else if( strcaseequal(token, "BorderRightColor") )
                pSection->setOrigBorderRightColor( value.c_str() );
            else if( strcaseequal(token, "BorderRightWidth") )
                pSection->setOrigBorderRightWidth( value.c_str() );
            else if( strcaseequal(token, "BorderRightStyle") )
                pSection->setOrigBorderRightStyle( value.c_str() );
            else if( strcaseequal(token, "BackgroundImage") )
                pSection->setOrigBackgroundImage( value.c_str() );
            else if( strcaseequal(token, "ImageStyle") )
                pSection->setOrigImageStyle( value.c_str() );
            /*>>>>>SECTION_READRTK*/
            else if ( strcaseequal( token, "level" ) || strcaseequal( token, "grouplevel" ) ) {
                Xtring errlevel;
                pSection->setGroupLevel( stringTo<int>( value.c_str(), errlevel ) );
                if ( !errlevel.isEmpty() ) {
                    pReport->addError( Error::ParseReportValue, __FUNCTION__, 0,
                                       pFileName, mLine, mCol, value.c_str(), token.c_str(),
                                       "SECTION", sectname );
                }
            } else if ( strcaseequal( token, "GroupValue" ) )
                pSection->setOrigValue( value.c_str() );
            else
                pReport->addWarning( Error::ParseReportProperty, __FUNCTION__, 0, pFileName, mLine, mCol, token.c_str(), "SECTION", sectname );
            if( getNextToken( text ) != ";" ) {
                pReport->addError(Error::ParseReportSemicolon, __FUNCTION__, 0, pFileName, mLine, mCol, value.c_str(), "SECTION", sectname );
                return 0;
            }
        } else { /* Section object */
            if ( strequal( name, "{" ) ) {
                name = "(unnamed)";
                pReport->addError( Error::ParseReportName, __FUNCTION__, 0, pFileName, mLine, mCol, "parseSection" );
            } else if( !strequal(getNextToken(text), "{") ) {
                pReport->addError(Error::ParseReportOpenBracket, __FUNCTION__, 0, pFileName, mLine, mCol,
                                  token.c_str(), "SECTION", sectname );
                return 0;
            }
            if ( strcaseequal( token, "object" ) ) {
                parseObject( pSection, name.c_str(), text );
            } else {
                pReport->addError( Error::ParseReportElement, __FUNCTION__, 0, pFileName, mLine, mCol, token.c_str(), "SECTION" );
                return 0;
            }
        }
    }
    return pSection;
}

Object *ReportDefParser::parseObject( Section *psect, const char *objectname, const char **text, bool skipping )
{
    _GONG_DEBUG_TRACE(5);
    Object *pObject  = psect->findObject( objectname );
    if( !pObject ) {
        pObject = new Object( psect, objectname );
        psect->insertObject( pObject );
    }
    Xtring token;
    while ( ( token = getNextToken( text ) ) != "}" ) {
        Xtring name = getNextToken( text );
        if ( strequal( name, "=" ) ) {
            // Section Properties
            Xtring value = getNextValue( text );
            if( value.isEmpty() )
                pReport->addWarning(Error::ParseReportValueMissing, __FUNCTION__, 0, pFileName, mLine, mCol,
                                    token.c_str(), "OBJECT", objectname );
/*<<<<<OBJECT_READRTK*/
            if( strcaseequal(token, "Value") )
                pObject->setOrigValue( value.c_str() );
            else if( strcaseequal(token, "FormulaBefore") )
                pObject->setOrigFormulaBefore( value.c_str() );
            else if( strcaseequal(token, "FormulaAfter") )
                pObject->setOrigFormulaAfter( value.c_str() );
            else if( strcaseequal(token, "Aggregate") )
                pObject->setOrigAggregate( value.c_str() );
            else if( strcaseequal(token, "PosX") )
                pObject->setOrigPosX( value.c_str() );
            else if( strcaseequal(token, "PosY") )
                pObject->setOrigPosY( value.c_str() );
            else if( strcaseequal(token, "SizeX") )
                pObject->setOrigSizeX( value.c_str() );
            else if( strcaseequal(token, "SizeY") )
                pObject->setOrigSizeY( value.c_str() );
            else if( strcaseequal(token, "Style") )
                pObject->setOrigStyle( value.c_str() );
            else if( strcaseequal(token, "Url") )
                pObject->setOrigUrl( value.c_str() );
            else if( strcaseequal(token, "Tag") )
                pObject->setOrigTag( value.c_str() );
            else if( strcaseequal(token, "MarginLeft") )
                pObject->setOrigMarginLeft( value.c_str() );
            else if( strcaseequal(token, "MarginRight") )
                pObject->setOrigMarginRight( value.c_str() );
            else if( strcaseequal(token, "MarginTop") )
                pObject->setOrigMarginTop( value.c_str() );
            else if( strcaseequal(token, "MarginBottom") )
                pObject->setOrigMarginBottom( value.c_str() );
            else if( strcaseequal(token, "Visible") )
                pObject->setOrigVisible( value.c_str() );
            else if( strcaseequal(token, "Supressed") )
                pObject->setOrigSupressed( value.c_str() );
            else if( strcaseequal(token, "SupressDup") )
                pObject->setOrigSupressDup( value.c_str() );
            else if( strcaseequal(token, "FontFamily") )
                pObject->setOrigFontFamily( value.c_str() );
            else if( strcaseequal(token, "FontSize") )
                pObject->setOrigFontSize( value.c_str() );
            else if( strcaseequal(token, "FontWeight") )
                pObject->setOrigFontWeight( value.c_str() );
            else if( strcaseequal(token, "FontItalic") )
                pObject->setOrigFontItalic( value.c_str() );
            else if( strcaseequal(token, "MinFontSize") )
                pObject->setOrigMinFontSize( value.c_str() );
            else if( strcaseequal(token, "Adjustment") )
                pObject->setOrigAdjustment( value.c_str() );
            else if( strcaseequal(token, "MaxLines") )
                pObject->setOrigMaxLines( value.c_str() );
            else if( strcaseequal(token, "Format") )
                pObject->setOrigFormat( value.c_str() );
            else if( strcaseequal(token, "Mask") )
                pObject->setOrigMask( value.c_str() );
            else if( strcaseequal(token, "Locale") )
                pObject->setOrigLocale( value.c_str() );
            else if( strcaseequal(token, "HAlignment") )
                pObject->setOrigHAlignment( value.c_str() );
            else if( strcaseequal(token, "VAlignment") )
                pObject->setOrigVAlignment( value.c_str() );
            else if( strcaseequal(token, "FontColor") )
                pObject->setOrigFontColor( value.c_str() );
            else if( strcaseequal(token, "BackColor") )
                pObject->setOrigBackColor( value.c_str() );
            else if( strcaseequal(token, "BorderColor") )
                pObject->setOrigBorderColor( value.c_str() );
            else if( strcaseequal(token, "BorderWidth") )
                pObject->setOrigBorderWidth( value.c_str() );
            else if( strcaseequal(token, "BorderStyle") )
                pObject->setOrigBorderStyle( value.c_str() );
            else if( strcaseequal(token, "BorderTopColor") )
                pObject->setOrigBorderTopColor( value.c_str() );
            else if( strcaseequal(token, "BorderTopWidth") )
                pObject->setOrigBorderTopWidth( value.c_str() );
            else if( strcaseequal(token, "BorderTopStyle") )
                pObject->setOrigBorderTopStyle( value.c_str() );
            else if( strcaseequal(token, "BorderBottomColor") )
                pObject->setOrigBorderBottomColor( value.c_str() );
            else if( strcaseequal(token, "BorderBottomWidth") )
                pObject->setOrigBorderBottomWidth( value.c_str() );
            else if( strcaseequal(token, "BorderBottomStyle") )
                pObject->setOrigBorderBottomStyle( value.c_str() );
            else if( strcaseequal(token, "BorderLeftColor") )
                pObject->setOrigBorderLeftColor( value.c_str() );
            else if( strcaseequal(token, "BorderLeftWidth") )
                pObject->setOrigBorderLeftWidth( value.c_str() );
            else if( strcaseequal(token, "BorderLeftStyle") )
                pObject->setOrigBorderLeftStyle( value.c_str() );
            else if( strcaseequal(token, "BorderRightColor") )
                pObject->setOrigBorderRightColor( value.c_str() );
            else if( strcaseequal(token, "BorderRightWidth") )
                pObject->setOrigBorderRightWidth( value.c_str() );
            else if( strcaseequal(token, "BorderRightStyle") )
                pObject->setOrigBorderRightStyle( value.c_str() );
            else if( strcaseequal(token, "BackgroundImage") )
                pObject->setOrigBackgroundImage( value.c_str() );
            else if( strcaseequal(token, "ImageStyle") )
                pObject->setOrigImageStyle( value.c_str() );
/*>>>>>OBJECT_READRTK*/
            // A few property sinonyms
            else if ( strcaseequal( token, "Alignment" ) || strcaseequal( token, "Align" ) || strcaseequal( token, "HAlign" ) )
                pObject->setOrigHAlignment( value.c_str() );
            else if ( strcaseequal( token, "VAlign" ) )
                pObject->setOrigVAlignment( value.c_str() );
            else
                pReport->addWarning( Error::ParseReportProperty, __FUNCTION__, 0, pFileName, mLine, mCol, token.c_str(), "OBJECT", objectname );
            if( getNextToken( text ) != ";" ) {
                pReport->addError(Error::ParseReportSemicolon, __FUNCTION__, 0, pFileName, mLine, mCol,
                                  value.c_str(), "OBJECT", objectname );
                return 0;
            }
        } else {
            // Ojbects have not got inner objects
            pReport->addError( Error::ParseReportCloseBracket, __FUNCTION__, 0, pFileName, mLine, mCol, "OBJECT", objectname );
            return 0;
        }
    }
    return pObject;
}

Style *ReportDefParser::parseStyle( const char *stylename, const char **text, bool skipping )
{
    _GONG_DEBUG_TRACE(5);
    Style *pStyle = pReport->findStyle( stylename );
    if( !pStyle ) {
        pStyle = new Style( stylename );
        pReport->insertStyle( pStyle );
    }
    Xtring token;
    while ( ( token = getNextToken( text ) ) != "}" ) {
        Xtring name = getNextToken( text );
        if ( strequal( name, "=" ) ) {
            // Section Properties
            Xtring value = getNextValue( text );
            if( value.isEmpty() )
                pReport->addWarning(Error::ParseReportValueMissing, __FUNCTION__, 0, pFileName, mLine, mCol,
                                    token.c_str(), "STYLE", stylename );
/*<<<<<STYLE_READRTK*/
            if( strcaseequal(token, "Visible") )
                pStyle->setOrigVisible( value.c_str() );
            else if( strcaseequal(token, "Supressed") )
                pStyle->setOrigSupressed( value.c_str() );
            else if( strcaseequal(token, "SupressDup") )
                pStyle->setOrigSupressDup( value.c_str() );
            else if( strcaseequal(token, "FontFamily") )
                pStyle->setOrigFontFamily( value.c_str() );
            else if( strcaseequal(token, "FontSize") )
                pStyle->setOrigFontSize( value.c_str() );
            else if( strcaseequal(token, "FontWeight") )
                pStyle->setOrigFontWeight( value.c_str() );
            else if( strcaseequal(token, "FontItalic") )
                pStyle->setOrigFontItalic( value.c_str() );
            else if( strcaseequal(token, "MaxLines") )
                pStyle->setOrigMaxLines( value.c_str() );
            else if( strcaseequal(token, "MinFontSize") )
                pStyle->setOrigMinFontSize( value.c_str() );
            else if( strcaseequal(token, "Format") )
                pStyle->setOrigFormat( value.c_str() );
            else if( strcaseequal(token, "Mask") )
                pStyle->setOrigMask( value.c_str() );
            else if( strcaseequal(token, "Locale") )
                pStyle->setOrigLocale( value.c_str() );
            else if( strcaseequal(token, "HAlignment") )
                pStyle->setOrigHAlignment( value.c_str() );
            else if( strcaseequal(token, "VAlignment") )
                pStyle->setOrigVAlignment( value.c_str() );
            else if( strcaseequal(token, "Adjustment") )
                pStyle->setOrigAdjustment( value.c_str() );
            else if( strcaseequal(token, "FontColor") )
                pStyle->setOrigFontColor( value.c_str() );
            else if( strcaseequal(token, "BackColor") )
                pStyle->setOrigBackColor( value.c_str() );
            else if( strcaseequal(token, "BorderColor") )
                pStyle->setOrigBorderColor( value.c_str() );
            else if( strcaseequal(token, "BorderWidth") )
                pStyle->setOrigBorderWidth( value.c_str() );
            else if( strcaseequal(token, "BorderStyle") )
                pStyle->setOrigBorderStyle( value.c_str() );
            else if( strcaseequal(token, "BorderTopColor") )
                pStyle->setOrigBorderTopColor( value.c_str() );
            else if( strcaseequal(token, "BorderTopWidth") )
                pStyle->setOrigBorderTopWidth( value.c_str() );
            else if( strcaseequal(token, "BorderTopStyle") )
                pStyle->setOrigBorderTopStyle( value.c_str() );
            else if( strcaseequal(token, "BorderBottomColor") )
                pStyle->setOrigBorderBottomColor( value.c_str() );
            else if( strcaseequal(token, "BorderBottomWidth") )
                pStyle->setOrigBorderBottomWidth( value.c_str() );
            else if( strcaseequal(token, "BorderBottomStyle") )
                pStyle->setOrigBorderBottomStyle( value.c_str() );
            else if( strcaseequal(token, "BorderLeftColor") )
                pStyle->setOrigBorderLeftColor( value.c_str() );
            else if( strcaseequal(token, "BorderLeftWidth") )
                pStyle->setOrigBorderLeftWidth( value.c_str() );
            else if( strcaseequal(token, "BorderLeftStyle") )
                pStyle->setOrigBorderLeftStyle( value.c_str() );
            else if( strcaseequal(token, "BorderRightColor") )
                pStyle->setOrigBorderRightColor( value.c_str() );
            else if( strcaseequal(token, "BorderRightWidth") )
                pStyle->setOrigBorderRightWidth( value.c_str() );
            else if( strcaseequal(token, "BorderRightStyle") )
                pStyle->setOrigBorderRightStyle( value.c_str() );
            else if( strcaseequal(token, "BackgroundImage") )
                pStyle->setOrigBackgroundImage( value.c_str() );
            else if( strcaseequal(token, "ImageStyle") )
                pStyle->setOrigImageStyle( value.c_str() );
            else if( strcaseequal(token, "PageBefore") )
                pStyle->setOrigPageBefore( value.c_str() );
            else if( strcaseequal(token, "PageAfter") )
                pStyle->setOrigPageAfter( value.c_str() );
            else if( strcaseequal(token, "ResetNPage") )
                pStyle->setOrigResetNPage( value.c_str() );
            else if( strcaseequal(token, "SupIfBlank") )
                pStyle->setOrigSupIfBlank( value.c_str() );
            else if( strcaseequal(token, "RepeatHeader") )
                pStyle->setOrigRepeatHeader( value.c_str() );
            else if( strcaseequal(token, "PrintInFirstPage") )
                pStyle->setOrigPrintInFirstPage( value.c_str() );
            else if( strcaseequal(token, "Padding") )
                pStyle->setOrigPadding( value.c_str() );
            else if( strcaseequal(token, "PaddingLeft") )
                pStyle->setOrigPaddingLeft( value.c_str() );
            else if( strcaseequal(token, "PaddingTop") )
                pStyle->setOrigPaddingTop( value.c_str() );
            /*>>>>>STYLE_READRTK*/
            else
                pReport->addWarning( Error::ParseReportProperty, __FUNCTION__, 0, pFileName, mLine, mCol, token.c_str(), "STYLE", stylename );
            if( getNextToken( text ) != ";" ) {
                pReport->addError(Error::ParseReportSemicolon, __FUNCTION__, 0, pFileName, mLine, mCol,
                                  value.c_str(), "STYLE", stylename );
                return 0;
            }
        } else {
            // Ojbects have not got inner objects
            pReport->addError( Error::ParseReportCloseBracket, __FUNCTION__, 0, pFileName, mLine, mCol, "STYLE", stylename );
            delete pStyle;
            return 0;
        }
    }
    return pStyle;
}

Parameter *ReportDefParser::parseParameter( const char *paramname, const char **text, bool skipping )
{
    _GONG_DEBUG_TRACE(5);
    Parameter * pParameter;
    Xtring paramValue;
    Xtring paramCaption;
    Variant::Type paramType = Variant::tString;
    Xtring token;
    while ( ( token = getNextToken( text ) ) != "}" ) {
        Xtring name = getNextToken( text );
        if ( strequal( name, "=" ) ) {
            // Section Properties
            Xtring value = getNextValue( text );
#if 0 // These values are normally empty
            if( value.isEmpty() )
                pReport->addWarning(Error::ParseReportValueMissing, __FUNCTION__, 0, pFileName, mLine, mCol,
                                    token.c_str(), "PARAMETER", paramname );
#endif
            if ( strcaseequal( token, "Value" ) )
                paramValue = value;
            else if ( strcaseequal( token, "Type" ) )
                paramType = Variant::nameToType( value.c_str() );
            else if ( strcaseequal( token, "Caption" ) )
                paramCaption = value;
            else
                pReport->addWarning( Error::ParseReportProperty, __FUNCTION__, 0, pFileName, mLine, mCol, token.c_str(), "PARAMETER", paramname );
            if( getNextToken( text ) != ";" ) {
                pReport->addError(Error::ParseReportSemicolon, __FUNCTION__, 0, pFileName, mLine, mCol,
                                  value.c_str(), "PARAMETER", paramname );
                return 0;
            }

        } else {
            // Ojbects have not got inner objects
            pReport->addError( Error::ParseReportCloseBracket, __FUNCTION__, 0, pFileName, mLine, mCol, "PARAMETER", paramname );
            return 0;
        }
    }
    pParameter = new Parameter( paramname, paramValue.c_str(), paramType, paramCaption.c_str() );
    return pParameter;
}

Image *ReportDefParser::parseImage( const char *imagename, const char **text, bool skipping )
{
    _GONG_DEBUG_TRACE(5);
    Image *pImage = pReport->findImage( imagename );
    if( !pImage ) {
        pImage = new Image( imagename );
        pReport->insertImage( pImage );
    }
    Xtring token;
    while ( ( token = getNextToken( text ) ) != "}" ) {
        Xtring name = getNextToken( text );
        if ( strequal( name, "=" ) ) {
            // Section Properties
            Xtring value = getNextValue( text );
            if( value.isEmpty() )
                pReport->addWarning(Error::ParseReportValueMissing, __FUNCTION__, 0, pFileName, mLine, mCol,
                                    token.c_str(), "IMAGE", imagename );
            /*<<<<<IMAGE_READRTK*/
            if( strcaseequal(token, "Url") )
                pImage->setOrigUrl( value.c_str() );
            else if( strcaseequal(token, "MimeType") )
                pImage->setOrigMimeType( value.c_str() );
            else if( strcaseequal(token, "ImageStyle") )
                pImage->setOrigImageStyle( value.c_str() );
            /*>>>>>IMAGE_READRTK*/
            else
                pReport->addWarning( Error::ParseReportProperty, __FUNCTION__, 0, pFileName, mLine, mCol, token.c_str(), "IMAGE", imagename );
            if( getNextToken( text ) != ";" ) {
                pReport->addError(Error::ParseReportSemicolon, __FUNCTION__, 0, pFileName, mLine, mCol,
                                  value.c_str(), "IMAGE", imagename );
                return 0;
            }
        } else {
            // Images have not got inner objects
            pReport->addError( Error::ParseReportCloseBracket, __FUNCTION__, 0, pFileName, mLine, mCol, "IMAGE", imagename );
            delete pImage;
            return 0;
        }
    }
    return pImage;
}

Input *ReportDefParser::parseInput( const char *inputname, Input *usethisinput, const char **text, bool skipping )
{
    _GONG_DEBUG_TRACE(5);
    Input * pInput = pReport->defaultInput();
    Xtring drivername;
    if( pInput == 0 ) {
        // First property must be drivername
        drivername = getNextToken( text );
        if ( !skipping && !strcaseequal( drivername, "driver" ) ) {
            pReport->addError( Error::ParseReportInputDriverName, __FUNCTION__, 0, pFileName, mLine, mCol, drivername.c_str() );
            return 0;
        }
        drivername = getNextToken( text );
        if ( !strequal( drivername, "=" ) ) {
            pReport->addError( Error::ParseReportColon, __FUNCTION__, 0, pFileName, mLine, mCol, drivername.c_str() );
            return 0;
        }
        drivername = getNextValue( text );
        if( getNextToken( text ) != ";" ) {
            pReport->addError(Error::ParseReportSemicolon, __FUNCTION__, 0, pFileName, mLine, mCol,
                              drivername.c_str(), "INPUT", inputname );
            return 0;
        }
    } else {
        drivername = pInput->driver();
        usethisinput = pInput;
    }
    // Rest of properties
    Xtring token;
    while ( ( token = getNextToken( text ) ) != "}" ) {
        Xtring name = getNextToken( text );
        if ( strequal( name, "=" ) ) {
            // Section Properties
            Xtring value = getNextValue( text );
            if( value.isEmpty() )
                pReport->addWarning(Error::ParseReportValueMissing, __FUNCTION__, 0, pFileName, mLine, mCol,
                                    token.c_str(), "INPUT", inputname );
            if ( !skipping ) {
                if ( strcaseequal( drivername, "mysql" ) ) {
                    if ( !usethisinput && !pInput ) {
#ifdef HAVE_MYSQL
                        pInput = usethisinput = new InputMySql( *pReport, inputname );
#else

                        pReport->addError( Error::FeatureNotCompiled, __FUNCTION__, 0, "MySQL" );
                        pInput = usethisinput = new NullInput( *pReport, inputname );
#endif

                    }
                } else if ( strcaseequal( drivername, "pgsql" ) || strcaseequal( drivername, "postgres" )
                            || strcaseequal( drivername, "postgresql" ) ) {
                    if ( !usethisinput && !pInput ) {
#ifdef HAVE_PGSQL
                        pInput = usethisinput = new InputPgSql( *pReport, inputname );
#else

                        pReport->addError( Error::FeatureNotCompiled, __FUNCTION__, 0, "PostGreSQL" );
                        pInput = usethisinput = new NullInput( *pReport, inputname );
#endif
                    }
                } else if ( strcaseequal( drivername, "xbase" ) || strcaseequal( drivername, "xbasesql" ) ) {
                    if ( !usethisinput && !pInput ) {
#ifdef RTK_HAVE_XBASE
                        pInput = usethisinput = new InputXBase( *pReport, inputname );
#else

                        pReport->addError( Error::FeatureNotCompiled, __FUNCTION__, 0, "XBaseSQL" );
                        pInput = usethisinput = new NullInput( *pReport, inputname );
#endif
                    }
                } else if ( strcaseequal( drivername, "csv" ) ) {
                    if ( !usethisinput && !pInput )
                        pInput = usethisinput = new InputCsv( *pReport, inputname );
                } else {
                    /// \todo {?} Look up the registered inputs table for this driver
#if 0
                    if ( !pInput )
                        pInput = Report::createRegisteredInput( inputname, drivername.c_str() );
#endif

                    if ( !usethisinput ) {
                        pReport->addError( Error::ParseReportInputDriverNotRegistered,
                                           __FUNCTION__, 0, pFileName, mLine, mCol, drivername.c_str() );
                        skipping = true; // Skip rest of input
                    }
                }
                // If there is an input, parse this attribute in its specialized class
                if ( usethisinput ) {
                    if( strcaseequal(token, "Encoding") )
                        usethisinput->propEncoding.setOrig( value.c_str() );
                    else if ( !usethisinput->parseAttribute( token.c_str(), value.c_str() ) )
                        pReport->addWarning( Error::ParseReportProperty, __FUNCTION__, 0, pFileName, mLine, mCol, token.c_str(), "INPUT", inputname );
                }
            } // skipping
            if( getNextToken( text ) != ";" ) {
                pReport->addError(Error::ParseReportSemicolon, __FUNCTION__, 0, pFileName, mLine, mCol,
                                  value.c_str(), "INPUT", inputname );
                return 0;
            }
        } else { /* Input fields */
            if ( strequal( name, "{" ) ) {
                name = "(unnamed)";
                pReport->addError( Error::ParseReportName, __FUNCTION__, 0, pFileName, mLine, mCol, "parseInput" );
            } else if( !strequal(getNextToken(text), "{") ) {
                pReport->addError(Error::ParseReportOpenBracket, __FUNCTION__, 0, pFileName, mLine, mCol,
                                  token.c_str(), "INPUT", inputname );
                if ( pInput )
                    delete pInput;
                return 0;
            }
            if ( strcaseequal( token, "inputfield" ) ) {
                parseInputField( usethisinput, name.c_str(), text, skipping );
            } else {
                pReport->addError( Error::ParseReportElement, __FUNCTION__, 0, pFileName, mLine, mCol, token.c_str(), "INPUT" );
                if ( pInput )
                    delete pInput;
                return 0;
            }
        }
    }
    return usethisinput;
}

InputField *ReportDefParser::parseInputField( Input *pInput, const char *ifname, const char **text, bool skipping )
{
    InputField *pInputField = 0;
    if( pInput ) {
        pInputField = pInput->getInputField(pInput->findInputField( ifname ) );
        if( !pInputField ) {
            pInputField = new InputField( ifname );
            if( !skipping )
                pInput->insertInputField( pInputField );
        }
    }
    Xtring token;
    while ( ( token = getNextToken( text ) ) != "}" ) {
        Xtring name = getNextToken( text );
        if ( strequal( name, "=" ) ) {
            // Section Properties
            Xtring value = getNextValue( text );
            if( value.isEmpty() )
                pReport->addWarning(Error::ParseReportValueMissing, __FUNCTION__, 0, pFileName, mLine, mCol,
                                    token.c_str(), "INPUTFIELD", ifname );

            if ( !skipping && pInputField ) {
                /*<<<<<INPUTFIELD_READRTK*/
                if( strcaseequal(token, "Source") )
                    pInputField->setOrigSource( value.c_str() );
                else if( strcaseequal(token, "ConstValue") )
                    pInputField->setOrigConstValue( value.c_str() );
                /*>>>>>INPUTFIELD_READRTK*/
                else if( strcaseequal(token, "Type") )
                    pInputField->setOrigType( value.c_str() );
                else
                    pReport->addWarning( Error::ParseReportProperty, __FUNCTION__, 0, pFileName, mLine, mCol,
                                         token.c_str(), "INPUTFIELD", ifname );
            }
            if( getNextToken( text ) != ";" ) {
                pReport->addError(Error::ParseReportSemicolon, __FUNCTION__, 0, pFileName, mLine, mCol,
                                  value.c_str(), "INPUTFIELD", ifname );
                delete pInputField;
                return 0;
            }
        } else {
            // Inputfields have not got inner objects
            pReport->addError( Error::ParseReportCloseBracket, __FUNCTION__, 0, pFileName, mLine, mCol, "INPUTFIELD", ifname );
            delete pInputField;
            return 0;
        }
    }
    return pInputField;
}



/**
    @relates Report

    @brief Reads a Report definition from a file in rtk format.
    @param rtkfilename The name of the report definition file (.rtk)
    @param defaultinput The name of the input to use

    Reads the report definition file named \a rtkfilename into this report.
    The file is looked up on the reports path
    If \a defaultinput is null (default), reads the input named 'default'
    in the definition file, otherwise, reads the input definition named
    \a defaultinput

    Returns true if the xml file contains a correct RTK definition.
    Note that it returns true even if the input couldn't be found.

    \todo {autodelete} delete the previous report definition
 */
bool Report::readRtkFile( const char *filename,
                          const char *defaultinput, Input *usethisinput, const Xtring &initdefines )
{
    std::ifstream f;
    f.open( filename, std::ios_base::binary | std::ios_base::in );
    if ( !f.is_open() || !f.good() ) {
        Xtring rtkfilename = FileUtils::findInPath( includePath(), filename, "informes/" );
        f.open( rtkfilename.c_str(), std::ios_base::binary | std::ios_base::in );
        _GONG_DEBUG_PRINT( 0, rtkfilename );
    }
    if ( !f.is_open() || !f.good() ) {
        addError( Error::OpenFile, __FUNCTION__, 0, filename, strerror(errno) );
        return false;
    }
    f.seekg( 0, std::ios_base::end );
    long end_pos = f.tellg();
    f.seekg( 0, std::ios_base::beg );
    char *buffer = new char[ end_pos + 1 ];
    f.read( buffer, end_pos );
    buffer[ end_pos ] = '\0';
    bool ret = readRtkString( buffer, defaultinput, filename, usethisinput, initdefines );
    delete buffer;
    return ret;
}

/**
	* Parses a string containing a text report definition into this Report object
	* @param rtkstring The string containing the text report definition
	* @param defaultinput The input to use in case of multiple input definitions
	* @param filename The filename this string came from, just to show it on errors
	* @param usethisinput If not null, do not read the input, use this instead
	* @return true if parsed correctly, false if wrong
	*/
bool Report::readRtkString( const char *rtkstring, const char *defaultinput,
                            const char *filename, Input *usethisinput, const Xtring &initdefines )
{
    _GONG_DEBUG_TRACE(3);
    setDefaultInput( 0 );
    if ( strempty( defaultinput ) )
        defaultinput = "default";
    ReportDefParser parser( this );
    return parser.parseReport( rtkstring, defaultinput, filename, usethisinput, initdefines );
}

} // namespace RTK


#if 0
// Move this to the Inputxxxx.cpp module
/*<<<<<INPUTCSV_READRTK*/
if( strcaseequal(token, "Filename") )
    pInputCsv->setOrigFilename( value.c_str() );
else if( strcaseequal(token, "Delimiters") )
    pInputCsv->setOrigDelimiters( value.c_str() );
else if( strcaseequal(token, "Quote") )
    pInputCsv->setOrigQuote( value.c_str() );
else if( strcaseequal(token, "HeaderLines") )
    pInputCsv->setOrigHeaderLines( value.c_str() );
/*>>>>>INPUTCSV_READRTK*/
/*<<<<<INPUTMYSQL_READRTK*/
if( strcaseequal(token, "Host") )
    pInputMySql->setOrigHost( value.c_str() );
else if( strcaseequal(token, "Port") )
    pInputMySql->setOrigPort( value.c_str() );
else if( strcaseequal(token, "User") )
    pInputMySql->setOrigUser( value.c_str() );
else if( strcaseequal(token, "Password") )
    pInputMySql->setOrigPassword( value.c_str() );
else if( strcaseequal(token, "Database") )
    pInputMySql->setOrigDatabase( value.c_str() );
else if( strcaseequal(token, "From") )
    pInputMySql->setOrigFrom( value.c_str() );
else if( strcaseequal(token, "Where") )
    pInputMySql->setOrigWhere( value.c_str() );
else if( strcaseequal(token, "OrderBy") )
    pInputMySql->setOrigOrderBy( value.c_str() );
else if( strcaseequal(token, "GroupBy") )
    pInputMySql->setOrigGroupBy( value.c_str() );
else if( strcaseequal(token, "PreSQL") )
    pInputMySql->setOrigPreSQL( value.c_str() );
else if( strcaseequal(token, "PostSQL") )
    pInputMySql->setOrigPostSQL( value.c_str() );
/*>>>>>INPUTMYSQL_READRTK*/
/*<<<<<INPUTPGSQL_READRTK*/
if( strcaseequal(token, "Host") )
    pInputPgSql->setOrigHost( value.c_str() );
else if( strcaseequal(token, "Port") )
    pInputPgSql->setOrigPort( value.c_str() );
else if( strcaseequal(token, "User") )
    pInputPgSql->setOrigUser( value.c_str() );
else if( strcaseequal(token, "Password") )
    pInputPgSql->setOrigPassword( value.c_str() );
else if( strcaseequal(token, "Database") )
    pInputPgSql->setOrigDatabase( value.c_str() );
else if( strcaseequal(token, "From") )
    pInputPgSql->setOrigFrom( value.c_str() );
else if( strcaseequal(token, "Where") )
    pInputPgSql->setOrigWhere( value.c_str() );
else if( strcaseequal(token, "OrderBy") )
    pInputPgSql->setOrigOrderBy( value.c_str() );
else if( strcaseequal(token, "GroupBy") )
    pInputPgSql->setOrigGroupBy( value.c_str() );
else if( strcaseequal(token, "PreSQL") )
    pInputPgSql->setOrigPreSQL( value.c_str() );
else if( strcaseequal(token, "PostSQL") )
    pInputPgSql->setOrigPostSQL( value.c_str() );
/*>>>>>INPUTPGSQL_READRTK*/
/*<<<<<PARAMETER_READRTK*/
if( strcaseequal(token, "Value") )
    pParameter->setOrigValue( value.c_str() );
/*>>>>>PARAMETER_READRTK*/
/*<<<<<INPUTXBASE_READRTK*/
if( strcaseequal(token, "DatabasePath") )
    pInputXBase->setOrigDatabasePath( value.c_str() );
else if( strcaseequal(token, "CaseSensitive") )
    pInputXBase->setOrigCaseSensitive( value.c_str() );
else if( strcaseequal(token, "From") )
    pInputXBase->setOrigFrom( value.c_str() );
else if( strcaseequal(token, "Where") )
    pInputXBase->setOrigWhere( value.c_str() );
else if( strcaseequal(token, "OrderBy") )
    pInputXBase->setOrigOrderBy( value.c_str() );
else if( strcaseequal(token, "GroupBy") )
    pInputXBase->setOrigGroupBy( value.c_str() );
else if( strcaseequal(token, "PreSQL") )
    pInputXBase->setOrigPreSQL( value.c_str() );
else if( strcaseequal(token, "PostSQL") )
    pInputXBase->setOrigPostSQL( value.c_str() );
/*>>>>>INPUTXBASE_READRTK*/
#endif
