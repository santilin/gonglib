/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de apuntes
// FIELD NumApunte int
// FIELD Cuenta_ID Reference(Cuenta,Cuenta,Descripcion)
// FIELD Debe Money - money
// FIELD Haber Money - money
// FIELD Concepto string
// FIELD Contrapartida string
// FIELD Notas text
// TYPE FrmEditRecDetail contab::Apunte
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITAPUNTE_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "contabfrmeditapunte.h"
/*>>>>>FRMEDITAPUNTE_INCLUDES*/
#include "contabmodule.h"
#include "contabfrmeditasiento.h"
#include "contabcuenta.h"
#include <gongdatatable.h>

namespace gong {
namespace contab {

/*<<<<<FRMEDITAPUNTE_CONSTRUCTOR*/
FrmEditApunte::FrmEditApunte(FrmEditRecMaster *frmmaster, int ndetail,
	dbRecord *detail, const Xtring &dettablename, dbRecordDataModel *dm,
	EditMode editmode, dbApplication::EditFlags editflags,
	QWidget* parent, const char* name, WidgetFlags fl )
		: FrmEditRecDetail( frmmaster, ndetail, detail, dettablename, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditApunte" );
/*>>>>>FRMEDITAPUNTE_CONSTRUCTOR*/

    /*<<<<<FRMEDITAPUNTE_INIT_CONTROLS*/
	QHBoxLayout *numapunteLayout = new QHBoxLayout(0, 0, 6, "numapunteLayout");
	QHBoxLayout *cuenta_idLayout = new QHBoxLayout(0, 0, 6, "cuenta_idLayout");
	QHBoxLayout *moneyLayout = new QHBoxLayout(0, 0, 6, "moneyLayout");
	QHBoxLayout *conceptoLayout = new QHBoxLayout(0, 0, 6, "conceptoLayout");
	QHBoxLayout *contrapartidaLayout = new QHBoxLayout(0, 0, 6, "contrapartidaLayout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
	editNumApunte = addEditField( pControlsFrame, "APUNTE", "NUMAPUNTE", numapunteLayout );

	searchCuentaCuenta = addSearchField( pControlsFrame, "CUENTA_ID", "CUENTA", "CUENTA", "DESCRIPCION", cuenta_idLayout );
	pushCuentaCuenta = searchCuentaCuenta->getButton();
	connect( pushCuentaCuenta, SIGNAL( clicked() ), this, SLOT( pushCuentaCuenta_clicked() ) );
	editCuentaCuenta = searchCuentaCuenta->getEditCode();
	editCuentaDescripcion = searchCuentaCuenta->getEditDesc();
	editDebe = addEditField( pControlsFrame, "APUNTE", "DEBE", moneyLayout );
	editHaber = addEditField( pControlsFrame, "APUNTE", "HABER", moneyLayout );
	editConcepto = addEditField( pControlsFrame, "APUNTE", "CONCEPTO", conceptoLayout );
	editContrapartida = addEditField( pControlsFrame, "APUNTE", "CONTRAPARTIDA", contrapartidaLayout );
	editNotas = addTextField( pControlsFrame, "APUNTE", "NOTAS", notasLayout );
	pControlsLayout->addLayout( numapunteLayout );
	pControlsLayout->addLayout( cuenta_idLayout );
	pControlsLayout->addLayout( moneyLayout );
	pControlsLayout->addLayout( conceptoLayout );
	pControlsLayout->addLayout( contrapartidaLayout );
	pControlsLayout->addLayout( notasLayout );
/*>>>>>FRMEDITAPUNTE_INIT_CONTROLS*/
    QHBoxLayout *contrapartida_idLayout = new QHBoxLayout(0, 0, 6, "contrapartida_idLayout");
    searchContrapartidaCuenta = addSearchField( pControlsFrame, "CONTRAPARTIDA_ID", "CUENTA", "CUENTA", "DESCRIPCION", contrapartida_idLayout);
    pushContrapartidaCuenta = searchContrapartidaCuenta->getButton();
    connect( pushContrapartidaCuenta, SIGNAL( clicked() ), this, SLOT( pushContrapartidaCuenta_clicked() ) );
    editContrapartidaCuenta = searchContrapartidaCuenta->getEditCode();
    editContrapartidaDescripcion = searchContrapartidaCuenta->getEditDesc();
    pControlsLayout->insertLayout(4, contrapartida_idLayout );
    setTabOrder( editConcepto, editContrapartidaCuenta );
    setTabOrder( editContrapartidaCuenta, editContrapartida );
    pRecContrapartida = static_cast<RecContrapartida *>(DBAPP->createRecord( "cuenta" ) );
    editNumApunte->setMustBeReadOnly( true );
}

FrmEditApunte::~FrmEditApunte()
{
    delete pRecContrapartida;
}

FrmEditAsiento *FrmEditApunte::getFrmAsiento() const
{
    return static_cast<FrmEditAsiento *>(getFrmMaster());
}


void FrmEditApunte::scatterFields()
{
    _GONG_DEBUG_ASSERT( getFrmAsiento() );
    if( getRecord()->getValue("NUMAPUNTE").toInt() == 0 )
        getRecord()->setValue( "NUMAPUNTE", getNumDetalle() + 1);
    editNumApunte->setMustBeReadOnly( true );
    if( pFocusWidget == 0 )
        pFocusWidget = editCuentaCuenta;
    /*<<<<<FRMEDITAPUNTE_SCATTER*/
	editNumApunte->setText(getRecApunte()->getValue("NUMAPUNTE").toInt());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editNumApunte;
	editDebe->setText(getRecApunte()->getValue("DEBE").toMoney());
	editHaber->setText(getRecApunte()->getValue("HABER").toMoney());
	editConcepto->setText(getRecApunte()->getValue("CONCEPTO").toString());
	editContrapartida->setText(getRecApunte()->getValue("CONTRAPARTIDA").toString());
	editNotas->setText(getRecApunte()->getValue("NOTAS").toString());
	scatterCuenta();
/*>>>>>FRMEDITAPUNTE_SCATTER*/
    if( isInserting() || pRecord->isEmpty() ) {
        editConcepto->setText( getFrmAsiento()->getLastConcepto() );
    }
    if( calcSaldo() != 0 && !isSelecting() ) {
        QPushButton *pushCuadrar = new QPushButton( "C&uadrar", this, "pushCuadrar" );
        connect( pushCuadrar, SIGNAL( clicked() ), this, SLOT( pushCuadrar_clicked() ));
        pButtonsLayout->insertWidget( 0, pushCuadrar );
    }

}

void FrmEditApunte::gatherFields()
{
    /*<<<<<FRMEDITAPUNTE_GATHER*/
	getRecApunte()->setValue( "NUMAPUNTE", editNumApunte->toInt());
	getRecApunte()->setValue( "CUENTA_ID", getRecCuenta()->getRecordID() );
	getRecApunte()->setValue( "DEBE", editDebe->toMoney());
	getRecApunte()->setValue( "HABER", editHaber->toMoney());
	getRecApunte()->setValue( "CONCEPTO", editConcepto->toString());
	getRecApunte()->setValue( "CONTRAPARTIDA", editContrapartida->toString());
	getRecApunte()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITAPUNTE_GATHER*/
}

void FrmEditApunte::validateFields(QWidget *sender, bool *isvalid, ValidResult *ir)
{
/// \todo {gui} saltarse el campo descripción, igual que en validSeekCode
    /*<<<<<FRMEDITAPUNTE_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	if( !sender && !pRecord->isValid( ValidResult::editing, validresult ) )
			*isvalid = false;
	if( focusWidget() != pushCuentaCuenta) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editCuentaCuenta, editCuentaDescripcion,
		getRecCuenta(), "CUENTA", "DESCRIPCION", Xtring::null) )
		scatterCuenta();
/*>>>>>FRMEDITAPUNTE_VALIDATE*/

#include "contabvalidatecuenta.inc"

    if( sender == editCuentaDescripcion ) {
        if( validSeekCode( sender, isvalid, *validresult, editCuentaCuenta, editCuentaDescripcion,
                           getRecCuenta(), "CUENTA", "DESCRIPCION", Xtring::null) )
            scatterCuenta();
    }

    // La validación de la contrapartida es casi igual que la de la cuenta, se cambia
    // editCuentaCuenta por editContrapartidaCuenta y editCuentaDescripcion por editContrapartidaDescripcion
    // La diferencia es que la contrapartida puede estar vacía


#define editCuentaCuenta editContrapartidaCuenta
#define getRecCuenta getRecContrapartida
#define scatterCuenta scatterContrapartida
#include <contabvalidatecuenta.inc>
#undef editCuentaCuenta
#undef getRecCuenta
#undef scatterCuenta

    if( sender == editContrapartidaDescripcion ) {
        if( validSeekCode( sender, isvalid, *validresult, editContrapartidaCuenta, editContrapartidaDescripcion,
                           getRecContrapartida(), "CUENTA", "DESCRIPCION", Xtring::null) )
            scatterContrapartida();
    }
    if (!ir) {
        showValidMessages(isvalid, *validresult, sender);
        delete validresult;
    }
}

void FrmEditApunte::scatterCuenta()
{
    /*<<<<<FRMEDITAPUNTE_SCATTER_CUENTA*/
	editCuentaCuenta->setText( getRecCuenta()->getValue("CUENTA") );
	editCuentaDescripcion->setText( getRecCuenta()->getValue("DESCRIPCION") );
/*>>>>>FRMEDITAPUNTE_SCATTER_CUENTA*/
}

void FrmEditApunte::pushCuentaCuenta_clicked()
{
    /*<<<<<FRMEDITAPUNTE_PUSH_CUENTA_CUENTA_CLICKED*/
	char action = mControlKeyPressed;
	if( !isEditing() || searchCuentaCuenta->mustBeReadOnly() )
		action = 'E';
	switch( action ) {
		case 'F':
		case '\0':
			editCuentaCuenta->setJustEdited( false );
			editCuentaCuenta->setCancelling();
			if( DBAPP->choose(this, getRecCuenta(), 0, dbApplication::editNone, this ) ) {
				setEdited(true);
				scatterCuenta();
				editCuentaCuenta->setJustEdited( true );
				setWiseFocus(editCuentaCuenta);
			}
			break;
		case 'M':
			{
				if( getRecCuenta()->getRecordID() ) {
					editCuentaCuenta->setJustEdited( false );
					if( DBAPP->editRecord(this,
							getRecCuenta(), 0, DataTable::updating,
							dbApplication::simpleEdition, this ) ) {
						editCuentaCuenta->setJustEdited( true );
						scatterCuenta();
					}
				setWiseFocus(editCuentaCuenta);
				}
			}
			break;
		case 'E':
			{
				if( getRecCuenta()->getRecordID() != 0 ) {
					editCuentaCuenta->setJustEdited( false );
					DBAPP->getMainWindow()->createClient( DBAPP->createEditForm(this, getRecCuenta(),
						0, DataTable::selecting, dbApplication::simpleEdition, this ) );
				}
			}
			break;
		case 'A':
			{
				RecCuenta *tmprec = static_cast<RecCuenta *>(DBAPP->createRecord( "Cuenta" ));
				editCuentaCuenta->setJustEdited( false );
				tmprec->clear( true ); // set default values
				DBAPP->setCodeNotFound( editCuentaCuenta->toString() );
				if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
					dbApplication::simpleEdition, this ) ) {
					editCuentaCuenta->setJustEdited( true );
					getRecCuenta()->copyRecord( tmprec );
					scatterCuenta();
				}
				setWiseFocus(editCuentaCuenta);
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITAPUNTE_PUSH_CUENTA_CUENTA_CLICKED*/
}

void FrmEditApunte::scatterContrapartida()
{
    editContrapartidaCuenta->setText( getRecContrapartida()->getValue("CUENTA") );
    editContrapartidaDescripcion->setText( getRecContrapartida()->getValue("DESCRIPCION") );
    editContrapartida->setText( getRecContrapartida()->getValue("CUENTA") );
}

void FrmEditApunte::specialControlKeyPressed(QWidget *sender, char key)
{
    /*<<<<<FRMEDITAPUNTE_SPECIALACTION*/
	mControlKeyPressed = key;
	FrmEditRecDetail::specialControlKeyPressed(sender,key); // calls the behaviors
	if( sender == editCuentaCuenta )
		pushCuentaCuenta_clicked();
	mControlKeyPressed = '\0';
/*>>>>>FRMEDITAPUNTE_SPECIALACTION*/
    if( sender == editContrapartidaCuenta ) {
        mControlKeyPressed = key;
        pushContrapartidaCuenta_clicked();
    }
}

void FrmEditApunte::pushContrapartidaCuenta_clicked()
{
    if( isEditing() ) {
        editContrapartidaCuenta->setCancelling();
        if( DBAPP->choose(this, getRecContrapartida(), 0, dbApplication::editNone, this ) ) {
            setEdited(true);
            scatterContrapartida();
        }
    }
}

bool FrmEditApunte::save()
{
    bool ret;
    if( (ret = FrmEditRecDetail::save()) == true ) {
        _GONG_DEBUG_PRINT(3, "Añadiendo apunte de contrapartida" );
        addContrapartida();
    }
    return ret;
}

void FrmEditApunte::addContrapartida()
{
    if( getRecContrapartida()->getRecordID() ) {
        RecApunte *apunte = static_cast<RecApunte *>(DBAPP->createRecord( "apunte" ) );
        apunte->copyRecord( getRecApunte() );
        if( apunte->findFieldDefinition( "PARTIDAPROYECTO_ID" ) )
            apunte->setValue( "PARTIDAPROYECTO_ID", 0 );
        apunte->setValue( "CUENTA_ID", getRecContrapartida()->getRecordID() );
        apunte->getRecCuenta()->copyRecord( getRecContrapartida() );
        apunte->setValue( "DEBE", getRecApunte()->getValue( "HABER" ).toMoney() );
        apunte->setValue( "HABER", getRecApunte()->getValue( "DEBE" ).toMoney() );
        apunte->setValue( "CONTRAPARTIDA", getRecCuenta()->getValue( "CUENTA" ) );
        apunte->setValue( "NUMAPUNTE", getRecApunte()->getValue( "NUMAPUNTE" ).toInt() + 1 );
        static_cast<FrmEditAsiento *>(getFrmMaster())->getRecAsiento()->getApuntes()->addRecord( apunte );
        static_cast<FrmEditAsiento *>(getFrmMaster())->refresh(); // Para que actualice los saldos
    }
}

// Calcula el saldo del asiento sin contar el debe y haber de este apunte
Money FrmEditApunte::calcSaldo() const
{
    Money debe = 0, haber = 0;
    for( unsigned int napunte = 0; napunte < getDetalles()->size(); napunte ++ ) {
        if( napunte != mNumDetalle ) {
            debe += getDetalles()->getRecord(napunte)->getValue("DEBE").toMoney();
            haber += getDetalles()->getRecord(napunte)->getValue("HABER").toMoney();
        }
    }
    return debe - haber;
}


void FrmEditApunte::pushCuadrar_clicked()
{
    Money saldo = calcSaldo();
    if( saldo <= 0 ) {
        editDebe->setText( -saldo );
        editHaber->setText( 0 );
    } else {
        editDebe->setText( 0 );
        editHaber->setText( saldo );
    }
}

/*<<<<<FRMEDITAPUNTE_FIN*/
} // namespace contab
} // namespace gong
/*>>>>>FRMEDITAPUNTE_FIN*/

