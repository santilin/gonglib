/*<<<<<COPYLEFT*/
/** @file factufrmedittipodoc.cpp Fichero de edición de tipos de facturas
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */
/*>>>>>COPYLEFT*/
/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de tipos de facturas
// FIELD Codigo int - codigo
// FIELD Nombre string - codigo
// FIELD Formato string - left2
// FIELD Serie string - left1
// FIELD IVADetallado comboint - left1
// FIELD FormaPago_ID Reference(pagos::FormaPago,Codigo,Nombre,dbApplication::InsertIfNotFound)
// FIELD Agente_ID Reference(Agente,Codigo,RazonSocial,dbApplication::InsertIfNotFound)
// FIELD Proyecto_ID Reference(empresa::Proyecto,Codigo,Nombre,dbApplication::InsertIfNotFound) - ndoclayout
// FIELD ActualizaStocks bool - bools
// FIELD NoGeneraASiento bool - bools MODULE_INCLUDED(Contab)
// FIELD NoFacturable bool - bools
// TYPE FrmEditRecMaster factu::TipoDoc ValidCodeAndDesc IncCode CodeNotFound
/*>>>>>MODULE_INFO*/

#include "config.h"
/*<<<<<FRMEDITTIPODOC_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "factufrmedittipodoc.h"
/*>>>>>FRMEDITTIPODOC_INCLUDES*/
#include "factumodule.h"

