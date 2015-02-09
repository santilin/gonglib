/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de empresas adaptado por el plugin factu
// FIELD Contabilidad bool tabContab
// FIELD InicioEjercicio date tabContab
// FIELD FinEjercicio date tabContab
// FIELD FechaContable date tabContab
// FIELD DigitosSubcuentas int tabContab
// FIELD CuentaCaja string tabContab
// FIELD MaxNivelGrupo int tabContab
// INHERIT empresa::Empresa
// TYPE FrmEditRecBehavior contab::Empresa
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITEMPRESABEHAVIOR_INCLUDES*/
#include <dbappmainwindow.h>
#include "contabfrmeditempresabehavior.h"
/*>>>>>FRMEDITEMPRESABEHAVIOR_INCLUDES*/
void FrmEditEmpresaBehavior::initGUI()
{
/*<<<<<FRMEDITEMPRESABEHAVIOR_INITGUI*/
	QWidget *pControlsFrame = getControlsFrame();
	QVBoxLayout* pControlsLayout = getControlsLayout();
	QWidget *tabContab = getOrCreateTab( "tabContab" );
	QVBoxLayout *tabContabLayout = static_cast<QVBoxLayout *>( tabContab->layout() );
	QHBoxLayout *contabilidadLayout = new QHBoxLayout(0, 0, 6, "contabilidadLayout");
	QHBoxLayout *inicioejercicioLayout = new QHBoxLayout(0, 0, 6, "inicioejercicioLayout");
	QHBoxLayout *finejercicioLayout = new QHBoxLayout(0, 0, 6, "finejercicioLayout");
	QHBoxLayout *fechacontableLayout = new QHBoxLayout(0, 0, 6, "fechacontableLayout");
	QHBoxLayout *digitossubcuentasLayout = new QHBoxLayout(0, 0, 6, "digitossubcuentasLayout");
	QHBoxLayout *cuentacajaLayout = new QHBoxLayout(0, 0, 6, "cuentacajaLayout");
	QHBoxLayout *maxnivelgrupoLayout = new QHBoxLayout(0, 0, 6, "maxnivelgrupoLayout");
	checkContabilidad = pTheForm->addCheckField( tabContab, "EMPRESA", "CONTABILIDAD", contabilidadLayout );
	editInicioEjercicio = pTheForm->addEditField( tabContab, "EMPRESA", "INICIOEJERCICIO", inicioejercicioLayout );
	editFinEjercicio = pTheForm->addEditField( tabContab, "EMPRESA", "FINEJERCICIO", finejercicioLayout );
	editFechaContable = pTheForm->addEditField( tabContab, "EMPRESA", "FECHACONTABLE", fechacontableLayout );
	editDigitosSubcuentas = pTheForm->addEditField( tabContab, "EMPRESA", "DIGITOSSUBCUENTAS", digitossubcuentasLayout );
	editCuentaCaja = pTheForm->addEditField( tabContab, "EMPRESA", "CUENTACAJA", cuentacajaLayout );
	editMaxNivelGrupo = pTheForm->addEditField( tabContab, "EMPRESA", "MAXNIVELGRUPO", maxnivelgrupoLayout );
	tabContabLayout->addLayout( contabilidadLayout );
	tabContabLayout->addLayout( inicioejercicioLayout );
	tabContabLayout->addLayout( finejercicioLayout );
	tabContabLayout->addLayout( fechacontableLayout );
	tabContabLayout->addLayout( digitossubcuentasLayout );
	tabContabLayout->addLayout( cuentacajaLayout );
	tabContabLayout->addLayout( maxnivelgrupoLayout );
/*>>>>>FRMEDITEMPRESABEHAVIOR_INITGUI*/
}

