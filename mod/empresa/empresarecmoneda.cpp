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