namespace gong {
namespace factu {

/*<<<<<FRMEDITTIPODOC_CONSTRUCTOR*/
FrmEditTipoDoc::FrmEditTipoDoc(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditTipoDoc" );
/*>>>>>FRMEDITTIPODOC_CONSTRUCTOR*/
    /*<<<<<FRMEDITTIPODOC_INIT_CONTROLS*/
	QHBoxLayout *codigoLayout = new QHBoxLayout(0, 0, 6, "codigoLayout");
	QHBoxLayout *left2Layout = new QHBoxLayout(0, 0, 6, "left2Layout");
	QHBoxLayout *left1Layout = new QHBoxLayout(0, 0, 6, "left1Layout");
	QHBoxLayout *formapago_idLayout = new QHBoxLayout(0, 0, 6, "formapago_idLayout");
	QHBoxLayout *agente_idLayout = new QHBoxLayout(0, 0, 6, "agente_idLayout");
	QHBoxLayout *ndoclayoutLayout = new QHBoxLayout(0, 0, 6, "ndoclayoutLayout");
	QHBoxLayout *boolsLayout = new QHBoxLayout(0, 0, 6, "boolsLayout");
	editCodigo = addEditField( pControlsFrame, "TIPODOC", "CODIGO", codigoLayout );
	editNombre = addEditField( pControlsFrame, "TIPODOC", "NOMBRE", codigoLayout );
	editFormato = addEditField( pControlsFrame, "TIPODOC", "FORMATO", left2Layout );
	editSerie = addEditField( pControlsFrame, "TIPODOC", "SERIE", left1Layout );
	comboIVADetallado = addComboIntField( pControlsFrame, "TIPODOC", "IVADETALLADO", left1Layout );

	searchFormaPagoCodigo = addSearchField( pControlsFrame, "FORMAPAGO_ID", "FORMAPAGO", "CODIGO", "NOMBRE", formapago_idLayout );
	pushFormaPagoCodigo = searchFormaPagoCodigo->getButton();
	connect( pushFormaPagoCodigo, SIGNAL( clicked() ), this, SLOT( pushFormaPagoCodigo_clicked() ) );
	editFormaPagoCodigo = searchFormaPagoCodigo->getEditCode();
	editFormaPagoNombre = searchFormaPagoCodigo->getEditDesc();

	searchAgenteCodigo = addSearchField( pControlsFrame, "AGENTE_ID", "AGENTE", "CODIGO", "RAZONSOCIAL", agente_idLayout );
	pushAgenteCodigo = searchAgenteCodigo->getButton();
	connect( pushAgenteCodigo, SIGNAL( clicked() ), this, SLOT( pushAgenteCodigo_clicked() ) );
	editAgenteCodigo = searchAgenteCodigo->getEditCode();
	editAgenteRazonSocial = searchAgenteCodigo->getEditDesc();

	searchProyectoCodigo = addSearchField( pControlsFrame, "PROYECTO_ID", "PROYECTO", "CODIGO", "NOMBRE", ndoclayoutLayout );
	pushProyectoCodigo = searchProyectoCodigo->getButton();
	connect( pushProyectoCodigo, SIGNAL( clicked() ), this, SLOT( pushProyectoCodigo_clicked() ) );
	editProyectoCodigo = searchProyectoCodigo->getEditCode();
	editProyectoNombre = searchProyectoCodigo->getEditDesc();
	checkActualizaStocks = addCheckField( pControlsFrame, "TIPODOC", "ACTUALIZASTOCKS", boolsLayout );
#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	checkNoGeneraASiento = addCheckField( pControlsFrame, "TIPODOC", "NOGENERAASIENTO", boolsLayout );
}
#endif
	checkNoFacturable = addCheckField( pControlsFrame, "TIPODOC", "NOFACTURABLE", boolsLayout );
	pControlsLayout->addLayout( codigoLayout );
	pControlsLayout->addLayout( left2Layout );
	alignLayout( left2Layout, true );
	pControlsLayout->addLayout( left1Layout );
	alignLayout( left1Layout, true );
	pControlsLayout->addLayout( formapago_idLayout );
	pControlsLayout->addLayout( agente_idLayout );
	pControlsLayout->addLayout( ndoclayoutLayout );
	pControlsLayout->addLayout( boolsLayout );
/*>>>>>FRMEDITTIPODOC_INIT_CONTROLS*/
}

void FrmEditTipoDoc::scatterFields()
{
    /*<<<<<FRMEDITTIPODOC_SCATTER*/
	editCodigo->setText(getRecTipoDoc()->getValue("CODIGO").toInt());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editCodigo;
	editNombre->setText(getRecTipoDoc()->getValue("NOMBRE").toString());
	editFormato->setText(getRecTipoDoc()->getValue("FORMATO").toString());
	editSerie->setText(getRecTipoDoc()->getValue("SERIE").toString());
	comboIVADetallado->setCurrentItemByValue(getRecTipoDoc()->getValue("IVADETALLADO").toInt());
	checkActualizaStocks->setChecked(getRecTipoDoc()->getValue("ACTUALIZASTOCKS").toBool());
#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	checkNoGeneraASiento->setChecked(getRecTipoDoc()->getValue("NOGENERAASIENTO").toBool());
}
#endif
	checkNoFacturable->setChecked(getRecTipoDoc()->getValue("NOFACTURABLE").toBool());
	if( isInserting() && editCodigo->toInt() == 0 ) {
		editCodigo->setText( getRecord()->selectNextInt( "CODIGO" ) );
	}
	if( isInserting() && !isDuplicating() && !DBAPP->codeNotFound().isEmpty() ) {
		if( DBAPP->codeNotFound().toInt() != 0 ) {
			editCodigo->setText( DBAPP->codeNotFound() );
			editCodigo->setJustEdited( true );
		} else {
		editNombre->setText( DBAPP->codeNotFound() );
		editNombre->setJustEdited( true );
		}
	}
	scatterFormaPago();
	scatterAgente();
	scatterProyecto();
/*>>>>>FRMEDITTIPODOC_SCATTER*/
}

void FrmEditTipoDoc::gatherFields()
{
    /*<<<<<FRMEDITTIPODOC_GATHER*/
	getRecTipoDoc()->setValue( "CODIGO", editCodigo->toInt());
	getRecTipoDoc()->setValue( "NOMBRE", editNombre->toString());
	getRecTipoDoc()->setValue( "FORMATO", editFormato->toString());
	getRecTipoDoc()->setValue( "SERIE", editSerie->toString());
	getRecTipoDoc()->setValue( "IVADETALLADO", comboIVADetallado->getCurrentItemValue());
	getRecTipoDoc()->setValue( "FORMAPAGO_ID", getRecFormaPago()->getRecordID() );
	getRecTipoDoc()->setValue( "AGENTE_ID", getRecAgente()->getRecordID() );
	getRecTipoDoc()->setValue( "PROYECTO_ID", getRecProyecto()->getRecordID() );
	getRecTipoDoc()->setValue( "ACTUALIZASTOCKS", checkActualizaStocks->isChecked());
#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	getRecTipoDoc()->setValue( "NOGENERAASIENTO", checkNoGeneraASiento->isChecked());
}
#endif
	getRecTipoDoc()->setValue( "NOFACTURABLE", checkNoFacturable->isChecked());
