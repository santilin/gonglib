/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de 
// FIELD Numero int - numero
// FIELD Automatico bool - numero
// FIELD Fecha date - numero
// FIELD TipoApunteTesoreria_ID Reference(TipoApunteTesoreria,Codigo,Nombre) - numero
// FIELD TablaTerceros combostring - terceros
// FIELD Tercero_ID Reference(CuentaTesoreria,Codigo,Nombre) - terceros
// FIELD Tercero string - tercerosextra
// FIELD TablaConceptos combostring - conceptos
// FIELD Concepto_ID Reference(ConceptoTesoreria,Codigo,Nombre) - conceptos
// FIELD Concepto string - conceptosextra
// FIELD Importe Money - money
// FIELD Referencia string
// FIELD Proyecto_ID Reference(empresa::Proyecto,Codigo,Nombre)
// FIELD Notas text
// TYPE FrmEditRecMaster tesoreria::ApunteTesoreria 
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITAPUNTETESORERIA_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "tesoreriafrmeditapuntetesoreria.h"
/*>>>>>FRMEDITAPUNTETESORERIA_INCLUDES*/

namespace gong {
namespace tesoreria {

/*<<<<<FRMEDITAPUNTETESORERIA_CONSTRUCTOR*/
FrmEditApunteTesoreria::FrmEditApunteTesoreria(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditApunteTesoreria" );
/*>>>>>FRMEDITAPUNTETESORERIA_CONSTRUCTOR*/
/*<<<<<FRMEDITAPUNTETESORERIA_INIT_CONTROLS*/
	QHBoxLayout *numeroLayout = new QHBoxLayout(0, 0, 6, "numeroLayout");
	QHBoxLayout *tercerosLayout = new QHBoxLayout(0, 0, 6, "tercerosLayout");
	QHBoxLayout *tercerosextraLayout = new QHBoxLayout(0, 0, 6, "tercerosextraLayout");
	QHBoxLayout *conceptosLayout = new QHBoxLayout(0, 0, 6, "conceptosLayout");
	QHBoxLayout *conceptosextraLayout = new QHBoxLayout(0, 0, 6, "conceptosextraLayout");
	QHBoxLayout *moneyLayout = new QHBoxLayout(0, 0, 6, "moneyLayout");
	QHBoxLayout *referenciaLayout = new QHBoxLayout(0, 0, 6, "referenciaLayout");
	QHBoxLayout *proyecto_idLayout = new QHBoxLayout(0, 0, 6, "proyecto_idLayout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
	editNumero = addEditField( pControlsFrame, "APUNTETESORERIA", "NUMERO", numeroLayout );
	checkAutomatico = addCheckField( pControlsFrame, "APUNTETESORERIA", "AUTOMATICO", numeroLayout );
	editFecha = addEditField( pControlsFrame, "APUNTETESORERIA", "FECHA", numeroLayout );

	searchTipoApunteTesoreriaCodigo = addSearchField( pControlsFrame, "TIPOAPUNTETESORERIA_ID", "TIPOAPUNTETESORERIA", "CODIGO", "NOMBRE", numeroLayout );
	pushTipoApunteTesoreriaCodigo = searchTipoApunteTesoreriaCodigo->getButton();
	connect( pushTipoApunteTesoreriaCodigo, SIGNAL( clicked() ), this, SLOT( pushTipoApunteTesoreriaCodigo_clicked() ) );
	editTipoApunteTesoreriaCodigo = searchTipoApunteTesoreriaCodigo->getEditCode();
	editTipoApunteTesoreriaNombre = searchTipoApunteTesoreriaCodigo->getEditDesc();
	comboTablaTerceros = addComboXtringField( pControlsFrame, "APUNTETESORERIA", "TABLATERCEROS", tercerosLayout );

	searchTerceroCodigo = addSearchField( pControlsFrame, "TERCERO_ID", "CUENTATESORERIA", "CODIGO", "NOMBRE", tercerosLayout );
	pushTerceroCodigo = searchTerceroCodigo->getButton();
	connect( pushTerceroCodigo, SIGNAL( clicked() ), this, SLOT( pushTerceroCodigo_clicked() ) );
	editTerceroCodigo = searchTerceroCodigo->getEditCode();
	editTerceroNombre = searchTerceroCodigo->getEditDesc();
	editTercero = addEditField( pControlsFrame, "APUNTETESORERIA", "TERCERO", tercerosextraLayout );
	comboTablaConceptos = addComboXtringField( pControlsFrame, "APUNTETESORERIA", "TABLACONCEPTOS", conceptosLayout );

	searchConceptoCodigo = addSearchField( pControlsFrame, "CONCEPTO_ID", "CONCEPTOTESORERIA", "CODIGO", "NOMBRE", conceptosLayout );
	pushConceptoCodigo = searchConceptoCodigo->getButton();
	connect( pushConceptoCodigo, SIGNAL( clicked() ), this, SLOT( pushConceptoCodigo_clicked() ) );
	editConceptoCodigo = searchConceptoCodigo->getEditCode();
	editConceptoNombre = searchConceptoCodigo->getEditDesc();
	editConcepto = addEditField( pControlsFrame, "APUNTETESORERIA", "CONCEPTO", conceptosextraLayout );
	editImporte = addEditField( pControlsFrame, "APUNTETESORERIA", "IMPORTE", moneyLayout );
	editReferencia = addEditField( pControlsFrame, "APUNTETESORERIA", "REFERENCIA", referenciaLayout );

	searchProyectoCodigo = addSearchField( pControlsFrame, "PROYECTO_ID", "PROYECTO", "CODIGO", "NOMBRE", proyecto_idLayout );
	pushProyectoCodigo = searchProyectoCodigo->getButton();
	connect( pushProyectoCodigo, SIGNAL( clicked() ), this, SLOT( pushProyectoCodigo_clicked() ) );
	editProyectoCodigo = searchProyectoCodigo->getEditCode();
	editProyectoNombre = searchProyectoCodigo->getEditDesc();
	editNotas = addTextField( pControlsFrame, "APUNTETESORERIA", "NOTAS", notasLayout );
	pControlsLayout->addLayout( numeroLayout );
	pControlsLayout->addLayout( tercerosLayout );
	pControlsLayout->addLayout( tercerosextraLayout );
	pControlsLayout->addLayout( conceptosLayout );
	pControlsLayout->addLayout( conceptosextraLayout );
	pControlsLayout->addLayout( moneyLayout );
	pControlsLayout->addLayout( referenciaLayout );
	pControlsLayout->addLayout( proyecto_idLayout );
	pControlsLayout->addLayout( notasLayout );
/*>>>>>FRMEDITAPUNTETESORERIA_INIT_CONTROLS*/
}

void FrmEditApunteTesoreria::scatterFields()
{
/*<<<<<FRMEDITAPUNTETESORERIA_SCATTER*/
	editNumero->setText(getRecApunteTesoreria()->getValue("NUMERO").toInt());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editNumero;
	checkAutomatico->setChecked(getRecApunteTesoreria()->getValue("AUTOMATICO").toBool());
	editFecha->setText(getRecApunteTesoreria()->getValue("FECHA").toDate());
	comboTablaTerceros->setText(getRecApunteTesoreria()->getValue("TABLATERCEROS").toString());
	editTercero->setText(getRecApunteTesoreria()->getValue("TERCERO").toString());
	comboTablaConceptos->setText(getRecApunteTesoreria()->getValue("TABLACONCEPTOS").toString());
	editConcepto->setText(getRecApunteTesoreria()->getValue("CONCEPTO").toString());
	editImporte->setText(getRecApunteTesoreria()->getValue("IMPORTE").toMoney());
	editReferencia->setText(getRecApunteTesoreria()->getValue("REFERENCIA").toString());
	editNotas->setText(getRecApunteTesoreria()->getValue("NOTAS").toString());
	scatterTipoApunteTesoreria();
	scatterTercero();
	scatterConcepto();
	scatterProyecto();
/*>>>>>FRMEDITAPUNTETESORERIA_SCATTER*/
	if( isInserting() ) {
		if( editNumero->toInt() == 0 ) {
			editNumero->setText( getRecord()->selectNextInt( "NUMERO" ) );
		}
		if( editFecha->toDate().isNull() ) {
			editFecha->setText( Date::currentDate() );
		}
	}
	validateFields(comboTablaTerceros, 0, 0);
	validateFields(comboTablaConceptos, 0, 0);
}

void FrmEditApunteTesoreria::gatherFields()
{
/*<<<<<FRMEDITAPUNTETESORERIA_GATHER*/
	getRecApunteTesoreria()->setValue( "NUMERO", editNumero->toInt());
	getRecApunteTesoreria()->setValue( "AUTOMATICO", checkAutomatico->isChecked());
	getRecApunteTesoreria()->setValue( "FECHA", editFecha->toDate());
	getRecApunteTesoreria()->setValue( "TIPOAPUNTETESORERIA_ID", getRecTipoApunteTesoreria()->getRecordID() );
	getRecApunteTesoreria()->setValue( "TABLATERCEROS", comboTablaTerceros->toString());
	getRecApunteTesoreria()->setValue( "TERCERO_ID", getRecTercero()->getRecordID() );
	getRecApunteTesoreria()->setValue( "TERCERO", editTercero->toString());
	getRecApunteTesoreria()->setValue( "TABLACONCEPTOS", comboTablaConceptos->toString());
	getRecApunteTesoreria()->setValue( "CONCEPTO_ID", getRecConcepto()->getRecordID() );
	getRecApunteTesoreria()->setValue( "CONCEPTO", editConcepto->toString());
	getRecApunteTesoreria()->setValue( "IMPORTE", editImporte->toMoney());
	getRecApunteTesoreria()->setValue( "REFERENCIA", editReferencia->toString());
	getRecApunteTesoreria()->setValue( "PROYECTO_ID", getRecProyecto()->getRecordID() );
	getRecApunteTesoreria()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITAPUNTETESORERIA_GATHER*/
}


void FrmEditApunteTesoreria::specialControlKeyPressed(QWidget *sender, char key)
{
/*<<<<<FRMEDITAPUNTETESORERIA_SPECIALACTION*/
	mControlKeyPressed = key;
	FrmEditRecMaster::specialControlKeyPressed(sender,key); // calls the behaviors
	if( sender == editTipoApunteTesoreriaCodigo )
		pushTipoApunteTesoreriaCodigo_clicked();
	if( sender == editTerceroCodigo )
		pushTerceroCodigo_clicked();
	if( sender == editConceptoCodigo )
		pushConceptoCodigo_clicked();
	if( sender == editProyectoCodigo )
		pushProyectoCodigo_clicked();
	mControlKeyPressed = '\0';
/*>>>>>FRMEDITAPUNTETESORERIA_SPECIALACTION*/
}

void FrmEditApunteTesoreria::scatterTipoApunteTesoreria()
{
/*<<<<<FRMEDITAPUNTETESORERIA_SCATTER_TIPOAPUNTETESORERIA*/
	editTipoApunteTesoreriaCodigo->setText( getRecTipoApunteTesoreria()->getValue("CODIGO") );
	editTipoApunteTesoreriaNombre->setText( getRecTipoApunteTesoreria()->getValue("NOMBRE") );
/*>>>>>FRMEDITAPUNTETESORERIA_SCATTER_TIPOAPUNTETESORERIA*/
}
void FrmEditApunteTesoreria::pushTipoApunteTesoreriaCodigo_clicked()
{
/*<<<<<FRMEDITAPUNTETESORERIA_PUSH_TIPOAPUNTETESORERIA_CODIGO_CLICKED*/
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
					DBAPP->getMainWindow()->createClient( DBAPP->createEditForm(this, getRecTipoApunteTesoreria(),
						0, DataTable::selecting, dbApplication::simpleEdition, this ) );
				}
			}
			break;
		case 'A':
			{
				RecTipoApunteTesoreria *tmprec = static_cast<RecTipoApunteTesoreria *>(DBAPP->createRecord( "TipoApunteTesoreria" ));
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
/*>>>>>FRMEDITAPUNTETESORERIA_PUSH_TIPOAPUNTETESORERIA_CODIGO_CLICKED*/
}


void FrmEditApunteTesoreria::scatterProyecto()
{
/*<<<<<FRMEDITAPUNTETESORERIA_SCATTER_PROYECTO*/
	editProyectoCodigo->setText( getRecProyecto()->getValue("CODIGO") );
	editProyectoNombre->setText( getRecProyecto()->getValue("NOMBRE") );
/*>>>>>FRMEDITAPUNTETESORERIA_SCATTER_PROYECTO*/
}
void FrmEditApunteTesoreria::pushProyectoCodigo_clicked()
{
/*<<<<<FRMEDITAPUNTETESORERIA_PUSH_PROYECTO_CODIGO_CLICKED*/
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
/*>>>>>FRMEDITAPUNTETESORERIA_PUSH_PROYECTO_CODIGO_CLICKED*/
}


void FrmEditApunteTesoreria::scatterTercero()
{
#if 0
/*<<<<<FRMEDITAPUNTETESORERIA_SCATTER_TERCERO*/
	editTerceroCodigo->setText( getRecTercero()->getValue("CODIGO") );
	editTerceroNombre->setText( getRecTercero()->getValue("NOMBRE") );
/*>>>>>FRMEDITAPUNTETESORERIA_SCATTER_TERCERO*/
#endif
	if( getRecTercero() ) {
		editTerceroCodigo->setText( getRecTercero()->getValue( getRecTercero()->getTableDefinition()->getCodeField() ) );
		editTerceroNombre->setText( getRecTercero()->getValue( getRecTercero()->getTableDefinition()->getDescField() ) );
		if (editTercero->toString().isEmpty() ) 
			editTercero->setText( getRecTercero()->getValue( getRecTercero()->getTableDefinition()->getDescField() ) );
	}
}

void FrmEditApunteTesoreria::pushTerceroCodigo_clicked()
{
/*<<<<<FRMEDITAPUNTETESORERIA_PUSH_TERCERO_CODIGO_CLICKED*/
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
					DBAPP->getMainWindow()->createClient( DBAPP->createEditForm(this, getRecTercero(),
						0, DataTable::selecting, dbApplication::simpleEdition, this ) );
				}
			}
			break;
		case 'A':
			{
				RecTercero *tmprec = static_cast<RecTercero *>(DBAPP->createRecord( "Tercero" ));
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
/*>>>>>FRMEDITAPUNTETESORERIA_PUSH_TERCERO_CODIGO_CLICKED*/
}

void FrmEditApunteTesoreria::scatterConcepto()
{
#if 0	
/*<<<<<FRMEDITAPUNTETESORERIA_SCATTER_CONCEPTO*/
	editConceptoCodigo->setText( getRecConcepto()->getValue("CODIGO") );
	editConceptoNombre->setText( getRecConcepto()->getValue("NOMBRE") );
/*>>>>>FRMEDITAPUNTETESORERIA_SCATTER_CONCEPTO*/
#endif
	if( getRecConcepto() ) {
		_GONG_DEBUG_PRINT(0, getRecConcepto()->toString(TOSTRING_DEBUG_COMPLETE));
		editConceptoCodigo->setText( getRecConcepto()->getValue( getRecConcepto()->getTableDefinition()->getCodeField() ) );
		editConceptoNombre->setText( getRecConcepto()->getValue( getRecConcepto()->getTableDefinition()->getDescField() ) );
		if (editConcepto->toString().isEmpty() ) 
			editConcepto->setText( getRecConcepto()->getValue( getRecConcepto()->getTableDefinition()->getDescField() ) );
	}
}

void FrmEditApunteTesoreria::pushConceptoCodigo_clicked()
{
/*<<<<<FRMEDITAPUNTETESORERIA_PUSH_CONCEPTO_CODIGO_CLICKED*/
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
					DBAPP->getMainWindow()->createClient( DBAPP->createEditForm(this, getRecConcepto(),
						0, DataTable::selecting, dbApplication::simpleEdition, this ) );
				}
			}
			break;
		case 'A':
			{
				RecConcepto *tmprec = static_cast<RecConcepto *>(DBAPP->createRecord( "Concepto" ));
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
/*>>>>>FRMEDITAPUNTETESORERIA_PUSH_CONCEPTO_CODIGO_CLICKED*/
}


