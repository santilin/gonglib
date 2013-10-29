/*<<<<<COPYLEFT*/
/** @file empresarecdepartamento.h Registro de departamentos
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
/*<<<<<DEPARTAMENTO_PREAMBLE*/
#ifndef _EMPRESA_EMPRESARECDEPARTAMENTO_H
#define _EMPRESA_EMPRESARECDEPARTAMENTO_H
/*>>>>>DEPARTAMENTO_PREAMBLE*/

/*<<<<<DEPARTAMENTO_INCLUDES*/
#include <dbappdbapplication.h>

/*>>>>>DEPARTAMENTO_INCLUDES*/

namespace gong
{
namespace empresa {

/*<<<<<DEPARTAMENTO_CONSTRUCTOR*/
class RecDepartamento: public dbRecord
{
public:
    RecDepartamento(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
        : dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("DEPARTAMENTO"), recid, user)
/*>>>>>DEPARTAMENTO_CONSTRUCTOR*/
    {}
}; // class

/*<<<<<DEPARTAMENTO_POSTAMBLE*/
} // namespace empresa
} // namespace gong
#endif // EMPRESA_EMPRESARECDEPARTAMENTO_H
/*>>>>>DEPARTAMENTO_POSTAMBLE*/
