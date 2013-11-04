/*<<<<<COPYLEFT*/
/** @file factufrmeditfamilia.cpp Fichero de edición de familias de artículos
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
// COPYLEFT Fichero de edición de familias de artículos
// FIELD Codigo int - codigo
// FIELD Nombre string - codigo
// FIELD TipoIVA_ID Reference(empresa::TipoIVA,Codigo,Nombre) - tipoiva
// FIELD SubcuentaCompras string - subcuentas MODULE_INCLUDED(Contab)
// FIELD SubcuentaVentas string - subcuentas MODULE_INCLUDED(Contab)
// FIELD Notas text - notas
// TYPE FrmEditRecMaster factu::Familia ValidCodeAndDesc IncCode CodeNotFound
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITFAMILIA_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "factufrmeditfamilia.h"
/*>>>>>FRMEDITFAMILIA_INCLUDES*/
#include "factumodule.h"

namespace gong {
namespace factu {

/*<<<<<FRMEDITFAMILIA_CONSTRUCTOR*/
FrmEditFamilia::FrmEditFamilia(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditFamilia" );
/*>>>>>FRMEDITFAMILIA_CONSTRUCTOR*/
    /*<<<<<FRMEDITFAMILIA_INIT_CONTROLS*/
	QHBoxLayout *codigoLayout = new QHBoxLayout(0, 0, 6, "codigoLayout");
	QHBoxLayout *tipoivaLayout = new QHBoxLayout(0, 0, 6, "tipoivaLayout");
	QHBoxLayout *subcuentasLayout = new QHBoxLayout(0, 0, 6, "subcuentasLayout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
	editCodigo = addEditField( pControlsFrame, "FAMILIA", "CODIGO", codigoLayout );
	editNombre = addEditField( pControlsFrame, "FAMILIA", "NOMBRE", codigoLayout );

	searchTipoIVACodigo = addSearchField( pControlsFrame, "TIPOIVA_ID", "TIPOIVA", "CODIGO", "NOMBRE", tipoivaLayout );
	pushTipoIVACodigo = searchTipoIVACodigo->getButton();
	connect( pushTipoIVACodigo, SIGNAL( clicked() ), this, SLOT( pushTipoIVACodigo_clicked() ) );
	editTipoIVACodigo = searchTipoIVACodigo->getEditCode();
	editTipoIVANombre = searchTipoIVACodigo->getEditDesc();
#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	editSubcuentaCompras = addEditField( pControlsFrame, "FAMILIA", "SUBCUENTACOMPRAS", subcuentasLayout );
}
#endif
#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	editSubcuentaVentas = addEditField( pControlsFrame, "FAMILIA", "SUBCUENTAVENTAS", subcuentasLayout );
}
#endif
	editNotas = addTextField( pControlsFrame, "FAMILIA", "NOTAS", notasLayout );
	pControlsLayout->addLayout( codigoLayout );
	pControlsLayout->addLayout( tipoivaLayout );
	pControlsLayout->addLayout( subcuentasLayout );
	pControlsLayout->addLayout( notasLayout );
/*>>>>>FRMEDITFAMILIA_INIT_CONTROLS*/
}

void FrmEditFamilia::scatterFields()
{
    /*<<<<<FRMEDITFAMILIA_SCATTER*/
	editCodigo->setText(getRecFamilia()->getValue("CODIGO").toInt());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editCodigo;
	editNombre->setText(getRecFamilia()->getValue("NOMBRE").toString());
#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	editSubcuentaCompras->setText(getRecFamilia()->getValue("SUBCUENTACOMPRAS").toString());
}
#endif
#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	editSubcuentaVentas->setText(getRecFamilia()->getValue("SUBCUENTAVENTAS").toString());
}
#endif
	editNotas->setText(getRecFamilia()->getValue("NOTAS").toString());
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
	scatterTipoIVA();
/*>>>>>FRMEDITFAMILIA_SCATTER*/

}

void FrmEditFamilia::gatherFields()
{
    /*<<<<<FRMEDITFAMILIA_GATHER*/
	getRecFamilia()->setValue( "CODIGO", editCodigo->toInt());
	getRecFamilia()->setValue( "NOMBRE", editNombre->toString());
	getRecFamilia()->setValue( "TIPOIVA_ID", getRecTipoIVA()->getRecordID() );
#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	getRecFamilia()->setValue( "SUBCUENTACOMPRAS", editSubcuentaCompras->toString());
}
#endif
#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	getRecFamilia()->setValue( "SUBCUENTAVENTAS", editSubcuentaVentas->toString());
}
#endif
	getRecFamilia()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITFAMILIA_GATHER*/
}

