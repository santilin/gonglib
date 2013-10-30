/*<<<<<MODULE_INFO*/
// COPYLEFT dbFieldDefinition que representa un cif
// MEMBER isValid
// TYPE dbFieldDefinition contactos::Cif
/*>>>>>MODULE_INFO*/

#include <ctype.h>
/*<<<<<DBFIELD_CIF_INCLUDES*/
#include "contactosfldcif.h"
/*>>>>>DBFIELD_CIF_INCLUDES*/

namespace gong {
namespace contactos {

Cif::Cif() : Xtring()
{
}

Cif::~Cif()
{
}

Cif::Cif(const Xtring &cif)
    : Xtring(cif)
{
}


Cif::Cif(const char *cif)
    : Xtring(cif)
{
}

char Cif::calcularCharControl() const
{
    // Debe tener una longitud igual a 9 caracteres;
    if (length() < 8 ) return '\0';
    char firstChar = at(0);
    if( isdigit( firstChar ) || firstChar == 'X' || firstChar == 'Y' || firstChar == 'Z' ) {
        static char letras[] = {'T', 'R', 'W', 'A', 'G', 'M',
                                'Y', 'F', 'P', 'D', 'X', 'B',
                                'N', 'J', 'Z', 'S', 'Q', 'V',
                                'H', 'L', 'C', 'K', 'E'
                               };
        int valor = 0;
        for ( unsigned int i=0; i<length(); i++ )
        {
            char ch = at(i);
            if( i==0 && !isdigit(ch) ) {
                switch( ch ) {
                case 'X':
                    valor = 0;
                    break;
                case 'Y':
                    valor = 1;
                    break;
                case 'Z':
                    valor = 2;
                    break;
                default:
                    return '\0';
                }
            }
            if ( isdigit(ch) )
                valor = (valor * 10) + int (ch-'0');
        }
        return letras[valor % 23];
    } else {
        long sumaPar = 0, sumaImpar = 0;
        // A continuación, la cadena debe tener 7 dígitos + el dígito de control.
        Xtring digits = mid(1, 7);
        for( uint n = 0; n <= digits.length() - 1; n += 2 )
        {
            if (n < 6)
            {
                // Sumo las cifras pares del número que se corresponderá
                // con los caracteres 1, 3 y 5 de la variable «digits».
                //
                sumaPar += int(digits[n+1]) - int('0');
            }
            // Multiplico por dos cada cifra impar (caracteres 0, 2, 4 y 6).
            //
            long dobleImpar = 2 * (int(digits[n]) - int('0'));
            // Acumulo la suma del doble de números impares.
            //
            sumaImpar += (dobleImpar % 10) + (dobleImpar / 10);
        }
        // Sumo las cifras pares e impares.
        //
        long sumaTotal = sumaPar + sumaImpar;
        // Me quedo con la cifra de las unidades y se la resto a 10, siempre
        // y cuando la cifra de las unidades sea distinta de cero
        //
        sumaTotal = (10 - (sumaTotal % 10)) % 10;
        // Devuelvo el Dígito de Control dependiendo del primer carácter
        // del NIF pasado a la función.
        //
        char digitoControl = '\0';
        switch (firstChar)
        {
        case 'N':
        case 'P':
        case 'Q':
        case 'R':
        case 'S':
        case 'W':
            // NIF de entidades cuyo dígito de control se corresponde
            // con una letra.
            //
            // Al estar los índices de los arrays en base cero, el primer
            // elemento del array se corresponderá con la unidad del número
            // 10, es decir, el número cero.
            //
            static const char *characters = "JABCDEFGHI";
            digitoControl = *(characters + sumaTotal);
            break;
        default:
            // NIF de las restantes entidades, cuyo dígito de control es un número.
            //
            digitoControl = sumaTotal + int('0');
            break;
        }
        return digitoControl;
    }
}

void Cif::eliminarCharControl()
{
    if( length() > 8 )
        *this = mid(0, 8 );
}

Xtring Cif::validate( RecContacto::Tratamiento tratamiento, bool fixme )
{
    Xtring ret;
    unsigned int i;
    if( fixme ) {
        Xtring fixedcif;
        // Eliminar caracteres incorrectos
        for ( uint i = 0; i < length(); i++ ) {
            char ch = at(i);
            if( i == 0 || i == length()-1 ) {
                if( isalnum( ch ) ) {
                    fixedcif += toupper( ch );
                } else if( ch == '-' ) {
                    fixedcif += calcularCharControl();
                }
            } else if( isdigit( ch ) ) {
                fixedcif += ch;
            } else if( ch == '-' || ch == '.' ) {
                // descartar silenciosamente
            } else {
                return Xtring::printf( _("el carácter '%c' es erróneo, solo se admiten dígitos o letras al inicio o al final"), at(i) );
            }
        }
        *this = fixedcif;
    }
    if( length() < 8 )
        return _("debe contener al menos 8 dígitos o letras");
// 	if( length() == 9 && isdigit(at(8)) )
// 		return _("contiene un dígito de más");
    if( length() > 9 )
        return _("contiene más de 9 dígitos o letras");
    if( !isEmpty() ) {
        for ( i=0; i<length(); i++ ) {
            if( i==0 || i==length()-1 ) {
                if( !isalnum(at(i)) && tratamiento == RecContacto::TratamientoNo ) {
                    return _("debe comenzar o acabar con una letra");
                }
            } else if( !isdigit(at(i)) ) {
                return Xtring::printf( _("el carácter '%c' es erróneo, solo se admiten dígitos o letras al inicio o al final"), at(i) );
            }
        }
        if( tratamiento == RecContacto::TratamientoDona
                || tratamiento == RecContacto::TratamientoDon
                || tratamiento == RecContacto::TratamientoAutonoma ) {
            if( isalpha( at(0) ) && strchr( "XYZxyz", at(0) ) == NULL  ) {
                return _("el CIF de una persona no puede comenzar por letra, salvo la X, Y y Z.\n");
            }
        } else if( tratamiento != RecContacto::TratamientoNo ) {
            if( !isalpha( at(0) ) ) {
                return _("el CIF de las sociedades debe comenzar por una letra.\n");
            }
        }
        char digcontrol = calcularCharControl();
        if( digcontrol ) {
            if( fixme ) {
                eliminarCharControl();
                *this += digcontrol;
            } else if( length() == 9 && at(8) != digcontrol ) {
                return Xtring::printf("el último dígito o letra '%c' es errónea, debería ser '%c'", at(8), digcontrol );
            } else if( length() == 8  ) {
                return Xtring::printf("falta el dígito o letra de control '%c'", digcontrol);
            }
        }

    }
    return ret;
}

/*<<<<<DBFIELD_CIF_ISVALID*/
bool FldCif::isValid( dbRecord *r, dbFieldValue *value, ValidResult::Context context, ValidResult *integres) const
{
    /*>>>>>DBFIELD_CIF_ISVALID*/
    return  FldStringAutoInc::isValid( r, value, context, integres );
#if 0
    Cif cif = value->toString().trim();
    Xtring error = cif.validate( fixvalue );
    if( fixvalue )
        value->setValue( cif );
    if( !error.isEmpty() ) {
        if( integres )
            integres->addError( Xtring::printf( _("%s '%s' erróneo: %s"),
                                                DBAPP->getDatabase()->findFieldDefinition( getFullName() )->getName().c_str(),
                                                cif.c_str(), error.c_str()),
                                getName() );
        if( sev >= ValidResult::valid )
            return false;
    }
    return true;
#endif
}

Variant FldCif::customDefaultValue() const
{
    Cif cif = FldStringAutoInc::customDefaultValue().toString();
    if( !cif.isEmpty() ) {
        cif.eliminarCharControl();
        cif += cif.calcularCharControl();
    }
    return Variant(cif);
}


} // namespace contactos
} // namespace gong


