#ifndef FACTUFRMCAMBIOSMASIVOSARTICULOS_H
#define FACTUFRMCAMBIOSMASIVOSARTICULOS_H

#include <dbappfrmcustom.h>
#include <QVBoxLayout>

class QVBoxLayout;

namespace gong {
namespace factu {

class FrmCambiosMasivosArticulos: public FrmCustom
{
public:
    FrmCambiosMasivosArticulos(QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );

protected:
    virtual void accept(); // from FrmBase
    virtual bool validate(); // from FrmCustom
    virtual Xtring getGlobalWhere() const;
    virtual XtringList getPVPFields() const;
    unsigned long int grabar_imagenes();
    unsigned long int cargar_imagenes();
    unsigned long int cambiar_precios( int operacion, double nuevoprecio,
                                       const Xtring &fldcambio, const Xtring &fldbase );

    ComboBoxInt *pComboOperacion, *pComboOperacionPrecio;
    SearchBox *pSearchFamilia, *pSearchArticulo, *pSearchProveedora;
    XtringList mOperaciones, mOperacionesPrecios;
    List<int> mCodigosOperaciones, mCodigosOperacionesPrecios;
    LineEdit *pNuevoPrecio;
    ComboBoxXtring *pComboCampoPrecioBase, *pComboCampoPrecioCambio;
};

} // namespace factu
} // namespace gong

#endif // FACTUFRMCAMBIOSMASIVOSARTICULOS_H
