#include "config.h"
#include <gonggettext.h>
#include "factumodule.h"
#include "factufrmpagar.h"

namespace gong {
namespace factu {

Date FrmPagar::sLastFechaPago = Date::currentDate();
Xtring FrmPagar::sLastCuentaPago = "57.";
Xtring FrmPagar::sLastDocumentoPago;

/**
 * @brief Mini formulario para recoger los datos de los pagos en documentos de factu
 *
 * @param has_contab ...
 * @param importe ...
 * @param fechapago ...
 * @param cuentapago ...
 * @param documentopago ...
 * @param parent ...
 * @param name ...
 * @param f ...
 **/
FrmPagar::FrmPagar(bool has_contab,
                   const Money &importe, Date fechapago,
                   const Xtring &cuentapago, const Xtring &documentopago,
                   QWidget * parent, const char * name, WidgetFlags f)
    : FrmCustom( parent, name, f ), mHasContab( has_contab )
{
    setCaption( _("Pagar/cobrar recibo") );
    pFocusWidget = editImporte = addInputField( 0, Xtring::null,  "COBRO", "IMPORTE", importe );
    editFechaPago = addInputField( 0, Xtring::null, "COBRO", "FECHAPAGO", fechapago );
	searchCuentaPago = 0;
#ifdef HAVE_CONTABMODULE
    if( has_contab ) {
        searchCuentaPago = addSearchField( 0, "CUENTA", "CUENTA", "DESCRIPCION" );
        searchCuentaPago->getEditCode()->setText( cuentapago );
        searchCuentaPago->getEditCode()->setJustEdited( true );
    }
#elif defined( HAVE_TESORERIAMODULE )
    if( has_contab ) {
        searchCuentaPago = addSearchField( 0, "CUENTATESORERIA", "CODIGO", "NOMBRE" );
        searchCuentaPago->getEditCode()->setText( cuentapago );
        searchCuentaPago->getEditCode()->setJustEdited( true );
    }
#endif
    editDocumentoPago = addInputField( 0, Xtring::null, "COBRO", "DOCUMENTOPAGO", documentopago );
}

void FrmPagar::accept()
{
    if( editFechaPago->toDate().getYear() != empresa::ModuleInstance->getEjercicio() ) {
        msgError(this,
                 Xtring::printf( _("La fecha de pago no es de este ejercicio. Cambia al ejercicio %d"),
                                 editFechaPago->toDate().getYear() ) );
        return;
    }
    FrmCustom::accept();
}

void FrmPagar::validate_input(QWidget* search, bool* is_valid)
{
#ifdef HAVE_CONTABMODULE
    if( mHasContab ) {
        if( search == searchCuentaPago->getEditCode() ) {
            contab::Cuenta c( searchCuentaPago->getEditCode()->toString(),
                              contab::ModuleInstance->getDigitosTrabajo() );
            c.expandir();
            searchCuentaPago->getEditCode()->setText( c.toString() );
            searchCuentaPago->getEditCode()->setJustEdited( true );
            return;
        }
    }
#endif
    FrmCustom::validate_input( search, is_valid );
}


FrmPagar::~FrmPagar()
{
	_GONG_DEBUG_PRINT(0, "Borro frmpagar");
}


} // namespace gong
} // namespace sociasfrmpagarrecibo