void FrmEditFamilia::validateFields( QWidget *sender, bool *isvalid, ValidResult *ir )
{
    /*<<<<<FRMEDITFAMILIA_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	if( !sender && !pRecord->isValid( ValidResult::editing, validresult ) )
			*isvalid = false;
	if( focusWidget() != pushTipoIVACodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editTipoIVACodigo, editTipoIVANombre,
		getRecTipoIVA(), "CODIGO", "NOMBRE", Xtring::null) )
		scatterTipoIVA();
	if( !validCodeAndDesc( sender, *validresult, editCodigo, editNombre, "codigo", "nombre" ) )
		if( !sender )
			*isvalid = false;
/*>>>>>FRMEDITFAMILIA_VALIDATE*/
    if ( !ir ) {
        showValidMessages( isvalid, *validresult, sender );
        delete validresult;
    }
}

void FrmEditFamilia::scatterTipoIVA()
{
    /*<<<<<FRMEDITFAMILIA_SCATTER_TIPOIVA*/
	editTipoIVACodigo->setText( getRecTipoIVA()->getValue("CODIGO") );
	editTipoIVANombre->setText( getRecTipoIVA()->getValue("NOMBRE") );
/*>>>>>FRMEDITFAMILIA_SCATTER_TIPOIVA*/
    editTipoIVANombre->setText( getRecTipoIVA()->toString( TOSTRING_CODE_AND_DESC ) );
}

void FrmEditFamilia::pushTipoIVACodigo_clicked()
{
    /*<<<<<FRMEDITFAMILIA_PUSH_TIPOIVA_CODIGO_CLICKED*/
	char action = mControlKeyPressed;
	if( !isEditing() || searchTipoIVACodigo->mustBeReadOnly() )
		action = 'E';
	switch( action ) {
		case 'F':
		case '\0':
			editTipoIVACodigo->setJustEdited( false );
			editTipoIVACodigo->setCancelling();
			if( DBAPP->choose(this, getRecTipoIVA(), 0, dbApplication::editNone, this ) ) {
				setEdited(true);
				scatterTipoIVA();
				editTipoIVACodigo->setJustEdited( true );
				editTipoIVACodigo->setFocus();
			}
			break;
		case 'M':
			{
				if( getRecTipoIVA()->getRecordID() ) {
					editTipoIVACodigo->setJustEdited( false );
					if( DBAPP->editRecord(this,
							getRecTipoIVA(), 0, DataTable::updating,
							dbApplication::simpleEdition, this ) ) {
						editTipoIVACodigo->setJustEdited( true );
						scatterTipoIVA();
					}
					editTipoIVACodigo->setFocus();
				}
			}
			break;
		case 'E':
			{
				if( getRecTipoIVA()->getRecordID() != 0 ) {
					editTipoIVACodigo->setJustEdited( false );
					DBAPP->getMainWindow()->createClient( DBAPP->createEditForm(this, getRecTipoIVA(),
						0, DataTable::selecting, dbApplication::simpleEdition, this ) );
				}
			}
			break;
		case 'A':
			{
				empresa::RecTipoIVA *tmprec = static_cast<empresa::RecTipoIVA *>(DBAPP->createRecord( "TipoIVA" ));
				editTipoIVACodigo->setJustEdited( false );
				tmprec->clear( true ); // set default values
				DBAPP->setCodeNotFound( editTipoIVACodigo->toString() );
				if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
					dbApplication::simpleEdition, this ) ) {
					editTipoIVACodigo->setJustEdited( true );
					getRecTipoIVA()->copyRecord( tmprec );
					scatterTipoIVA();
				}
				editTipoIVACodigo->setFocus();
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITFAMILIA_PUSH_TIPOIVA_CODIGO_CLICKED*/
}

void FrmEditFamilia::specialControlKeyPressed( QWidget *sender, char key )
{
    /*<<<<<FRMEDITFAMILIA_SPECIALACTION*/
	mControlKeyPressed = key;
	FrmEditRecMaster::specialControlKeyPressed(sender,key); // calls the behaviors
	if( sender == editTipoIVACodigo )
		pushTipoIVACodigo_clicked();
	mControlKeyPressed = '\0';
/*>>>>>FRMEDITFAMILIA_SPECIALACTION*/
}

/*<<<<<FRMEDITFAMILIA_FIN*/
} // namespace factu
} // namespace gong
/*>>>>>FRMEDITFAMILIA_FIN*/


