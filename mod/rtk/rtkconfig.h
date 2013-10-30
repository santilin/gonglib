#ifndef RTKCONFIG_H
#define RTKCONFIG_H
/*<<<<<COPYLEFT*/
/** @file rtkconfig.h Config include
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
#include <config.h>

#define USE_CXX_FORMULA 0
#define HAVE_BOOSTSPIRIT 1
//#define RTK_HAVE_XBASE

#define RTK_VERSION "0.1-beta"

#define RTK_JPEG_SUPPORT 1
#define RTK_PNG_SUPPORT 1

#ifdef _GONG_DEBUG
// Increase the value of the following constants to increase the debug output
# define GONG_DEBUG_REPORT_INIT 0
# define GONG_DEBUG_INPUT 0
# define GONG_DEBUG_OUTPUT 10
# define GONG_DEBUG_PRINT 3
# define GONG_DEBUG_VALUES 0
# define GONG_DEBUG_AGGREGATES 0
# define GONG_DEBUG_FORMULA 10
# define GONG_DEBUG_LOCALE 0
# include <gongdebug.h>
#endif

#endif // RTKCONFIG_H