void FrmEditApunteTesoreria::validateFields(QWidget *sender, bool *isvalid, ValidResult *ir)
{
/*<<<<<FRMEDITAPUNTETESORERIA_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	if( !sender && !pRecord->isValid( ValidResult::editing, validresult ) )
			*isvalid = false;
	if( focusWidget() != pushTipoApunteTesoreriaCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editTipoApunteTesoreriaCodigo, editTipoApunteTesoreriaNombre,
		getRecTipoApunteTesoreria(), "CODIGO", "NOMBRE", Xtring::null) )
		scatterTipoApunteTesoreria();
	if( focusWidget() != pushTerceroCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editTerceroCodigo, editTerceroNombre,
		getRecTercero(), "CODIGO", "NOMBRE", Xtring::null) )
		scatterTercero();
	if( focusWidget() != pushConceptoCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editConceptoCodigo, editConceptoNombre,
		getRecConcepto(), "CODIGO", "NOMBRE", Xtring::null) )
		scatterConcepto();
	if( focusWidget() != pushProyectoCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editProyectoCodigo, editProyectoNombre,
		getRecProyecto(), "CODIGO", "NOMBRE", Xtring::null) )
		scatterProyecto();
/*>>>>>FRMEDITAPUNTETESORERIA_VALIDATE*/
// {capel} comentar los validSeekCode de tercero y concepto
	if( getRecTercero() && focusWidget() != pushTerceroCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editTerceroCodigo, editTerceroNombre, getRecTercero(), 
			getRecTercero()->getTableDefinition()->getCodeField(), 
			getRecTercero()->getTableDefinition()->getDescField(), Xtring::null) )
		scatterTercero();
	if( getRecConcepto() && focusWidget() != pushConceptoCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editConceptoCodigo, editConceptoNombre, getRecConcepto(), 
			getRecConcepto()->getTableDefinition()->getCodeField(), 
			getRecConcepto()->getTableDefinition()->getDescField(), Xtring::null) )
		scatterConcepto();
	if( sender == comboTablaTerceros ) {
		getRecord()->setValue("TABLATERCEROS", comboTablaTerceros->getCurrentItemValue() );
		if( comboTablaTerceros->getCurrentItemValue().isEmpty() ) {
			searchTerceroCodigo->setEnabled( false );
			editTercero->setEnabled( true );
		} else {
			searchTerceroCodigo->setEnabled( true );
			editTercero->setEnabled( false );
			searchTerceroCodigo->getButton()->setText( comboTablaTerceros->currentText() );
		}
	}
	if( sender == comboTablaConceptos ) {
		getRecord()->setValue("TABLACONCEPTOS", comboTablaConceptos->getCurrentItemValue() );
		if( comboTablaConceptos->getCurrentItemValue().isEmpty() ) {
			searchConceptoCodigo->setEnabled( false );
			editConcepto->setEnabled( true );
		} else {
			searchConceptoCodigo->setEnabled( true );
			editConcepto->setEnabled( false );
			searchConceptoCodigo->getButton()->setText( comboTablaConceptos->currentText() );
		}
	}
	if( sender == editTipoApunteTesoreriaCodigo && editTipoApunteTesoreriaCodigo->isJustEdited() ) {
		changeTipoApunte();
	}
	if( !ir ) {
		showValidMessages(isvalid, *validresult, sender);
		delete validresult;
	}
}


