#ifndef VENTASWEBFRMCAMBIOSMASIVOSARTICULOS_H
#define VENTASWEBFRMCAMBIOSMASIVOSARTICULOS_H

#include <factufrmcambiosmasivosarticulos.h>

namespace gong {
namespace ventasweb {

class FrmCambiosMasivosArticulos: public factu::FrmCambiosMasivosArticulos
{
public:
    FrmCambiosMasivosArticulos(QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
    virtual void accept(); // from FrmBase
protected:
    int subir_imagenes();
    int publicar_articulos(bool publicar);
};

} // namespace ventasweb
} // namespace gong

#endif // FACTUFRMCAMBIOSMASIVOSARTICULOS_H
