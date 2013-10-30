#include "gongdbfieldstyle.h"

namespace gong {

dbFieldStyle::Alignment dbFieldStyle::fromString( const Xtring &str)
{
    const char *origvalue = str.c_str();
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
    }
    return AlignAuto;
}

} // namespace gong

