#include "rtkproperty.h"
#include "rtkparameter.h"

namespace gong {}
namespace RTK = gong;

namespace gong {


const char *const null_char_ptr = "";

Xtring replaceParams(const char *orig,
                     const ParametersList &parameters, const Xtring &delim)
{
    Xtring ret = orig;
    Xtring d = delim;
    if( !d.size() )
        d = "###";
    if( ret.find(d) != Xtring::npos ) {
        for( uint i=0; i<parameters.size(); i++ )
            ret.replace(d + parameters[i]->name() + d, parameters[i]->value());
    }
    return ret;
}

} // namespace RTK
