/*<<<<<MODULE_INFO*/
// COPYLEFT Registro de apuntes del módulo de socias
// RELATION PartidaProyecto
// MEMBER save
// MEMBER remove
// Inherit contab::Apunte
// TYPE dbRecord socias::Apunte
/*>>>>>MODULE_INFO*/

/*<<<<<APUNTE_INCLUDES*/
#include "sociasrecapunte.h"
/*>>>>>APUNTE_INCLUDES*/
#ifdef HAVE_CONTABMODULE

namespace gong {
namespace socias {

int RecApunte::actImportesPartida(bool sumar, const dbRecordID partida_id, Money importeorig)
{
    int ret = 0;
    Money importe;
    if ( sumar )
        importe = importeorig;
    else
        importe = importeorig * -1L;
    RecPartidaProyecto partida(getConnection());
    if ( partida.read( partida_id ) )
    {
        partida.setValue( "IMPORTE", partida.getValue( "IMPORTE" ).toMoney() + importe );
        ret = partida.save(false);
        // Actualización recursiva de la partida madre
        Xtring madre;
        dbRecordID madre_id = 0;
        do {
            // Puede haber partidas que no tengan todas las partidas madres creadas
            madre = partida.getValue( "MADRE" ).toString();
            if( madre == partida.getValue( "CODIGO" ).toString() ) {
                _GONG_DEBUG_WARNING( "Madre = Codigo" );
                break;
            }
            if ( !madre.isEmpty() ) {
                madre_id = getConnection()->selectInt(
                               "SELECT id FROM PARTIDAPROYECTO WHERE codigo=" + getConnection()->toSQL( madre )
                               + " AND PROYECTO_ID=" + getConnection()->toSQL( partida.getValue( "PROYECTO_ID" ) ) );
            }
        }
        while ( !madre.isEmpty() && madre_id == 0 );
        if ( madre_id != 0 )
            ret += actImportesPartida(sumar, madre_id, importeorig );
    }
    return 1; // No se puede cancelar la grabación de un apunte porque haya cuentas sin madre
}


/*<<<<<APUNTE_SAVE*/
bool RecApunte::save(bool validate, bool saverelated = true) throw( dbError )
{
/*>>>>>APUNTE_SAVE*/
    // Es más importante grabar el apunte que actualizar los saldos de las cuentas
    bool wasnew = isNew();
    int ret = contab::RecApunte::save( false );
    if ( ret )
    {
        if ( !wasnew )
            actImportesPartida( false /*restar*/, getValue( "PARTIDAPROYECTO_ID" ).toInt(),
                                getValue( "DEBE" ).toMoney() - getValue( "HABER" ).toMoney() );
        actImportesPartida( true /*sumar*/, getValue( "PARTIDAPROYECTO_ID" ).toInt(),
                            getValue( "DEBE" ).toMoney() - getValue( "HABER" ).toMoney() );
    }
    return ret;
}

/*<<<<<APUNTE_REMOVE*/
bool RecApunte::remove() throw( dbError )
{
/*>>>>>APUNTE_REMOVE*/
    // Es más importante grabar el apunte que actualizar los saldos de las cuentas
    int ret = contab::RecApunte::remove();
    if ( ret )
        actImportesPartida( false /*restar*/, getValue( "PARTIDAPROYECTO_ID" ).toInt(),
                            getValue( "DEBE" ).toMoney() - getValue( "HABER" ).toMoney() );
    return ret;
}

/*<<<<<APUNTE_RELATIONS*/
RecPartidaProyecto *RecApunte::getRecPartidaProyecto() const
{
	return static_cast<RecPartidaProyecto*>(findRelatedRecord("APUNTE.PARTIDAPROYECTO_ID"));
}

/*>>>>>APUNTE_RELATIONS*/

} // namespace socias
} // namespace gong

#endif
