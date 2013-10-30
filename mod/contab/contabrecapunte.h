/*<<<<<COPYLEFT*/
/** @file contabrecapunte.h Registro de apuntes
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
/*<<<<<APUNTE_PREAMBLE*/
#ifndef _CONTAB_CONTABRECAPUNTE_H
#define _CONTAB_CONTABRECAPUNTE_H
/*>>>>>APUNTE_PREAMBLE*/

#include "gongmoney.h"
/*<<<<<APUNTE_INCLUDES*/
#include <dbappdbapplication.h>
#include <gongdbvalidresult.h>
#include <gongregexp.h>
#include "contabreccuenta.h"
/*>>>>>APUNTE_INCLUDES*/
#include "contabcuenta.h"
#include "contabreccuenta.h"

namespace gong {
namespace contab {

/*<<<<<APUNTE_CONSTRUCTOR*/
class RecApunte: public dbRecord
{
public:
    RecApunte(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
        : dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("APUNTE"), recid, user)
/*>>>>>APUNTE_CONSTRUCTOR*/
    {}

    /*<<<<<APUNTE_MEMBERS*/
    virtual bool isValid(ValidResult::Context context, ValidResult *result=0); // from dbRecord
    Xtring toString(int format, const RegExp &includedFields=RegExp()) const;
    bool isEmpty( const Xtring &nocheck_fields = Xtring::null ) const; // from dbRecord
    virtual void beforeSaveOrDeleteRelated(dbRecord *master, bool saving); // from dbRecord
    /*>>>>>APUNTE_MEMBERS*/
    /*<<<<<APUNTE_RELATIONS*/
    RecCuenta *getRecCuenta() const;
    /*>>>>>APUNTE_RELATIONS*/
    void quickValues( Cuenta cuenta, const Xtring &desc_cuenta, Money debe, Money haber,
                      const Xtring &concepto, const Xtring &contrapartida );
    bool actSaldosCuenta(bool sumar, const dbRecordID cuenta_id,
                         int ejercicio, Money debeorig, Money haberorig) const;
}; // class

/*<<<<<APUNTE_POSTAMBLE*/
} // namespace contab
} // namespace gong
#endif // CONTAB_CONTABRECAPUNTE_H
/*>>>>>APUNTE_POSTAMBLE*/
