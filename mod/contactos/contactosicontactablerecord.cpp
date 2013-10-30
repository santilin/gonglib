#include <gongdbrecord.h>
#include <dbappdbapplication.h>
#include "contactosmodule.h"
#include "contactosicontactablerecord.h"

namespace gong {
namespace contactos {

Xtring IContactableRecord::fullAddress( const Xtring &format ) const
{
    Xtring address = format;
    if( address.isEmpty() ) {
        address = ModuleInstance->getModuleSetting( "FORMATO_DIRECCION" ).toString();
    }
    address.replace( "{cp}", pRecord->getValue("CONTACTO.CP").toString() );
    address.replace( "{direccion}", pRecord->getValue("CONTACTO.DIRECCION").toString() );
    address.replace( "{barrio}", pRecord->getValue("CONTACTO.BARRIO").toString() );
    address.replace( "{localidad}", pRecord->getValue("CONTACTO.LOCALIDAD").toString() );
    address.replace( "{provincia}", pRecord->getValue("CONTACTO.PROVINCIA").toString() );
    address.replace( "{apartadocorreos}", pRecord->getValue("CONTACTO.APDOCORREOS").toString() );
    address.replace( "{pais}", pRecord->getValue("CONTACTO.PAIS").toString() );
    address.replace( "{telefono}", pRecord->getValue("CONTACTO.TELEFONO").toString() );
    address.replace( "{fax}", pRecord->getValue("CONTACTO.FAX").toString() );
    address.replace( "{email}", pRecord->getValue("CONTACTO.EMAIL").toString() );
    address.replace( "{web}", pRecord->getValue("CONTACTO.WEB").toString() );
    return address;
}


} // namespace contactos
} // namespace gong
