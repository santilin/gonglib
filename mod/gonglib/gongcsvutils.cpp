#include <fstream>
#include "gongdebug.h"
#include "gongcsvutils.h"

namespace gong {

CsvUtils::CsvUtils()
{
}


CsvUtils::~CsvUtils()
{
}

bool CsvUtils::readCSVFile(const Xtring &fname, XtringList &lines, Xtring &headers,
                           bool hasheaders, char quotes )
{
    _GONG_DEBUG_PRINT(5, fname);
    std::ifstream *file = new std::ifstream(fname.c_str());
    if( !file->good() ) {
        return false;
    }
    if( hasheaders ) {
        std::getline(*file, headers);
        _GONG_DEBUG_PRINT(5, "Headers: " + headers);
    }
    Xtring line;
    if( quotes != '\0' ) {
        // We can handle multiple line values
        while( !file->eof() ) {
            line.clear();
            std::getline(*file, line);
            if( line.trim().isEmpty() )
                continue;
            while( 1 ) {
                bool quotes_ok = true;
                for( uint pos = 0; pos < line.length(); ++pos ) {
                    if( line[pos] == quotes && !( pos > 0 && line[pos-1] == '\\') )
                        quotes_ok = !quotes_ok;
                }
                if( !quotes_ok ) {
                    Xtring continuation;
                    std::getline( *file, continuation );
                    if( !continuation.isEmpty() && !file->eof() )
                        line += "\n" + continuation;
                    if( file->eof() ) {
                        break;
                    }
                } else {
                    break;
                }
            }
            lines.push_back( line );
        }
    } else {
        while( !file->eof() ) {
            std::getline(*file, line);
            if( line.trim().isEmpty() )
                continue;
        }
    }
    delete file;
    return true;
}

int CsvUtils::tokenize(XtringList &values, const Xtring &line,
                       char quotes, char delim, const Xtring &encoding)
{
    _GONG_DEBUG_PRINT(4, line);
    int ncol = 0;
    Xtring token;
    int len = line.length();
    char quoted = '\0';
    bool escaped = false, remainingtoken = false;
    for( int i = 0; i < len; i++)
    {
        if( !escaped && line[i] == '\\' ) {
            escaped = true;
            continue;
        }
        if( escaped ) {
            if( line[i] == 'n' )
                token += "\n";
            else if( line[i] == 't' )
                token += "\t";
            else
                token += line[ i ];
            remainingtoken = true;
            escaped = false;
            continue;
        }
        // Ojo. Don't use Xtring(const char *).find
        if( !quoted && (token.length()==0) && quotes == line[i] ) {
            quoted = line[i];
            remainingtoken = true;
            continue;
        }
        if( quoted && (line[i] == quoted ) ) {
            if( line[i+1] == quoted ) {
                token += line[ i ];
                i++;
                remainingtoken = true;
                continue;
            } else {
                quoted = '\0';
                continue;
            }
        }
        if ( !quoted && delim == line[i] )
        {
            values.push_back( token.trim() );
            _GONG_DEBUG_PRINT(4, Xtring::printf("Adding token '%s'", token.trim().c_str()) );
            i++; // Refactor, changing the i variable is not elegant
            while( isspace(line[i]) && i < len )
                i++;
            i--;
            token.clear();
            remainingtoken = false;
            ncol++;
        }
        else
        {
            token += line[ i ];
            remainingtoken = true;
        }
    }
    if( remainingtoken ) { // There might be a final "" token that must be added
        values.push_back( token.trim() );
        _GONG_DEBUG_PRINT(4, Xtring::printf("Adding token '%s'", token.trim().c_str()) );
    }
    _GONG_DEBUG_PRINT(3, Xtring::printf("%d tokens", values.size()) );
    return ncol;
}

Xtring CsvUtils::dupQuotes( const Xtring &source, char quote )
{
    Xtring ret;
    for( uint i = 0; i < source.length(); ++i ) {
        ret += source[i];
        if( source[i] == quote )
            ret += quote;
    }
    return ret;
}

}
