#include <iostream>
#include <fstream>
#include <string>
#include <cerrno>

#include <gongfileutils.h>

#include "rtkinputcsv.h"
#include "rtkreport.h"

namespace gong {}
namespace RTK = gong;

namespace gong {


/**
  @class InputCsv
  @brief Standard csv input object

*/

bool InputCsv::init()
{
    // Locate the csv file in the same dir than the report
    Xtring csvfile = FileUtils::findInPath(Xtring::null, filename(), mReport.getCWD());
    if( csvfile != filename() )
        propFilename.set(csvfile.c_str());
    pFile = new std::ifstream(filename());
    if( !pFile->good() ) {
        mReport.addError(Error::OpenFile, "InputCsv::init", 0, propFilename.get(), strerror(errno) );
        return false;
    }
    Xtring skip;
    for( int hl = propHeaderLines.get(); hl>0; hl-- ) // skip them
        std::getline(*pFile, skip);
    return true;
}


InputCsv::~InputCsv()
{
    if( pFile )
        delete pFile;
}


// Fills the vector of values with the column values
bool InputCsv::next()
{
    if( pFile->eof() )
        return false;
    Xtring line;
    std::getline(*pFile, line);
    if( !line.size() )
        return false;
    Xtring token;
    int len = line.length();
    mRowValues.clear();
    char quoted = '\0';
    bool escaped = false;
    for( int i = 0; i < len; i++)
    {
        if( !escaped && line[i] == '\\' ) {
            escaped = true;
            continue;
        }
        if( escaped ) {
            token += line[ i ];
            continue;
        }
        // Ojo. Don't use Xtring(const char *).find
        if( !quoted && (token.length()==0) && Xtring(propQuote.get()).find( line[ i ] ) != Xtring::npos ) {
            quoted = line[i];
            continue;
        }
        if( quoted && (line[i] == quoted ) ) {
            quoted = '\0';
            continue;
        }
        if ( !quoted && Xtring(propDelimiters.get()).find( line[ i ] ) != Xtring::npos )
        {
            mRowValues.push_back( token );
            i++; // Refactor, changing the i variable is not elegant
            while( isspace(line[i]) && i < len )
                i++;
            i--;
            token.clear();
        }
        else
        {
            token += line[ i ];
        }
    }
    if ( token.length() > 0 )
    {
        mRowValues.push_back( token );
    }
    return mRowValues.size();
}

Variant InputCsv::getValue(uint i, Variant::Type type) const
{
    if( i>=mRowValues.size() )
        return Variant();
    else
        return Variant(mRowValues[i].c_str(), type, 0, getRegConfig());
}

void InputCsv::fixInputParameters(const ParametersList &parameters, const char *delim)
{
    Input::fixInputParameters(parameters, delim);
/*<<<<<INPUTCSV_FIXPARAMETERS*/
	propFilename.fix( parameters, delim );
	propDelimiters.fix( parameters, delim, "," );
	propQuote.fix( parameters, delim, "\"" );
	propHeaderLines.fix( parameters, delim, 0 );
/*>>>>>INPUTCSV_FIXPARAMETERS*/
    if( propQuote.isEmpty() )
        propQuote.set("\"");
}


bool InputCsv::parseAttribute(const char *token, const char *value)
{
    /*<<<<<INPUTCSV_READRTK*/
    if( strcaseequal(token, "Filename") )
        setOrigFilename( value );
    else if( strcaseequal(token, "Delimiters") )
        setOrigDelimiters( value );
    else if( strcaseequal(token, "Quote") )
        setOrigQuote( value );
    else if( strcaseequal(token, "HeaderLines") )
        setOrigHeaderLines( value );
    /*>>>>>INPUTCSV_READRTK*/
    else
        return false;
    return true;
}

} // namespace
