// FIELD Tercero_ID Reference(Tercero,Codigo,Nombre) - terceros
// FIELD Concepto_ID Reference(ConceptoTesoreria,Codigo,Nombre) - conceptos
/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de 
// FIELD Numero int - numero
// FIELD Automatico bool - numero
// FIELD TipoApunteTesoreria_ID Reference(TipoApunteTesoreria,Codigo,Nombre) - tipoapunte
// FIELD Cargo bool - fecha
// FIELD Fecha date - fecha
// FIELD Importe Money - fecha
// FIELD CuentaTesoreria_ID Reference(CuentaTesoreria,Codigo,Nombre)
// FIELD TablaTerceros combostring - terceros
// FIELD Tercero string - terceros
// FIELD TablaConceptos combostring - conceptos
// FIELD Concepto string - conceptos
// FIELD Referencia string
// FIELD Proyecto_ID Reference(empresa::Proyecto,Codigo,Nombre) - proyectos if(empresa::ModuleInstance->usaProyectos())
// FIELD Notas text
// FIELD espacio vspacer
// TYPE FrmEditRecMaster tesoreria::ApunteTesoreria 
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITAPUNTETESORERIA_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "tesoreriafrmeditapuntetesoreria.h"
/*>>>>>FRMEDITAPUNTETESORERIA_INCLUDES*/
#include "empresamodule.h"
#include "tesoreriafldpedircampo.h"

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
	searchTerceroCodigo = 0;
	searchConceptoCodigo = 0;
