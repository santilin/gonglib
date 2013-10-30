/*<<<<<COPYLEFT*/
/** @file empresarecempresa.h
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */
/*>>>>>COPYLEFT*/
/*<<<<<EMPRESA_PREAMBLE*/
#ifndef _EMPRESA_EMPRESARECEMPRESA_H
#define _EMPRESA_EMPRESARECEMPRESA_H
/*>>>>>EMPRESA_PREAMBLE*/

/*<<<<<EMPRESA_INCLUDES*/
#include <dbappdbapplication.h>

#include "contactosreccontacto.h"
#include "empresarecmoneda.h"
/*>>>>>EMPRESA_INCLUDES*/

namespace gong
{
namespace empresa {

/*<<<<<EMPRESA_CONSTRUCTOR*/
class RecEmpresa: public dbRecord
{
public:
    RecEmpresa(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
        : dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("EMPRESA"), recid, user)
/*>>>>>EMPRESA_CONSTRUCTOR*/
    {};

    /*<<<<<EMPRESA_RELATIONS*/
    contactos::RecContacto *getRecContacto() const;
    RecMoneda *getRecMoneda() const;
    /*>>>>>EMPRESA_RELATIONS*/
    /*<<<<<EMPRESA_MEMBERS*/
    virtual bool save(bool saverelated) throw( dbError ); // from dbRecord
    /*>>>>>EMPRESA_MEMBERS*/
}; // class

/*<<<<<EMPRESA_POSTAMBLE*/
} // namespace empresa
} // namespace gong
#endif // EMPRESA_EMPRESARECEMPRESA_H
/*>>>>>EMPRESA_POSTAMBLE*/
