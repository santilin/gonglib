/*<<<<<MODULE_INFO*/
// COPYLEFT Registro de agentes comerciales
// RELATION contactos::Contacto
// TYPE dbRecord factu::Agente
/*>>>>>MODULE_INFO*/

/*<<<<<AGENTE_INCLUDES*/
#include "facturecagente.h"
/*>>>>>AGENTE_INCLUDES*/
#include <empresamodule.h>

namespace gong {
namespace factu {

/*<<<<<AGENTE_RELATIONS*/
contactos::RecContacto *RecAgente::getRecContacto() const
{
	return static_cast<contactos::RecContacto*>(findRelatedRecord("AGENTE.CONTACTO_ID"));
}

/*>>>>>AGENTE_RELATIONS*/

} // namespace factu
} // namespace gong

