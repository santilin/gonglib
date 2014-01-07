/*<<<<<MODULE_INFO*/
// COPYLEFT Registro de partidas de proyectos
// Relation Proyecto
// TYPE dbRecord socias::PartidaProyecto
/*>>>>>MODULE_INFO*/

/*<<<<<PARTIDAPROYECTO_INCLUDES*/
#include "sociasrecpartidaproyecto.h"
/*>>>>>PARTIDAPROYECTO_INCLUDES*/

namespace gong {
namespace socias {

/*<<<<<PARTIDAPROYECTO_RELATIONS*/
RecProyecto *RecPartidaProyecto::getRecProyecto() const
{
	return static_cast<RecProyecto*>(findRelatedRecord("PARTIDAPROYECTO.PROYECTO_ID"));
}

/*>>>>>PARTIDAPROYECTO_RELATIONS*/

} // namespace socias
} // namespace gong
