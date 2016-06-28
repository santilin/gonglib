/*<<<<<APUNTE_PREAMBLE*/
#ifndef _SOCIAS_SOCIASRECAPUNTE_H
#define _SOCIAS_SOCIASRECAPUNTE_H
/*>>>>>APUNTE_PREAMBLE*/

/*<<<<<COPYLEFT*/
/** @file sociasrecapunte.h Registro de apuntes del módulo de socias
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

#include "config.h"
#ifdef HAVE_CONTABMODULE

/*<<<<<APUNTE_INCLUDES*/
#include <dbappdbapplication.h>

#include "sociasrecpartidaproyecto.h"
/*>>>>>APUNTE_INCLUDES*/
#include <contabrecapunte.h>

namespace gong {
namespace socias {

/*<<<<<APUNTE_CONSTRUCTOR*/
class RecApunte: public contab::RecApunte
{
public:
	RecApunte(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: contab::RecApunte(conn, recid, user)
/*>>>>>APUNTE_CONSTRUCTOR*/
    {};
    /*<<<<<APUNTE_RELATIONS*/
	RecPartidaProyecto *getRecPartidaProyecto() const;
/*>>>>>APUNTE_RELATIONS*/
    int actImportesPartida(bool sumar, const dbRecordID partida_id, Money importe);
    /*<<<<<APUNTE_MEMBERS*/
	virtual bool save(bool validate, bool saverelated) throw( dbError ); // from dbRecord
	virtual bool remove() throw( dbError ); // from dbRecord
/*>>>>>APUNTE_MEMBERS*/
}; // end class

/*<<<<<APUNTE_POSTAMBLE*/
} // namespace socias
} // namespace gong
#endif // SOCIAS_SOCIASRECAPUNTE_H
/*>>>>>APUNTE_POSTAMBLE*/
#endif

