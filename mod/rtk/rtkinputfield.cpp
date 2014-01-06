#include "rtkinputfield.h"

namespace gong {}
namespace RTK = gong;

namespace gong {


InputField::InputField(const char * name)
    : propName(name), mSourcePos((uint)-1)
{
}


void InputField::fixParameters(const ParametersList &parameters, const char *delim)
{
    /*<<<<<INPUTFIELD_FIXPARAMETERS*/
	propSource.fix( parameters, delim );
	propType.fix( parameters, delim, Variant::tInvalid );
	propConstValue.fix( parameters, delim );
/*>>>>>INPUTFIELD_FIXPARAMETERS*/
    if( !propConstValue.isEmpty() )
        mSourcePos = -1;
    else {// TODO: Revisar esto porque falla
        bool ok = false;
        mSourcePos = Xtring(propSource.get()).toInt(&ok);
        if( !ok )
            mSourcePos = -1;
    }
}


} // namespace


