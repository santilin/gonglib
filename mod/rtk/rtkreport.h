#ifndef RTKREPORT_H
#define RTKREPORT_H

/*<<<<<COPYLEFT*/
/** @file rtkreport.h RTK report object
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
#include <map>

#include "rtkconfig.h"
#include <gongxtring.h>
#include <gongvariant.h>
#include "rtkformula.h"
#include "rtkstyle.h"
#include "rtkparameter.h"
#include "rtkoutput.h"
#include "rtkinput.h"
#include "rtkobject.h"
#include "rtksection.h"
#include "rtkerror.h"



#ifdef HAVE_BOOSTSPIRIT
namespace dscript {
class context;
}
#endif

namespace gong {

class InputCached;
class Formatter;
class RegConfig;

class Report: public Object
{
public:
    Report(RegConfig *pRegConfig = 0);
    ~Report();

    bool readRtkFile( const char *rtkfilename, const char *defaultinput = 0,
                      Input *usethisinput = 0, const Xtring &initdefines = Xtring::null);
    bool readRtkString( const char *rtkstring, const char *defaultinput = 0,
                        const char *filename = 0, Input *usethisinput = 0, const Xtring &initdefines = Xtring::null );

    bool print( Input *in, Output *out );
    bool print( Output *out ) { return print(pDefaultInput, out); }
    const Formatter &formatter() const { return *pFormatter; }
    char *serializeGroupValues(int *len) const;
    char *serializeFields(const Input *in, int *len) const;
    uint groupLevels() const { return mGroupLevels; }
    void fixAllProperties(const Output *out);

    const Xtring &location() const {
        return mLocation;
    }
    const Xtring &subLocation() const {
        return mSubLocation;
    }
    void setLocation(const Xtring &location);
    void setSubLocation(const Xtring &sublocation);
    virtual int showProgress(const Xtring &location, int pos, int max);

    /** @brief Gets a pointer to the input associated to this Report */
    Input *defaultInput() const {
        return pDefaultInput;
    }
    /** @brief Sets the input that will be used with this Report */
    void setDefaultInput( Input *input );

    /** @brief Finds an Image in the Report by name*/
    Image *findImage( const Xtring &name ) const;
    /** @brief Deletes an Image from the Report */
    bool eraseImage( const Xtring &name );
    /** @brief Inserts an Image in the Report */
    int insertImage( Image *image );

    /** @brief Inserts a Section in the Report */
    int insertSection( Section *section );
    /** @brief Finds a section->by its name */
    Section *findSection( const Xtring &name ) const;
    /** @brief Finds a section->by its index */
    Section *getSection( uint nsection ) const;
    /** @brief Returns the number of sections */
    uint sectionsCount() const {
        return mSections.size();
    }

    /** @brief Finds a style by its name */
    Style *findStyle( const Xtring &name ) const;
    /** @brief Finds a style by its index */
    Style *getStyle( uint nstyle ) const;
    /** @brief Inserts a Style in the Report */
    int insertStyle( Style *style );
    /** @brief Deletes a Style from the Report */
    bool eraseStyle( const Xtring &name );

    /** @brief Finds a Parameter by its name */
    Parameter *findParameter( const Xtring &name ) const;
    /** @brief Finds a Parameter by its index */
    Parameter *getParameter( uint nParameter ) const;
    /** @brief Finds a Parameter by its index */
    uint getParametersCount() const {
        return mParameters.size();
    }
    /** @brief Inserts a Parameter in the Report */
    int insertParameter( Parameter *Parameter );
    /** @brief Deletes a Parameter from the Report */
    bool eraseParameter( const Xtring &name );
    /** @brief Sets a parameter value */
    bool setParameterValue( const Xtring &name, const Xtring &value );

    /** @brief Appends an Error to the Report */
    int addError( Error *anError );
    int addError( Error::ErrorCode code, const char *loc, const char *message...);
    int addWarning( Error::ErrorCode code, const char *loc, const char *message...);
    int addNotice( Error::ErrorCode code, const char *loc, const char *message...);
    const Error *lastError() const;
    const Error *getError(uint i) const;
    int errorsCount() const {
        return mErrors.size();
    }

    void setImagesPath(const Xtring &path);
    void setStylesPath(const Xtring &path);
    void setIncludePath( const Xtring &path);
    const Xtring &getCWD() const {
        return mCWD;
    }

    RegConfig *getRegConfig() const {
        return pRegConfig;
    }

