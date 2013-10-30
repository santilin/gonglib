/*<<<<<COPYLEFT*/
/** @file contabrectipoiva.h Registro de tipos de IVA
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
/*<<<<<TIPOIVA_PREAMBLE*/
#ifndef _CONTAB_CONTABRECTIPOIVA_H
#define _CONTAB_CONTABRECTIPOIVA_H
/*>>>>>TIPOIVA_PREAMBLE*/

/*<<<<<TIPOIVA_INCLUDES*/
#include <dbappdbapplication.h>

/*>>>>>TIPOIVA_INCLUDES*/

namespace gong {
namespace contab {

/*<<<<<TIPOIVA_CONSTRUCTOR*/
class RecTipoIVA: public empresa::RecTipoIVA
{
public:
    RecTipoIVA(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
        : empresa::RecTipoIVA(conn, recid, user)
/*>>>>>TIPOIVA_CONSTRUCTOR*/
    {};
    /*<<<<<TIPOIVA_MEMBERS*/
    virtual void clear();
    void init();
    /*>>>>>TIPOIVA_MEMBERS*/

}; // class
/*<<<<<TIPOIVA_POSTAMBLE*/
} // namespace contab
} // namespace gong
#endif // CONTAB_CONTABRECTIPOIVA_H
/*>>>>>TIPOIVA_POSTAMBLE*/

