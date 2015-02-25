#ifndef _GONG_CSVUTILS_H
#define _GONG_CSVUTILS_H

/** @file gongcsvutils.h Class for reading/writing csv files
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include "gongxtring.h"

namespace gong {

class CSVUtils
{
public:
    CSVUtils();
    ~CSVUtils();
    static bool readCSVFile( const Xtring &fname, XtringList &lines,
                             Xtring &headers, bool hasheaders = true, char quotes = '\0');
    static int tokenize(XtringList &values, const Xtring &line,
                        char quotes, char delim, const Xtring &encoding = "UTF-8");
    static Xtring dupQuotes( const Xtring &source, char quote );
};

}

#endif
