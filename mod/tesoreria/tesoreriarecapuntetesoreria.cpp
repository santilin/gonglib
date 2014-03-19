/*<<<<<MODULE_INFO*/
// COPYLEFT Registro de apuntes de tesorería
// MEMBER init
// RELATION CuentaTesoreria
// RELATION empresa::Proyecto
// TYPE dbRecord tesoreria::ApunteTesoreria
/*>>>>>MODULE_INFO*/

#include <empresamodule.h>
/*<<<<<APUNTETESORERIA_INCLUDES*/
#include "tesoreriarecapuntetesoreria.h"
/*>>>>>APUNTETESORERIA_INCLUDES*/

namespace gong {
namespace tesoreria {

/*<<<<<APUNTETESORERIA_INIT*/
void RecApunteTesoreria::init()
{
/*>>>>>APUNTETESORERIA_INIT*/
    addStructuralFilter( "APUNTETESORERIA.EMPRESA_ID=" + getConnection()->toSQL( empresa::ModuleInstance->getEmpresaID() ) );
}

/*<<<<<APUNTETESORERIA_RELATIONS*/
RecCuentaTesoreria *RecApunteTesoreria::getRecCuentaTesoreria() const
{
	return static_cast<RecCuentaTesoreria*>(findRelatedRecord("CUENTATESORERIA_ID"));
}

empresa::RecProyecto *RecApunteTesoreria::getRecProyecto() const
{
	return static_cast<empresa::RecProyecto*>(findRelatedRecord("PROYECTO_ID"));
}

/*>>>>>APUNTETESORERIA_RELATIONS*/

} // namespace tesoreria
} // namespace gong
