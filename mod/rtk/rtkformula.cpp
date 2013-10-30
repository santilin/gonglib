#include <cerrno>
#include <cctype>
#include "rtkformula.h"

namespace gong {}
namespace RTK = gong;

namespace gong {

FormulaToken getNextToken(const Xtring &code, Variant *value, int *pos)
{
    int staticpos = 0;
    int len = code.length();
    if( !pos ) pos = &staticpos;

    while( *pos < len ) {
        while( isspace(code[*pos]) && *pos < len ) (*pos)++;
        if( *pos == len )
            return FT_END;
        if( isdigit(code[*pos]) ) { // Number constant
            char numberconstant[256], *pnc = numberconstant;
            bool isfloat = false;
            *pnc++ = code[(*pos)++];
            while( isdigit(code[*pos]) || code[*pos] == 'e' || code[*pos] == '.' ) {
                if( code[*pos] == 'e' || code[*pos] == '.' )
                    isfloat = true;
                *pnc++ = code[(*pos)++];
            }
            *pnc = '\0';
            if( code[*pos] == '$' ) { // Money constant
                (*pos)++;
                bool ok;
                double d = Xtring(numberconstant).toDoubleLocIndep(&ok);
                if( ok ) {
                    *value = Money(d);
                    return FT_NUMBER;
                }
            } else if( code[*pos] == 'b' ) { // Binary constant
                (*pos)++;
                errno = 0;
                int i = strtol(numberconstant, 0, 2 );
                if( errno == 0 ) {
                    *value = i;
                    return FT_NUMBER;
                }
            } else if( isfloat ) { // float constant
                bool ok;
                double d = Xtring(numberconstant).toDoubleLocIndep(&ok);
                if( ok ) {
                    *value = d;
                    return FT_NUMBER;
                }
            } else { // integer constant
                errno = 0;
                int l = strtol(numberconstant, 0, 0);
                if( errno == 0 ) {
                    *value = l;
                    return FT_NUMBER;
                }
            }
            *value = pnc;
            return FT_ERROR;
        } else if( isalpha(code[*pos]) || code[*pos] == '_' || code[*pos] == '@'
                   || code[*pos] == '$' || code[*pos] < 0 )  { // a name
            Xtring name(1, code[(*pos)++]);
            while( isalpha(code[*pos]) || isdigit(code[*pos]) || code[*pos] == '_' || code[*pos] < 0 )
                name += code[(*pos)++];
            *value = name;
            return FT_NAME;
        } else { // May be an operator, comment or string literal
            char op[3]="\0\0";
            switch( *op = code[*pos] ) {
            case '.':
                (*pos)++;
                return FT_DOT;
            case '\"':
            case '\'':
            {   // Todo "hola""adios" y escape chars
                (*pos)++;
                Xtring literal;
                while( code[*pos] != *op && *pos < len )
                    literal += code[(*pos)++];
                (*pos)++;
                *value = literal;
                return FT_STRING;
            }
            case '+': // May be += or ++
            case '-': // May be -= or --
                if( code[(*pos)+1] == *op || code[(*pos)+1] == '=' )  {
                    (*pos)++;
                    *(op+1) = code[(*pos)+1];
                }
                (*pos)++;
                *value = op;
                return FT_OPERATOR;
            case '%': // May be op or op=
            case '*':
            case '=':
            case '<':
            case '>':
                if( code[(*pos)+1] == '=' )  {
                    (*pos)++;
                    *(op+1) = '=';
                }
                (*pos)++;
                *value = op;
                return FT_OPERATOR;
            case '/': // May be a comment or / or /=
                switch( code[(*pos)+1] ) {
                case '=': // Was operator /=
                    (*pos)++;
                    *(op+1) = '=';
                    (*pos)++;
                    *value = op;
                    return FT_OPERATOR;
                case '/': // Was C++ comment
                    (*pos)+=2;
                    while( code[*pos] != '\n' && *pos < len ) (*pos)++;
                    (*pos)++;
                    break;
                case '*': // Was C comment
                    (*pos)+=2;
                    while( *pos < len ) {
                        while( code[*pos] != '*' && *pos < len ) (*pos)++;
                        if( *pos == len )
                            continue;
                        (*pos)++;
                        if( code[*pos] == '/' )
                            break;
                    }
                    (*pos)++;
                    break;
                }
                break;
            case ':':
            case '?':
                (*pos)++;
                *value = op;
                return FT_OPERATOR;
            case '(':
            case ')':
            case '{':
            case '}':
            case '[':
            case ']':
                (*pos)++;
                *value = op;
                return FT_BRACKET;
            case ';':
            case ',':
                (*pos)++;
                *value = op;
                return FT_SEMICOLON;
            default:
                *value = op;
                return FT_ERROR;
                break;
            }
        }
    }
    return FT_END;
}

} // Namespace RTK