protected:
    void clearErrors() {
        mErrors.clear();
    }

// Report properties
public:
    /* Looks for a property in the whole report, usually called from formulae */
    virtual Variant getExternalPropertyValue(const Xtring &propname) const;
    virtual bool setExternalPropertyValue(const Xtring &propname, const Variant &value);
	virtual Variant callExternalFunction(const Xtring &function, Variant &value1, Variant &value2);
    /* Gets a report's property value by name */
    virtual Variant getPropValue(const Xtring &propname) const;
    /* Sets a report's property working value by name */
    virtual bool setPropValue(const Xtring &propname, const Variant &value);
    /* Sets a report's property original value by name */
    bool setPropOrigValue(const Xtring & propname, const Xtring &value);
    const DateTime reportDate() const {
        return mReportDate;
    }
    uint recordCount() const {
        return mRecordCount;
    }
    uint recordNumber() const {
        return mRecordNumber;
    }
    uint pageCount() const {
        return mPageCount;
    }
    uint pageNumber() const {
        return mPageNumber;
    }
    void setTitle(const char *title) {
        propTitle.set(title);
    }

    /*<<<<<REPORT_PROPERTIES*/
public:
    const char * title() const {
        return propTitle.get();
    }
    void setOrigTitle(const char * title )		{
        propTitle.setOrig(title);
    }
    UnitsType units() const {
        return propUnits.get();
    }
    void setOrigUnits(const char * units )		{
        propUnits.setOrig(units);
    }
    PageOrientation pageOrientation() const {
        return propPageOrientation.get();
    }
    void setOrigPageOrientation(const char * pageorientation )		{
        propPageOrientation.setOrig(pageorientation);
    }
    const char * stylesPath() const {
        return propStylesPath.get();
    }
    void setOrigStylesPath(const char * stylespath )		{
        propStylesPath.setOrig(stylespath);
    }
    const char * imagesPath() const {
        return propImagesPath.get();
    }
    void setOrigImagesPath(const char * imagespath )		{
        propImagesPath.setOrig(imagespath);
    }
    const char * includePath() const {
        return propIncludePath.get();
    }
    void setOrigIncludePath(const char * includepath )		{
        propIncludePath.setOrig(includepath);
    }
    const char * paramDelim() const {
        return propParamDelim.get();
    }
    void setOrigParamDelim(const char * paramdelim )		{
        propParamDelim.setOrig(paramdelim);
    }
    const char * order() const {
        return propOrder.get();
    }
    void setOrigOrder(const char * order )		{
        propOrder.setOrig(order);
    }
    const char * filter() const {
        return propFilter.get();
    }
    void setOrigFilter(const char * filter )		{
        propFilter.setOrig(filter);
    }
    int copies() const {
        return propCopies.get();
    }
    void setOrigCopies(const char * copies )		{
        propCopies.setOrig(copies);
    }
    int pagesPerFolio() const {
        return propPagesPerFolio.get();
    }
    void setOrigPagesPerFolio(const char * pagesperfolio )		{
        propPagesPerFolio.setOrig(pagesperfolio);
    }
    virtual void fixParameters(const ParametersList &parameters, const char *delim); // from Input

public:
    FixableProperty<const char *> propTitle;
    FixableProperty<UnitsType> propUnits;
    FixableProperty<PageOrientation> propPageOrientation;
    FixableProperty<const char *> propStylesPath;
    FixableProperty<const char *> propImagesPath;
    FixableProperty<const char *> propIncludePath;
    FixableProperty<const char *> propParamDelim;
    FixableProperty<const char *> propOrder;
    FixableProperty<const char *> propFilter;
    FixableProperty<int> propCopies;
    FixableProperty<int> propPagesPerFolio;
    /*>>>>>REPORT_PROPERTIES*/

