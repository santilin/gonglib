#ifndef GONGCUENTA_H
#define GONGCUENTA_H

#include <gongxtring.h>

namespace gong {
namespace contab {

#define MAX_DIGITOS_TRABAJO 14

class Cuenta: public Xtring
{
public:
    Cuenta(unsigned int digitostrabajo);
    Cuenta(const Xtring &cuenta, unsigned int digitostrabajo);
    Cuenta(const char *cuenta, unsigned int digitostrabajo);
    void expandir();
    void contraer( uint max_nivel_grupo );
    bool isValid(Xtring &message) const;
    bool isExpandida() const;
    bool isContraida() const;
    bool isDiario() const;
    bool isGrupo() const;
    unsigned int getNivel() const;
    Cuenta getPadre() const;
    Cuenta toGrupo() const;
    Cuenta sigCuentaDiario() const;
    Cuenta antCuentaDiario() const;
    Cuenta sigCuentaGrupo() const;
    Cuenta antCuentaGrupo() const;
    const Xtring &toString() const {
        return *this;
    }
private:
    unsigned int mDigitosTrabajo;
};

} // namespace contab
} // namespace gong

#endif // GONGCUENTA_H
