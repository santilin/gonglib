/*<<<<<MODULE_INFO*/
// COPYLEFT Registro de formas de pago
// RELATION tesoreria::CuentaTesoreria MODULE_INCLUDED(Tesoreria)
// TYPE dbRecord pagos::FormaPago
/*>>>>>MODULE_INFO*/

/*<<<<<FORMAPAGO_INCLUDES*/
#include "pagosrecformapago.h"
/*>>>>>FORMAPAGO_INCLUDES*/
#include "empresamodule.h"

namespace gong {
namespace pagos {

/*<<<<<FORMAPAGO_RELATIONS*/
#ifdef HAVE_TESORERIAMODULE
// if( ModuleInstance->getTesoreriaModule() ) {
tesoreria::RecCuentaTesoreria *RecFormaPago::getRecCuentaTesoreria() const
{
	return static_cast<tesoreria::RecCuentaTesoreria*>(findRelatedRecord("CUENTATESORERIA_ID"));
}

// }
#endif
/*>>>>>FORMAPAGO_RELATIONS*/
//{capel} eliminar el if fuera de la función

} // namespace pagos
} // namespace gong
