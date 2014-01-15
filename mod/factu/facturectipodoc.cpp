/*<<<<<MODULE_INFO*/
// COPYLEFT Registro de tipos de documentos
// RELATION pagos::FormaPago
// RELATION empresa::Proyecto
// RELATION Agente
// TYPE dbRecord factu::TipoDoc
/*>>>>>MODULE_INFO*/

/*<<<<<TIPODOC_INCLUDES*/
#include "facturectipodoc.h"
/*>>>>>TIPODOC_INCLUDES*/
#undef _FORTIFY_SOURCE

namespace gong {
namespace factu {

/*<<<<<TIPODOC_RELATIONS*/
pagos::RecFormaPago *RecTipoDoc::getRecFormaPago() const
{
	return static_cast<pagos::RecFormaPago*>(findRelatedRecord("FORMAPAGO_ID"));
}

empresa::RecProyecto *RecTipoDoc::getRecProyecto() const
{
	return static_cast<empresa::RecProyecto*>(findRelatedRecord("PROYECTO_ID"));
}

RecAgente *RecTipoDoc::getRecAgente() const
{
	return static_cast<RecAgente*>(findRelatedRecord("AGENTE_ID"));
}

/*>>>>>TIPODOC_RELATIONS*/


bool RecTipoDoc::generaAsientos() const
{
    return getValue( "NOGENERAASIENTO" ).toBool() == false;
}

/**
 * @brief Formatea un número de documento según el formato
 *
 * No puedo usar printf para esto porque no se pueden saltar los parámetros posicionales
 *	// https://wiki.ubuntu.com/ToolChain/CompilerFlags
 *
 * @param codempresa ...
 * @param ejercicio ...
 * @param numero ...
 * @param codagente ...
 * @param formato {emp:x}{eje:x}{num:x}{age:x}
 * @return Xtring
 **/
Xtring RecTipoDoc::formatNumDocumento(int codempresa, int ejercicio,
                                      int numero, int codagente, const Xtring& formato)
{
    if( formato.isEmpty() )
        return Xtring::number( numero );
    else {
        Xtring result;
        for( uint i = 0; i < formato.size(); ++ i ) {
            if( formato[i] == '{' ) {
                ++i;
                Xtring fieldandsize, field, size;
                while( formato[i] != '}' && i < formato.size() )
                    fieldandsize += formato[i++];
                fieldandsize.splitIn2( field, size, ":" );
                Xtring bit;
                if( field == "eje" || field == "ejercicio" ) {
                    bit = Xtring::number( ejercicio );
                } else if( field == "emp" || field == "empresa" || field == "codemp" ) {
                    bit = Xtring::number( codempresa );
                } else if( field == "num"  ) {
                    bit = Xtring::number( numero );
                } else if( field == "age" || field == "agente" ) {
                    bit = Xtring::number( codagente);
                } else {
                    _GONG_DEBUG_WARNING( "Field " + field + " not found" );
                    result += fieldandsize;
                    continue;
                }
                if( size.toInt() != 0 )
                    bit.padL( size.toInt(), '0' );
                result += bit;
            } else {
                result += formato[i];
            }
        }
        return result;
    }
}


} // namespace factu
} // namespace gong
