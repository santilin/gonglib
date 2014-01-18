/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de asientos
// FIELD NumAsiento int - nasientolayout
// FIELD Fecha Date - nasientolayout
// FIELD Descripcion string - nasientolayout
// FIELD Automatico bool - ndoclayout
// FIELD TipoAsiento comboint - ndoclayout
// FIELD Proyecto_ID Reference(empresa::Proyecto,Codigo,Nombre,dbApplication::InsertIfNotFound) - ndoclayout
// FIELD Contador int - ndoclayout
// FIELD Apunte FrmEditRecDetail - apunteslayout
// FIELD Debe Money - saldolayout
// FIELD Haber Money - saldolayout
// FIELD Saldo Money - saldolayout
// FIELD Notas text - notaslayout
// TYPE FrmEditRecMaster contab::Asiento
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITASIENTO_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "contabfrmeditasiento.h"
/*>>>>>FRMEDITASIENTO_INCLUDES*/

#include <gongdatatable.h>
#include <dbappfrmcustom.h>
#include "contabfrmeditapunte.h"
#include "contabmodule.h"

namespace gong {
namespace contab {

/*<<<<<FRMEDITASIENTO_CONSTRUCTOR*/
FrmEditAsiento::FrmEditAsiento(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditAsiento" );
/*>>>>>FRMEDITASIENTO_CONSTRUCTOR*/

    /*<<<<<FRMEDITASIENTO_INIT_CONTROLS*/
	QHBoxLayout *nasientolayoutLayout = new QHBoxLayout(0, 0, 6, "nasientolayoutLayout");
	QHBoxLayout *ndoclayoutLayout = new QHBoxLayout(0, 0, 6, "ndoclayoutLayout");
	QHBoxLayout *apunteslayoutLayout = new QHBoxLayout(0, 0, 6, "apunteslayoutLayout");
	QHBoxLayout *saldolayoutLayout = new QHBoxLayout(0, 0, 6, "saldolayoutLayout");
	QHBoxLayout *notaslayoutLayout = new QHBoxLayout(0, 0, 6, "notaslayoutLayout");
	editNumAsiento = addEditField( pControlsFrame, "ASIENTO", "NUMASIENTO", nasientolayoutLayout );
	editFecha = addEditField( pControlsFrame, "ASIENTO", "FECHA", nasientolayoutLayout );
	editDescripcion = addEditField( pControlsFrame, "ASIENTO", "DESCRIPCION", nasientolayoutLayout );
	checkAutomatico = addCheckField( pControlsFrame, "ASIENTO", "AUTOMATICO", ndoclayoutLayout );
	comboTipoAsiento = addComboIntField( pControlsFrame, "ASIENTO", "TIPOASIENTO", ndoclayoutLayout );

	searchProyectoCodigo = addSearchField( pControlsFrame, "PROYECTO_ID", "PROYECTO", "CODIGO", "NOMBRE", ndoclayoutLayout );
	pushProyectoCodigo = searchProyectoCodigo->getButton();
	connect( pushProyectoCodigo, SIGNAL( clicked() ), this, SLOT( pushProyectoCodigo_clicked() ) );
	editProyectoCodigo = searchProyectoCodigo->getEditCode();
	editProyectoNombre = searchProyectoCodigo->getEditDesc();
	editContador = addEditField( pControlsFrame, "ASIENTO", "CONTADOR", ndoclayoutLayout );

// frmDetails: Apunte
	QFrame *apunteFrame = new QFrame(this);
	apunteFrame->setFrameStyle(QFrame::Sunken | QFrame::Panel);
	apunteFrame->setLineWidth(4);
	QVBoxLayout *apunteFrameLayout = new QVBoxLayout(apunteFrame);
	RecApunte *apunte = getRecAsiento()->getRecApunte(-1);
	dbViewDefinitionsList apunteviews;
	DBAPP->getDatabase()->getViewsForTable( apunte, apunteviews );
	dbRecordListDataModel *apuntedm =
		new dbRecordListDataModel(getRecAsiento()->getListApunte(), apunte, apunteviews, "1=0");
	pFrmApunte = static_cast<FrmEditApunte *>(
		DBAPP->createEditDetailForm(this,
		-1, apunte, "APUNTE",apuntedm, DataTable::browsing,
		static_cast<dbApplication::EditFlags>(dbApplication::embedded + dbApplication::tableEditEvent ),
		pControlsFrame) );
	pFrmApunte->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding);
	apunteFrameLayout->addWidget( pFrmApunte );
	apunteslayoutLayout->addWidget(apunteFrame);
	editDebe = addEditField( pControlsFrame, "ASIENTO", "DEBE", saldolayoutLayout );
	editHaber = addEditField( pControlsFrame, "ASIENTO", "HABER", saldolayoutLayout );
	editSaldo = addEditField( pControlsFrame, "ASIENTO", "SALDO", saldolayoutLayout );
	editNotas = addTextField( pControlsFrame, "ASIENTO", "NOTAS", notaslayoutLayout );
	pControlsLayout->addLayout( nasientolayoutLayout );
	pControlsLayout->addLayout( ndoclayoutLayout );
	pControlsLayout->addLayout( apunteslayoutLayout );
	pControlsLayout->addLayout( saldolayoutLayout );
	pControlsLayout->addLayout( notaslayoutLayout );
/*>>>>>FRMEDITASIENTO_INIT_CONTROLS*/
    editContador->setMustBeReadOnly( true );
    editNotas->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Minimum);
}


