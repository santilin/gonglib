#ifndef RTKFORMULA_H
#define RTKFORMULA_H

/*<<<<<COPYLEFT*/
/** @file rtkformula.h Formula for the rtk scripting language
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

#include "rtkconfig.h"

#ifdef USE_CXX_FORMULA

#include <dlfcn.h>
#include <gongxtring.h>
#include <gongvariant.h>

namespace gong {


enum FormulaToken {
    FT_END,
    FT_ERROR,
    FT_NUMBER,
    FT_STRING,
    FT_NAME,
    FT_OPERATOR,
    FT_DOT,
    FT_SEMICOLON, // ; ,
    FT_BRACKET // { } ( ) [ ]
};

FormulaToken getNextToken(const Xtring &code, Variant *value, int *pos);

} // namespace


#endif
#ifdef HAVE_BOOSTSPIRIT
#include "rtkcontext.h"
#include "rtkstdlib.h"
#endif

#endif // RTKFORMULA_H
