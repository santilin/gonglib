/*<<<<<MODULE_INFO*/
// COPYLEFT Registro de proveedoras
// RELATION contactos::Contacto
// RELATION Agente
// RELATION pagos::FormaPago
// TYPE dbRecord factu::Proveedora
/*>>>>>MODULE_INFO*/

/*<<<<<PROVEEDORA_INCLUDES*/
#include "facturecproveedora.h"
/*>>>>>PROVEEDORA_INCLUDES*/
#include <empresamodule.h>

namespace gong {
namespace factu {

/*<<<<<PROVEEDORA_RELATIONS*/
contactos::RecContacto *RecProveedora::getRecContacto() const
{
	return static_cast<contactos::RecContacto*>(findRelatedRecord("PROVEEDORA.CONTACTO_ID"));
}

RecAgente *RecProveedora::getRecAgente() const
{
	return static_cast<RecAgente*>(findRelatedRecord("PROVEEDORA.AGENTE_ID"));
}

pagos::RecFormaPago *RecProveedora::getRecFormaPago() const
{
	return static_cast<pagos::RecFormaPago*>(findRelatedRecord("PROVEEDORA.FORMAPAGO_ID"));
}

/*>>>>>PROVEEDORA_RELATIONS*/

} // namespace factu
} // namespace gong

