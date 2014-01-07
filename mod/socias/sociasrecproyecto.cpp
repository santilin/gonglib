/*<<<<<MODULE_INFO*/
// TYPE dbRecord socias::Proyecto
/*>>>>>MODULE_INFO*/

/*<<<<<PROYECTO_INCLUDES*/
#include "sociasrecproyecto.h"
/*>>>>>PROYECTO_INCLUDES*/

#include "sociasmodule.h"

namespace gong {
namespace socias {

bool RecProyecto::isActivo() const
{
    return getValue("SOCIASESTADO").toInt() == SociasModule::activo;
}

} // namespace socias
} // namespace gong



