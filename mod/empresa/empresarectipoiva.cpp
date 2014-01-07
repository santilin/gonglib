/*<<<<<MODULE_INFO*/
// COPYLEFT Registro de tipos de IVA
// TYPE dbRecord empresa::TipoIVA
/*>>>>>MODULE_INFO*/

#include "empresamodule.h"
/*<<<<<TIPOIVA_INCLUDES*/
#include "empresarectipoiva.h"
/*>>>>>TIPOIVA_INCLUDES*/


/* Tipos de iva por defecto:
 * General: 18,0%, 4,0%
 * Reducido: 8,0%, 1,0%
 * Superreducido: 4,0%, 0,5%
 */

namespace gong {
namespace empresa {

Money RecTipoIVA::calcIVA(Money cantidad) const
{
    return cantidad * getValue("IVA").toDouble() / 100;
}

Money RecTipoIVA::calcRecargo(Money cantidad) const
{
    return cantidad * getValue("RECARGO").toDouble() / 100;
}

Money RecTipoIVA::calcIVARecargo(Money cantidad) const
{
    return cantidad * (getValue("IVA").toDouble() + getValue("RECARGO").toDouble()) / 100;
}

Money RecTipoIVA::masIVA(Money cantidad) const
{
    return cantidad + (cantidad * getValue("IVA").toDouble() / 100);
}

Money RecTipoIVA::masRecargo(Money cantidad) const
{
    return cantidad + (cantidad * getValue("RECARGO").toDouble() / 100);
}

Money RecTipoIVA::masIVARecargo(Money cantidad) const
{
    return cantidad + (cantidad * (getValue("IVA").toDouble() + getValue("RECARGO").toDouble()) / 100);
}

Money RecTipoIVA::menosIVA(Money cantidad) const
{
    if( getValue("IVA").toDouble() != -100.0 )
        return cantidad / (1 + getValue("IVA").toDouble() / 100);
    else
        return 0.0;
}


Money RecTipoIVA::menosRecargo(Money cantidad) const
{
    if( getValue("RECARGO").toDouble() != -100.0 )
        return cantidad / (1 + getValue("RECARGO").toDouble() / 100);
    else
        return 0.0;
}

Money RecTipoIVA::menosIVARecargo(Money cantidad) const
{
    if( (getValue("IVA").toDouble() + getValue("RECARGO").toDouble() ) != -100.0 )
        return cantidad / (1 + (getValue("IVA").toDouble() + getValue("RECARGO").toDouble() ) / 100);
    else
        return 0.0;
}

} // namespace empresa
} // namespace gong
