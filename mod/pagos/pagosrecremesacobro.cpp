/*<<<<<MODULE_INFO*/
// COPYLEFT Registro de remesas de recibos de proyectos
// RELATION empresa::Proyecto
// MEMBER save
// TYPE dbRecord pagos::RemesaCobro
/*>>>>>MODULE_INFO*/

/*<<<<<REMESACOBRO_INCLUDES*/
#include "pagosrecremesacobro.h"
/*>>>>>REMESACOBRO_INCLUDES*/

namespace gong {
namespace pagos {

void RecRemesaCobro::actTotales()
{
    Variant nrecibos = 0, importe = Money(0.0), resto = Money(0.0);
    if( getRecordID() )
        getConnection()->selectValues( "SELECT COUNT(*), SUM(IMPORTE), SUM(RESTO) FROM COBRO"
                                       " WHERE REMESACOBRO_ID=" + getConnection()->toSQL( getRecordID() ), &nrecibos, &importe, &resto );
    setValue( "NRECIBOS", nrecibos );
    setValue( "IMPORTE", importe );
    setValue( "RESTO", importe );
}

/*<<<<<REMESACOBRO_SAVE*/
bool RecRemesaCobro::save(bool validate, bool saverelated = true) throw( dbError )
{
/*>>>>>REMESACOBRO_SAVE*/
    actTotales();
    return dbRecord::save(validate, saverelated);
}

/*<<<<<REMESACOBRO_RELATIONS*/
empresa::RecProyecto *RecRemesaCobro::getRecProyecto() const
{
	return static_cast<empresa::RecProyecto*>(findRelatedRecord("PROYECTO_ID"));
}

/*>>>>>REMESACOBRO_RELATIONS*/

} // namespace pagos
} // namespace gong