void FrmEditAsiento::scatterProyecto()
{
    /*<<<<<FRMEDITASIENTO_SCATTER_PROYECTO*/
	editProyectoCodigo->setText( getRecProyecto()->getValue("CODIGO") );
	editProyectoNombre->setText( getRecProyecto()->getValue("NOMBRE") );
/*>>>>>FRMEDITASIENTO_SCATTER_PROYECTO*/
}

void FrmEditAsiento::pushProyectoCodigo_clicked()
{
    /*<<<<<FRMEDITASIENTO_PUSH_PROYECTO_CODIGO_CLICKED*/
	char action = mControlKeyPressed;
	if( !isEditing() || searchProyectoCodigo->mustBeReadOnly() )
		action = 'E';
	switch( action ) {
		case 'F':
		case '\0':
			editProyectoCodigo->setJustEdited( false );
			editProyectoCodigo->setCancelling();
			if( DBAPP->choose(this, getRecProyecto(), 0, dbApplication::editNone, this ) ) {
				setEdited(true);
				scatterProyecto();
				editProyectoCodigo->setJustEdited( true );
				setWiseFocus(editProyectoCodigo);
			}
			break;
		case 'M':
			{
				if( getRecProyecto()->getRecordID() ) {
					editProyectoCodigo->setJustEdited( false );
					if( DBAPP->editRecord(this,
							getRecProyecto(), 0, DataTable::updating,
							dbApplication::simpleEdition, this ) ) {
						editProyectoCodigo->setJustEdited( true );
						scatterProyecto();
					}
				setWiseFocus(editProyectoCodigo);
				}
			}
			break;
		case 'E':
			{
				if( getRecProyecto()->getRecordID() != 0 ) {
					editProyectoCodigo->setJustEdited( false );
					DBAPP->getMainWindow()->createClient( DBAPP->createEditForm(this, getRecProyecto(),
						0, DataTable::selecting, dbApplication::simpleEdition, this ) );
				}
			}
			break;
		case 'A':
			{
				empresa::RecProyecto *tmprec = static_cast<empresa::RecProyecto *>(DBAPP->createRecord( "Proyecto" ));
				editProyectoCodigo->setJustEdited( false );
				tmprec->clear( true ); // set default values
				DBAPP->setCodeNotFound( editProyectoCodigo->toString() );
				if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
					dbApplication::simpleEdition, this ) ) {
					editProyectoCodigo->setJustEdited( true );
					getRecProyecto()->copyRecord( tmprec );
					scatterProyecto();
				}
				setWiseFocus(editProyectoCodigo);
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITASIENTO_PUSH_PROYECTO_CODIGO_CLICKED*/
}
void FrmEditAsiento::specialControlKeyPressed(QWidget *sender, char key)
{
    /*<<<<<FRMEDITASIENTO_SPECIALACTION*/
	mControlKeyPressed = key;
	FrmEditRecMaster::specialControlKeyPressed(sender,key); // calls the behaviors
	if( sender == editProyectoCodigo )
		pushProyectoCodigo_clicked();
	mControlKeyPressed = '\0';
/*>>>>>FRMEDITASIENTO_SPECIALACTION*/
}

