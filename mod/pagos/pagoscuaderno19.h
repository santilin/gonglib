//
// C++ Interface: gongcuaderno19
//
// Description:
//
//
// Author: Francisco Santiago Capel Torres <santiagocapel@yahoo.es>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef GONGCUADERNO19_H
#define GONGCUADERNO19_H

#include <vector>

#include <gongxtring.h>
#include <gongdate.h>
#include <gongmoney.h>

namespace gong {
namespace socias {

class Cuaderno19
{
public:
    Cuaderno19(int tipo);
    void addCabeceraPresentador(const Xtring &cifpresentador, const Xtring &nombrepresentador,
                                const Date fechaenvio, const Xtring &entidad, const Xtring &oficina);
    void addCabeceraOrdenante(const Xtring &cifordenante, const Xtring &nombreordenante,
                              const Date fechaenvio, const Date fechacargo, const Xtring &CCC);
    void addRegistroIndividual(int referencia, const Xtring &nombre, const Xtring &CCC,
                               Money importe, const Xtring &refdevolucion, const Xtring &refinterna, const Xtring &concepto);
    void addRegistroTotalOrdenante();
    void addRegistroTotalGeneral();
    bool save(const Xtring &filename);
    ~Cuaderno19();
private:
    std::vector<Xtring> mLineas;
    int mTipo;
    int mNumeroRegistrosOrdenante, mNumeroRegistrosTotal, mNumeroDomiciliaciones, mNumeroOrdenantes;
    Xtring mCifPresentador;
    Xtring mCifOrdenante;
    Xtring mNombreOrdenante;
    Money mImporteOrdenante, mImporteTotal;
};

} // namespace socias
} // namespace gong

#endif