private:
    enum Passes { PASS_NONE = 0, PASS_ORDER = 1, PASS_GROUP_AHEAD = 1 << 1, PASS_PRINT = 1 << 2,
                  PASS_ALL = PASS_ORDER|PASS_GROUP_AHEAD|PASS_PRINT
                };
    void fixStyles();
    void fixImages();
    void fixInput(Input *in);
    void fixOutput(Output *out);
    void startScripting();
    void stopScripting();
    int internalPrint(Passes whatpass, Input *in, Output *out, InputCached *cachedinput = 0);
    int printReportHeaders( Output *out );
    int printReportFooters( Output *out );
    int printPageHeaders( Output *out );
    int printPageFooters( Output *out );
    int printPageFooterIfNeeded( Output *, Section * );
    int printGroupHeaders( Output *, uint level);
    uint printGroup( Passes whatpass, Input *in, Output *out, uint level, InputCached *cachedinput );
    uint changedGroupValue() const;
    void swapNonConstValues();
    void swapAggregateValues( uint level );
    int printSection( Output *, Section * );
    int printObject( Output *, Object * );
    void updatePageNumbers();
    void issueNewPage( Output * );

    void setGroupLevels();
    Measure calcSizeYSections( Section::SectionType type ) const;
    void setDetailsValue(const Xtring &value);

    int readNext( Passes whatpass, Input *in, InputCached *cachedinput );
    void createAggregateValues();
    void resetAggregateValues( uint level );
    void calcAggregateValues(uint level = 0/*all*/);
    void evalConstValues(const Input *in);
    Variant evalConstValue( const Xtring &code, const Input *in, bool *isconst, int *nfield );
    void evalNonConstValues(const Input *in);
    Variant evalNonConstValue( Object *object, const Input *in, int ninputfield);
    Passes evalPasses(const Object *object) const;
    Passes evalPassesFormula(const Object *object, const Xtring &code) const;
    int incRecordCount( uint level, int inc );
    void resetRecordCount( uint level );
    void deserializeFields(Input *in, const char *groupvalues, const char *fieldvalues);
    void fixTotalOrder();

    DateTime mReportDate;
    int mPageNumber, mPageCount;
    int mRecordNumber, mRecordCount;
    Xtring mCWD;
    Section *pFirstDetailsSection;
    uint mGroupLevels;
    Measure mFootersHeight;
    Xtring mInputName;
    std::vector<Section *>mSections;
    std::vector<Style *>mStyles;
    ParametersList mParameters;
    std::vector<Image *>mImages;
    bool mStarted, mFinished;
    Input *pDefaultInput;
    Formatter *pFormatter;
    RegConfig *pRegConfig;
    bool mDeleteRegConfig;
    Xtring mTotalOrder;
    std::vector<Error *>mErrors;
    Xtring mLocation, mSubLocation;
    bool mDupObjectCanBeSupr;
    bool mNewPageIssued; /** true if a new page header has been printed or start of report */
    Passes mPasses;

public:
    typedef Input* ( *InputCreatorFuncPtr ) ( const Xtring &name, const Xtring &driver );
    typedef std::map<Xtring, Report::InputCreatorFuncPtr> RegisteredInputs;
    static int registerInput( const Xtring &name, InputCreatorFuncPtr creator );
    static Input *createRegisteredInput( const Xtring &name, const Xtring &driver );
private:
    static RegisteredInputs mRegisteredInputs;

#ifdef HAVE_BOOSTSPIRIT
public:
    class dscript::context *getFormulaContext() const {
            return pFormulaContext;
        }
private:
    class dscript::context *pFormulaContext;
#endif
#ifdef USE_CXX_FORMULA
public:
    typedef Variant (cxx_formula) (Object *obj, const char *param);
    cxx_formula *findDLSymbol(const char *formulaname);
    bool loadFormula(const char *libname = 0);
private:
    std::vector<void *> mDLHandles;
#endif

#ifdef _GONG_DEBUG
protected:
    virtual Xtring debugPrint(const Xtring &file, const Xtring &function, int line) const;
#endif

};

} // namespace

#endif // RTKREPORT_H