/*>>>>>FRMEDITTIPODOC_GATHER*/
}

void FrmEditTipoDoc::scatterFormaPago()
{
    /*<<<<<FRMEDITTIPODOC_SCATTER_FORMAPAGO*/
	editFormaPagoCodigo->setText( getRecFormaPago()->getValue("CODIGO") );
	editFormaPagoNombre->setText( getRecFormaPago()->getValue("NOMBRE") );
/*>>>>>FRMEDITTIPODOC_SCATTER_FORMAPAGO*/
}
void FrmEditTipoDoc::pushFormaPagoCodigo_clicked()
{
    /*<<<<<FRMEDITTIPODOC_PUSH_FORMAPAGO_CODIGO_CLICKED*/
	char action = mControlKeyPressed;
	if( !isEditing() || searchFormaPagoCodigo->mustBeReadOnly() )
		action = 'E';
	switch( action ) {
		case 'F':
		case '\0':
			editFormaPagoCodigo->setJustEdited( false );
			editFormaPagoCodigo->setCancelling();
			if( DBAPP->choose(this, getRecFormaPago(), 0, dbApplication::editNone, this ) ) {
				setEdited(true);
				scatterFormaPago();
				editFormaPagoCodigo->setJustEdited( true );
				editFormaPagoCodigo->setFocus();
			}
			break;
		case 'M':
			{
				if( getRecFormaPago()->getRecordID() ) {
					editFormaPagoCodigo->setJustEdited( false );
					if( DBAPP->editRecord(this,
							getRecFormaPago(), 0, DataTable::updating,
							dbApplication::simpleEdition, this ) ) {
						editFormaPagoCodigo->setJustEdited( true );
						scatterFormaPago();
					}
					editFormaPagoCodigo->setFocus();
				}
			}
			break;
		case 'E':
			{
				if( getRecFormaPago()->getRecordID() != 0 ) {
					editFormaPagoCodigo->setJustEdited( false );
					DBAPP->getMainWindow()->createClient( DBAPP->createEditForm(this, getRecFormaPago(),
						0, DataTable::selecting, dbApplication::simpleEdition, this ) );
				}
			}
			break;
		case 'A':
			{
				pagos::RecFormaPago *tmprec = static_cast<pagos::RecFormaPago *>(DBAPP->createRecord( "FormaPago" ));
				editFormaPagoCodigo->setJustEdited( false );
				tmprec->clear( true ); // set default values
				DBAPP->setCodeNotFound( editFormaPagoCodigo->toString() );
				if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
					dbApplication::simpleEdition, this ) ) {
					editFormaPagoCodigo->setJustEdited( true );
					getRecFormaPago()->copyRecord( tmprec );
					scatterFormaPago();
				}
				editFormaPagoCodigo->setFocus();
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITTIPODOC_PUSH_FORMAPAGO_CODIGO_CLICKED*/
}

void FrmEditTipoDoc::scatterAgente()
{
    /*<<<<<FRMEDITTIPODOC_SCATTER_AGENTE*/
	editAgenteCodigo->setText( getRecAgente()->getValue("CODIGO") );
	editAgenteRazonSocial->setText( getRecAgente()->getValue("RAZONSOCIAL") );
/*>>>>>FRMEDITTIPODOC_SCATTER_AGENTE*/
}
void FrmEditTipoDoc::pushAgenteCodigo_clicked()
{
    /*<<<<<FRMEDITTIPODOC_PUSH_AGENTE_CODIGO_CLICKED*/
	char action = mControlKeyPressed;
	if( !isEditing() || searchAgenteCodigo->mustBeReadOnly() )
		action = 'E';
	switch( action ) {
		case 'F':
		case '\0':
			editAgenteCodigo->setJustEdited( false );
			editAgenteCodigo->setCancelling();
			if( DBAPP->choose(this, getRecAgente(), 0, dbApplication::editNone, this ) ) {
				setEdited(true);
				scatterAgente();
				editAgenteCodigo->setJustEdited( true );
				editAgenteCodigo->setFocus();
			}
			break;
		case 'M':
			{
				if( getRecAgente()->getRecordID() ) {
					editAgenteCodigo->setJustEdited( false );
					if( DBAPP->editRecord(this,
							getRecAgente(), 0, DataTable::updating,
							dbApplication::simpleEdition, this ) ) {
						editAgenteCodigo->setJustEdited( true );
						scatterAgente();
					}
					editAgenteCodigo->setFocus();
				}
			}
			break;
		case 'E':
			{
				if( getRecAgente()->getRecordID() != 0 ) {
					editAgenteCodigo->setJustEdited( false );
					DBAPP->getMainWindow()->createClient( DBAPP->createEditForm(this, getRecAgente(),
						0, DataTable::selecting, dbApplication::simpleEdition, this ) );
				}
			}
			break;
		case 'A':
			{
				RecAgente *tmprec = static_cast<RecAgente *>(DBAPP->createRecord( "Agente" ));
				editAgenteCodigo->setJustEdited( false );
				tmprec->clear( true ); // set default values
				DBAPP->setCodeNotFound( editAgenteCodigo->toString() );
				if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
					dbApplication::simpleEdition, this ) ) {
					editAgenteCodigo->setJustEdited( true );
					getRecAgente()->copyRecord( tmprec );
					scatterAgente();
				}
				editAgenteCodigo->setFocus();
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITTIPODOC_PUSH_AGENTE_CODIGO_CLICKED*/
}