namespace gong {
namespace contab {


/*<<<<<FRMEDITEMPRESA_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "contabfrmeditempresa.h"
/*>>>>>FRMEDITEMPRESA_INCLUDES*/
#include "contabcuenta.h"

namespace gong {
namespace contab {

/*<<<<<FRMEDITEMPRESA_CONSTRUCTOR*/
FrmEditEmpresa::FrmEditEmpresa(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : empresa::FrmEditEmpresa( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditEmpresa" );
/*>>>>>FRMEDITEMPRESA_CONSTRUCTOR*/
    /*<<<<<FRMEDITEMPRESA_INIT_CONTROLS*/
	showTabs(true);
	QWidget *tabContab = new QWidget( pTabWidget, "tabContab" );
	QVBoxLayout *tabContabLayout = new QVBoxLayout(tabContab, 11, 6, "tabContabLayout");
	QHBoxLayout *contabilidadLayout = new QHBoxLayout(0, 0, 6, "contabilidadLayout");
	QHBoxLayout *inicioejercicioLayout = new QHBoxLayout(0, 0, 6, "inicioejercicioLayout");
	QHBoxLayout *finejercicioLayout = new QHBoxLayout(0, 0, 6, "finejercicioLayout");
	QHBoxLayout *fechacontableLayout = new QHBoxLayout(0, 0, 6, "fechacontableLayout");
	QHBoxLayout *digitossubcuentasLayout = new QHBoxLayout(0, 0, 6, "digitossubcuentasLayout");
	QHBoxLayout *cuentacajaLayout = new QHBoxLayout(0, 0, 6, "cuentacajaLayout");
	QHBoxLayout *maxnivelgrupoLayout = new QHBoxLayout(0, 0, 6, "maxnivelgrupoLayout");
	checkContabilidad = addCheckField( tabContab, "EMPRESA", "CONTABILIDAD", contabilidadLayout );
	editInicioEjercicio = addEditField( tabContab, "EMPRESA", "INICIOEJERCICIO", inicioejercicioLayout );
	editFinEjercicio = addEditField( tabContab, "EMPRESA", "FINEJERCICIO", finejercicioLayout );
	editFechaContable = addEditField( tabContab, "EMPRESA", "FECHACONTABLE", fechacontableLayout );
	editDigitosSubcuentas = addEditField( tabContab, "EMPRESA", "DIGITOSSUBCUENTAS", digitossubcuentasLayout );
	editCuentaCaja = addEditField( tabContab, "EMPRESA", "CUENTACAJA", cuentacajaLayout );
	editMaxNivelGrupo = addEditField( tabContab, "EMPRESA", "MAXNIVELGRUPO", maxnivelgrupoLayout );
	tabContabLayout->addLayout( contabilidadLayout );
	tabContabLayout->addLayout( inicioejercicioLayout );
	tabContabLayout->addLayout( finejercicioLayout );
	tabContabLayout->addLayout( fechacontableLayout );
	tabContabLayout->addLayout( digitossubcuentasLayout );
	tabContabLayout->addLayout( cuentacajaLayout );
	tabContabLayout->addLayout( maxnivelgrupoLayout );
/*>>>>>FRMEDITEMPRESA_INIT_CONTROLS*/
    pTabWidget->addTab( tabContab, toGUI( _( "&Contabilidad" ) ) );
}

void FrmEditEmpresa::scatterFields()
{
    /*<<<<<FRMEDITEMPRESA_SCATTER*/
	empresa::FrmEditEmpresa::scatterFields();
	checkContabilidad->setChecked(getRecEmpresa()->getValue("CONTABILIDAD").toBool());
	editInicioEjercicio->setText(getRecEmpresa()->getValue("INICIOEJERCICIO").toDate());
	editFinEjercicio->setText(getRecEmpresa()->getValue("FINEJERCICIO").toDate());
	editFechaContable->setText(getRecEmpresa()->getValue("FECHACONTABLE").toDate());
	editDigitosSubcuentas->setText(getRecEmpresa()->getValue("DIGITOSSUBCUENTAS").toInt());
	editCuentaCaja->setText(getRecEmpresa()->getValue("CUENTACAJA").toString());
	editMaxNivelGrupo->setText(getRecEmpresa()->getValue("MAXNIVELGRUPO").toInt());
/*>>>>>FRMEDITEMPRESA_SCATTER*/
}

void FrmEditEmpresa::gatherFields()
{
    /*<<<<<FRMEDITEMPRESA_GATHER*/
	empresa::FrmEditEmpresa::gatherFields();
	getRecEmpresa()->setValue( "CONTABILIDAD", checkContabilidad->isChecked());
	getRecEmpresa()->setValue( "INICIOEJERCICIO", editInicioEjercicio->toDate());
	getRecEmpresa()->setValue( "FINEJERCICIO", editFinEjercicio->toDate());
	getRecEmpresa()->setValue( "FECHACONTABLE", editFechaContable->toDate());
	getRecEmpresa()->setValue( "DIGITOSSUBCUENTAS", editDigitosSubcuentas->toInt());
	getRecEmpresa()->setValue( "CUENTACAJA", editCuentaCaja->toString());
	getRecEmpresa()->setValue( "MAXNIVELGRUPO", editMaxNivelGrupo->toInt());
/*>>>>>FRMEDITEMPRESA_GATHER*/
}

void FrmEditEmpresa::validateFields(QWidget *sender, bool *isvalid, ValidResult *ir)
{
    /*<<<<<FRMEDITEMPRESA_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	empresa::FrmEditEmpresa::validateFields(sender, isvalid, validresult);
/*>>>>>FRMEDITEMPRESA_VALIDATE*/
    if( (!sender || sender == editDigitosSubcuentas) && checkContabilidad->isOn() ) {
        if( editDigitosSubcuentas->toInt() < 5 || editDigitosSubcuentas->toInt() > MAX_DIGITOS_TRABAJO ) {
            validresult->addError( Xtring::printf(_("El número de dígitos de las subcuentas ha de estar entre %d y %d"),
                                                  5, MAX_DIGITOS_TRABAJO), "DIGITOSSUBCUENTAS" );
            *isvalid = false;
        }
    }
    if( !ir ) {
        showValidMessages(isvalid, *validresult, sender);
        delete validresult;
    }
}

/*<<<<<FRMEDITEMPRESA_FIN*/
} // namespace contab
} // namespace gong
/*>>>>>FRMEDITEMPRESA_FIN*/


void FrmEditEmpresaBehavior::scatterFields( bool is_pre )
{
	if( !is_pre) return;
/*<<<<<FRMEDITEMPRESABEHAVIOR_SCATTER*/
	empresa::FrmEditEmpresa::scatterFields( bool is_pre );
	checkContabilidad->setChecked( pTheForm->getRecord()->getValue("CONTABILIDAD").toBool());
	editInicioEjercicio->setText( pTheForm->getRecord()->getValue("INICIOEJERCICIO").toDate());
	editFinEjercicio->setText( pTheForm->getRecord()->getValue("FINEJERCICIO").toDate());
	editFechaContable->setText( pTheForm->getRecord()->getValue("FECHACONTABLE").toDate());
	editDigitosSubcuentas->setText( pTheForm->getRecord()->getValue("DIGITOSSUBCUENTAS").toInt());
	editCuentaCaja->setText( pTheForm->getRecord()->getValue("CUENTACAJA").toString());
	editMaxNivelGrupo->setText( pTheForm->getRecord()->getValue("MAXNIVELGRUPO").toInt());
/*>>>>>FRMEDITEMPRESABEHAVIOR_SCATTER*/
}

void FrmEditEmpresaBehavior::gatherFields()
{
/*<<<<<FRMEDITEMPRESABEHAVIOR_GATHER*/
	empresa::FrmEditEmpresa::gatherFields();
	pTheForm->getRecord()->setValue( "CONTABILIDAD", checkContabilidad->isChecked());
	pTheForm->getRecord()->setValue( "INICIOEJERCICIO", editInicioEjercicio->toDate());
	pTheForm->getRecord()->setValue( "FINEJERCICIO", editFinEjercicio->toDate());
	pTheForm->getRecord()->setValue( "FECHACONTABLE", editFechaContable->toDate());
	pTheForm->getRecord()->setValue( "DIGITOSSUBCUENTAS", editDigitosSubcuentas->toInt());
	pTheForm->getRecord()->setValue( "CUENTACAJA", editCuentaCaja->toString());
	pTheForm->getRecord()->setValue( "MAXNIVELGRUPO", editMaxNivelGrupo->toInt());
/*>>>>>FRMEDITEMPRESABEHAVIOR_GATHER*/
}

void FrmEditEmpresaBehavior::validateFields(bool is_pre,
		QWidget *sender, bool *isvalid, ValidResult *ir)
{
	if( !is_pre) return;
/*<<<<<FRMEDITEMPRESABEHAVIOR_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	empresa::FrmEditEmpresa::validateFields(bool is_pre,
		sender, isvalid, validresult);
/*>>>>>FRMEDITEMPRESABEHAVIOR_VALIDATE*/
	if( !ir ) {
		showValidMessages(isvalid, *validresult, sender);
		delete validresult;
	}
}

/*<<<<<FRMEDITEMPRESABEHAVIOR_FIN*/
} // namespace contab
} // namespace gong
/*>>>>>FRMEDITEMPRESABEHAVIOR_FIN*/

