//
// C++ Implementation: gongcuaderno19
//
// Description:
//
//
// Author: Francisco Santiago Capel Torres <santiagocapel@yahoo.es>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <math.h>
#include <fstream>

#include <gongdebug.h>
#include "pagoscuaderno19.h"

#define CONST_CHAR_PTR(x) (x).c_str()

/// \todo {0.4} addRegistroOpcional y tipo 01

#include <iostream>
using namespace std;


namespace gong {
namespace socias {



Cuaderno19::Cuaderno19(int tipo)
{
    mTipo = tipo;
    mImporteOrdenante = 0.0;
    mImporteTotal = 0.0;
    mNumeroRegistrosOrdenante = 0;
    mNumeroRegistrosTotal = 0;
    mNumeroDomiciliaciones = 0;
    mNumeroOrdenantes = 0;
}

void Cuaderno19::addCabeceraPresentador(const Xtring &cifpresentador, const Xtring &nombrepresentador,
                                        const Date fechaenvio, const Xtring &entidad, const Xtring &oficina)
{
    _GONG_DEBUG_ASSERT(	mNumeroRegistrosOrdenante == 0 && mNumeroRegistrosTotal == 0
                        && mNumeroDomiciliaciones == 0 && mNumeroOrdenantes == 0 );
    mCifPresentador = cifpresentador;
    char buffer[163];
    sprintf(buffer,
            "5180%9s000%02d%02d%02d      %-40s                    %4s%4s"
            "            " // E3
            "                                        " // F
            "              ", // G
            CONST_CHAR_PTR(mCifPresentador), fechaenvio.getDay(), fechaenvio.getMonth(), fechaenvio.getYear()%100,
            CONST_CHAR_PTR(nombrepresentador.left(40)), CONST_CHAR_PTR(entidad), CONST_CHAR_PTR(oficina));
    mLineas.push_back( Xtring(buffer).upper() );
    mNumeroRegistrosTotal = 1;
}

void Cuaderno19::addCabeceraOrdenante(const Xtring &cifordenante, const Xtring &nombreordenante,
                                      const Date fechaenvio, const Date fechacargo, const Xtring &CCC)
{
    mCifOrdenante = cifordenante;
    mNombreOrdenante = nombreordenante;

    char buffer[163];
    sprintf( buffer,
             "5380%9s000%02d%02d%02d%02d%02d%02d%-40s%-20s"
             "        " // E1
             "%02d"
             "          " // E3
             "                                        " // F
             "              ", // G
             CONST_CHAR_PTR(mCifOrdenante), fechaenvio.getDay(), fechaenvio.getMonth(), fechaenvio.getYear()%100,
             fechacargo.getDay(), fechacargo.getMonth(), fechacargo.getYear()%100,
             CONST_CHAR_PTR(mNombreOrdenante.left(40)), CONST_CHAR_PTR(CCC.left(20)), mTipo );
    mLineas.push_back( Xtring(buffer).upper() );
    mNumeroDomiciliaciones = 0;
    mNumeroRegistrosOrdenante = 1;
    mNumeroOrdenantes++;
    mNumeroRegistrosTotal++;
    mImporteOrdenante = 0.0;
}

void Cuaderno19::addRegistroIndividual(int referencia, const Xtring &_nombre, const Xtring &CCC,
                                       Money importe, const Xtring &refdevolucion, const Xtring &refinterna, const Xtring &_concepto)
{
    Xtring concepto = _concepto.left(17).upper();
    Xtring nombre = _nombre.left(40).upper();
    char buffer[163];
    sprintf( buffer,
             "5680%9s000%012d%-40s%-20s"
             "%010d%-6s%-10s%-17s"
             "                     " // G2(23)
             "        ", //H He tenido que borrar 10 caracteres del final según corrección de José Mainer Triodos
             CONST_CHAR_PTR(mCifOrdenante), referencia, CONST_CHAR_PTR(nombre), CONST_CHAR_PTR(CCC.left(20)),
             int(floor(importe.toDouble()*100)),
             CONST_CHAR_PTR(refdevolucion.left(6)), CONST_CHAR_PTR(refinterna.left(10)), CONST_CHAR_PTR(concepto) );
    std::cout << "(" << strlen(buffer) << ") " << buffer << std::endl;
    mLineas.push_back( buffer );
    mNumeroRegistrosTotal++;
    mNumeroRegistrosOrdenante++;
    mNumeroDomiciliaciones++;
    mImporteOrdenante += importe;
    mImporteTotal += importe;
}

void Cuaderno19::addRegistroTotalOrdenante()
{
    char buffer[163];
    mNumeroRegistrosOrdenante++;
    sprintf( buffer,
             "5880%9s000"
             "            " // B2(12)
             "                                        " // C(40)
             "                    " // D (20)
             "%010d" // E1
             "      " // E2
             "%010d" // F1
             "%010d" // F2 // diferente del de arriba
             "                    " // F3 (20)
             "                  ", // G(18)
             CONST_CHAR_PTR(mCifOrdenante), int(floor(mImporteOrdenante.toDouble()*100)), mNumeroDomiciliaciones,
             mNumeroRegistrosOrdenante );
    mLineas.push_back( Xtring(buffer).upper() );
    mNumeroRegistrosTotal++;
}

void Cuaderno19::addRegistroTotalGeneral()
{
    char buffer[163];
    mNumeroRegistrosTotal++;
    sprintf( buffer,
             "5980%9s000"
             "            " // B2(12)
             "                                        " // C(40)
             "%04d" // D1
             "                " // D2
             "%010d" // E1
             "      " // E2
             "%010d" // F1
             "%010d" // F2
             "                    " // F3 (20)
             "                  ", // G(18)
             CONST_CHAR_PTR(mCifPresentador), mNumeroOrdenantes, int(floor(mImporteTotal.toDouble()*100)),
             mNumeroDomiciliaciones, mNumeroRegistrosTotal);
    mLineas.push_back( Xtring(buffer).upper() );
}

bool Cuaderno19::save(const Xtring &filename)
{
    std::ofstream out(CONST_CHAR_PTR(filename));
    if( out ) {
        for( std::vector<Xtring>::const_iterator it = mLineas.begin();
                it != mLineas.end();
                ++it ) {
            out << *it << "\x0D\x0A";
        }
        out.close();
        return true;
    }
    return false;
}

Cuaderno19::~Cuaderno19()
{
}


} // namespace gong
} // namespace socias

