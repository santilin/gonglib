/*<<<<<MODULE_INFO*/
// COPYLEFT Registro de clientes
// RELATION contactos::Contacto
// RELATION pagos::FormaPago
// RELATION Agente
// INTERFACE public contactos::IContactableRecord
// TYPE dbRecord factu::Cliente
/*>>>>>MODULE_INFO*/

/*<<<<<CLIENTE_INCLUDES*/
#include "factureccliente.h"
/*>>>>>CLIENTE_INCLUDES*/
#include <empresamodule.h>

namespace gong {
namespace factu {

/*<<<<<CLIENTE_RELATIONS*/
contactos::RecContacto *RecCliente::getRecContacto() const
{
	return static_cast<contactos::RecContacto*>(findRelatedRecord("CLIENTE.CONTACTO_ID"));
}

pagos::RecFormaPago *RecCliente::getRecFormaPago() const
{
	return static_cast<pagos::RecFormaPago*>(findRelatedRecord("CLIENTE.FORMAPAGO_ID"));
}

RecAgente *RecCliente::getRecAgente() const
{
	return static_cast<RecAgente*>(findRelatedRecord("CLIENTE.AGENTE_ID"));
}

/*>>>>>CLIENTE_RELATIONS*/

} // namespace factu
} // namespace gong

