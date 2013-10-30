#include <cerrno>
#include <fstream>

#include "gongsettings.h"

namespace gong {

bool Settings::read(const Xtring &filename)
{
    const char *cfilename = filename.isEmpty() ? mFilename.c_str() : filename.c_str();

    std::ifstream f;
    f.open( cfilename, std::ios_base::binary | std::ios_base::in );
    if ( !f.is_open() || !f.good() ) {
        _GONG_DEBUG_WARNING( Xtring::printf("%s: %s", strerror( errno ), cfilename) );
        return false;
    }
    _GONG_DEBUG_PRINT(0, Xtring::printf("Reading settings from %s", cfilename) );
    f.seekg( 0, std::ios_base::end );
    long end_pos = f.tellg();
    f.seekg( 0, std::ios_base::beg );
    char *buffer = new char[ end_pos + 1 ];
    f.read( buffer, end_pos );
    buffer[ end_pos ] = '\0';
    bool ret = readString( buffer );
    delete [] buffer;
    return ret;
}

bool Settings::readString(const char *wholetext)
{
    if( !wholetext || !*wholetext )
        return true;
    const char **text = &wholetext;
    mLine = mCol = 0;
    Xtring token;
    // FIXME: getNextToken( &mLIne, text) para que actualice el número de línea
    while( !((token = getNextToken( text )).isEmpty()) ) {
        Xtring name = token;
        Xtring type;
        token = getNextToken(text);
        if( token == "(" ) {
            type = getNextToken(text);
            token = getNextToken(text);
            if( token != ")" ) {
                _GONG_DEBUG_WARNING(Xtring::printf("Line: %d, '=' expected after type %s", mLine, type.c_str() ) );
                return false; /// TODO: Skip until end of line
            } else {
                token = getNextToken(text);
            }
        }
        if( token != "=" ) {
            _GONG_DEBUG_WARNING(Xtring::printf("Line: %d, '=' expected after %s", mLine, name.c_str() ) );
            return false; /// TODO: Skip until end of line
        }
        Xtring value = getNextValue(text);
        _GONG_DEBUG_PRINT(10, Xtring::printf("Inserting %s(%s)='%s' in source %s",
                                             name.c_str(), type.c_str(),
                                             value.c_str(), mFilename.c_str() ) );
        Variant vvalue( Variant::nameToType( type.c_str() ) );
        vvalue.copy(value); // Sets the value of the variant without changing its type
        mSettingsDict.set(name, vvalue);
        const char *save_text = *text;
        token = getNextToken( text );
        if ( token != ";" ) {
            _GONG_DEBUG_WARNING(Xtring::printf("Line: %d, ';' expected after %s", mLine, value.c_str() ) );
            *text = save_text;
        }
    }
    return true;
}


bool Settings::write(const Xtring &filename)
{
    const char *cfilename = filename.isEmpty() ? mFilename.c_str() : filename.c_str();
    if( mIsReadOnly ) {
        _GONG_DEBUG_PRINT(1, Xtring::printf("NOT writing readonly settings to %s", cfilename) );
        return true;
    }

    _GONG_DEBUG_PRINT(1, Xtring::printf("Writing settings to %s", cfilename) );
    std::ofstream of(cfilename);
    if( !of ) {
        _GONG_DEBUG_PERROR(cfilename);
        return false;
    } else {
        for( unsigned int nv=0; nv<mSettingsDict.size(); nv++ ) {
            Variant v(mSettingsDict[nv]);
            Xtring s_v = v.toString();
            s_v = s_v.replace("\\", "\\\\").replace("\"", "\\\"");
            of << mSettingsDict.getKey(nv) << "(" << v.typeToName( v.type() ) << ")"
               << " = \"" << s_v << "\";" << std::endl;
        }
        if( of.bad()  ) {
            _GONG_DEBUG_PERROR(cfilename);
            return false;
        }
        of.close();
    }
    return true;
}

Variant Settings::getValue(const Xtring &settingname, const Variant &defaultvalue) const
{
    _GONG_DEBUG_ASSERT( this );
    Variant v(mSettingsDict[settingname]);
    if( v.isValid() ) {
        _GONG_DEBUG_PRINT(10, Xtring::printf("FOUND %s in source %s, value=%s",
                                             settingname.c_str(), mFilename.c_str(),
                                             v.toString().c_str() ) );
        return v;
    }
    _GONG_DEBUG_PRINT(6, Xtring::printf("NOT FOUND %s in source %s, with keys:\n%s",
                                        settingname.c_str(), mFilename.c_str(), toString().c_str() ) );
    return defaultvalue;
}

void Settings::setValue(const Xtring &settingname, const Variant &val)
{
    _GONG_DEBUG_ASSERT( this );
    Variant v(mSettingsDict[settingname]);
// 	_GONG_DEBUG_PRINT(0, Xtring::printf("Setting %s(%s)='%s' in source %s",
// 		                                     settingname.c_str(), Variant::typeToName(v.type()),
// 		                                     val.toString().c_str(), mFilename.c_str() ) );
    if( v.isValid() ) { // preserve the type of the setting
        v.copy( val );
        mSettingsDict[settingname] = v;
    }
    else
        mSettingsDict.insert(settingname, val);
}


// Las siguientes funciones las he copiado exactamente de reports/rtklib/rtkparsertk.cpp,
// pero le añado que el punto no separa los tokens y los comentarios con #

Xtring Settings::getNextToken( const char **starttoken )
{
    const char * code = *starttoken;
    Xtring ret;

    while ( *code ) {
        while ( isspace( *code ) && *code ) {
            if ( *code == '\n' ) {
                mLine++;
                mCol = 1;
                code++;
            } else {
                mCol++, code++;
            }
        }
        if( !*code )
            return Xtring();
        *starttoken = code;
        if ( isdigit( *code ) ) { // Number constant
            while ( isdigit( *code ) || *code == 'e' || *code == '.' ) {
                mCol++, code++;
            }
            ret = Xtring( *starttoken, code - *starttoken );
            *starttoken = code;
            return ret;
        } else if ( isalpha( *code ) || *code == '_' || *code == '.' || ( unsigned char ) ( *code ) >= '\x7f' ) { // a name
            while ( isalpha( *code ) || isdigit( *code ) || *code == '_' || *code == '.' || ( unsigned char ) ( *code ) >= '\x7f' ) {
                mCol++, code++;
            }
            ret = Xtring( *starttoken, code - *starttoken );
            *starttoken = code;
            return ret;
        } else { // May be an operator, comment or string literal
            char op = '\0';
            switch ( op = *code ) {
            case '\"':
            case '\'': { // Todo "hola""adios" y escape chars
                while ( *code != op && *code ) {
                    if ( *code == '\n' ) {
                        mLine++;
                        mCol = 1;
                        code++;
                    } else {
                        mCol++, code++;
                    }
                }
                if ( *code ) {
                    ret = Xtring( *starttoken, code - *starttoken );
                    *starttoken = code;
                    return ret;
                } else {
                    *starttoken = code;
                    return Xtring(); // Error
                }
            }
            case '-':  // May be a comment
            case '/':
                if ( *( code + 1 ) == *code ) {
                    while ( *code != '\n' && *code ) {
                        mCol++, code++;
                    }
                    if ( *code == '\n' ) {
                        mLine++;
                        mCol = 1;
                        code++;
                    }
                } else {
                    *starttoken = code;
                    return Xtring(1, *code );
                }
                break;
            default:
                *starttoken = code + 1;
                return Xtring(1, *code );
            }
        }
    }
    return ret;
}


// Values can be between quotes, or if not, up to the next un-quoted semicolon
// If the value is not quoted, only the next word is taken

/// \todo {0.3.4} Check line numbers
Xtring Settings::getNextValue( const char **starttoken )
{
    const char * code = *starttoken;
    char quotes = '\0';
    bool escaping = false;
    Xtring ret;
    if ( *code ) {
        while ( isspace( *code ) && *code && *code != ';' && *code != '\n' ) {
            mCol++, code++;
        }
        if ( !*code || *code == ';' || *code == '\n' )
            return Xtring();
        if ( *code == '\"' || *code == '\'' ) { // Initial quotes
            quotes = *code++;
        }
        *starttoken = code;
        while ( *code ) {
            if( escaping ) {
                escaping = false;
            } else {
                if ( *code == quotes ) { /// \todo {0.3.4} concatenate "abc" "def" ?
                    ret = Xtring( *starttoken, code - *starttoken );
                    *starttoken = code + 1;
                    break;
                } else if ( *code == '\\' ) {
                    escaping = true;
                } else if ( *code == ';' || *code == '\n' || *code == ' ' ) {
                    if ( !quotes ) {
                        ret = Xtring( *starttoken, code - *starttoken ).trimRight();
                        *starttoken = code;
                        break;
                    }
                }
                if ( *code == '\n' ) {
                    mLine++;
                    mCol = 0; // It will be incremented below
                }
            }
            mCol++;
            code++;
        }
    }
    return ret.replace("\\\n", "").replace("\\t", "\t").replace("\\\r", "")
           .replace("\\\"", "\"");
}

void Settings::clear()
{
    mSettingsDict.clear();
}


Xtring Settings::toString() const
{
    return mSettingsDict.toString();
}

void Settings::removeSetting(const Xtring& settingname)
{
    mSettingsDict.erase( settingname );
}


#ifdef _GONG_DEBUG
std::ostream &operator<<(std::ostream &out, const Settings &settings)
{
    out << settings.getName() << std::endl << settings.toString();
    return out;
}
#endif


}