/*<<<<<FRMEDITAPUNTETESORERIA_INIT_CONTROLS*/
	QHBoxLayout *numeroLayout = new QHBoxLayout(0, 0, 6, "numeroLayout");
	QHBoxLayout *tipoapunteLayout = new QHBoxLayout(0, 0, 6, "tipoapunteLayout");
	QHBoxLayout *fechaLayout = new QHBoxLayout(0, 0, 6, "fechaLayout");
	QHBoxLayout *cuentatesoreria_idLayout = new QHBoxLayout(0, 0, 6, "cuentatesoreria_idLayout");
	QHBoxLayout *tercerosLayout = new QHBoxLayout(0, 0, 6, "tercerosLayout");
	QHBoxLayout *conceptosLayout = new QHBoxLayout(0, 0, 6, "conceptosLayout");
	QHBoxLayout *referenciaLayout = new QHBoxLayout(0, 0, 6, "referenciaLayout");
	QHBoxLayout *proyectosLayout = new QHBoxLayout(0, 0, 6, "proyectosLayout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
	QHBoxLayout *espacioLayout = new QHBoxLayout(0, 0, 6, "espacioLayout");
	editNumero = addEditField( pControlsFrame, "APUNTETESORERIA", "NUMERO", numeroLayout );
	checkAutomatico = addCheckField( pControlsFrame, "APUNTETESORERIA", "AUTOMATICO", numeroLayout );

	searchTipoApunteTesoreriaCodigo = addSearchField( pControlsFrame, "TIPOAPUNTETESORERIA_ID", "TIPOAPUNTETESORERIA", "CODIGO", "NOMBRE", tipoapunteLayout );
	pushTipoApunteTesoreriaCodigo = searchTipoApunteTesoreriaCodigo->getButton();
	connect( pushTipoApunteTesoreriaCodigo, SIGNAL( clicked() ), this, SLOT( pushTipoApunteTesoreriaCodigo_clicked() ) );
	editTipoApunteTesoreriaCodigo = searchTipoApunteTesoreriaCodigo->getEditCode();
	editTipoApunteTesoreriaNombre = searchTipoApunteTesoreriaCodigo->getEditDesc();
	checkCargo = addCheckField( pControlsFrame, "APUNTETESORERIA", "CARGO", fechaLayout );
	editFecha = addEditField( pControlsFrame, "APUNTETESORERIA", "FECHA", fechaLayout );
	editImporte = addEditField( pControlsFrame, "APUNTETESORERIA", "IMPORTE", fechaLayout );

	searchCuentaTesoreriaCodigo = addSearchField( pControlsFrame, "CUENTATESORERIA_ID", "CUENTATESORERIA", "CODIGO", "NOMBRE", cuentatesoreria_idLayout );
	pushCuentaTesoreriaCodigo = searchCuentaTesoreriaCodigo->getButton();
	connect( pushCuentaTesoreriaCodigo, SIGNAL( clicked() ), this, SLOT( pushCuentaTesoreriaCodigo_clicked() ) );
	editCuentaTesoreriaCodigo = searchCuentaTesoreriaCodigo->getEditCode();
	editCuentaTesoreriaNombre = searchCuentaTesoreriaCodigo->getEditDesc();
	comboTablaTerceros = addComboXtringField( pControlsFrame, "APUNTETESORERIA", "TABLATERCEROS", tercerosLayout );
	editTercero = addEditField( pControlsFrame, "APUNTETESORERIA", "TERCERO", tercerosLayout );
	comboTablaConceptos = addComboXtringField( pControlsFrame, "APUNTETESORERIA", "TABLACONCEPTOS", conceptosLayout );
	editConcepto = addEditField( pControlsFrame, "APUNTETESORERIA", "CONCEPTO", conceptosLayout );
	editReferencia = addEditField( pControlsFrame, "APUNTETESORERIA", "REFERENCIA", referenciaLayout );

if(empresa::ModuleInstance->usaProyectos()){
	searchProyectoCodigo = addSearchField( pControlsFrame, "PROYECTO_ID", "PROYECTO", "CODIGO", "NOMBRE", proyectosLayout );
	pushProyectoCodigo = searchProyectoCodigo->getButton();
	connect( pushProyectoCodigo, SIGNAL( clicked() ), this, SLOT( pushProyectoCodigo_clicked() ) );
	editProyectoCodigo = searchProyectoCodigo->getEditCode();
	editProyectoNombre = searchProyectoCodigo->getEditDesc();
}
	editNotas = addTextField( pControlsFrame, "APUNTETESORERIA", "NOTAS", notasLayout );
	QSpacerItem* espacio = new QSpacerItem ( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
	espacioLayout->addItem ( espacio);
	pControlsLayout->addLayout( numeroLayout );
	pControlsLayout->addLayout( tipoapunteLayout );
	pControlsLayout->addLayout( fechaLayout );
	pControlsLayout->addLayout( cuentatesoreria_idLayout );
	pControlsLayout->addLayout( tercerosLayout );
	pControlsLayout->addLayout( conceptosLayout );
	pControlsLayout->addLayout( referenciaLayout );
	pControlsLayout->addLayout( proyectosLayout );
	pControlsLayout->addLayout( notasLayout );
	pControlsLayout->addLayout( espacioLayout );
/*>>>>>FRMEDITAPUNTETESORERIA_INIT_CONTROLS*/
	editNumero->setMustBeReadOnly(true);
	pTercerosLayout = tercerosLayout; // Lo necesitamos en el validate
	pConceptosLayout = conceptosLayout; // Lo necesitamos en el validate
}

void FrmEditApunteTesoreria::scatterFields()
{
	if( pFocusWidget == 0 && isEditing() )
		pFocusWidget = editTipoApunteTesoreriaCodigo;
/*<<<<<FRMEDITAPUNTETESORERIA_SCATTER*/
	editNumero->setText(getRecApunteTesoreria()->getValue("NUMERO").toInt());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editNumero;
	checkAutomatico->setChecked(getRecApunteTesoreria()->getValue("AUTOMATICO").toBool());
	checkCargo->setChecked(getRecApunteTesoreria()->getValue("CARGO").toBool());
	editFecha->setText(getRecApunteTesoreria()->getValue("FECHA").toDate());
	editImporte->setText(getRecApunteTesoreria()->getValue("IMPORTE").toMoney());
	comboTablaTerceros->setText(getRecApunteTesoreria()->getValue("TABLATERCEROS").toString());
	editTercero->setText(getRecApunteTesoreria()->getValue("TERCERO").toString());
	comboTablaConceptos->setText(getRecApunteTesoreria()->getValue("TABLACONCEPTOS").toString());
	editConcepto->setText(getRecApunteTesoreria()->getValue("CONCEPTO").toString());
	editReferencia->setText(getRecApunteTesoreria()->getValue("REFERENCIA").toString());
	editNotas->setText(getRecApunteTesoreria()->getValue("NOTAS").toString());
	scatterTipoApunteTesoreria();
	scatterCuentaTesoreria();
if(empresa::ModuleInstance->usaProyectos()){
	scatterProyecto();
}
	_GONG_DEBUG_PRINT(0, pRecord->toString(TOSTRING_DEBUG_COMPLETE));
/*>>>>>FRMEDITAPUNTETESORERIA_SCATTER*/
	bool isvalid = true;
	validateFields(comboTablaTerceros, &isvalid);
	scatterTercero();
	validateFields(comboTablaConceptos, &isvalid);
	scatterConcepto();
	if( isInserting() ) {
		if( editNumero->toInt() == 0 ) {
			editNumero->setText( getRecord()->selectNextInt( "NUMERO" ) );
		}
		if( editFecha->toDate().isNull() ) {
			editFecha->setText( Date::currentDate() );
		}
	}
}

void FrmEditApunteTesoreria::gatherFields()
{
/*<<<<<FRMEDITAPUNTETESORERIA_GATHER*/
	getRecApunteTesoreria()->setValue( "NUMERO", editNumero->toInt());
	getRecApunteTesoreria()->setValue( "AUTOMATICO", checkAutomatico->isChecked());
	getRecApunteTesoreria()->setValue( "TIPOAPUNTETESORERIA_ID", getRecTipoApunteTesoreria()->getRecordID() );
	getRecApunteTesoreria()->setValue( "CARGO", checkCargo->isChecked());
	getRecApunteTesoreria()->setValue( "FECHA", editFecha->toDate());
	getRecApunteTesoreria()->setValue( "IMPORTE", editImporte->toMoney());
	getRecApunteTesoreria()->setValue( "CUENTATESORERIA_ID", getRecCuentaTesoreria()->getRecordID() );
	getRecApunteTesoreria()->setValue( "TABLATERCEROS", comboTablaTerceros->toString());
	getRecApunteTesoreria()->setValue( "TERCERO", editTercero->toString());
	getRecApunteTesoreria()->setValue( "TABLACONCEPTOS", comboTablaConceptos->toString());
	getRecApunteTesoreria()->setValue( "CONCEPTO", editConcepto->toString());
	getRecApunteTesoreria()->setValue( "REFERENCIA", editReferencia->toString());
if(empresa::ModuleInstance->usaProyectos()){
	getRecApunteTesoreria()->setValue( "PROYECTO_ID", getRecProyecto()->getRecordID() );
}
	getRecApunteTesoreria()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITAPUNTETESORERIA_GATHER*/
	getRecApunteTesoreria()->setValue( "TERCERO_ID", getRecTercero() ? getRecTercero()->getRecordID() : 0 );
	getRecApunteTesoreria()->setValue( "CONCEPTO_ID", getRecConcepto() ? getRecConcepto()->getRecordID() : 0 );
}


void FrmEditApunteTesoreria::specialControlKeyPressed(QWidget *sender, char key)
{
/*<<<<<FRMEDITAPUNTETESORERIA_SPECIALACTION*/
	mControlKeyPressed = key;
	FrmEditRecMaster::specialControlKeyPressed(sender,key); // calls the behaviors
	if( sender == editTipoApunteTesoreriaCodigo )
		pushTipoApunteTesoreriaCodigo_clicked();
	if( sender == editCuentaTesoreriaCodigo )
		pushCuentaTesoreriaCodigo_clicked();
if(empresa::ModuleInstance->usaProyectos()){
	if( sender == editProyectoCodigo )
		pushProyectoCodigo_clicked();
}
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
	if( getRecTercero() ) {
		_GONG_DEBUG_PRINT(0, getRecTercero()->toString(TOSTRING_DEBUG_COMPLETE));
		editTerceroCodigo->setText( getRecTercero()->getValue( getRecTercero()->getTableDefinition()->getCodeField() ) );
		editTerceroNombre->setText( getRecTercero()->getValue( getRecTercero()->getTableDefinition()->getDescField() ) );
		if (editTercero->toString().isEmpty() ) 
			editTercero->setText( getRecTercero()->getValue( getRecTercero()->getTableDefinition()->getDescField() ) );
	}
}

void FrmEditApunteTesoreria::pushTerceroCodigo_clicked()
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
					DBAPP->getMainWindow()->createClient( DBAPP->createEditForm(this, getRecTercero(),
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

void FrmEditApunteTesoreria::scatterConcepto()
{
	if( getRecConcepto() ) {
		editConceptoCodigo->setText( getRecConcepto()->getValue( getRecConcepto()->getTableDefinition()->getCodeField() ) );
		editConceptoNombre->setText( getRecConcepto()->getValue( getRecConcepto()->getTableDefinition()->getDescField() ) );
		if (editConcepto->toString().isEmpty() ) 
			editConcepto->setText( getRecConcepto()->getValue( getRecConcepto()->getTableDefinition()->getDescField() ) );
	}
}

void FrmEditApunteTesoreria::pushConceptoCodigo_clicked()
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
					DBAPP->getMainWindow()->createClient( DBAPP->createEditForm(this, getRecConcepto(),
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

void FrmEditApunteTesoreria::scatterCuentaTesoreria()
{
/*<<<<<FRMEDITAPUNTETESORERIA_SCATTER_CUENTATESORERIA*/
	editCuentaTesoreriaCodigo->setText( getRecCuentaTesoreria()->getValue("CODIGO") );
	editCuentaTesoreriaNombre->setText( getRecCuentaTesoreria()->getValue("NOMBRE") );
/*>>>>>FRMEDITAPUNTETESORERIA_SCATTER_CUENTATESORERIA*/
}
void FrmEditApunteTesoreria::pushCuentaTesoreriaCodigo_clicked()
{
/*<<<<<FRMEDITAPUNTETESORERIA_PUSH_CUENTATESORERIA_CODIGO_CLICKED*/
	char action = mControlKeyPressed;
	if( !isEditing() || searchCuentaTesoreriaCodigo->mustBeReadOnly() )
		action = 'E';
	switch( action ) {
		case 'F':
		case '\0':
			editCuentaTesoreriaCodigo->setJustEdited( false );
			editCuentaTesoreriaCodigo->setCancelling();
			if( DBAPP->choose(this, getRecCuentaTesoreria(), 0, dbApplication::editNone, this ) ) {
				setEdited(true);
				scatterCuentaTesoreria();
				editCuentaTesoreriaCodigo->setJustEdited( true );
				setWiseFocus(editCuentaTesoreriaCodigo);
			}
			break;
		case 'M':
			{
				if( getRecCuentaTesoreria()->getRecordID() ) {
					editCuentaTesoreriaCodigo->setJustEdited( false );
					if( DBAPP->editRecord(this,
							getRecCuentaTesoreria(), 0, DataTable::updating,
							dbApplication::simpleEdition, this ) ) {
						editCuentaTesoreriaCodigo->setJustEdited( true );
						scatterCuentaTesoreria();
					}
				setWiseFocus(editCuentaTesoreriaCodigo);
				}
			}
			break;
		case 'E':
			{
				if( getRecCuentaTesoreria()->getRecordID() != 0 ) {
					editCuentaTesoreriaCodigo->setJustEdited( false );
					DBAPP->getMainWindow()->createClient( DBAPP->createEditForm(this, getRecCuentaTesoreria(),
						0, DataTable::selecting, dbApplication::simpleEdition, this ) );
				}
			}
			break;
		case 'A':
			{
				RecCuentaTesoreria *tmprec = static_cast<RecCuentaTesoreria *>(DBAPP->createRecord( "CuentaTesoreria" ));
				editCuentaTesoreriaCodigo->setJustEdited( false );
				tmprec->clear( true ); // set default values
				DBAPP->setCodeNotFound( editCuentaTesoreriaCodigo->toString() );
				if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
					dbApplication::simpleEdition, this ) ) {
					editCuentaTesoreriaCodigo->setJustEdited( true );
					getRecCuentaTesoreria()->copyRecord( tmprec );
					scatterCuentaTesoreria();
				}
				setWiseFocus(editCuentaTesoreriaCodigo);
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITAPUNTETESORERIA_PUSH_CUENTATESORERIA_CODIGO_CLICKED*/
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
	if( focusWidget() != pushCuentaTesoreriaCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editCuentaTesoreriaCodigo, editCuentaTesoreriaNombre,
		getRecCuentaTesoreria(), "CODIGO", "NOMBRE", Xtring::null) )
		scatterCuentaTesoreria();
if(empresa::ModuleInstance->usaProyectos()){
	if( focusWidget() != pushProyectoCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editProyectoCodigo, editProyectoNombre,
		getRecProyecto(), "CODIGO", "NOMBRE", Xtring::null) )
		scatterProyecto();
}
/*>>>>>FRMEDITAPUNTETESORERIA_VALIDATE*/
// {capel} comentar los validSeekCode de tercero y concepto
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
			connect( pushTerceroCodigo, SIGNAL( clicked() ), this, SLOT( pushTerceroCodigo_clicked() ) );
			editTerceroCodigo = searchTerceroCodigo->getEditCode();
			editTerceroNombre = searchTerceroCodigo->getEditDesc();
//			searchTerceroCodigo->setMustBeReadOnly(true);
			
			searchTerceroCodigo->setVisible( true );
			editTercero->setVisible( false );
			searchTerceroCodigo->getButton()->setText( comboTablaTerceros->currentText() );
			setTabOrder( comboTablaTerceros, editTerceroCodigo );
			setTabOrder( editTerceroCodigo, comboTablaConceptos );
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
			connect( pushConceptoCodigo, SIGNAL( clicked() ), this, SLOT( pushConceptoCodigo_clicked() ) );
			editConceptoCodigo = searchConceptoCodigo->getEditCode();
			editConceptoNombre = searchConceptoCodigo->getEditDesc();
//			searchConceptoCodigo->setMustBeReadOnly(true);
			
			searchConceptoCodigo->setVisible( true );
			editConcepto->setVisible( false );
			searchConceptoCodigo->getButton()->setText( comboTablaConceptos->currentText() );
			setTabOrder( comboTablaConceptos, editConceptoCodigo );
			setTabOrder( editConceptoCodigo, editReferencia );
		}
	}
	if( sender == editTipoApunteTesoreriaCodigo && editTipoApunteTesoreriaCodigo->isJustEdited() ) {
		changeTipoApunte();
	}
	if( !sender ) {
		if( comboTablaTerceros->getCurrentItemValue() == "CUENTATESORERIA" 
			&& getRecCuentaTesoreria()->getRecordID() == getRecTercero()->getRecordID() ) {
			validresult->addError( _("La cuenta de tesorería y la del tercero no pueden ser la misma"), "CUENTATESORERIA");
			*isvalid = false;
		}
	}
	if( !ir ) {
		showValidMessages(isvalid, *validresult, sender);
		delete validresult;
	}
}


void FrmEditApunteTesoreria::changeTipoApunte()
{
	QWidget *control = 0;
	XtringList camposapedir;
	camposapedir << "CARGO" << "FECHA" << "REFERENCIA" << "IMPORTE" 
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
			fldname = "push_CUENTATESORERIA_id_CODIGO";
		}
		Variant value( getRecTipoApunteTesoreria()->getValue(*it) );
		switch( pedir ) {
			case FldPedirCampo::Pedir:
				control = findControl(fldname);
				enableEditControl(control, true);
				break;
			case FldPedirCampo::FijarValor:
				control = fixControl(fldname, value);
				enableEditControl(control, false);
				break;
			case FldPedirCampo::SugerirValor:
				control = fixControl(fldname, value);
				enableEditControl(control, true);
				break;
			case FldPedirCampo::DejarVacio:
				control = fixControl(fldname, Variant());
				enableEditControl(control, false);
				break;
			case FldPedirCampo::Buscar:
				///@todo buscar field = value y pasarlo al campo. si es un searchbox, pasar la condición tal cual
				// Sólo si es un searchField
				{
					Xtring sfield, svalue;
					value.toString().splitIn2(sfield, svalue, "=");
				}
				break;
			case FldPedirCampo::FijarOcultar:
				control = fixControl(fldname, value);
				control->hide();
				break;
			case FldPedirCampo::DejarVacioOcultar:
				control = fixControl(fldname, Variant());
				control->hide();
				break;
		}
	}
}


/*<<<<<FRMEDITAPUNTETESORERIA_FIN*/
} // namespace tesoreria
} // namespace gong
/*>>>>>FRMEDITAPUNTETESORERIA_FIN*/


