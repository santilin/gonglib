/*<<<<<COPYLEFT*/
/** @file tpvrecsesiontpv.h Registro de sesiones de TPV
 * Proyecto GestiONG. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */
/*>>>>>COPYLEFT*/
/*<<<<<SESIONTPV_PREAMBLE*/
#ifndef _TPV_TPVRECSESIONTPV_H
#define _TPV_TPVRECSESIONTPV_H
/*>>>>>SESIONTPV_PREAMBLE*/

/*<<<<<SESIONTPV_INCLUDES*/
#include <dbappdbapplication.h>

/*>>>>>SESIONTPV_INCLUDES*/
#include <empresamodule.h>

namespace gong {
namespace tpv {

/*<<<<<SESIONTPV_CONSTRUCTOR*/
class RecSesionTPV: public dbRecord
{
public:
	RecSesionTPV(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("SESIONTPV"), recid, user)
/*>>>>>SESIONTPV_CONSTRUCTOR*/
	{};
	void init();	
	static dbRecordID getCurrentSesionID( dbConnection *conn, uint numerotpv );
	static RecSesionTPV *getCurrentSesion( dbConnection *conn, uint numerotpv );
	int getNextSession( uint numerotpv );
}; // end class

/*<<<<<SESIONTPV_POSTAMBLE*/
} // namespace tpv
} // namespace gong
#endif // TPV_TPVRECSESIONTPV_H
/*>>>>>SESIONTPV_POSTAMBLE*/

