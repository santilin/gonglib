#ifndef RTKSECTION_H
#define RTKSECTION_H

/*<<<<<COPYLEFT*/
/** @file rtksection.h RTK section object
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
#include "rtkobject.h"

namespace gong {}
namespace RTK = gong;

namespace gong {

class Section: public Object
{
public:
    friend class Report;
    friend class Object;

    enum SectionType {
        ReportHeader,
        ReportFooter,
        PageHeader,
        PageFooter,
        GroupHeader,
        GroupFooter,
        Details
    };

    Section(SectionType type, Object *parent, const char *name );
    ~Section();

    int insertObject(Object *object);
    uint objectsCount() const {
        return mObjects.size();
    }
    Object *findObject(const Xtring &name) const;
    Object *getObject(int i) const;
    bool isReportHeader() const {
        return type() == ReportHeader;
    }
    bool isReportFooter() const {
        return type() == ReportFooter;
    }
    bool isPageHeader() const {
        return type() == PageHeader;
    }
    bool isPageFooter() const {
        return type() == PageFooter;
    }
    bool isGroupHeader() const {
        return type() == GroupHeader;
    }
    bool isGroupFooter() const {
        return type() == GroupFooter;
    }
    bool isDetails() const {
        return type() == Details;
    }
    void setGroupLevel(uint level) {
        propGroupLevel.set(level);
    }
    int recordNumber() const {
        return mRecordNumber;
    }
    int recordCount() const {
        return mRecordCount;
    }
    /* Gets a section's property value by name */
    virtual Variant getPropValue(const Xtring &propname) const;
    /* Sets a section's property working value by name */
    virtual bool setPropValue(const Xtring &propname, const Variant &value);
    /* Sets a section's property original value by name */
    bool setPropOrigValue(const Xtring &propname, const Xtring &value);

    /*<<<<<SECTION_PROPERTIES*/
public:
    SectionType type() const {
        return propType.get();
    }
    uint groupLevel() const {
        return propGroupLevel.get();
    }
    int columns() const {
        return propColumns.get();
    }
    void setOrigColumns(const char * columns )		{
        propColumns.setOrig(columns);
    }
    bool pageBefore() const;
    void setOrigPageBefore(const char * pagebefore )		{
        propPageBefore.setOrig(pagebefore);
    }
    bool pageAfter() const;
    void setOrigPageAfter(const char * pageafter )		{
        propPageAfter.setOrig(pageafter);
    }
    bool resetNPage() const;
    void setOrigResetNPage(const char * resetnpage )		{
        propResetNPage.setOrig(resetnpage);
    }
    bool supIfBlank() const;
    void setOrigSupIfBlank(const char * supifblank )		{
        propSupIfBlank.setOrig(supifblank);
    }
    bool repeatHeader() const;
    void setOrigRepeatHeader(const char * repeatheader )		{
        propRepeatHeader.setOrig(repeatheader);
    }
    bool printInFirstPage() const;
    void setOrigPrintInFirstPage(const char * printinfirstpage )		{
        propPrintInFirstPage.setOrig(printinfirstpage);
    }
    Measure padding() const;
    void setOrigPadding(const char * padding )		{
        propPadding.setOrig(padding);
    }
    Measure paddingLeft() const;
    void setOrigPaddingLeft(const char * paddingleft )		{
        propPaddingLeft.setOrig(paddingleft);
    }
    Measure paddingTop() const;
    void setOrigPaddingTop(const char * paddingtop )		{
        propPaddingTop.setOrig(paddingtop);
    }
    virtual void fixParameters(const ParametersList &parameters, const char *delim); // from Input

public:
    ReadOnlyProperty<SectionType> propType;
    Property<uint> propGroupLevel;
    FixableProperty<int> propColumns;
    FixableProperty<bool> propPageBefore;
    FixableProperty<bool> propPageAfter;
    FixableProperty<bool> propResetNPage;
    FixableProperty<bool> propSupIfBlank;
    FixableProperty<bool> propRepeatHeader;
    FixableProperty<bool> propPrintInFirstPage;
    FixableProperty<Measure> propPadding;
    FixableProperty<Measure> propPaddingLeft;
    FixableProperty<Measure> propPaddingTop;
    /*>>>>>SECTION_PROPERTIES*/

private:
    std::vector<Object *> mObjects;
    int mRecordNumber, mRecordCount;
};

} // namespace

#endif
