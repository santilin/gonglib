/*<<<<<MODULE_INFO*/
// COPYLEFT Behavior para editar cobros del módulo de socias
// TYPE FrmEditRecBehavior socias::RemesaCobro Q_OBJECT
/*>>>>>MODULE_INFO*/

#include <pagosfrmeditcobro.h>
/*<<<<<FRMEDITREMESACOBROBEHAVIOR_INCLUDES*/
#include <dbappmainwindow.h>
#include "sociasfrmeditremesacobrobehavior.h"
/*>>>>>FRMEDITREMESACOBROBEHAVIOR_INCLUDES*/
#include "sociasmodule.h"

namespace gong {
namespace socias {

void FrmEditRemesaCobroBehavior::initGUI()
{
/*<<<<<FRMEDITREMESACOBROBEHAVIOR_INITGUI*/
	QWidget *pControlsFrame = getControlsFrame();
	QVBoxLayout* pControlsLayout = getControlsLayout();
/*>>>>>FRMEDITREMESACOBROBEHAVIOR_INITGUI*/
    pushGenerarRecibo = new QPushButton( pTheForm );
    pushGenerarRecibo->setText( toGUI(_("Añadir recibo")) );
    connect( pushGenerarRecibo, SIGNAL( clicked() ), this, SLOT( slotGenerarRecibo_clicked() ) );
    getButtonsLayout()->addWidget( pushGenerarRecibo );
}

void FrmEditRemesaCobroBehavior::scatterFields( bool is_pre )
{
    if( !is_pre) return;
    /*<<<<<FRMEDITREMESACOBROBEHAVIOR_SCATTER*/

/*>>>>>FRMEDITREMESACOBROBEHAVIOR_SCATTER*/
}

void FrmEditRemesaCobroBehavior::gatherFields()
{
    /*<<<<<FRMEDITREMESACOBROBEHAVIOR_GATHER*/

/*>>>>>FRMEDITREMESACOBROBEHAVIOR_GATHER*/
}

void FrmEditRemesaCobroBehavior::validateFields(bool is_pre,
        QWidget *sender, bool *isvalid, ValidResult *ir)
{
    if( !is_pre) return;
    /*<<<<<FRMEDITREMESACOBROBEHAVIOR_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
/*>>>>>FRMEDITREMESACOBROBEHAVIOR_VALIDATE*/
    if( !ir ) {
        showValidMessages(isvalid, *validresult, sender);
        delete validresult;
    }
}

void FrmEditRemesaCobroBehavior::slotGenerarRecibo_clicked()
{
    if( !pTheForm->read(0) )
        return;

    pagos::RecCobro *cobro = static_cast<pagos::RecCobro *>( DBAPP->createRecord( "COBRO" ) );
    cobro->setValue( "TABLAFACTURAS", "MIEMBRO" );
    cobro->setValue( "REMESA_ID", pTheForm->getRecord()->getValue( "ID" ) );
    cobro->setValue( "TABLATERCEROS", "CONTACTO" );
#ifdef HAVE_FACTUMODULE
    if( ModuleInstance->getFactuModule() )
        cobro->setValue( "TABLATERCEROS", "CLIENTE" );
#endif
    pagos::FrmEditCobro *frm = static_cast<pagos::FrmEditCobro *>(
                                   DBAPP->createEditForm( pTheForm, cobro, 0, DataTable::inserting, dbApplication::simpleEdition ) );
#ifdef HAVE_FACTUMODULE
    if( ModuleInstance->getFactuModule() )
        frm->completa( "MIEMBRO", "NUMEROSOCIA", "FECHAALTA",
                       "CLIENTE", "CODIGO", "RAZONSOCIAL", Xtring::null, Xtring::null, true );
    else
#endif
        frm->completa( "MIEMBRO", "NUMEROSOCIA", "FECHAALTA", "CONTACTO", "CIF", "NOMBRE",
                       Xtring::null, Xtring::null, true );
    if( frm ) {
        frm->setUsarRemesas( true );
        frm->showModalFor( pTheForm, false, true );
        delete frm;
    }
}


/*<<<<<FRMEDITREMESACOBROBEHAVIOR_FIN*/
} // namespace socias
} // namespace gong
/*>>>>>FRMEDITREMESACOBROBEHAVIOR_FIN*/

