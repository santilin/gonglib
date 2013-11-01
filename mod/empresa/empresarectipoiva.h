/*<<<<<COPYLEFT*/
/** @file empresarectipoiva.h Registro de tipos de IVA
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
/*<<<<<TIPOIVA_PREAMBLE*/
#ifndef _EMPRESA_EMPRESARECTIPOIVA_H
#define _EMPRESA_EMPRESARECTIPOIVA_H
/*>>>>>TIPOIVA_PREAMBLE*/

/*<<<<<TIPOIVA_INCLUDES*/
#include <dbappdbapplication.h>

/*>>>>>TIPOIVA_INCLUDES*/

namespace gong
{
namespace empresa {

/*<<<<<TIPOIVA_CONSTRUCTOR*/
class RecTipoIVA: public dbRecord
{
public:
	RecTipoIVA(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("TIPOIVA"), recid, user)
/*>>>>>TIPOIVA_CONSTRUCTOR*/
    {};
    Money calcIVA(Money cantidad) const;
    Money calcRecargo(Money cantidad) const;
    Money calcIVARecargo(Money cantidad) const;
    Money masIVA(Money cantidad) const;
    Money masRecargo(Money cantidad) const;
    Money masIVARecargo(Money cantidad) const;
    Money menosIVA(Money cantidad) const;
    Money menosRecargo(Money cantidad) const;
    Money menosIVARecargo(Money cantidad) const;
}; // class

/*<<<<<TIPOIVA_POSTAMBLE*/
} // namespace empresa
} // namespace gong
#endif // EMPRESA_EMPRESARECTIPOIVA_H
/*>>>>>TIPOIVA_POSTAMBLE*/
