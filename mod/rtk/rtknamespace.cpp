#include <gongvariant.h>
#include <gongfileutils.h>
#include <gonglist.h>
#include "rtknamespace.h"


/**
    @class RTK
    @brief The RTK class is a namespace for miscellaneous identifiers that need to be global-like.

    @author Santiago Capel Torres

    \ingroup misc
*/

namespace gong {}
namespace RTK = gong;

namespace gong {

template<> bool stringTo(const char *origvalue, bool *isvalid, bool *isnull )
{
	if( isvalid ) *isvalid = true;
    bool localnull;
    if( !isnull )
        isnull = &localnull;
    *isnull = false;
    if( origvalue && *origvalue ) {
        if ( strcasecmp( origvalue, "false" ) == 0
                || strcasecmp( origvalue, "no" ) == 0
                || strcasecmp( origvalue, "n" ) == 0
                || strcasecmp( origvalue, "off" ) == 0
                || strcmp( origvalue, "0" ) == 0 )
            return false;
        if ( strcasecmp( origvalue, "true" ) == 0
                || strcasecmp( origvalue, "yes" ) == 0
                || strcasecmp( origvalue, "on" ) == 0
                || strcmp( origvalue, "1" ) == 0 )
            return true;
		if( isvalid )
			*isvalid = false;
    }
    *isnull = true;
    return false;
}


template<> const char *stringTo(const char *origvalue, bool *isvalid, bool *isnull )
{
	if( isvalid ) *isvalid = true;
    bool localnull;
    if( !isnull )
        isnull = &localnull;
    *isnull = false;
    if( !origvalue || !*origvalue ) {
        *isnull = true;
        return "";
    } else
        return origvalue;
}

template<> Xtring stringTo(const char *origvalue, bool *isvalid, bool *isnull )
{
	if( isvalid ) *isvalid = true;
    bool localnull;
    if( !isnull )
        isnull = &localnull;
    *isnull = false;
    if( !origvalue || !*origvalue ) {
        *isnull = true;
        return Xtring::null;
    } else
        return origvalue;
}

template<> Alignment stringTo(const char *origvalue, bool *isvalid, bool *isnull )
{
	if( isvalid ) *isvalid = true;
    bool localnull;
    if( !isnull )
        isnull = &localnull;
    *isnull = false;
    if( origvalue && *origvalue ) {
        if( strncasecmp(origvalue, "alignment", 9) == 0 )
            origvalue+=9;
        else if( strncasecmp(origvalue, "align", 5) == 0 )
            origvalue+=5;
        if ( strcasecmp( origvalue, "left" ) == 0 )
            return AlignLeft;
        else if ( strcasecmp( origvalue, "right" ) == 0 )
            return AlignRight;
        else if ( strcasecmp( origvalue, "center" ) == 0 )
            return AlignHCenter;
        else if ( strcasecmp( origvalue, "justify" ) == 0 || strcasecmp( origvalue, "justified" ) == 0 )
            return AlignJustify;
        else if ( strcasecmp( origvalue, "top" ) == 0 || strcasecmp( origvalue, "up" ) == 0 )
            return AlignTop;
        else if ( strcasecmp( origvalue, "middle" ) == 0 )
            return AlignVCenter;
        else if ( strcasecmp( origvalue, "bottom" ) == 0 || strcasecmp( origvalue, "down" ) == 0 )
            return AlignBottom;
        else if ( strcasecmp( origvalue, "auto" ) == 0 )
            return AlignAuto;
		else if( isvalid )
			*isvalid = false;
    }
    *isnull = true;
    return AlignAuto;
}

template<> AggregateType stringTo(const char *origvalue, bool *isvalid, bool *isnull )
{
	if( isvalid ) *isvalid = true;
    // There is no invalid value
    if( isnull )
        *isnull = false;
    *isnull = false;
    if( origvalue && *origvalue ) {
        if( strcasecmp( origvalue, "AggNone" ) == 0 || strcasecmp( origvalue, "None" ) == 0 )
            return AggNone;
        else if ( strcasecmp( origvalue, "AggCount" ) == 0 || strcasecmp( origvalue, "Count" ) == 0 )
            return AggCount;
        else if ( strcasecmp( origvalue, "AggSum" ) == 0 || strcasecmp( origvalue, "Sum" ) == 0 )
            return AggSum;
        else if ( strcasecmp( origvalue, "AggMax" ) == 0 || strcasecmp( origvalue, "Max" ) == 0 )
            return AggMax;
        else if ( strcasecmp( origvalue, "AggMin" ) == 0 || strcasecmp( origvalue, "Min" ) == 0 )
            return AggMin;
        else if ( strcasecmp( origvalue, "AggAvg" ) == 0 || strcasecmp( origvalue, "Avg" ) == 0 || strcasecmp( origvalue, "Average" ) == 0 )
            return AggAvg;
        else if ( strcasecmp( origvalue, "AggStdDv" ) == 0 || strcasecmp( origvalue, "StdDv" ) == 0 )
            return AggStdDv;
        else if ( strcasecmp( origvalue, "AggVariance" ) == 0 || strcasecmp( origvalue, "Variance" ) == 0 )
            return AggVariance;
        else if ( strcasecmp( origvalue, "AggDistinctCount" ) == 0 || strcasecmp( origvalue, "DistinctCount" ) == 0 )
            return AggDistinctCount;
        else if ( strcasecmp( origvalue, "AggDistinctSum" ) == 0 || strcasecmp( origvalue, "DistinctSum" ) == 0 )
            return AggDistinctSum;
        else if ( strcasecmp( origvalue, "AggDistinctAvg" ) == 0 || strcasecmp( origvalue, "DistinctAvg" ) == 0 || strcasecmp( origvalue, "DistinctAverage" ) == 0 )
            return AggDistinctAvg;
        else if ( strcasecmp( origvalue, "AggDistinctStdDv" ) == 0 || strcasecmp( origvalue, "DistinctStdDv" ) == 0 )
            return AggDistinctStdDv;
        else if ( strcasecmp( origvalue, "AggDistinctVariance" ) == 0 || strcasecmp( origvalue, "DistinctVariance" ) == 0 )
            return AggDistinctVariance;
		else if( isvalid )
			*isvalid = false;
    } else {
        *isnull = true;
    }
    return AggNone;
}


template<> PaperSize stringTo(const char *origvalue, bool *isvalid, bool *isnull )
{
	if( isvalid ) *isvalid = true;
    // There is no invalid value
    if( isnull )
        *isnull = false;
    if( !origvalue || !*origvalue || strcasecmp( origvalue, "A4" ) == 0 )
        return A4;
    else if ( strcasecmp( origvalue, "B5" ) == 0 )
        return B5;
    else if ( strcasecmp( origvalue, "Letter" ) == 0 )
        return B5;
    else if ( strcasecmp( origvalue, "Legal" ) == 0 )
        return Legal;
    else if ( strcasecmp( origvalue, "Executive" ) == 0 )
        return Executive;
    else if ( strcasecmp( origvalue, "A0" ) == 0 )
        return A0;
    else if ( strcasecmp( origvalue, "A1" ) == 0 )
        return A1;
    else if ( strcasecmp( origvalue, "A2" ) == 0 )
        return A2;
    else if ( strcasecmp( origvalue, "A3" ) == 0 )
        return A3;
    else if ( strcasecmp( origvalue, "A4" ) == 0 )
        return A4;
    else if ( strcasecmp( origvalue, "A5" ) == 0 )
        return A5;
    else if ( strcasecmp( origvalue, "A6" ) == 0 )
        return A6;
    else if ( strcasecmp( origvalue, "A7" ) == 0 )
        return A7;
    else if ( strcasecmp( origvalue, "A8" ) == 0 )
        return A8;
    else if ( strcasecmp( origvalue, "A9" ) == 0 )
        return A9;
    else if ( strcasecmp( origvalue, "B0" ) == 0 )
        return B0;
    else if ( strcasecmp( origvalue, "B1" ) == 0 )
        return B1;
    else if ( strcasecmp( origvalue, "B10" ) == 0 )
        return B10;
    else if ( strcasecmp( origvalue, "B2" ) == 0 )
        return B2;
    else if ( strcasecmp( origvalue, "B3" ) == 0 )
        return B3;
    else if ( strcasecmp( origvalue, "B4" ) == 0 )
        return B4;
    else if ( strcasecmp( origvalue, "B5" ) == 0 )
        return B5;
    else if ( strcasecmp( origvalue, "B6" ) == 0 )
        return B6;
    else if ( strcasecmp( origvalue, "B7" ) == 0 )
        return B7;
    else if ( strcasecmp( origvalue, "B8" ) == 0 )
        return B8;
    else if ( strcasecmp( origvalue, "B9" ) == 0 )
        return B9;
    else if ( strcasecmp( origvalue, "C5E" ) == 0 )
        return C5E;
    else if ( strcasecmp( origvalue, "Comm10E" ) == 0 )
        return Comm10E;
    else if ( strcasecmp( origvalue, "DLE" ) == 0 )
        return DLE;
    else if ( strcasecmp( origvalue, "Folio" ) == 0 )
        return Folio;
    else if ( strcasecmp( origvalue, "Ledger" ) == 0 )
        return Ledger;
    else if ( strcasecmp( origvalue, "Tabloid" ) == 0 )
        return Tabloid;
    else if( isvalid )
		*isvalid = false;
	return Custom;
}

template<> PageOrientation stringTo(const char *origvalue, bool *isvalid, bool *isnull )
{
	if( isvalid ) *isvalid = true;
	bool localnull;
    if( !isnull )
        isnull = &localnull;
    *isnull = false;
    if( origvalue && *origvalue ) {
        if ( strcasecmp( origvalue, "portrait" ) == 0 )
            return Portrait;
        else if ( strcasecmp( origvalue, "landscape" ) == 0 )
            return Landscape;
		else if( isvalid )
			*isvalid = false;
		return DefaultOrientation;
    }
    *isnull = true;
    return DefaultOrientation;
}


template<> int stringTo(const char *origvalue, bool *isvalid, bool *isnull )
{
    if( isvalid ) *isvalid = true;
    bool localnull;
    if( !isnull )
        isnull = &localnull;
    *isnull = false;
    if( !origvalue || !*origvalue ) {
        *isnull = true;
        return 100;
    } else if ( strcasecmp( origvalue, "Light" ) == 0 )
        return 25;
    else if ( strcasecmp( origvalue, "Normal" ) == 0 || strcasecmp( origvalue, "Medium" ) == 0 )
        return 50;
    else if ( strcasecmp( origvalue, "DemiBold" ) == 0 )
        return 63;
    else if ( strcasecmp( origvalue, "Bold" ) == 0 )
        return 75;
    else if ( strcasecmp( origvalue, "Black" ) == 0 )
        return 100;
    else  {
        int fw = Xtring(origvalue).toInt(isnull);
		if( isvalid )
			*isvalid = *isnull;
        *isnull = !*isnull;
        return fw;
    }
}


/**
    Extract the measure value from the measure string
 */
template<> Measure stringTo(const char *origvalue, bool *isvalid, bool *isnull )
{
    if( isvalid ) *isvalid = true;
    bool localnull;
    if( !isnull )
        isnull = &localnull;
    *isnull = false;
    if( origvalue && *origvalue ) {
        double d = Xtring(origvalue).toDoubleLocIndep(isnull);
        // strtod(origvalue, &badpos);
		if( isvalid )
			*isvalid = *isnull;
        *isnull = !( *isnull );
        return d;
    } else {
        *isnull = true;
        return 0.0;
    }
}


template<> Adjustment stringTo(const char *origvalue, bool *isvalid, bool *isnull )
{
    if( isvalid ) *isvalid = true;
    bool localnull;
    if( !isnull )
        isnull = &localnull;
    *isnull = false;
    if( origvalue && *origvalue ) {
        if( strncasecmp(origvalue, "adjustment", 10) == 0 )
            origvalue+=10;
        else if( strncasecmp(origvalue, "adjust", 6) == 0 )
            origvalue+=6;
        if ( strcasecmp( origvalue, "none" ) == 0 )
            return AdjustNone;
        else if ( strcasecmp( origvalue, "grow" ) == 0 )
            return AdjustGrow;
        else if ( strcasecmp( origvalue, "reducefont" ) == 0 )
            return AdjustReduceFont;
        else if ( strcasecmp( origvalue, "shrink" ) == 0 )
            return AdjustShrink;
        else if ( strcasecmp( origvalue, "trim" ) == 0 )
            return AdjustTrim;
		else if( isvalid )
			*isvalid = false;
    }
    *isnull = true;
    return AdjustTrim;
}

/*!
    Sets the RGB value to \a name, which may be in one of these
    formats:
    \list
    \i #RGB (each of R, G and B is a single hex digit)
    \i #RRGGBB
    \i #RRRGGGBBB
    \i #RRRRGGGGBBBB
    \i A name from the X color database (rgb.txt) (e.g.
    "steelblue" or "gainsboro"). These color names also work
    under Windows.
    \endlist

    The color is invalid if \a name cannot be parsed.
 */

template<> Color stringTo(const char *origvalue, bool *isvalid, bool *isnull )
{
    if( isvalid ) *isvalid = true;
    bool localnull;
    if( !isnull )
        isnull = &localnull;
    *isnull = false;
    if( origvalue && *origvalue ) {
        Color c(origvalue);
        if( !c.isValid() ) {
			if( isvalid )
				*isvalid = false;
            *isnull = true;
            return Color::Black;
        } else
            return origvalue;
    }
    *isnull = true;
    return Color();
}


template<> BorderStyle stringTo(const char *origvalue, bool *isvalid, bool *isnull )
{
    if( isvalid ) *isvalid = true;
    bool localnull;
    if( !isnull )
        isnull = &localnull;
    *isnull = false;
    if( origvalue && *origvalue ) {
        if( strncasecmp(origvalue, "borderstyle", 11) == 0 )
            origvalue+=11;
        else if( strncasecmp(origvalue, "border", 6) == 0 )
            origvalue+=6;
        if ( strcasecmp( origvalue, "None" ) == 0 )
            return BorderNone;
        else if ( strcasecmp( origvalue, "Solid" ) == 0 )
            return BorderSolid;
        else if ( strcasecmp( origvalue, "Dash" ) == 0 || strcasecmp( origvalue, "Dashed" ) == 0)
            return BorderDash;
        else if ( strcasecmp( origvalue, "Dot" ) == 0 || strcasecmp( origvalue, "Dotted" ) == 0)
            return BorderDot;
        else if ( strcasecmp( origvalue, "DashDot" ) == 0 )
            return BorderDashDot;
        else if ( strcasecmp( origvalue, "DashDotDot" ) == 0 )
            return BorderDashDotDot;
        else if ( strcasecmp( origvalue, "Raised3D" ) == 0 )
            return BorderRaised3D;
        else if ( strcasecmp( origvalue, "Lowered3D" ) == 0 )
            return BorderLowered3D;
		else if( isvalid )
			*isvalid = false;
    }
    *isnull = true;
    return BorderNone;

}

/**
    If the fontsize is in the form +2 or -4, it increases or decreases the fontsize
    sdefault is the current fontsize of the object
 */
int stringToFontSize(const char *origvalue, bool *isvalid, bool *isnull )
{
    if( isvalid ) *isvalid = true;
    bool localnull;
    if( !isnull )
        isnull = &localnull;
    *isnull = false;
    Xtring realsfs(origvalue);
    realsfs.remove_white_space();
    if( !realsfs.size() ) {
        *isnull = true;
        return 10;
    } else {
        int fsinc = realsfs.toInt(isnull);
        *isnull = !*isnull;
        return fsinc;
    }
}

/**
    Extract the units from the measure string
 */

template<> UnitsType stringTo(const char *origvalue, bool *isvalid, bool *isnull )
{
    if( isvalid ) *isvalid = true;
    bool localnull;
    if( !isnull )
        isnull = &localnull;
    *isnull = false;
    if( origvalue && *origvalue ) {
        Xtring units;
        for( uint i=0; i<strlen(origvalue); i++ ) {
            if( !isdigit(origvalue[i]) && strchr("., +-e", origvalue[i]) == 0 ) {
                units = Xtring(origvalue+i);
                units.remove_white_space();
                units = units.substr(0,2);
                units = units.lower();
                if( units == "mm" )
                    return mm;
                else if( units=="cm" )
                    return cm;
                else if( units=="in" )
                    return inches;
                else if( units=="px" || units=="pi" || units=="do" )
                    return dots;
                else if( units=="ch" )
                    return chars;
                else if( units=="p1" || units=="%" )
                    return p100;
				else if( isvalid )
					*isvalid = false;
				return dots;
            }
        }
    }
    *isnull = true;
    return chars;
}


template<> ImageStyle stringTo(const char *origvalue, bool *isvalid, bool *isnull )
{
    if( isvalid ) *isvalid = true;
    bool localnull;
    if( !isnull )
        isnull = &localnull;
    *isnull = false;
    if( origvalue && *origvalue ) {
        if ( strncasecmp( origvalue, "center", 6 ) == 0 )
            return ImageCenter;
        else if ( strncasecmp( origvalue, "tile", 4 ) == 0 )
            return ImageTile;
        else if ( strncasecmp( origvalue, "expand", 6 ) == 0 )
            return ImageExpand;
		else if( isvalid )
			*isvalid = false;
		return ImageNone;
    }
    *isnull = true;
    return ImageNone;
}



template<> Variant::Type stringTo(const char *origvalue, bool *isvalid, bool *isnull )
{
    if( isvalid ) *isvalid = true;
    bool localnull;
    if( !isnull )
        isnull = &localnull;
    *isnull = false;
    if( origvalue && *origvalue ) {
        Variant::Type t = Variant::nameToType(origvalue);
        if( t == Variant::tInvalid ) {
			if( isvalid )
				*isvalid = false;
			t = Variant::tString;
		}
        return t;
    }
    *isnull = true;
    return Variant::tString;
}


Xtring strReplace(const char *constbuf,
                  const Xtring &search, const Xtring &repl)
{
    Xtring buffer = constbuf;
    int searchlen = search.length();
    if( searchlen == 0 )
        return buffer;
    int repllen = repl.length();
    Xtring::size_type match = buffer.find(search);
    while( match != Xtring::npos ) {
        buffer.erase(match, searchlen);
        buffer.insert(match, repl);
        match = buffer.find(search, match + repllen);
    }
    return buffer;
}


Measure fixOneMeasure(UnitsType objunit, Measure measure,
                      UnitsType repunit, Measure repsize,  Measure outsize )
{
    if( outsize == 0 )
        return 0.0;
    if( repsize == 0 )
        repsize = outsize;
    if ( objunit == p100 ) {
        // Adjust percent to report size
        objunit = repunit;
        measure *= repsize / 100;
    } else if ( objunit == defaultunit ) {
        objunit = repunit;
    }
    return measure * ( outsize / repsize );
}

UnitsType extractUnits( const char *smeasure, UnitsType defaultunits, bool *isvalid )
{
    bool isnull;
    UnitsType ret = stringTo<UnitsType>(smeasure, isvalid, &isnull);
    if( isnull )
        return defaultunits;
    else
        return ret;
}

bool strequal(const char *str1, const char *str2)
{
    if( !str1 && !str2 )
        return true;
    else if( (!str1 && !*str2) || (!str2 && !*str1) )
        return true;
    else if( (!str1 || !str2) )
        return false;
    else return !strcmp(str1, str2);
}

bool strcaseequal(const char *str1, const char *str2)
{
    if( !str1 && !str2 )
        return true;
    else if( (!str1 && !*str2) || (!str2 && !*str1) )
        return true;
    else if( (!str1 || !str2) )
        return false;
    else return !strcasecmp(str1, str2);
}

bool strequal(const Xtring &s1, const char *str2)
{
    return s1 == str2;
}

bool strcaseequal(const Xtring &s2, const char *str2)
{
    return strcaseequal(s2.c_str(), str2);
}


} // namespace RTK

