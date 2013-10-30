/*<<<<<COPYLEFT*/
/** @file contabreccuenta.h Registro de cuentas
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
/*<<<<<CUENTA_PREAMBLE*/
#ifndef _CONTAB_CONTABRECCUENTA_H
#define _CONTAB_CONTABRECCUENTA_H
/*>>>>>CUENTA_PREAMBLE*/

/*<<<<<CUENTA_INCLUDES*/
#include <dbappdbapplication.h>
#include <gongdbvalidresult.h>
#include <gongregexp.h>
#include "contactosreccontacto.h"
/*>>>>>CUENTA_INCLUDES*/


namespace gong {
namespace contab {

/*<<<<<CUENTA_CONSTRUCTOR*/
class RecCuenta: public dbRecord
{
public:
    RecCuenta(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
        : dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("CUENTA"), recid, user)
/*>>>>>CUENTA_CONSTRUCTOR*/
    {
        init();
    }
    /*<<<<<CUENTA_MEMBERS*/
    virtual bool isValid(ValidResult::Context context, ValidResult *result=0); // from dbRecord
    Xtring toString(int format, const RegExp &includedFields=RegExp()) const;
    /*>>>>>CUENTA_MEMBERS*/
    void resetSaldos();
    static RecCuenta *quickCreate( dbConnection *conn, const Xtring &cuenta, const Xtring &desc );
    static dbRecordID findCuentaID( dbConnection *conn, const Xtring &cuenta );
    /*<<<<<CUENTA_RELATIONS*/
    contactos::RecContacto *getRecContacto() const;
    /*>>>>>CUENTA_RELATIONS*/
private:
    void init();
};

/*<<<<<CUENTA_POSTAMBLE*/
} // namespace contab
} // namespace gong
#endif // CONTAB_CONTABRECCUENTA_H
/*>>>>>CUENTA_POSTAMBLE*/

