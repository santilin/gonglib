// // FIELD Tercero_ID Reference(Tercero,Codigo,Nombre) - terceros
// FIELD Concepto_ID Reference(ConceptoTesoreria,Codigo,Nombre) - conceptos
/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de
// FIELD Numero int - fecha
// FIELD Automatico bool - fecha
// FIELD Fecha date - fecha
// FIELD Importe Money - fecha
// FIELD CuentaOferta_ID Reference(CuentaOferta,Codigo,Nombre)
// FIELD TablaTerceros combostring - terceros
// FIELD Tercero string - terceros
// FIELD TablaConceptos combostring - conceptos
// FIELD Concepto string - conceptos
// FIELD Referencia string
// FIELD Proyecto_ID Reference(empresa::Proyecto,Codigo,Nombre) - proyectos if(empresa::ModuleInstance->usaProyectos())
// FIELD Notas text
// FIELD espacio vspacer
// TYPE FrmEditRecMaster ofertas::ApunteOferta
/*>>>>>MODULE_INFO*/
// FIELD TipoApunteTesoreria_ID Reference(tesoreria::TipoApunteTesoreria,Codigo,Nombre) - tipoapunte

/*<<<<<FRMEDITAPUNTEOFERTA_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "ofertasfrmeditapunteoferta.h"
/*>>>>>FRMEDITAPUNTEOFERTA_INCLUDES*/
#include <empresamodule.h>
#ifdef HAVE_TESORERIAMODULE
#include <tesoreriafldpedircampo.h>
#endif

