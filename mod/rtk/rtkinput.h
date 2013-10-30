#ifndef RTKINPUT_H
#define RTKINPUT_H

/*<<<<<COPYLEFT*/
/** @file rtkinput.h Input object
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

#include <vector>

#include <gongxtring.h>
#include <gongvariant.h>
#include "rtkproperty.h"
#include "rtkparameter.h"
#include "rtkinputfield.h"

namespace gong {}
namespace RTK = gong;

namespace gong {

class RegConfig;

class Input {
public:
    Input(class Report &r, const char * name, const char *driver = 0)
        : propName(name), propDriver(driver), mReport(r), pRegConfig(0)
    {};
    virtual ~Input();
    /* Override to init the query and set the source field of every input field */
    virtual bool init() = 0;
    /* Override to read the next record from the input */
    virtual bool next() = 0;
    /* Override to get the value of the nth source field from the input */
    virtual Variant getValue(uint sourcepos, Variant::Type type) const = 0;
    /* Override to order the input on this fields */
    virtual bool setOrder(const Xtring &/*orderby*/) {
        return false;
    }
    /* Override to filter the records */
    virtual bool setFilter(const Xtring &/*filter*/) {
        return false;
    }
    /* Override to parse extra attrs declared in the rtk template for this input */
    virtual bool parseAttribute(const char */*attribute*/, const char */*value*/)
    {
        return true;
    };
    RegConfig *getRegConfig() const {
        return pRegConfig;
    }
    void setRegConfig(RegConfig *rc) {
        pRegConfig = rc;
    }

    uint countFields() const {
        return mInputFields.size();
    }
    // Must return int
    int findInputField(const Xtring &name) const;
    InputField *getInputField(uint i) const {
        return ((i>=mInputFields.size())?0:mInputFields[i]);
    }
    int insertInputField(InputField *inputfield);
    int feedInputFieldValues();
    virtual void fixInputParameters(const ParametersList &parameters, const char *delim);

    /*<<<<<INPUT_PROPERTIES*/
public:
    const char * name() const {
        return propName.get();
    }
    const char * driver() const {
        return propDriver.get();
    }
    const char * encoding() const {
        return propEncoding.get();
    }
    void setOrigEncoding(const char * encoding )		{
        propEncoding.setOrig(encoding);
    }

public:
    ReadOnlyProperty<const char *> propName;
    ReadOnlyProperty<const char *> propDriver;
    FixableProperty<const char *> propEncoding;
    /*>>>>>INPUT_PROPERTIES*/


protected:
    std::vector<InputField *> mInputFields;
    class Report &mReport;
    RegConfig *pRegConfig;

#ifdef _GONG_DEBUG
protected:
    virtual Xtring debugPrint(const Xtring &file, const Xtring &function, int nline) const;
    friend class TestInput;
#endif


};

} // namespace

#endif
