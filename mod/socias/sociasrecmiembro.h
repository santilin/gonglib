/*<<<<<COPYLEFT*/
/** @file sociasrecmiembro.h 
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
/*<<<<<MIEMBRO_PREAMBLE*/
#ifndef _SOCIAS_SOCIASRECMIEMBRO_H
#define _SOCIAS_SOCIASRECMIEMBRO_H
/*>>>>>MIEMBRO_PREAMBLE*/

#include "config.h"
/*<<<<<MIEMBRO_INCLUDES*/
#include <dbappdbapplication.h>

#include "sociasrecproyecto.h"
#ifdef HAVE_PAGOSMODULE
#include "pagosrecformapago.h"
#endif
#include "contactosreccontacto.h"
#ifdef HAVE_FACTUMODULE
#include "factureccliente.h"
#endif
#include "sociasrectiposocia.h"
/*>>>>>MIEMBRO_INCLUDES*/
#include "sociasipagables.h"

namespace gong {
namespace socias {

/*<<<<<MIEMBRO_CONSTRUCTOR*/
class RecMiembro: public dbRecord,
#ifdef HAVE_PAGOSMODULE
	public IPagableMiembro
#endif

{
public:
	RecMiembro(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("MIEMBRO"), recid, user)
/*>>>>>MIEMBRO_CONSTRUCTOR*/
        , IPagableMiembro( this ) {}
    int getLastNumeroSocia(int proyecto_id) const;
    /*<<<<<MIEMBRO_RELATIONS*/
	RecProyecto *getRecProyecto() const;
#ifdef HAVE_PAGOSMODULE
	pagos::RecFormaPago *getRecFormaPago() const;
#endif
	contactos::RecContacto *getRecContacto() const;
#ifdef HAVE_FACTUMODULE
	factu::RecCliente *getRecCliente() const;
#endif
	RecTipoSocia *getRecTipoSocia() const;
/*>>>>>MIEMBRO_RELATIONS*/
/*<<<<<MIEMBRO_MEMBERS*/
	virtual bool findMatchingRecord(dbRecordID *matchingid) const; // from dbRecord
	Xtring toString(int format, const Xtring &includedFields = Xtring::null) const;
	virtual bool save(bool saverelated) throw( dbError ); // from dbRecord
	virtual bool remove() throw( dbError ); // from dbRecord
/*>>>>>MIEMBRO_MEMBERS*/
    virtual bool fromString(const Xtring &source, int format,
                            const Xtring &includedFields = Xtring::null);
}; // class

/*<<<<<MIEMBRO_POSTAMBLE*/
} // namespace socias
} // namespace gong
#endif // SOCIAS_SOCIASRECMIEMBRO_H
/*>>>>>MIEMBRO_POSTAMBLE*/


