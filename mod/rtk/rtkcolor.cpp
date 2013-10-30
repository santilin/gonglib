#include <cctype>
#include <cstdio>
#include <cstring>

#include "rtkcolor.h"

namespace gong {}
namespace RTK = gong;

namespace gong {

const uint Color::White = 0x00ffffff;
const uint Color::Black = 0x00000000;
const uint Color::Red =   0x00ff0000;
const uint Color::Green = 0x0000ff00;
const uint Color::Blue  = 0x000000ff;
const uint Color::Cyan  = 0x0000ffff;
const uint Color::Magenta = 0x00ff00ff;
const uint Color::Yellow = 0x00ffff00;
const uint Color::Gray = 0x00f0f0f0;
const uint Color::Invalid = 0xffffffff;



// Parse a color string and return true if it was a valid color, otherwise
// false.  Accetable inputs are HTML/X11 style #xxxxxx colors (where x is
// hexadecimal digit) or one of a list of named colors.
Color::Color(const char* s)
    : mCode(Invalid)
{
    if (s[0] == '#')
    {
        s++;

        int length = strlen(s);

        // Verify that the string contains only hex digits
        for (int i = 0; i < length; i++)
        {
            if (!isxdigit(s[i]))
                return;
        }

        uint n;
        sscanf(s, "%x", &n);
        if (length == 3)
        {
            setRed( (n >> 8) * 17 );
            setGreen( ((n & 0x0f0) >> 4) * 17);
            setBlue( (n & 0x00f) * 17);
        }
        else if (length == 6)
        {
            setRed(n >> 16);
            setGreen( (n & 0x00ff00) >> 8 );
            setBlue( (n & 0x0000ff) );
        }
    }
    else
    {
        if (!strcasecmp(s, "red"))
            mCode = Red;
        else if (!strcasecmp(s, "green"))
            mCode = Green;
        else if (!strcasecmp(s, "blue"))
            mCode = Blue;
        else if (!strcasecmp(s, "cyan")) {
            mCode = Cyan;
        } else if (!strcasecmp(s, "magenta")) {
            mCode = Magenta;
        } else if (!strcasecmp(s, "yellow")) {
            mCode = Yellow;
        } else if (!strcasecmp(s, "white")) {
            mCode = White;
        } else if (!strcasecmp(s, "gray") || !strcasecmp(s, "grey")) {
            mCode = Gray;
        } else if (!strcasecmp(s, "black")) {
            mCode = Black;
        }
    }
}

const char *Color::name() const
{
    if( mCode == Black )
        return "black";
    else if( mCode == White )
        return "white";
    else if( mCode == Green )
        return "green";
    else if( mCode == Blue )
        return "blue";
    else if( mCode == Cyan )
        return "cyan";
    else if( mCode == Magenta )
        return "magenta";
    else if( mCode == Yellow )
        return "yellow";
    else if( mCode == Blue )
        return "blue";
    else if( mCode == Red )
        return "red";
    else {
        static char buffer[25];
        sprintf(buffer,"%d,%d,%d", red(), green(), blue());
        return buffer;
    }
}

} // namespace RTK
