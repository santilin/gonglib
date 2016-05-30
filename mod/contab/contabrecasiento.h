/*<<<<<COPYLEFT*/
/** @file contabrecasiento.h Registro de Asientos
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
/*<<<<<ASIENTO_PREAMBLE*/
#ifndef _CONTAB_CONTABRECASIENTO_H
#define _CONTAB_CONTABRECASIENTO_H
/*>>>>>ASIENTO_PREAMBLE*/

/*<<<<<ASIENTO_INCLUDES*/
#include <dbappdbapplication.h>

#include "empresarecproyecto.h"
#include "contabrecapunte.h"
/*>>>>>ASIENTO_INCLUDES*/
#include <gongmoney.h>

namespace gong {
namespace contab {

/*<<<<<ASIENTO_CONSTRUCTOR*/
class RecAsiento: public dbRecord
{
public:
	RecAsiento(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("ASIENTO"), recid, user)
/*>>>>>ASIENTO_CONSTRUCTOR*/
    {
        init();
    }
    /*<<<<<ASIENTO_MEMBERS*/
	Xtring toString(int format, const Xtring &includedFields = Xtring::null) const;
/*>>>>>ASIENTO_MEMBERS*/
    void rescateValues( RecAsiento *other );

    /*<<<<<ASIENTO_RELATIONS*/
	empresa::RecProyecto *getRecProyecto() const;
	RecApunte *getRecApunte( int napunte = -1 ) const;
	dbRecordList *getListApunte() const;
/*>>>>>ASIENTO_RELATIONS*/

    class RecApunte *getRecDetalle(int napunte = -1) const {
        return getRecApunte(napunte);
    }
    dbRecordList *getApuntes() const {
        return getListApunte();
    }
// 	int getLastNumDocumento( int proyecto_id ) const;
// 	Xtring formatNumDocumento( int contador, int codproyecto, const Xtring &formato ) const;
    void borrarDebeYHaberApuntes();
    Money updateSaldo();
    unsigned int numeraApuntes();
    unsigned int fixConceptos();
    Money cuadraUltimoApunte(); // Devuelve el saldo antes de cuadrar
    void addApunte(const RecApunte *apunte, int pos = -1);

private:
    void init();
}; // class

/*<<<<<ASIENTO_POSTAMBLE*/
} // namespace contab
} // namespace gong
#endif // CONTAB_CONTABRECASIENTO_H
/*>>>>>ASIENTO_POSTAMBLE*/


