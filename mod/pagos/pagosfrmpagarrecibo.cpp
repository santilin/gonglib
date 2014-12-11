#include <gonggettext.h>
#include "pagosmodule.h"
#include "pagosfrmpagarrecibo.h"

#ifdef HAVE_CONTABMODULE
#include <contabmodule.h>
#endif

namespace gong {
namespace pagos {

FrmPagarRecibo::FrmPagarRecibo(bool has_contab, const Money &importe, Date fechapago,
                               const Xtring &cuentapago, const Xtring &documentopago,
                               const Xtring &moneda_codigo, QWidget * parent, const char * name, WidgetFlags f)
    : FrmCustom( parent, "FrmPagarRecibo", f ), mHasContab( has_contab )
{
    setCaption( _("Pagar/cobrar recibo") );
    pFocusWidget = editImporte = addInputField( 0, Xtring::null,  "COBRO", "IMPORTE", importe );
    editFechaPago = addInputField( 0, Xtring::null, "COBRO", "FECHAPAGO", fechapago );
#ifdef HAVE_CONTABMODULE
    if( has_contab ) {
        searchCuentaPago = addSearchField( 0, "CUENTA", "CUENTA", "DESCRIPCION" );
        searchCuentaPago->setValue( cuentapago );
    }
#elif defined ( HAVE_TESORERIAMODULE )
    if( has_contab ) {
        searchCuentaPago = addSearchField( 0, "CUENTATESORERIA", "CODIGO", "NOMBRE" );
        //searchCuentaPago->setValue( cuentapago );
    }
#endif
    editDocumentoPago = addInputField( 0, Xtring::null, "COBRO", "DOCUMENTOPAGO", documentopago );
    searchMoneda = addSearchField( 0, "MONEDA", "CODIGO", "NOMBRE" );
//    searchMoneda->setValue( moneda_codigo );
}

dbRecordID FrmPagarRecibo::getRecMonedaID() const
{
    return searchMoneda->getRecord()->getRecordID();
}

void FrmPagarRecibo::accept()
{
    if( editFechaPago->toDate().getYear() != empresa::ModuleInstance->getEjercicio() ) {
        msgError(this,
                 Xtring::printf( _("La fecha de pago no es de este ejercicio. Cambia al ejercicio %d"),
                                 editFechaPago->toDate().getYear() ) );
        return;
    }
    FrmCustom::accept();
}

void FrmPagarRecibo::validate_input(QWidget* sender, bool* is_valid)
{
#ifdef HAVE_CONTABMODULE
    if( mHasContab ) {
        if( sender == searchCuentaPago->getEditCode() ) {
            Xtring old_cuenta = searchCuentaPago->getEditCode()->toString();
            contab::Cuenta c( old_cuenta, contab::ModuleInstance->getDigitosTrabajo() );
            c.expandir();
            if( c.toString() != old_cuenta )
                searchCuentaPago->setValue( c.toString() );
            return;
        }
    }
#endif
    FrmCustom::validate_input( sender, is_valid );
}


} // namespace gong
} // namespace pagos

