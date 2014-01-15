/*<<<<<MODULE_INFO*/
// COPYLEFT Registro de familias de artículos
// RELATION empresa::TipoIVA
// TYPE dbRecord factu::Familia
/*>>>>>MODULE_INFO*/

/*<<<<<FAMILIA_INCLUDES*/
#include "facturecfamilia.h"
/*>>>>>FAMILIA_INCLUDES*/
#include <empresamodule.h>

namespace gong {
namespace factu {

/*<<<<<FAMILIA_RELATIONS*/
empresa::RecTipoIVA *RecFamilia::getRecTipoIVA() const
{
	return static_cast<empresa::RecTipoIVA*>(findRelatedRecord("TIPOIVA_ID"));
}

/*>>>>>FAMILIA_RELATIONS*/

} // namespace factu
} // namespace gong
