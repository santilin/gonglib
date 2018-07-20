/*<<<<<REMESACOBRO_PREAMBLE*/
#ifndef _PAGOS_PAGOSRECREMESACOBRO_H
#define _PAGOS_PAGOSRECREMESACOBRO_H
/*>>>>>REMESACOBRO_PREAMBLE*/

/*<<<<<COPYLEFT*/
/** @file pagosrecremesacobro.h Registro de remesas de recibos de proyectos
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

/*<<<<<REMESACOBRO_INCLUDES*/
#include <dbappdbapplication.h>

#include "empresarecproyecto.h"
/*>>>>>REMESACOBRO_INCLUDES*/

namespace gong {
namespace pagos {

/*<<<<<REMESACOBRO_CONSTRUCTOR*/
class RecRemesaCobro: public dbRecord
{
public:
	RecRemesaCobro(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("REMESACOBRO"), recid, user)
/*>>>>>REMESACOBRO_CONSTRUCTOR*/
    {};
    void actTotales();
    /*<<<<<REMESACOBRO_MEMBERS*/
	virtual bool save(bool validate, bool saverelated)/* throw( dbError ) */; // from dbRecord
/*>>>>>REMESACOBRO_MEMBERS*/
    /*<<<<<REMESACOBRO_RELATIONS*/
	empresa::RecProyecto *getRecProyecto() const;
/*>>>>>REMESACOBRO_RELATIONS*/
};

/*<<<<<REMESACOBRO_POSTAMBLE*/
} // namespace pagos
} // namespace gong
#endif // PAGOS_PAGOSRECREMESACOBRO_H
/*>>>>>REMESACOBRO_POSTAMBLE*/
