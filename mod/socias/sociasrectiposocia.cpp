/*<<<<<MODULE_INFO*/
// COPYLEFT Registro de tipos de socia
// MEMBER isValid
// TYPE dbRecord socias::TipoSocia
/*>>>>>MODULE_INFO*/

/*<<<<<TIPOSOCIA_INCLUDES*/
#include "sociasrectiposocia.h"
/*>>>>>TIPOSOCIA_INCLUDES*/

namespace gong {
namespace socias {

/*<<<<<TIPOSOCIA_ISVALID*/
bool RecTipoSocia::isValid(ValidResult::Context context)
{
	bool ret = dbRecord::isValid(context, result);
/*>>>>>TIPOSOCIA_ISVALID*/
    return ret;
}

} // namespace socias
} // namespace gong

