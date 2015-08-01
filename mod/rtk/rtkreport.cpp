#include <exception>
#include <cctype>      // toupper, isdigit
#include <cmath>
#include <clocale>
#include <cstdarg>      // addError

#include <gongdebug.h>
#include <gongformatter.h>
#include "rtkinput.h"
#include "rtkreport.h"
#include "rtkimage.h"
#include "rtkinputcached.h"
#include "rtkoutputcached.h"
#include "rtkformula.h"

namespace gong {}
namespace RTK = gong;

namespace gong {

/**
  @class Report
  @brief RTK main report object

  A report definition contains several Section objects which
  in turn contain several Object objects.

  The fact that the Report itself is an Object, implies that it inherits all the
  properties of the Object class, and are considered as global properties. For instance,
  the BackColor property of a Report is considered the global BackColor of the report
  which can be overrided by the BackColor property of a Section or an Object.

  To create a report definition, call the Report constructor with the appropiate arguments,
  taking into account that all of them have default values. Then, read the definition
  from an xml file with readXml. The definition of the report has its Input implicit,
  but you can override it with setInput. Then, you print the report to a given Output.
  Here is an example of how to do this:

  \code
    Report aReport;
    if ( aReport.readXmlFile ( filename ) ) {
        OutputODFCalc1 o( ArgOutputFileName );
        r.print ( o );
    }
  \endcode


  You can create a Report definition on the fly by creating Sections
  and Objects. Once you have created a Section, insert it in the Report
  with \a insertSection.

*/

Report::Report(RegConfig *prc)
    : Object(0, "Report"), propUnits(chars),
      mGroupLevels( uint( -1 ) ), mStarted( false ), mFinished(false),
      pDefaultInput( 0 ), pRegConfig (prc),
      mDeleteRegConfig( false ), mPasses(PASS_NONE)
#ifdef HAVE_BOOST_SPIRIT
	  , pFormulaContext( 0 )
#endif

{
    if( !pRegConfig ) {
        pRegConfig = new RegConfig( "" );
        mDeleteRegConfig = true;
    }
    pFormatter = new Formatter(*pRegConfig);
    pFirstDetailsSection = 0;
#ifdef HAVE_BOOST_SPIRIT
    pFormulaContext = new dscript::context( this );
    dscript::link_stdlib( *pFormulaContext );
    pFormulaContext->enable_logging( &std::cout );
#endif
}

/** Borra el objeto Input y la definición del informe (secciones y estilos) */
Report::~Report()
{
    if ( pDefaultInput )
        delete pDefaultInput;
    uint i;
    for ( i = 0; i < mSections.size(); i++ )
        delete mSections[ i ];
    for ( i = 0; i < mStyles.size(); i++ )
        delete mStyles[ i ];
    for ( i = 0; i < mImages.size(); i++ )
        delete mImages[ i ];
    for ( i = 0; i < mErrors.size(); i++ )
        delete mErrors[ i ];
    for ( i = 0; i < mParameters.size(); i++ )
        delete mParameters[ i ];
    delete pFormatter;
    if( mDeleteRegConfig )
        delete pRegConfig;
}

/**
  @param input Pointer to the input to set

  Just one input at a time can be used by a Report
  */
void Report::setDefaultInput( Input *input )
{
    if ( pDefaultInput != input )
        delete pDefaultInput;
    pDefaultInput = input;
}

/**
  @param section Pointer to the section to insert. The report takes ownership of the section
  @return The number of sections in the report
  */
int Report::insertSection( Section *section )
{
    mSections.push_back( section );
    return mSections.size();
}

/**
  @param image Pointer to the image to insert. The report takes ownership of the image
  @return The number of images in the report
  */
int Report::insertImage( Image *image )
{
    mImages.push_back( image );
    return mImages.size();
}

/**
    Returns a Image object by its name
    Call the getImage method to get the underlying QPixmap object.
 */
Image *Report::findImage( const Xtring &name ) const
{
    std::vector<Image *>::const_iterator it;
    for( it=mImages.begin(); it != mImages.end(); ++it ) {
        if ( (*it)->name() == name ) {
            return *it;
        }
    }
    return 0;
}

bool Report::eraseImage( const Xtring &name )
{
    std::vector<Image *>::iterator it;
    for( it=mImages.begin(); it != mImages.end(); ++it ) {
        if ( (*it)->name() == name ) {
            delete (*it);
            mImages.erase(it);
            return true;
        }
    }
    return false;
}


void Report::fixImages()
{
    for ( uint i = 0; i < mImages.size(); i++ ) {
        mImages[i]->fixParameters(mParameters, paramDelim());
    }
}


/**
    \relates Report

    @brief Gets a section by its name
    \param name The name of the section to find
    \return A pointer to the section or 0 if not found
  */
Section *Report::findSection( const Xtring &name ) const {
    for ( uint i = 0; i < mSections.size(); i++ ) {
        if ( mSections[ i ] ->name() == name )
            return mSections[ i ];
    }
    return 0;
}

/**
  @param nsection The index of the section to find
  @return A pointer to the section or 0 if not found
  */
Section *Report::getSection( uint nsection ) const {
    if ( nsection < mSections.size() )
        return mSections[ nsection ];
    else
        return 0;
}


/**
  @param nstyle The position of the style to get
  @return A pointer to the style or 0 if not found
 */
Style *Report::getStyle( uint nstyle ) const {
    if ( nstyle < mStyles.size() )
        return mStyles[ nstyle ];
    else
        return 0;
}

/**
  @param name The name of the style to find
  @return A pointer to the style or 0 if not found
  */
Style *Report::findStyle( const Xtring &name ) const
{
    for ( uint i = 0; i < mStyles.size(); i++ ) {
        if ( mStyles[ i ] ->name() == name )
            return mStyles[ i ];
    }
    return 0;
}

/**
  @param style Pointer to the style to insert. The report takes ownership of the style
  @return The number of styles in the report
 */
int Report::insertStyle( Style *style )
{
    eraseStyle(style->name());
    mStyles.push_back( style );
    return mStyles.size();
}

bool Report::eraseStyle( const Xtring &name )
{
    std::vector<Style *>::iterator it;
    for( it=mStyles.begin(); it != mStyles.end(); ++it ) {
        if ( (*it)->name() == name ) {
            mStyles.erase(it);
            delete *it;
            return true;
        }
    }
    return false;
}

void Report::fixStyles()
{
    for ( uint i = 0; i < mStyles.size(); i++ ) {
        mStyles[i]->fixParameters(mParameters, paramDelim());
    }
}


/**
    This function goes through every object in the report and marks if its value is constant.
    It is executed only once at the start of the printing

    Here, we set mRealValue for every constant object. This is different to setting it via setRealValue,
    which moves mNextRealValue to mRealValue and sets mNextRealValue. We don't need this here as the values
    are constant.
 */
void Report::evalConstValues(const Input *in)
{
    _GONG_DEBUG_TRACE(4);
    bool isconst;
    int ninputfield;
    setRealValue( evalConstValue( value(), in, &isconst, &ninputfield ) );
    setConst( isconst );
    setInputSourcePos( ninputfield );
    for ( uint ns = 0; ns < mSections.size(); ns++ ) {
        Section *sect = mSections[ns];
        sect->mRealValue = evalConstValue(sect->value(), in, &isconst, &ninputfield );
        sect->setConst( isconst );
        sect->setInputSourcePos( ninputfield );
        /*  OJO if( !isconst && ninputfield == -1 && mPasses != PASS_ALL )
                    mPasses = evalPasses(sect);*/
#ifdef _GONG_DEBUG
        if( isconst || ninputfield != -1 )
            _GONG_DEBUG_PRINT(6, Xtring::printf("%s.value:%s", sect->name(), sect->realValue().toString().c_str() ) );
#endif
        for ( uint no = 0; no < sect->objectsCount(); no++ ) {
            Object *object = sect->getObject( no );
            object->mRealValue = evalConstValue( object->value(), in, &isconst, &ninputfield);
            object->setConst( isconst );
            object->setInputSourcePos( ninputfield );
#if 0 /// \todo {?} Calc mPasses for every object?
            if( !isconst && ninputfield == -1 )
                mPasses = evalPasses(sect);
            else
                mPasses = PASS_NONE;
#endif
#ifdef _GONG_DEBUG
            if( isconst || ninputfield != -1 )
                _GONG_DEBUG_PRINT(6, Xtring::printf("%s.value:%s", object->name(), object->realValue().toString().c_str() ) );
#endif
        }
    }
}


Report::Passes Report::evalPasses(const Object *obj) const
{
    Xtring code = obj->value();
    Passes ret = PASS_NONE;
    if( code[0] == '=' )
        ret = evalPassesFormula(obj, code);
    if( ret != PASS_ALL )
        ret = evalPassesFormula(obj, obj->formulaBefore());
    if( ret != PASS_ALL )
        ret = evalPassesFormula(obj, obj->formulaAfter());
    return ret;
}

Report::Passes Report::evalPassesFormula(const Object *obj, const Xtring &code) const
{
    Passes ret = PASS_NONE;
    FormulaToken ft;
    Xtring name;
    int pos = 0;
    Variant value;
    while( ((ft = getNextToken(code, &value, &pos)) != FT_ERROR) && (ft!=FT_END) ) {
        switch( ft ) {
        case FT_NUMBER:
        case FT_STRING:
            break; // May be const if it is alone
        case FT_NAME:
            name = value.toString();
            if( (ft = getNextToken(code, &value, &pos)) == FT_DOT ) {
                if( name.lower() == "report" ) {
                    ft = getNextToken(code, &value, &pos); // Is a section name?
                    Section *refsect = report()->findSection(value.toString());
                    if( refsect ) {
                        ft = getNextToken(code, &value, &pos); // Is a dot?
                        if( ft == FT_DOT ) {
                            ft = getNextToken(code, &value, &pos); // Is an object name?
                            Object *refobj = refsect->findObject(value.toString());
                            if( refobj ) {
                                ft = getNextToken(code, &value, &pos); // Is a dot?
                                if( ft == FT_DOT ) {
                                    ft = getNextToken(code, &value, &pos); // Is Value or ValueInt?
                                    name = value.toString();
                                    if( name.lower() == "value" || name.lower() == "valueint" ) {
                                        if( refobj->aggregate() != AggNone ) {
                                            if( refsect->groupLevel()
                                                    < static_cast<Section*>(obj->parent())->groupLevel() )
                                                ret = Passes(ret | PASS_GROUP_AHEAD);
                                        }
                                        if( ret == PASS_ALL )
                                            return ret;
                                    }
                                }
                            }
                        }
                    }
                } else while ( (ft = getNextToken(code, &value, &pos)) == FT_DOT || ft == FT_NAME ) {
                        if( ft == FT_ERROR || ft == FT_END )
                            return ret;
                    }
            }
        default:
            ; // Ignore any other thing
        }
    }
    return ret;
}



/**
    This function is called for every object when the printing starts to see whether the object is
    constant or not.
	As a special case, a formula of the form: = input.XXXXXXX will be considered const
 */
Variant Report::evalConstValue( const Xtring &code, const Input *in, bool *isconst, int *ninputfield )
{
    *ninputfield = -1;
    int pos = 0;
    if( code[0] != '=' ) {
        *isconst = true;
        return code;
    }
    Variant value;
    FormulaToken ft;
    Xtring name;
    *isconst = false;
    pos = 1;
    switch( ft = getNextToken(code, &value, &pos) ) {
    case FT_ERROR:
        return value;
    case FT_END:    // Empty formula
        *isconst = true;
        return value;
    case FT_NUMBER:
    case FT_STRING:
        *isconst = true;
        break; // May be const if it is alone
    case FT_NAME:
        name = value.toString();
        if( (ft = getNextToken(code, &value, &pos)) == FT_DOT ) {
            if( name.lower() == "input" || name.lower() == "@input" ) {
                if( getNextToken(code, &value, &pos) == FT_NAME ) {
                    *ninputfield = in->findInputField(value.toString());
                    if( *ninputfield == -1 )
                        addWarning(Error::FormulaInputField, __FUNCTION__, 0, value.toString().c_str(), code.c_str());
                } /// \todo {check} input.getName()
            }
        }
    default:
        ;
        // Any other thing is not constant
    }
    if( *isconst || (*ninputfield != -1)) {
        // So far, if it is the only expression, it is constant
        while( (ft = getNextToken(code, &value, &pos)) != FT_END ) {
            if( ft != FT_SEMICOLON && ft != FT_BRACKET ) {
                *isconst = false;
                *ninputfield = -1;
                value = Variant();
                break;
            }
        }
    }
    /// \todo {check} see if the report uses summaries in headers
    if( *ninputfield != -1 ) {
        *isconst = false;
        return Variant();
    } else if( *isconst ) {
        *ninputfield = -1;
        return value;
    } else
        return Variant();
}

void Report::evalNonConstValues(const Input *in)
{
    _GONG_DEBUG_TRACE( 4 );
    if( !isConst() )
        setRealValue( evalNonConstValue( this, in, inputFieldNumber() ) );
    for ( uint ns = 0; ns < mSections.size(); ns++ ) {
        Section *sect = mSections[ns];
        for ( uint no = 0; no < sect->objectsCount(); no++ ) {
            Object *object = sect->getObject( no );
            if( ! object->isConst() ) {
                object->setRealValue( evalNonConstValue( object, in, object->inputFieldNumber() ) );
#ifdef _GONG_DEBUG
                _GONG_DEBUG_PRINT(6, Xtring::printf("%s.value:%s", object->name(), object->realValue().toString().c_str() ) );
#endif
            }
        }
        if( !sect->isConst() ) {
            sect->setRealValue( evalNonConstValue( sect, in, sect->inputFieldNumber() ) );
#ifdef _GONG_DEBUG
            _GONG_DEBUG_PRINT(5, Xtring::printf("%s.value:%s (prev=%s)",
                                                sect->name(), sect->realValue().toString().c_str(),
                                                sect->mPrevRealValue.toString().c_str() ) );
#endif
        }
    }
}

/**
 * Evals a non constant object, which can be an aggregate, a direct input field or any other formula.
 * If the object is an aggregate, evaluates it anyway, as we need it for the aggregation
 * @param obj
 * @param in
 * @param ninputfield
 * @return
 */
Variant Report::evalNonConstValue(Object *obj, const Input *in, int ninputfield)
{
    Variant v;
    if( ninputfield != -1 )
        v = in->getInputField(ninputfield)->value();
    else
        v = obj->execFormula(obj->value());
    _GONG_DEBUG_PRINT(5, Xtring::printf("NonConstValue for object %s=%s",
                                        obj->name(), v.toString().c_str() ) );
    return v;
}

Measure Report::calcSizeYSections( Section::SectionType type ) const
{
    Measure h = 0.0;
    for ( uint i = 0; i < mSections.size(); i++ ) {
        if ( mSections[ i ] ->type() == type )
            h += mSections[ i ] ->sizeY();
    }
    return h;
}

void Report::fixAllProperties(const Output *out)
{
    // Set default properties for the report, so that every object has a
    // reasonable default value

    propVisible.set( true );
    propSupressed.set( false );
    propStyle.set( "report" );
    propFontFamily.set( out->defaultFontFamily() );
    propFontSize.set( out->defaultFontSize() );
    propFontWeight.set( out->defaultFontWeight() );
    propFontItalic.set( out->defaultFontItalic() );
    propFormat.set(0);
    propHAlignment.set(AlignAuto);
    propVAlignment.set(AlignAuto);
    propMaxLines.set(0);
    propMinFontSize.set( out->defaultMinFontSize() );
    propAdjustment.set( out->defaultAdjustment() );
    propFontColor.set( out->defaultFontColor() );
    propBackColor.set( out->defaultBackColor() );
    propBorderStyle.set( BorderNone );
    propBorderColor.set( out->defaultBorderColor() );
    propBorderWidth.set( out->defaultBorderWidth() );
    propBorderTopStyle.set( BorderNone );
    propBorderTopColor.set( out->defaultBorderTopColor() );
    propBorderTopWidth.set( out->defaultBorderTopWidth() );
    propBorderBottomStyle.set( BorderNone );
    propBorderBottomColor.set( out->defaultBorderBottomColor() );
    propBorderBottomWidth.set( out->defaultBorderBottomWidth() );
    propBorderLeftStyle.set( BorderNone );
    propBorderLeftColor.set( out->defaultBorderLeftColor() );
    propBorderLeftWidth.set( out->defaultBorderLeftWidth() );
    propBorderRightStyle.set( BorderNone );
    propBorderRightColor.set( out->defaultBorderRightColor() );
    propBorderRightWidth.set( out->defaultBorderRightWidth() );
    propBackgroundImage.set(0);

    // Now set the original properties
    fixParameters(mParameters, paramDelim());
    // Now fix the properties for the resf of objects
    for ( uint ns = 0; ns < mSections.size(); ns++ ) {
        Section *sect = mSections[ns];
        sect->fixParameters(mParameters, paramDelim());
    }

    // Calc the sizes of the report in the reports units,
    // because we need them to fix the measures of the rest of the objects
    Measure repsizex = stringTo<Measure>(propSizeX.getOrig());
    Measure repsizey = stringTo<Measure>(propSizeY.getOrig());
    if( repsizey == 0 ) // The sizey is calculated accordingly to the output's sizey and fontsize
        repsizey = ( out->sizeY() / ((fontSize()!=0.0?fontSize():out->defaultFontSize())*1.7) );

    // Sets the sizes of the report in output units
    fixMeasures( out->sizeX() - out->marginLeft() - out->marginRight(),
                 out->sizeY() - out->marginTop() - out->marginBottom(),
                 units(), repsizex, repsizey);
    // Transform all the report's measures to output's measures
    for ( uint ns = 0; ns < mSections.size(); ns++ ) {
        Section *sect = mSections[ns];
        sect->fixMeasures(
            out->sizeX() - out->marginLeft() - out->marginRight(),
            out->sizeY() - out->marginTop() - out->marginBottom() ,
            units(), repsizex, repsizey );
        uint nobj;
        Object *prevobj = 0, *thisobj;
        for( nobj=0; nobj<sect->objectsCount(); nobj++ ) {
            thisobj = sect->mObjects[nobj];
            thisobj->fixMeasures(out->sizeX() - out->marginLeft() - out->marginRight(),
                                 out->sizeY() - out->marginTop() - out->marginBottom(),
                                 units(), repsizex, repsizey, prevobj);
            prevobj=thisobj;
        }
    }
    // Finally, get the total height of the page footers
    mFootersHeight = calcSizeYSections( Section::PageFooter );
    // Set grouplevel = 0 for non-group sections
    for ( uint ns = 0; ns < mSections.size(); ns++ ) {
        Section *sect = mSections[ns];
        if( sect->groupLevel() > 0 )
            if( sect->type() == Section::PageHeader ||
                    sect->type() == Section::PageFooter ||
                    sect->type() == Section::ReportFooter ||
                    sect->type() == Section::ReportHeader ||
                    sect->type() == Section::Details )
                sect->setGroupLevel( 0 );
    }
}


void Report::fixParameters(const ParametersList &parameters, const char *delim)
{
    Object::fixParameters(parameters, delim);
    /*<<<<<REPORT_FIXPARAMETERS*/
	propTitle.fix( parameters, delim );
	propUnits.fix( parameters, delim, chars );
	propPageOrientation.fix( parameters, delim, Portrait );
	propStylesPath.fix( parameters, delim );
	propImagesPath.fix( parameters, delim );
	propIncludePath.fix( parameters, delim );
	propParamDelim.fix( parameters, delim, "###" );
	propOrder.fix( parameters, delim );
	propFilter.fix( parameters, delim );
	propCopies.fix( parameters, delim, 1 );
	propPagesPerFolio.fix( parameters, delim, 1 );
/*>>>>>REPORT_FIXPARAMETERS*/
}

void Report::startScripting()
{
}

void Report::stopScripting()
{
}

int Report::printReportHeaders( Output *out )
{
    int ret = 0;
    for ( uint i = 0; i < mSections.size(); i++ ) {
        if ( mSections[i]->isReportHeader() )
            ret = printSection( out, mSections[i] );
    }
    return ret;
}

int Report::printReportFooters( Output *out )
{
    int ret = 0;
    for ( uint i = 0; i < mSections.size(); i++ ) {
        if ( mSections[i]->isReportFooter() ) {
            if( !out->sectionFits( mSections[i], mSections[i]->sizeY() ) ) {
                issueNewPage(out);
            }
            ret = printSection( out, mSections[ i ] );
        }
    }
    return ret;
}


int Report::printPageHeaders( Output *out )
{
    int ret = 0;
    for ( uint i = 0; i < mSections.size(); i++ ) {
        Section *s = mSections[ i ];
        if ( s->isPageHeader() ) {
            if( pageNumber() == 1 && !s->printInFirstPage() ) {
                _GONG_DEBUG_PRINT(3, "Not printing in first page");
                continue;
            } else
                ret = printSection( out, s );
        }
    }
    return ret;
}

/**
    Prints all the visible footer sections.
    The position is calculated from the bottom of the page, adding the sizeys of all the page footers
 */

int Report::printPageFooters( Output *out )
{
    int ret = 0;
    Measure totalsizey = 0.0;
    for ( uint i = 0; i < mSections.size(); i++ ) {
        if ( mSections[i]->isPageFooter() ) {
            if( mSections[i]->visible() )
                totalsizey += mSections[ i ] ->sizeY();
        }
    }
    _GONG_DEBUG_PRINT(5, Xtring::printf( "Positioning for page footers at Y: %f", out->getFolioSizeY() - out->marginBottom() - totalsizey) );
    out->setPosY( 
		(out->getFolioSizeY() / 
			(pagesPerFolio() > 1 ? (pagesPerFolio() * out->getCurrentPageInFolio()) : 1)) 
			- out->marginBottom() - totalsizey );
    for ( uint i = 0; i < mSections.size(); i++ ) {
        if ( mSections[i]->isPageFooter() ) {
            ret = printSection( out, mSections[ i ] );
        }
    }
    return ret;
}


uint Report::changedGroupValue() const
{
    for ( uint nl = 1; nl <= mGroupLevels; nl++ ) {
        for( uint ns = 0; ns < sectionsCount(); ns++ ) {
            if( mSections[ns]->groupLevel() == nl ) {
                _GONG_DEBUG_PRINT(5, Xtring::printf( "%s, realvalue=%s, prevvalue=%s",
                                                     mSections[ns]->name(), mSections[ns]->realValue().toString().c_str(),
                                                     mSections[ns]->mPrevRealValue.toString().c_str() ) );
                if( mSections[ns]->valueChanged() ) {
                    _GONG_DEBUG_PRINT(4,  Xtring::printf("Group Value Changed for level %d", ns ) );
                    return nl;
                } else
                    break;
            }
        }
    }
    return 0;
}

int Report::readNext( Passes whatpass, Input *in, InputCached *cachedinput )
{
    _GONG_DEBUG_TRACE(4);
    if ( !mStarted ) {
        mPageNumber = mPageCount = 1;
        mReportDate = DateTime::currentDateTime();
        createAggregateValues();
        if( cachedinput ) {
            if ( !cachedinput->init() )
                return false;
        } else {
            if ( !in->init() )
                return false;
        }
        mStarted = true;
        mRecordNumber = mRecordCount = 1;
        evalConstValues(in);
    }
    while( true ) { // Maybe we have to skip records based on the filter
        if( cachedinput ) {
            if ( cachedinput->next() ) {
                deserializeFields(in, static_cast<const char *>(cachedinput->key()),
                                  static_cast<const char *>(cachedinput->data()));
                evalNonConstValues(in);
                calcAggregateValues();
                // Todo: getAggregateValues from cache
                return true;
            }
            else
                return false;
        } else if ( in->next() ) {
            in->feedInputFieldValues();
            evalNonConstValues(in);
            // Check the filter
            if( !strempty(filter()) ) {
                if( !execFormula(filter()).toBool() ) {
                    _GONG_DEBUG_PRINT(4, Xtring("Skipped:") + filter() );
                    swapNonConstValues();
                    continue;
                }
            }
            if( whatpass != PASS_ORDER )
                calcAggregateValues();
            return true;
        } else {
            return false;
        }
    }
    return false;
}



/*
	In spite of mPageNumber being updated in printPageFooterIfNeeded,
	it is not updated in the object's mRealValue until the next calcConstValues,
	but that is done after printPageHeaders, so, the page number is
	always one behind. This function updates mRealValue for objects whose value =pageNumber()
	when a page change occurs.
	This problem, and many others, can only be solved when values can be references to members
*/
void Report::updatePageNumbers()
{
    for ( uint ns = 0; ns < mSections.size(); ns++ ) {
        Section *sect = mSections[ns];
        for( uint nobj=0; nobj<sect->objectsCount(); nobj++ ) {
            Object *obj = sect->mObjects[nobj];
            Xtring svalue = obj->value();
            if( svalue[0] == '=' ) {
                svalue = svalue.replace(" ","").lower();
                if( svalue == "=@report.pagenumber;"
                        || svalue == "=@report.pagenumber" )
                    obj->setRealValue( mPageNumber );
                else if( svalue == "=@report.pagecount;"
                         || svalue == "=@report.pagencuont" )
                    obj->setRealValue( mPageCount );
            }
        }
    }
}

/**
 * @brief Prints the page footer and starts a new page if section does not fit in the page
 *        This function is called before printing each section (except page or report footers)
 *
 * @param out ...
 * @param section The section that is about to be printed
 * @return int
 **/
int Report::printPageFooterIfNeeded( Output *out, Section *section )
{
    if( !out ) return 0;
    // See if the section fits in the page
    int ret = 0;
    bool mustprintpagefooter =
        !( mNewPageIssued && section->pageBefore() )
        && !section->isPageHeader() && !section->isPageFooter()
        && !out->sectionFits( section, mFootersHeight );
    if( mustprintpagefooter ) {
        ret += printPageFooters( out );
        issueNewPage(out);
        ret += printPageHeaders( out );
    }
    return ret;
}

void Report::issueNewPage( Output *out )
{
    out->endPage();
    mPageNumber++;
    mPageCount++;
    mNewPageIssued = true;
    updatePageNumbers();
    out->startPage();
}


int Report::printSection( Output *out, Section *section )
{
    mDupObjectCanBeSupr = true;
    uint i;
    if( !section->supressed() )
        section->execFormula(section->formulaBefore());
    if( mPasses == PASS_PRINT ) {
        _GONG_DEBUG_PRINT(3, Xtring::printf("Printing section: %s, RecordNumber: %d", section->name(), section->recordNumber() ) );
        if( !section->visible() || section->supressed() ) {
            _GONG_DEBUG_PRINT(3, "Section is invisible or supressed" );
            if( section->supressed() )
                return 0;
            else
                return 1;
        }
        bool mustsupress = false;
        if( section->supressDup() ) {
            mustsupress = true;
            for ( i = 0; i < section->objectsCount(); i++ ) {
                Object *obj = section->getObject(i);
                _GONG_DEBUG_PRINT(6, Xtring::printf("Object:%s, Prev:%s, Curr:%s",
                                                    obj->name(),
                                                    obj->mPrevRealValue.toString().c_str(),
                                                    obj->mRealValue.toString().c_str() ) );
                if( obj->visible() && !obj->supressed() && obj->mPrevRealValue != obj->mRealValue ) {
                    mustsupress = false;
                    break;
                }
            }
        }
        if( !mustsupress && section->supIfBlank() ) {
            mustsupress = true;
            for ( i = 0; i < section->objectsCount(); i++ ) {
                Object *obj = section->getObject(i);
                if( !obj->mRealValue.isZero() ) {
                    mustsupress = false;
                    break;
                }
            }
        }
        if( mustsupress ) {
            _GONG_DEBUG_PRINT(3, "Section is supressed because is duplicated or blank" );
            return 0;
        }
		if( !section->isPageFooter() ) {
			if (section->pageBefore() && !mNewPageIssued )  {
				printPageFooters(out);
				issueNewPage(out);
			} else {
				printPageFooterIfNeeded( out, section );
			}
		}
        // A nice hack to have alternated backcolored sections
        if( strcaseequal( section->propBackColor.getOrig(), "gray&white" ) ) {
            if( (section->recordNumber() % 2) == 0 )
                section->setBackColor( Color::Gray );
            else
                section->setBackColor( Color::White );
        }
    }
    out->startSection( *section );
    printObject( out, section ); // Print the section
    for ( i = 0; i < section->objectsCount(); i++ )
        printObject( out, section->getObject( i ) );
    section->execFormula(section->formulaAfter());
    out->endSection( *section );
    if( mPasses == PASS_PRINT ) {
        if( !section->isPageFooter() && (section->pageAfter() && !mNewPageIssued ) )
            printPageFooters( out );
	}
    if( section->isDetails() || section->isGroupFooter() || section->isGroupHeader()
            || section->isPageFooter() || section->isReportFooter() )
        mNewPageIssued = false;
    return i;
}

/**
	* @brief Prints an object in a section
	* Suppressed if duplicated:
		- The object property 'supressdup' is true, wheter originally or after the formula
		- The previous value of the object is the same than this one
		- This is not the first line in the page
		- The closest previous object that had to be supressed has in fact been supressed, so the
		  supressed values are done from left to right
		- This is not the first record in this section
	* @param out
	* @param object
	* @return
	*/
int Report::printObject( Output *out, Object *object )
{
    if( !object->isSection() && !object->supressed() )
        object->execFormula(object->formulaBefore());
    _GONG_DEBUG_PRINT(10, Xtring::printf("Formatting text for object %s" , object->name() ) );
    object->formatText();
    if( mPasses == PASS_PRINT ) {
        if ( !object->visible() || object->supressed() )
            return 0;
    }
    Section *s = (object->parent() && object->parent()->isSection()) ? static_cast<Section *>(object->parent()) : 0;
    if( object->supressDup()
            && ( s && s->recordCount() > 0 && s->recordNumber() > 0 )
            && !mNewPageIssued
            && !object->formattedValueChanged() && mDupObjectCanBeSupr ) {
        Xtring saved = object->mFormattedText;
        object->mFormattedText = "\"";
        out->printObject( *object );
        object->mFormattedText = saved;
    } else {
        if( (object->supressDup() && object->formattedValueChanged()) )
            mDupObjectCanBeSupr = false;
        out->printObject( *object );
    }
    if( !object->isSection() )
        object->execFormula(object->formulaAfter());
    return 1;
}


/**
 * Creates or resets the array of aggregate values for every object.
 * Every object contains in the mAggregateValues list the aggregate value for every group level.
 * The first element in the list (0) is the agg. value for the group level 1, and so on.
 * There is no aggregate value for the details sections, it is stored in mAggregateValue.
 * As with the RealValue, the previous mAggregateValue is stored in mPrevAggregateValue because
 * we need it if we are swapping values when a group is finished.
 */
void Report::createAggregateValues()
{
    _GONG_DEBUG_PRINT(5, Xtring::printf("mGroupLevels=%d", mGroupLevels) );
    for ( uint ns = 0; ns < mSections.size(); ns++ ) {
        for ( uint no = 0; no < mSections[ns]->objectsCount(); no++ ) {
            Object *object = mSections[ns]->getObject(no);
            if ( object->aggregate() != AggNone ) {
                for ( uint i = 0; i <= mGroupLevels; i++ ) {
                    if ( object->mAggregateValues.size() <= mGroupLevels ) {
                        object->mAggregateValues.push_back( Variant() );
                        object->mPrevAggregateValues.push_back( Variant() );
                    } else {
                        object->mAggregateValues[i] = Variant();
                        object->mPrevAggregateValues[i] = Variant();
                    }
                }
            }
        }
    }
}


/**
 * Reset the aggregate values for all the levels greater than \level
 *   for every object in the report. This is necessary when a group is finished
 *   to start the next one with all the aggregates for that group and the nested ones to 0
 * @param level group level
 */
void Report::resetAggregateValues( uint level )
{
    if( level == 0 )
        return;
    _GONG_DEBUG_PRINT(5, Xtring::printf("Reseting aggregates for level=%d", level) );
    for ( uint ns = 0; ns < mSections.size(); ns++ ) {
        for ( uint no = 0; no < mSections[ns]->objectsCount(); no++ ) {
            Object *object = mSections[ns]->getObject(no);
            if ( object->aggregate() != AggNone ) {
                for ( uint i = level; i <= mGroupLevels; i++ )  {
                    _GONG_DEBUG_PRINT(5, Xtring::printf("of %s, level %d, that was %s",
                                                        object->name(), i,
                                                        object->mAggregateValues[i].toString().c_str() ) );
                    object->mAggregateValues[i] = Variant();
                }
            }
        }
    }
}

// The current aggregate value is in mAggregateValues[mGroupLevels]

void Report::calcAggregateValues( uint level )
{
    _GONG_DEBUG_TRACE(4);
    for ( uint ns = 0; ns < mSections.size(); ns++ ) {
        for ( uint no = 0; no < mSections[ns]->objectsCount(); no++ ) {
            Object *object = mSections[ns]->getObject(no);
			if( object->aggregate() != AggNone ) {
				_GONG_DEBUG_PRINT(5, Xtring::printf("%s:%s.realValue=%s, prevValue=%s",
												object->parent()->name(),
												object->name(),
												object->parent()->mRealValue.toString().c_str(),
												object->parent()->mPrevRealValue.toString().c_str() ) );
			}
            switch ( object->aggregate() ) {
            case AggCount:
                for ( uint i = level; i <= mGroupLevels; i++ ) {
                    object->mPrevAggregateValues[i] = object->mAggregateValues[i];
                    object->mAggregateValues[i] += 1;
                    _GONG_DEBUG_PRINT(5, Xtring::printf("Counting object: %s of section %s for level %d, value=%d",
                                                        object->name(), object->parent()->name(), i, object->mAggregateValues[i].toInt() ) );
                }
                break;
            case AggDistinctCount:
                for ( uint i = level; i <= mGroupLevels; i++ ) {
                    if( object->parent()->realValueChanged() ) {
                        object->mPrevAggregateValues[i] = object->mAggregateValues[i];
                        object->mAggregateValues[i] += 1;
                        _GONG_DEBUG_PRINT(5, Xtring::printf("Distinct counting object: %s of section %s for level %d, value=%d",
                                                            object->name(), object->parent()->name(), i, object->mAggregateValues[i].toInt() ) );
                    }
                }
                break;
            case AggSum:
                for ( uint i = level; i <= mGroupLevels; i++ ) {
                    object->mPrevAggregateValues[i] = object->mAggregateValues[i];
                    object->mAggregateValues[i] += object->mRealValue; // Not realValue()
                    _GONG_DEBUG_PRINT(5, Xtring::printf("Sum=%f of object '%s' of section '%s' for level %d, value=%f",
                                                        object->mAggregateValues[i].toDouble(), object->name(), object->parent()->name(), i, object->mRealValue.toDouble() ) );
                }
                break;
            case AggDistinctSum:
                for ( uint i = level; i <= mGroupLevels; i++ ) {
                    _GONG_DEBUG_PRINT(5, Xtring::printf("%s:%s.realValue=%s, prevValue=%s",
                                                        object->parent()->name(),
                                                        object->name(),
                                                        object->parent()->mRealValue.toString().c_str(),
                                                        object->parent()->mPrevRealValue.toString().c_str() ) );
                    if( object->parent()->realValueChanged() ) {
                        object->mPrevAggregateValues[i] = object->mAggregateValues[i];
                        object->mAggregateValues[i] += object->mRealValue; // Not realValue()
                        _GONG_DEBUG_PRINT(5, Xtring::printf("Sum=%f of object '%s' of section '%s' for level %d, value=%f",
                                                            object->mAggregateValues[i].toDouble(), object->name(), object->parent()->name(), i, object->mRealValue.toDouble() ) );
                    }
                }
                break;
            case AggMax:
                for ( uint i = level; i <= mGroupLevels; i++ ) {
                    object->mPrevAggregateValues[i] = object->mAggregateValues[i];
                    if( object->mRealValue > object->mAggregateValues[i] ) {
                        object->mAggregateValues[i] = object->mRealValue;
                    }
                }
                break;
            case AggMin:
                for ( uint i = level; i <= mGroupLevels; i++ ) {
                    object->mPrevAggregateValues[i] = object->mAggregateValues[i];
                    if( object->mRealValue < object->mAggregateValues[i] ) {
                        object->mAggregateValues[i] = object->mRealValue;
                    }
                }
                break;
            case AggNone:
			default: /// TODO
                break;
            }
        }
    }
}

int Report::incRecordCount(uint level, int inc)
{
    _GONG_DEBUG_ASSERT( level >= 0 );
    for ( uint ns = 0; ns < mSections.size(); ns++ ) {
        if( mSections[ns]->groupLevel() >= level || mSections[ns]->isDetails() ) {
            mSections[ns]->mRecordNumber += inc;
            mSections[ns]->mRecordCount += inc;
        }
    }
    return 1;
}

void Report::resetRecordCount( uint level )
{
    if( level == 0 )
        return;
    for ( uint ns = 0; ns < mSections.size(); ns++ ) {
        if( mSections[ns]->groupLevel() >= level - 1 ) {
            mSections[ns]->mRecordCount = 0;
            mSections[ns]->mRecordNumber = 0;
        }
    }
}


#if 0
Report::RegisteredInputs Report::mRegisteredInputs;
/**
    Adds an Input plugin to the report engine
 */
int Report::registerInput(const Xtring &name, InputCreatorFuncPtr creator)
{
    mRegisteredInputs.insert( RegisteredInputs::value_type(name, creator) );
    return mRegisteredInputs.size();
}


Input *Report::createRegisteredInput(const char *name, const char *driver) {
    InputCreatorFuncPtr creator = mRegisteredInputs[driver];
    if( creator )
        return creator(name, driver);
    else
        return 0;
}

#endif

void Report::setStylesPath(const Xtring &path)
{
    propStylesPath.set(path.c_str());
}

void Report::setImagesPath(const Xtring &path)
{
    propImagesPath.set(path.c_str());
}

void Report::setIncludePath(const Xtring& path)
{
    propIncludePath.set(path.c_str());
}


/**
  @param nparameter The position of the parameter to get
  @return A pointer to the parameter or 0 if not found
 */
Parameter *Report::getParameter( uint nparameter ) const {
    if ( nparameter < mParameters.size() )
        return mParameters[ nparameter ];
    else
        return 0;
}

/**
  @param name The name of the parameter to find
  @return A pointer to the parameter or 0 if not found
 */
Parameter *Report::findParameter( const Xtring &name ) const {
    for ( uint i = 0; i < mParameters.size(); i++ ) {
        if ( strcasecmp(mParameters[ i ]->name(), name.upper().c_str())==0 )
            return mParameters[ i ];
    }
    return 0;
}

/**
  @param parameter Pointer to the parameter to insert. The report takes ownership of the parameter
  @return The number of parameters in the report
 */
int Report::insertParameter( Parameter *parameter ) {
    eraseParameter(parameter->name());
    mParameters.push_back( parameter );
    return mParameters.size();
}

bool Report::eraseParameter( const Xtring &name ) {
    std::vector<Parameter *>::iterator it;
    for( it=mParameters.begin(); it != mParameters.end(); ++it ) {
        if ( (*it)->name() == name ) {
            mParameters.erase(it);
            return true;
        }
    }
    return false;
}

bool Report::setParameterValue( const Xtring &name, const Xtring &value )
{
    Parameter *p = findParameter(name);
    if( p )
        p->setValue(value.c_str());
    return (p != 0);
}


void Report::fixInput(Input *in)
{
    _GONG_DEBUG_TRACE(4);
    in->fixInputParameters(mParameters, paramDelim());
}

void Report::fixOutput(Output *out)
{
    _GONG_DEBUG_TRACE(4);
    out->initPaperSizes();
}

char *Report::serializeFields(const Input *in, int *len) const
{
#ifdef _GONG_DEBUG
    Xtring dbstr = "serializeFields:";
#endif

    int buflen = 1024;
    char *buffer = new char[buflen];
    int pos = 0;
    *len = 0;
    for( uint i=0; i<in->countFields(); i++ ) {
        InputField *ifld = in->getInputField(i);
        *len += ifld->value().serialize(&buffer, &pos, &buflen);
#ifdef _GONG_DEBUG
        dbstr += ifld->value().toString() + ",";
#endif
    }
#if 0
    for ( uint ns = 0; ns < mSections.size(); ns++ ) {
        for( uint no = 0; no < mSections[ns]->objectsCount(); no++ ) {
            const Object *obj = mSections[ns]->getObject(no);
            if( !obj->isConst() ) {
                *len += obj->mRealValue.serialize(&buffer, &pos, &buflen);
#if GONG_DEBUG_CACHE > 1
                dbstr += obj->mRealValue.toString() + ",";
#endif
            }
        }
    }
#endif
    _GONG_DEBUG_PRINT( 3, dbstr );
    return buffer;
}


/**
 * Serialize the group values, only one for level.
 * The values must be strings as we are ordering the cache file.
 * If they were doubles, they wouldn't be ordered
 * @param len
 * @return
 */
char *Report::serializeGroupValues(int *len) const
{
    int buflen = 1024;
    char *buffer = new char[buflen];
    int pos = 0;
    *len = 0;
#if GONG_DEBUG_CACHE > 1
    Xtring dbgstr = "Serializing Group values:";
#endif
    /// Todo: asc/desc
    for ( uint nl = 1; nl <= mGroupLevels; nl++ ) {
        for( uint ns = 0; ns < sectionsCount(); ns++ ) {
            if( mSections[ns]->groupLevel() == nl ) {
                Variant v(mSections[ns]->mRealValue);
                *len += v.serializeAscendent(true, &buffer, &pos, &buflen);
#if GONG_DEBUG_CACHE > 1
                dbgstr += v.toString() + ",";
#endif
                continue;
            }
        }
    }
    // The order of the report was copied here
    /// Todo: list of orders and asc/desc
    *len += pFirstDetailsSection->realValue().serializeAscendent(true, &buffer, &pos, &buflen);
    // The recordcount value must be serialized (as there could be records with the same order)
    // and the last one
    *len += Variant(mRecordNumber).serializeAscendent(true, &buffer, &pos, &buflen);
#if GONG_DEBUG_CACHE > 1
    dbgstr += Variant(mRecordNumber).toString();
    _GONG_DEBUG_PRINT( RTK_DEBUG_CACHE, dbgstr );
#endif

    return buffer;
}


void Report::deserializeFields(Input *in, const char *key, const char *data)
{
#if GONG_DEBUG_CACHE > 1
    Xtring dbgstr = "Deserializing data:";
#endif
    for( uint i=0; i<in->countFields(); i++ ) {
        InputField *ifld = in->getInputField(i);
        Variant v = Variant::deserialize(&data);
        ifld->setValue(v);
#if GONG_DEBUG_CACHE > 1
        dbgstr += v.toString() + ",";
#endif
    }
#if GONG_DEBUG_CACHE > 1
    _GONG_DEBUG_PRINT( RTK_DEBUG_CACHE, dbgstr );
#endif

    /*
        for ( uint ns = 0; ns < mSections.size(); ns++ ) {
            if( !isDetails(ns) ) {
                for( uint no = 0; no < mSections[ns]->objectsCount(); no++ ) {
                    const Object *obj = mSections[ns]->getObject(no);
                    Variant v(&data, &pos);
                    obj->mAggregateValues[mSections[ns]->groupLevel()] = v;
                }
            }
        }
    */
}

void Report::setGroupLevels()
{
    mGroupLevels = 0;
    pFirstDetailsSection = 0;
    for ( uint ns = 0; ns < mSections.size(); ns++ ) {
        Section *sect = mSections[ns];
        if( sect->type() == Section::Details && !pFirstDetailsSection )
            pFirstDetailsSection = sect;
        if ( sect->groupLevel() > mGroupLevels )
            mGroupLevels = sect->groupLevel();
    }
}


void Report::setDetailsValue(const Xtring &value)
{
    if( pFirstDetailsSection )
        pFirstDetailsSection->setOrigValue( value.c_str() );
}


/**
 * @relates Report
 * @brief Prints this report to the given output object using the given input object
 *
 * @param in
 * @param out
 * @return 0 if successfull
 */
bool Report::print( Input *in, Output *out )
{
    bool ret = false;
    try {
        if( !in )
            in = defaultInput();
        _GONG_DEBUG_ASSERT( in );
        _GONG_DEBUG_ASSERT( out );
        // These functions are called only once in every print
        setGroupLevels();
        propPageOrientation.fix(mParameters, paramDelim());
        // FixOutput(out) is done at the beginning in order to set the correct sizes of the objects of this report, but the pageorientation of the output must be done once the report's properties are fixed
        if( out->pageOrientation() == DefaultOrientation ) {
            if( pageOrientation() != DefaultOrientation )
                out->setPageOrientation( pageOrientation() );
            else
                out->setPageOrientation( Portrait ) ;
        }
        fixOutput(out);
        // Need this properties fixed here
        propOrder.fix(mParameters, paramDelim());
        propFilter.fix(mParameters, paramDelim());
        fixStyles();
        fixImages();
        fixInput(in);

        InputCached *inputcached = 0;
        OutputCached *outputcachedtosort = 0, *outputcachedtogroup = 0;
        mPasses = PASS_NONE;
        mNewPageIssued = true;
        fixTotalOrder();
        bool mustorder = (mTotalOrder.size() && !in->setOrder( mTotalOrder ) );
        bool mustfilter = ( strlen(propFilter.get()) && !in->setFilter( propFilter.get() ) );
        if( !mustfilter )
            propFilter = "";
        if( mustorder | mustfilter ) {
            _GONG_DEBUG_PRINT(1, "------------ ORDERING & FILTERING INPUT:" + mTotalOrder + " ------------" );
            /* OPTIM: This pass doesn't require calculate the aggregates or
            		any operation with groups, as it doesn't make any sense
            */
            mPasses = PASS_ORDER;
            if( !strempty(order()) ) {
                if( *order() == '=' )
                    setDetailsValue( order() );
                else
                    setDetailsValue( Xtring("=") + order() );
            }
            outputcachedtosort = new OutputCached(*this, in, OutputCached::CacheMemory);
            /// \todo {check} errors here
            internalPrint( mPasses, in, outputcachedtosort );
            inputcached = new InputCached(*this, outputcachedtosort);
        }
        if( mPasses & PASS_GROUP_AHEAD ) {
            _GONG_DEBUG_PRINT( 1, "------------ AHEAD GROUPING ------------" );
            outputcachedtogroup = new OutputCached(*this, in, OutputCached::CacheMemory);
            internalPrint( mPasses, in, outputcachedtogroup, inputcached );
            delete inputcached;
            if( outputcachedtosort ) {
                delete outputcachedtosort;
                outputcachedtosort = 0;
            }
            inputcached = new InputCached(*this, outputcachedtogroup);
        }
        mPasses  = PASS_PRINT;
        _GONG_DEBUG_PRINT( 1, "------------ PRINTING ------------" );
        ret = internalPrint(mPasses, in, out, inputcached);
        if( inputcached )
            delete inputcached;
        if( outputcachedtosort )
            delete outputcachedtosort;
        if( outputcachedtogroup )
            delete outputcachedtogroup;
		ret = true;
    } catch ( Error &e ) { // This error has already been added
        ret = false;
    }
#if 0
    // Do not catch any other exception we don't know about.
    // It may be from an Input or Output plugin
    catch ( std::exception e ) {
        // Avoid rethrowing
        Error::AbortType at = Error::sErrorAbort;
        Error::sErrorAbort = Error::AbortNever;
        addError( Error::InternalError, 0, e.what() );
        Error::sErrorAbort = at;
        ret = 1;
    }
#endif
    _GONG_DEBUG_PRINT(1, Xtring::printf("FINISH:(%d)", ret) );
    return ret;
}



/**
 * As the formulas can change the values of the propertyes, we need to fix them
 * every time the report is printed (and in every pass). That is the reason why
 * we store the original properties for every object.
 * OPTIM: Fix them only if they have changed.
 * @param in
 * @param out
 * @param cachedinput
 * @return
 */
int Report::internalPrint( Passes whatpass, Input *in, Output *out, InputCached *cachedinput )
{
    _GONG_DEBUG_TRACE(2);
    int ret = 0;
    mStarted = mFinished = false;
    fixAllProperties( out );
    startScripting();
    if( (ret = out->startReport()) == 0 ) {
        this->execFormula(formulaBefore());
        out->startPage();
        if( readNext( whatpass, in, cachedinput ) ) {
            printReportHeaders(out);
            printPageHeaders( out );
            printGroup( whatpass, in, out, 1, cachedinput );
            printReportFooters(out);
            printPageFooters( out );
        } else { // The report is empty. Print something
            _GONG_DEBUG_WARNING("Empty report");
            evalConstValues(in);
            evalNonConstValues(in);
            printReportHeaders(out);
            printPageHeaders( out );
            printReportFooters(out);
            printPageFooters( out );
        }
        out->endPage();
        ret += out->endReport();
        this->execFormula(formulaAfter());
        mStarted = false;
    }
    stopScripting();
    return ret;
}


int Report::printGroupHeaders( Output *out, uint level)
{
    int ret = 0;
    for ( uint i = 0; i < mSections.size(); i++ ) {
        Section *sect = mSections[i];
        if ( sect->isGroupHeader() && sect->groupLevel() == level ) {
            ret += printSection( out, sect );
        }
    }
    return ret;
}

/**
 *
 * @param in
 * @param out
 * @param level
 * @param cachedinput
 * @return the outermost group that has changed
 */
uint Report::printGroup( Passes whatpass, Input *in, Output *out, uint level, InputCached *cachedinput)
{
    _GONG_DEBUG_PRINT(2, Xtring::printf("Printing group level:%d, pass:%d, cached:%d",
                                        level, whatpass, cachedinput != 0) );
    uint othergroupchanged = level + 1;
    while( othergroupchanged >= level && !mFinished ) {
#if GONG_DEBUG_PRINT > 1
        // This loop is just to print debug information
        for ( uint i = 0; i < mSections.size(); i++ ) {
            Section *sect = mSections[i];
            if ( sect->groupLevel() == level ) {
                _GONG_DEBUG_PRINT( 3, Xtring::printf("Group value: %s=%s)",
                                                     sect->propValue.get(), sect->realValue().toString().c_str()) );
                break;
            }
        }
#endif
        if( whatpass != PASS_ORDER && level <= mGroupLevels ) {
            printGroupHeaders(out, level);
        }
        if ( whatpass != PASS_ORDER && level <= mGroupLevels ) { // Print nested group
            othergroupchanged = printGroup( whatpass, in, out, level + 1, cachedinput );
            // Print group footer section(s) from othergroupchanged to level
            if( level >= othergroupchanged || mFinished ) {
                for ( uint i = 0; i < mSections.size(); i++ )
                    if ( mSections[i]->isGroupFooter() && mSections[i]->groupLevel() == level )
                        printSection( out, mSections[i] );
            }
            if( level == othergroupchanged && othergroupchanged > 0 && !mFinished ) {
                // Swap again summary and nonconst values to get the current ones
                ++mRecordNumber;
                ++mRecordCount;
                swapNonConstValues();
				swapAggregateValues( level - 1 );
                // The aggregate values now must be reset from this level upwards,
                // but the aggregates where computed from the last record read
                // which belonged to the new group. So, we first reset the aggregates
                // and then recalculate them
                resetAggregateValues( level );
                calcAggregateValues( level );
                resetRecordCount( level );
            }
            // Print group footer section(s)
        } else {
            // Print details section(s) until group changes or input finishes
            do {
                for ( uint i = 0; i < mSections.size(); i++ )
                    if ( mSections[i]->isDetails() )
                        if( !printSection( out, mSections[ i ] ) && mSections[i]->supressed() )
                            // section has been suppressed, so undo aggregations
                            swapAggregateValues( groupLevels() );
                mFinished = !readNext( whatpass, in, cachedinput );
                if( whatpass == PASS_ORDER )
                    othergroupchanged = 0;
                else
                    othergroupchanged = changedGroupValue();
                if( !mFinished ) {
                    ++mRecordNumber;
                    ++mRecordCount;
                    incRecordCount( level-1, +1 );
                }
            } while( !othergroupchanged && !mFinished );
            if( !mFinished ) {
                // If the input has ended, do not swap values
                // The group has changed and the last record read belongs to this new group,
                // but we have not printed the summary sections of the previous group yet, so we have
                // to recover the former aggregate and non-constant fields, that is why we swap them.
                --mRecordNumber;
                --mRecordCount;
                incRecordCount( level-1, -1 );
                swapNonConstValues();
                swapAggregateValues( groupLevels() );
            }
        }
    }
    return othergroupchanged;
}


/*
    To detect group changes, we compare values with previous values, but when the group
    actually changes, we have to print the values of the previous record. So we swap them
    before printing and swap again after printing to left the values unchanged.
    We dont need to swap recordcounts as these are not updated yet.
*/

void Report::swapNonConstValues()
{
    _GONG_DEBUG_TRACE(4);
    for ( uint ns = 0; ns < mSections.size(); ns++ ) {
        Section *s = mSections[ns];
        if( !s->isConst() )
            s->swapNonConstValues();
        for( uint no = 0; no < s->objectsCount(); no++ ) {
            Object *obj = s->getObject(no);
            if( !obj->isConst() )
                obj->swapNonConstValues();
        }
    }
}

void Report::swapAggregateValues( uint level )
{
    _GONG_DEBUG_TRACE(4);
    for ( uint ns = 0; ns < mSections.size(); ns++ ) {
        Section *s = mSections[ns];
        if( s->aggregate() != AggNone )
            s->swapAggregateValues( level );
        for( uint no = 0; no < s->objectsCount(); no++ ) {
            Object *obj = s->getObject(no);
            if( obj->aggregate() != AggNone )
                obj->swapAggregateValues( level );
        }
    }
}

#ifdef _GONG_DEBUG
Xtring Report::debugPrint(const Xtring &file, const Xtring &function, int line) const
{
    return file + ":" + Xtring::number(line) + ':' + function + ": ";
}
#endif

/// \todo {?} Create a list of order fields, with asc and desc
void Report::fixTotalOrder()
{
    mTotalOrder.clear();
    for( uint l=1; l<=mGroupLevels; l++ ) {
        for( uint s=0; s<sectionsCount(); s++ ) {
            Section *ps = mSections[s];
            if( ps->groupLevel() == l ) {
                ps->propValue.fix( mParameters, paramDelim() );
                if( !strempty(ps->value()) ) {
                    if( mTotalOrder.size() )
                        mTotalOrder += ',';
                    mTotalOrder += ps->value();
                }
            }
        }
    }
    if( !strempty(order()) ) {
        if( mTotalOrder.size() )
            mTotalOrder += ',';
        mTotalOrder += order();
    }
    _GONG_DEBUG_PRINT(4, mTotalOrder );
}


int Report::addError( Error *anError )
{
    mErrors.push_back( anError );
    if( (anError->isError() && (Error::errorAbort() == Error::ThrowOnError) ) )
        throw Error( anError->type(), anError->code(), anError->location(), anError->what(), anError->text() );
    if( (anError->isError() && (Error::errorAbort() == Error::AbortOnError) )
            || Error::sErrorAbort == Error::AbortAlways ) {
        for( int e=0; e < errorsCount(); e++ ) {
            std::cerr << "RTK " << (getError(e)->isWarning()? "Warning: ":"Error: ")
                      << getError(e)->location() << ": " << getError(e)->message() << std::endl << getError(e)->text() << std::endl;
        }
        std::cerr << "RTK Aborting ( Error::sErrorAbort = " <<
                  ((Error::sErrorAbort == Error::AbortAlways) ?  "AbortAlways" :
                   (Error::sErrorAbort == Error::AbortOnError) ?  "AbortOnError" : "AbortOnWarning")
                  << ", exitcode:" << anError->code() << " )" <<  std::endl;
        exit(anError->code());
    }
    return mErrors.size();
}

void Report::setLocation(const Xtring &location)
{
    if( location != mLocation ) {
        mLocation = location;
        showProgress(location, 0, 0);
    }
}

void Report::setSubLocation(const Xtring &sublocation)
{
    mSubLocation = sublocation;
}

int Report::showProgress(const Xtring &, int, int)
{
    return 0;
}


int Report::addError( Error::ErrorCode code, const char *loc, const char *text, const char *message, ...)
{
    va_list args;
    char buffer[2040] = "";
    if( strempty(loc) )
        loc = mLocation.c_str();
    if( message == 0 )
        message = Error::getFormat(code);
    if( message != 0 ) {
        va_start(args, message);
        vsnprintf(buffer, 2039, message, args);
        va_end( args );
    }
    Error *perr = new Error(Error::EError, code, loc, buffer, text);
    return addError(perr);
}

int Report::addWarning( Error::ErrorCode code, const char *loc, const char *text, const char *message, ...)
{
    va_list args;
    char buffer[2040] = "";
    if( strempty(loc) )
        loc = mLocation.c_str();
    if( message == 0 )
        message = Error::getFormat(code);
    if( message != 0 ) {
        va_start(args, message);
        vsnprintf(buffer, 2039, message, args);
        va_end( args );
    }
    Error *perr = new Error(Error::EWarning, code, loc, buffer, text);
    return addError(perr);
}

int Report::addNotice( Error::ErrorCode code, const char *loc, const char *text, const char *message, ...)
{
    va_list args;
    char buffer[2040] = "";
    if( strempty(loc) )
        loc = mLocation.c_str();
    if( message == 0 )
        message = Error::getFormat(code);
    if( message != 0 ) {
        va_start(args, message);
        vsnprintf(buffer, 2039, message, args);
        va_end( args );
    }
    Error *perr = new Error(Error::ENotice, code, loc, buffer, text);
    return addError(perr);
}


const Error *Report::lastError() const
{
    if( !mErrors.size() )
        return 0;
    return mErrors[mErrors.size()-1];
}

const Error *Report::getError(uint i) const
{
    if( i >= mErrors.size() )
        return 0;
    return mErrors[i];
}


Variant Report::getPropValue(const Xtring & propname) const
{
    const char *name = propname.c_str();
    if( strcasecmp(name, "VALUE" ) == 0 )
        return mRealValue;
    else if( strcasecmp(name, "Filter") == 0 )
        return propFilter.getOrig();
    /*<<<<<REPORT_GETPROPVALUE*/
	else if( strcasecmp(name, "Title") == 0 )
		return propTitle.get();
	else if( strcasecmp(name, "Units") == 0 )
		return propUnits.get();
	else if( strcasecmp(name, "PageOrientation") == 0 )
		return propPageOrientation.get();
	else if( strcasecmp(name, "StylesPath") == 0 )
		return propStylesPath.get();
	else if( strcasecmp(name, "ImagesPath") == 0 )
		return propImagesPath.get();
	else if( strcasecmp(name, "IncludePath") == 0 )
		return propIncludePath.get();
	else if( strcasecmp(name, "ParamDelim") == 0 )
		return propParamDelim.get();
	else if( strcasecmp(name, "Order") == 0 )
		return propOrder.get();
	else if( strcasecmp(name, "Copies") == 0 )
		return propCopies.get();
	else if( strcasecmp(name, "PagesPerFolio") == 0 )
		return propPagesPerFolio.get();
/*>>>>>REPORT_GETPROPVALUE*/
// {capel} Eliminar el strcasecmp del filter
    else if( strcasecmp(name, "Date") == 0 )
        return reportDate();
    else if ( strcaseequal(name, "RecordCount" ) )
        return recordCount();
    else if ( strcaseequal(name, "RecordNumber" ) )
        return recordNumber();
    else if ( strcaseequal(name, "PageNumber" ) )
        return pageNumber();
    else if ( strcaseequal(name, "PageCount" ) )
        return pageCount();
    else if( Object::getPropValue(propname) == Variant() ) {
        if( propname.find(".") != Xtring::npos ) {
            Xtring sectname, pname;
            propname.splitIn2( sectname, pname, "." );
            for( uint ns=0; ns<mSections.size(); ns++ ) {
                Section *sect = mSections[ns];
                if( !strcaseequal( sect->name(), sectname.c_str() ) )
                    return sect->getPropValue( pname );
            }
        }
    }
    return Variant();
}

bool Report::setPropValue(const Xtring & propname, const Variant &value)
{
    const char *name = propname.c_str();
    if( strcasecmp(name, "VALUE" ) == 0 )
        mRealValue = value;
    /*<<<<<REPORT_SETPROPVALUE*/
	else if( strcasecmp(name, "Title") == 0 )
		propTitle.set( value.toString().c_str());
	else if( strcasecmp(name, "Units") == 0 )
		propUnits.set( static_cast<UnitsType>( value.toInt() ));
	else if( strcasecmp(name, "PageOrientation") == 0 )
		propPageOrientation.set( static_cast<PageOrientation>( value.toInt() ));
	else if( strcasecmp(name, "StylesPath") == 0 )
		propStylesPath.set( value.toString().c_str());
	else if( strcasecmp(name, "ImagesPath") == 0 )
		propImagesPath.set( value.toString().c_str());
	else if( strcasecmp(name, "IncludePath") == 0 )
		propIncludePath.set( value.toString().c_str());
	else if( strcasecmp(name, "ParamDelim") == 0 )
		propParamDelim.set( value.toString().c_str());
	else if( strcasecmp(name, "Order") == 0 )
		propOrder.set( value.toString().c_str());
	else if( strcasecmp(name, "Filter") == 0 )
		propFilter.set( value.toString().c_str());
	else if( strcasecmp(name, "Copies") == 0 )
		propCopies.set( value.toInt());
	else if( strcasecmp(name, "PagesPerFolio") == 0 )
		propPagesPerFolio.set( value.toInt());
/*>>>>>REPORT_SETPROPVALUE*/
    else if( strcasecmp(name, "Date") == 0 )
        mReportDate = value.toDateTime();
    else if( !Object::setPropValue(propname, value) ) {
        if( propname.find(".") != Xtring::npos ) {
            Xtring sectname, pname;
            propname.splitIn2( sectname, pname, "." );
            for ( uint ns = 0; ns < mSections.size(); ns++ ) {
                Section *sect = mSections[ns];
                if( !strcaseequal( sect->name(), sectname.c_str() ) )
                    return sect->setPropValue( pname, value );
            }
        }
        return false;
    }
    return true;
}

bool Report::setPropOrigValue(const Xtring & propname, const Xtring &value)
{
    const char *name = propname.c_str();
    if( strcasecmp(name, "VALUE" ) == 0 )
        mRealValue = value;
    /*<<<<<REPORT_SETORIGPROPVALUE*/
	else if( strcasecmp(name, "Title") == 0 )
		propTitle.setOrig( value.c_str() );
	else if( strcasecmp(name, "Units") == 0 )
		propUnits.setOrig( value.c_str() );
	else if( strcasecmp(name, "PageOrientation") == 0 )
		propPageOrientation.setOrig( value.c_str() );
	else if( strcasecmp(name, "StylesPath") == 0 )
		propStylesPath.setOrig( value.c_str() );
	else if( strcasecmp(name, "ImagesPath") == 0 )
		propImagesPath.setOrig( value.c_str() );
	else if( strcasecmp(name, "IncludePath") == 0 )
		propIncludePath.setOrig( value.c_str() );
	else if( strcasecmp(name, "ParamDelim") == 0 )
		propParamDelim.setOrig( value.c_str() );
	else if( strcasecmp(name, "Order") == 0 )
		propOrder.setOrig( value.c_str() );
	else if( strcasecmp(name, "Filter") == 0 )
		propFilter.setOrig( value.c_str() );
	else if( strcasecmp(name, "Copies") == 0 )
		propCopies.setOrig( value.c_str() );
	else if( strcasecmp(name, "PagesPerFolio") == 0 )
		propPagesPerFolio.setOrig( value.c_str() );
/*>>>>>REPORT_SETORIGPROPVALUE*/
    else if( !Object::setPropOrigValue(propname, value) ) {
        if( propname.find(".") != Xtring::npos ) {
            Xtring sectname, pname;
            propname.splitIn2( sectname, pname, "." );
            for ( uint ns = 0; ns < mSections.size(); ns++ ) {
                Section *sect = mSections[ns];
                if( !strcaseequal( sect->name(), sectname.c_str() ) )
                    return sect->setPropOrigValue( pname, value );
            }
        }
        return false;
    }
    return true;
}

/**
 * This function is called by spirit to get the value of any property of the report or of the input
 * The properties in formulae must be preceeded by @:
 *  ex: value = "=return \"Remesa número \" + @input.REMESACOBRO_NUMERO + \", \" + @input.REMESACOBRO_DESCRIPCION;";
 * @param propname Ex: report.PageOrientation, input.CODE, ...
 * @return the value of the property
 */
Variant Report::getExternalPropertyValue(const Xtring &propname) const
{
    _GONG_DEBUG_PRINT(5, propname );
    Xtring object = propname.left(propname.find('.') );
    Xtring property = propname.mid( propname.find('.')+1 );
    if( object.lower() == "input" ) {
        int ninputfield = defaultInput()->findInputField(property);
        if( ninputfield != -1 ) {
            return defaultInput()->getInputField(ninputfield)->value();
        }
    } else if( object.lower() == "report" ) {
        return this->getPropValue( property );
    } else { // test if the property belongs to a section
        for( uint ns = 0; ns < sectionsCount(); ns ++ ) {
            Section *sect = getSection(ns);
            if( Xtring(sect->name()).lower() == object.lower() )
                return sect->getPropValue( property );
            else {
                // test if the property belongs to an object
                for( uint no = 0; no < sect->objectsCount(); no ++ ) {
                    if( Xtring(sect->getObject(no)->name()).lower() == object.lower() )
                        return sect->getObject(no)->getPropValue( property );
                }
            }
        }
    }
    return Variant();
}


bool Report::setExternalPropertyValue(const Xtring &propname, const Variant &value)
{
    Xtring object = propname.left(propname.find('.') );
    Xtring property = propname.mid( propname.find('.')+1 );
    /*
    	if( object.lower() == "input" ) {
    		int ninputfield = defaultInput()->findInputField(property);
    		if( ninputfield != -1 ) {
    			return defaultInput()->getInputField(ninputfield)->value();
    		}
    	} else
    */
    if( object.lower() == "report" ) {
        return this->setPropValue( property, value );
    } else { // test if the property belongs to a section
        for( uint ns = 0; ns < sectionsCount(); ns ++ ) {
            Section *sect = getSection(ns);
            if( Xtring(sect->name()).lower() == object.lower() )
                return sect->setPropValue( property, value );
            else {
                // test if the property belongs to an object
                for( uint no = 0; no < sect->objectsCount(); no ++ ) {
                    if( Xtring(sect->getObject(no)->name()).lower() == object.lower() )
                        return sect->getObject(no)->setPropValue( property, value );
                }
            }
        }
    }
    return false;
}

Variant Report::callExternalFunction(const Xtring& function, Variant& value1, Variant &value2)
{
	return Variant();
}


#if USE_CXX_FORMULA

Report::cxx_formula *Report::findDLSymbol(const char *formulaname)
{
    Report::cxx_formula *retfunc;
    char *dlerror_result;
    for( std::vector<void *>::const_iterator it = mDLHandles.begin();
            it != mDLHandles.end();
            ++ it ) {
        dlerror();
        retfunc = reinterpret_cast<Report::cxx_formula *>(dlsym( *it, formulaname ));
        dlerror_result = dlerror();
        if( dlerror_result )
            addWarning(Error::FormulaCxxNotFound, __FUNCTION__, 0, formulaname, name() );
        else
            return retfunc;
    }
    return 0;
}

bool Report::loadFormula(const char *libname)
{
    void *hiden_handle = dlopen(libname, RTLD_LAZY);
    if( hiden_handle ) {
        mDLHandles.push_back(hiden_handle); // Main program
        return true;
    } else {
        addError( Error::FormulaLibraryNotFound, __FUNCTION__, 0, libname, dlerror());
    }
    return false;
}

#endif

} // namespace RTK
