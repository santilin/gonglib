/*<<<<<COPYLEFT*/
/** @file contabreccuenta.cpp Registro de cuentas
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */
/*>>>>>COPYLEFT*/

/*<<<<<MODULE_INFO*/
// COPYLEFT Registro de cuentas
// MEMBER isValid
// MEMBER toString
// RELATION contactos::Contacto
// TYPE dbRecord contab::Cuenta
/*>>>>>MODULE_INFO*/

#include <dbappdbapplication.h>
#include "contabmodule.h"
#include "contabcuenta.h"

/*<<<<<CUENTA_INCLUDES*/
#include "contabreccuenta.h"
/*>>>>>CUENTA_INCLUDES*/

namespace gong {
namespace contab {

void RecCuenta::init()
{
    addStructuralFilter( "CUENTA.EJERCICIO=" + getConnection()->toSQL( empresa::ModuleInstance->getEjercicio() ) );
    addStructuralFilter( "CUENTA.EMPRESA_ID=" + getConnection()->toSQL( empresa::ModuleInstance->getEmpresaID() ) );
}

void RecCuenta::resetSaldos()
{
    setValue("SALDO", 0.0 );
    setValue("DEBE", 0.0 );
    setValue("HABER", 0.0 );
}

/*<<<<<CUENTA_ISVALID*/
bool RecCuenta::isValid(ValidResult::Context context, ValidResult *result )
{
    bool ret = dbRecord::isValid(context, result);
    /*>>>>>CUENTA_ISVALID*/
    if( context == ValidResult::fixing ) {
        Cuenta cuenta(getValue("CUENTA").toString(), ModuleInstance->getDigitosTrabajo() );
        cuenta.expandir();
        setValue("CUENTA", cuenta);
    }
    return ret;
}

/*<<<<<CUENTA_TOSTRING*/
Xtring RecCuenta::toString(int format, const RegExp &includedFields) const
{
    Xtring result;
    /*>>>>>CUENTA_TOSTRING*/
#if 0
    if( format == TOSTRING_USER ) {
        result = "(id=" + Xtring::number( getRecordID() ) + "), " + getValue( "CUENTA" ).toString()
                 + ", " + getValue( "DESCRIPCION" ).toString();
    } else
#endif
        result = dbRecord::toString(format, includedFields);
    return result;
}

/*<<<<<CUENTA_RELATIONS*/
contactos::RecContacto *RecCuenta::getRecContacto() const
{
    return static_cast<contactos::RecContacto*>(findRelatedRecord("CUENTA.CONTACTO_ID"));
}

/*>>>>>CUENTA_RELATIONS*/

RecCuenta* RecCuenta::quickCreate(dbConnection* conn, const Xtring& cuenta, const Xtring& desc)
{
    RecCuenta *reccuenta = static_cast<RecCuenta *>( DBAPP->createRecord("CUENTA") );
    Cuenta c(cuenta, contab::ModuleInstance->getDigitosTrabajo() );
    c.expandir();
    if( !reccuenta->readWithFilter( "CUENTA=" + conn->toSQL( c ) ) ) {
        reccuenta->clear(true);
        reccuenta->setValue("CUENTA", c);
        reccuenta->setValue("DESCRIPCION", desc);
        reccuenta->save( false );
    }
    return reccuenta;
}

dbRecordID RecCuenta::findCuentaID(dbConnection* conn, const Xtring& cuenta)
{
    dbRecordID ret = 0;
    RecCuenta *reccuenta = static_cast<RecCuenta *>( DBAPP->createRecord("CUENTA") );
    Cuenta c(cuenta, contab::ModuleInstance->getDigitosTrabajo() );
    c.expandir();
    if( reccuenta->readWithFilter( "CUENTA=" + conn->toSQL( c ) ) )
        ret = reccuenta->getRecordID();
    delete reccuenta;
    return ret;
}


} // namespace contab
} // namespace gong
