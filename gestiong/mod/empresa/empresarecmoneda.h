/*<<<<<COPYLEFT*/
/** @file empresarecmoneda.h Registro de monedas
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
/*<<<<<MONEDA_PREAMBLE*/
#ifndef _EMPRESA_EMPRESARECMONEDA_H
#define _EMPRESA_EMPRESARECMONEDA_H
/*>>>>>MONEDA_PREAMBLE*/

/*<<<<<MONEDA_INCLUDES*/
#include <dbappdbapplication.h>

/*>>>>>MONEDA_INCLUDES*/

namespace gong {
namespace empresa {

/*<<<<<MONEDA_CONSTRUCTOR*/
class RecMoneda: public dbRecord
{
public:
    RecMoneda(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
        : dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("MONEDA"), recid, user)
/*>>>>>MONEDA_CONSTRUCTOR*/
    {};
    void setValuesFromRegConfig( const RegConfig &regconfig );
    void setRegConfigFromValues( RegConfig &regconfig ) const ;
}; // end class

/*<<<<<MONEDA_POSTAMBLE*/
} // namespace empresa
} // namespace gong
#endif // EMPRESA_EMPRESARECMONEDA_H
/*>>>>>MONEDA_POSTAMBLE*/
