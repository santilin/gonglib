#include <pagosreccobro.h>
#include "sociascobrobehavior.h"
#include "sociasrecmiembro.h"

namespace gong {
namespace socias {

CobroBehavior::CobroBehavior()
    : dbRecordBehavior()
{
}

bool CobroBehavior::save(dbRecord* therecord, bool is_pre, bool& result)
{
    if( !is_pre ) {
        pagos::RecCobro *cobro = static_cast<pagos::RecCobro *>( therecord );
        RecMiembro *miembro = static_cast<RecMiembro *>( cobro->getRecFactura() );
        if( miembro ) {
            Money old_importe = cobro->getOrigValue( "IMPORTE" ).toMoney();
            _GONG_DEBUG_PRINT(0, "Old importe al grabar: " + old_importe.toString() );
            Money new_importe = cobro->getValue( "IMPORTE" ).toMoney();
            Money importe = miembro->getValue( "IMPORTETOTAL" ).toMoney()
                            - old_importe + new_importe;
            miembro->setValue( "IMPORTETOTAL", importe );
            Money old_resto = cobro->getOrigValue( "RESTO" ).toMoney();
            Money new_resto = cobro->getValue( "RESTO" ).toMoney();
            miembro->setValue( "RESTOTOTAL", miembro->getValue( "RESTOTOTAL" ).toMoney()
                               - old_resto + new_resto );
            miembro->setValue( "PAGOSTOTAL", miembro->getValue( "IMPORTETOTAL" ).toMoney()
                               - miembro->getValue( "RESTOTOTAL" ).toMoney() );
            miembro->save( false );
        }
    }
    return true;
}

bool CobroBehavior::remove(dbRecord* therecord, bool is_pre)
{
    if( !is_pre ) {
        pagos::RecCobro *cobro = static_cast<pagos::RecCobro *>( therecord );
        RecMiembro *miembro = static_cast<RecMiembro *>( cobro->getRecFactura() );
        if( miembro ) {
            miembro->setValue( "IMPORTETOTAL", miembro->getValue( "IMPORTETOTAL" ).toMoney() -
                               cobro->getOrigValue( "IMPORTE" ).toMoney() );
            _GONG_DEBUG_PRINT(0, "Old importe al borrar: " + cobro->getOrigValue( "IMPORTE" ).toMoney().toString() );
            miembro->setValue( "RESTOTOTAL", miembro->getValue( "RESTOTOTAL" ).toMoney() -
                               cobro->getOrigValue( "RESTO" ).toMoney() );
            miembro->setValue( "PAGOSTOTAL", miembro->getValue( "PAGOSTOTAL" ).toMoney() -
                               cobro->getOrigValue( "PAGOS" ).toMoney() );
            miembro->save( false );
        }
    }
    return true;
}

} // namespace socias
} // namespace gong


