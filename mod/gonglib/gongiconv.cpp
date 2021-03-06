#include <cerrno>
#include <stdexcept>
#include "gongdebug.h"
#include "gongiconv.h"

namespace gong {

/**
       @class IConv
       @brief Charset conversions
*/

/*
	charmap aliases from glibc:
	cd localedata/charmaps
	grep "%.*alias" *
	In kdevelop, replace: ^(.*):% alias (.*)$   by  {"\2", "\1"},
*/

static const char *charmap_aliases[][2] = {

    {"UTF8", "UTF-8"},
    {"ISO-10646/UTF-8", "UTF-8"},
    {"ISO_8859-1", "ISO-8859-1"},
    {"LATIN1", "ISO-8859-1"},
    {"ISO-IR-100", "ISO-8859-1"},
    {"ISO_8859-1:1987", "ISO-8859-1"},
    {"L1", "ISO-8859-1"},
    {"IBM819", "ISO-8859-1"},
    {"CP819", "ISO-8859-1"},
    {"ISO-IR-99", "ANSI_X3.110-1983"},
    {"CSA_T500-1983", "ANSI_X3.110-1983"},
    {"NAPLPS", "ANSI_X3.110-1983"},
    {"ISO-IR-6", "ANSI_X3.4-1968"},
    {"ANSI_X3.4-1986", "ANSI_X3.4-1968"},
    {"ISO_646.IRV:1991", "ANSI_X3.4-1968"},
    {"ASCII", "ANSI_X3.4-1968"},
    {"ISO646-US", "ANSI_X3.4-1968"},
    {"US-ASCII", "ANSI_X3.4-1968"},
    {"US", "ANSI_X3.4-1968"},
    {"IBM367", "ANSI_X3.4-1968"},
    {"CP367", "ANSI_X3.4-1968"},
    {"ISO_9036", "ASMO_449"},
    {"ARABIC7", "ASMO_449"},
    {"ISO-IR-89", "ASMO_449"},
    {"BIG5-CP950", "BIG5"},
    {"BIG5HKSCS", "BIG5-HKSCS"},
    {"ISO-IR-4", "BS_4730"},
    {"ISO646-GB", "BS_4730"},
    {"GB", "BS_4730"},
    {"UK", "BS_4730"},
    {"ISO-IR-47", "BS_VIEWDATA"},
    {"RUSCII", "CP1125"},
    {"IBM848", "CP1125"},
    {"MS-EE", "CP1250"},
    {"MS-CYRL", "CP1251"},
    {"MS-ANSI", "CP1252"},
    {"MS-GREEK", "CP1253"},
    {"MS-TURK", "CP1254"},
    {"MS-HEBR", "CP1255"},
    {"MS-ARAB", "CP1256"},
    {"WINBALTRIM", "CP1257"},
    {"ISO-IR-121", "CSA_Z243.4-1985-1"},
    {"ISO646-CA", "CSA_Z243.4-1985-1"},
    {"CSA7-1", "CSA_Z243.4-1985-1"},
    {"CA", "CSA_Z243.4-1985-1"},
    {"ISO-IR-122", "CSA_Z243.4-1985-2"},
    {"ISO646-CA2", "CSA_Z243.4-1985-2"},
    {"CSA7-2", "CSA_Z243.4-1985-2"},
    {"ISO-IR-123", "CSA_Z243.4-1985-GR"},
    {"ISO-IR-139", "CSN_369103"},
    {"CWI-2", "CWI"},
    {"CP-HU", "CWI"},
    {"DEC", "DEC-MCS"},
    {"ISO-IR-21", "DIN_66003"},
    {"DE", "DIN_66003"},
    {"ISO646-DE", "DIN_66003"},
    {"DS2089", "DS_2089"},
    {"ISO646-DK", "DS_2089"},
    {"DK", "DS_2089"},
    {"FRISS", "EBCDIC-IS-FRISS"},
    {"ISO-IR-111", "ECMA-CYRILLIC"},
    {"ISO-IR-17", "ES"},
    {"ISO646-ES", "ES"},
    {"ISO-IR-85", "ES2"},
    {"ISO646-ES2", "ES2"},
    {"ISO-IR-57", "GB_1988-80"},
    {"CN", "GB_1988-80"},
    {"ISO646-CN", "GB_1988-80"},
    {"CP936", "GBK"},
    {"MS936", "GBK"},
    {"WINDOWS-936", "GBK"},
    {"ST_SEV_358-88", "GOST_19768-74"},
    {"ISO-IR-153", "GOST_19768-74"},
    {"ISO-IR-88", "GREEK7"},
    {"ISO-IR-18", "GREEK7-OLD"},
    {"ISO-IR-150", "GREEK-CCITT"},
    {"ROMAN8", "HP-ROMAN8"},
    {"R8", "HP-ROMAN8"},
    {"CP037", "IBM037"},
    {"EBCDIC-CP-US", "IBM037"},
    {"EBCDIC-CP-CA", "IBM037"},
    {"EBCDIC-CP-WT", "IBM037"},
    {"EBCDIC-CP-NL", "IBM037"},
    {"EBCDIC-INT", "IBM038"},
    {"CP038", "IBM038"},
    {"CP1004", "IBM1004"},
    {"OS2LATIN1", "IBM1004"},
    {"CP1026", "IBM1026"},
    {"1026", "IBM1026"},
    {"IBM-1047", "IBM1047"},
    {"CP1047", "IBM1047"},
    {"1047", "IBM1047"},
    {"CP1124", "IBM1124"},
    {"CP1129", "IBM1129"},
    {"CP1132", "IBM1132"},
    {"CP1133", "IBM1133"},
    {"CP1160", "IBM1160"},
    {"CP1161", "IBM1161"},
    {"CP1133", "IBM1162"},
    {"CP1163", "IBM1163"},
    {"CP1164", "IBM1164"},
    {"EBCDIC-INT1", "IBM256"},
    {"CP273", "IBM273"},
    {"EBCDIC-BE", "IBM274"},
    {"CP274", "IBM274"},
    {"EBCDIC-BR", "IBM275"},
    {"CP275", "IBM275"},
    {"EBCDIC-CP-DK", "IBM277"},
    {"EBCDIC-CP-NO", "IBM277"},
    {"CP278", "IBM278"},
    {"EBCDIC-CP-FI", "IBM278"},
    {"EBCDIC-CP-SE", "IBM278"},
    {"CP280", "IBM280"},
    {"EBCDIC-CP-IT", "IBM280"},
    {"EBCDIC-JP-E", "IBM281"},
    {"CP281", "IBM281"},
    {"CP284", "IBM284"},
    {"EBCDIC-CP-ES", "IBM284"},
    {"CP285", "IBM285"},
    {"EBCDIC-CP-GB", "IBM285"},
    {"CP290", "IBM290"},
    {"EBCDIC-JP-KANA", "IBM290"},
    {"CP297", "IBM297"},
    {"EBCDIC-CP-FR", "IBM297"},
    {"CP420", "IBM420"},
    {"EBCDIC-CP-AR1", "IBM420"},
    {"CP423", "IBM423"},
    {"EBCDIC-CP-GR", "IBM423"},
    {"CP424", "IBM424"},
    {"EBCDIC-CP-HE", "IBM424"},
    {"CP437", "IBM437"},
    {"437", "IBM437"},
    {"CP500", "IBM500"},
    {"500", "IBM500"},
    {"500V1", "IBM500"},
    {"EBCDIC-CP-BE", "IBM500"},
    {"EBCDIC-CP-CH", "IBM500"},
    {"CP850", "IBM850"},
    {"850", "IBM850"},
    {"CP851", "IBM851"},
    {"851", "IBM851"},
    {"CP852", "IBM852"},
    {"852", "IBM852"},
    {"CP855", "IBM855"},
    {"855", "IBM855"},
    {"CP856", "IBM856"},
    {"856", "IBM856"},
    {"CP857", "IBM857"},
    {"857", "IBM857"},
    {"CP860", "IBM860"},
    {"860", "IBM860"},
    {"CP861", "IBM861"},
    {"861", "IBM861"},
    {"CP-IS", "IBM861"},
    {"CP862", "IBM862"},
    {"862", "IBM862"},
    {"CP863", "IBM863"},
    {"863", "IBM863"},
    {"CP864", "IBM864"},
    {"CP865", "IBM865"},
    {"865", "IBM865"},
    {"CP866", "IBM866"},
    {"866", "IBM866"},
    {"CP866NAV", "IBM866NAV"},
    {"866NAV", "IBM866NAV"},
    {"CP868", "IBM868"},
    {"CP-AR", "IBM868"},
    {"CP869", "IBM869"},
    {"869", "IBM869"},
    {"CP-GR", "IBM869"},
    {"CP870", "IBM870"},
    {"EBCDIC-CP-ROECE", "IBM870"},
    {"EBCDIC-CP-YU", "IBM870"},
    {"CP871", "IBM871"},
    {"EBCDIC-CP-IS", "IBM871"},
    {"CP874", "IBM874"},
    {"WINDOWS-874", "IBM874"},
    {"CP875", "IBM875"},
    {"EBCDIC-GREEK", "IBM875"},
    {"CP880", "IBM880"},
    {"EBCDIC-CYRILLIC", "IBM880"},
    {"CP891", "IBM891"},
    {"CP903", "IBM903"},
    {"CP904", "IBM904"},
    {"904", "IBM904"},
    {"CP905", "IBM905"},
    {"EBCDIC-CP-TR", "IBM905"},
    {"CP918", "IBM918"},
    {"EBCDIC-CP-AR2", "IBM918"},
    {"CP922", "IBM922"},
    {"ISO-IR-143", "IEC_P27-1"},
    {"ISO-IR-49", "INIS"},
    {"ISO-IR-50", "INIS-8"},
    {"ISO-IR-51", "INIS-CYRILLIC"},
    {"ISO-IR-170", "INVARIANT"},
    {"ISO-IR-155", "ISO_10367-BOX"},
    {"ISO/TR_11548-1", "ISO_11548-1"},
    {"ISO-IR-98", "ISO_2033-1983"},
    {"E13B", "ISO_2033-1983"},
    {"ISO-IR-37", "ISO_5427"},
    {"KOI-7", "ISO_5427"},
    {"ISO-IR-54", "ISO_5427-EXT"},
    {"ISO_5427:1981", "ISO_5427-EXT"},
    {"ISO-IR-55", "ISO_5428"},
    {"ISO_5428:1980", "ISO_5428"},
    {"REF", "ISO_646.BASIC"},
    {"ISO_646.BASIC:1983", "ISO_646.BASIC"},
    {"ISO-IR-2", "ISO_646.IRV"},
    {"ISO_646.IRV:1983", "ISO_646.IRV"},
    {"IRV", "ISO_646.IRV"},
    {"ISO-IR-156", "ISO_6937"},
    {"ISO_6937:1992", "ISO_6937"},
    {"ISO6937", "ISO_6937"},
    {"ISO-IR-152", "ISO_6937-2-25"},
    {"ISO-IR-142", "ISO_6937-2-ADD"},
    {"ISO-IR-157", "ISO-8859-10"},
    {"ISO_8859-10:1992", "ISO-8859-10"},
    {"ISO_8859-10", "ISO-8859-10"},
    {"L6", "ISO-8859-10"},
    {"LATIN6", "ISO-8859-10"},
    {"ISO-IR-179", "ISO-8859-13"},
    {"LATIN7", "ISO-8859-13"},
    {"L7", "ISO-8859-13"},
    {"LATIN8", "ISO-8859-14"},
    {"L8", "ISO-8859-14"},
    {"ISO_8859-15", "ISO-8859-15"},
    {"LATIN-9", "ISO-8859-15"},
    {"ISO-IR-226", "ISO-8859-16"},
    {"ISO_8859-16:2001", "ISO-8859-16"},
    {"ISO_8859-16", "ISO-8859-16"},
    {"LATIN10", "ISO-8859-16"},
    {"L10", "ISO-8859-16"},
    {"ISO-IR-101", "ISO-8859-2"},
    {"ISO_8859-2:1987", "ISO-8859-2"},
    {"ISO_8859-2", "ISO-8859-2"},
    {"LATIN2", "ISO-8859-2"},
    {"L2", "ISO-8859-2"},
    {"ISO-IR-109", "ISO-8859-3"},
    {"ISO_8859-3:1988", "ISO-8859-3"},
    {"ISO_8859-3", "ISO-8859-3"},
    {"LATIN3", "ISO-8859-3"},
    {"L3", "ISO-8859-3"},
    {"ISO-IR-110", "ISO-8859-4"},
    {"ISO_8859-4:1988", "ISO-8859-4"},
    {"ISO_8859-4", "ISO-8859-4"},
    {"LATIN4", "ISO-8859-4"},
    {"L4", "ISO-8859-4"},
    {"ISO-IR-144", "ISO-8859-5"},
    {"ISO_8859-5:1988", "ISO-8859-5"},
    {"ISO_8859-5", "ISO-8859-5"},
    {"CYRILLIC", "ISO-8859-5"},
    {"ISO-IR-127", "ISO-8859-6"},
    {"ISO_8859-6:1987", "ISO-8859-6"},
    {"ISO_8859-6", "ISO-8859-6"},
    {"ECMA-114", "ISO-8859-6"},
    {"ASMO-708", "ISO-8859-6"},
    {"ARABIC", "ISO-8859-6"},
    {"ISO-IR-126", "ISO-8859-7"},
    {"ISO_8859-7:2003", "ISO-8859-7"},
    {"ISO_8859-7:1987", "ISO-8859-7"},
    {"ISO_8859-7", "ISO-8859-7"},
    {"ELOT_928", "ISO-8859-7"},
    {"ECMA-118", "ISO-8859-7"},
    {"GREEK", "ISO-8859-7"},
    {"GREEK8", "ISO-8859-7"},
    {"ISO-IR-138", "ISO-8859-8"},
    {"ISO_8859-8:1988", "ISO-8859-8"},
    {"ISO_8859-8", "ISO-8859-8"},
    {"HEBREW", "ISO-8859-8"},
    {"ISO-IR-148", "ISO-8859-9"},
    {"ISO_8859-9:1989", "ISO-8859-9"},
    {"ISO_8859-9", "ISO-8859-9"},
    {"LATIN5", "ISO-8859-9"},
    {"L5", "ISO-8859-9"},
    {"ISO-IR-154", "ISO_8859-SUPP"},
    {"LATIN1-2-5", "ISO_8859-SUPP"},
    {"ISO_6937-2:1983", "ISO-IR-90"},
    {"ISO-IR-15", "IT"},
    {"ISO646-IT", "IT"},
    {"JIS_C6220-1969", "JIS_C6220-1969-JP"},
    {"ISO-IR-13", "JIS_C6220-1969-JP"},
    {"KATAKANA", "JIS_C6220-1969-JP"},
    {"X0201-7", "JIS_C6220-1969-JP"},
    {"ISO-IR-14", "JIS_C6220-1969-RO"},
    {"JP", "JIS_C6220-1969-RO"},
    {"ISO646-JP", "JIS_C6220-1969-RO"},
    {"ISO-IR-91", "JIS_C6229-1984-A"},
    {"JP-OCR-A", "JIS_C6229-1984-A"},
    {"ISO-IR-92", "JIS_C6229-1984-B"},
    {"ISO646-JP-OCR-B", "JIS_C6229-1984-B"},
    {"JP-OCR-B", "JIS_C6229-1984-B"},
    {"ISO-IR-93", "JIS_C6229-1984-B-ADD"},
    {"JP-OCR-B-ADD", "JIS_C6229-1984-B-ADD"},
    {"ISO-IR-94", "JIS_C6229-1984-HAND"},
    {"JP-OCR-HAND", "JIS_C6229-1984-HAND"},
    {"ISO-IR-95", "JIS_C6229-1984-HAND-ADD"},
    {"JP-OCR-HAND-ADD", "JIS_C6229-1984-HAND-ADD"},
    {"ISO-IR-96", "JIS_C6229-1984-KANA"},
    {"X0201", "JIS_X0201"},
    {"ISO-IR-141", "JUS_I.B1.002"},
    {"ISO646-YU", "JUS_I.B1.002"},
    {"JS", "JUS_I.B1.002"},
    {"YU", "JUS_I.B1.002"},
    {"MACEDONIAN", "JUS_I.B1.003-MAC"},
    {"ISO-IR-147", "JUS_I.B1.003-MAC"},
    {"ISO-IR-146", "JUS_I.B1.003-SERB"},
    {"SERBIAN", "JUS_I.B1.003-SERB"},
    {"ISO646-KR", "KSC5636"},
    {"ISO-IR-19", "LATIN-GREEK"},
    {"ISO-IR-27", "LATIN-GREEK-1"},
    {"CP10007", "MAC-CYRILLIC"},
    {"MAC", "MACINTOSH"},
    {"ISO-IR-86", "MSZ_7795.3"},
    {"ISO646-HU", "MSZ_7795.3"},
    {"HU", "MSZ_7795.3"},
    {"ISO-IR-9-1", "NATS-DANO"},
    {"ISO-IR-9-2", "NATS-DANO-ADD"},
    {"ISO-IR-8-1", "NATS-SEFI"},
    {"ISO-IR-8-2", "NATS-SEFI-ADD"},
    {"CUBA", "NC_NC00-10"},
    {"NC_NC00-10:81", "NC_NC00-10"},
    {"ISO-IR-151", "NC_NC00-10"},
    {"ISO646-CU", "NC_NC00-10"},
    {"NEXT", "NEXTSTEP"},
    {"ISO-IR-69", "NF_Z_62-010"},
    {"ISO646-FR", "NF_Z_62-010"},
    {"FR", "NF_Z_62-010"},
    {"ISO-IR-25", "NF_Z_62-010_1973"},
    {"ISO646-FR1", "NF_Z_62-010_1973"},
    {"ISO-IR-60", "NS_4551-1"},
    {"ISO646-NO", "NS_4551-1"},
    {"NO", "NS_4551-1"},
    {"ISO646-NO2", "NS_4551-2"},
    {"ISO-IR-61", "NS_4551-2"},
    {"NO2", "NS_4551-2"},
    {"ISO-IR-16", "PT"},
    {"ISO646-PT", "PT"},
    {"Cyrillic-Asian", "PT154"},
    {"ISO-IR-84", "PT2"},
    {"ISO646-PT2", "PT2"},
    {"STRK1048-2002", "RK1048"},
    {"LATIN-LAP", "SAMI"},
    {"LAP", "SAMI"},
    {"ISO-IR-158", "SAMI"},
    {"WS2", "SAMI-WS2"},
    {"WINDOWS-SAMI2", "SAMI-WS2"},
    {"ISO-IR-10", "SEN_850200_B"},
    {"FI", "SEN_850200_B"},
    {"ISO646-FI", "SEN_850200_B"},
    {"ISO646-SE", "SEN_850200_B"},
    {"SE", "SEN_850200_B"},
    {"SS636127", "SEN_850200_B"},
    {"ISO-IR-11", "SEN_850200_C"},
    {"ISO646-SE2", "SEN_850200_C"},
    {"SE2", "SEN_850200_C"},
    {"SJIS", "SHIFT_JIS"},
    {"ISO-IR-128", "T.101-G2"},
    {"ISO-IR-102", "T.61-7BIT"},
    {"T.61", "T.61-8BIT"},
    {"ISO-IR-103", "T.61-8BIT"},
    {"TCVN", "TCVN5712-1"},
    {"TCVN-5712", "TCVN5712-1"},
    {"TCVN5712-1:1993", "TCVN5712-1"},
    {"TIS620", "TIS-620"},
    {"TIS620-0", "TIS-620"},
    {"TIS620.2529-1", "TIS-620"},
    {"TIS620.2533-0", "TIS-620"},
    {"ISO-IR-166", "TIS-620"},
    {"ISO-IR-70", "VIDEOTEX-SUPPL"},
    {"CP932", "WINDOWS-31J"}

};


IConv::IConv(const Xtring &to, const Xtring& from)
    : mToCode( canonicalCodeName(to.c_str()) ), mFromCode( canonicalCodeName(from.c_str()) )
{
    cd = iconv_open(mToCode.c_str(), mFromCode.c_str());
    if (cd == reinterpret_cast<iconv_t>(-1) )
    {
        if (errno == EINVAL)
        {
            Xtring msg = "conversion not supported; from=\"";
            msg += mFromCode;
            msg += "\" to \"";
            msg += mToCode;
            throw std::runtime_error(msg);
        }
    }
}


IConv::~IConv()
{
    iconv_close( cd );
}


Xtring IConv::convert(const Xtring & _str)
{
    Xtring str = _str;
    if( mToCode == "ISO-8859-1" ) // This encoding haven't got the euro sign
        str = str.replace( "\342\202\254", "e" ).replace("\xa4", "e");
    size_t inbytesleft = str.size(), outbytesleft = inbytesleft * 3;
    char *dest = new char[outbytesleft+1];
    char *savedest = dest;
    char *orig = const_cast<char *>(str.c_str());
    while( inbytesleft ) {
        if( iconv( cd, &orig, &inbytesleft, &dest, &outbytesleft) == static_cast<size_t>(-1) ) {
#ifdef _GONG_DEBUG
            Xtring msg = "iconv: error converting \"" + str + " from "
                         + mFromCode + " to " + 	mToCode;
            _GONG_DEBUG_WARNING( msg );
#endif
            return str;
        }
    }
    *dest = '\0';
    Xtring ret( savedest );
    delete savedest;
    return ret;
}


const char *IConv::canonicalCodeName(const char * alias)
{
    // Check if this is already a canonical name
    for( uint i=0; i<sizeof(charmap_aliases)/sizeof(charmap_aliases[0]); i++ ) {
        if( strcasecmp( charmap_aliases[i][1], alias) == 0 )
            return charmap_aliases[i][1];
    }
    // If not, check for the aliases
    for( uint i=0; i<sizeof(charmap_aliases)/sizeof(charmap_aliases[0]); i++ ) {
        if( strcasecmp( charmap_aliases[i][0], alias) == 0 )
            return charmap_aliases[i][1];
    }
    return "";
}

} // namespace gong
