/*<<<<<MODULE_INFO*/
// COPYLEFT Registro de cuentas de ofertas
// MEMBER init
// TYPE dbRecord ofertas::CuentaOferta
/*>>>>>MODULE_INFO*/

#include <empresamodule.h>
/*<<<<<CUENTAOFERTA_INCLUDES*/
#include "ofertasreccuentaoferta.h"
/*>>>>>CUENTAOFERTA_INCLUDES*/

namespace gong {
namespace ofertas {

/*<<<<<CUENTAOFERTA_INIT*/
void RecCuentaOferta::init()
{
/*>>>>>CUENTAOFERTA_INIT*/
    addStructuralFilter( "CUENTAOFERTA.EMPRESA_ID=" + getConnection()->toSQL( empresa::ModuleInstance->getEmpresaID() ) );
}

} // namespace ofertas
} // namespace gong