void FrmEditTipoDoc::scatterProyecto()
{
#ifdef HAVE_CONTABMODULE
    /*<<<<<FRMEDITTIPODOC_SCATTER_PROYECTO*/
	editProyectoCodigo->setText( getRecProyecto()->getValue("CODIGO") );
	editProyectoNombre->setText( getRecProyecto()->getValue("NOMBRE") );
/*>>>>>FRMEDITTIPODOC_SCATTER_PROYECTO*/
#endif
}

void FrmEditTipoDoc::pushProyectoCodigo_clicked()
{
#ifdef HAVE_CONTABMODULE
    /*<<<<<FRMEDITTIPODOC_PUSH_PROYECTO_CODIGO_CLICKED*/
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
				editProyectoCodigo->setFocus();
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
					editProyectoCodigo->setFocus();
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
				editProyectoCodigo->setFocus();
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITTIPODOC_PUSH_PROYECTO_CODIGO_CLICKED*/
#endif
}

void FrmEditTipoDoc::specialControlKeyPressed(QWidget *sender, char key)
{
    /*<<<<<FRMEDITTIPODOC_SPECIALACTION*/
	mControlKeyPressed = key;
	FrmEditRecMaster::specialControlKeyPressed(sender,key); // calls the behaviors
	if( sender == editFormaPagoCodigo )
		pushFormaPagoCodigo_clicked();
	if( sender == editAgenteCodigo )
		pushAgenteCodigo_clicked();
	if( sender == editProyectoCodigo )
		pushProyectoCodigo_clicked();
	mControlKeyPressed = '\0';
/*>>>>>FRMEDITTIPODOC_SPECIALACTION*/
}


void FrmEditTipoDoc::validateFields(QWidget *sender, bool *isvalid, ValidResult *ir)
{
    /*<<<<<FRMEDITTIPODOC_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	if( !sender && !pRecord->isValid( ValidResult::editing, validresult ) )
			*isvalid = false;
	if( focusWidget() != pushFormaPagoCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editFormaPagoCodigo, editFormaPagoNombre,
		getRecFormaPago(), "CODIGO", "NOMBRE", Xtring::null, dbApplication::SeekCodeFlags( dbApplication::InsertIfNotFound )) )
		scatterFormaPago();
	if( focusWidget() != pushAgenteCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editAgenteCodigo, editAgenteRazonSocial,
		getRecAgente(), "CODIGO", "RAZONSOCIAL", Xtring::null, dbApplication::SeekCodeFlags( dbApplication::InsertIfNotFound )) )
		scatterAgente();
	if( focusWidget() != pushProyectoCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editProyectoCodigo, editProyectoNombre,
		getRecProyecto(), "CODIGO", "NOMBRE", Xtring::null, dbApplication::SeekCodeFlags( dbApplication::InsertIfNotFound )) )
		scatterProyecto();
	if( !validCodeAndDesc( sender, *validresult, editCodigo, editNombre, "codigo", "nombre" ) )
		if( !sender )
			*isvalid = false;
/*>>>>>FRMEDITTIPODOC_VALIDATE*/
    if( !ir ) {
        showValidMessages(isvalid, *validresult, sender);
        delete validresult;
    }
}

/*<<<<<FRMEDITTIPODOC_FIN*/
} // namespace factu
} // namespace gong
/*>>>>>FRMEDITTIPODOC_FIN*/

