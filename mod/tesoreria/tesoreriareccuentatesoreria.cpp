/*<<<<<MODULE_INFO*/
// COPYLEFT Registro de cuentas de tesorería
// MEMBER init
// TYPE dbRecord tesoreria::CuentaTesoreria
/*>>>>>MODULE_INFO*/

#include <empresamodule.h>

/*<<<<<CUENTATESORERIA_INCLUDES*/
#include "tesoreriareccuentatesoreria.h"
/*>>>>>CUENTATESORERIA_INCLUDES*/

namespace gong {
namespace tesoreria {

/*<<<<<CUENTATESORERIA_INIT*/
void RecCuentaTesoreria::init()
{
/*>>>>>CUENTATESORERIA_INIT*/
    addStructuralFilter( "CUENTATESORERIA.EMPRESA_ID=" + getConnection()->toSQL( empresa::ModuleInstance->getEmpresaID() ) );
}

} // namespace tesoreria
} // namespace gong