namespace gong {
namespace ofertas {

/*<<<<<FRMEDITAPUNTEOFERTA_CONSTRUCTOR*/
FrmEditApunteOferta::FrmEditApunteOferta(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditApunteOferta" );
/*>>>>>FRMEDITAPUNTEOFERTA_CONSTRUCTOR*/
    searchTerceroCodigo = 0;
	editTerceroCodigo = 0;
	editTerceroNombre = 0;
    searchConceptoCodigo = 0;
	editConceptoCodigo = 0;
	editConceptoNombre = 0;
    /*<<<<<FRMEDITAPUNTEOFERTA_INIT_CONTROLS*/
	QHBoxLayout *fechaLayout = new QHBoxLayout(0, 0, 6, "fechaLayout");
	QHBoxLayout *cuentaoferta_idLayout = new QHBoxLayout(0, 0, 6, "cuentaoferta_idLayout");
	QHBoxLayout *tercerosLayout = new QHBoxLayout(0, 0, 6, "tercerosLayout");
	QHBoxLayout *conceptosLayout = new QHBoxLayout(0, 0, 6, "conceptosLayout");
	QHBoxLayout *referenciaLayout = new QHBoxLayout(0, 0, 6, "referenciaLayout");
	QHBoxLayout *proyectosLayout = new QHBoxLayout(0, 0, 6, "proyectosLayout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
	QHBoxLayout *espacioLayout = new QHBoxLayout(0, 0, 6, "espacioLayout");
	editNumero = addEditField( pControlsFrame, "APUNTEOFERTA", "NUMERO", fechaLayout );
	checkAutomatico = addCheckField( pControlsFrame, "APUNTEOFERTA", "AUTOMATICO", fechaLayout );
	editFecha = addEditField( pControlsFrame, "APUNTEOFERTA", "FECHA", fechaLayout );
	editImporte = addEditField( pControlsFrame, "APUNTEOFERTA", "IMPORTE", fechaLayout );

	searchCuentaOfertaCodigo = addSearchField( pControlsFrame, "CUENTAOFERTA_ID", "CUENTAOFERTA", "CODIGO", "NOMBRE", cuentaoferta_idLayout );
	pushCuentaOfertaCodigo = searchCuentaOfertaCodigo->getButton();
	connect( pushCuentaOfertaCodigo, SIGNAL( clicked() ), this, SLOT( pushCuentaOfertaCodigo_clicked() ) );
	editCuentaOfertaCodigo = searchCuentaOfertaCodigo->getEditCode();
	editCuentaOfertaNombre = searchCuentaOfertaCodigo->getEditDesc();
	comboTablaTerceros = addComboXtringField( pControlsFrame, "APUNTEOFERTA", "TABLATERCEROS", tercerosLayout );
	editTercero = addEditField( pControlsFrame, "APUNTEOFERTA", "TERCERO", tercerosLayout );
	comboTablaConceptos = addComboXtringField( pControlsFrame, "APUNTEOFERTA", "TABLACONCEPTOS", conceptosLayout );
	editConcepto = addEditField( pControlsFrame, "APUNTEOFERTA", "CONCEPTO", conceptosLayout );
	editReferencia = addEditField( pControlsFrame, "APUNTEOFERTA", "REFERENCIA", referenciaLayout );

if(empresa::ModuleInstance->usaProyectos()){
	searchProyectoCodigo = addSearchField( pControlsFrame, "PROYECTO_ID", "PROYECTO", "CODIGO", "NOMBRE", proyectosLayout );
	pushProyectoCodigo = searchProyectoCodigo->getButton();
	connect( pushProyectoCodigo, SIGNAL( clicked() ), this, SLOT( pushProyectoCodigo_clicked() ) );
	editProyectoCodigo = searchProyectoCodigo->getEditCode();
	editProyectoNombre = searchProyectoCodigo->getEditDesc();
}
	editNotas = addTextField( pControlsFrame, "APUNTEOFERTA", "NOTAS", notasLayout );
	QSpacerItem* espacio = new QSpacerItem ( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
	espacioLayout->addItem ( espacio);
	pControlsLayout->addLayout( fechaLayout );
	pControlsLayout->addLayout( cuentaoferta_idLayout );
	pControlsLayout->addLayout( tercerosLayout );
	pControlsLayout->addLayout( conceptosLayout );
	pControlsLayout->addLayout( referenciaLayout );
	pControlsLayout->addLayout( proyectosLayout );
	pControlsLayout->addLayout( notasLayout );
	pControlsLayout->addLayout( espacioLayout );
/*>>>>>FRMEDITAPUNTEOFERTA_INIT_CONTROLS*/
    editNumero->setMustBeReadOnly(true);
    pTercerosLayout = tercerosLayout; // Lo necesitamos en el validate
    pConceptosLayout = conceptosLayout; // Lo necesitamos en el validate
}

void FrmEditApunteOferta::scatterFields()
{
/*<<<<<FRMEDITAPUNTEOFERTA_SCATTER*/
	editNumero->setText(getRecApunteOferta()->getValue("NUMERO").toInt());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editNumero;
	checkAutomatico->setChecked(getRecApunteOferta()->getValue("AUTOMATICO").toBool());
	editFecha->setText(getRecApunteOferta()->getValue("FECHA").toDate());
	editImporte->setText(getRecApunteOferta()->getValue("IMPORTE").toMoney());
	comboTablaTerceros->setText(getRecApunteOferta()->getValue("TABLATERCEROS").toString());
	editTercero->setText(getRecApunteOferta()->getValue("TERCERO").toString());
	comboTablaConceptos->setText(getRecApunteOferta()->getValue("TABLACONCEPTOS").toString());
	editConcepto->setText(getRecApunteOferta()->getValue("CONCEPTO").toString());
	editReferencia->setText(getRecApunteOferta()->getValue("REFERENCIA").toString());
	editNotas->setText(getRecApunteOferta()->getValue("NOTAS").toString());
	scatterCuentaOferta();
if(empresa::ModuleInstance->usaProyectos()){
	scatterProyecto();
}
/*>>>>>FRMEDITAPUNTEOFERTA_SCATTER*/
    bool isvalid = true;
	if (isFirstScatter() ) {
		scatterTercero();
		scatterConcepto();
	}
    if( isInserting() ) {
        if( editNumero->toInt() == 0 ) {
            editNumero->setText( getRecord()->selectNextInt( "NUMERO" ) );
        }
        if( editFecha->toDate().isNull() ) {
            editFecha->setText( Date::currentDate() );
        }
    }
    bool esAutomatico = checkAutomatico->isOn();
	if( isFirstScatter() ) {
		editFecha->setMustBeReadOnly(esAutomatico);
		editImporte->setMustBeReadOnly(esAutomatico);
		editTercero->setMustBeReadOnly(esAutomatico);
		editConcepto->setMustBeReadOnly(esAutomatico);
		searchCuentaOfertaCodigo->setMustBeReadOnly(esAutomatico);
		comboTablaTerceros->setVisible(!esAutomatico);
		comboTablaConceptos->setVisible(!esAutomatico);
		editReferencia->setMustBeReadOnly(esAutomatico);
		searchTipoApunteTesoreriaCodigo->setVisible(!esAutomatico);
	}
	if( searchTerceroCodigo )
		searchTerceroCodigo->setMustBeReadOnly(esAutomatico);
	if( searchConceptoCodigo )
		searchConceptoCodigo->setMustBeReadOnly(esAutomatico);
}

void FrmEditApunteOferta::gatherFields()
{
    /*<<<<<FRMEDITAPUNTEOFERTA_GATHER*/
	getRecApunteOferta()->setValue( "NUMERO", editNumero->toInt());
	getRecApunteOferta()->setValue( "AUTOMATICO", checkAutomatico->isChecked());
	getRecApunteOferta()->setValue( "FECHA", editFecha->toDate());
	getRecApunteOferta()->setValue( "IMPORTE", editImporte->toMoney());
	getRecApunteOferta()->setValue( "CUENTAOFERTA_ID", getRecCuentaOferta()->getRecordID() );
	getRecApunteOferta()->setValue( "TABLATERCEROS", comboTablaTerceros->toString());
	getRecApunteOferta()->setValue( "TERCERO", editTercero->toString());
	getRecApunteOferta()->setValue( "TABLACONCEPTOS", comboTablaConceptos->toString());
	getRecApunteOferta()->setValue( "CONCEPTO", editConcepto->toString());
	getRecApunteOferta()->setValue( "REFERENCIA", editReferencia->toString());
if(empresa::ModuleInstance->usaProyectos()){
	getRecApunteOferta()->setValue( "PROYECTO_ID", getRecProyecto()->getRecordID() );
}
	getRecApunteOferta()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITAPUNTEOFERTA_GATHER*/
    getRecApunteOferta()->setValue( "TERCERO_ID", getRecTercero() ? getRecTercero()->getRecordID() : 0 );
    getRecApunteOferta()->setValue( "CONCEPTO_ID", getRecConcepto() ? getRecConcepto()->getRecordID() : 0 );
}


void FrmEditApunteOferta::specialControlKeyPressed(QWidget *sender, char key)
{
/*<<<<<FRMEDITAPUNTEOFERTA_SPECIALACTION*/
	mControlKeyPressed = key;
	FrmEditRecMaster::specialControlKeyPressed(sender,key); // calls the behaviors
	if( sender == editCuentaOfertaCodigo )
		pushCuentaOfertaCodigo_clicked();
if(empresa::ModuleInstance->usaProyectos()){
	if( sender == editProyectoCodigo )
		pushProyectoCodigo_clicked();
}
	mControlKeyPressed = '\0';
/*>>>>>FRMEDITAPUNTEOFERTA_SPECIALACTION*/
	mControlKeyPressed = key;
	if( sender == editConceptoCodigo )
		pushConceptoCodigo_clicked();
	if( sender == editTerceroCodigo )
		pushTerceroCodigo_clicked();
	mControlKeyPressed = '\0';
}



void FrmEditApunteOferta::scatterProyecto()
{
/*<<<<<FRMEDITAPUNTEOFERTA_SCATTER_PROYECTO*/
	editProyectoCodigo->setText( getRecProyecto()->getValue("CODIGO") );
	editProyectoNombre->setText( getRecProyecto()->getValue("NOMBRE") );
/*>>>>>FRMEDITAPUNTEOFERTA_SCATTER_PROYECTO*/
}
void FrmEditApunteOferta::pushProyectoCodigo_clicked()
{
    /*<<<<<FRMEDITAPUNTEOFERTA_PUSH_PROYECTO_CODIGO_CLICKED*/
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
					DBAPP->createClient( DBAPP->createEditForm(this, getRecProyecto(),
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
/*>>>>>FRMEDITAPUNTEOFERTA_PUSH_PROYECTO_CODIGO_CLICKED*/
}


void FrmEditApunteOferta::scatterTercero()
{
    if( getRecTercero() ) {
        editTerceroCodigo->setText( getRecTercero()->getValue( getRecTercero()->getTableDefinition()->getCodeField() ) );
        editTerceroNombre->setText( getRecTercero()->getValue( getRecTercero()->getTableDefinition()->getDescField() ) );
        if (editTercero->toString().isEmpty() )
            editTercero->setText( getRecTercero()->getValue( getRecTercero()->getTableDefinition()->getDescField() ) );
    }
}

void FrmEditApunteOferta::pushTerceroCodigo_clicked()
{
    char action = mControlKeyPressed;
    if( !isEditing() || searchTerceroCodigo->mustBeReadOnly() )
        action = 'E';
    switch( action ) {
    case 'F':
    case '\0':
        editTerceroCodigo->setJustEdited( false );
        editTerceroCodigo->setCancelling();
        if( DBAPP->choose(this, getRecTercero(), 0, dbApplication::editNone, this ) ) {
            setEdited(true);
            scatterTercero();
            editTerceroCodigo->setJustEdited( true );
            setWiseFocus(editTerceroCodigo);
        }
        break;
    case 'M':
    {
        if( getRecTercero()->getRecordID() ) {
            editTerceroCodigo->setJustEdited( false );
            if( DBAPP->editRecord(this,
                                  getRecTercero(), 0, DataTable::updating,
                                  dbApplication::simpleEdition, this ) ) {
                editTerceroCodigo->setJustEdited( true );
                scatterTercero();
            }
            setWiseFocus(editTerceroCodigo);
        }
    }
    break;
    case 'E':
    {
        if( getRecTercero()->getRecordID() != 0 ) {
            editTerceroCodigo->setJustEdited( false );
            DBAPP->createClient( DBAPP->createEditForm(this, getRecTercero(),
                                                  0, DataTable::selecting, dbApplication::simpleEdition, this ) );
        }
    }
    break;
    case 'A':
    {
        RecTercero *tmprec = static_cast<RecTercero *>(DBAPP->createRecord( getRecTercero()->getTableName() ));
        editTerceroCodigo->setJustEdited( false );
        tmprec->clear( true ); // set default values
        DBAPP->setCodeNotFound( editTerceroCodigo->toString() );
        if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
                              dbApplication::simpleEdition, this ) ) {
            editTerceroCodigo->setJustEdited( true );
            getRecTercero()->copyRecord( tmprec );
            scatterTercero();
        }
        setWiseFocus(editTerceroCodigo);
        DBAPP->setCodeNotFound( Xtring() );
    }
    break;
    }
}

void FrmEditApunteOferta::scatterConcepto()
{
    if( getRecConcepto() ) {
        editConceptoCodigo->setText( getRecConcepto()->getValue( getRecConcepto()->getTableDefinition()->getCodeField() ) );
        editConceptoNombre->setText( getRecConcepto()->getValue( getRecConcepto()->getTableDefinition()->getDescField() ) );
        if (editConcepto->toString().isEmpty() )
            editConcepto->setText( getRecConcepto()->getValue( getRecConcepto()->getTableDefinition()->getDescField() ) );
    }
}

void FrmEditApunteOferta::pushConceptoCodigo_clicked()
{
    char action = mControlKeyPressed;
    if( !isEditing() || searchConceptoCodigo->mustBeReadOnly() )
        action = 'E';
    switch( action ) {
    case 'F':
    case '\0':
        editConceptoCodigo->setJustEdited( false );
        editConceptoCodigo->setCancelling();
        if( DBAPP->choose(this, getRecConcepto(), 0, dbApplication::editNone, this ) ) {
            setEdited(true);
            scatterConcepto();
            editConceptoCodigo->setJustEdited( true );
            setWiseFocus(editConceptoCodigo);
        }
        break;
    case 'M':
    {
        if( getRecConcepto()->getRecordID() ) {
            editConceptoCodigo->setJustEdited( false );
            if( DBAPP->editRecord(this,
                                  getRecConcepto(), 0, DataTable::updating,
                                  dbApplication::simpleEdition, this ) ) {
                editConceptoCodigo->setJustEdited( true );
                scatterConcepto();
            }
            setWiseFocus(editConceptoCodigo);
        }
    }
    break;
    case 'E':
    {
        if( getRecConcepto()->getRecordID() != 0 ) {
            editConceptoCodigo->setJustEdited( false );
            DBAPP->createClient( DBAPP->createEditForm(this, getRecConcepto(),
                                                  0, DataTable::selecting, dbApplication::simpleEdition, this ) );
        }
    }
    break;
    case 'A':
    {
        RecConcepto *tmprec = static_cast<RecConcepto *>(DBAPP->createRecord( getRecConcepto()->getTableName() ));
        editConceptoCodigo->setJustEdited( false );
        tmprec->clear( true ); // set default values
        DBAPP->setCodeNotFound( editConceptoCodigo->toString() );
        if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
                              dbApplication::simpleEdition, this ) ) {
            editConceptoCodigo->setJustEdited( true );
            getRecConcepto()->copyRecord( tmprec );
            scatterConcepto();
        }
        setWiseFocus(editConceptoCodigo);
        DBAPP->setCodeNotFound( Xtring() );
    }
    break;
    }
}

void FrmEditApunteOferta::scatterTipoApunteTesoreria()
{
/*<<<<<FRMEDITAPUNTEOFERTA_SCATTER_TIPOAPUNTETESORERIA*/
	editTipoApunteTesoreriaCodigo->setText( getRecTipoApunteTesoreria()->getValue("CODIGO") );
	editTipoApunteTesoreriaNombre->setText( getRecTipoApunteTesoreria()->getValue("NOMBRE") );
/*>>>>>FRMEDITAPUNTEOFERTA_SCATTER_TIPOAPUNTETESORERIA*/
}
void FrmEditApunteOferta::pushTipoApunteTesoreriaCodigo_clicked()
{
/*<<<<<FRMEDITAPUNTEOFERTA_PUSH_TIPOAPUNTETESORERIA_CODIGO_CLICKED*/
	char action = mControlKeyPressed;
	if( !isEditing() || searchTipoApunteTesoreriaCodigo->mustBeReadOnly() )
		action = 'E';
	switch( action ) {
		case 'F':
		case '\0':
			editTipoApunteTesoreriaCodigo->setJustEdited( false );
			editTipoApunteTesoreriaCodigo->setCancelling();
			if( DBAPP->choose(this, getRecTipoApunteTesoreria(), 0, dbApplication::editNone, this ) ) {
				setEdited(true);
				scatterTipoApunteTesoreria();
				editTipoApunteTesoreriaCodigo->setJustEdited( true );
				setWiseFocus(editTipoApunteTesoreriaCodigo);
			}
			break;
		case 'M':
			{
				if( getRecTipoApunteTesoreria()->getRecordID() ) {
					editTipoApunteTesoreriaCodigo->setJustEdited( false );
					if( DBAPP->editRecord(this,
							getRecTipoApunteTesoreria(), 0, DataTable::updating,
							dbApplication::simpleEdition, this ) ) {
						editTipoApunteTesoreriaCodigo->setJustEdited( true );
						scatterTipoApunteTesoreria();
					}
				setWiseFocus(editTipoApunteTesoreriaCodigo);
				}
			}
			break;
		case 'E':
			{
				if( getRecTipoApunteTesoreria()->getRecordID() != 0 ) {
					editTipoApunteTesoreriaCodigo->setJustEdited( false );
					DBAPP->createClient( DBAPP->createEditForm(this, getRecTipoApunteTesoreria(),
						0, DataTable::selecting, dbApplication::simpleEdition, this ) );
				}
			}
			break;
		case 'A':
			{
				tesoreria::RecTipoApunteTesoreria *tmprec = static_cast<tesoreria::RecTipoApunteTesoreria *>(DBAPP->createRecord( "TipoApunteTesoreria" ));
				editTipoApunteTesoreriaCodigo->setJustEdited( false );
				tmprec->clear( true ); // set default values
				DBAPP->setCodeNotFound( editTipoApunteTesoreriaCodigo->toString() );
				if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
					dbApplication::simpleEdition, this ) ) {
					editTipoApunteTesoreriaCodigo->setJustEdited( true );
					getRecTipoApunteTesoreria()->copyRecord( tmprec );
					scatterTipoApunteTesoreria();
				}
				setWiseFocus(editTipoApunteTesoreriaCodigo);
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITAPUNTEOFERTA_PUSH_TIPOAPUNTETESORERIA_CODIGO_CLICKED*/
}

void FrmEditApunteOferta::scatterCuentaOferta()
{
/*<<<<<FRMEDITAPUNTEOFERTA_SCATTER_CUENTAOFERTA*/
	editCuentaOfertaCodigo->setText( getRecCuentaOferta()->getValue("CODIGO") );
	editCuentaOfertaNombre->setText( getRecCuentaOferta()->getValue("NOMBRE") );
/*>>>>>FRMEDITAPUNTEOFERTA_SCATTER_CUENTAOFERTA*/
}
void FrmEditApunteOferta::pushCuentaOfertaCodigo_clicked()
{
/*<<<<<FRMEDITAPUNTEOFERTA_PUSH_CUENTAOFERTA_CODIGO_CLICKED*/
	char action = mControlKeyPressed;
	if( !isEditing() || searchCuentaOfertaCodigo->mustBeReadOnly() )
		action = 'E';
	switch( action ) {
		case 'F':
		case '\0':
			editCuentaOfertaCodigo->setJustEdited( false );
			editCuentaOfertaCodigo->setCancelling();
			if( DBAPP->choose(this, getRecCuentaOferta(), 0, dbApplication::editNone, this ) ) {
				setEdited(true);
				scatterCuentaOferta();
				editCuentaOfertaCodigo->setJustEdited( true );
				setWiseFocus(editCuentaOfertaCodigo);
			}
			break;
		case 'M':
			{
				if( getRecCuentaOferta()->getRecordID() ) {
					editCuentaOfertaCodigo->setJustEdited( false );
					if( DBAPP->editRecord(this,
							getRecCuentaOferta(), 0, DataTable::updating,
							dbApplication::simpleEdition, this ) ) {
						editCuentaOfertaCodigo->setJustEdited( true );
						scatterCuentaOferta();
					}
				setWiseFocus(editCuentaOfertaCodigo);
				}
			}
			break;
		case 'E':
			{
				if( getRecCuentaOferta()->getRecordID() != 0 ) {
					editCuentaOfertaCodigo->setJustEdited( false );
					DBAPP->createClient( DBAPP->createEditForm(this, getRecCuentaOferta(),
						0, DataTable::selecting, dbApplication::simpleEdition, this ) );
				}
			}
			break;
		case 'A':
			{
				RecCuentaOferta *tmprec = static_cast<RecCuentaOferta *>(DBAPP->createRecord( "CuentaOferta" ));
				editCuentaOfertaCodigo->setJustEdited( false );
				tmprec->clear( true ); // set default values
				DBAPP->setCodeNotFound( editCuentaOfertaCodigo->toString() );
				if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
					dbApplication::simpleEdition, this ) ) {
					editCuentaOfertaCodigo->setJustEdited( true );
					getRecCuentaOferta()->copyRecord( tmprec );
					scatterCuentaOferta();
				}
				setWiseFocus(editCuentaOfertaCodigo);
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITAPUNTEOFERTA_PUSH_CUENTAOFERTA_CODIGO_CLICKED*/
}


void FrmEditApunteOferta::validateFields(QWidget *sender, bool *isvalid, ValidResult *ir)
{
/*<<<<<FRMEDITAPUNTEOFERTA_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	if( !sender && !pRecord->validate( ValidResult::editing) ) {
		validresult->append( pRecord->getErrors() );
		*isvalid = false;
	}
	if( focusWidget() != pushCuentaOfertaCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editCuentaOfertaCodigo, editCuentaOfertaNombre,
		getRecCuentaOferta(), "CODIGO", "NOMBRE", Xtring::null) )
		scatterCuentaOferta();
if(empresa::ModuleInstance->usaProyectos()){
	if( focusWidget() != pushProyectoCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editProyectoCodigo, editProyectoNombre,
		getRecProyecto(), "CODIGO", "NOMBRE", Xtring::null) )
		scatterProyecto();
}
/*>>>>>FRMEDITAPUNTEOFERTA_VALIDATE*/
    if( getRecTercero() && focusWidget() != pushTerceroCodigo) // To avoid triggering the validating if the button is pressed
        if( validSeekCode( sender, isvalid, *validresult, editTerceroCodigo, editTerceroNombre, getRecTercero(),
                           getRecTercero()->getTableDefinition()->getCodeField(),
                           getRecTercero()->getTableDefinition()->getDescField(), Xtring::null) ) {
            scatterTercero();
        }
    if( getRecConcepto() && focusWidget() != pushConceptoCodigo) // To avoid triggering the validating if the button is pressed
        if( validSeekCode( sender, isvalid, *validresult, editConceptoCodigo, editConceptoNombre, getRecConcepto(),
                           getRecConcepto()->getTableDefinition()->getCodeField(),
                           getRecConcepto()->getTableDefinition()->getDescField(), Xtring::null) )
            scatterConcepto();
    if( sender == comboTablaTerceros ) {
        Xtring tablaterceros = comboTablaTerceros->getCurrentItemValue(), mFldTercCodigo = "CODIGO", mFldTercDesc = "NOMBRE";
        getRecord()->setValue("TABLATERCEROS", tablaterceros );
        if( comboTablaTerceros->getCurrentItemValue().isEmpty() ) {
            if (searchTerceroCodigo )
                searchTerceroCodigo->setVisible( false );
            editTercero->setVisible( true );
            setTabOrder( comboTablaTerceros, editTercero );
            setTabOrder( editTercero, editReferencia );
        } else {
            removeControl( searchTerceroCodigo );
            dbTableDefinition *tbldef = DBAPP->getDatabase()->findTableDefinition( tablaterceros );
            dbFieldDefinition *flddeffc = tbldef->findFieldByFlags( dbFieldDefinition::CODE );
            if( flddeffc )
                mFldTercCodigo = flddeffc->getName();
            dbFieldDefinition *flddeffd = tbldef->findFieldByFlags( dbFieldDefinition::DESCRIPTION );
            if( flddeffd )
                mFldTercDesc = flddeffd->getName();
            searchTerceroCodigo = addSearchField( pControlsFrame, "TERCERO_ID", tablaterceros,
                                                  mFldTercCodigo, mFldTercDesc, pTercerosLayout );
            pushTerceroCodigo = searchTerceroCodigo->getButton();
            connect( pushTerceroCodigo, SIGNAL( clicked() ), this, SLOT( pushTerceroCodigo_clicked() ), Qt::UniqueConnection );
            editTerceroCodigo = searchTerceroCodigo->getEditCode();
			connect( editTerceroCodigo, SIGNAL( specialControlKeyPressed( QWidget *, char ) ),
                 this, SLOT( specialControlKeyPressed( QWidget *, char ) ), Qt::UniqueConnection );
            editTerceroNombre = searchTerceroCodigo->getEditDesc();
            searchTerceroCodigo->setVisible( true );
            editTercero->setVisible( false );
            searchTerceroCodigo->getButton()->setText( toGUI(tbldef->getDescSingular()) );
            setTabOrder( comboTablaTerceros, editTerceroCodigo );
            setTabOrder( editTerceroCodigo, comboTablaConceptos );
			resize(0,0);
			DBAPP->processEvents();
        }
    }
    if( sender == comboTablaConceptos ) {
        Xtring tablaterceros = comboTablaConceptos->getCurrentItemValue(), mFldTercCodigo = "CODIGO", mFldTercDesc = "NOMBRE";
        getRecord()->setValue("TABLACONCEPTOS", tablaterceros );
        if( comboTablaConceptos->getCurrentItemValue().isEmpty() ) {
            if (searchConceptoCodigo )
                searchConceptoCodigo->setVisible( false );
            editConcepto->setVisible( true );
            setTabOrder( comboTablaConceptos, editConcepto );
            setTabOrder( editConcepto, editReferencia );
        } else {
            removeControl( searchConceptoCodigo );
            dbTableDefinition *tbldef = DBAPP->getDatabase()->findTableDefinition( tablaterceros );
            dbFieldDefinition *flddeffc = tbldef->findFieldByFlags( dbFieldDefinition::CODE );
            if( flddeffc )
                mFldTercCodigo = flddeffc->getName();
            dbFieldDefinition *flddeffd = tbldef->findFieldByFlags( dbFieldDefinition::DESCRIPTION );
            if( flddeffd )
                mFldTercDesc = flddeffd->getName();
            searchConceptoCodigo = addSearchField( pControlsFrame, "CONCEPTO_ID", tablaterceros,
                                                   mFldTercCodigo, mFldTercDesc, pConceptosLayout );
            pushConceptoCodigo = searchConceptoCodigo->getButton();
            connect( pushConceptoCodigo, SIGNAL( clicked() ), this, SLOT( pushConceptoCodigo_clicked() ), Qt::UniqueConnection );
            editConceptoCodigo = searchConceptoCodigo->getEditCode();
			connect( editConceptoCodigo, SIGNAL( specialControlKeyPressed( QWidget *, char ) ),
                 this, SLOT( specialControlKeyPressed( QWidget *, char ) ), Qt::UniqueConnection );
            editConceptoNombre = searchConceptoCodigo->getEditDesc();
//			searchConceptoCodigo->setMustBeReadOnly(true);

            searchConceptoCodigo->setVisible( true );
            editConcepto->setVisible( false );
            searchConceptoCodigo->getButton()->setText( toGUI(tbldef->getDescSingular()));
            setTabOrder( comboTablaConceptos, editConceptoCodigo );
            setTabOrder( editConceptoCodigo, editReferencia );
			resize(0,0);
			DBAPP->processEvents();
        }
    }
    if( sender == editTipoApunteTesoreriaCodigo && editTipoApunteTesoreriaCodigo->isJustEdited() ) {
        changeTipoApunte();
    }
    if( !sender ) {
        if( comboTablaTerceros->getCurrentItemValue() == "CUENTAOFERTA"
                && getRecCuentaOferta()->getRecordID() == getRecTercero()->getRecordID() ) {
            validresult->addError( _("La cuenta de tesorería y la del tercero no pueden ser la misma"), "CUENTAOFERTA");
            *isvalid = false;
        }
    }
    if( !ir ) {
        showValidMessages(isvalid, *validresult, sender);
        delete validresult;
    }
}


void FrmEditApunteOferta::changeTipoApunte()
{
    QWidget *control = 0;
    XtringList camposapedir;
    camposapedir << "FECHA" << "REFERENCIA" << "IMPORTE"
                 << "CUENTA" << "TABLATERCEROS" << "TERCERO" << "TABLACONCEPTOS" << "CONCEPTO"
                 << "NOTAS";
    for( XtringList::const_iterator it = camposapedir.begin(); it != camposapedir.end(); ++it ) {
        int pedir = getRecTipoApunteTesoreria()->getValue("PEDIR" + *it).toInt();
        Xtring fldname = *it;
        if( *it == "TERCERO" && !comboTablaTerceros->toString().isEmpty() ) {
            fldname = "push_" + searchTerceroCodigo->getTableName() + "_id_" + searchTerceroCodigo->getFldCode();
        } else if( *it == "CONCEPTO" && !comboTablaConceptos->toString().isEmpty() ) {
            fldname = "push_" + searchConceptoCodigo->getTableName() + "_id_" + searchConceptoCodigo->getFldCode();
        } else if( *it == "CUENTA" ) {
            fldname = "push_CUENTAOFERTA_id_CODIGO";
        }
        Variant value( getRecTipoApunteTesoreria()->getValue(*it) );
        switch( pedir ) {
		case tesoreria::FldPedirCampo::Pedir:
            control = findControl(fldname);
            enableEditControl(control, true);
            break;
        case tesoreria::FldPedirCampo::FijarValor:
            control = fixControl(fldname, value);
            enableEditControl(control, false);
            break;
        case tesoreria::FldPedirCampo::SugerirValor:
            control = fixControl(fldname, value);
            enableEditControl(control, true);
            break;
        case tesoreria::FldPedirCampo::DejarVacio:
            control = fixControl(fldname, Variant());
            enableEditControl(control, false);
            break;
        case tesoreria::FldPedirCampo::Buscar:
            ///@todo buscar field = value y pasarlo al campo. si es un searchbox, pasar la condición tal cual
            // Sólo si es un searchField
        {
             Xtring sfield, svalue;
            value.toString().splitIn2(sfield, svalue, "=");
        }
        break;
        case tesoreria::FldPedirCampo::FijarOcultar:
            control = fixControl(fldname, value);
            control->hide();
            break;
        case tesoreria::FldPedirCampo::DejarVacioOcultar:
            control = fixControl(fldname, Variant());
            control->hide();
            break;
        }
    }
}

bool FrmEditApunteOferta::canBeginEdit(EditMode newmode)
{
	if( newmode == DataTable::deleting && getRecord()->getValue("AUTOMATICO").toBool() ) {
		msgOk(this, Xtring::printf("No se puede borrar este apunte porque proviene de %s",
			DBAPP->getTableDescSingular( getRecord()->getValue("TABLADOCUMENTOS").toString(), "una").c_str() ) );
		return false;
	}
	return FrmEditRecMaster::canBeginEdit(newmode);
}


/*<<<<<FRMEDITAPUNTEOFERTA_FIN*/
} // namespace ofertas
} // namespace gong
/*>>>>>FRMEDITAPUNTEOFERTA_FIN*/