dbRecordList *FrmEditAsiento::getApuntes() const
{
    return 	getRecAsiento()->getApuntes();
}


bool FrmEditAsiento::save()
{
    getRecAsiento()->numeraApuntes();
    bool ret = FrmEditRecMaster::save();
    if ( ret ) {
        ContabModule::mLastAsientoID = getRecAsiento()->getRecordID();
        if( DBAPP->getMainWindow() ) {
            DBAPP->getMainWindow()->refreshByName( name(), "Cuenta" );
            DBAPP->getMainWindow()->refreshByName( name(), "Asiento" );
        }
    }
    return ret;
}

bool FrmEditAsiento::remove()
{
    bool borrar = true;
    if( !getRecAsiento()->getValue("REC_ORIGEN").isEmpty() ) {
        switch( msgYesNoCancel(this,
                               Xtring::printf(_("Este asiento proviene de:\n%s\n¿Estás segura de borrarlo?"),
                                              getRecAsiento()->getValue("REC_ORIGEN").toString().c_str() ) ) ) {
        case FrmBase::No:
        case FrmBase::Cancel:
            borrar = false;
            break;
        }
    }
    if( borrar )
        return FrmEditRecMaster::remove();
    else
        return false;
}


void FrmEditAsiento::scatterFields()
{
    if( isDuplicating() ) {
        getRecAsiento()->setValue( "CONTADOR", 0 );
        getRecAsiento()->setValue( "AUTOMATICO", false );
    }
    /*<<<<<FRMEDITASIENTO_SCATTER*/
	editNumAsiento->setText(getRecAsiento()->getValue("NUMASIENTO").toInt());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editNumAsiento;
	editFecha->setText(getRecAsiento()->getValue("FECHA").toDate());
	editDescripcion->setText(getRecAsiento()->getValue("DESCRIPCION").toString());
	checkAutomatico->setChecked(getRecAsiento()->getValue("AUTOMATICO").toBool());
	comboTipoAsiento->setCurrentItemByValue(getRecAsiento()->getValue("TIPOASIENTO").toInt());
	editContador->setText(getRecAsiento()->getValue("CONTADOR").toInt());
	editDebe->setText(getRecAsiento()->getValue("DEBE").toMoney());
	editHaber->setText(getRecAsiento()->getValue("HABER").toMoney());
	editSaldo->setText(getRecAsiento()->getValue("SALDO").toMoney());
	editNotas->setText(getRecAsiento()->getValue("NOTAS").toString());
	scatterProyecto();
/*>>>>>FRMEDITASIENTO_SCATTER*/
    editSaldo->setMustBeReadOnly(true); // porque despues enableEditControls lo hace readonly
    editDebe->setMustBeReadOnly(true);
    editHaber->setMustBeReadOnly(true);
    if ( isDuplicating() ) {
        editNumAsiento->setText( getLastNumAsiento() + 1 );
        getRecAsiento()->setNullValue( "REC_ORIGEN" );
    }
    if ( isInserting() ) {
        if( editNumAsiento->toInt() == 0 )
            editNumAsiento->setText( getLastNumAsiento() + 1 );
        if ( editFecha->text().isEmpty() )
            editFecha->setText( Date::currentDate() );
    }
    if( checkAutomatico->isOn() ) {
        pFrmApunte->setReadOnly( true );
    } else {
        pFrmApunte->setReadOnly( false );
    }
    pFrmApunte->addDetailIfNeeded();
    if( editContador->toInt() == 0 )
        editContador->setText( empresa::ModuleInstance->getMaxContador() );
    actSaldos();
}

void FrmEditAsiento::gatherFields()
{
    /*<<<<<FRMEDITASIENTO_GATHER*/
	getRecAsiento()->setValue( "NUMASIENTO", editNumAsiento->toInt());
	getRecAsiento()->setValue( "FECHA", editFecha->toDate());
	getRecAsiento()->setValue( "DESCRIPCION", editDescripcion->toString());
	getRecAsiento()->setValue( "AUTOMATICO", checkAutomatico->isChecked());
	getRecAsiento()->setValue( "TIPOASIENTO", comboTipoAsiento->getCurrentItemValue());
	getRecAsiento()->setValue( "PROYECTO_ID", getRecProyecto()->getRecordID() );
	getRecAsiento()->setValue( "CONTADOR", editContador->toInt());
	getRecAsiento()->setValue( "DEBE", editDebe->toMoney());
	getRecAsiento()->setValue( "HABER", editHaber->toMoney());
	getRecAsiento()->setValue( "SALDO", editSaldo->toMoney());
	getRecAsiento()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITASIENTO_GATHER*/
}

