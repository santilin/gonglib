/*<<<<<COPYLEFT*/
/** @file empresafldcuentabanco.cpp dbFieldDefinition que representa una cuenta bancaria
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
// COPYLEFT dbFieldDefinition que representa una cuenta bancaria
// MEMBER isValid
// TYPE dbFieldDefinition empresa::CuentaBanco NoConstr
/*>>>>>MODULE_INFO*/

/*<<<<<DBFIELD_CUENTABANCO_INCLUDES*/
#include "empresafldcuentabanco.h"
/*>>>>>DBFIELD_CUENTABANCO_INCLUDES*/

namespace gong
{
namespace empresa {

/*<<<<<DBFIELD_CUENTABANCO_ISVALID*/
bool FldCuentaBanco::isValid( dbRecord *r, dbFieldValue *value, ValidResult::Context context, ValidResult *integres) const
{
/*>>>>>DBFIELD_CUENTABANCO_ISVALID*/
    if ( !dbFieldDefinition::isValid( r, value, context, integres ) )
        return false;
    Xtring cuenta = value->toString().trim();
    for ( unsigned int i = 0; i < cuenta.length(); i++ ) {
        if ( !isdigit( cuenta[i] ) ) {
            if( integres )
                integres->addError( "Las cuentas bancarias sólo pueden contener dígitos",
                                    getName() );
            return false;
        }
    }
    if ( !cuenta.isEmpty() ) {
        Xtring dc = cuenta.mid( 8, 2 );
        if ( dc.toInt() != calcDigitosControl( cuenta ) ) {
            if( integres )
                integres->addError(
                    Xtring::printf( "El dígito de control: %s es erróneo (debería ser %d)",
                                    dc.c_str(), calcDigitosControl( cuenta ) ), getName() );
            return false;
        }
    }
    return true;
}

/*
	http://www.bes.es/pdf/aeb/aeb_19.pdf
	Ejemplo correcto: 00720101930000122351
*/
int FldCuentaBanco::modulo11( const Xtring & digitos )
{
    static int pesos[] = {6, 3, 7, 9, 10, 5, 8, 4, 2, 1 };
    if ( digitos.length() != 8 && digitos.length() != 10 )
        return -10000;
    int peso = 0;
    for ( int i = digitos.length() - 1, p = 0; i >= 0; --i, ++p )
        peso += int( digitos[i] - '0' ) * pesos[p];
    peso = 11 - ( peso % 11 );
    if ( peso == 10 )
        peso = 1;
    else if ( peso == 11 )
        peso = 0;
    return peso;
}

int FldCuentaBanco::calcDigitosControl( const Xtring & cuenta )
{
    int dig1 = modulo11( cuenta.left( 8 ) );
    int dig2 = modulo11( cuenta.mid( 10 ) );
    return dig1 * 10 + dig2;
}


} // Namespace empresa
} // Namespace gong
