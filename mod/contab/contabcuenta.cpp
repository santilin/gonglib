#include <ctype.h>
#include <gongdebug.h>
#include <gonggettext.h>
#include <gongxtring.h>
#include "contabcuenta.h"

namespace gong {
namespace contab {

Cuenta::Cuenta(unsigned int digitostrabajo)
    : Xtring(), mDigitosTrabajo(digitostrabajo)
{
}

Cuenta::Cuenta(const Xtring &cuenta, unsigned int digitostrabajo)
    : Xtring(cuenta), mDigitosTrabajo(digitostrabajo)
{
}

// Esta función es necesaria porque no se puede realizar una conversión automática
// de const Xtring & a const char *
Cuenta::Cuenta(const char *cuenta, unsigned int digitostrabajo)
    : Xtring(cuenta), mDigitosTrabajo(digitostrabajo)
{
}


/**
  Expande un numero de cuenta abreviada para convertirla en una
  cuenta de trabajo, por ejemplo,
  43.1  ==> 43000001 si nivel de trabajo=8
 */
void Cuenta::expandir()
{
    trim();
    int nceros, pospunto;
    pospunto = find(".");
    if( pospunto != -1 ) {
        Xtring sceros;
        for( nceros = mDigitosTrabajo - pospunto - mid(pospunto+1).length();
                nceros>0;
                nceros--)
            sceros += '0';
        *this = Cuenta(left(pospunto) + sceros + mid(pospunto+1), mDigitosTrabajo);
    } // else nada, ya está expandida
}

/**
  Lo contrario de la función anterior. Contrae una cuenta sustituyendo
  ceros por punto, hasta el mayor subgrupo
  Por ejemplo, para un plan contable de 1,2,3 y 8 digitos
  43000022 ==> 430.22
  */
void Cuenta::contraer( uint max_nivel_grupo )
{
    trim();
    if( find(".") != Xtring::npos )
        return;
    if( length() <= max_nivel_grupo + 1 )
        return;
    Cuenta contraida( this->left(max_nivel_grupo), mDigitosTrabajo);
    try {
        bool eliminandoceros = (at( max_nivel_grupo + 1) == '0');
        if( eliminandoceros )
            contraida += ".";
        for( unsigned int i=max_nivel_grupo + 1; i < length() && i < mDigitosTrabajo; i++ ) {
            if( at(i) == '0' && eliminandoceros ) {
            } else {
                eliminandoceros = false;
                if( i < mDigitosTrabajo )
                    contraida += at(i);
            }
        }
    } catch( ... ) {
        _GONG_DEBUG_WARNING( "Catchenlamar" );
    }
    *this = contraida;
}

/**
  Determina si una cuenta es de diario
 */
bool Cuenta::isDiario() const
{
    return length() == mDigitosTrabajo;
}

bool Cuenta::isGrupo() const
{
    return !isDiario();
}

bool Cuenta::isContraida() const
{
    return find(".") != Xtring::npos;
}

bool Cuenta::isExpandida() const
{
    return !isContraida();
}

Cuenta Cuenta::toGrupo() const
{
    Xtring::size_type pospunto = find(".");
    if( pospunto == Xtring::npos )
        return Cuenta(left(mDigitosTrabajo), mDigitosTrabajo);
    else
        return Cuenta(left(pospunto-1), mDigitosTrabajo);
}

Cuenta Cuenta::sigCuentaDiario() const
{
    return Cuenta(stringInc(*this), mDigitosTrabajo);
}

Cuenta Cuenta::antCuentaDiario() const
{
    return Cuenta(stringDec(*this), mDigitosTrabajo);
}

Cuenta Cuenta::sigCuentaGrupo() const
{
    return Cuenta(stringInc(toGrupo()), mDigitosTrabajo );
}

Cuenta Cuenta::antCuentaGrupo() const
{
    return Cuenta( stringDec(toGrupo()), mDigitosTrabajo );
}

Cuenta Cuenta::getPadre() const
{
    unsigned int l = length();
    if( l <= 1 )
        return Cuenta( "", mDigitosTrabajo);
    else if( l <= 5 )
        return Cuenta( left(l-1), mDigitosTrabajo );
    else
        return Cuenta( left(5), mDigitosTrabajo );
}


unsigned int Cuenta::getNivel() const
{
    if( isContraida() || length() == mDigitosTrabajo )
        return 5;
    else if( length() <=5 )
        return length();
    else
        return 0;
}

bool Cuenta::isValid(Xtring &message) const
{
    if( length()==0 ) {
        message = _("La cuenta no puede estar vacía");
        return false;
    }
    for( unsigned int i=0; i<length(); i++ ) {
        if( !isdigit(at(i)) && at(i)!='.') {
            message = Xtring::printf(_("Cuenta '%1s' no válida, carácter '%2c' no válido"),
                                     toString().c_str(), at(i));
            return false;
        }
    }
    if( length() > mDigitosTrabajo && mDigitosTrabajo != 0 ) {
        message = Xtring::printf(_("La longitud de la cuenta '%1s' no puede ser mayor de %2d dígitos"),
                                 toString().c_str(), mDigitosTrabajo);
        return false;
    }
    if( getNivel() == 0 ) {
        message = Xtring::printf(_("La cuenta '%1s' no se ajusta a ningún nivel"), toString().c_str() );
        return false;
    }
    return true;
}

} // namespace contab
} // namespace gong