Xtring FrmEditAsiento::getLastConcepto() const
{
    Xtring ret;
    if( getApuntes()->size() == 0 )
        return Xtring::null;
    for( int i = getApuntes()->size()-1; i>=0; i-- ) {
        dbRecord *apunte = getApuntes()->at(i);
        ret = apunte->getValue("CONCEPTO").toString();
        if( !ret.isEmpty() )
            break;
    }
    return ret;
}

int FrmEditAsiento::getLastNumAsiento() const
{
    return getRecord()->getConnection()->selectInt(
               "SELECT MAX(numasiento) FROM ASIENTO " + getRecord()->getFilter("WHERE") );
}

void FrmEditAsiento::updateFromDetails( class FrmEditRecDetail *detailfrm )
{
    actSaldos();
}

/*
  Calcula la suma del debe y del haber y el saldo del asiento
  */
void FrmEditAsiento::actSaldos()
{
    dbRecordList * reclst = getApuntes();
    Money sumadebe = 0.0, sumahaber = 0.0;
    unsigned int i;
    for ( i = 0; i < reclst->size(); i++ ) {
        RecApunte *papunte = static_cast<RecApunte *>( reclst->at( i ) );
        sumadebe = sumadebe + papunte->getValue("DEBE").toMoney();
        sumahaber = sumahaber + papunte->getValue("HABER").toMoney();
    }
    editDebe->setText( sumadebe );
    editHaber->setText( sumahaber );
    editSaldo->setText( sumadebe - sumahaber );
}

void FrmEditAsiento::validateFields(QWidget *sender, bool *isvalid, ValidResult *ir)
{
    /*<<<<<FRMEDITASIENTO_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	if( !sender && !pRecord->isValid( ValidResult::editing, validresult ) )
			*isvalid = false;
	if( focusWidget() != pushProyectoCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editProyectoCodigo, editProyectoNombre,
		getRecProyecto(), "CODIGO", "NOMBRE", Xtring::null, dbApplication::SeekCodeFlags( dbApplication::InsertIfNotFound )) )
		scatterProyecto();
/*>>>>>FRMEDITASIENTO_VALIDATE*/
    if ( sender == editFecha ) {
        if( editFecha->toDate().getYear() != empresa::ModuleInstance->getEjercicio() ) {
            validresult->addError( Xtring::printf("La fecha no es del ejercicio actual: %d",
                                                  empresa::ModuleInstance->getEjercicio() ), "FECHA" );
            *isvalid = false;
        }
        Date fechacontable = empresa::ModuleInstance->getRecEmpresa()->getValue("FECHACONTABLE").toDate();
        if( fechacontable.isValid() && editFecha->toDate() < fechacontable ) {
            validresult->addError( Xtring::printf("En esta empresa no se pueden introducir asientos anteriores al: %s",
                                                  fechacontable.toString().c_str() ), "FECHA" );
            *isvalid = false;
        }
    }
    if( !sender
            && (getApuntes()->size() == 0
                || (getApuntes()->size() == 1 && getApuntes()->getRecord(0)->isEmpty() ) ) ) {
        validresult->addWarning("El asiento no tiene apuntes", "NUMASIENTO");
    }
    if( !sender && isInserting() ) {
        int contador = empresa::ModuleInstance->getMaxContador();
        if( contador > editContador->toInt() ) {
            editContador->setText( contador );
            validresult->addWarning( Xtring::printf(
                                         _("El contador ha cambiado durante la edición de este registro. El nuevo contador es %d"),
                                         contador ), "CONTADOR" );
        }
    }

    if (!ir) {
        showValidMessages(isvalid, *validresult, sender);
        delete validresult;
    }
}

/*<<<<<FRMEDITASIENTO_FIN*/
} // namespace contab
} // namespace gong
/*>>>>>FRMEDITASIENTO_FIN*/

