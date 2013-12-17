#include <gongdebug.h>
#include <gongmoney.h>
#include <gongiconv.h>
#include "rtkinput.h"

namespace gong {}
namespace RTK = gong;

namespace gong {

Input::~Input()
{
    for( uint i=0; i<mInputFields.size(); i++ ) {
        delete mInputFields[i];
    }
}

int Input::findInputField(const Xtring &name) const
{
    for( uint i=0; i<mInputFields.size(); i++ ) {
        _GONG_DEBUG_PRINT( 6, mInputFields[i]->name() );
        if( !strcasecmp(mInputFields[i]->name(), name.c_str() ) )
            return i;
    }
    return -1;
}

int Input::insertInputField(InputField *inputfield)
{
    mInputFields.push_back(inputfield);
    return mInputFields.size();
}

int Input::feedInputFieldValues()
{
    IConv *c = 0; /// \todo {refactor} Move this to RTK::Report or have a different locale for input?
    if( !strempty( encoding() )
            && (strcasecmp( IConv::canonicalCodeName(encoding()), "UTF-8") ) != 0 )
        c = new IConv( "UTF-8", encoding() );
    for( uint i=0; i<mInputFields.size(); i++ ) {
        InputField *ifld = mInputFields[i];
        if( ifld->sourcePos() == -1 ) {
            if( strlen( ifld->constValue() ) != 0 ) {
                if( ifld->value().type() == Variant::tInvalid ) {
                    ifld->setValue( Variant( ifld->constValue(), ifld->type() ) );
                }
                continue; // Do not iconvert again
            } else {
                ifld->setValue( getValue( i, ifld->type() ) );
            }
        } else
            ifld->setValue( getValue( ifld->sourcePos(), ifld->type() ) );
        if( c && ifld->type() == Variant::tString )
            ifld->setValue( c->convert( ifld->value().toString() ) );
// 		_GONG_DEBUG_PRINT(0, Xtring::printf("%s = %s", ifld->name(), ifld->value().toString().c_str() ) );
    }
    if( c )
        delete c;
    return mInputFields.size();
}


void Input::fixInputParameters(const ParametersList &parameters, const char *delim)
{
    propEncoding.fix( parameters, delim );
    for( uint i=0; i<countFields(); i++ ) {
        InputField *ifld = getInputField(i);
		_GONG_DEBUG_PRINT(0, ifld->name() );
        ifld->fixParameters(parameters, delim);
    }
}


#ifdef _GONG_DEBUG
Xtring Input::debugPrint(const Xtring &file, const Xtring &function, int line) const
{
    return file + ":" + Xtring::number(line) + ':' + function + ": ";
}
#endif

} // namespace RTK
