/*<<<<<MODULE_INFO*/
// COPYLEFT Registro de sesiones de TPV
// TYPE dbRecord tpv::SesionTPV
/*>>>>>MODULE_INFO*/

/*<<<<<SESIONTPV_INCLUDES*/
#include "tpvrecsesiontpv.h"
/*>>>>>SESIONTPV_INCLUDES*/

namespace gong {
namespace tpv {

/*<<<<<SESIONTPV_INIT*/
void RecSesionTPV::init()
{
/*>>>>>SESIONTPV_INIT*/
    addStructuralFilter( "SESIONTPV.EMPRESA_ID=" + getConnection()->toSQL( empresa::ModuleInstance->getEmpresaID() ) );
    addStructuralFilter( "SESIONTPV.EJERCICIO=" + getConnection()->toSQL( empresa::ModuleInstance->getEjercicio() ) );
}


RecSesionTPV* RecSesionTPV::getCurrentSesion(dbConnection *conn, uint ntpv)
{
	DateTime dt = DateTime::currentDateTime();
	RecSesionTPV *sesion = static_cast<RecSesionTPV *>( DBAPP->createRecord("SESIONTPV") );
	sesion->read( " INICIO < " + conn->toSQL( dt )
			+ " AND FIN IS NULL"
			+ " AND NUMEROTPV =" + conn->toSQL( ntpv )
			+ sesion->getFilter( " AND" ));
	return sesion;
}


dbRecordID RecSesionTPV::getCurrentSesionID(dbConnection *conn, uint ntpv)
{
	RecSesionTPV *rec = getCurrentSesion(conn, ntpv);
	dbRecordID id = 0;
	if( rec ) {
		id = rec->getRecordID();
		delete rec;
	}
	return id;
}


} // namespace tpv
} // namespace gong