void FrmEditApunteTesoreria::changeTipoApunte()
{
	QWidget *control;
	XtringList camposapedir;
	camposapedir << "FECHA" << "REFERENCIA" << "IMPORTE";
	for( XtringList::const_iterator it = camposapedir.begin(); it != camposapedir.end(); ++it ) {
		if( !control ) {
			_GONG_DEBUG_WARNING("El control " + *it + " no se ha encontrado");
			continue;
		}
		Variant value( getRecTipoApunteTesoreria()->getValue(*it) );
		switch( getRecTipoApunteTesoreria()->getValue("PEDIR" + *it).toInt() ) {
			case FldPedirCampo::Pedir:
				control = findControl(*it);
				enableEditControl(control, true);
				break;
			case FldPedirCampo::FijarValor:
				control = fixControl(*it, value);
				enableEditControl(control, false);
				break;
			case FldPedirCampo::SugerirValor:
				control = fixControl(*it, value);
				enableEditControl(control, true);
				break;
			case FldPedirCampo::DejarVacio:
				control = fixControl(*it, Variant());
				enableEditControl(control, false);
				break;
			case FldPedirCampo::Elegir:
				break;
			case FldPedirCampo::Buscar:
				break;
			case FldPedirCampo::FijarOcultar:
				control = fixControl(*it, value);
				control->hide();
				break;
			case FldPedirCampo::DejarVacioOcultar:
				control = fixControl(*it, Variant());
				control->hide();
				break;
		}
	}
}


/*<<<<<FRMEDITAPUNTETESORERIA_FIN*/
} // namespace tesoreria
} // namespace gong
/*>>>>>FRMEDITAPUNTETESORERIA_FIN*/

