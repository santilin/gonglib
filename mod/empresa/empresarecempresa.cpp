/*<<<<<MODULE_INFO*/
// RELATION contactos::Contacto
// RELATION Moneda
// MEMBER save
// TYPE dbRecord empresa::Empresa
/*>>>>>MODULE_INFO*/

#include <empresamodule.h>
/*<<<<<EMPRESA_INCLUDES*/
#include "empresarecempresa.h"
/*>>>>>EMPRESA_INCLUDES*/


namespace gong {
namespace empresa {

/*<<<<<EMPRESA_RELATIONS*/
contactos::RecContacto *RecEmpresa::getRecContacto() const
{
	return static_cast<contactos::RecContacto*>(findRelatedRecord("CONTACTO_ID"));
}

RecMoneda *RecEmpresa::getRecMoneda() const
{
	return static_cast<RecMoneda*>(findRelatedRecord("MONEDA_ID"));
}

/*>>>>>EMPRESA_RELATIONS*/

/*<<<<<EMPRESA_SAVE*/
bool RecEmpresa::save(bool saverelated) throw( dbError )
{
/*>>>>>EMPRESA_SAVE*/
    // Si se modifica la empresa actual, actualizar la referencia en este módulo a la empresa actual
    bool ret = dbRecord::save(saverelated);
    if( ret && getRecordID() == ModuleInstance->getRecEmpresa()->getRecordID() )
        ModuleInstance->getRecEmpresa()->copyRecord( this );
    return ret;
}

} // namespace empresa
} // namespace gong

