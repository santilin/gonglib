/*<<<<<MODULE_INFO*/
// COPYLEFT Registro de Contactos
// RELATION ContactoImagen
// MEMBER isValid
// MEMBER toString
// MEMBER findMatchingRecord
// MEMBER save
// TYPE dbRecord contactos::Contacto
/*>>>>>MODULE_INFO*/

#include <gongfileutils.h>
#include "contactosmodule.h"
/*<<<<<CONTACTO_INCLUDES*/
#include "contactosreccontacto.h"
/*>>>>>CONTACTO_INCLUDES*/

namespace gong {
namespace contactos {

Xtring RecContacto::sLocalidadesLineas;
Xtring RecContacto::sCallesLineas;
Dictionary<Xtring> RecContacto::sProvincias;

const char *notocar[] = { "a", "al", "de", "del", "la", "y", "las", "los" };

Xtring normalizaPalabra(const Xtring &palabra, int npalabra)
{
    Xtring normalizada = palabra.lower();
    if( npalabra > 0 ) {
        for( unsigned int i=0; i < sizeof(notocar)/sizeof(notocar[0]); i++ ) {
            if( normalizada == notocar[i] ) {
                return normalizada;
            }
        }
    }
    // Proper, pero si hay guiones, hacer proper también de la segunda palabra
    XtringList conguiones;
    normalizada.tokenize( conguiones, "-");
    normalizada.clear();
    for( XtringList::const_iterator it = conguiones.begin(); it != conguiones.end(); ++it ) {
        if( !normalizada.isEmpty() )
            normalizada += "-";
        normalizada += (*it).proper();
    }
    return normalizada;
}


/*<<<<<CONTACTO_RELATIONS*/
RecContactoImagen *RecContacto::getRecContactoImagen() const
{
	return static_cast<RecContactoImagen*>(findRelatedRecord("CONTACTO.CONTACTOIMAGEN_ID"));
}

/*>>>>>CONTACTO_RELATIONS*/

/*<<<<<CONTACTO_ISVALID*/
bool RecContacto::isValid(ValidResult::Context context, ValidResult *result )
{
	bool ret = dbRecord::isValid(context, result);
/*>>>>>CONTACTO_ISVALID*/
    if( context == ValidResult::editing || context == ValidResult::fixing ) {
        // Las funciones normaliza actúan según la configuración
        if( getValue("TRATAMIENTOCONTACTO").toInt() == RecContacto::TratamientoDona
                || getValue("TRATAMIENTOCONTACTO").toInt() == RecContacto::TratamientoDon ) {
            setValue("NOMBRE", normalizaNombre(getValue("NOMBRE").toString()));
        }
        setValue("BARRIO", normalizaNombre(getValue("BARRIO").toString()));
        setValue("LOCALIDAD", normalizaNombre(getValue("LOCALIDAD").toString()));
        setValue("PROVINCIA", normalizaNombre(getValue("PROVINCIA").toString()));
    }
    return ret;
}

/*<<<<<CONTACTO_TOSTRING*/
Xtring RecContacto::toString(int format, const RegExp &includedFields) const
{
	Xtring result;
/*>>>>>CONTACTO_TOSTRING*/
    if( format == TOSTRING_USER ) {
        result = "(id=" + Xtring::number ( getRecordID() ) + "), ";
        if( !getValue("CIF").toString().isEmpty() )
            result += getValue( "CIF" ).toString() + ",";
        result += getValue("NOMBRE").toString();
    } else
        result = dbRecord::toString(format, includedFields);
    return result;
}

/*<<<<<CONTACTO_FINDMATCHINGRECORD*/
bool RecContacto::findMatchingRecord(dbRecordID *matchingid) const
{
	bool found;
/*>>>>>CONTACTO_FINDMATCHINGRECORD*/
    found = dbRecord::findMatchingRecord(matchingid);
    if( !found ) {
        Xtring nombre = getValue("NOMBRE").toString();
        // Añadir comas al nombre para ver si es que lo tenemos grabado con comas y aquí no tiene comas
        // En MySQL, la búsqueda no tiene en cuenta los acentos
        for( unsigned int i = nombre.length(); i > 0; --i ) {
            if( nombre[i] == ' ' ) {
                Xtring concoma = nombre.mid(0, i) + "," + nombre.mid(i);
                if( (*matchingid = existsAnother("NOMBRE", concoma) ) ) {
                    const_cast<RecContacto *>(this)->setValue("NOMBRE", concoma);
                    return true;
                }
            }
        }
    }
    return found;
}

/*<<<<<CONTACTO_SAVE*/
bool RecContacto::save(bool saverelated) throw( dbError )
{
/*>>>>>CONTACTO_SAVE*/
    switch( getValue( "TRATAMIENTOCONTACTO" ).toInt() ) {
    case RecContacto::TratamientoDona:
    case RecContacto::TratamientoDon:
    {
        Xtring nombre = getValue("NOMBRE").toString();
        Xtring nombrecompleto;
        size_t poscoma = nombre.find("," );
        if( poscoma != Xtring::npos ) {
            nombrecompleto = nombre.mid( poscoma + 1 ).trim()
                             + " " + nombre.left( poscoma ).trim();
            setValue( "NOMBRECOMPLETO", nombrecompleto );
        }
    }
    break;
    default:
        setValue( "NOMBRECOMPLETO", getValue("NOMBRE") );
        break;
    }
    int ret = dbRecord::save(saverelated);
    return ret;
}


Xtring RecContacto::creaCIFProvisional( dbConnection* conn )
{
    Xtring ret = conn->selectString("SELECT MAX(LEFT(CIF,8)) FROM CONTACTO WHERE CIF LIKE '0000%' ORDER BY 1");
    if( ret.isEmpty() )
        ret = "00000000";
    else
        ret = ret.stringInc( ret );
    Cif c( ret );
    c += c.calcularCharControl();
    return c;
}

bool RecContacto::findCodigosPostales(const Xtring& cp, const Xtring &barrio,
                                      const Xtring& localidad, XtringList &localidades)
{
    if( sLocalidadesLineas.isEmpty() )
        loadLocalidades();
    localidades.clear();
    Xtring search = localidad;
    if( !search.isEmpty() && !cp.isEmpty() )
        search += ";" + cp;
    else if( !cp.isEmpty() )
        search = cp;
    if( !search.isEmpty() ) {
        // Vitoria-Gasteiz;01001;ABREVADERO
        const char *haystack = sCallesLineas.c_str();
        const char *p = haystack;
        const char *needle = search.c_str();
        while( p && *p ) {
            bool valido = true;
            p = strcasestr( p, needle );
            if( p ) {
                const char *pos_ini = p;
                while( pos_ini > haystack && *pos_ini != '\n')
                    --pos_ini;
                if( p - pos_ini > 1 ) // No está al inicio de la linea
                    valido = false;
                while( *p && *p != '\n')
                    ++p;
                if( valido )
                    localidades << Xtring( pos_ini+1, p-pos_ini-1 );
            }
        }
    }
    search = cp;
    if( !search.isEmpty() && !localidad.isEmpty() )
        search += ";" + localidad;
    else if( !localidad.isEmpty() )
        search = localidad;
    if( !search.isEmpty() ) {
        // 01007;ARMENTIA
        const char *haystack = sLocalidadesLineas.c_str();
        const char *p = haystack;
        const char *needle = search.c_str();
        while( p && *p ) {
            p = strcasestr( p, needle );
            if( p ) {
                const char *pos_ini = p;
                while( pos_ini > haystack && *pos_ini != '\n')
                    --pos_ini;
                while( *p && *p != '\n')
                    ++p;
                localidades << Xtring( pos_ini+1, p-pos_ini-1 );
            }
        }
    }
    return localidades.size() != 0;
}

bool RecContacto::loadLocalidades()
{
    Xtring provincias = FileUtils::readFile( ModuleInstance->getModuleGlobalDataDir() + "provincias.txt" );
    const char *p = provincias.c_str();
    while( *p ) {
        Xtring cp;
        while( *p != ';' && *p != '\n' && *p != '\0' )
            cp += *p++;
        p++;
        Xtring provincia;
        while( *p != ';' && *p != '\n' && *p != '\0' )
            provincia += *p++;
        p++;
        sProvincias.insert( cp, provincia );
    }
    sCallesLineas = FileUtils::readFile( ModuleInstance->getModuleGlobalDataDir() + "calles.txt" );
    sLocalidadesLineas = FileUtils::readFile( ModuleInstance->getModuleGlobalDataDir() + "codigospostales.txt" );
    return !sLocalidadesLineas.isEmpty();
}

Xtring RecContacto::getProvinciaFromCp(const Xtring& cp)
{
    if( sProvincias.size() == 0 )
        loadLocalidades();
    return sProvincias[ cp.trim().left(2) ];
}

bool RecContacto::slotSearchCPClicked( FrmBase *frm, LineEdit* editCP, LineEdit *editBarrio,
                                       LineEdit* editLocalidad, LineEdit* editProvincia)
{
    DBAPP->waitCursor(true);
    Xtring cp = editCP->toString().trim();
    Xtring barrio = editBarrio->toString().trim();
    Xtring localidad = editLocalidad->toString().trim();
    if( !cp.isEmpty() || !localidad.isEmpty() ) {
        XtringList localidades;
        if( RecContacto::findCodigosPostales( cp, barrio, localidad, localidades ) ) {
            int choosenindex = 0;
            if( localidades.size() > 1 )
                choosenindex = frm->msgXtringList( frm,
                                                   _("Elige el código postal y la localidad"), localidades );
            if( choosenindex != -1 ) {
                localidad = localidades[choosenindex];
                XtringList loc_items;
                localidad.tokenize( loc_items, ";" );
                if( loc_items.size() == 3 ) { // Vitoria-Gasteiz;01001;ABREVADERO
                    localidad = loc_items[0];
                    cp = loc_items[1];
                } else { // 01007;ARMENTIA
                    localidad = loc_items[1];
                    cp = loc_items[0];
                }
                size_t posparini = localidad.find("(");
                size_t posparfin = localidad.find(")");
                if(  posparini != Xtring::npos && posparfin != Xtring::npos ) {
                    barrio = localidad.left( posparini );
                    localidad = localidad.mid( posparini + 1, posparfin - posparini - 1 );
                }
                if( barrio.find(",") != Xtring::npos ) {
                    Xtring antes, despues;
                    barrio.splitIn2( antes, despues, "," );
                    barrio = despues.trim().proper() + " " + antes;
                }
                if( localidad.find(",") != Xtring::npos ) {
                    Xtring antes, despues;
                    localidad.splitIn2( antes, despues, "," );
                    localidad = despues.trim().proper() + " " + antes;
                }
                editBarrio->setText( RecContacto::normalizaNombre(barrio) );
                editLocalidad->setText( RecContacto::normalizaNombre(localidad) );
                editCP->setText( cp );
                editProvincia->setText( RecContacto::normalizaNombre(RecContacto::getProvinciaFromCp( cp ) ) );
                DBAPP->resetCursor();
                return true;
            }
        } else {
            DBAPP->showOSD(  _("No se ha encontrado ningún código postal para:"),
                             cp + " " + localidad + " " );
        }
    }
    DBAPP->resetCursor();
    return false;
}

Xtring RecContacto::normalizaEmpresa(const Xtring& nombre, bool force )
{
    int formato = ModuleInstance->getModuleSetting("FORMATO_EMPRESA").toInt();
    if( formato == 0 && !force )
        return nombre;
    switch( formato ) {
    case 1: // Programación Social, S.Coop
		formato = 1;
        break;
    case 2: // PROGRAMACIÓN SOCIAL, S.Coop
		formato = 3;
        break;
    }
    Xtring ret = normalizaNombreOEmpresa(formato, nombre, force);
    size_t pos = ret.find_last_of(',');
    if( pos == Xtring::npos )
        pos = ret.find_last_of(' ');
    if( pos != Xtring::npos ) {
        Xtring coletilla = ret.mid( pos + 1 ).upper().replace(".","").replace(" ","").replace(",","");
        if( coletilla == "SA" )
            ret = ret.mid(0, pos) + ", S.A.";
        else if( coletilla == "SL" )
            ret = ret.mid(0, pos) + ", S.L.";
        else if( coletilla == "SLL" )
            ret = ret.mid(0, pos) + ", S.L.L.";
        else if( coletilla == "SRL" )
            ret = ret.mid(0, pos) + ", S.R.L.";
        else if( coletilla == "SC" )
            ret = ret.mid(0, pos) + ", S.C.";
        else if( coletilla == "SRC" )
            ret = ret.mid(0, pos) + ", S.R.C.";
        else if( coletilla == "COOP" )
            ret = ret.mid(0, pos) + ", S. Coop.";
        else if( coletilla == "S.COOP" )
            ret = ret.mid(0, pos) + ", S. Coop.";
        else if( coletilla == "S.COOP." )
            ret = ret.mid(0, pos) + ", S. Coop.";
        else if( coletilla == "S COOP" )
            ret = ret.mid(0, pos) + ", S. Coop.";
		// TODO Usar expresiones regulares
    }
    if( formato == 2 )
        ret = ret.upper();
    return ret;
}

Xtring RecContacto::normalizaNombre(const Xtring& nombre, bool force)
{
    int formato = ModuleInstance->getModuleSetting("FORMATO_NOMBRE").toInt();
	return normalizaNombreOEmpresa( formato, nombre, force );
}


Xtring RecContacto::normalizaNombreOEmpresa(int formato, const Xtring &nombre, bool force)
{
    if( formato == 0 && !force )
        return nombre;
    Xtring normalizado = nombre;
    normalizado.simplify_white_space();
    XtringList palabras;
    normalizado.tokenize( palabras, " " );
    Xtring ret;
    int contador = 0;
    for( XtringList::const_iterator it = palabras.begin();
            it != palabras.end(); ++ it, ++ contador ) {
        if( contador != 0 )
            ret += " ";
        ret += normalizaPalabra( *it, contador );
    }
    switch( formato ) {
    case 1: // Nombre Apellidos
        break;
    case 2: // Nombre, Apellidos
        break;
    case 3: // NOMBRE APELLIDOS
    case 4: // APELLIDOS, NOMBRE
        ret = ret.upper();
        break;
    }
    return ret;
}

RecContacto::Tratamiento RecContacto::tratamientoFromNombre(const Xtring& nombre)
{
    // Primero ver si es una empresa
    Xtring empresa = normalizaEmpresa( nombre, true ); // force
    if( empresa.endsWith( ", S.A.") )
        return TratamientoEmpresa;
    if( empresa.endsWith( ", S.L." ) )
        return TratamientoEmpresa;
    if( empresa.endsWith( ", S.L.L." ) )
        return TratamientoEmpresa;
    if( empresa.endsWith( ", S.R.L." ) )
        return TratamientoEmpresa;
    if( empresa.endsWith( ", S.C." ) )
        return TratamientoEmpresa;
    if( empresa.endsWith( ", S.R.C." ) )
        return TratamientoEmpresa;
    if( empresa.endsWith( ", S. Coop." ) )
        return TratamientoEmpresa;
    else
        return TratamientoNo;
}

RecContacto::Tratamiento RecContacto::tratamientoFromCIF(const Xtring& cif)
{
    Xtring _cif = cif.trim();
    if( !_cif.isEmpty() ) {
        char ch = cif[0];
        switch( ch ) {
        case 'J':
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'H':
        case 'I':
            return TratamientoEmpresa;
        case 'F':
            return TratamientoCooperativa;
        case 'G':
            return TratamientoAsociacion;
        default:
            return TratamientoNo;
        }
    }
    return TratamientoNo;
}

} // namespace contactos
} // namespace gong

