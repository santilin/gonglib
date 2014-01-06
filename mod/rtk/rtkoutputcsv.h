#ifndef RTKOUTPUTCVS_H
#define RTKOUTPUTCVS_H

/*<<<<<COPYLEFT*/
/** @file rtkoutputcsv.h RTK standard csv output object
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

#include <fstream>

#include "rtkoutput.h"

namespace gong {}
namespace RTK = gong;

namespace gong {

class OutputCsv : public Output
{
public:
    OutputCsv(class Report &r, const char *filename, const char *delimiters = ",",
              const char *quote = "\"", bool columnnames = false)
        :Output(r)
        , propFileName(filename), propDelimiters(delimiters),
        propQuote(quote), propColumnNames(columnnames)
    {};

    ~OutputCsv() {};
    int startReport();
    int endReport();
    Measure startSection(const Section &section);
    Measure endSection(const Section &section);
    Measure printObject(const Object &object);

    /*<<<<<OUTPUTCSV_PROPERTIES*/
public:
	const char * fileName() const { return propFileName.get(); }
	const char * delimiters() const { return propDelimiters.get(); }
	const char * quote() const { return propQuote.get(); }
	bool columnNames() const { return propColumnNames.get(); }

public:
	Property<const char *> propFileName;
	Property<const char *> propDelimiters;
	Property<const char *> propQuote;
	Property<bool> propColumnNames;
/*>>>>>OUTPUTCSV_PROPERTIES*/

private:
    std::ofstream *pFile;
    Xtring mLine;
    bool mStdCout;
};

} // namespace

#endif // RTKOUTPUTTEXT_H
