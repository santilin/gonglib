/*<<<<<COPYLEFT*/
/** @file empresarecmoneda.cpp Registro de monedas
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
// COPYLEFT Registro de monedas
// TYPE dbRecord empresa::Moneda
/*>>>>>MODULE_INFO*/

#include "empresamodule.h"
/*<<<<<MONEDA_INCLUDES*/
#include "empresarecmoneda.h"
/*>>>>>MONEDA_INCLUDES*/

namespace gong {
namespace empresa {

void RecMoneda::setRegConfigFromValues(RegConfig& regconfig) const
{
    regconfig.setCurrencyFormat(
        getValue( "SIMBOLO" ).toString(),
        getValue( "DECIMALES" ).toInt(),
        getValue( "PUNTODECIMAL" ).toString().isEmpty()
        ? '.' : getValue( "PUNTODECIMAL" ).toString().at( 0 ),
        getValue( "SEPARADORMILES" ).toString().isEmpty()
        ? ',' : getValue( "SEPARADORMILES" ).toString().at( 0 ) );
//			rc.setNCSPrecedes( false ); /// \todo {0.3.4} Tomar estos valores de RecEmpresa
//			rc.setPCSPrecedes( false );

}

void RecMoneda::setValuesFromRegConfig(const RegConfig& regconfig)
{
    setValue( "SIMBOLO", regconfig.getCurrencySymbol() );
    setValue( "DECIMALES", regconfig.getCurrencyFracDigits() );
    setValue( "PUNTODECIMAL", regconfig.getCurrencyDecimalPoint() );
    setValue( "SEPARADORMILES", regconfig.getCurrencyThousandsSep() );
}


} // namespace empresa
} // namespace gong
