#ifndef RTKNAMESPACE_H
#define RTKNAMESPACE_H

/*<<<<<COPYLEFT*/
/** @file rtknamespace.h RTK namespace
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
#ifdef _GONG_DEBUG
#include <gongdebug.h>
#endif

#include <gongxtring.h>
#include "rtkcolor.h"

namespace gong {}
namespace RTK = gong;

namespace gong {

typedef double Measure;
typedef int FontWeight;

enum AggregateType {
    AggNone,
    AggCount,
    AggDistinctCount,
    AggSum,
    AggMax,
    AggMin,
    AggDistinctSum,
    AggAvg,
    AggDistinctAvg,
    AggStdDv,
    AggDistinctStdDv,
    AggDistinctVariance,
    AggVariance
};

enum BorderStyle {
    BorderNone = 0,
    BorderSolid,
    BorderDash,
    BorderDot,
    BorderDashDot,
    BorderDashDotDot,
    BorderRaised3D,
    BorderLowered3D
};

enum Alignment {
    AlignAuto, AlignLeft, AlignRight, AlignHCenter,
    AlignCenter = AlignHCenter, AlignJustify,
    AlignTop, AlignBottom, AlignVCenter, AlignMiddle = AlignVCenter
};


enum Adjustment {
    AdjustTrim =       0,
    AdjustGrow =       1 << 1,
    AdjustNone =       1 << 2,
    AdjustReduceFont = 1 << 3,
    AdjustShrink     = 1 << 4
};


enum ImageStyle {
    ImageNone,
    ImageCenter,
    ImageExpand,
    ImageTile
};

extern ImageStyle *null_imagestyle_ptr;

const Measure dotsperinch = 81;
const Measure charsperinch = 10;
const Measure cmperinch = 2.545;
const Measure mmperinch = cmperinch*10;
const Measure dotspercm = dotsperinch/cmperinch;
const Measure dotsperchar = dotsperinch/charsperinch;

enum UnitsType {
    chars=0,
    dots=1,
    inches=2,
    cm=3,
    mm=4,
    p100=5,
    defaultunit /*always the last one*/
};

enum PageOrientation { DefaultOrientation, Portrait, Landscape };
enum PaperSize {
    A4, B5, Letter, Legal, Executive,
    A0, A1, A2, A3, A5, A6, A7, A8, A9, B0, B1,
    B10, B2, B3, B4, B6, B7, B8, B9, C5E, Comm10E,
    DLE, Folio, Ledger, Tabloid, Custom, NPageSize = Custom
};

UnitsType extractUnits(const char *smeasure, UnitsType defaultunits, bool *isvalid = 0 );
Xtring strReplace(const char *constbuf, const Xtring &search, const Xtring &repl);
Measure fixOneMeasure(UnitsType objunit, Measure measure,
                      UnitsType repunit, Measure repsize,  Measure outsize );
bool strequal(const char *str1, const char *str2);
bool strcaseequal(const char *str1, const char *str2);
bool strequal(const Xtring &s1, const char *str2);
bool strcaseequal(const Xtring &s2, const char *str2);
template<typename _ValueT>
_ValueT stringTo(const char *origvalue, bool *isvalid = 0, bool *isnull = 0 );
} // namespace


#endif // RTKNAMESPACE_H
