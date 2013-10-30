#ifndef INPUTCSV_H
#define INPUTCSV_H

/*<<<<<COPYLEFT*/
/** @file rtkinputcsv.h Standard csv input object
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
#include "rtkinput.h"

/**
  *@author Francisco Santiago Capel Torres
  */

namespace gong {}
namespace RTK = gong;

namespace gong {

class InputCsv : public Input
{
public:
    InputCsv(class Report &r, const char *name, const char *filename = 0,
             const char *delims = 0, const char *quote = 0, int headerlines = 0)
        :Input(r, name, "csv"), propFilename(filename), propDelimiters(delims),
         propQuote(quote), propHeaderLines(headerlines), pFile( 0 )
    {};
    ~InputCsv();

    virtual bool parseAttribute(const char *attribute, const char *value); // from Input
    virtual bool init(); // from Input
    virtual bool next(); // from Input
    virtual Variant getValue(uint i, Variant::Type type) const; // from Input

    /*<<<<<INPUTCSV_PROPERTIES*/
public:
    const char * filename() const {
        return propFilename.get();
    }
    void setOrigFilename(const char * filename )		{
        propFilename.setOrig(filename);
    }
    const char * delimiters() const {
        return propDelimiters.get();
    }
    void setOrigDelimiters(const char * delimiters )		{
        propDelimiters.setOrig(delimiters);
    }
    const char * quote() const {
        return propQuote.get();
    }
    void setOrigQuote(const char * quote )		{
        propQuote.setOrig(quote);
    }
    int headerLines() const {
        return propHeaderLines.get();
    }
    void setOrigHeaderLines(const char * headerlines )		{
        propHeaderLines.setOrig(headerlines);
    }
    virtual void fixInputParameters(const ParametersList &parameters, const char *delim); // from Input

public:
    FixableProperty<const char *> propFilename;
    FixableProperty<const char *> propDelimiters;
    FixableProperty<const char *> propQuote;
    FixableProperty<int> propHeaderLines;
    /*>>>>>INPUTCSV_PROPERTIES*/

private:
    std::ifstream *pFile;
    std::vector<Xtring> mRowValues;
};

} // namespace

#endif


